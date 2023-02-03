#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevActiveJiffies_{0.0};
  float prevJiffies_{0.0};
  float cpuUtilization_{0.0};
};

#endif