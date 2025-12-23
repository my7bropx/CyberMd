// /home/my9broxpki/GitProjects/CyberMD/cpp-ui/src: [codeeditor.cpp]

#include "codeeditor.h"
#include "codefolding.h"
#include "foldingarea.h"
#include "linenumberarea.h"
#include "theme.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent), codeFolding_(nullptr), codeFoldingEnabled_(false),
      theme_(nullptr) {
  lineNumberArea_ = new LineNumberArea(this);

  connect(this, &CodeEditor::blockCountChanged, this,
          &CodeEditor::updateLineNumberAreaWidth);
  connect(this, &CodeEditor::updateRequest, this,
          &CodeEditor::updateLineNumberArea);
  connect(this, &CodeEditor::cursorPositionChanged, this,
          &CodeEditor::highlightCurrentLine);
  connect(this, &CodeEditor::textChanged, this, &CodeEditor::onTextChanged);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();

  // Initialize code folding
  codeFolding_ = new CodeFolding(this, this);
  codeFoldingEnabled_ = true;
}

void CodeEditor::setTheme(Theme *theme) {
  theme_ = theme;
  viewport()->update();
}

int CodeEditor::lineNumberAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  // Add space for fold markers if enabled
  if (codeFoldingEnabled_) {
    space += foldingAreaWidth();
  }

  return space;
}

int CodeEditor::foldingAreaWidth() {
  return 16; // Width for fold marker icons
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy)
    lineNumberArea_->scroll(0, dy);
  else
    lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(),
                            rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea_->setGeometry(
      QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = theme_ ? theme_->editorCurrentLine()
                              : QColor(Qt::darkGray).lighter(120);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(lineNumberArea_);
  QColor bgColor = theme_ ? theme_->lineNumberBackground() : QColor(40, 40, 40);
  painter.fillRect(event->rect(), bgColor);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top =
      qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  int foldMarkerWidth = codeFoldingEnabled_ ? foldingAreaWidth() : 0;

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      // Draw line number
      QString number = QString::number(blockNumber + 1);
      QColor fgColor =
          theme_ ? theme_->lineNumberForeground() : QColor(128, 128, 128);
      painter.setPen(fgColor);
      painter.drawText(0, top, lineNumberArea_->width() - foldMarkerWidth - 3,
                       fontMetrics().height(), Qt::AlignRight, number);

      // Draw fold marker if this line is foldable
      if (codeFoldingEnabled_ && codeFolding_ &&
          codeFolding_->isFoldable(blockNumber)) {
        int markerX = lineNumberArea_->width() - foldMarkerWidth;
        int markerY = top + (fontMetrics().height() / 2);

        // Draw fold icon (triangle or +/-)
        QPolygon triangle;
        if (codeFolding_->isFolded(blockNumber)) {
          // Folded: draw right-pointing triangle ▶
          triangle << QPoint(markerX + 4, markerY - 4)
                   << QPoint(markerX + 4, markerY + 4)
                   << QPoint(markerX + 10, markerY);
          painter.setBrush(fgColor);
        } else {
          // Unfolded: draw down-pointing triangle ▼
          triangle << QPoint(markerX + 3, markerY - 2)
                   << QPoint(markerX + 11, markerY - 2)
                   << QPoint(markerX + 7, markerY + 4);
          painter.setBrush(fgColor);
        }

        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);
      }
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

