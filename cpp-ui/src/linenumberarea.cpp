#include "linenumberarea.h"
#include "codeeditor.h"
#include "codefolding.h"
#include <QMouseEvent>
#include <QPaintEvent>

LineNumberArea::LineNumberArea(CodeEditor *editor)
    : QWidget(editor), codeEditor_(editor) {}

QSize LineNumberArea::sizeHint() const {
  return QSize(codeEditor_->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
  codeEditor_->lineNumberAreaPaintEvent(event);
}

void LineNumberArea::mousePressEvent(QMouseEvent *event) {
  if (!codeEditor_->codeFolding()) {
    return;
  }

  // Calculate which line was clicked
  QTextBlock block = codeEditor_->getFirstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = qRound(codeEditor_->getBlockBoundingGeometry(block)
                       .translated(codeEditor_->getContentOffset())
                       .top());
  int bottom = top + qRound(codeEditor_->getBlockBoundingRect(block).height());

  int clickY = qRound(event->position().y());
  int clickX = qRound(event->position().x());

  while (block.isValid()) {
    if (clickY >= top && clickY < bottom) {
      // Check if click is in fold marker area
      int foldMarkerX = width() - codeEditor_->foldingAreaWidth();
      if (clickX >= foldMarkerX) {
        // Toggle fold at this line
        if (codeEditor_->codeFolding()->isFoldable(blockNumber)) {
          codeEditor_->codeFolding()->toggleFoldAtLine(blockNumber);
          codeEditor_->viewport()->update();
          update();
        }
      }
      break;
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(codeEditor_->getBlockBoundingRect(block).height());
    ++blockNumber;
  }
}
