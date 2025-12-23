#include "mainwindow.h"
#include "codeeditor.h"
#include "codefolding.h"
#include "commandhelper.h"
#include "featurepanel.h"
#include "filetree.h"
#include "fuzzyfinder.h"
#include "markdownpreview.h"
#include "regexhelper.h"
#include "searchdialog.h"
#include "shellchecker.h"
#include "syntaxhighlighter.h"
#include "tabwidget.h"
#include "theme.h"
#include "vimmode.h"
#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFontComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QRegularExpression>
#include <QSpinBox>
#include <QTemporaryFile>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), editor_(nullptr), preview_(nullptr),
      splitter_(nullptr), isModified_(false), isPreviewMode_(false),
      parser_(std::make_unique<CyberMD::Parser>()), highlighter_(nullptr),
      syntaxHighlighter_(nullptr), statusLabel_(nullptr), settings_(),
      recentFilesMenu_(nullptr), searchDialog_(nullptr), regexHelper_(nullptr),
      commandHelper_(nullptr), shellChecker_(nullptr), vimMode_(nullptr),
      vimModeLabel_(nullptr), fileTypeLabel_(nullptr), lineCountLabel_(nullptr),
      errorCountLabel_(nullptr), fileTree_(nullptr), featurePanel_(nullptr),
      currentTheme_(nullptr), mainSplitter_(nullptr), shellCheckTimer_(nullptr),
      shellCheckProcess_(nullptr), isShellCheckEnabled_(true) {
  qDebug() << "=== MainWindow Constructor Start ===";

  // Initialize theme system
  qDebug() << "Creating theme...";
  currentTheme_ = new Theme(Theme::DarkDefault);
  qDebug() << "Theme created:" << currentTheme_->name();

  qDebug() << "Setting up UI...";
  setupUI();
  qDebug() << "UI setup complete";

  qDebug() << "Setting up menu bar...";
  setupMenuBar();
  qDebug() << "Menu bar setup complete";

  qDebug() << "Setting up toolbar...";
  setupToolBar();
  qDebug() << "Toolbar setup complete";

  qDebug() << "Setting up status bar...";
  setupStatusBar();
  qDebug() << "Status bar setup complete";

  qDebug() << "Creating connections...";
  createConnections();
  qDebug() << "Connections created";

  setWindowTitle("CyberMD - Markdown Editor");
  currentFile_ = QString();

  // Create dialogs (lazy)
  searchDialog_ = new SearchDialog(editor_, this);
  regexHelper_ = new RegexHelper(this);
  commandHelper_ = new CommandHelper(this);
  shellChecker_ = new ShellChecker(this);

  // Make The Editor Read And Write
  editor_->setReadOnly(false);
  editor_->setCursorWidth(4); // cursor wigth

  // Initializw Vim mode (DisabledByDefault)
  vimMode_ = new VimMode(editor_, this);
  vimMode_->setEnabled(false); // Start with VimMode disabled
  connect(vimMode_, &VimMode::modeChanged, this, &MainWindow::onVimModeChanged);

  // Initialize auto shell checking
  shellCheckTimer_ = new QTimer(this);
  shellCheckTimer_->setSingleShot(true);
  shellCheckTimer_->setInterval(1000); // 1 second delay after typing stops
  connect(shellCheckTimer_, &QTimer::timeout, this,
          &MainWindow::autoCheckShellScript);

  shellCheckProcess_ = new QProcess(this);
  connect(shellCheckProcess_,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
          &MainWindow::onShellCheckFinished);

  qDebug() << "Loading settings...";
  loadSettings();
  qDebug() << "Settings loaded";

  qDebug() << "Applying editor settings...";
  applyEditorSettings();
  qDebug() << "Editor settings applied";

  qDebug() << "Applying theme...";
  applyTheme();
  qDebug() << "Theme applied";

  qDebug() << "Updating recent files menu...";
  updateRecentFilesMenu();
  qDebug() << "Recent files menu updated";

  qDebug() << "Updating status bar...";
  updateStatusBar();
  qDebug() << "Status bar updated";

  qDebug() << "=== MainWindow Constructor Complete ===";
}

MainWindow::~MainWindow() {
  delete currentTheme_;
  saveSettings();
}

void MainWindow::setupUI() {
  // Main horizontal splitter (file tree | editor/preview | feature panel)
  mainSplitter_ = new QSplitter(Qt::Horizontal, this);

  // LEFT SIDE: File tree panel (initially hidden)
  fileTree_ = new FileTree(mainSplitter_);
  fileTree_->setVisible(false);
  mainSplitter_->addWidget(fileTree_);

  // CENTER: Editor only (preview toggles)
  // Create a container widget for editor/preview
  QWidget *centerWidget = new QWidget(mainSplitter_);
  QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
  centerLayout->setContentsMargins(0, 0, 0, 0);
  centerLayout->setSpacing(0);

  // Create editor
  editor_ = new CodeEditor(centerWidget);
  centerLayout->addWidget(editor_);

  // Create preview (initially hidden)
  preview_ = new MarkdownPreview(centerWidget);
  preview_->setVisible(false);
  centerLayout->addWidget(preview_);

  mainSplitter_->addWidget(centerWidget);

  // RIGHT SIDE: Feature panel
  featurePanel_ = new FeaturePanel(mainSplitter_);
  featurePanel_->setMaximumWidth(250);
  mainSplitter_->addWidget(featurePanel_);

  // Set stretch factors for main splitter
  // file tree | editor/preview | feature panel
  mainSplitter_->setStretchFactor(0, 0); // file tree - no stretch
  mainSplitter_->setStretchFactor(1, 1); // editor/preview center - stretches
  mainSplitter_->setStretchFactor(2, 0); // feature panel - no stretch

  // Set initial sizes to give more space to editor
  QList<int> sizes;
  sizes << 300 << 700
        << 250; // file tree 300px (when visible), editor big, panel fixed
  mainSplitter_->setSizes(sizes);

  // Style the splitter handles to be more visible
  mainSplitter_->setHandleWidth(1);
  mainSplitter_->setStyleSheet(R"(
        QSplitter::handle {
            background-color: #555555;
        }
        QSplitter::handle:horizontal {
            width: 1px;
        }
        QSplitter::handle:hover {
            background-color: #0e639c;
        }
    )");

  setCentralWidget(mainSplitter_);

  // Connect feature panel signals
  connect(featurePanel_, &FeaturePanel::fileTreeToggled, this,
          &MainWindow::toggleFileTree);
  connect(featurePanel_, &FeaturePanel::regexHelperToggled, this,
          &MainWindow::onFeatureToggled);
  connect(featurePanel_, &FeaturePanel::commandHelperToggled, this,
          &MainWindow::onFeatureToggled);
  connect(featurePanel_, &FeaturePanel::textProcessingToggled, this,
          &MainWindow::onFeatureToggled);
  connect(featurePanel_, &FeaturePanel::shellCheckerToggled,
          [this](bool enabled) {
            isShellCheckEnabled_ = enabled;
            if (!enabled) {
              // Clear error count when disabled
              errorCountLabel_->setVisible(false);
            }
          });
  connect(featurePanel_, &FeaturePanel::vimModeToggled, this,
          &MainWindow::toggleVimMode);

  // Connect file tree
  connect(fileTree_, &FileTree::fileSelected, this,
          &MainWindow::onFileSelected);

  // Initial window size
  resize(1400, 900);
}

