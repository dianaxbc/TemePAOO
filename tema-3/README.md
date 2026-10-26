# Tema 3 PAOO - Managementul Resurselor în C++

Aplicație C++ realizată pentru tema 3 la disciplina PAOO. Proiectul exemplifică conceptele de bază pentru gestionarea resurselor pe Heap și implementarea regulilor de copiere și mutare (Rule of Five).

## Cerințe abordate

1. **Build cu CMake**: Proiectul este structurat modular (`include/`, `src/`, `build/`) și configurat prin `CMakeLists.txt`.
2. **Încapsulare**: Datele membre ale clasei `Resource` (`m_size`, `m_data`) sunt private, accesul la ele făcându-se prin metode publice.
3. **Lista de inițializare**: În constructor, membrii sunt inițializați folosind constructor member initializer list.
4. **Eliberarea memoriei**: Destructorul clasei eliberează memoria alocată dinamic pe Heap folosind `delete[]`.
5. **Copy Constructor (Deep Copy)**: S-a suprascris copy constructor-ul pentru a aloca o nouă zonă de memorie pe Heap. Fără această suprascriere (la un shallow copy implicit), ambii pointeri ar arăta către aceeași adresă, provocând o eroare de tip *double free* la apelul destructorilor.
6. **Move Constructor**: Implementat pentru a permite preluarea pointerilor din obiecte temporare (`r-value`), evitând alocările inutile și îmbunătățind performanța.


