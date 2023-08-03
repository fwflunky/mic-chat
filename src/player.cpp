//
// Created by user on 03.08.23.
//

#include <string>
#include <pulse/error.h>
#include <stdexcept>
#include "include/player.h"
#include "spdlog/spdlog.h"


player::player() {

}

void player::play(const std::string &buffer) {
    int error;
    pa_simple_write(s, buffer.data(), (size_t) buffer.size(), &error);
    //maybe throw here
}

void player::startPlayer() {
    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = rate,
            .channels = 2
    };
    int error;
    if (!(s = pa_simple_new(nullptr, "Player", PA_STREAM_PLAYBACK, nullptr, "playback", &ss, nullptr, nullptr, &error))) {
        throw std::invalid_argument(fmt::format("pa_simple_new() failed: {}", pa_strerror(error)));
    }
}

void player::stopPlayer() {
    int error;
    pa_simple_drain(s, &error);
    //maybe throw here
}
