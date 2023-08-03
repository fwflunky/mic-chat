//
// Created by user on 03.08.23.
//

#ifndef GETMIC_RECORDER_H
#define GETMIC_RECORDER_H


#include <cstdint>
#include <pulse/simple.h>
#include <vector>
#include <future>

class recorder {
public:
    recorder();
    void startRecording();
    void stopRecording();
    std::string flushBuffer();
    unsigned int getBufferSize();
private:
    static inline const int bufSize = 512;
    static inline const int rate = 44100;
    static inline const int maxSizeWithoutFlush = 5000;

    pa_simple *s = nullptr;

    std::vector<std::string> buffer = {};

    std::future<void> recordingThread = {};

    std::atomic_bool isShutdown = false;
    std::mutex sync = {};

    std::string* getWritableSector();
    void prepareForWrite();
    void doneOfWrite();

    void pulseaudioBegin();
    void pulseaudioRead(char *buf);
    void pulseaudioEnd();

    void startRecordingThread();
};


#endif //GETMIC_RECORDER_H
