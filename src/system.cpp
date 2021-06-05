#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // add new processes
    auto currentPids = LinuxParser::Pids();
    for (auto pid : currentPids) {
        if (!pids_.count(pid) != 0) {
            pids_.insert(pid);
            processes_.emplace_back(Process{pid, LinuxParser::User(pid),
                                            LinuxParser::Command(pid),
                                            LinuxParser::StartTime(pid)});
        }
    }
    // remove dead processes
    for (auto process = processes_.begin(); process != processes_.end();) {
        if (std::find(currentPids.begin(), currentPids.end(), process->Pid()) ==
            currentPids.end()) {
            pids_.erase(process->Pid());
            process = processes_.erase(process);
            continue;
        }
        ++process;
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime();
}  // LinuxParser::UpTime(); }