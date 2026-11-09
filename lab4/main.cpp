// Lucrarea 4 - Effective C++ (Scott Meyers): Item 4, Item 5, Item 6
// Compilare: g++ -std=c++17 -Wall -Wextra -o lucrare4 main.cpp
#include <iostream>
#include <string>
using namespace std;

// Clasa "martor": isi afiseaza fiecare apel, ca sa vedem ce se intampla in spate.
class Tracer {
public:
    Tracer() : v() { cout << "    Tracer()            [constructor implicit]\n"; }
    Tracer(const string& s) : v(s) { cout << "    Tracer(string)      [initializare directa]\n"; }
    Tracer(const Tracer& o) : v(o.v) { cout << "    Tracer(const Tracer&) [constructor de copiere]\n"; }
    Tracer& operator=(const Tracer& o) { v = o.v; cout << "    Tracer::operator=   [atribuire]\n"; return *this; }
    Tracer& operator=(const string& s) { v = s; cout << "    Tracer::operator=(string) [atribuire]\n"; return *this; }
    ~Tracer() { cout << "    ~Tracer()           [destructor]\n"; }
private:
    string v;
};

// =====================================================================
// ITEM 4: Make sure that objects are initialized before they're used
// =====================================================================

// (a) Atribuire in corpul constructorului  vs.  lista de initializare.
class EntryBad {
public:
    EntryBad(const string& name) {
        theName = name;          // ATRIBUIRE: mai intai s-a apelat constructorul implicit!
        numTimesConsulted = 0;   // atribuire (pentru tipuri built-in nu conteaza costul, dar e stil gresit)
    }
private:
    Tracer theName;
    int numTimesConsulted;
};

class EntryGood {
public:
    EntryGood(const string& name)
        : theName(name),         // INITIALIZARE: un singur apel de constructor
          numTimesConsulted(0) {}
    EntryGood() : theName(), numTimesConsulted(0) {}  // si constructorul implicit initializeaza explicit
private:
    // Membrii se initializeaza in ORDINEA DECLARARII, nu in ordinea din lista!
    Tracer theName;
    int numTimesConsulted;
};

// Membrii const si referintele TREBUIE initializati in lista (nu pot fi atribuiti).
class MustUseInitList {
public:
    MustUseInitList(int& r, int c) : ref(r), constant(c) {}
    void print() const { cout << "    ref=" << ref << " constant=" << constant << "\n"; }
private:
    int& ref;
    const int constant;
};

// (b) Ordinea initializarii obiectelor non-locale statice din unitati de traducere diferite
//     este nedefinita -> solutia: obiect local static intr-o functie (Singleton Meyers).
class FileSystem {
public:
    FileSystem() { cout << "    FileSystem construit\n"; }
    size_t numDisks() const { return 4; }
};

FileSystem& tfs() {              // inlocuieste "extern FileSystem tfs;"
    static FileSystem fs;        // initializat la PRIMA apelare a functiei
    return fs;
}

class Directory {
public:
    Directory() {
        size_t disks = tfs().numDisks();   // prima apelare construieste FileSystem
        cout << "    Directory construit, discuri: " << disks << "\n";
    }
};

void item4() {
    cout << "=== ITEM 4 ===\n";
    cout << "  EntryBad (atribuire in corp):\n";
    { EntryBad e("Ana"); }
    cout << "  EntryGood (lista de initializare):\n";
    { EntryGood e("Ana"); }

    int x = 10;
    cout << "  Membri const/referinta (doar prin lista de initializare):\n";
    MustUseInitList m(x, 5);
    m.print();

    cout << "  Initializare statica sigura (Meyers singleton):\n";
    Directory d;   // tfs() e creat exact cand are nevoie de el
    (void)d;
}

// =====================================================================
// ITEM 5: Know what functions C++ silently writes and calls
// =====================================================================

// Daca scriem "class Empty {};" compilatorul genereaza (la nevoie):
// constructor implicit, constructor de copiere, operator= de copiere, destructor
// (si, in C++11, constructor/operator= de mutare).
class Empty {};

class Verbose {           // aceleasi functii, dar scrise de mana ca sa le vedem
public:
    Verbose()                          { cout << "    Verbose()\n"; }
    Verbose(const Verbose&)            { cout << "    Verbose(const Verbose&)\n"; }
    Verbose& operator=(const Verbose&) { cout << "    Verbose::operator=\n"; return *this; }
    ~Verbose()                         { cout << "    ~Verbose()\n"; }
};

class Holder {            // clasa FARA functii scrise: compilatorul le genereaza
    Verbose v;            // si apeleaza functiile membrului
    int n = 0;
};

// Cand compilatorul REFUZA sa genereze operator=: membru referinta sau const.
template<typename T>
class NamedObject {
public:
    NamedObject(string& name, const T& value) : nameValue(name), objectValue(value) {}
    T get() const { return objectValue; }
private:
    string& nameValue;    // referinta
    const T objectValue;  // const
};

void item5() {
    cout << "\n=== ITEM 5 ===\n";
    cout << "  sizeof(Empty) = " << sizeof(Empty) << " (nu e 0, obiectele au adrese distincte)\n";

    Empty e1;             // constructor implicit generat
    Empty e2(e1);         // constructor de copiere generat
    e2 = e1;              // operator= generat
    (void)e1; (void)e2;   // destructor generat la sfarsitul functiei

    cout << "  Holder h1; (constructor implicit generat apeleaza Verbose())\n";
    Holder h1;
    cout << "  Holder h2(h1); (copiere generata apeleaza Verbose(const Verbose&))\n";
    Holder h2(h1);
    cout << "  h2 = h1; (atribuire generata apeleaza Verbose::operator=)\n";
    h2 = h1;
    cout << "  (la sfarsit, destructorii generati apeleaza ~Verbose())\n";

    string newDog("Persephone"), oldDog("Satch");
    NamedObject<int> p(newDog, 2);
    NamedObject<int> s(oldDog, 36);
    (void)p; (void)s;
    // p = s;   // EROARE DE COMPILARE: compilatorul nu poate genera operator=
    //          // pentru clase cu membri referinta sau const. Trebuie scris manual.
    cout << "  p = s; NU compileaza (membru referinta/const) - vezi linia comentata\n";
}

// =====================================================================
// ITEM 6: Explicitly disallow the use of compiler-generated functions you do not want
// =====================================================================

// Varianta 1 (C++11): "= delete"
class HomeForSale {
public:
    HomeForSale() = default;
    HomeForSale(const HomeForSale&) = delete;
    HomeForSale& operator=(const HomeForSale&) = delete;
};

// Varianta 2 (stil vechi, cartea originala): clasa de baza Uncopyable
// cu functiile de copiere PRIVATE si nedefinite.
class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

class HomeForSaleOld : private Uncopyable {
    // nu mai declaram nimic: copierea e blocata prin mostenire
};

void item6() {
    cout << "\n=== ITEM 6 ===\n";
    HomeForSale h1;
    // HomeForSale h2(h1);   // EROARE: use of deleted function
    // HomeForSale h3; h3 = h1;   // EROARE: use of deleted function
    HomeForSaleOld o1;
    // HomeForSaleOld o2(o1);     // EROARE: constructorul de copiere e inaccesibil (private)
    (void)h1; (void)o1;
    cout << "  Copierea obiectelor HomeForSale / HomeForSaleOld este interzisa la compilare.\n";
    cout << "  (decomenteaza liniile de mai sus ca sa vezi erorile)\n";
}

int main() {
    item4();
    item5();
    item6();
    return 0;
}
