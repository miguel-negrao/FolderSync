#include "customfilesystemmodel.h"
#include <iostream>
#include <QTextStream>

//from http://www.qtforum.org/article/33153/subclassing-qfilesystemmodel-for-showing-checkboxes.html?s=08e3b1fcefdd34c11327203380f63d80e0b5e055#post106871
//by MatthieuB

using namespace std;

CustomFileSystemModel::CustomFileSystemModel(QObject* parent)
    : QFileSystemModel(parent)
{
    setReadOnly(true);
    setFilter(QDir::AllDirs | QDir::Dirs | QDir::NoDotAndDotDot);
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags f = QFileSystemModel::flags(index);
    if (index.column() == 0) { // make the first column checkable
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

QVariant CustomFileSystemModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
        // the item is checked only if we have stored its path
        if (directories.contains(filePath(index)) || directories.contains(filePath(parent(index)))) {
            return Qt::Checked;
        } else {
            return Qt::Unchecked;
        }
    }
    return QFileSystemModel::data(index, role);
}

bool CustomFileSystemModel::setData(const QModelIndex& i, const QVariant& value, int role)
{
    if (i.isValid() && i.column() == 0 && role == Qt::CheckStateRole) {
        // store checked paths, remove unchecked paths
        if (isDir(i)) {
            if (value.toInt() == Qt::Checked) {
                // if we check a top level item then do not store children
                if (!directories.contains(filePath(parent(i)))) {
                    directories << filePath(i);
                }
            } else {
                directories.removeAll(filePath(i));
            }
            directories.sort();
            QModelIndex sib = i.sibling(i.row()+1, 0);
            emit dataChanged(i, sib);
        }
        return true;
    }
    return QFileSystemModel::setData(i, value, role);
}
