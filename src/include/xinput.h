//
// Created by user on 03.08.23.
//

#ifndef GETMIC_XINPUT_H
#define GETMIC_XINPUT_H


#include <functional>
#include <atomic>
#include <future>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>

class xinput {
public:
    static void startGrabber();
    static void stopGrabber();
    static void cancelPress(int key);
    static void cancelRelease(int key);

    static inline std::function<void(int key)> onKeyPressed = nullptr;
    static inline std::function<void(int key)> onKeyReleased = nullptr;
private:
    static inline Display* dpy = nullptr;
    static inline Window root = {};
    static inline XEvent event = {};
    static inline XRecordContext rc = {};

    static void onKeyPressedHook(XPointer arg, XRecordInterceptData *d);
};


#endif //GETMIC_XINPUT_H
