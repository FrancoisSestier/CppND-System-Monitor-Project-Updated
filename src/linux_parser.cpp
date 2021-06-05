#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::string LinuxParser::kPidDirectory(int pid) {
    return "/" + std::to_string(pid) + "/";
}

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
    string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    string line, token;
    float memTotale;
    float memFree;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> token >> memTotale >> token;
        std::getline(stream, line);
        linestream = std::istringstream(line);
        linestream >> token >> memFree >> token;
    }
    return (memTotale - memFree) / memTotale;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string line, key;
    long system, idle;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> system >> idle;
    }
    return system + idle;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    std::vector<string> data;
    string line, token;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        for (int i = 0; i < 10; ++i) {
            std::getline(linestream, token, ' ');
            if (!token.empty() && token != "cpu") {
                data.push_back(token);
            }
        }
    }
    return data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    return helper::ExtractValue<int>(kProcDirectory + kStatFilename,
                                     "processes");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    return helper::ExtractValue<int>(kProcDirectory + kStatFilename,
                                     "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    std::ifstream stream(kProcDirectory + kPidDirectory(pid) +
                         kCmdlineFilename);
    std::string line;
    if (stream.is_open()) {
        std::getline(stream, line);
    };
    return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) {
    auto memUsage =
        helper::ExtractValue<int>(
            kProcDirectory + kPidDirectory(pid) + kStatusFilename, "VmSize:") /
        1024;
    return std::to_string(memUsage);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    return helper::ExtractValue<string>(
        kProcDirectory + kPidDirectory(pid) + kStatusFilename, "Uid:");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    auto requestedUid = Uid(pid);
    std::ifstream stream(kPasswordPath);
    std::string user, x, Uid, line;
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            std::getline(linestream, user, ':');
            std::getline(linestream, x, ':');
            std::getline(linestream, Uid, ':');
            if (Uid == requestedUid) {
                return user;
            }
        }
    }
    return string("unkown");
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::StartTime(int pid) {
    std::vector<uint64_t> data;
    string line, token;
    std::ifstream stream(kProcDirectory + kPidDirectory(pid) + kStatFilename);
    if (stream) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        for (size_t i = 0; i <= pidStartTimeIndex_; ++i) {
            std::getline(linestream, token, ' ');
            if (i == pidStartTimeIndex_) {
                return std::stol(token) / sysconf(_SC_CLK_TCK);
            }
        }
    }
    return 0;
}

template <typename T>
T LinuxParser::helper::ExtractValue(std::string filename,
                                    const std::string& requestedKey) {
    std::ifstream stream(filename);
    std::string key, line;
    T value;
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == requestedKey) {
                    return value;
                }
            }
        }
    }
    return value;
}
std::vector<uint64_t> LinuxParser::CpuUtilization(int pid) {
    std::vector<uint64_t> data;
    string line, token;
    std::ifstream stream(kProcDirectory + kPidDirectory(pid) + kStatFilename);
    if (stream) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        for (size_t i = 0; i <= 22; ++i) {
            std::getline(linestream, token, ' ');
            if (std::find(std::begin(pidCPUStatIndexes_),
                          std::end(pidCPUStatIndexes_),
                          i) != std::end(pidCPUStatIndexes_)) {
                data.push_back(std::stoul(token));
            }
        }
        return data;
    }
    return std::vector<uint64_t>{0, 0, 0, 0};
}
