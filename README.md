# FolderSync

Selective folder sync using rsync 

The program will copy all folders selected in the tree view to a destination folder, maintaining the folder structure.

Using the program:

* Select the source directory.
* Select the destination directory.
* Select folders to copy in the tree view.
* The 'Sync' button will copy the selected folders using rsync.
* The folder list is saved as 'folderlist.txt' in the source directory.
* The folder list is saved on program exit or on hitting the 'Save' button.

## Getting Started


### Prerequisites

Qt (>= 5.0)
rsync

### Installing

The project should build with QtCreator

## Authors

* **Miguel Negrão** - [github](https://github.com/miguel-negrao)

## License

This project is licensed under the GPL v3 license

## Acknowledgments

* Code for the check boxes in the tree view came from [here](http://www.qtforum.org/article/33153/subclassing-qfilesystemmodel-for-showing-checkboxes.html?s=08e3b1fcefdd34c11327203380f63d80e0b5e055#post106871).

