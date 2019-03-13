#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEyes w;
    w.show();

    return a.exec();
}
