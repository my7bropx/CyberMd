#ifndef FILETREE_H
#define FILETREE_H

#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QToolButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <qtreewidget.h>
class Theme;

// File type enumeration for icon selection
enum class FileType {
  Folder,
  FolderOpen,
  Markdown,
  Cpp,
  Header,
  Python,
  Rust,
  JavaScript,
  TypeScript,
  Html,
  Css,
  Json,
  Yaml,
  Toml,
  Xml,
  Shell,
  Makefile,
  CMake,
  Git,
  Image,
  Document,
  Archive,
  Binary,
  Config,
  Lock,
  License,
  Readme,
  Unknown
};

// Custom tree widget item with file info
class FileTreeItem : public QTreeWidgetItem {
public:
  explicit FileTreeItem(QTreeWidgetItem *parent = nullptr);
  explicit FileTreeItem(QTreeWidget *parent = nullptr);

  void setFileInfo(const QFileInfo &info);
  QFileInfo fileInfo() const { return fileInfo_; }
  QString filePath() const { return fileInfo_.absoluteFilePath(); }
  bool isDirectory() const { return fileInfo_.isDir(); }
  FileType fileType() const { return fileType_; }

private:
  void determineFileType();

  QFileInfo fileInfo_;
  FileType fileType_;
};

// Main file tree widget
class FileTree : public QTreeWidget {
  Q_OBJECT

public:
  explicit FileTree(QWidget *parent = nullptr);
  ~FileTree();

  void setRootPath(const QString &path);
  QString rootPath() const { return currentPath_; }
  QString currentFilePath() const;

  void setTheme(Theme *theme);
  void refresh();

  // Filter options
  void setShowHiddenFiles(bool show);
  bool showHiddenFiles() const { return showHidden_; }
  void setFilter(const QString &filter);
  void setFilterPattern(const QString &filter);
  // Selection
  void selectFile(const QString &filePath);
  void expandToFile(const QString &filePath);

signals:
  void fileSelected(const QString &filePath);
  void fileDoubleClicked(const QString &filePath);
  void folderSelected(const QString &folderPath);
  void contextMenuRequested(const QString &path, const QPoint &pos);
  void directoryChanged(const QString &path);
private slots:
  void onItemClicked(QTreeWidgetItem *item, int column);
  void onItemDoubleClicked(QTreeWidgetItem *item, int column);
  void onItemExpanded(QTreeWidgetItem *item);
  void onItemCollapsed(QTreeWidgetItem *item);
  void onCustomContextMenu(const QPoint &pos);
  void refreshTree();
  void navigateUp();
  void navigateHome();
  void onFilterChanged(const QString &text);
  void onDirectoryChanged(const QString &path);
  void onLoadDirectory(const QString &path);

private:
  void setupUI();
  void setupConnections();
  void populateTree(const QString &path);
  void populateItem(FileTreeItem *item);
  void addDirectoryItems(QTreeWidgetItem *parentItem, const QString &path);
  void updateItemIcon(FileTreeItem *item);
  void applyFilter(FileTreeItem *item, const QString &filter);
  void loadDirectory(const QString &path, QTreeWidgetItem *parent);

  // Icon generation - use QString path to match .cpp implementation
  QIcon getFileIcon(const QString &filePath);

  // Path display
  void updatePathDisplay();
  QStringList getPathBreadcrumbs() const;

  // UI Components
  QTreeWidget *treeWidget_;
  QLineEdit *pathEdit_;
  QLabel *pathLabel_;
  QWidget *breadcrumbWidget_;
  QHBoxLayout *breadcrumbLayout_;
  QToolButton *homeButton_;
  QToolButton *settingsButton_;
  QPushButton *refreshButton_;
  QPushButton *upButton_;
  // State
  QString currentPath_;
  QString filterpattern_;
  bool showHidden_;
  Theme *theme_;

  // File watching
  QFileSystemWatcher *watcher_;
  QTimer *refreshTimer_;

  // Icon cache
  mutable QMap<FileType, QIcon> iconCache_;
};

#endif // FILETREE_H
