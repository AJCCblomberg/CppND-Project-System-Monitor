#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <sstream>
#include <iostream>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    // Get active jiffies of system
    long activeJiffies = LinuxParser::ActiveJiffies();
    // Get total jiffies of system
    long jiffies = LinuxParser::Jiffies();

    // Delta jiffies
    float dActiveJiffies = (float) activeJiffies - prevActiveJiffies_;
    float dJiffies = (float) jiffies - prevJiffies_;

    // Store previous value
    prevActiveJiffies_ = activeJiffies;
    prevJiffies_ = jiffies;

    // Calculate CPU utilization
    cpuUtilization_ = dActiveJiffies/dJiffies;

    return cpuUtilization_;
}