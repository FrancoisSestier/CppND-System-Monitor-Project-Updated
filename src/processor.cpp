#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> data = LinuxParser::CpuUtilization();
    Processor::PocessorStat current{
        std::stoul(data[LinuxParser::CPUStates::kUser_]),
        std::stoul(data[LinuxParser::CPUStates::kNice_]),
        std::stoul(data[LinuxParser::CPUStates::kSystem_]),
        std::stoul(data[LinuxParser::CPUStates::kIdle_]),
        std::stoul(data[LinuxParser::CPUStates::kIOwait_]),
        std::stoul(data[LinuxParser::CPUStates::kIRQ_]),
        std::stoul(data[LinuxParser::CPUStates::kSoftIRQ_]),
        std::stoul(data[LinuxParser::CPUStates::kSteal_])};

    float deltaTime = current.Total() - previousStat.Total();
    float deltaIldeTime = current.Idle() - previousStat.Idle();

    previousStat = current;

    return (deltaTime - deltaIldeTime) / deltaTime;
}