#ifndef CODEFOLDING_H
#define CODEFOLDING_H

#include <QMap>
#include <QObject>
#include <QPlainTextEdit>
#include <QSet>
#include <QTextBlock>
#include <QVector>

// Fold region structure with enhanced info
struct FoldRegion {
  int startLine;
  int endLine;
  bool isFolded;
  QString
      foldType; // "header", "codeblock", "list", "function", "class", "region"
  int indentLevel;     // For nested folding
  QString previewText; // First line preview when folded

  FoldRegion() : startLine(0), endLine(0), isFolded(false), indentLevel(0) {}
};

class CodeFolding : public QObject {
  Q_OBJECT
public:
  explicit CodeFolding(QPlainTextEdit *editor, QObject *parent = nullptr);
  ~CodeFolding();

  // Main operations
  void analyzeFoldRegions();
  void toggleFoldAtLine(int line);
  void foldAll();
  void unfoldAll();
  void foldLevel(int level); // Fold all regions at specific indent level

  // Queries
  bool isFoldable(int line) const;
  bool isFolded(int line) const;
  bool isLineHidden(int line) const;
  int foldDepth(int line) const;
  FoldRegion *regionAtLine(int line);
  const FoldRegion *regionAtLine(int line) const;

  // Get all fold regions (for painting)
  QMap<int, FoldRegion> &regions() { return foldRegions_; }
  const QMap<int, FoldRegion> &regions() const { return foldRegions_; }

  // Language-specific folding
  void setLanguage(const QString &language);
  QString language() const { return language_; }

signals:
  void foldingChanged();
  void regionFolded(int startLine, int endLine);
  void regionUnfolded(int startLine);

private:
  void updateFolding();
  void foldRegion(int startLine, int endLine);
  void unfoldRegion(int startLine);
  void hideLines(int startLine, int endLine);
  void showLines(int startLine, int endLine);

  // Markdown-specific detection
  int findHeaderEndLine(int startLine, int headerLevel);
  int findCodeBlockEndLine(int startLine);
  int findListEndLine(int startLine);
  int findBlockQuoteEndLine(int startLine);

  // Code-specific detection (C++, Python, Rust, etc.)
  int findBraceBlockEndLine(int startLine);
  int findIndentBlockEndLine(int startLine);
  int findRegionEndLine(int startLine);

  // Detection helpers
  bool isHeader(const QString &text, int &level);
  bool isCodeBlockDelimiter(const QString &text); // Single-argument version
  bool isListItem(const QString &text);
  bool isBlockQuote(const QString &text);
  bool isFunctionStart(const QString &text);
  bool isClassStart(const QString &text);
  bool isRegionStart(const QString &text, QString &name);
  bool isRegionEnd(const QString &text);
  int getIndentLevel(const QString &text);

  // State
  QPlainTextEdit *editor_;
  QMap<int, FoldRegion> foldRegions_; // Key is start line
  QSet<int>
      foldedLines_; // Lines that are folded (start lines of folded regions)
  QSet<int> hiddenLines_;
  QString language_; // "markdown", "cpp", "python", "rust", etc.
};

#endif // CODEFOLDING_H
