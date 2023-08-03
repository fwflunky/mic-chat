//
// Created by user on 03.08.23.
//

#include <pulse/error.h>
#include <stdexcept>
#include "include/recorder.h"
#include "spdlog/spdlog.h"

recorder::recorder() {
    //buffer.resize(rate);
}

void recorder::startRecording() {
    std::scoped_lock<std::mutex> lock(sync);
    buffer.clear();
    isShutdown = false;
    pulseaudioBegin();
    recordingThread = std::async(std::launch::async, &recorder::startRecordingThread, this);
}

std::string recorder::flushBuffer() {
    std::scoped_lock<std::mutex> lock(sync);
    if(buffer.empty())
        return {};
    std::string data = buffer.at(0);
    buffer.erase(buffer.begin());
    return data;
}

void recorder::stopRecording() {
    isShutdown = true;
    recordingThread.wait();
}

unsigned int recorder::getBufferSize() {
    std::scoped_lock<std::mutex> lock(sync);
    return buffer.size();
}


std::string *recorder::getWritableSector() {
    auto lastSector = &buffer.back();
    lastSector->resize(bufSize);
    return lastSector;
}

void recorder::prepareForWrite() {
    if(buffer.size() > maxSizeWithoutFlush){
        buffer.erase(buffer.begin());
        spdlog::debug("Possible buffer overflow, missing flush");
    } else if(buffer.empty()){
        spdlog::debug("Empty buffer, resizing");
        buffer.resize(1);
    }
}

void recorder::doneOfWrite() {
    buffer.resize(buffer.size() + 1);
}

void recorder::pulseaudioBegin() {
    int error;

    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = rate,
            .channels = 2
    };

    if (!(s = pa_simple_new(nullptr, "Recorder", PA_STREAM_RECORD, nullptr, "record", &ss, nullptr, nullptr, &error))) {
        throw std::invalid_argument(fmt::format("pa_simple_new() failed: {}", pa_strerror(error)));
    }
}

void recorder::pulseaudioRead(char *buf) {
    int error;
    if (pa_simple_read(s, buf, bufSize, &error) < 0) {
        throw std::invalid_argument(fmt::format("pa_simple_read() failed: {}", pa_strerror(error)));
    }
}

void recorder::pulseaudioEnd() {
    if (s) {
        pa_simple_free(s);
        s = nullptr;
    }
}

void recorder::startRecordingThread() {
    spdlog::debug("Starting recording thread");
    while(!isShutdown){
        {
            std::scoped_lock<std::mutex> lock(sync);
            prepareForWrite();
            pulseaudioRead(getWritableSector()->data());
            doneOfWrite();
        }
        usleep(100);
    }
    pulseaudioEnd();
}


