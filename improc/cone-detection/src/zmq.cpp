#include "../include/zmq.hpp"

void ZmqProvider::setup(const std::string& port) {
    this->context = zmq::context_t(1);
    this->socket = zmq::socket_t(context, ZMQ_PUSH);
    this->socket.bind(port);
}

void ZmqProvider::stop() {
    this->socket.close();
}

std::string ZmqProvider::create_cone_list_from_cone_data(const std::vector<cone_data>& cones) {
    ConeList cone_list;
    for (auto& cone : cones) {
        Cone* conePB = cone_list.add_cones();
        conePB->set_angle(cone.angle);
        conePB->set_distance(cone.distance);
        conePB->set_width(cone.width);
    }
    return cone_list.SerializeAsString();
}

void ZmqProvider::send_cones(const std::vector<cone_data>& cones) {
    std::string serialized_cones = create_cone_list_from_cone_data(cones);
    zmq::message_t message(serialized_cones.data(), serialized_cones.size());
    this->socket.send(message);
}
