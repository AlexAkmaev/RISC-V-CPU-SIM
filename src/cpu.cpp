#include "simulator.h"

int main() {
    Simulator cpu = Simulator();
    if (!cpu.Run()) {
        return 1;
    }
    return 0;
}