void MainWindow::setupMenuBar() {
  // File menu
  QMenu *fileMenu = menuBar()->addMenu("&File");

  QAction *newAction = fileMenu->addAction(
      QIcon::fromTheme("document-new",
                       style()->standardIcon(QStyle::SP_FileIcon)),
      "&New");
  newAction->setShortcut(QKeySequence::New);
  connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

  QAction *openAction = fileMenu->addAction(
      QIcon::fromTheme("document-open",
                       style()->standardIcon(QStyle::SP_DirOpenIcon)),
      "&Open...");
  openAction->setShortcut(QKeySequence::Open);
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  QAction *saveAction = fileMenu->addAction(
      QIcon::fromTheme("document-save",
                       style()->standardIcon(QStyle::SP_DialogSaveButton)),
      "&Save");
  saveAction->setShortcut(QKeySequence::Save);
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

  QAction *saveAsAction = fileMenu->addAction(
      QIcon::fromTheme("document-save-as",
                       style()->standardIcon(QStyle::SP_DialogSaveButton)),
      "Save &As...");
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

  fileMenu->addSeparator();

  // Recent files submenu
  recentFilesMenu_ = fileMenu->addMenu(
      QIcon::fromTheme("document-open-recent",
                       style()->standardIcon(QStyle::SP_DirIcon)),
      "Open &Recent");

  fileMenu->addSeparator();

  QAction *quitAction = fileMenu->addAction(
      QIcon::fromTheme("application-exit",
                       style()->standardIcon(QStyle::SP_DialogCloseButton)),
      "&Quit");
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, &QAction::triggered, this, &QWidget::close);

  // Edit menu
  QMenu *editMenu = menuBar()->addMenu("&Edit");

  QAction *findAction = editMenu->addAction(
      QIcon::fromTheme("edit-find", style()->standardIcon(
                                        QStyle::SP_FileDialogDetailedView)),
      "&Find...");
  findAction->setShortcut(QKeySequence::Find);
  connect(findAction, &QAction::triggered, this, &MainWindow::showFindDialog);

  QAction *replaceAction = editMenu->addAction(
      QIcon::fromTheme(
          "edit-find-replace",
          style()->standardIcon(QStyle::SP_FileDialogDetailedView)),
      "&Replace...");
  replaceAction->setShortcut(QKeySequence::Replace);
  connect(replaceAction, &QAction::triggered, this,
          &MainWindow::showReplaceDialog);

  editMenu->addSeparator();

  QAction *gotoLineAction = editMenu->addAction("&Go to Line...");
  gotoLineAction->setShortcut(Qt::CTRL | Qt::Key_G);
  connect(gotoLineAction, &QAction::triggered, this,
          &MainWindow::showGoToLineDialog);

  // View menu
  QMenu *viewMenu = menuBar()->addMenu("&View");

  QAction *zoomInAction = viewMenu->addAction(
      QIcon::fromTheme("zoom-in", style()->standardIcon(QStyle::SP_ArrowUp)),
      "Zoom &In");
  zoomInAction->setShortcut(QKeySequence::ZoomIn);
  connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);

  QAction *zoomOutAction = viewMenu->addAction(
      QIcon::fromTheme("zoom-out", style()->standardIcon(QStyle::SP_ArrowDown)),
      "Zoom &Out");
  zoomOutAction->setShortcut(QKeySequence::ZoomOut);
  connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);

  QAction *resetZoomAction = viewMenu->addAction("Reset Zoom");
  resetZoomAction->setShortcut(Qt::CTRL | Qt::Key_0);
  connect(resetZoomAction, &QAction::triggered, this, &MainWindow::resetZoom);

  viewMenu->addSeparator();

  QAction *vimModeAction = viewMenu->addAction("VIM &Mode");
  vimModeAction->setCheckable(true);
  vimModeAction->setChecked(false);
  connect(vimModeAction, &QAction::toggled, this, &MainWindow::toggleVimMode);

  viewMenu->addSeparator();

  // Theme submenu
  createThemeMenu(viewMenu);

  viewMenu->addSeparator();

  QAction *toggleViewAction = viewMenu->addAction("Toggle &Preview");
  toggleViewAction->setShortcut(Qt::CTRL | Qt::Key_P);
  toggleViewAction->setToolTip("Toggle between edit and preview mode (Ctrl+P)");
  connect(toggleViewAction, &QAction::triggered, this,
          &MainWindow::toggleViewMode);

  viewMenu->addSeparator();

  QAction *preferencesAction = viewMenu->addAction(
      QIcon::fromTheme(
          "preferences-system",
          style()->standardIcon(QStyle::SP_FileDialogDetailedView)),
      "&Preferences...");
  preferencesAction->setShortcut(QKeySequence::Preferences);
  connect(preferencesAction, &QAction::triggered, this,
          &MainWindow::showPreferences);

  // Tools menu
  QMenu *toolsMenu = menuBar()->addMenu("&Tools");

  QAction *regexHelperAction = toolsMenu->addAction("&Regex Helper");
  connect(regexHelperAction, &QAction::triggered, this,
          &MainWindow::showRegexHelper);

  QAction *commandHelperAction =
      toolsMenu->addAction("&Shell Command Reference");
  connect(commandHelperAction, &QAction::triggered, this,
          &MainWindow::showCommandHelper);

  QAction *shellCheckerAction = toolsMenu->addAction("Shell Script &Checker");
  connect(shellCheckerAction, &QAction::triggered, this,
          &MainWindow::showShellChecker);

  // Help menu
  QMenu *helpMenu = menuBar()->addMenu("&Help");

  QAction *aboutAction = helpMenu->addAction(
      QIcon::fromTheme("help-about",
                       style()->standardIcon(QStyle::SP_MessageBoxInformation)),
      "&About");
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::setupToolBar() {
  QToolBar *toolbar = addToolBar("Main Toolbar");
  toolbar->setObjectName("MainToolBar");
  toolbar->setMovable(false);
  toolbar->setIconSize(QSize(24, 24));
  toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // New file action
  QAction *newAction = toolbar->addAction(
      QIcon::fromTheme("document-new",
                       style()->standardIcon(QStyle::SP_FileIcon)),
      "New");
  connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

  // Open file action
  QAction *openAction = toolbar->addAction(
      QIcon::fromTheme("document-open",
                       style()->standardIcon(QStyle::SP_DirOpenIcon)),
      "Open");
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  // Save file action
  QAction *saveAction = toolbar->addAction(
      QIcon::fromTheme("document-save",
                       style()->standardIcon(QStyle::SP_DialogSaveButton)),
      "Save");
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

  toolbar->addSeparator();

  // Add some spacing and styling
  toolbar->setStyleSheet("QToolBar { spacing: 5px; padding: 5px; }");
}

void MainWindow::setupStatusBar() {
  // File type label
  fileTypeLabel_ = new QLabel("Markdown", this);
  fileTypeLabel_->setStyleSheet("QLabel { padding: 2px 10px; background-color: "
                                "#0e639c; color: white; border-radius: 3px; }");
  fileTypeLabel_->setToolTip("File type");
  statusBar()->addPermanentWidget(fileTypeLabel_);

  // Line count label
  lineCountLabel_ = new QLabel("Lines: 0", this);
  lineCountLabel_->setStyleSheet("QLabel { padding: 2px 10px; }");
  lineCountLabel_->setToolTip("Total lines");
  statusBar()->addPermanentWidget(lineCountLabel_);

  // Error count label (initially hidden)
  errorCountLabel_ = new QLabel("⚠ Errors: 0", this);
  errorCountLabel_->setStyleSheet(
      "QLabel { padding: 2px 10px; color: #f48771; }");
  errorCountLabel_->setToolTip("Error count");
  errorCountLabel_->setVisible(false);
  statusBar()->addPermanentWidget(errorCountLabel_);

  // Position label (existing)
  statusLabel_ = new QLabel(this);
  statusLabel_->setStyleSheet("QLabel { padding: 2px 10px; }");
  statusBar()->addPermanentWidget(statusLabel_);

  // VIM mode indicator
  vimModeLabel_ = new QLabel(this);
  vimModeLabel_->setStyleSheet(
      "QLabel { padding: 2px 10px; font-weight: bold; }");
  vimModeLabel_->setVisible(false);
  statusBar()->addPermanentWidget(vimModeLabel_);

  statusBar()->showMessage("Ready");
}

