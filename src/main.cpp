#include <QApplication>
#include "EyesTrayIcon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setQuitOnLastWindowClosed(false);
    
    EyesTrayIcon eyesTray;
    
    return a.exec();
}
