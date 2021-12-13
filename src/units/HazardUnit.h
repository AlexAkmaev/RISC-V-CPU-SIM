#ifndef SIMULATOR_HAZARDUNIT_H
#define SIMULATOR_HAZARDUNIT_H

class HazardUnit final {
public:
    struct Flags final {
        bool HU_MEM_RD;
    };
private:
    std::bitset<5> HU_RS1;
    std::bitset<5> HU_RS2;
    PC HU_PC_REDIRECT;
};

#endif //SIMULATOR_HAZARDUNIT_H
