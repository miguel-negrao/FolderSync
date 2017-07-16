#include <QApplication>
#include "file_sync.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FileSync window;

    QObject::connect(&app, SIGNAL(lastWindowClosed()), &window, SLOT(saveListFile()));

    return app.exec();
}
