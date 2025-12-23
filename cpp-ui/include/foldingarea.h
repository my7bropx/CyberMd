#ifndef FOLDINGAREA_H
#define FOLDINGAREA_H

#include <QWidget>

// Forward declaration
class CodeEditor;
class QPaintEvent;
class QMouseEvent;

/**
 * FoldingArea - Sidebar widget for displaying code folding markers
 * 
 * This class must be separate (not nested in CodeEditor) because Qt's 
 * meta-object compiler (moc) doesn't support Q_OBJECT in nested classes.
 */
class FoldingArea : public QWidget {
  Q_OBJECT

public:
  explicit FoldingArea(CodeEditor *editor);

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  CodeEditor *codeEditor_;
};

#endif // FOLDINGAREA_H
