#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QColor>
#include <QMap>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QTextBlock>
#include <QTimer>
#include <QVector>
#include <QWidget>

// Forward declarations
class CodeEditor;
class CodeFolding;
class LineNumberArea;
class FoldingArea;
class Theme;
class QPaintEvent;
class QResizeEvent;
class QWheelEvent;
class QMouseEvent;
class QKeyEvent;

// Diagnostic information for syntax checking
struct Diagnostic {
  enum Severity { Error, Warning, Info, Hint };

  int line;
  int column;
  int length;
  QString severity;
  QString message;
  QString source; // "shellcheck", "syntax", etc.
}; // ← CLOSE THE STRUCT HERE!

// NOW CodeEditor is NOT nested!
class CodeEditor : public QPlainTextEdit {
  Q_OBJECT

public:
  explicit CodeEditor(QWidget *parent = nullptr);
  ~CodeEditor();

  void setTheme(Theme *theme);
  Theme *theme() const { return theme_; }

  // Line number area
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

  // Fold marker area
  void foldingAreaPaintEvent(QPaintEvent *event);
  int foldingAreaWidth();

  // Code folding support
  void enableCodeFolding(bool enable);
  bool isCodeFoldingEnabled() const { return codeFoldingEnabled_; }
  CodeFolding *codeFolding() { return codeFolding_; }

  // Syntax checking
  void enableAutoCheck(bool enable);
  bool isAutoCheckEnabled() const { return autoCheckEnabled_; }
  void setDiagnostics(const QVector<Diagnostic> &diagnostics);
  QVector<Diagnostic> diagnostics() const { return diagnostics_; }

  // Line decorations
  void setLineDecoration(int line, const QColor &color,
                         const QString &tooltip = QString());
  void clearLineDecorations();

  // Public accessors for areas
  QTextBlock getFirstVisibleBlock() const;
  QRectF getBlockBoundingGeometry(const QTextBlock &block) const;
  QPointF getContentOffset() const;
  QRectF getBlockBoundingRect(const QTextBlock &block) const;

  // File info
  void setFilePath(const QString &path) { filePath_ = path; }
  QString filePath() const { return filePath_; }

  // Minimap support (future)
  void enableMinimap(bool enable);

signals:
  void foldToggled(int line, bool folded);
  void diagnosticsChanged();
  void checkRequested(const QString &text);

protected:
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);
  void updateFoldingArea(const QRect &rect, int dy);
  void onTextChanged();
  void triggerAutoCheck();
  void updateDiagnosticHighlights();

private:
  void updateSidebarGeometry();
  bool isPointInFoldMarkerArea(const QPoint &pos, int line);
  void paintDiagnosticUnderlines(QPainter &painter);
  void paintFoldedRegionPlaceholders(QPainter &painter);

  // Sidebar widgets
  LineNumberArea *lineNumberArea_;

  // Code folding
  CodeFolding *codeFolding_;
  bool codeFoldingEnabled_;

  // Theme
  Theme *theme_;

  // Auto syntax checking
  bool autoCheckEnabled_;
  QTimer *autoCheckTimer_;
  QVector<Diagnostic> diagnostics_;

  // Line decorations (for errors, warnings, etc.)
  QMap<int, QPair<QColor, QString>> lineDecorations_;

  // File info
  QString filePath_;

  // Minimap
  bool minimapEnabled_;
};

#endif // CODEEDITOR_H