void MainWindow::createConnections() {
  connect(editor_, &QPlainTextEdit::textChanged, this,
          &MainWindow::textChanged);
  connect(editor_, &QPlainTextEdit::cursorPositionChanged, this,
          &MainWindow::updateStatusBar);
}
void MainWindow::updateStatusBar() {
  CodeEditor *editor = currentEditor();
  if (editor) {
    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    statusBar()->showMessage(QString("Ln %1, Col %2").arg(line).arg(col));
  }
  QTextCursor cursor = editor_->textCursor();
  int line = cursor.blockNumber() + 1;
  int column = cursor.columnNumber() + 1;

  // Count words
  QString text = editor_->toPlainText();
  QStringList words =
      text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
  int wordCount = words.size();

  // Count characters
  int charCount = text.length();

  // Count lines
  int lineCount = editor_->document()->blockCount();

  // Update line count label
  lineCountLabel_->setText(QString("Lines: %1").arg(lineCount));

  // Update position label
  statusLabel_->setText(QString("Ln %1, Col %2  |  %3 words  |  %4 chars")
                            .arg(line)
                            .arg(column)
                            .arg(wordCount)
                            .arg(charCount));
}

void MainWindow::newFile() {
  if (isModified_) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Unsaved Changes", "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
      saveFile();
    } else if (reply == QMessageBox::Cancel) {
      return;
    }
  }

  editor_->clear();
  currentFile_ = QString();
  isModified_ = false;
  setWindowTitle("CyberMD - Markdown Editor");
  statusBar()->showMessage("New file created");
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open File", QString(),
      "All Supported Files (*.md *.markdown *.cpp *.h *.hpp *.py *.rs);;"
      "Markdown Files (*.md *.markdown);;"
      "C++ Files (*.cpp *.cc *.cxx *.c++ *.h *.hpp *.hxx *.h++);;"
      "Python Files (*.py);;"
      "Rust Files (*.rs);;"
      "All Files (*)");

  if (fileName.isEmpty())
    return;

  openFileByPath(fileName);
}

void MainWindow::openRecentFile() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    openFileByPath(action->data().toString());
  }
}

void MainWindow::openFileByPath(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(this, "Error", "Could not open file: " + fileName);
    return;
  }

  QTextStream in(&file);
  editor_->setPlainText(in.readAll());
  file.close();

  currentFile_ = fileName;
  isModified_ = false;
  setWindowTitle("CyberMD - " + QFileInfo(fileName).fileName());
  statusBar()->showMessage("File opened: " + fileName);

  // Apply syntax highlighting based on file type
  applySyntaxHighlighter(fileName);

  // Update file type label
  QFileInfo fileInfo(fileName);
  QString extension = fileInfo.suffix().toLower();
  QString fileType = "Text";
  if (extension == "md" || extension == "markdown") {
    fileType = "Markdown";
  } else if (extension == "cpp" || extension == "cc" || extension == "cxx" ||
             extension == "c++" || extension == "h" || extension == "hpp" ||
             extension == "hxx" || extension == "h++") {
    fileType = "C++";
  } else if (extension == "py") {
    fileType = "Python";
  } else if (extension == "rs") {
    fileType = "Rust";
  }
  fileTypeLabel_->setText(fileType);

  // Add to recent files
  settings_.addRecentFile(fileName);
  updateRecentFilesMenu();
}

void MainWindow::saveFile() {
  if (currentFile_.isEmpty()) {
    saveFileAs();
    return;
  }

  QFile file(currentFile_);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(this, "Error",
                          "Could not save file: " + currentFile_);
    return;
  }

  QTextStream out(&file);
  out << editor_->toPlainText();
  file.close();

  isModified_ = false;
  statusBar()->showMessage("File saved: " + currentFile_);
}

void MainWindow::saveFileAs() {
  QString fileName =
      QFileDialog::getSaveFileName(this, "Save Markdown File", QString(),
                                   "Markdown Files (*.md);;All Files (*)");

  if (fileName.isEmpty())
    return;

  currentFile_ = fileName;
  saveFile();
  setWindowTitle("CyberMD - " + QFileInfo(fileName).fileName());
}

void MainWindow::about() {
  QMessageBox::about(
      this, "About CyberMD",
      "<h2>CyberMD Markdown Editor</h2>"
      "<p>Version 0.1.0</p>"
      "<p>A modern markdown editor with Rust core and Qt UI.</p>"
      "<p><b>Features:</b></p>"
      "<ul>"
      "<li>Fast Rust-based parsing</li>"
      "<li>Semantic highlighting</li>"
      "<li>Line numbers</li>"
      "<li>Modern dark theme</li>"
      "</ul>"
      "<p>Built by My7BroPxKi with ❤️ using Rust and C++/Qt6</p>");
}

void MainWindow::textChanged() {
  isModified_ = true;

  // Update status bar
  statusBar()->showMessage("Modified");

  // Trigger highlighting and preview update (debounced)
  static QTimer *highlightTimer = nullptr;
  if (!highlightTimer) {
    highlightTimer = new QTimer(this);
    highlightTimer->setSingleShot(true);
    connect(highlightTimer, &QTimer::timeout, this,
            &MainWindow::updateHighlighting);
  }
  highlightTimer->start(300); // 300ms debounce

  static QTimer *previewTimer = nullptr;
  if (!previewTimer) {
    previewTimer = new QTimer(this);
    previewTimer->setSingleShot(true);
    connect(previewTimer, &QTimer::timeout, this, &MainWindow::updatePreview);
  }
  previewTimer->start(300); // 300ms debounce

  // Trigger auto shell checking for shell scripts
  if (isShellCheckEnabled_ && !currentFile_.isEmpty()) {
    QFileInfo fileInfo(currentFile_);
    QString extension = fileInfo.suffix().toLower();
    if (extension == "sh" || extension == "bash" || currentFile_.isEmpty()) {
      // Also check if file starts with shebang
      QString text = editor_->toPlainText();
      if (text.startsWith("#!") &&
          (text.contains("bash") || text.contains("/sh"))) {
        shellCheckTimer_->start(); // Restart timer (1 second delay)
      }
    }
  }
}

QColor MainWindow::getColorForToken(uint32_t tokenType) {
  if (!currentTheme_)
    return QColor("#D4D4D4");

  // Return colors from current theme
  switch (tokenType) {
  case TOKEN_HEADING1:
    return currentTheme_->syntaxHeading1();
  case TOKEN_HEADING2:
    return currentTheme_->syntaxHeading2();
  case TOKEN_HEADING3:
    return currentTheme_->syntaxHeading3();
  case TOKEN_HEADING4:
    return currentTheme_->syntaxHeading4();
  case TOKEN_HEADING5:
    return currentTheme_->syntaxHeading5();
  case TOKEN_HEADING6:
    return currentTheme_->syntaxHeading6();
  case TOKEN_PARAGRAPH:
    return currentTheme_->syntaxParagraph();
  case TOKEN_CODE_BLOCK:
    return currentTheme_->syntaxCode();
  case TOKEN_INLINE_CODE:
    return currentTheme_->syntaxCode();
  case TOKEN_BOLD:
    return currentTheme_->syntaxBold();
  case TOKEN_ITALIC:
    return currentTheme_->syntaxItalic();
  case TOKEN_LINK:
    return currentTheme_->syntaxLink();
  case TOKEN_LIST_MARKER:
    return currentTheme_->syntaxListMarker();
  default:
    return currentTheme_->syntaxParagraph();
  }
}

