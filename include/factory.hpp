//
// Created by SZK11 on 22.12.2023.
//

#ifndef SERWERY_AIR_FACTORY_HPP
#define SERWERY_AIR_FACTORY_HPP

#include "nodes.hpp"

class Factory {
public:
    // Metoda sprawdzająca spójność sieci
    bool is_consistent();

    // Metoda wykonująca ewentualne dostawy na rampy
    void do_deliveries();

    // Metoda wykonująca ewentualne przekazywanie półproduktów
    void do_package_passing();

    // Metoda wykonująca ewentualne przetwarzanie półproduktów przez robotników
    void do_work();

private:
    // Metoda pomocnicza usuwająca odbiorcę z kolekcji
    template <typename NodeType>
    void remove_receiver(NodeCollection<NodeType>& collection, const ElementID& id);

    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;

    // Inne metody i pola klasy Factory...
};

template <typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    // Metoda dodająca węzeł do kolekcji
    void add(Node node) {
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


#endif //SERWERY_AIR_FACTORY_HPP
