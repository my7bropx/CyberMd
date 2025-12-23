#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rustbridge.h"
#include "settings.h"
#include "theme.h"
#include <QColor>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QProcess>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QString>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <memory>

class CodeEditor;
class EditorTabWidget;
class BaseSyntaxHighlighter;
class MarkdownPreview;
class SearchDialog;
class RegexHelper;
class CommandHelper;
class ShellChecker;
class VimMode;
class FileTree;
class FeaturePanel;
class FuzzyFinder;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  CodeEditor *currentEditor();
private slots:
  // File operations
  void newFile();
  void newWindow();
  void openFile();
  void openFolder();
  void openRecentFile();
  void saveFile();
  void saveFileAs();
  void saveAllFiles();
  void closeTab();
  void closeAllTabs();
  void closeWindow();
  void updateStatusBar();
  // Edit operations
  void undo();
  void redo();
  void cut();
  void copy();
  void paste();
  void selectAll();
  void duplicateLine();
  void deleteLine();
  void moveLineUp();
  void moveLineDown();
  void toggleComment();

  // Find/Replace
  void showFindDialog();
  void showReplaceDialog();
  void showGoToLineDialog();
  void findNext();
  void findPrevious();

  // View operations
  void toggleViewMode();
  void toggleSplitView();
  void zoomIn();
  void zoomOut();
  void resetZoom();
  void toggleFullScreen();
  void toggleFileTree(bool enabled);
  void toggleFeaturePanel(bool enabled);
  void toggleMinimap(bool enabled);
  void toggleWordWrap(bool enabled);
  void toggleLineNumbers(bool enabled);
  void toggleWhitespace(bool enabled);

  // Code operations
  void foldAll();
  void unfoldAll();
  void foldCurrentSection();
  void toggleCodeFolding(bool enabled);
  void formatDocument();
  void sortLines();
  void removeDuplicateLines();

  // Tools
  void showRegexHelper();
  void showCommandHelper();
  void showShellChecker();
  void runShellCheck();
  void toggleVimMode(bool enabled);

  // Fuzzy finder (FZF-style)
  void showFuzzyFileSearch();
  void showFuzzyContentSearch();
  void showFuzzyCommandSearch();
  void showFuzzyBufferSearch();
  void onFuzzyFileSelected(const QString &filePath);
  void onFuzzyContentSelected(const QString &filePath, int lineNumber);
  void onFuzzyCommandSelected(const QString &command);

  // Tab/File management
  void onTabChanged(int index);
  void onTabModified(int index, bool modified);
  void onFileSelected(const QString &filePath);
  void onEditorChanged(CodeEditor *editor);

  // VIM mode
  void onVimModeChanged(int mode);

  // Theme
  void changeTheme(Theme::ThemeType themeType);

  // Settings
  void showPreferences();
  void about();

  // Auto-checking
  void autoCheckShellScript();
  void onShellCheckFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void onDiagnosticsReady(const QString &output);

  // Text changed handling
  void textChanged();

  // Feature toggle
  void onFeatureToggled();

private:
  void setupUI();
  void setupCentralWidget();
  void setupMenuBar();
  void setupFileMenu();
  void setupEditMenu();
  void setupViewMenu();
  void setupCodeMenu();
  void setupToolsMenu();
  void setupHelpMenu();
  void setupToolBar();
  void setupStatusBar();
  void setupFuzzyFinder();
  void createConnections();
  void createShortcuts();

  // Theme application
  void applyTheme();
  void createThemeMenu(QMenu *viewMenu);

  // Settings
  void loadSettings();
  void saveSettings();
  void applyEditorSettings();

  // File operations helpers
  void updateRecentFilesMenu();
  void openFileByPath(const QString &filePath);
  void addToRecentFiles(const QString &filePath);

  // Highlighting
  void updateHighlighting();
  void applyHighlighting(const std::vector<CyberMD::HighlightRange> &ranges);
  QColor getColorForToken(uint32_t tokenType);

  // Syntax highlighter
  void applySyntaxHighlighter(const QString &filePath);
  void applyRustHighlighter();

  // Outline
  void updateOutline();

  // Preview
  void updatePreview();
  void syncPreviewScroll();

  // Shell checking
  void performAutoCheck();
  QString getCurrentLanguage();

  // ========== UI COMPONENTS ==========

  // Main layout
  QSplitter *mainSplitter_;   // File tree | Editor/Preview
  QSplitter *editorSplitter_; // Editor | Preview (when split view)
  QSplitter *splitter_;       // Generic splitter for backward compatibility
  QStackedWidget *viewStack_; // Editor/Preview switch

  // Tab widget for multiple files
  EditorTabWidget *tabWidget_;

  // Editor (single editor for backward compatibility)
  CodeEditor *editor_;

  // Preview
  MarkdownPreview *preview_;
  bool isPreviewMode_;
  bool isSplitView_;

  // Sidebar
  FileTree *fileTree_;
  FeaturePanel *featurePanel_;

  // Status bar widgets
  QLabel *statusLabel_;
  QLabel *positionLabel_;
  QLabel *encodingLabel_;
  QLabel *lineEndingLabel_;
  QLabel *languageLabel_;
  QLabel *vimModeLabel_;
  QLabel *diagnosticsLabel_;
  QLabel *fileTypeLabel_;
  QLabel *lineCountLabel_;
  QLabel *errorCountLabel_;

  // Menus
  QMenu *recentFilesMenu_;
  QMenu *themeMenu_;

  // Dialogs
  SearchDialog *searchDialog_;
  RegexHelper *regexHelper_;
  CommandHelper *commandHelper_;
  ShellChecker *shellChecker_;
  FuzzyFinder *fuzzyFinder_;

  // VIM mode
  VimMode *vimMode_;
  bool vimModeEnabled_;

  // ========== STATE ==========

  // Theme
  Theme *currentTheme_;

  // Settings
  Settings settings_;

  // Current file
  QString currentFile_;
  bool isModified_;

  // Syntax highlighting
  BaseSyntaxHighlighter *syntaxHighlighter_;

  // Rust core components
  std::unique_ptr<CyberMD::Parser> parser_;
  std::unique_ptr<CyberMD::Highlighter> highlighter_;

  // Auto-checking
  QTimer *autoCheckTimer_;
  QTimer *shellCheckTimer_;
  QProcess *shellCheckProcess_;
  bool isAutoCheckEnabled_;
  bool isShellCheckEnabled_;
  QString lastCheckedContent_;

  // Tracking
  QString currentWorkingDirectory_;
  bool isClosing_;
  bool splitViewEnabled_ = false;
  bool minimapEnabled_ = false;
  bool lineNumbersVisible_ = true;
  bool showWhitespace_ = false;
};

#endif // MAINWINDOW_H
