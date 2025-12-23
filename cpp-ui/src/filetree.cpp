#include "filetree.h"
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStyle>
#include <QUrl>

FileTree::FileTree(QWidget *parent) : QTreeWidget(parent) {
  setupUI();
  setRootPath(QDir::homePath());
}

FileTree::~FileTree() {}

void FileTree::onItemExpanded(QTreeWidgetItem *item) {
  if (!item)
    return;

  QString path = item->data(0, Qt::UserRole).toString();

  // Check if we need to load children (lazy loading)
  if (item->childCount() == 1 && item->child(0)->text(0).isEmpty()) {
    delete item->takeChild(0); // Remove placeholder
    void loadDirectory(const QString &path, QTreeWidgetItem *parent);
  }

  // Update folder icon to open state
  item->setIcon(
      0, QIcon::fromTheme("folder-open", QIcon(":/icons/folder-open. svg")));
}

void FileTree::onItemCollapsed(QTreeWidgetItem *item) {
  if (!item)
    return;

  // Update folder icon to closed state
  item->setIcon(0, QIcon::fromTheme("folder", QIcon(":/icons/folder.svg")));
}

void FileTree::onCustomContextMenu(const QPoint &pos) {
  QTreeWidgetItem *item = itemAt(pos);
  QMenu menu(this);

  if (item) {
    QString path = item->data(0, Qt::UserRole).toString();
    QFileInfo fileInfo(path);

    if (fileInfo.isDir()) {
      menu.addAction("New File.. .", [this, path]() {
        bool ok;
        QString fileName = QInputDialog::getText(
            this, "New File", "File name:", QLineEdit::Normal, "", &ok);
        if (ok && !fileName.isEmpty()) {
          QFile file(path + "/" + fileName);
          if (file.open(QIODevice::WriteOnly)) {
            file.close();
            refresh();
          }
        }
      });

      menu.addAction("New Folder.. .", [this, path]() {
        bool ok;
        QString folderName = QInputDialog::getText(
            this, "New Folder", "Folder name:", QLineEdit::Normal, "", &ok);
        if (ok && !folderName.isEmpty()) {
          QDir dir(path);
          dir.mkdir(folderName);
          refresh();
        }
      });
      menu.addSeparator();
    }

    menu.addAction("Rename.. .", [this, item, path]() {
      bool ok;
      QString newName = QInputDialog::getText(
          this, "Rename", "New name:", QLineEdit::Normal, item->text(0), &ok);
      if (ok && !newName.isEmpty()) {
        QFileInfo fi(path);
        QString newPath = fi.dir().filePath(newName);
        QFile::rename(path, newPath);
        refresh();
      }
    });

    menu.addAction("Delete", [this, path]() {
      QFileInfo fi(path);
      if (QMessageBox::question(this, "Delete",
                                QString("Delete '%1'?").arg(fi.fileName())) ==
          QMessageBox::Yes) {
        if (fi.isDir()) {
          QDir(path).removeRecursively();
        } else {
          QFile::remove(path);
        }
        refresh();
      }
    });

    menu.addSeparator();
    menu.addAction("Copy Path",
                   [path]() { QGuiApplication::clipboard()->setText(path); });
  } else {
    menu.addAction("Refresh", this, &FileTree::refresh);
  }

  menu.exec(mapToGlobal(pos));
}

void FileTree::navigateHome() { setRootPath(QDir::homePath()); }

void FileTree::onFilterChanged(const QString &filter) {
  filterpattern_ = filter;
  refresh();
}

void FileTree::onDirectoryChanged(const QString &path) { setRootPath(path); }

void FileTree::loadDirectory(const QString &path, QTreeWidgetItem *parent) {
  QDir dir(path);
  QFileInfoList entries = dir.entryInfoList(
      QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

  for (const QFileInfo &entry : entries) {
    // Apply filter
    if (!filterpattern_.isEmpty() && !entry.isDir()) {
      if (!entry.fileName().contains(filterpattern_, Qt::CaseInsensitive)) {
        continue;
      }
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, entry.fileName());
    item->setData(0, Qt::UserRole, entry.absoluteFilePath());

    if (entry.isDir()) {
      item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
      item->setData(0, Qt::UserRole + 1, "folder");
      // Add placeholder for lazy loading
      QTreeWidgetItem *placeholder = new QTreeWidgetItem();
      item->addChild(placeholder);
    } else {
      item->setIcon(0, getFileIcon(entry.absoluteFilePath()));
      item->setData(0, Qt::UserRole + 1, "file");

      // Format file size
      qint64 size = entry.size();
      QString sizeStr;
      if (size < 1024) {
        sizeStr = QString("%1 B").arg(size);
      } else if (size < 1024 * 1024) {
        sizeStr = QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
      } else {
        sizeStr = QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 1);
      }
      item->setText(1, sizeStr);
    }

    if (parent) {
      parent->addChild(item);
    } else {
      addTopLevelItem(item);
    }
  }
}

void FileTree::refresh() { populateTree(currentPath_); }

