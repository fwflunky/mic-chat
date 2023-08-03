//
// Created by user on 03.08.23.
//

#ifndef GETMIC_RAKCLIENT_H
#define GETMIC_RAKCLIENT_H

#include <functional>
#include "slikenet/peer.h"
class rakClient {
public:
    rakClient();
    ~rakClient();

    std::function<void()> onConnect = nullptr;
    std::function<void()> onDisconnect = nullptr;
    std::function<void(std::string pk)> onPacket = nullptr;
    void connect(const std::string &ip, unsigned short port, unsigned short localPort);
    void tick();
    bool sendPacket(const std::string &pk);
private:
    SLNet::AddressOrGUID identifier = {};
    SLNet::RakPeerInterface* instance = nullptr;
};


#endif //GETMIC_RAKCLIENT_H
