#include <cstdlib>
#include <set>

#ifndef SERWERYAIR_PACKAGE_HPP
#define SERWERYAIR_PACKAGE_HPP

class assigned_IDs {
public:
    assigned_IDs() = default;
    ~assigned_IDs() = default;

private:
    static int counter_;
    static std::set<unsigned int> set_;
};


class freed_IDs {
public:
    freed_IDs() = default;
    ~freed_IDs() = default;

private:
    static int counter_;
    static std::set<unsigned int> set_;
};


extern unsigned int counter;


class Package {
public:
    Package

    Package(unsigned int element_id) : ElementID_(element_id) {}
private:

    unsigned int ElementID_;

};

#endif //SERWERYAIR_PACKAGE_HPP
