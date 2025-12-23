#ifndef FUZZYFINDER_H
#define FUZZYFINDER_H

#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QStringList>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class Theme;

// Match result with score and positions
struct FuzzyMatch {
  QString text;
  QString filePath;
  int score;
  QVector<int> matchPositions; // Positions of matched characters

  bool operator<(const FuzzyMatch &other) const {
    return score > other.score; // Higher score first
  }
};

// Custom list widget item with highlighted matches
class FuzzyListItem : public QListWidgetItem {
public:
  FuzzyListItem(const FuzzyMatch &match, QListWidget *parent = nullptr);

  QString filePath() const { return filePath_; }
  int score() const { return score_; }

private:
  QString filePath_;
  int score_;
};

// Custom line edit that handles special keys
class FuzzyLineEdit : public QLineEdit {
  Q_OBJECT

public:
  explicit FuzzyLineEdit(QWidget *parent = nullptr);

signals:

  void moveUp();
  void moveDown();
  void selectCurrent();

protected:
  void keyPressEvent(QKeyEvent *event) override;
};

// Main fuzzy finder dialog
class FuzzyFinder : public QDialog {
  Q_OBJECT

public:
  enum Mode {
    FileMode,    // Search for files
    ContentMode, // Search within file contents (grep-like)
    CommandMode, // Search for commands/actions
    BufferMode   // Search open buffers/tabs
  };

  explicit FuzzyFinder(QWidget *parent = nullptr);
  ~FuzzyFinder();

  void setMode(Mode mode);
  void setRootPath(const QString &path);
  void setOpenFiles(const QStringList &files);
  void setTheme(Theme *theme);

  // Show with specific mode
  void showFileSearch(const QString &rootPath);
  void showContentSearch(const QString &rootPath);
  void showBufferSearch(const QStringList &openFiles);
  void showCommandSearch();

signals:
  void fileSelected(const QString &filePath);
  void contentSelected(const QString &filePath, int lineNumber);
  void commandSelected(const QString &command);
  void bufferSelected(const QString &filePath);

protected:
  void showEvent(QShowEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void onTextChanged(const QString &text);
  void onItemActivated(QListWidgetItem *item);
  void onMoveUp();
  void onMoveDown();
  void onSelectCurrent();
  void performSearch();

private:
  void setupUI();
  void applyTheme();
  void clearResults();

  // Fuzzy matching algorithm
  int fuzzyScore(const QString &pattern, const QString &text,
                 QVector<int> &positions);
  bool fuzzyMatch(const QString &pattern, const QString &text);

  // Search functions
  void searchFiles(const QString &pattern);
  void searchContent(const QString &pattern);
  void searchBuffers(const QString &pattern);
  void searchCommands(const QString &pattern);

  // File scanning
  void scanDirectory(const QString &path, QStringList &files, int maxDepth = 5);
  void indexFiles();

  // Result display
  void displayResults(const QVector<FuzzyMatch> &matches);
  QString highlightMatches(const QString &text, const QVector<int> &positions);

  FuzzyLineEdit *searchInput_;
  QListWidget *resultsList_;
  QLabel *statusLabel_;
  QLabel *modeLabel_;

  Mode currentMode_;
  QString rootPath_;
  QStringList openFiles_;
  QStringList indexedFiles_;
  Theme *theme_;

  QTimer *searchTimer_;
  QString lastPattern_;

  // Commands for command mode
  QMap<QString, QString> commands_;
};

#endif // FUZZYFINDER_H
