#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include "src/include/recorder.h"
#include "src/include/xinput.h"
#include "spdlog/spdlog.h"
#include "src/include/rakClient.h"
#include "src/include/player.h"

int main() {
    spdlog::set_level(spdlog::level::debug);

    recorder rec;
    player play;
    xinput::onKeyReleased = [&](int key) {
        if (key == 47) {
            rec.stopRecording();
        }
    };
    xinput::onKeyPressed = [&](int key) {
        if (key == 47) { //;
            rec.startRecording();
        }
    };

    auto s = std::async(std::launch::async, xinput::startGrabber);

    rakClient c;
    c.onPacket = [&](const std::string& pk){
        play.play(pk);
    };
    c.onConnect = [&](){
        play.startPlayer();
        spdlog::info("Connected");
    };
    c.onDisconnect = [&](){
        spdlog::info("Disconnected");
        play.stopPlayer();
        exit(1);
    };

    c.connect("127.0.0.1", 2222, 2425);
    while(true){
        auto buf = rec.flushBuffer();
        if(!buf.empty()) {
            c.sendPacket("\xfe" + buf); //false if not connected //todo zlib
        }
        c.tick();
        usleep(50);
    }
}