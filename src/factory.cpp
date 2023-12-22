//
// Created by SZK11 on 22.12.2023.
//

#include <stdexcept>
#include "factory.hpp"

void Factory::do_deliveries(Time time){
    for (auto & ramp : ramps_)
        ramp.deliver_goods(time);
}

void Factory::do_package_passing(){
    for (auto & ramp : ramps_)
        ramp.send_package();

    for(auto & worker : workers_)
        worker.send_package();
}

void Factory::do_work(Time time){
    for (auto & worker : workers_)
        worker.do_work(time);
}

void Factory::remove_receiver(NodeCollection<Worker>& collection, ElementID id) {
    Worker& receiver = *collection.find_by_id(id);
    for (auto &i : ramps_){
        i.receiver_preferences_.remove_receiver(&receiver);
    }
}

void Factory::remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {
    Storehouse& receiver = *collection.find_by_id(id);
    for (auto &i : workers_){
        i.receiver_preferences_.remove_receiver(&receiver);
    }
}

void Factory::remove_storehouse(ElementID id){
    remove_receiver(storehouses_, id);
    storehouses_.remove_by_id(id);
}

void Factory::remove_worker(ElementID id){
    remove_receiver(workers_, id);
    workers_.remove_by_id(id);
}

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const {

    if (node_colors[sender] == NodeColor::VERIFIED)
        return true;

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("No receivers defined.");
    }

    bool has_other_receiver = false;
    for (auto & i : sender->receiver_preferences_.get_preferences()){

        // Sprawdza, czy każdy cykl kończy się w magazynie
        if (i.first->get_receiver_type() == STOREHOUSE)
            has_other_receiver = true;
        else if(i.first->get_receiver_type() == WORKER){
            IPackageReceiver* receiver_ptr = i.first;

            // Konwersja pracownika (odbiorcy) na nadawcę
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            // Pomija, jeśli pracownik jest jednocześnie nadawcą
            if(sendrecv_ptr == sender){
                continue;
            }
            has_other_receiver = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                // Rekurencyjne wywołanie DFS dla pracownika (nadawcy)
                has_reachable_storehouse(sendrecv_ptr, node_colors);

            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;

    if(has_other_receiver){
        return true;
    }

    else{
        throw std::logic_error("No reachable storehouse.");
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> color;

    // Inicjalizacja kolorów dla każdego węzła
    for (auto& ramp : ramps_) {
        color[&ramp] = NodeColor::UNVISITED;
    }
    for (auto& worker : workers_) {
        color[&worker] = NodeColor::UNVISITED;
    }

    try{
        for (auto & ramp : ramps_)
            has_reachable_storehouse(&ramp, color);
        return true; // Sieć jest spójna
    }
    catch(std::logic_error &e){
        return false; // Wyjątek oznacza błąd spójności
    }
}

