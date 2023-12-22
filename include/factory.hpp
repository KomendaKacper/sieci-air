//
// Created by SZK11 on 22.12.2023.
//

#ifndef SERWERY_AIR_FACTORY_HPP
#define SERWERY_AIR_FACTORY_HPP

#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "config.hpp"
#include "nodes.hpp"

#include <list>
#include <algorithm>

enum NodeColor
{
    UNVISITED,
    VISITED,
    VERIFIED
};


template <typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    NodeCollection() = default;

    // Metoda dodająca węzeł do kolekcji
    void add(Node &&node) {
        collection_.emplace_back(std::move(node));
    }

    // Metoda wyszukująca węzeł o podanym ID
    iterator find_by_id(const ElementID& id) {
        return std::find_if(collection_.begin(), collection_.end(),
                            [&id](const Node& node) { return node.get_id() == id; });
    }

    // Metoda wyszukująca węzeł o podanym ID (wersja const)
    const_iterator find_by_id(const ElementID& id) const {
        return std::find_if(collection_.begin(), collection_.end(),
                            [&id](const Node& node) { return node.get_id() == id; });
    }

    // Metoda usuwająca węzeł o podanym ID
    void remove_by_id(const ElementID& id) {
        auto it = find_by_id(id);
        if (it != collection_.end()) {
            collection_.erase(it);
        }
    }

    // Metoda zwracająca iterator na początek kolekcji
    iterator begin() {
        return collection_.begin();
    }

    // Metoda zwracająca iterator na koniec kolekcji
    iterator end() {
        return collection_.end();
    }

    // Metoda zwracająca const iterator na początek kolekcji
    const_iterator cbegin() const {
        return collection_.cbegin();
    }

    // Metoda zwracająca const iterator na koniec kolekcji
    const_iterator cend() const {
        return collection_.cend();
    }

private:
    container_t collection_;
};

class Factory {
public:

    Factory() = default;

    // Metoda sprawdzająca spójność sieci
    bool is_consistent();

    void do_deliveries(Time time);

    void do_package_passing();

    void do_work(Time time);

    // Ramp
    void add_ramp(Ramp &&ramp) { ramps_.add(std::move(ramp)); }

    void remove_ramp(ElementID& id) { ramps_.remove_by_id(id); }

    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID& id) const { return ramps_.find_by_id(id); }

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_.cbegin(); }

    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_.cend(); }

    // Worker
    void add_worker(Worker &&worker) {workers_.add(std::move(worker));}

    void remove_worker(ElementID id) ;

    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); };

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); };

    NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_.cbegin(); }

    NodeCollection<Worker>::const_iterator worker_cend() const { return workers_.cend(); }

    // Storehouse
    void add_storehouse(Storehouse &&storehouse) {storehouses_.add(std::move(storehouse));}

    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouses_.find_by_id(id); };

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); };

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_.cbegin(); }

    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_.cend(); }

private:
    // Metody pomocnicza usuwająca odbiorcę z kolekcji
    void remove_receiver(NodeCollection<Worker> &collection, ElementID id);
    void remove_receiver(NodeCollection<Storehouse> &collection, ElementID id);


    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const;


    NodeCollection<Worker> workers_;
    NodeCollection<Ramp> ramps_;
    NodeCollection<Storehouse> storehouses_;

};



#endif //SERWERY_AIR_FACTORY_HPP
