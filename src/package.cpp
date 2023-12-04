#include "package.hpp"


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


Package::~Package() {

    assigned_IDs.erase(ElementID_);
    freed_IDs.insert(ElementID_);
}

