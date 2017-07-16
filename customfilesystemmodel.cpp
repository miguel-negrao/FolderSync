#include "customfilesystemmodel.h"
#include <iostream>
#include <QTextStream>

using namespace std;

CustomFileSystemModel::CustomFileSystemModel(QObject* parent)
    : QFileSystemModel(parent)
{
    setReadOnly(true);
    //setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags f = QFileSystemModel::flags(index);
    if (index.column() == 0) { // make the first column checkable
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

/*
QVariant CustomFileSystemModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
        // the item is checked only if we have stored its path
        if (directories.contains(QFileInfo(filePath(index))) || directories.contains(QFileInfo(filePath(parent(index))))) {
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
                    directories << QFileInfo(filePath(i));
                }
            } else {
                directories.removeAll(QFileInfo(filePath(i)));
            }
            //directories.sort();
            QModelIndex sib = i.sibling(i.row()+1, 0);
            emit dataChanged(i, sib);
        }
        return true;
    }
    return QFileSystemModel::setData(i, value, role);
}
*/

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
