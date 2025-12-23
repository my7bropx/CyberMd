#include "tabwidget.h"
#include "codeeditor.h"
#include "syntaxhighlighter.h"
#include "theme.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QStringConverter>
#include <QStyleOption>
#include <QTextStream>
// =================== EditorTabBar ====================

EditorTabBar::EditorTabBar(QWidget *parent) : QTabBar(parent), theme_(nullptr) {
  setTabsClosable(true);
  setMovable(true);
  setExpanding(false);
  setDocumentMode(true);
  setElideMode(Qt::ElideRight);
}

void EditorTabBar::setTheme(Theme *theme) {
  theme_ = theme;
  update();
}

void EditorTabBar::paintEvent(QPaintEvent *event) {
  // Use default painting if no theme
  if (!theme_) {
    QTabBar::paintEvent(event);
    return;
  }

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Draw background
  painter.fillRect(rect(), theme_->tabBackground());

  // Draw each tab
  for (int i = 0; i < count(); ++i) {
    QRect tabRect = this->tabRect(i);
    bool isActive = (i == currentIndex());
    bool isHovered = tabRect.contains(mapFromGlobal(QCursor::pos()));

    // Tab background
    QColor bgColor;
    if (isActive) {
      bgColor = theme_->tabActiveBackground();
    } else if (isHovered) {
      bgColor = theme_->tabHoverBackground();
    } else {
      bgColor = theme_->tabBackground();
    }
    painter.fillRect(tabRect, bgColor);

    // Tab border (bottom line for active tab)
    if (isActive) {
      painter.setPen(QPen(theme_->uiAccent(), 2));
      painter.drawLine(tabRect.bottomLeft(), tabRect.bottomRight());
    }

    // Tab text
    QColor textColor =
        isActive ? theme_->tabActiveForeground() : theme_->tabForeground();
    painter.setPen(textColor);

    QString text = tabText(i);
    QRect textRect =
        tabRect.adjusted(10, 0, -30, 0); // Leave space for close button
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    // Modified indicator (dot before text)
    if (tabData(i).toBool()) { // isModified stored in data
      painter.setPen(Qt::NoPen);
      painter.setBrush(theme_->tabModifiedIndicator());
      painter.drawEllipse(tabRect.left() + 4, tabRect.center().y() - 3, 6, 6);
    }

    // Close button area
    QRect closeRect(tabRect.right() - 24, tabRect.center().y() - 8, 16, 16);
    if (closeRect.contains(mapFromGlobal(QCursor::pos()))) {
      painter.setPen(theme_->tabCloseButtonHover());
    } else {
      painter.setPen(theme_->tabCloseButton());
    }

    // Draw X
    int margin = 4;
    painter.drawLine(closeRect.topLeft() + QPoint(margin, margin),
                     closeRect.bottomRight() - QPoint(margin, margin));
    painter.drawLine(closeRect.topRight() + QPoint(-margin, margin),
                     closeRect.bottomLeft() + QPoint(margin, -margin));
  }
}

