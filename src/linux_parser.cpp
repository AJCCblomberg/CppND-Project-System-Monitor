#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // From file: "cat /proc/meminfo"
    float memTotal{0}, memFree{0}, buffers{0}, cached{0}, sReclaimable{0}, shmem{0};
    string line, key;
    float value;
    float memUsed{0}, memNonCached{0}, memCached{0};
    

    // Get system memory information from meminfo
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) { // Check if file is open
        while(std::getline(filestream, line)) { // Loop over all lines in the file
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "MemTotal:") {
                memTotal = value;
            } else if (key == "MemFree:") {
                memFree = value;
            } else if (key == "Buffers:") {
                buffers = value;
            } else if (key == "Cached:") {
                cached = value;
            } else if (key == "SReclaimable:") {
                sReclaimable = value;
            } else if (key == "Shmem:") {
                shmem = value;
            }
        }
    }
    // Source: https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
    // Total used memory
    memUsed = memTotal - memFree;
    // Non cached/buffer memory
    memNonCached = memTotal - memFree - (buffers + cached + sReclaimable - shmem);
    // Cached memory
    memCached = cached + sReclaimable - shmem;
    // Return memory utilization
    return (memTotal - memFree)/memTotal; 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  // From file: "cat /proc/uptime"
  string line;
  float time{0.0};
  long upTime{0};

  // Input file stream
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) { // Check if file is open
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> time) {
      upTime = std::round(time);
      return upTime;
    }
  }
  return upTime;
 }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  // Calculate total jiffies for the system (ActiveJiffies + IdleJiffies)
  long jiffies = ActiveJiffies() + IdleJiffies();

  return jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  // From file: "cat /proc/[pid]/stat"
  long activeJiffies{0}, utime{0}, stime{0}, cutime{0}, cstime{0};
  string line, value;
  int i{0};

  // Get process jiffies from stat file
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        i += 1;
        if (i==14) {        // #14 utime in Jiffies
          utime = std::stol(value);
        } else if (i==15) { // #15 stime in Jiffies
          stime = std::stol(value);
        } else if (i==16) { // #16 cutime in Jiffies
          cutime = std::stol(value);
        } else if (i==17) { // #17 cstime in Jiffies
          cstime = std::stol(value);
          // Calculate active jiffies (utime + stime + cutime + cstime)
          activeJiffies = utime + stime + cutime + cstime;
          return activeJiffies;
          break;
        }
      }
    }
  }
  return activeJiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {

  // Get CPU utilization in jiffies
  vector<string> jiffies = CpuUtilization();

  // Calculate active jiffies (user + nice + system + irq + softirq + steal + guest + guestnice)
  long activeJiffies = std::stol(jiffies[kUser_]) + std::stol(jiffies[kNice_]) + \
  std::stol(jiffies[kSystem_]) + std::stol(jiffies[kIRQ_]) + std::stol(jiffies[kSoftIRQ_]) + \
  std::stol(jiffies[kSteal_]) + std::stol(jiffies[kGuest_]) + std::stol(jiffies[kGuestNice_]);

  return activeJiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idleJiffies{0};

  // Get CPU utilization in jiffies
  vector<string> jiffies = CpuUtilization();

  // Calculate idle jiffies (idle + iowait)
  idleJiffies = std::stol(jiffies[CPUStates::kIdle_]) + std::stol(jiffies[CPUStates::kIOwait_]);

  return idleJiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  // From file: "cat /proc/stat"
  vector<string> cpuUtilization{};
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  string line;
  string key;

  // Input file stream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) { // Check if file is open
    while(std::getline(filestream, line)) { // Loop over all lines in the file
      std::istringstream linestream(line);
      while(linestream >> key) {
        if(key == "cpu") {
          linestream >> user;
          cpuUtilization.push_back(user);
          linestream >> nice;
          cpuUtilization.push_back(nice);
          linestream >> system;
          cpuUtilization.push_back(system);
          linestream >> idle;
          cpuUtilization.push_back(idle);
          linestream >> iowait;
          cpuUtilization.push_back(iowait);
          linestream >> irq;
          cpuUtilization.push_back(irq);
          linestream >> softirq;
          cpuUtilization.push_back(softirq);
          linestream >> steal;
          cpuUtilization.push_back(steal);
          linestream >> guest;
          cpuUtilization.push_back(guest);
          linestream >> guestnice;
          cpuUtilization.push_back(guestnice);
          return cpuUtilization;
          break;
        }
      }
    }
  }
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // From file: "cat /proc/stat"
  string line, key;
  int procsTotal;

  // Input file stream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) { // Check if file is open
    while (std::getline(filestream, line)) { // Loop over all lines in the file
      std::istringstream linestream(line);
      while (linestream >> key >> procsTotal) {
        if (key == "processes") {
          return procsTotal;
        }
      }
    }
  }
  return procsTotal;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  // From file: "cat /proc/stat"
  string line;
  string key;
  int procsRunning;

  // Input file stream
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) { // Check if file is open
      while(std::getline(filestream, line)) { // Loop over all lines in the file
          std::istringstream linestream(line);
          while(linestream >> key >> procsRunning) {
              if(key == "procs_running") {
                  return procsRunning;
              }
          }
      }
  }
  return procsRunning;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  // From file: "cat /proc/[pid]/cmdline"
  string line;
  string cmdline;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cmdline;
    return cmdline;
  }
  return cmdline;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  // From file: "cat /proc/[pid]/status"
  string key;
  string ramKb, ramMb;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> ramKb;
      if (key == "VmSize") {
        long ram = std::stol(ramKb)/1000;
        ramMb = std::to_string(ram);
        return ramMb;
      }
    }
  }
  return ramMb;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  // From file: "cat /proc/[pid]/status"
  string key;
  string uid;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid") {
        return uid;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  // From file: "cat /etc/passwd"
  string user, x, uid, UID;
  string line;

  UID = LinuxParser::Uid(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if (uid == UID) {
        string subUser = user.substr(0, 6);
        return subUser;
      }
    }
  }
  return user;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  // From file: "cat /proc/[pid]/stat"
  long startime{0};
  string line, value;
  int i{0};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        i += 1;
        if (i==22) { // #22 starttime in Jiffies
          startime = std::stol(value);
          break;
        }
      }
    }
  }
  // Convert process starttime to seconds
  long clockTicks = sysconf(_SC_CLK_TCK);
  long pidStartTime = startime/clockTicks;

  // Get system uptime
  long systemUpTime = LinuxParser::UpTime();

  // Calculate process uptime             
  long pidUpTime = systemUpTime - pidStartTime;

  return pidUpTime;
}
