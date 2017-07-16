#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QStringList>
#include <QList>
#include <QFileInfo>

class CustomFileSystemModel: public QFileSystemModel
{
    Q_OBJECT

private:

public:
    QStringList directories;
    //QList<QFileInfo> directories;

    explicit CustomFileSystemModel(QObject* parent = Q_NULLPTR);
    inline QStringList getCheckedDirectories() const { return directories; }

    Qt::ItemFlags flags(const QModelIndex& index) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

public slots:

};

#endif // CUSTOMFILESYSTEMMODEL_H
