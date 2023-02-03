#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <math.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() {
    return cpu_;
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // Clear processes_ vector to prevent multiple instances of the same process
    processes_.clear();
    // Get vector of system process id's
    std::vector<int> pids = LinuxParser::Pids();
    
    // Create processes vector  
    for(int i=0; i<pids.size(); i++) {
        Process process(pids[i]);
        processes_.push_back(process);
    }

    // Sort processes vector (based on CPU utilization, see Process::operator<())
    std::sort(processes_.begin(), processes_.end());

    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
    kernel_ = LinuxParser::Kernel();
    return kernel_;
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
    memUtil_ = LinuxParser::MemoryUtilization();
    return memUtil_;
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
    os_ = LinuxParser::OperatingSystem();
    return os_;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
    procsRunning_ = LinuxParser::RunningProcesses();
    return procsRunning_;
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
    procsTotal_ = LinuxParser::TotalProcesses();
    return procsTotal_;
}

// DONE: Return the number of seconds since the system started running
long System::UpTime() {
    upTime_ = LinuxParser::UpTime();
    return upTime_;
}