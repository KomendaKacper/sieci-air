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




template<typename T>;

class IPackageStockpile {
public:
    using const_iterator = typename T::const_iterator;

    virtual void push(T&& package) = 0;
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile<Package> {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};

class PackageQueue {
public:
    PackageQueue(PackageQueueType) = default;

    ~PackageQueue(PackageQueueType) = default;

private:
}

#endif //SERWERYAIR_STORAGE_TYPES_HPP
