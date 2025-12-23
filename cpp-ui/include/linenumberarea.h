#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

// Forward declaration
class CodeEditor;
class QPaintEvent;
class QMouseEvent;

/**
 * LineNumberArea - Sidebar widget for displaying line numbers and fold markers
 *
 * This class must be separate (not nested in CodeEditor) because Qt's
 * meta-object compiler (moc) doesn't support Q_OBJECT in nested classes.
 */
class LineNumberArea : public QWidget {
  Q_OBJECT

public:
  explicit LineNumberArea(CodeEditor *editor);

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  CodeEditor *codeEditor_;
};

#endif // LINENUMBERAREA_H