void FileTree::setupUI() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(5);
  mainLayout->setContentsMargins(5, 5, 5, 5);

  // Top navigation bar
  QHBoxLayout *navLayout = new QHBoxLayout();

  upButton_ = new QPushButton("↑", this);
  upButton_->setMaximumWidth(30);
  upButton_->setToolTip("Go to parent directory");
  connect(upButton_, &QPushButton::clicked, this, &FileTree::navigateUp);
  navLayout->addWidget(upButton_);

  pathEdit_ = new QLineEdit(this);
  pathEdit_->setReadOnly(true);
  navLayout->addWidget(pathEdit_);

  refreshButton_ = new QPushButton("⟳", this);
  refreshButton_->setMaximumWidth(30);
  refreshButton_->setToolTip("Refresh");
  connect(refreshButton_, &QPushButton::clicked, this, &FileTree::refreshTree);
  navLayout->addWidget(refreshButton_);

  mainLayout->addLayout(navLayout);

  // Tree widget
  treeWidget_ = new QTreeWidget(this);
  treeWidget_->setHeaderLabels(QStringList() << "Name" << "Size");
  treeWidget_->setColumnWidth(0, 200);
  treeWidget_->setAlternatingRowColors(true);
  treeWidget_->setAnimated(true);
  treeWidget_->setIndentation(20);

  connect(treeWidget_, &QTreeWidget::itemClicked, this,
          &FileTree::onItemClicked);
  connect(treeWidget_, &QTreeWidget::itemDoubleClicked, this,
          &FileTree::onItemDoubleClicked);

  mainLayout->addWidget(treeWidget_);

  treeWidget_->setAlternatingRowColors(false);
  treeWidget_->header()->setStretchLastSection(false);
  treeWidget_->header()->setSectionResizeMode(0, QHeaderView::Stretch);
  treeWidget_->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void FileTree::setRootPath(const QString &path) {
  currentPath_ = path;
  pathEdit_->setText(path);
  populateTree(path);
  emit directoryChanged(path);
}

void FileTree::populateTree(const QString &path) {
  treeWidget_->clear();

  QDir dir(path);
  if (!dir.exists()) {
    return;
  }

  // Add folders first
  QFileInfoList folders =
      dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  for (const QFileInfo &folderInfo : folders) {
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget_);
    item->setText(0, folderInfo.fileName());
    item->setText(1, "");
    item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    item->setData(0, Qt::UserRole, folderInfo.absoluteFilePath());
    item->setData(0, Qt::UserRole + 1, "folder");
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
  }

  // Add files
  QFileInfoList files = dir.entryInfoList(QDir::Files, QDir::Name);
  for (const QFileInfo &fileInfo : files) {
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget_);
    item->setText(0, fileInfo.fileName());

    qint64 size = fileInfo.size();
    QString sizeStr;
    if (size < 1024) {
      sizeStr = QString("%1 B").arg(size);
    } else if (size < 1024 * 1024) {
      sizeStr = QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
    } else {
      sizeStr = QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 1);
    }
    item->setText(1, sizeStr);

    item->setIcon(0, getFileIcon(fileInfo.absoluteFilePath()));
    item->setData(0, Qt::UserRole, fileInfo.absoluteFilePath());
    item->setData(0, Qt::UserRole + 1, "file");
  }
}

QIcon FileTree::getFileIcon(const QString &filePath) {
  QFileInfo fileInfo(filePath);
  QString suffix = fileInfo.suffix().toLower();

  if (suffix == "md" || suffix == "markdown") {
    return style()->standardIcon(QStyle::SP_FileDialogDetailedView);
  } else if (suffix == "cpp" || suffix == "h" || suffix == "c" ||
             suffix == "hpp") {
    return style()->standardIcon(QStyle::SP_FileIcon);
  } else if (suffix == "txt") {
    return style()->standardIcon(QStyle::SP_FileDialogContentsView);
  } else if (suffix == "sh" || suffix == "bash") {
    return style()->standardIcon(QStyle::SP_CommandLink);
  } else {
    return style()->standardIcon(QStyle::SP_FileIcon);
  }
}

void FileTree::addDirectoryItems(QTreeWidgetItem *parentItem,
                                 const QString &path) {
  QDir dir(path);
  if (!dir.exists()) {
    return;
  }

  QFileInfoList folders =
      dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  for (const QFileInfo &folderInfo : folders) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(0, folderInfo.fileName());
    item->setText(1, "");
    item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    item->setData(0, Qt::UserRole, folderInfo.absoluteFilePath());
    item->setData(0, Qt::UserRole + 1, "folder");
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
  }

  QFileInfoList files = dir.entryInfoList(QDir::Files, QDir::Name);
  for (const QFileInfo &fileInfo : files) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(0, fileInfo.fileName());

    qint64 size = fileInfo.size();
    QString sizeStr;
    if (size < 1024) {
      sizeStr = QString::number(size) + " B";
    } else if (size < 1024 * 1024) {
      sizeStr = QString::number(size / 1024.0, 'f', 1) + " KB";
    } else {
      sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 1) + " MB";
    }
    item->setText(1, sizeStr);

    item->setIcon(0, getFileIcon(fileInfo.absoluteFilePath()));
    item->setData(0, Qt::UserRole, fileInfo.absoluteFilePath());
    item->setData(0, Qt::UserRole + 1, "file");
  }
}

void FileTree::onItemClicked(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column);

  QString path = item->data(0, Qt::UserRole).toString();
  QString type = item->data(0, Qt::UserRole + 1).toString();

  if (type == "folder") {
    emit folderSelected(path);
  } else if (type == "file") {
    emit fileSelected(path);
  }
}

void FileTree::onItemDoubleClicked(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column);

  QString path = item->data(0, Qt::UserRole).toString();
  QString type = item->data(0, Qt::UserRole + 1).toString();

  if (type == "folder") {
    setRootPath(path);
  } else if (type == "file") {
    emit fileSelected(path);
  }
}

void FileTree::refreshTree() { populateTree(currentPath_); }

void FileTree::navigateUp() {
  QDir dir(currentPath_);
  if (dir.cdUp()) {
    setRootPath(dir.absolutePath());
  }
}

QString FileTree::currentFilePath() const {
  QTreeWidgetItem *item = treeWidget_->currentItem();
  if (item) {
    return item->data(0, Qt::UserRole).toString();
  }
  return QString();
}
void FileTree::onLoadDirectory(const QString &path) {
  loadDirectory(path, nullptr);
}