void CodeEditor::foldingAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(
      lineNumberArea_); // Note: currently drawing on lineNumberArea
  QColor bgColor = theme_ ? theme_->lineNumberBackground() : QColor(40, 40, 40);
  painter.fillRect(event->rect(), bgColor);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top =
      qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      // Draw fold marker if this line is foldable
      if (codeFoldingEnabled_ && codeFolding_ &&
          codeFolding_->isFoldable(blockNumber)) {
        int markerX = 4; // Starting position in the folding area
        int markerY = top + (fontMetrics().height() / 2);

        QColor fgColor =
            theme_ ? theme_->lineNumberForeground() : QColor(128, 128, 128);

        // Draw fold icon (triangle)
        QPolygon triangle;
        if (codeFolding_->isFolded(blockNumber)) {
          // Folded: draw right-pointing triangle ▶
          triangle << QPoint(markerX + 2, markerY - 4)
                   << QPoint(markerX + 2, markerY + 4)
                   << QPoint(markerX + 8, markerY);
          painter.setBrush(fgColor);
        } else {
          // Unfolded: draw down-pointing triangle ▼
          triangle << QPoint(markerX + 1, markerY - 2)
                   << QPoint(markerX + 9, markerY - 2)
                   << QPoint(markerX + 5, markerY + 4);
          painter.setBrush(fgColor);
        }

        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);
      }
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

void CodeEditor::keyPressEvent(QKeyEvent *event) {
  QTextCursor cursor = textCursor();

  // Auto-pairing for brackets, quotes, etc.
  QString text = event->text();

  if (text == "(" && !cursor.hasSelection()) {
    cursor.insertText("()");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  } else if (text == "[" && !cursor.hasSelection()) {
    cursor.insertText("[]");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  } else if (text == "{" && !cursor.hasSelection()) {
    cursor.insertText("{}");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  } else if (text == "\"" && !cursor.hasSelection()) {
    cursor.insertText("\"\"");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  } else if (text == "'" && !cursor.hasSelection()) {
    cursor.insertText("''");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  } else if (text == "`" && !cursor.hasSelection()) {
    cursor.insertText("``");
    cursor.movePosition(QTextCursor::Left);
    setTextCursor(cursor);
    return;
  }

  // Handle closing brackets - skip if next char is the same
  if ((text == ")" || text == "]" || text == "}" || text == "\"" ||
       text == "'" || text == "`") &&
      !cursor.hasSelection()) {
    QChar nextChar = document()->characterAt(cursor.position());
    if (nextChar == text[0]) {
      cursor.movePosition(QTextCursor::Right);
      setTextCursor(cursor);
      return;
    }
  }
  // Handle Tab key for indentation
  if (event->key() == Qt::Key_Tab && !cursor.hasSelection()) {
    cursor.insertText("    "); // Insert 4 spaces
    setTextCursor(cursor);
    return;
  }

  // Handle backspace - delete pair if both exist
  if (event->key() == Qt::Key_Backspace && !cursor.hasSelection()) {
    int pos = cursor.position();
    if (pos > 0) {
      QChar prevChar = document()->characterAt(pos - 1);
      QChar nextChar = document()->characterAt(pos);

      bool isPair = (prevChar == '(' && nextChar == ')') ||
                    (prevChar == '[' && nextChar == ']') ||
                    (prevChar == '{' && nextChar == '}') ||
                    (prevChar == '"' && nextChar == '"') ||
                    (prevChar == '\'' && nextChar == '\'') ||
                    (prevChar == '`' && nextChar == '`');

      if (isPair) {
        cursor.deletePreviousChar();
        cursor.deleteChar();
        setTextCursor(cursor);
        return;
      }
    }
  }

  // Auto-indent on Enter
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    QString currentLine = cursor.block().text();
    int indent = 0;
    for (QChar c : currentLine) {
      if (c == ' ')
        indent++;
      else if (c == '\t')
        indent += 4;
      else
        break;
    }

    // Add extra indent if line ends with {, [, or (
    bool addExtraIndent = false;
    QString trimmed = currentLine.trimmed();
    if (trimmed.endsWith('{') || trimmed.endsWith('[') ||
        trimmed.endsWith('(')) {
      addExtraIndent = true;
    }

    QPlainTextEdit::keyPressEvent(event);

    QString indentString = QString(" ").repeated(indent);
    if (addExtraIndent) {
      indentString += "    "; // Add 4 spaces for extra indent
    }
    cursor = textCursor();
    cursor.insertText(indentString);
    setTextCursor(cursor);
    return;
  }

  // Default behavior
  QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::enableCodeFolding(bool enable) {
  codeFoldingEnabled_ = enable;
  updateLineNumberAreaWidth(0);
  viewport()->update();
}

