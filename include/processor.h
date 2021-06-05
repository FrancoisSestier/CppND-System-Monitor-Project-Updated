#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <cstdint>

class Processor {
    struct PocessorStat {
        uint64_t Idle() const { return idle + iowait; }
        uint64_t NonIdle() const {
            return user + nice + system + irq + softirq + steal;
        }

        uint64_t Total() const { return Idle() + NonIdle(); }

        uint64_t user;
        uint64_t nice;
        uint64_t system;
        uint64_t idle;
        uint64_t iowait;
        uint64_t irq;
        uint64_t softirq;
        uint64_t steal;
    };

   public:
    float Utilization();  // TODO: See src/processor.cpp

    // TODO: Declare any necessary private members
   private:
    PocessorStat previousStat;
};

#endif