#ifndef SIMULATOR_DECODE_H
#define SIMULATOR_DECODE_H

#include "units/Basics.h"

class Decode final : public Stage {
public:
    bool Run() override;

private:
    /*=== units ===*/
    ControlUnit cu_;
    RegisterFile reg_file_;
    /*=============*/

    /*=== inputs ===*/
    RISCVInstr instr_;
    /*==============*/

    /*=== outputs ===*/
    
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_DECODE_H