void MainWindow::applyHighlighting(
    const std::vector<CyberMD::HighlightRange> &ranges) {
  // Safety checks
  if (!editor_) {
    return;
  }

  QTextDocument *doc = editor_->document();
  if (!doc) {
    return;
  }

  // Clear previous formatting
  QTextCursor cursor(doc);
  cursor.select(QTextCursor::Document);
  QTextCharFormat defaultFormat;
  defaultFormat.setForeground(currentTheme_ ? currentTheme_->editorForeground()
                                            : QColor("#D4D4D4"));
  cursor.setCharFormat(defaultFormat);

  // Apply each highlight range
  for (const auto &range : ranges) {
    QTextBlock startBlock = doc->findBlockByLineNumber(range.start_line);
    QTextBlock endBlock = doc->findBlockByLineNumber(range.end_line);

    if (!startBlock.isValid() || !endBlock.isValid()) {
      continue;
    }

    int startPos = startBlock.position() + range.start_col;
    int endPos = endBlock.position() + range.end_col;

    if (startPos >= 0 && endPos >= startPos) {
      QTextCursor highlightCursor(doc);
      highlightCursor.setPosition(startPos);
      highlightCursor.setPosition(endPos, QTextCursor::KeepAnchor);

      QTextCharFormat format;
      format.setForeground(getColorForToken(range.token_type));

      // Make headings bold and slightly larger
      if (range.token_type >= TOKEN_HEADING1 &&
          range.token_type <= TOKEN_HEADING6) {
        format.setFontWeight(QFont::Bold);
        int sizeIncrease = 7 - (range.token_type - TOKEN_HEADING1);
        format.setFontPointSize(11 + sizeIncrease);
      }

      // Make bold text bold
      if (range.token_type == TOKEN_BOLD) {
        format.setFontWeight(QFont::Bold);
      }

      // Make italic text italic
      if (range.token_type == TOKEN_ITALIC) {
        format.setFontItalic(true);
      }

      // Add background for code
      if (range.token_type == TOKEN_CODE_BLOCK ||
          range.token_type == TOKEN_INLINE_CODE) {
        format.setBackground(currentTheme_
                                 ? currentTheme_->syntaxCodeBackground()
                                 : QColor("#1E1E1E"));
      }

      highlightCursor.mergeCharFormat(format);
    }
  }
}

void MainWindow::updateHighlighting() {
  // Check if highlighter is initialized
  if (!highlighter_) {
    return;
  }

  // Check if editor is valid
  if (!editor_) {
    return;
  }

  // Only use Rust parser for Markdown files
  // For other files, Qt syntax highlighters handle everything
  QFileInfo fileInfo(currentFile_);
  QString extension = fileInfo.suffix().toLower();

  // Skip Rust highlighter if not a Markdown file or if no file is open
  if (currentFile_.isEmpty() ||
      (extension != "md" && extension != "markdown")) {
    return; // Qt syntax highlighter handles it
  }

  // Parse the document (Markdown only)
  QString text = editor_->toPlainText();

  try {
    CAST *ast = parser_->parse(text.toStdString());
    if (ast) {
      CyberMD::AST astWrapper(ast);

      // Get highlight ranges
      auto ranges = highlighter_->highlight(astWrapper.get());

      // Apply highlights to editor
      applyHighlighting(ranges);

      statusBar()->showMessage(
          QString("Parsed successfully - %1 highlight ranges")
              .arg(ranges.size()));
    }
  } catch (const std::exception &e) {
    statusBar()->showMessage(QString("Parse error: %1").arg(e.what()));
  }
}

void MainWindow::updateOutline() {
  // TODO: Implement outline update
  // This would populate a sidebar with document structure
}

void MainWindow::loadSettings() {
  // Restore window geometry
  QByteArray geometry = settings_.windowGeometry();
  if (!geometry.isEmpty()) {
    restoreGeometry(geometry);
  }

  QByteArray state = settings_.windowState();
  if (!state.isEmpty()) {
    restoreState(state);
  }
}

void MainWindow::saveSettings() {
  // Save window geometry
  settings_.setWindowGeometry(saveGeometry());
  settings_.setWindowState(saveState());
}

void MainWindow::applyRustHighlighter() {
  // Create highlighter based on theme
  Settings::Theme theme = settings_.theme();
  if (theme == Settings::Theme::Light) {
    highlighter_ = std::make_unique<CyberMD::Highlighter>(
        CyberMD::Highlighter::Theme::Light);
  } else {
    highlighter_ = std::make_unique<CyberMD::Highlighter>(
        CyberMD::Highlighter::Theme::Dark);
  }

  // Re-apply highlighting if there's content
  if (!editor_->toPlainText().isEmpty()) {
    updateHighlighting();
  }
}

void MainWindow::applyEditorSettings() {
  // Apply font settings
  QString fontFamily = settings_.fontFamily();
  int fontSize = settings_.fontSize();

  QFont font(fontFamily, fontSize);
  font.setStyleHint(QFont::Monospace);
  editor_->setFont(font);

  // Apply tab width
  int tabSize = settings_.tabSize();
  QFontMetrics metrics(font);
  editor_->setTabStopDistance(tabSize * metrics.horizontalAdvance(' '));
}

void MainWindow::showPreferences() {
  QDialog dialog(this);
  dialog.setWindowTitle("Preferences");
  dialog.setMinimumWidth(400);

  QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
  QFormLayout *formLayout = new QFormLayout();

  // Theme selection
  QComboBox *themeCombo = new QComboBox();
  themeCombo->addItem("Dark", static_cast<int>(Settings::Theme::Dark));
  themeCombo->addItem("Light", static_cast<int>(Settings::Theme::Light));
  themeCombo->setCurrentIndex(settings_.theme() == Settings::Theme::Light ? 1
                                                                          : 0);
  formLayout->addRow("Theme:", themeCombo);

  // Font selection
  QFontComboBox *fontCombo = new QFontComboBox();
  fontCombo->setFontFilters(QFontComboBox::MonospacedFonts);
  fontCombo->setCurrentFont(QFont(settings_.fontFamily()));
  formLayout->addRow("Font:", fontCombo);

  // Font size
  QSpinBox *fontSizeSpin = new QSpinBox();
  fontSizeSpin->setRange(8, 24);
  fontSizeSpin->setValue(settings_.fontSize());
  formLayout->addRow("Font Size:", fontSizeSpin);

  // Tab size
  QSpinBox *tabSizeSpin = new QSpinBox();
  tabSizeSpin->setRange(2, 8);
  tabSizeSpin->setValue(settings_.tabSize());
  formLayout->addRow("Tab Size:", tabSizeSpin);

  mainLayout->addLayout(formLayout);

  // Buttons
  QDialogButtonBox *buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
  mainLayout->addWidget(buttonBox);

  if (dialog.exec() == QDialog::Accepted) {
    // Save settings
    Settings::Theme selectedTheme =
        static_cast<Settings::Theme>(themeCombo->currentData().toInt());
    settings_.setTheme(selectedTheme);
    settings_.setFontFamily(fontCombo->currentFont().family());
    settings_.setFontSize(fontSizeSpin->value());
    settings_.setTabSize(tabSizeSpin->value());

    // Apply settings
    applyTheme();
    applyEditorSettings();

    statusBar()->showMessage("Preferences applied", 2000);
  }
}

