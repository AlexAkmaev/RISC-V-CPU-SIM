#include <iostream>

#ifndef SIMULATOR_FETCH_H
#define SIMULATOR_FETCH_H

class FetchStage {
public:
    bool Run();

private:
    enum class State : uint32_t {
        PC
    };

    void getInstrFromIMEM();
};

#endif //SIMULATOR_FETCH_H
