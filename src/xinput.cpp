//
// Created by user on 03.08.23.
//

#include "include/xinput.h"

void xinput::startGrabber() {
    dpy = XOpenDisplay(nullptr);
    XRecordRange* rr = XRecordAllocRange();
    rr->device_events.first = KeyPress;
    rr->device_events.last = ButtonReleaseMask;
    XRecordClientSpec rcs = XRecordAllClients;
    rc = XRecordCreateContext (dpy, 0, &rcs, 1, &rr, 1);
    XFree (rr);
    XRecordEnableContext(dpy, rc, onKeyPressedHook, nullptr);

}

void xinput::onKeyPressedHook(XPointer arg, XRecordInterceptData *d) {
    if (d->category != XRecordFromServer)
        return;

    int key = ((unsigned char*) d->data)[1];
    int type = ((unsigned char*) d->data)[0] & 0x7F;
    int repeat = d->data[2] & 1;

    if(!repeat) {
        switch (type) {
            case KeyPress:
                if(onKeyPressed)
                    onKeyPressed(key);
                break;
            case KeyRelease:
                if(onKeyReleased)
                    onKeyReleased(key);
                break;
            default:
                break;
        }
    }
    XRecordFreeData(d);
}

void xinput::stopGrabber() {
    XRecordDisableContext(dpy, rc); //todo async
}

void xinput::cancelPress(int key) {
    //todo
}

void xinput::cancelRelease(int key) {
    //todo
}