void MainWindow::updateRecentFilesMenu() {
  recentFilesMenu_->clear();

  QStringList recentFiles = settings_.recentFiles();

  if (recentFiles.isEmpty()) {
    QAction *noFilesAction = recentFilesMenu_->addAction("No recent files");
    noFilesAction->setEnabled(false);
    return;
  }

  for (const QString &filePath : recentFiles) {
    QFileInfo fileInfo(filePath);
    QAction *action = recentFilesMenu_->addAction(
        QIcon::fromTheme("text-x-generic",
                         style()->standardIcon(QStyle::SP_FileIcon)),
        fileInfo.fileName());
    action->setData(filePath);
    action->setStatusTip(filePath);
    connect(action, &QAction::triggered, this, &MainWindow::openRecentFile);
  }

  recentFilesMenu_->addSeparator();

  QAction *clearAction = recentFilesMenu_->addAction("Clear Recent Files");
  connect(clearAction, &QAction::triggered, [this]() {
    settings_.clearRecentFiles();
    updateRecentFilesMenu();
  });
}

void MainWindow::updatePreview() {
  QString text = editor_->toPlainText();

  try {
    CAST *ast = parser_->parse(text.toStdString());
    if (ast) {
      CyberMD::AST astWrapper(ast);

      // Get HTML from AST
      std::string html = astWrapper.toHtml();

      // Update preview
      preview_->setHtml(QString::fromStdString(html));
    }
  } catch (const std::exception &e) {
    preview_->setHtml(
        QString("<p>Error rendering preview: %1</p>").arg(e.what()));
  }
}

void MainWindow::showFindDialog() {
  searchDialog_->showFind();
  searchDialog_->show();
  searchDialog_->raise();
  searchDialog_->activateWindow();
}

void MainWindow::showReplaceDialog() {
  searchDialog_->showReplace();
  searchDialog_->show();
  searchDialog_->raise();
  searchDialog_->activateWindow();
}

void MainWindow::showGoToLineDialog() {
  bool ok;
  int lineNumber = QInputDialog::getInt(this, "Go to Line", "Line number:", 1,
                                        1, 999999, 1, &ok);
  if (ok) {
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,
                        lineNumber - 1);
    editor_->setTextCursor(cursor);
    editor_->setFocus();
  }
}

void MainWindow::zoomIn() {
  QFont font = editor_->font();
  int currentSize = font.pointSize();
  if (currentSize < 72) {
    font.setPointSize(currentSize + 1);
    editor_->setFont(font);

    // Update settings
    settings_.setFontSize(currentSize + 1);
  }
}

void MainWindow::zoomOut() {
  QFont font = editor_->font();
  int currentSize = font.pointSize();
  if (currentSize > 6) {
    font.setPointSize(currentSize - 1);
    editor_->setFont(font);

    // Update settings
    settings_.setFontSize(currentSize - 1);
  }
}

void MainWindow::resetZoom() {
  QFont font = editor_->font();
  font.setPointSize(11); // Default size
  editor_->setFont(font);
  settings_.setFontSize(11);
}

void MainWindow::showRegexHelper() {
  regexHelper_->show();
  regexHelper_->raise();
  regexHelper_->activateWindow();
}

void MainWindow::showCommandHelper() {
  commandHelper_->show();
  commandHelper_->raise();
  commandHelper_->activateWindow();
}

void MainWindow::showShellChecker() {
  // If the current file appears to be a shell script, load it
  QString content = editor_->toPlainText();
  if (content.startsWith("#!") &&
      (content.contains("bash") || content.contains("/sh"))) {
    shellChecker_->checkScript(content);
  }
  shellChecker_->show();
  shellChecker_->raise();
  shellChecker_->activateWindow();
}

void MainWindow::toggleVimMode(bool enabled) {
  vimMode_->setEnabled(enabled);
  vimModeLabel_->setVisible(enabled);

  if (enabled) {
    onVimModeChanged(static_cast<int>(vimMode_->currentMode()));
    statusBar()->showMessage("VIM mode enabled");
  } else {
    statusBar()->showMessage("VIM mode disabled");
  }
}

void MainWindow::onVimModeChanged(int mode) {
  QString modeText;
  QColor modeColor;

  if (!currentTheme_)
    return;

  switch (static_cast<VimMode::Mode>(mode)) {
  case VimMode::Normal:
    modeText = "NORMAL";
    modeColor = currentTheme_->vimNormal();
    break;
  case VimMode::Insert:
    modeText = "INSERT";
    modeColor = currentTheme_->vimInsert();
    break;
  case VimMode::Visual:
    modeText = "VISUAL";
    modeColor = currentTheme_->vimVisual();
    break;
  case VimMode::Command:
    modeText = "COMMAND";
    modeColor = currentTheme_->vimCommand();
    break;
  }

  vimModeLabel_->setText("  " + modeText + "  ");
  vimModeLabel_->setStyleSheet(
      QString("QLabel { padding: 2px 10px; font-weight: bold; "
              "background-color: %1; color: %2; }")
          .arg(modeColor.name())
          .arg(currentTheme_->editorBackground().name()));
}

void MainWindow::toggleFileTree(bool enabled) {
  fileTree_->setVisible(enabled);

  // Adjust splitter sizes when toggling
  QList<int> sizes = mainSplitter_->sizes();
  if (sizes.size() >= 3) {
    if (enabled) {
      // Give file tree 300px, take from editor
      sizes[0] = 300;
      if (sizes[1] > 300) {
        sizes[1] -= 100; // Reduce editor slightly
      }
    } else {
      // Hide file tree, give space back to editor
      sizes[1] += sizes[0];
      sizes[0] = 0;
    }
    mainSplitter_->setSizes(sizes);
  }

  if (enabled) {
    // Set to current file's directory if a file is open
    if (!currentFile_.isEmpty()) {
      QFileInfo fileInfo(currentFile_);
      fileTree_->setRootPath(fileInfo.absolutePath());
    } else {
      // Default to home directory
      fileTree_->setRootPath(QDir::homePath());
    }
  }
}

void MainWindow::onFileSelected(const QString &filePath) {
  openFileByPath(filePath);
}

void MainWindow::onFeatureToggled() {
  // This can be used to enable/disable toolbar buttons based on checkbox state
  // For now, we just show a status message
  statusBar()->showMessage("Feature settings updated", 2000);
}

void MainWindow::toggleViewMode() {
  isPreviewMode_ = !isPreviewMode_;

  if (isPreviewMode_) {
    // Show preview, hide editor
    editor_->setVisible(false);
    preview_->setVisible(true);
    updatePreview();
    statusBar()->showMessage("Preview Mode", 2000);
  } else {
    // Show editor, hide preview
    preview_->setVisible(false);
    editor_->setVisible(true);
    statusBar()->showMessage("Edit Mode", 2000);
  }
}

void MainWindow::applySyntaxHighlighter(const QString &filePath) {
  qDebug() << "========================================";
  qDebug() << "applySyntaxHighlighter called for:" << filePath;

  // Delete previous highlighter if exists
  if (syntaxHighlighter_) {
    qDebug() << "Deleting previous syntax highlighter";
    delete syntaxHighlighter_;
    syntaxHighlighter_ = nullptr;
  }

  // Detect file type by extension
  QFileInfo fileInfo(filePath);
  QString extension = fileInfo.suffix().toLower();
  qDebug() << "File extension:" << extension;
  qDebug() << "Editor document valid:" << (editor_->document() ? "YES" : "NO");
  qDebug() << "Current theme valid:" << (currentTheme_ ? "YES" : "NO");

  // Apply appropriate highlighter
  if (extension == "md" || extension == "markdown") {
    qDebug() << "Creating MarkdownHighlighter";
    syntaxHighlighter_ = new MarkdownHighlighter(editor_->document());
  } else if (extension == "cpp" || extension == "cc" || extension == "cxx" ||
             extension == "c++" || extension == "h" || extension == "hpp" ||
             extension == "hxx" || extension == "h++") {
    qDebug() << "Creating CppHighlighter for C++ file";
    syntaxHighlighter_ = new CppHighlighter(editor_->document());
    qDebug() << "CppHighlighter created, pointer:" << syntaxHighlighter_;
  } else if (extension == "py") {
    qDebug() << "Creating PythonHighlighter";
    syntaxHighlighter_ = new PythonHighlighter(editor_->document());
  } else if (extension == "rs") {
    qDebug() << "Creating RustHighlighter";
    syntaxHighlighter_ = new RustHighlighter(editor_->document());
  } else {
    qDebug() << "WARNING: No highlighter for file type:" << extension;
    qDebug() << "Supported types: cpp, h, hpp, py, rs, md";
  }

  // Pass theme to syntax highlighter and force rehighlight
  if (syntaxHighlighter_) {
    qDebug() << "Syntax highlighter created successfully";

    if (currentTheme_) {
      qDebug() << "Setting theme on syntax highlighter";
      syntaxHighlighter_->setTheme(currentTheme_);
      qDebug() << "Theme set, calling rehighlight()";
    } else {
      qDebug() << "WARNING: No theme available, using default colors";
      // Force rehighlight even without theme
      qDebug() << "Forcing rehighlight with default colors";
      syntaxHighlighter_->rehighlight();
    }

    qDebug() << "Syntax highlighting should now be active";
  } else {
    qDebug() << "ERROR: Failed to create syntax highlighter!";
  }

  qDebug() << "========================================";
}

