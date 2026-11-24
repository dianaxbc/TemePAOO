# Lucrarea 5 – PAOO (Effective C++: Items 10–12)

Implementare în C++ care demonstrează gestionarea corectă a operatorilor de atribuire, a copierii obiectelor și a semanticei de mutare (*move semantics*), conform recomandărilor din *Effective C++* (Scott Meyers)[cite: 1, 12].

## Cerințe acoperite

* **Item 10 – Have assignment operators return a reference to `*this`:** Operatorii `operator=` și `operator+=` returnează `*this` prin referință, permițând atribuiri înlănțuite (`l3 = l2 = l1`)[cite: 2, 12].
* **Item 11 – Handle assignment to self in `operator=`:** Tratarea auto-atribuirii (`l1 = l1`) și garantarea *exception-safety* prin alocarea copiei noii resurse înainte de eliberarea memoriei vechi[cite: 3, 4, 12].
* **Item 12 – Copy all parts of an object:** În clasa derivată (`Laptop`), constructorul de copiere și `operator=` apelează explicit funcțiile corespunzătoare din clasa de bază (`Device(rhs)` și `Device::operator=(rhs)`), asigurând copierea completă a tuturor membrilor[cite: 5, 12].
* **C++11 Move Semantics:** Implementarea move constructorului și a move assignment operatorului (`operator=(Laptop&& rhs) noexcept`) cu eliberarea resursei vechi și resetarea sursei la `nullptr`[cite: 6].

## Structura proiectului

* `Classes.hpp` – Definirea claselor `Device` (bază) și `Laptop` (derivată).
* `main.cpp` – Scenarii de testare pentru atribuiri înlănțuite, auto-atribuire, copiere completă și `std::move`.
* `CMakeLists.txt` – Configurația de build CMake (C++17).

## Compilare și rulare

```bash
g++ -std=c++17 main.cpp -o lab5
./lab5