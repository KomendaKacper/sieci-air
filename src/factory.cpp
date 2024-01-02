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
        if (static_cast<ElementType>(i.first->get_receiver_type()) == STOREHOUSE)
            has_other_receiver = true;
        else if(static_cast<ElementType>(i.first->get_receiver_type()) == WORKER){
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

struct Argument {
    std::string title;
    std::string value;

    Argument(std::string t, std::string v) : title(t), value(v) {}
};

struct LineParser {
private:
    std::function<bool(Factory &, std::vector<Argument>)> parserFunc;
public:

    std::string tag;
    size_t argsCount;

    LineParser(std::string t, size_t ac, std::function<bool(Factory &, std::vector<Argument>)> f) : parserFunc(f),
                                                                                                    tag(t),
                                                                                                    argsCount(ac) {}

    bool Execute(Factory &f, const std::vector<Argument>& args) {
        if (args.size() != argsCount) {
            std::cout << "Wrong arguments count" << std::endl;
            return false;
        }
        return parserFunc(f, args);
    }
};

template<class T>
T S2T(std::string s) {
    std::stringstream ss(s);
    T n;
    ss >> n;

    return n;
}

bool ParseRamp(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset deliveryInterval = S2T<TimeOffset>(args[1].value);

    f.add_ramp(Ramp(id, deliveryInterval));

    return true;
}

bool ParseWorker(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset processingTime = S2T<TimeOffset>(args[1].value);

    PackageQueueType type = PackageQueueType::FIFO;

    if (args[2].value == "FIFO") {
        type = PackageQueueType::FIFO;
    } else if (args[2].value == "LIFO") {
        type = PackageQueueType::LIFO;
    } else {
        std::cout << "Unknown queue type" << std::endl;
        return false;
    }

    f.add_worker(Worker(id, processingTime, std::make_unique<PackageQueue>(type)));

    return true;
}

bool ParseStorehouse(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);

    f.add_storehouse(Storehouse(id));

    return true;
}

bool ParseLink(Factory &f, std::vector<Argument> args) {
    std::string src = args[0].value;
    std::string dest = args[1].value;

    std::vector<std::string> srcSplitted = splitString(src, '-');
    std::vector<std::string> destSplitted = splitString(dest, '-');

    PackageSender *srcObject = nullptr;
    IPackageReceiver *destObject = nullptr;

    ElementID srcId = S2T<ElementID>(srcSplitted[1]);
    ElementID destId = S2T<ElementID>(destSplitted[1]);
    if (srcSplitted[0] == "ramp")
        srcObject = &*f.find_ramp_by_id(srcId);
    else if (srcSplitted[0] == "worker")
        srcObject = &*f.find_worker_by_id(srcId);
    else {
        std::cout << "Unknown source object type" << std::endl;
        return false;
    }

    if (destSplitted[0] == "worker")
        destObject = &*f.find_worker_by_id(destId);
    else if (destSplitted[0] == "store")
        destObject = &*f.find_storehouse_by_id(destId);
    else {
        std::cout << "Unknown destination object type" << std::endl;
        return false;
    }

    srcObject->receiver_preferences_.add_receiver(destObject);

    return true;
}

std::array<LineParser, 4> parsers = {
        LineParser("LOADING_RAMP", 2, ParseRamp),
        LineParser("WORKER", 3, ParseWorker),
        LineParser("STOREHOUSE", 1, ParseStorehouse),
        LineParser("LINK", 2, ParseLink),
};

std::vector<Argument> GenerateArgs(std::vector<std::string> parts, int startIndex = 1) {
    std::vector<Argument> args;
    for (size_t i = startIndex; i < parts.size(); i++) {
        auto p = splitString(parts[i], '=');
        if (p.size() != 2) {
            std::cout << "Error getting argument from \"" << parts[i] << "\" - wrong split string size" << std::endl;
            return std::vector<Argument>();
        }

        args.push_back(Argument(p[0], p[1]));
    }

    return args;
}

bool ParseLine(Factory &f, std::string line) {
    line = removeWhitespaces(line);

    if (line.empty() || line[0] == ';' || line[0] == ' ')
        return true;

    auto data = splitString(line, ' ');
    std::string tag = data[0];
    for (auto & parser : parsers) {
        if (parser.tag == tag) {
            auto args = GenerateArgs(data);
            return parser.Execute(f, args);
        }
    }

    std::cout << "Parser not found for line \"" << line << "\"" << std::endl;
    return false;
}

Factory load_factory_structure(std::istream &is) {
    Factory f;

    std::string s;
    for (int i = 0; getline(is, s); i++) {
        try {
            if (!ParseLine(f, s))
                std::cout << "Error parsing line at " << i + 1 << std::endl;
        }
        catch (const std::exception &e) {
            std::cout << "Exception thrown while parsing file at line " << i + 1 << std::endl;
        }

    }

    return f;
}

void save_factory_structure(Factory &fact, std::ostream &os) {
    auto &f = (Factory &) fact;

    os << std::endl << "; == LOADING RAMPS ==" << std::endl << std::endl;
    for (auto i = f.ramp_cbegin(); i != f.ramp_cend(); i++) {
        os << "LOADING_RAMP id=" << i->get_id() << " delivery-interval=" << i->get_delivery_interval() << std::endl;
    }

    os << std::endl << "; == WORKERS ==" << std::endl << std::endl;
    for (auto i = f.worker_cbegin(); i != f.worker_cend(); i++) {
        os << "WORKER id=" << i->get_id() << " processing-time=" << i->get_processing_duration() << " queue-type=";
        switch (i->get_queue()->get_queue_type()) {
            case PackageQueueType::FIFO:
                os << "FIFO" << std::endl;
                break;

            case PackageQueueType::LIFO:
                os << "LIFO" << std::endl;
                break;
        }
    }

    os << std::endl << "; == STOREHOUSES ==" << std::endl << std::endl;
    for (auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); i++) {
        os << "STOREHOUSE id=" << i->get_id() << std::endl;
    }

    os << std::endl << "; == LINKS ==" << std::endl << std::endl;


    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        bool ok = false;
        auto prefs = it->receiver_preferences_.get_preferences();
        for (auto & pref : prefs) {
            ok = true;
            os << "LINK src=ramp-" << it->get_id() << " dest=";
            std::string typeStr;
            switch (pref.first->get_receiver_type()) {
                case ReceiverType::STOREHOUSE:
                    typeStr = "store";
                    break;

                case ReceiverType::WORKER:
                    typeStr = "worker";
                    break;
            }

            os << typeStr << "-" << pref.first->get_id() << std::endl;
        }

        if (ok)
            os << std::endl;
    }

    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++) {
        bool ok = false;
        auto prefs = it->receiver_preferences_.get_preferences();
        for (auto & pref : prefs) {
            ok = true;
            os << "LINK src=worker-" << it->get_id() << " dest=";
            std::string typeStr;
            switch (pref.first->get_receiver_type()) {
                case ReceiverType::STOREHOUSE:
                    typeStr = "store";
                    break;

                case ReceiverType::WORKER:
                    typeStr = "worker";
                    break;
            }

            os << typeStr << "-" << pref.first->get_id() << std::endl;
        }

        if (ok)
            os << std::endl;
    }
}