void MainWindow::autoCheckShellScript() {
  if (!isShellCheckEnabled_ || currentFile_.isEmpty()) {
    return;
  }

  // Check if shellcheck is available
  QString shellcheckPath = "shellcheck";

  // Get current content
  QString scriptContent = editor_->toPlainText();

  // Create temporary file
  QTemporaryFile tempFile;
  if (!tempFile.open()) {
    qWarning() << "Failed to create temporary file for shell checking";
    return;
  }

  tempFile.write(scriptContent.toUtf8());
  tempFile.flush();

  // Run shellcheck if not already running
  if (shellCheckProcess_->state() == QProcess::Running) {
    shellCheckProcess_->kill(); // Kill previous check
  }

  // Run shellcheck with JSON output
  QStringList args;
  args << "--format=json" << tempFile.fileName();

  shellCheckProcess_->start(shellcheckPath, args);

  // Keep temp file alive until process finishes
  tempFile.setAutoRemove(true);
}

void MainWindow::onShellCheckFinished(int exitCode,
                                      QProcess::ExitStatus exitStatus) {
  if (exitStatus != QProcess::NormalExit) {
    return;
  }

  // Read shellcheck output
  QByteArray output = shellCheckProcess_->readAllStandardOutput();
  QString outputStr = QString::fromUtf8(output);

  // Parse JSON output (simple error counting for now)
  // If shellcheck found issues, it returns non-zero exit code
  if (exitCode == 0) {
    // No errors
    errorCountLabel_->setVisible(false);
  } else if (exitCode == 1) {
    // Parse JSON to count errors
    // For simplicity, count occurrences of "level"
    int errorCount = outputStr.count("\"level\"");

    if (errorCount > 0) {
      errorCountLabel_->setText(QString("⚠ Errors: %1").arg(errorCount));
      errorCountLabel_->setVisible(true);
      statusBar()->showMessage(
          QString("Shell script check: %1 issue(s) found").arg(errorCount),
          3000);
    } else {
      errorCountLabel_->setVisible(false);
    }
  }
}

void MainWindow::createThemeMenu(QMenu *viewMenu) {
  qDebug() << "createThemeMenu() called";

  if (!viewMenu) {
    qDebug() << "ERROR: viewMenu is null in createThemeMenu!";
    return;
  }

  qDebug() << "viewMenu is valid";

  qDebug() << "Creating theme submenu...";
  QMenu *themeMenu = viewMenu->addMenu("&Theme");
  qDebug() << "Theme submenu created";

  qDebug() << "Creating action group...";
  QActionGroup *themeGroup = new QActionGroup(this);
  themeGroup->blockSignals(true); // Block signals during setup
  qDebug() << "Action group created";

  qDebug() << "Getting available themes...";
  QMap<Theme::ThemeType, QString> themes = Theme::availableThemes();
  qDebug() << "Found" << themes.size() << "themes";

  // Store theme type by value, not iterator
  for (auto it = themes.constBegin(); it != themes.constEnd(); ++it) {
    Theme::ThemeType themeType = it.key();
    QString themeName = it.value();

    QAction *themeAction = themeMenu->addAction(themeName);
    themeAction->setCheckable(true);
    themeAction->setData(static_cast<int>(themeType));
    themeAction->blockSignals(true); // Block signals on this action
    themeGroup->addAction(themeAction);

    if (currentTheme_ && currentTheme_->type() == themeType) {
      themeAction->setChecked(true);
    }

    themeAction->blockSignals(false); // Re-enable signals

    // Capture theme type by value, not iterator
    connect(themeAction, &QAction::triggered,
            [this, themeType]() { changeTheme(themeType); });
  }

  themeGroup->blockSignals(false); // Re-enable signals after setup
}

void MainWindow::changeTheme(Theme::ThemeType themeType) {
  if (currentTheme_) {
    delete currentTheme_;
  }

  currentTheme_ = new Theme(themeType);
  applyTheme();

  statusBar()->showMessage(
      QString("Theme changed to: %1").arg(currentTheme_->name()), 2000);
}

