#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// ==========================================
// ITEM 13: Use objects to manage resources
// ==========================================
class DatabaseConnection {
private:
    std::string dbName;

public:
    explicit DatabaseConnection(std::string name) : dbName(std::move(name)) {
        std::cout << "[DB] Conexiune deschisa catre: " << dbName << "\n";
    }

    ~DatabaseConnection() {
        std::cout << "[DB] Conexiune inchisa automat pentru: " << dbName << "\n";
    }

    void query(const std::string& sql) const {
        std::cout << "[DB - " << dbName << "] Execut query: " << sql << "\n";
    }
};

// Factory function (similara cu createInvestment() din curs)
inline std::unique_ptr<DatabaseConnection> createConnection(const std::string& name) {
    return std::make_unique<DatabaseConnection>(name);
}

// =========================================================
// ITEM 14: Copying behavior in resource-managing classes
// =========================================================
struct Mutex {
    std::string name;
    bool isLocked = false;
};

// C-style API (ca in slide-urile de curs)
inline void lock(Mutex* pm) {
    if (pm && !pm->isLocked) {
        pm->isLocked = true;
        std::cout << "[Mutex] LOCK pe: " << pm->name << "\n";
    }
}

inline void unlock(Mutex* pm) {
    if (pm && pm->isLocked) {
        pm->isLocked = false;
        std::cout << "[Mutex] UNLOCK pe: " << pm->name << "\n";
    }
}

// Varianta 1 (Item 14): Interzicem copierea (Prohibit copying)
class UniqueLock {
private:
    Mutex* mutexPtr;

public:
    explicit UniqueLock(Mutex* pm) : mutexPtr(pm) {
        lock(mutexPtr);
    }

    ~UniqueLock() {
        unlock(mutexPtr);
    }

    // Interzicem copierea si atribuirea
    UniqueLock(const UniqueLock&) = delete;
    UniqueLock& operator=(const UniqueLock&) = delete;
};

// Varianta 2 (Item 14): Reference-count cu std::shared_ptr si custom deleter
class SharedLock {
private:
    std::shared_ptr<Mutex> mutexPtr;

public:
    explicit SharedLock(Mutex* pm)
        : mutexPtr(pm, unlock) { // custom deleter: apeleaza unlock cand use_count() == 0
        lock(mutexPtr.get());
    }

    long ownersCount() const {
        return mutexPtr.use_count();
    }
};