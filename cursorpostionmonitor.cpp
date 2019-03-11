#include "cursorpostionmonitor.h"
#include "common.h"

#include <QDebug>

CursorPostionMonitor::CursorPostionMonitor(QObject *parent) : QObject(parent)
{

}

void CursorPostionMonitor::startMonitor()
{
    Display *dpy = XOpenDisplay(NULL);
    int xi_opcode, event, error;

    if (!dpy) {
        qDebug() << "Failed to open display.";
        Q_EMIT failed();
        return;
    }

    if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &event, &error)) {
        qDebug() << "X Input extension not available.";
        Q_EMIT failed();
        return;
    }

    if (!has_xi2(dpy)) {
        Q_EMIT failed();
        return;
    }

    /* select for XI2 events */
    select_events(dpy, DefaultRootWindow(dpy));

    XEvent ev;

    while(1) {
        XGenericEventCookie *cookie = &ev.xcookie;

        XNextEvent(dpy, &ev);

        if (cookie->type != GenericEvent ||
            cookie->extension != xi_opcode ||
                !XGetEventData(dpy, cookie)) {
            continue;
        }

        switch (cookie->evtype) {
        case XI_RawMotion:
            qDebug() << QCursor::pos();
            Q_EMIT cursorMoved(QCursor::pos());
            break;
        }
        XFreeEventData(dpy, cookie);
    }
}
