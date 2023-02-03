#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization() const;            // DONE: See src/process.cpp (Added const to make it work for the
                                           //                            operator < overload function)
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

 // DONE: Declare any necessary private members
 private:
    int pid_;
    std::string uid_{};
    std::string user_{};
    std::string cmdline_{};

public:
    float cpu{0.0};
    std::string ram{0};
    long int upTime{0};
    std::vector<std::string> cpuTimes{};
};

#endif