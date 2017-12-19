#pragma once

#include <iostream>
#include <string>

class ExpertInterface {
public:
    virtual void Run() = 0;
    bool ifRepeat() {
        std::string temp;
        int val = -1;
        while (true) {
            std::cout << "Czy chcesz spr�bowa� jeszcze raz? Odpowiedz 0 (nie) lub 1 (tak)." << std::endl;
            std::cin >> temp;
            try {
                val = std::stoi(temp);
            }
            catch (...) {
                std::cout << "Niezrozumia�a odpowied�, spr�buj jeszcze raz." << std::endl;
                continue;
            }
            if ((val != 0) && (val != 1)) {
                continue;
            }
            break;
        }
        return val ? true : false;
    }
};
