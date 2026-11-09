# Lucrarea 4 – Effective C++ (Itemii 4, 5 și 6)

**Disciplina:** Paradigme de Programare și Obiecte (PAOO) – UPT, Automatică și Calculatoare
**Autor:** _<nume prenume, grupa>_
**Limbaj / standard:** C++17
**Sursa teoretică:** Scott Meyers, *Effective C++: 55 Specific Ways to Improve Your Programs and Designs*, ediția a 3-a, Addison-Wesley, 2005.

## 1. Obiectiv

Proiectul ilustrează, prin exemple executabile, trei recomandări din *Effective C++*:

| Item | Enunț |
|------|-------|
| 4 | *Make sure that objects are initialized before they're used.* |
| 5 | *Know what functions C++ silently writes and calls.* |
| 6 | *Explicitly disallow the use of compiler-generated functions you do not want.* |

Pentru a face vizibile operațiile efectuate „în spate" de compilator, clasele-martor (`Tracer`, `Verbose`) își afișează fiecare apel de constructor, atribuire sau destructor.

## 2. Structura proiectului

```
lab4/
├── main.cpp    # codul sursă (conține cele trei secțiuni, câte o funcție pentru fiecare item)
└── README.md   # acest document
```

Funcția `main()` apelează pe rând `item4()`, `item5()` și `item6()`.

## 3. Descrierea implementării

### Item 4 – Inițializarea obiectelor înainte de utilizare

**Idee.** Tipurile built-in (`int`, pointeri etc.) nu sunt inițializate automat, iar pentru membrii de tip clasă trebuie distins între *inițializare* și *atribuire*.

**Ce demonstrează codul:**

- **Atribuire în corpul constructorului vs. listă de inițializare** (`EntryBad` vs. `EntryGood`). În varianta greșită se apelează mai întâi constructorul implicit al membrului, apoi `operator=`; în varianta corectă se face un singur apel de constructor.
- **Membri `const` și referințe** (`MustUseInitList`): nu pot fi atribuiți, deci trebuie obligatoriu inițializați în lista de inițializare.
- **Ordinea inițializării:** membrii se inițializează în ordinea *declarării* în clasă, nu în ordinea din lista de inițializare.
- **Ordinea inițializării obiectelor statice non-locale din unități de traducere diferite este nedefinită.** Soluția din carte este înlocuirea lor cu obiecte *locale statice* returnate de o funcție (variantă a șablonului Singleton): funcția `tfs()` construiește obiectul `FileSystem` la prima apelare. În proiect, exemplul este într-un singur fișier; principiul este același și pentru mai multe fișiere sursă.

**Rezultat observat (extras):**

```
EntryBad (atribuire in corp):
    Tracer()                    [constructor implicit]
    Tracer::operator=(string)   [atribuire]
EntryGood (lista de initializare):
    Tracer(string)              [initializare directa]
```

### Item 5 – Funcțiile generate implicit de compilator

**Idee.** Pentru o clasă care nu le declară, compilatorul generează (dacă sunt folosite): constructorul implicit, constructorul de copiere, operatorul de atribuire prin copiere și destructorul. (Începând cu C++11 pot fi generate și operațiile de mutare.)

**Ce demonstrează codul:**

- `Empty` (clasă goală): se pot construi, copia și atribui obiecte, deși nu s-a scris nicio funcție; `sizeof(Empty)` este 1, nu 0.
- `Holder`: conține un membru `Verbose` și nu declară nicio funcție specială. Funcțiile generate de compilator apelează funcțiile corespunzătoare ale membrului (`Verbose()`, `Verbose(const Verbose&)`, `Verbose::operator=`, `~Verbose()`), ceea ce se vede în ieșire.
- `NamedObject<T>`: clasă cu membru **referință** și membru **`const`**. Compilatorul **refuză** să genereze `operator=` (linia `p = s;` este lăsată comentată; decomentată produce eroare de compilare), deoarece o referință nu poate fi reîndreptată, iar un membru `const` nu poate fi modificat. În acest caz operatorul trebuie scris manual.

### Item 6 – Interzicerea explicită a funcțiilor nedorite

**Idee.** Dacă o clasă nu trebuie să poată fi copiată (de exemplu, un obiect cu identitate unică), copierea trebuie blocată explicit, altfel compilatorul o generează.

**Cele două abordări din proiect:**

1. **`= delete` (C++11):** `HomeForSale` declară constructorul de copiere și `operator=` ca `= delete`. Orice încercare de copiere produce eroare de compilare (`use of deleted function`). Este varianta modernă și recomandată.
2. **Clasa de bază `Uncopyable` (varianta din carte, pre-C++11):** funcțiile de copiere sunt `private` și nedefinite; `HomeForSaleOld` moștenește privat din `Uncopyable`, deci copierea ei este și ea blocată la compilare.

Liniile care ar încălca restricția (`HomeForSale h2(h1);`, `HomeForSaleOld o2(o1);`) sunt comentate în `item6()`; pot fi decomentate pentru a observa erorile de compilare.

## 4. Compilare și rulare

Cerințe: un compilator C++ compatibil cu C++17 (testat cu `g++ 11.4.0` pe Ubuntu 22.04, în WSL2).

```bash
g++ -std=c++17 -Wall -Wextra -o lucrare4 main.cpp
./lucrare4
```

Programul se compilează fără avertismente și afișează cele trei secțiuni (`=== ITEM 4 ===`, `=== ITEM 5 ===`, `=== ITEM 6 ===`).

## 5. Concluzii

- Lista de inițializare este mai eficientă (evită construirea implicită urmată de atribuire) și este singura variantă posibilă pentru membrii `const` și referințe.
- Este util să se știe ce funcții generează compilatorul și când nu o poate face (membri referință sau `const`), pentru a evita comportamente neașteptate.
- Funcțiile nedorite trebuie dezactivate explicit (`= delete` în C++ modern), iar eroarea apare la compilare, nu la execuție.

## 6. Bibliografie

1. S. Meyers, *Effective C++*, 3rd Edition, Addison-Wesley, 2005 – Items 4, 5, 6.
2. ISO/IEC 14882:2017 (C++17).