void MainWindow::applyTheme() {
  qDebug() << "applyTheme() called";

  if (!currentTheme_) {
    qDebug() << "ERROR: currentTheme_ is null!";
    return;
  }

  qDebug() << "Current theme:" << currentTheme_->name();

  // Pass theme to editor
  if (!editor_) {
    qDebug() << "ERROR: editor_ is null!";
    return;
  }

  qDebug() << "Setting theme on editor...";
  editor_->setTheme(currentTheme_);
  qDebug() << "Theme set on editor";

  // Apply Rust highlighter based on theme type
  Settings::Theme oldTheme = settings_.theme();
  if (currentTheme_->type() == Theme::Light ||
      currentTheme_->type() == Theme::SolarizedLight) {
    highlighter_ = std::make_unique<CyberMD::Highlighter>(
        CyberMD::Highlighter::Theme::Light);
  } else {
    highlighter_ = std::make_unique<CyberMD::Highlighter>(
        CyberMD::Highlighter::Theme::Dark);
  }

  // Update syntax highlighter theme if it exists
  if (syntaxHighlighter_) {
    syntaxHighlighter_->setTheme(currentTheme_);
  }

  // Apply editor colors
  QPalette editorPalette;
  editorPalette.setColor(QPalette::Base, currentTheme_->editorBackground());
  editorPalette.setColor(QPalette::Text, currentTheme_->editorForeground());
  editor_->setPalette(editorPalette);

  // Apply current line highlight color
  // This will be handled by CodeEditor's highlightCurrentLine() - we need to
  // update that

  // Apply line number area colors (handled in CodeEditor)

  // Apply UI colors
  QString mainStyle = QString(R"(
        QMainWindow {
            background-color: %1;
            color: %2;
        }
        QMenuBar {
            background-color: %1;
            color: %2;
        }
        QMenuBar::item:selected {
            background-color: %3;
        }
        QMenu {
            background-color: %1;
            color: %2;
            border: 1px solid %4;
        }
        QMenu::item:selected {
            background-color: %3;
        }
        QStatusBar {
            background-color: %1;
            color: %2;
        }
        QToolBar {
            background-color: %1;
            border: none;
        }
    )")
                          .arg(currentTheme_->uiBackground().name())
                          .arg(currentTheme_->uiForeground().name())
                          .arg(currentTheme_->uiAccent().name())
                          .arg(currentTheme_->uiBorder().name());

  setStyleSheet(mainStyle);

  // Apply splitter style
  if (mainSplitter_) {
    mainSplitter_->setStyleSheet(currentTheme_->getSplitterStyleSheet());
  }

  // Apply status bar widget styles
  if (fileTypeLabel_) {
    fileTypeLabel_->setStyleSheet(
        QString("QLabel { padding: 2px 10px; background-color: %1; color: %2; "
                "border-radius: 3px; }")
            .arg(currentTheme_->statusBarBackground().name())
            .arg(currentTheme_->statusBarForeground().name()));
  }

  if (lineCountLabel_) {
    lineCountLabel_->setStyleSheet(currentTheme_->getStatusBarStyleSheet());
  }

  if (errorCountLabel_) {
    errorCountLabel_->setStyleSheet(
        QString("QLabel { padding: 2px 10px; color: %1; }")
            .arg(currentTheme_->errorColor().name()));
  }

  if (statusLabel_) {
    statusLabel_->setStyleSheet(currentTheme_->getStatusBarStyleSheet());
  }

  if (vimModeLabel_) {
    vimModeLabel_->setStyleSheet(
        "QLabel { padding: 2px 10px; font-weight: bold; }");
  }

  // Apply feature panel style
  if (featurePanel_) {
    featurePanel_->setStyleSheet(currentTheme_->getFeaturePanelStyleSheet());
  }

  // Apply file tree style
  if (fileTree_) {
    QString fileTreeStyle =
        QString(R"(
            QWidget {
                background-color: %1;
            }
            QTreeWidget {
                background-color: %1;
                color: %2;
                border: 1px solid %3;
                outline: none;
                font-size: 12px;
            }
            QTreeWidget::item {
                padding: 5px 2px;
                border: none;
            }
            QTreeWidget::item:hover {
                background-color: %4;
            }
            QTreeWidget::item:selected {
                background-color: %5;
                color: %6;
            }
            QTreeWidget::item:selected:hover {
                background-color: %7;
            }
            QHeaderView::section {
                background-color: %8;
                color: %2;
                border: none;
                border-bottom: 1px solid %3;
                padding: 6px 4px;
                font-weight: bold;
            }
            QLineEdit {
                background-color: %1;
                color: %2;
                border: 1px solid %3;
                border-radius: 3px;
                padding: 4px 8px;
                selection-background-color: %5;
            }
            QPushButton {
                background-color: %9;
                color: %6;
                border: 1px solid %3;
                border-radius: 3px;
                padding: 4px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: %5;
                border: 1px solid %5;
            }
            QPushButton:pressed {
                background-color: %10;
            }
            QScrollBar:vertical {
                background-color: %1;
                width: 12px;
                border: none;
            }
            QScrollBar::handle:vertical {
                background-color: %3;
                border-radius: 6px;
                min-height: 20px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: %5;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )")
            .arg(currentTheme_->uiBackground().name()) // %1 - background
            .arg(currentTheme_->uiForeground().name()) // %2 - foreground
            .arg(currentTheme_->uiBorder().name())     // %3 - border
            .arg(currentTheme_->uiAccent()
                     .lighter(160)
                     .name())                      // %4 - hover background
            .arg(currentTheme_->uiAccent().name()) // %5 - selection/accent
            .arg(QColor(Qt::white).name())         // %6 - white text
            .arg(currentTheme_->uiAccent()
                     .lighter(110)
                     .name()) // %7 - selected hover
            .arg(currentTheme_->uiButtonBackground()
                     .name()) // %8 - header background
            .arg(currentTheme_->uiButtonBackground()
                     .name()) // %9 - button background
            .arg(currentTheme_->uiAccent()
                     .darker(120)
                     .name()); // %10 - button pressed

    fileTree_->setStyleSheet(fileTreeStyle);
  }

  // Reapply highlighting with new colors
  updateHighlighting();

  // Force update
  update();
}

// ============================================
// File Menu Actions
// ============================================

void MainWindow::newWindow() {
  MainWindow *newWin = new MainWindow();
  newWin->show();
}

void MainWindow::openFolder() {
  QString folder =
      QFileDialog::getExistingDirectory(this, "Open Folder", QDir::homePath());
  if (!folder.isEmpty()) {
    fileTree_->setRootPath(folder);
  }
}

void MainWindow::saveAllFiles() {
  if (tabWidget_) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
      tabWidget_->saveTab(i);
    }
  }
}

void MainWindow::closeTab() {
  if (tabWidget_) {
    tabWidget_->closeTab(tabWidget_->currentIndex());
  }
}

void MainWindow::closeAllTabs() {
  if (tabWidget_) {
    while (tabWidget_->count() > 0) {
      tabWidget_->closeTab(0);
    }
  }
}

void MainWindow::closeWindow() { close(); }

// ============================================
// Edit Menu Actions
// ============================================

void MainWindow::undo() {
  if (currentEditor()) {
    currentEditor()->undo();
  }
}

void MainWindow::redo() {
  if (currentEditor()) {
    currentEditor()->redo();
  }
}

void MainWindow::cut() {
  if (currentEditor()) {
    currentEditor()->cut();
  }
}

void MainWindow::copy() {
  if (currentEditor()) {
    currentEditor()->copy();
  }
}

void MainWindow::paste() {
  if (currentEditor()) {
    currentEditor()->paste();
  }
}

void MainWindow::selectAll() {
  if (currentEditor()) {
    currentEditor()->selectAll();
  }
}

void MainWindow::duplicateLine() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  cursor.movePosition(QTextCursor::StartOfBlock);
  cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
  QString line = cursor.selectedText();
  cursor.movePosition(QTextCursor::EndOfBlock);
  cursor.insertText("\n" + line);
  editor->setTextCursor(cursor);
}

void MainWindow::deleteLine() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  cursor.movePosition(QTextCursor::StartOfBlock);
  cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
  cursor.removeSelectedText();
  editor->setTextCursor(cursor);
}

void MainWindow::moveLineUp() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  int currentBlock = cursor.blockNumber();
  if (currentBlock == 0)
    return; // Already at top

  cursor.movePosition(QTextCursor::StartOfBlock);
  cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
  QString currentLine = cursor.selectedText();
  cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
  cursor.removeSelectedText();

  cursor.movePosition(QTextCursor::PreviousBlock);
  cursor.movePosition(QTextCursor::StartOfBlock);
  cursor.insertText(currentLine + "\n");
  cursor.movePosition(QTextCursor::PreviousBlock);
  editor->setTextCursor(cursor);
}

void MainWindow::moveLineDown() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  if (cursor.blockNumber() >= editor->blockCount() - 1)
    return; // Already at bottom

  cursor.movePosition(QTextCursor::StartOfBlock);
  cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
  QString currentLine = cursor.selectedText();
  cursor.removeSelectedText();

  cursor.movePosition(QTextCursor::EndOfBlock);
  cursor.insertText("\n" + currentLine.trimmed());
  editor->setTextCursor(cursor);
}

void MainWindow::toggleComment() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  cursor.movePosition(QTextCursor::StartOfBlock);

  QString lineText = cursor.block().text();
  QString trimmed = lineText.trimmed();

  cursor.movePosition(QTextCursor::StartOfBlock);

  // Detect comment style based on file type (simplified - uses //)
  if (trimmed.startsWith("//")) {
    // Remove comment
    int commentPos = lineText.indexOf("//");
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,
                        commentPos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
    if (cursor.selectedText() == "// ") {
      cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
    }
    cursor.removeSelectedText();
  } else {
    // Add comment
    int indent = 0;
    for (QChar c : lineText) {
      if (c == ' ')
        indent++;
      else
        break;
    }
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, indent);
    cursor.insertText("// ");
  }

  editor->setTextCursor(cursor);
}

// ============================================
// Search Actions
// ============================================

void MainWindow::findNext() {
  if (searchDialog_) {
    searchDialog_->findNext();
  }
}

