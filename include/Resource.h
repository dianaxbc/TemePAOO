#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <cstring>

class Resource {
private:
    size_t m_size;
    int* m_data;

public:
    explicit Resource(size_t size);
    ~Resource();
    Resource(const Resource& other);
    Resource(Resource&& other) noexcept;

    void printData() const;
};

#endif