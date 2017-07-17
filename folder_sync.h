#ifndef FILESYNC_H
#define FILESYNC_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextStream>
#include <QTreeView>
#include <QPlainTextEdit>
#include <QProcess>
#include <QLabel>

#include "custom_filesystem_model.h"

class FolderSync : public QObject
{
    Q_OBJECT
public:
    explicit FolderSync();

signals:

public slots:
    void sync();
    void cleanupRsync();
    void updateStdOut();
    void updateError();
    void saveListFile();
    void selectSource();
    void selectDest();

private:
    void updateLabel();
    void readListFile();

    CustomFileSystemModel model;

    bool rsyncRunning = false;
    QProcess *rsync;

    QString sourceDir;
    QString destDir;

    //widgets
    QWidget w;
    QVBoxLayout mainLayout;
    QHBoxLayout buttonsLayout;

    QPushButton selectSourceBut;
    QPushButton selectDestBut;
    QPushButton saveBut;
    QPushButton syncBut;
    QLabel label;

    QTreeView filetree;
    QPlainTextEdit textView;
};

#endif // FILESYNC_H