// Public accessor implementations
QTextBlock CodeEditor::getFirstVisibleBlock() const {
  return firstVisibleBlock();
}

QRectF CodeEditor::getBlockBoundingGeometry(const QTextBlock &block) const {
  return blockBoundingGeometry(block);
}

QPointF CodeEditor::getContentOffset() const { return contentOffset(); }

QRectF CodeEditor::getBlockBoundingRect(const QTextBlock &block) const {
  return blockBoundingRect(block);
}

void CodeEditor::onTextChanged() {
  if (codeFoldingEnabled_ && codeFolding_) {
    // Delay fold region analysis to avoid doing it on every keystroke
    static QTimer *foldTimer = nullptr;
    if (!foldTimer) {
      foldTimer = new QTimer(this);
      foldTimer->setSingleShot(true);
      connect(foldTimer, &QTimer::timeout, [this]() {
        if (codeFolding_) {
          codeFolding_->analyzeFoldRegions();
          viewport()->update();
        }
      });
    }
    foldTimer->start(500); // 500ms delay
  }
}

void CodeEditor::mousePressEvent(QMouseEvent *event) {
  QPlainTextEdit::mousePressEvent(event);
}

bool CodeEditor::isPointInFoldMarkerArea(const QPoint &pos, int line) {
  if (!codeFoldingEnabled_ || !codeFolding_) {
    return false;
  }

  QTextBlock block = document()->findBlockByNumber(line);
  if (!block.isValid()) {
    return false;
  }

  int top =
      qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  int markerX = lineNumberArea_->width() - foldingAreaWidth();

  return pos.x() >= markerX && pos.x() < lineNumberArea_->width() &&
         pos.y() >= top && pos.y() < bottom;
}

// Destructor
CodeEditor::~CodeEditor() {
  if (codeFolding_) {
    delete codeFolding_;
    codeFolding_ = nullptr;
  }
}

// Slot: Update folding area (connected via signal)
void CodeEditor::updateFoldingArea(const QRect &rect, int dy) {
  Q_UNUSED(rect)
  Q_UNUSED(dy)
  if (codeFoldingEnabled_) {
    lineNumberArea_->update();
  }
}

// Slot: Trigger auto check (for diagnostics/linting)
void CodeEditor::triggerAutoCheck() {
  // Emit signal or run diagnostics here
  // This could trigger a linter, spell checker, etc.
}

// Slot: Update diagnostic highlights (for errors/warnings)
void CodeEditor::updateDiagnosticHighlights() {
  // Update any error/warning highlights in the editor
  // You can implement syntax error highlighting here
  viewport()->update();
}

// Override: wheelEvent for zoom or scroll
void CodeEditor::wheelEvent(QWheelEvent *event) {
  // Check for Ctrl+Wheel for zoom
  if (event->modifiers() & Qt::ControlModifier) {
    int delta = event->angleDelta().y();
    if (delta > 0) {
      // Zoom in
      zoomIn(1);
    } else if (delta < 0) {
      // Zoom out
      zoomOut(1);
    }
    event->accept();
    return;
  }

  // Default scroll behavior
  QPlainTextEdit::wheelEvent(event);
}

// Override: paintEvent for custom rendering
void CodeEditor::paintEvent(QPaintEvent *event) {
  // Call base class paint first
  QPlainTextEdit::paintEvent(event);

  // Add any custom painting here (e.g., indent guides, minimap, etc.)
  // Example: Draw indent guides

  QPainter painter(viewport());
  QColor guideColor = theme_ ? theme_->indentGuideColor() : QColor(60, 60, 60);
  painter.setPen(guideColor);

  // Draw vertical lines at tab stops
  int tabWidth = fontMetrics().horizontalAdvance("    ");
  QTextBlock block = firstVisibleBlock();
  while (block.isValid() && block.isVisible()) {
    // Draw indent guides logic here
    block = block.next();
  }
}
