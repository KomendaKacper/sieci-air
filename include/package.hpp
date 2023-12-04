#include <cstdlib>

#ifndef SERWERYAIR_PACKAGE_HPP
#define SERWERYAIR_PACKAGE_HPP

class assigned_IDs {
public:
    assigned_IDs() = default;
    ~assigned_IDs() = default;

private:
    static int counter_;
    static std::set<int> set_;
};

extern int counter;

class Package {
public:
    Package() : ElementID_() {}
private:

    unsigned int ElementID_;

};

#endif //SERWERYAIR_PACKAGE_HPP
