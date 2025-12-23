#include "foldingarea.h"
#include "codeeditor.h"
#include "codefolding.h"
#include <QMouseEvent>
#include <QPaintEvent>

FoldingArea::FoldingArea(CodeEditor *editor)
    : QWidget(editor), codeEditor_(editor) {}

QSize FoldingArea::sizeHint() const {
  return QSize(codeEditor_->foldingAreaWidth(), 0);
}

void FoldingArea::paintEvent(QPaintEvent *event) {
  codeEditor_->foldingAreaPaintEvent(event);
}

void FoldingArea::mousePressEvent(QMouseEvent *event) {
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

  while (block.isValid()) {
    if (clickY >= top && clickY < bottom) {
      // Toggle fold at this line
      if (codeEditor_->codeFolding()->isFoldable(blockNumber)) {
        codeEditor_->codeFolding()->toggleFoldAtLine(blockNumber);
        codeEditor_->viewport()->update();
        update();
      }
      break;
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(codeEditor_->getBlockBoundingRect(block).height());
    ++blockNumber;
  }
}
