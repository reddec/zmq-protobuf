/* 
 * File:   device.h
 * Author: reddec
 *
 * Created on 28 Feb 2015 y., 12:46
 */

#ifndef DEVICE_H
#define	DEVICE_H
#include <zmqpp/zmqpp.hpp>
#include <google/protobuf/message.h>

zmqpp::socket & operator<<(zmqpp::socket &pub_, const google::protobuf::Message & obj) {
    zmqpp::message message;
    message << obj.GetDescriptor()->full_name()
            << obj.SerializeAsString();
    pub_.send(message);
    return pub_;
}

template <class ProtoBufClass>
void accept_type(zmqpp::socket &sub_) {
    sub_.subscribe(ProtoBufClass::descriptor()->full_name());
}

zmqpp::socket & operator>>(zmqpp::socket &sub, google::protobuf::Message &message) {
    do {
        zmqpp::message zmsg;
        sub.receive(zmsg);
        // Filter required type if used multiple subscriptions
        if (zmsg.parts() > 1 && zmsg.get(0) == message.GetDescriptor()->full_name()) {
            message.ParseFromString(zmsg.get(1));
            break;
        }
    } while (1);
    return sub;
}

#endif	/* DEVICE_H */

