#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  auto cpuStat = LinuxParser::CpuUtilization(pid_);
  float totalTime = cpuStat[LinuxParser::PidCPUStats::utime_] +
                    cpuStat[LinuxParser::PidCPUStats::stime_] +
                    cpuStat[LinuxParser::PidCPUStats::cstime_] +
                    cpuStat[LinuxParser::PidCPUStats::cutime_];
  auto cpuTime = totalTime / static_cast<float>(sysconf(_SC_CLK_TCK));
  auto upTime = UpTime();
  if (upTime == 0) {
    upTime = 1;
  };
  auto cpuPercentage = 100.f * (cpuTime / upTime);
  return cpuPercentage;
}

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime() - startTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& other) const {
  return this->CpuUtilization() > other.CpuUtilization();
}

bool Process::operator==(const Process& other) const {
  return this->Pid() == other.Pid();
};
