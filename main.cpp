#include <QApplication>
#include "folder_sync.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FolderSync window;

    QObject::connect(&app, SIGNAL(lastWindowClosed()), &window, SLOT(saveListFile()));

    return app.exec();
}
