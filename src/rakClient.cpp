//
// Created by user on 03.08.23.
//

#include <stdexcept>
#include "include/rakClient.h"
#include "slikenet/MessageIdentifiers.h"
#include "spdlog/spdlog.h"

rakClient::rakClient() {
    instance = SLNet::RakPeerInterface::GetInstance();
    instance->SetMaximumIncomingConnections(1);

    identifier.SetUndefined();
}

rakClient::~rakClient() {
    identifier.SetUndefined();

    instance->Shutdown(1000);
    SLNet::RakPeerInterface::DestroyInstance(instance);
}

void rakClient::connect(const std::string &ip, unsigned short port, unsigned short localPort) {
    auto connectorSocket = new SLNet::SocketDescriptor(localPort, "0.0.0.0");
    connectorSocket->socketFamily = AF_INET;
    if(auto code = instance->Startup(1, connectorSocket, 1); code != SLNet::StartupResult::RAKNET_STARTED) {
        throw std::invalid_argument(fmt::format("Unable to connect: {0}", (int) code));
    }
    //delete connectorSocket; todo shared ptr
    auto at = instance->Connect(ip.data(), port, nullptr, 0);
    if(at != SLNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED){
        throw std::invalid_argument("Unable to connect: CONNECTION_ATTEMPT_STARTED != at");
    }
}

void rakClient::tick() {
    SLNet::Packet *pk = instance->Receive();
    if (pk) {
        if (pk->data[0] == ID_CONNECTION_REQUEST_ACCEPTED) {
            identifier = pk->guid;
            onConnect();
        } else if(pk->data[0] == ID_DISCONNECTION_NOTIFICATION || pk->data[0] == ID_CONNECTION_LOST) {
            identifier.SetUndefined();
            onDisconnect();
        } else if(pk->data[0] == 0xfe) {
            onPacket({(char*) pk->data + 1, pk->length - 1});
        }
        instance->DeallocatePacket(pk);
    }
}

bool rakClient::sendPacket(const std::string &pk) {
    if(identifier.IsUndefined())
        return false;
    return instance->Send(pk.data(), (int) pk.size(), PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, identifier, false) > 0;
}