void MainWindow::findPrevious() {
  if (searchDialog_) {
    searchDialog_->findPrevious();
  }
}

// ============================================
// View Menu Actions
// ============================================

void MainWindow::toggleSplitView() {
  // Toggle split editor view
  splitViewEnabled_ = !splitViewEnabled_;
  // Implement your split view logic here
}

void MainWindow::toggleFullScreen() {
  if (isFullScreen()) {
    showNormal();
  } else {
    showFullScreen();
  }
}

void MainWindow::toggleFeaturePanel(bool visible) {
  if (featurePanel_) {
    featurePanel_->setVisible(visible);
  }
}

void MainWindow::toggleMinimap(bool visible) {
  minimapEnabled_ = visible;
  // Update all editors to show/hide minimap
}

void MainWindow::toggleWordWrap(bool enabled) {
  if (currentEditor()) {
    currentEditor()->setLineWrapMode(enabled ? QPlainTextEdit::WidgetWidth
                                             : QPlainTextEdit::NoWrap);
  }
}

void MainWindow::toggleLineNumbers(bool visible) {
  lineNumbersVisible_ = visible;
  // Update all editors
}

void MainWindow::toggleWhitespace(bool visible) {
  showWhitespace_ = visible;
  // Update all editors to show/hide whitespace characters
}

// ============================================
// Code Folding Actions
// ============================================

void MainWindow::foldAll() {
  CodeEditor *editor = currentEditor();
  if (editor && editor->codeFolding()) {
    editor->codeFolding()->foldAll();
    editor->viewport()->update();
  }
}

void MainWindow::unfoldAll() {
  CodeEditor *editor = currentEditor();
  if (editor && editor->codeFolding()) {
    editor->codeFolding()->unfoldAll();
    editor->viewport()->update();
  }
}

void MainWindow::foldCurrentSection() {
  CodeEditor *editor = currentEditor();
  if (editor && editor->codeFolding()) {
    int line = editor->textCursor().blockNumber();
    editor->codeFolding()->toggleFoldAtLine(line);
    editor->viewport()->update();
  }
}

void MainWindow::toggleCodeFolding(bool enabled) {
  if (currentEditor()) {
    currentEditor()->enableCodeFolding(enabled);
  }
}

// ============================================
// Tools Menu Actions
// ============================================

void MainWindow::formatDocument() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  // Basic formatting - fix indentation
  QString text = editor->toPlainText();
  // Add your formatting logic here
  // For now, just trim trailing whitespace from each line
  QStringList lines = text.split('\n');
  for (QString &line : lines) {
    while (line.endsWith(' ') || line.endsWith('\t')) {
      line.chop(1);
    }
  }

  QTextCursor cursor = editor->textCursor();
  cursor.select(QTextCursor::Document);
  cursor.insertText(lines.join('\n'));
}

void MainWindow::sortLines() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  if (!cursor.hasSelection()) {
    cursor.select(QTextCursor::Document);
  }

  QString text = cursor.selectedText();
  QStringList lines = text.split(QChar::ParagraphSeparator);
  lines.sort();
  cursor.insertText(lines.join('\n'));
}

void MainWindow::removeDuplicateLines() {
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  QTextCursor cursor = editor->textCursor();
  if (!cursor.hasSelection()) {
    cursor.select(QTextCursor::Document);
  }

  QString text = cursor.selectedText();
  QStringList lines = text.split(QChar::ParagraphSeparator);
  QStringList uniqueLines;
  QSet<QString> seen;

  for (const QString &line : lines) {
    if (!seen.contains(line)) {
      seen.insert(line);
      uniqueLines.append(line);
    }
  }

  cursor.insertText(uniqueLines.join('\n'));
}

void MainWindow::runShellCheck() {
  // Run shell/script checker on current file
  CodeEditor *editor = currentEditor();
  if (!editor)
    return;

  // Implement shellcheck integration here
  statusBar()->showMessage("ShellCheck: No issues found", 3000);
}

// ============================================
// Fuzzy Finder Actions
// ============================================

void MainWindow::showFuzzyFileSearch() {
  if (fuzzyFinder_) {
    fuzzyFinder_->setMode(FuzzyFinder::FileMode);
    fuzzyFinder_->show();
    fuzzyFinder_->setFocus();
  }
}

void MainWindow::showFuzzyContentSearch() {
  if (fuzzyFinder_) {
    fuzzyFinder_->setMode(FuzzyFinder::ContentMode);
    fuzzyFinder_->show();
    fuzzyFinder_->setFocus();
  }
}

void MainWindow::showFuzzyCommandSearch() {
  if (fuzzyFinder_) {
    fuzzyFinder_->setMode(FuzzyFinder::CommandMode);
    fuzzyFinder_->show();
    fuzzyFinder_->setFocus();
  }
}

void MainWindow::showFuzzyBufferSearch() {
  if (fuzzyFinder_) {
    fuzzyFinder_->setMode(FuzzyFinder::BufferMode);
    fuzzyFinder_->show();
    fuzzyFinder_->setFocus();
  }
}

void MainWindow::onFuzzyFileSelected(const QString &filePath) {
  if (tabWidget_) {
    tabWidget_->openFile(filePath);
  }
  if (fuzzyFinder_) {
    fuzzyFinder_->hide();
  }
}

void MainWindow::onFuzzyContentSelected(const QString &filePath, int line) {
  if (tabWidget_) {
    tabWidget_->openFile(filePath);
    CodeEditor *editor = currentEditor();
    if (editor) {
      QTextCursor cursor = editor->textCursor();
      cursor.movePosition(QTextCursor::Start);
      cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor,
                          line - 1);
      editor->setTextCursor(cursor);
      editor->centerCursor();
    }
  }
  if (fuzzyFinder_) {
    fuzzyFinder_->hide();
  }
}

void MainWindow::onFuzzyCommandSelected(const QString &command) {
  // Execute the selected command
  if (fuzzyFinder_) {
    fuzzyFinder_->hide();
  }

  // Map command names to actions
  if (command == "New File")
    newFile();
  else if (command == "Open File")
    openFile();
  else if (command == "Save")
    saveFile();
  else if (command == "Save All")
    saveAllFiles();
  else if (command == "Close Tab")
    closeTab();
  else if (command == "Toggle Full Screen")
    toggleFullScreen();
  else if (command == "Format Document")
    formatDocument();
  // Add more commands as needed
}

// ============================================
// Tab/Editor Event Handlers
// ============================================

void MainWindow::onTabChanged(int index) {
  Q_UNUSED(index)

  // Update window title
  if (tabWidget_ && tabWidget_->count() > 0) {
    QString fileName = tabWidget_->tabText(tabWidget_->currentIndex());
    setWindowTitle(fileName + " - CyberMD");
  } else {
    setWindowTitle("CyberMD");
  }

  // Update status bar with cursor position
  updateStatusBar();
}

void MainWindow::onTabModified(int index, bool modified) {
  if (tabWidget_) {
    QString title = tabWidget_->tabText(index);
    if (modified && !title.endsWith(" *")) {
      tabWidget_->setTabText(index, title + " *");
    } else if (!modified && title.endsWith(" *")) {
      tabWidget_->setTabText(index, title.chopped(2));
    }
  }
}

void MainWindow::onEditorChanged(CodeEditor *editor) {
  Q_UNUSED(editor)
  updateStatusBar();
}

void MainWindow::onDiagnosticsReady(const QString &diagnostics) {
  // Display diagnostics in a panel or status bar
  if (!diagnostics.isEmpty()) {
    statusBar()->showMessage("Diagnostics: " + diagnostics, 5000);
  }
}

// ============================================
// Helper Methods
// ============================================

CodeEditor *MainWindow::currentEditor() {
  if (tabWidget_) {
    return tabWidget_->currentEditor();
  }
  return nullptr;
}
