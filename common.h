#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDebug>
#include <X11/extensions/XInput2.h>

/* Return 1 if XI2 is available, 0 otherwise */
static int has_xi2(Display *dpy)
{
    int major, minor;
    int rc;

    /* We support XI 2.2 */
    major = 2;
    minor = 2;

    rc = XIQueryVersion(dpy, &major, &minor);
    if (rc == BadRequest) {
        qDebug() << (QString("No XI2 support. Server supports version %d.%d only.")
                    .arg(major).arg(minor));
        return 0;
    } else if (rc != Success) {
        qDebug() << "Internal Error! This is a bug in Xlib.";
    }

    qDebug() << (QString("XI2 supported. Server provides version %d.%d.")
                .arg(major).arg(minor));

    return 1;
}

static void select_events(Display *dpy, Window win)
{
    XIEventMask evmasks[1];
    unsigned char mask1[(XI_LASTEVENT + 7)/8];

    memset(mask1, 0, sizeof(mask1));

    /* select for button and key events from all master devices */
    XISetMask(mask1, XI_RawMotion);

    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask1);
    evmasks[0].mask = mask1;

    XISelectEvents(dpy, win, evmasks, 1);
    XFlush(dpy);
}

#endif // COMMON_H
