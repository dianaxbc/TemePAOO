#include "Classes.hpp"

int main() {
    std::cout << "=== 1. Initializare obiecte ===\n";
    Laptop l1("Dell", 80, 16);
    Laptop l2("Lenovo", 50, 8);
    Laptop l3("Asus", 30, 32);

    std::cout << "\n=== 2. Test Item 10: Chain Assignment (l3 = l2 = l1) ===\n";
    l3 = l2 = l1;
    (l3 += 15); // Test += returning *this
    l1.print();
    l2.print();
    l3.print();

    std::cout << "\n=== 3. Test Item 11: Self-Assignment (l1 = l1) ===\n";
    l1 = l1;
    l1.print();

    std::cout << "\n=== 4. Test Item 12: Copy Constructor (copiaza si baza, si derivata) ===\n";
    Laptop l4(l1);
    l4.print();

    std::cout << "\n=== 5. Test Move Assignment (std::move) ===\n";
    Laptop l5("HP", 10, 4);
    l5 = std::move(l4);
    std::cout << "Dupa move, l5: ";
    l5.print();
    std::cout << "Dupa move, l4 (golit): ";
    l4.print();

    return 0;
}