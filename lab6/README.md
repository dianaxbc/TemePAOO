# Lucrarea 6 – PAOO (Effective C++: Items 13–14)

Implementare în C++ care demonstrează principiul RAII (*Resource Acquisition Is Initialization*), utilizarea smart pointerilor și gestionarea copierii pentru clasele care administrează resurse, conform *Effective C++* (Scott Meyers).

## Cerințe acoperite

* **Item 13 – Use objects to manage resources:**
  * Gestionarea automată a ciclului de viață al resurselor prin destructori (RAII).
  * Utilizarea `std::unique_ptr` pentru proprietate exclusivă și transfer explicit prin `std::move`.
  * Utilizarea `std::shared_ptr` pentru proprietate partajată cu contorizare a referințelor (`use_count()`).
* **Item 14 – Think carefully about copying behavior in resource-managing classes:**
  * `UniqueLock`: interzicerea copierii (`= delete`) pentru resurse care nu pot fi partajate.
  * `SharedLock`: partajarea resursei folosind `std::shared_ptr` cu un *custom deleter* (`unlock`), asigurând eliberarea o singură dată la distrugerea ultimului proprietar.

## Structura proiectului

* `Resources.hpp` – Clasele `DatabaseConnection`, `UniqueLock` și `SharedLock`.
* `main.cpp` – Scenarii de testare pentru `unique_ptr`, `shared_ptr`, interzicerea copierii și custom deleter.
* `CMakeLists.txt` – Configurația de build CMake (C++17).

## Compilare și rulare

```bash
g++ -std=c++17 main.cpp -o lab6
./lab6