void EditorTabBar::mousePressEvent(QMouseEvent *event) {
  // Check if click is on close button
  for (int i = 0; i < count(); ++i) {
    QRect tabRect = this->tabRect(i);
    QRect closeRect(tabRect.right() - 24, tabRect.center().y() - 8, 16, 16);

    if (closeRect.contains(event->pos())) {
      emit tabCloseRequested(i);
      return;
    }
  }

  QTabBar::mousePressEvent(event);
}

                     250)); // Add space for modified dot and close button
                     size.setHeight(32); // Fixed height
                     return size;
                     }

                     // ==================== EditorTabWidget
                     // ====================

                     EditorTabWidget::EditorTabWidget(QWidget *parent)
                         : QTabWidget(parent), tabBar_(new EditorTabBar(this)),
                           theme_(nullptr), untitledCounter_(0) {
                       setupUI();
                     }

                     EditorTabWidget::~EditorTabWidget() {
                       // Clean up highlighters
                       for (auto &info : tabInfoMap_) {
                         delete info.highlighter;
                       }
                     }

                     void EditorTabWidget::setupUI() {
                       setTabBar(tabBar_);
                       setTabsClosable(true);
                       setMovable(true);
                       setDocumentMode(true);

                       // Connections
                       connect(this, &QTabWidget::tabCloseRequested, this,
                               &EditorTabWidget::onTabCloseRequested);
                       connect(this, &QTabWidget::currentChanged, this,
                               &EditorTabWidget::onCurrentChanged);
                       connect(tabBar_, &QTabBar::tabMoved, this,
                               &EditorTabWidget::onTabMoved);
                     }

                     int EditorTabWidget::newTab(const QString &filePath) {
                       CodeEditor *editor = new CodeEditor(this);

                       if (theme_) {
                         editor->setTheme(theme_);
                       }

                       TabInfo info;

                       if (filePath.isEmpty()) {
                         // New untitled file
                         info.isUntitled = true;
                         info.fileName =
                             QString("Untitled-%1").arg(++untitledCounter_);
                       } else {
                         // Existing file
                         QFile file(filePath);
                         if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                           QTextStream stream(&file);
                           stream.setEncoding(QStringConverter::Utf8);
                           editor->setPlainText(stream.readAll());
                           file.close();
                         }

                         info.isUntitled = false;
                         info.filePath = filePath;
                         info.fileName = QFileInfo(filePath).fileName();
                         editor->setFilePath(filePath);
                       }

                       info.isModified = false;

                       int index = addTab(editor, info.fileName);
                       tabInfoMap_[index] = info;

                       // Connect text changed signal
                       connect(editor->document(),
                               &QTextDocument::modificationChanged,
                               [this, editor](bool changed) {
                                 int idx = indexOf(editor);
                                 if (idx >= 0) {
                                   tabInfoMap_[idx].isModified = changed;
                                   updateTabTitle(idx);
                                   emit tabModified(idx, changed);
                                 }
                               });

                       // Apply highlighter
                       applyHighlighter(index, filePath);

                       setCurrentIndex(index);
                       emit tabCountChanged(count());

                       return index;
                     }

                     bool EditorTabWidget::openFile(const QString &filePath) {
                       // Check if file is already open
                       int existingIndex = findTabByPath(filePath);
                       if (existingIndex >= 0) {
                         setCurrentIndex(existingIndex);
                         return true;
                       }

                       // Check if file exists
                       if (!QFile::exists(filePath)) {
                         QMessageBox::warning(
                             this, "Error",
                             QString("File not found: %1").arg(filePath));
                         return false;
                       }

                       // If current tab is untitled and unmodified, use it
                       if (count() > 0) {
                         int current = currentIndex();
                         if (tabInfoMap_.contains(current) &&
                             tabInfoMap_[current].isUntitled &&
                             !tabInfoMap_[current].isModified) {
                           // Load into current tab
                           CodeEditor *editor = currentEditor();
                           QFile file(filePath);
                           if (file.open(QIODevice::ReadOnly |
                                         QIODevice::Text)) {
                             QTextStream stream(&file);
                             stream.setEncoding(QStringConverter::Utf8);
                             editor->setPlainText(stream.readAll());
                             file.close();

                             TabInfo &info = tabInfoMap_[current];
                             info.isUntitled = false;
                             info.filePath = filePath;
                             info.fileName = QFileInfo(filePath).fileName();
                             info.isModified = false;
                             editor->setFilePath(filePath);
                             editor->document()->setModified(false);

                             updateTabTitle(current);
                             applyHighlighter(current, filePath);

                             emit currentFileChanged(filePath);
                             return true;
                           }
                         }
                       }

                       // Create new tab
                       newTab(filePath);
                       return true;
                     }

                     bool EditorTabWidget::saveTab(int index) {
                       if (index < 0)
                         index = currentIndex();
                       if (index < 0 || !tabInfoMap_.contains(index))
                         return false;

                       TabInfo &info = tabInfoMap_[index];

                       if (info.isUntitled) {
                         return saveTabAs(index);
                       }

                       CodeEditor *editor = editorAt(index);
                       if (!editor)
                         return false;

                       QFile file(info.filePath);
                       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                         QMessageBox::critical(this, "Error",
                                               QString("Cannot save file: %1")
                                                   .arg(info.filePath));
                         return false;
                       }

                       QTextStream stream(&file);
                       stream.setEncoding(QStringConverter::Utf8);
                       stream << editor->toPlainText();
                       file.close();

                       editor->document()->setModified(false);
                       info.isModified = false;
                       updateTabTitle(index);

                       return true;
                     }

                     bool EditorTabWidget::saveTabAs(int index) {
                       if (index < 0)
                         index = currentIndex();
                       if (index < 0 || !tabInfoMap_.contains(index))
                         return false;

                       CodeEditor *editor = editorAt(index);
                       if (!editor)
                         return false;

                       QString filePath = QFileDialog::getSaveFileName(
                           this, "Save File As", tabInfoMap_[index].filePath,
                           "All Files (*);;Markdown (*.md);;Text (*.txt)");

                       if (filePath.isEmpty())
                         return false;

                       QFile file(filePath);
                       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                         QMessageBox::critical(
                             this, "Error",
                             QString("Cannot save file: %1").arg(filePath));
                         return false;
                       }

                       QTextStream stream(&file);
                       stream.setEncoding(QStringConverter::Utf8);
                       stream << editor->toPlainText();
                       file.close();

                       TabInfo &info = tabInfoMap_[index];
                       info.filePath = filePath;
                       info.fileName = QFileInfo(filePath).fileName();
                       info.isUntitled = false;
                       info.isModified = false;
                       editor->setFilePath(filePath);
                       editor->document()->setModified(false);

                       updateTabTitle(index);
                       applyHighlighter(index, filePath);

                       emit currentFileChanged(filePath);

                       return true;
                     }

                     bool EditorTabWidget::saveAllTabs() {
                       for (int i = 0; i < count(); ++i) {
                         if (tabInfoMap_.contains(i) &&
                             tabInfoMap_[i].isModified) {
                           if (!saveTab(i))
                             return false;
                         }
                       }
                       return true;
                     }

                     bool EditorTabWidget::closeTab(int index) {
                       if (index < 0 || index >= count())
                         return false;

                       if (tabInfoMap_.contains(index) &&
                           tabInfoMap_[index].isModified) {
                         QMessageBox::StandardButton reply =
                             QMessageBox::question(
                                 this, "Save Changes",
                                 QString("Save changes to %1?")
                                     .arg(tabInfoMap_[index].fileName),
                                 QMessageBox::Save | QMessageBox::Discard |
                                     QMessageBox::Cancel);

                         if (reply == QMessageBox::Save) {
                           if (!saveTab(index))
                             return false;
                         } else if (reply == QMessageBox::Cancel) {
                           return false;
                         }
                       }

                       // Clean up highlighter
                       if (tabInfoMap_.contains(index) &&
                           tabInfoMap_[index].highlighter) {
                         delete tabInfoMap_[index].highlighter;
                       }
                       tabInfoMap_.remove(index);

                       // Remove tab
                       removeTab(index);

                       // Renumber remaining tabs in map
                       QMap<int, TabInfo> newMap;
                       for (auto it = tabInfoMap_.begin();
                            it != tabInfoMap_.end(); ++it) {
                         int newIndex = it.key();
                         if (newIndex > index)
                           newIndex--;
                         newMap[newIndex] = it.value();
                       }
                       tabInfoMap_ = newMap;

                       emit tabCountChanged(count());

                       return true;
                     }

                     bool EditorTabWidget::closeAllTabs() {
                       while (count() > 0) {
                         if (!closeTab(0))
                           return false;
                       }
                       return true;
                     }

                     CodeEditor *EditorTabWidget::currentEditor() const {
                       return qobject_cast<CodeEditor *>(currentWidget());
                     }

                     CodeEditor *EditorTabWidget::editorAt(int index) const {
                       return qobject_cast<CodeEditor *>(widget(index));
                     }

                     QString EditorTabWidget::currentFilePath() const {
                       int index = currentIndex();
                       if (index >= 0 && tabInfoMap_.contains(index)) {
                         return tabInfoMap_[index].filePath;
                       }
                       return QString();
                     }

                     QString EditorTabWidget::filePathAt(int index) const {
                       if (tabInfoMap_.contains(index)) {
                         return tabInfoMap_[index].filePath;
                       }
                       return QString();
                     }

                     bool EditorTabWidget::isModified(int index) const {
                       if (index < 0)
                         index = currentIndex();
                       if (tabInfoMap_.contains(index)) {
                         return tabInfoMap_[index].isModified;
                       }
                       return false;
                     }

                     bool EditorTabWidget::hasUnsavedChanges() const {
                       for (const auto &info : tabInfoMap_) {
                         if (info.isModified)
                           return true;
                       }
                       return false;
                     }

                     int EditorTabWidget::findTabByPath(
                         const QString &filePath) const {
                       for (auto it = tabInfoMap_.begin();
                            it != tabInfoMap_.end(); ++it) {
                         if (it->filePath == filePath) {
                           return it.key();
                         }
                       }
                       return -1;
                     }

                     void EditorTabWidget::setTheme(Theme *theme) {
                       theme_ = theme;
                       tabBar_->setTheme(theme);

                       // Apply theme to all editors
                       for (int i = 0; i < count(); ++i) {
                         CodeEditor *editor = editorAt(i);
                         if (editor) {
                           editor->setTheme(theme);
                         }

                         // Apply theme to highlighters
                         if (tabInfoMap_.contains(i) &&
                             tabInfoMap_[i].highlighter) {
                           tabInfoMap_[i].highlighter->setTheme(theme);
                           tabInfoMap_[i].highlighter->rehighlight();
                         }
                       }

                       // Apply stylesheet
                       if (theme) {
                         setStyleSheet(theme->getTabWidgetStyleSheet());
                       }
                     }

                     void EditorTabWidget::applyHighlighter(
                         int index, const QString &filePath) {
                       CodeEditor *editor = editorAt(index);
                       if (!editor)
                         return;

                       // Remove old highlighter
                       if (tabInfoMap_.contains(index) &&
                           tabInfoMap_[index].highlighter) {
                         delete tabInfoMap_[index].highlighter;
                         tabInfoMap_[index].highlighter = nullptr;
                       }

                       // Create new highlighter
                       BaseSyntaxHighlighter *highlighter =
                           HighlighterFactory::createHighlighterForFile(
                               filePath, editor->document());

                       if (highlighter) {
                         if (theme_) {
                           highlighter->setTheme(theme_);
                         }
                         tabInfoMap_[index].highlighter = highlighter;
                       }
                     }

                     void EditorTabWidget::updateTabTitle(int index) {
                       if (!tabInfoMap_.contains(index))
                         return;

                       tabInfoMap_[index].highlighter = highlighter;
                     }
                     }

                     void EditorTabWidget::updateTabTitle(int index) {
                       if (!tabInfoMap_.contains(index))
                         return;

                       const TabInfo &info = tabInfoMap_[index];
                       QString title = info.fileName;
                       if (info.isModified) {
                         title = "● " + title;
                       }

                       setTabText(index, title);
                       tabBar_->setTabData(index, info.isModified);
                       tabBar_->update();
                     }

                     void EditorTabWidget::onTextChanged() {
                       // Handled by document's modificationChanged signal
                     }

                     void EditorTabWidget::onTabCloseRequested(int index) {
                       closeTab(index);
                     }

                     void EditorTabWidget::onCurrentChanged(int index) {
                       if (index >= 0 && tabInfoMap_.contains(index)) {
                         emit currentFileChanged(tabInfoMap_[index].filePath);
                         emit editorChanged(editorAt(index));
                       }
                     }

                     void EditorTabWidget::onTabMoved(int from, int to) {
                       // Update tab info map
                       TabInfo fromInfo = tabInfoMap_[from];

                       if (from < to) {
                         for (int i = from; i < to; ++i) {
                           tabInfoMap_[i] = tabInfoMap_[i + 1];
                         }
                       } else {
                         for (int i = from; i > to; --i) {
                           tabInfoMap_[i] = tabInfoMap_[i - 1];
                         }
                       }

                       tabInfoMap_[to] = fromInfo;
                     }

                     QString EditorTabWidget::getLanguageFromPath(
                         const QString &filePath) {
                       QString suffix = QFileInfo(filePath).suffix().toLower();

                       static const QMap<QString, QString> langMap = {
                           {"md", "markdown"},   {"markdown", "markdown"},
                           {"cpp", "cpp"},       {"cxx", "cpp"},
                           {"cc", "cpp"},        {"c", "cpp"},
                           {"h", "cpp"},         {"hpp", "cpp"},
                           {"hxx", "cpp"},       {"py", "python"},
                           {"pyw", "python"},    {"rs", "rust"},
                           {"js", "javascript"}, {"jsx", "javascript"},
                           {"ts", "typescript"}, {"tsx", "typescript"},
                           {"json", "json"},     {"yaml", "yaml"},
                           {"yml", "yaml"},      {"html", "html"},
                           {"htm", "html"},      {"css", "css"},
                           {"scss", "css"},      {"sass", "css"},
                           {"sh", "shell"},      {"bash", "shell"},
                           {"zsh", "shell"},     {"toml", "toml"}};

                       return langMap.value(suffix, "text");
                     }

                     QIcon
                     EditorTabWidget::getFileIcon(const QString &filePath) {
                       // Simple icon based on file type - could be enhanced
                       QString lang = getLanguageFromPath(filePath);

                       // Return appropriate icon based on language
                       // For now, return empty icon - can be enhanced with
                       // actual icons
                       return QIcon();
                     }

                     BaseSyntaxHighlighter *
                     EditorTabWidget::createHighlighter(const QString &filePath,
                                                        QTextDocument *doc) {
                       return HighlighterFactory::createHighlighterForFile(
                           filePath, doc);
                     }
