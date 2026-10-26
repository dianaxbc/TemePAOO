#include "Resource.h"
#include <utility>

int main() {
    std::cout << "=== 1. Creare Obiect Initial ===" << std::endl;
    Resource r1(10);

    std::cout << "\n=== 2. Testare Copy Constructor (Deep Copy) ===" << std::endl;
    Resource r2 = r1;

    std::cout << "\n=== 3. Testare Move Constructor ===" << std::endl;
    Resource r3 = std::move(r1);

    std::cout << "\n=== Starea obiectelor ===" << std::endl;
    std::cout << "r1: "; r1.printData();
    std::cout << "r2: "; r2.printData();
    std::cout << "r3: "; r3.printData();

    std::cout << "\n=== 4. Curatare automata (Destructori) ===" << std::endl;
    return 0;
}