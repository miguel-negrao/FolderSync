#include "file_sync.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>
#include <QFileDialog>

FileSync::FileSync() :
    selectSourceBut("Source"),
    selectDestBut("Destination"),
    saveBut("Save list to file"),
    syncBut("Sync")
{
    QCoreApplication::setOrganizationName("miguelnegrao");
    QCoreApplication::setOrganizationDomain("friendlyvirus.org");
    QCoreApplication::setApplicationName("FolderSync");

    //init
    QSettings settings;
    sourceDir = settings.value("sourceDir", QDir::homePath()).toString();
    destDir = settings.value("destDir", QDir::tempPath()).toString();
    updateLabel();
    readListFile();

    //widgets
    w.resize(1200,800);

    filetree.setModel(&model);
    filetree.setColumnWidth(0, 600);
    filetree.setRootIndex(model.setRootPath(sourceDir));

    textView.setAcceptDrops(false);
    textView.setReadOnly(true);

    //add widgets to layout
    buttonsLayout.addWidget(&selectSourceBut);
    buttonsLayout.addWidget(&selectDestBut);
    buttonsLayout.addWidget(&saveBut);
    buttonsLayout.addWidget(&syncBut);
    mainLayout.addLayout(&buttonsLayout);
    mainLayout.addWidget(&label);
    mainLayout.addWidget(&filetree,3);
    mainLayout.addWidget(&textView,1);
    w.setLayout(&mainLayout);

    //connect signals to slots
    QObject::connect(&selectSourceBut, SIGNAL(clicked(bool)), this, SLOT(selectSource()));
    QObject::connect(&selectDestBut, SIGNAL(clicked(bool)), this, SLOT(selectDest()));
    QObject::connect(&saveBut, SIGNAL(clicked(bool)), this, SLOT(saveListFile()));
    QObject::connect(&syncBut, SIGNAL(clicked(bool)), this, SLOT(sync()));

    w.show();
}

void FileSync::saveListFile()
{

    QString listFile = QString(sourceDir).append("/folderlist.txt");
    QDir base(sourceDir);
    QFile data(listFile);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out.setCodec("UTF-8");

        for (const QString &v : model.directories) {
            out << base.relativeFilePath(v) << endl;
        }
    }
}

void FileSync::readListFile()
{
    QString listFile = QString(sourceDir).append("/folderlist.txt");
    QFileInfo info(listFile);
    if (info.exists())
    {
        QFile textFile(listFile);
        if (textFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            model.directories.clear();
            QTextStream textStream(&textFile);
            while (true)
            {
                QString line = textStream.readLine();

                if (line.isNull())
                    break;
                else
                    model.directories.append(line.prepend("/").prepend(sourceDir));
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot open file.");
            msgBox.exec();
        }
    }
}

void FileSync::sync()
{
    if(rsyncRunning)
        return;

    QString program = "/usr/bin/rsync"; // ;  "/bin/echo"
    QStringList arguments;
    arguments << "-rvR" << "--size-only";

    QDir base(sourceDir);
    // /./ makes create directory structure starting from that point on.
    for (const QString &v : model.directories) {
        QString path = base.relativeFilePath(v).prepend(QString(sourceDir).append("/./"));
        arguments << path;
    }
    arguments << destDir;

    textView.clear();
    syncBut.setEnabled(false);

    rsync = new QProcess;

    connect(rsync, SIGNAL(finished(int)), this, SLOT(cleanupRsync()));
    connect(rsync, SIGNAL(readyReadStandardOutput()), this, SLOT(updateStdOut()));
    connect(rsync, SIGNAL(readyReadStandardError()), this, SLOT(updateError()));

    rsync->start(program, arguments);

    if (!rsync->waitForStarted())
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot run rsync.");
        msgBox.exec();
    }
}

void FileSync::updateStdOut()
{
    QByteArray result = rsync->readAllStandardOutput();
    textView.appendPlainText(result);
}

void FileSync::updateError()
{
    QByteArray result = rsync->readAllStandardError();
    textView.appendPlainText(result);
}

void FileSync::cleanupRsync()
{
    delete rsync;
    rsyncRunning = false;
    syncBut.setEnabled(true);
}

void FileSync::selectSource()
{
    QString dir = QFileDialog::getExistingDirectory(&w, tr("Open Directory"),
                                                  "/home",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        sourceDir = dir;
        QSettings settings;
        settings.setValue("sourceDir", sourceDir);
        settings.sync();
        filetree.setRootIndex(model.setRootPath(sourceDir));
        updateLabel();
        readListFile();
    }

}

void FileSync::selectDest()
{
    QString dir = QFileDialog::getExistingDirectory(&w, tr("Open Directory"),
                                                  "/home",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        destDir = dir;
        QSettings settings;
        settings.setValue("destDir", destDir);
        settings.sync();
        updateLabel();
    }

}

void FileSync::updateLabel()
{
    label.setText(QString("source: %1 dest: %2").arg(sourceDir).arg(destDir));
}
