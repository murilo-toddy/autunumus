#ifndef PATHDETECTIONALGORITHM_ZMQPROVIDER_HPP
#define PATHDETECTIONALGORITHM_ZMQPROVIDER_HPP

#include <zmq.hpp>

#include "../../protos/cones.pb.h"
#include "contour_processing.hpp"


class ZmqProvider {
private:
    std::string port;
    zmq::context_t context;
    zmq::socket_t socket;

    void setup() {
        this->context = zmq::context_t(1);
        this->socket = zmq::socket_t(context, ZMQ_PUSH);
        this->socket.bind(this->port);
    }

    static std::string create_cone_list_from_cone_data(const std::vector<cone_data>& cones) {
        ConeList cone_list;
        for (auto& cone : cones) {
            Cone* conePB = cone_list.add_cones();
            conePB->set_angle(cone.angle);
            conePB->set_distance(cone.distance);
            conePB->set_width(cone.width);
        }
        return cone_list.SerializeAsString();
    }

public:
    ZmqProvider() {
        this->port = "tcp://*:5556";
        this->setup();
    }

    explicit ZmqProvider(const std::string& port) {
        this->port = port;
        this->setup();
    }

    void send_cones(const std::vector<cone_data>& cones) {
        std::string serialized_cones = create_cone_list_from_cone_data(cones);
        zmq::message_t message(serialized_cones.data(), serialized_cones.size());
        this->socket.send(message);
    }
};


#endif //PATHDETECTIONALGORITHM_ZMQPROVIDER_HPP
