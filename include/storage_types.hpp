//
// Created by Fujitsu on 04.12.2023.
//
#include <cstdlib>
#include <set>
#include "types.hpp"
#include "package.hpp"

#ifndef SERWERYAIR_STORAGE_TYPES_HPP
#define SERWERYAIR_STORAGE_TYPES_HPP


enum class PackageQueueType{
    FIFO,
    LIFO
};

PackageQueueType x ;

switch (x){
    case PackageQueueType::FIFO:
        break;
    case PackageQueueType::LIFO:
        break;
    default:
        return NULL
}

class PackageQueue{
public:
    PackageQueue(PackageQueueType) = default;
    ~PackageQueue(PackageQueueType) = default;
private:

class IPageStockpile {
public:
    IPageStockpile () = default;
    using const_iterator = std::list<Package>::const_iterator; // alias na typ list<Package>

    ~IPageStockpile () = default;
private:

};

#endif //SERWERYAIR_STORAGE_TYPES_HPP
