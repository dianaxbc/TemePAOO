#include "Resource.h"

Resource::Resource(size_t size) : m_size(size), m_data(new int[size]{0}) {
    std::cout << "[Constructor] Memorie alocata pe Heap la adresa: " << m_data << std::endl;
}

Resource::~Resource() {
    std::cout << "[Destructor] Memorie eliberata de la adresa: " << m_data << std::endl;
    delete[] m_data;
}

Resource::Resource(const Resource& other) : m_size(other.m_size), m_data(new int[other.m_size]) {
    std::memcpy(m_data, other.m_data, m_size * sizeof(int));
    std::cout << "[Copy Constructor] Copie noua facuta la adresa: " << m_data << std::endl;
}

Resource::Resource(Resource&& other) noexcept : m_size(other.m_size), m_data(other.m_data) {
    other.m_size = 0;
    other.m_data = nullptr;
    std::cout << "[Move Constructor] Resursa a fost mutata cu succes." << std::endl;
}

void Resource::printData() const {
    if (m_data == nullptr) {
        std::cout << "Obiectul este gol (a fost mutat)." << std::endl;
    } else {
        std::cout << "Obiectul foloseste memoria de la: " << m_data << std::endl;
    }
}