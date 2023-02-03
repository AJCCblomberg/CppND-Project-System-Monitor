#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <math.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Constructor for process's
Process::Process(int pid) : pid_(pid) {
    // Initialize user ID of the process
    uid_ = LinuxParser::Uid(pid);
    // Initialize user of the process
    user_ = LinuxParser::User(pid);
    // Initialize command line of the process
    cmdline_ = LinuxParser::Command(pid);
};


// DONE: Return this process's ID
int Process::Pid() { 
    return pid_;
}

// DONE: Return this process's CPU utilization
// Added const to the function to make it work for the operator < overload function
float Process::CpuUtilization() const {
    // Get process active jiffies
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);
    
    // Convert process active jiffied to seconds
    long clockTicks = sysconf(_SC_CLK_TCK);
    float activeTime = (activeJiffies / clockTicks);

    // Get process uptime
    long pidUpTime = LinuxParser::UpTime(pid_);

    // Return process CPU utilization
    return activeTime / pidUpTime;
}

// DONE: Return the command that generated this process
string Process::Command() {
    return cmdline_;
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    ram = LinuxParser::Ram(pid_);
    return ram;
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
    return user_;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    upTime = LinuxParser::UpTime(pid_);
    return upTime;
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    if (a.CpuUtilization() < CpuUtilization()) {
        return true;
    } else {
        return false;
    }
}