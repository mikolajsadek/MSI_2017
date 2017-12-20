#include "ExpertSystem.h"
#include "RoughSetExpert.h"

#include <array>
#include <iostream>

namespace {
    constexpr int kTree = 0;
    constexpr int kRoughSet = 1;
}  // namespace

int ChooseKind();

int main() {
    setlocale(LC_ALL, "");
    bool repeat = false;
    try {
        switch (ChooseKind()) {
        case kTree:
            do {
                ExpertSystem TreeExpert{};
                TreeExpert.Run();
                repeat = TreeExpert.ifRepeat();
            } while (repeat);
            break;
        case kRoughSet:
            do {
                RoughSetExpert SetExpert{};
                SetExpert.Prepare();
                SetExpert.Run();
                repeat = SetExpert.ifRepeat();
            } while (repeat);
        }
    }
    catch (...) {
        std::cout << "Invalid input, terminating" << std::endl;
    }
}

int ChooseKind() {
    std::string temp;
    int val = -1;
    while (true) {
        std::cout << "Wybierz rodzaj systemu eksperckiego: oparty o drzewo (0) lub zbiory przybli¿one (1):" << std::endl;
        std::cin >> temp;
        try {
            val = std::stoi(temp);
        }
        catch (...) {
            std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
            continue;
        }
        if ((val != 0) && (val != 1)) {
            continue;
        }
        break;
    }
    return val;
}
