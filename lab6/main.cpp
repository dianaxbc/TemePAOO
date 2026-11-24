#include "Resources.hpp"

int main() {
    std::cout << "=== 1. Test Item 13: std::unique_ptr (proprietate unica) ===\n";
    {
        auto conn1 = createConnection("UsersDB");
        conn1->query("SELECT * FROM students;");

        // auto conn2 = conn1; // EROARE de compilare: unique_ptr nu permite copierea!
        auto conn2 = std::move(conn1); // Transfer explicit de proprietate
        if (!conn1) {
            std::cout << "conn1 este acum nullptr dupa std::move\n";
        }
        conn2->query("UPDATE grades SET grade = 10;");
    } // conn2 iese din scope -> destructorul inchide conexiunea automat

    std::cout << "\n=== 2. Test Item 13: std::shared_ptr (proprietate partajata) ===\n";
    {
        auto sharedConn1 = std::make_shared<DatabaseConnection>("AnalyticsDB");
        std::cout << "Reference count initial: " << sharedConn1.use_count() << "\n";
        {
            auto sharedConn2 = sharedConn1; // Copiere permisa
            std::cout << "Reference count dupa copiere: " << sharedConn1.use_count() << "\n";
            sharedConn2->query("SELECT COUNT(*) FROM logs;");
        } // sharedConn2 distrus -> count revine la 1, conexiunea ramane deschisa
        std::cout << "Reference count dupa iesirea lui sharedConn2: " << sharedConn1.use_count() << "\n";
    } // sharedConn1 distrus -> count ajunge la 0, conexiunea se inchide

    std::cout << "\n=== 3. Test Item 14 (Varianta 1): UniqueLock (copiere interzisa) ===\n";
    Mutex m1{"PrinterMutex"};
    {
        UniqueLock lock1(&m1);
        // UniqueLock lock2(lock1); // EROARE de compilare: copierea este interzisa!
    } // Se apeleaza unlock o singura data

    std::cout << "\n=== 4. Test Item 14 (Varianta 2): SharedLock (custom deleter) ===\n";
    Mutex m2{"FileMutex"};
    {
        SharedLock sLock1(&m2);
        std::cout << "Proprietari mutex: " << sLock1.ownersCount() << "\n";
        {
            SharedLock sLock2(sLock1); // Copiere permisa, partajeaza acelasi Mutex
            std::cout << "Proprietari mutex dupa copiere: " << sLock1.ownersCount() << "\n";
        } // sLock2 distrus -> NU face unlock inca, mai exista sLock1!
        std::cout << "Proprietari mutex ramasi: " << sLock1.ownersCount() << "\n";
    } // Ultimul proprietar (sLock1) iese din scope -> acum se apeleaza unlock!

    return 0;
}