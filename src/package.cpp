#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {

    if(freed_IDs.empty() and assigned_IDs.empty()){
        ElementID_ = 1;

    }else if(!freed_IDs.empty()){
        ElementID_ = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());

    }else {
        ElementID_ = *assigned_IDs.end() + 1;
    }

    assigned_IDs.insert(ElementID_);
}

Package &Package::operator=(Package &&package)  noexcept {
    if(this == &package)
        return *this;


    assigned_IDs.erase(this -> ElementID_);
    freed_IDs.insert(this -> ElementID_);

    this -> ElementID_ = package.ElementID_;
    assigned_IDs.insert(this -> ElementID_);

    return *this;
}

Package::~Package() {

    assigned_IDs.erase(ElementID_);
    freed_IDs.insert(ElementID_);
}

