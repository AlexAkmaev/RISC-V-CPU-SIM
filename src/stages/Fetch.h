#ifndef SIMULATOR_FETCH_H
#define SIMULATOR_FETCH_H

#include "units/Basics.h"

class Fetch final : public Stage {
public:
    explicit Fetch(uint32_t instr_count) : imem_(IMEM{instr_count}) {}
    bool Run() override;

private:
    void getInstrFromIMEM();

    /*=== units ===*/
    IMEM imem_;
    /*=============*/

    /*=== inputs ===*/
    PC pc_{0};
    /*==============*/

    /*=== outputs ===*/
    RISCVInstr instr_;
    PC pc_next_{0};
    /*===============*/
};

#endif //SIMULATOR_FETCH_H
