#pragma once
#include <iostream>
#include <cstring>
#include <utility>

// Clasa de baza (demonstreaza Item 10 si Item 11)
class Device {
protected:
    char* brand;
    int batteryLevel;

public:
    // Constructor normal
    Device(const char* brandName = "Generic", int battery = 100)
        : batteryLevel(battery) {
        brand = new char[std::strlen(brandName) + 1];
        std::strcpy(brand, brandName);
    }

    // Copy Constructor
    Device(const Device& rhs) : batteryLevel(rhs.batteryLevel) {
        brand = new char[std::strlen(rhs.brand) + 1];
        std::strcpy(brand, rhs.brand);
        std::cout << "[Device] Copy constructor apelat\n";
    }

    // Move Constructor
    Device(Device&& rhs) noexcept
        : brand(rhs.brand), batteryLevel(rhs.batteryLevel) {
        rhs.brand = nullptr;
        rhs.batteryLevel = 0;
        std::cout << "[Device] Move constructor apelat\n";
    }

    // Item 10 & Item 11: Assignment operator (safe la self-assignment & exception-safe)
    Device& operator=(const Device& rhs) {
        std::cout << "[Device] Copy operator= apelat\n";
        // Alocam mai intai copia (functioneaza si daca this == &rhs si daca new arunca exceptie)
        char* oldBrand = brand;
        brand = new char[std::strlen(rhs.brand) + 1];
        std::strcpy(brand, rhs.brand);
        delete[] oldBrand;

        batteryLevel = rhs.batteryLevel;
        return *this; // Item 10: return reference to *this
    }

    // Move Assignment Operator (Slide 6)
    Device& operator=(Device&& rhs) noexcept {
        std::cout << "[Device] Move operator= apelat\n";
        if (this != &rhs) {
            delete[] brand;
            brand = rhs.brand;
            batteryLevel = rhs.batteryLevel;

            rhs.brand = nullptr;
            rhs.batteryLevel = 0;
        }
        return *this; // Item 10
    }

    // Item 10: si operatorii compusi returneaza *this
    Device& operator+=(int charge) {
        batteryLevel += charge;
        if (batteryLevel > 100) batteryLevel = 100;
        return *this;
    }

    virtual ~Device() {
        delete[] brand;
    }

    virtual void print() const {
        std::cout << "Brand: " << (brand ? brand : "null")
                  << " | Baterie: " << batteryLevel << "%";
    }
};

// Clasa derivata (demonstreaza Item 12: copierea tuturor partilor obiectului)
class Laptop : public Device {
private:
    int ramGB;

public:
    Laptop(const char* brandName = "Generic", int battery = 100, int ram = 8)
        : Device(brandName, battery), ramGB(ram) {}

    // Item 12: Apelam explicit copy constructorul clasei de baza!
    Laptop(const Laptop& rhs)
        : Device(rhs), ramGB(rhs.ramGB) {
        std::cout << "[Laptop] Copy constructor apelat\n";
    }

    // Move Constructor
    Laptop(Laptop&& rhs) noexcept
        : Device(std::move(rhs)), ramGB(rhs.ramGB) {
        rhs.ramGB = 0;
        std::cout << "[Laptop] Move constructor apelat\n";
    }

    // Item 12: Apelam explicit operator= din clasa de baza!
    Laptop& operator=(const Laptop& rhs) {
        std::cout << "[Laptop] Copy operator= apelat\n";
        if (this != &rhs) {
            Device::operator=(rhs); // Copiaza partea de baza (brand + batteryLevel)
            ramGB = rhs.ramGB;      // Copiaza partea derivata (ramGB)
        }
        return *this; // Item 10
    }

    // Move Assignment
    Laptop& operator=(Laptop&& rhs) noexcept {
        std::cout << "[Laptop] Move operator= apelat\n";
        if (this != &rhs) {
            Device::operator=(std::move(rhs));
            ramGB = rhs.ramGB;
            rhs.ramGB = 0;
        }
        return *this;
    }

    void print() const override {
        Device::print();
        std::cout << " | RAM: " << ramGB << "GB\n";
    }
};