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
            std::cout << "Czy chcesz spróbowaæ jeszcze raz? Odpowiedz 0 (nie) lub 1 (tak)." << std::endl;
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
        return val ? true : false;
    }
};
