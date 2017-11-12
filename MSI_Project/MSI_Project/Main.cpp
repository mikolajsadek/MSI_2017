#include "ExpertSystem.h"

#include <array>
#include <iostream>



int main() {
    setlocale(LC_ALL, "");
    bool repeat = false;
    do {
        ExpertSystem Expert{};
        Expert.Run();
        repeat = Expert.ifRepeat();
    } while (repeat);
}
