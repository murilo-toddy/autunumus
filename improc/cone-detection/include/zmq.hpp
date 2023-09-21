#ifndef PATHDETECTIONALGORITHM_ZMQ_HPP
#define PATHDETECTIONALGORITHM_ZMQ_HPP

#include <zmq.hpp>

#include "../../protos/cones.pb.h"
#include "cone.hpp"


class ZmqProvider {
private:
    zmq::context_t context;
    zmq::socket_t socket;

    void setup(const std::string &port);
    void stop();
    std::string create_cone_list_from_cone_data(const std::vector<cone_data>& cones);

public:
    explicit ZmqProvider(const std::string& port = "tcp://*:5556") {
        this->setup(port);
    }

    void send_cones(const std::vector<cone_data>& cone);
};


#endif //PATHDETECTIONALGORITHM_ZMQ_HPP
