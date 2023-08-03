//
// Created by user on 03.08.23.
//

#ifndef GETMIC_PLAYER_H
#define GETMIC_PLAYER_H
#include <pulse/simple.h>

class player {
public:
    player();
    void startPlayer();
    void stopPlayer();
    void play(const std::string& buffer);
private:
    static inline const int rate = 44100;

    pa_simple *s = nullptr;
};


#endif //GETMIC_PLAYER_H
