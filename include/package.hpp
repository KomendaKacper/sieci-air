#include <cstdlib>
#include <set>
#include "types.hpp"

#ifndef SERWERYAIR_PACKAGE_HPP
#define SERWERYAIR_PACKAGE_HPP


class Package {
public:
    Package();

    Package(ElementID element_id) : ElementID_(element_id) { assigned_IDs.insert(ElementID_); }

    Package(Package &&package) : ElementID_(package.ElementID_) {}

    Package &operator=(Package &&package) noexcept ;

    ElementID get_id() const { return ElementID_; }

    ~Package();

private:

    ElementID ElementID_;
    static std::set<ElementID> freed_IDs;
    static std::set<ElementID> assigned_IDs;


};

#endif //SERWERYAIR_PACKAGE_HPP