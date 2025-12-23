#include "vimmode.h"
#include <QApplication>
#include <QTextBlock>
#include <QScrollBar>

VimMode::VimMode(QPlainTextEdit *editor, QObject *parent)
    : QObject(parent),
      editor_(editor),
      mode_(Normal),
      enabled_(false),
      repeatCount_(0)
{
    if (editor_) {
        editor_->installEventFilter(this);
        // Don't set read-only here - only when VimMode is explicitly enabled
    }
}

VimMode::~VimMode() {
    if (editor_) {
        editor_->removeEventFilter(this);
    }
}

void VimMode::setEnabled(bool enabled) {
    enabled_ = enabled;
    if (enabled) {
        setMode(Normal);
    } else {
        // Disable VimMode - make editor editable and remove read-only
        if (editor_) {
            editor_->setReadOnly(false);
        }
        mode_ = Insert;  // Set to Insert mode internally but don't emit signal
    }
}

bool VimMode::eventFilter(QObject *obj, QEvent *event) {
    if (!enabled_ || obj != editor_ || event->type() != QEvent::KeyPress) {
        return QObject::eventFilter(obj, event);
    }

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

    switch (mode_) {
        case Normal:
            handleNormalMode(keyEvent);
            return true;
        case Insert:
            handleInsertMode(keyEvent);
            return false;  // Allow normal typing in insert mode
        case Visual:
            handleVisualMode(keyEvent);
            return true;
        case Command:
            handleCommandMode(keyEvent);
            return true;
    }

    return false;
}

void VimMode::setMode(Mode mode) {
    mode_ = mode;

    // Update editor read-only state based on mode
    // Only make read-only if VimMode is enabled
    if (editor_) {
        if (enabled_) {
            editor_->setReadOnly(mode == Normal || mode == Visual || mode == Command);
        } else {
            editor_->setReadOnly(false);
        }
    }

    emit modeChanged(mode);
}

void VimMode::handleNormalMode(QKeyEvent *event) {
    QString key = event->text();
    int keyCode = event->key();

    // Handle repeat count (e.g., 5j moves down 5 lines)
    if (key >= "1" && key <= "9" && repeatCount_ == 0) {
        repeatCount_ = key.toInt();
        return;
    } else if (key >= "0" && key <= "9" && repeatCount_ > 0) {
        repeatCount_ = repeatCount_ * 10 + key.toInt();
        return;
    }

    int count = (repeatCount_ > 0) ? repeatCount_ : 1;
    repeatCount_ = 0;

    // Navigation
    if (key == "h" || keyCode == Qt::Key_Left) {
        for (int i = 0; i < count; ++i) moveLeft();
    } else if (key == "j" || keyCode == Qt::Key_Down) {
        for (int i = 0; i < count; ++i) moveDown();
    } else if (key == "k" || keyCode == Qt::Key_Up) {
        for (int i = 0; i < count; ++i) moveUp();
    } else if (key == "l" || keyCode == Qt::Key_Right) {
        for (int i = 0; i < count; ++i) moveRight();
    } else if (key == "w") {
        for (int i = 0; i < count; ++i) moveWordForward();
    } else if (key == "b") {
        for (int i = 0; i < count; ++i) moveWordBackward();
    } else if (key == "0" || keyCode == Qt::Key_Home) {
        moveLineStart();
    } else if (key == "$" || keyCode == Qt::Key_End) {
        moveLineEnd();
    } else if (key == "g" && event->modifiers() == Qt::NoModifier) {
        // Wait for second 'g'
        // Simplified: just go to start
        moveDocumentStart();
    } else if (key == "G") {
        moveDocumentEnd();
    }

    // Insert mode
    else if (key == "i") {
        enterInsertMode();
    } else if (key == "I") {
        enterInsertModeLineStart();
    } else if (key == "a") {
        enterInsertModeAfter();
    } else if (key == "A") {
        enterInsertModeLineEnd();
    } else if (key == "o") {
        enterInsertModeNewLineBelow();
    } else if (key == "O") {
        enterInsertModeNewLineAbove();
    }

    // Editing
    else if (key == "x") {
        // Delete character under cursor
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        for (int i = 0; i < count; ++i) {
            cursor.deleteChar();
        }
        editor_->setTextCursor(cursor);
    } else if (key == "d") {
        // Wait for next key (dd, dw, etc.)
        // Simplified: treat as delete line
        deleteLine();
    } else if (key == "y") {
        // Yank (copy) line
        yankLine();
    } else if (key == "p") {
        paste();
    }

    // Undo/Redo
    else if (key == "u") {
        undo();
    } else if (keyCode == Qt::Key_R && (event->modifiers() & Qt::ControlModifier)) {
        redo();
    }

    // Visual mode
    else if (key == "v") {
        setMode(Visual);
    }

    // Command mode
    else if (key == ":") {
        setMode(Command);
        commandBuffer_ = ":";
    }

    // Escape (safety)
    else if (keyCode == Qt::Key_Escape) {
        // Already in normal mode, do nothing
    }
}

void VimMode::handleInsertMode(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        setMode(Normal);
        // Move cursor back one position (VIM behavior)
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        if (cursor.position() > 0) {
            cursor.movePosition(QTextCursor::Left);
            editor_->setTextCursor(cursor);
        }
    }
    // Otherwise, allow normal text input (return false in eventFilter)
}

void VimMode::handleVisualMode(QKeyEvent *event) {
    QString key = event->text();
    int keyCode = event->key();

    if (keyCode == Qt::Key_Escape) {
        // Return to normal mode
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        cursor.clearSelection();
        editor_->setTextCursor(cursor);
        setMode(Normal);
    } else if (key == "y") {
        // Yank selection
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        yankBuffer_ = cursor.selectedText();
        cursor.clearSelection();
        editor_->setTextCursor(cursor);
        setMode(Normal);
    } else if (key == "d" || key == "x") {
        // Delete selection
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        yankBuffer_ = cursor.selectedText();
        cursor.removeSelectedText();
        editor_->setTextCursor(cursor);
        setMode(Normal);
    } else {
        // Navigation in visual mode extends selection
        if (!editor_) return;
        QTextCursor cursor = editor_->textCursor();
        QTextCursor::MoveMode moveMode = QTextCursor::KeepAnchor;

        if (key == "h" || keyCode == Qt::Key_Left) {
            cursor.movePosition(QTextCursor::Left, moveMode);
        } else if (key == "j" || keyCode == Qt::Key_Down) {
            cursor.movePosition(QTextCursor::Down, moveMode);
        } else if (key == "k" || keyCode == Qt::Key_Up) {
            cursor.movePosition(QTextCursor::Up, moveMode);
        } else if (key == "l" || keyCode == Qt::Key_Right) {
            cursor.movePosition(QTextCursor::Right, moveMode);
        } else if (key == "w") {
            cursor.movePosition(QTextCursor::NextWord, moveMode);
        } else if (key == "b") {
            cursor.movePosition(QTextCursor::PreviousWord, moveMode);
        } else if (key == "0") {
            cursor.movePosition(QTextCursor::StartOfLine, moveMode);
        } else if (key == "$") {
            cursor.movePosition(QTextCursor::EndOfLine, moveMode);
        }

        editor_->setTextCursor(cursor);
    }
}

void VimMode::handleCommandMode(QKeyEvent *event) {
    int keyCode = event->key();

    if (keyCode == Qt::Key_Escape) {
        commandBuffer_.clear();
        setMode(Normal);
    } else if (keyCode == Qt::Key_Return || keyCode == Qt::Key_Enter) {
        executeCommand(commandBuffer_);
        commandBuffer_.clear();
        setMode(Normal);
    } else if (keyCode == Qt::Key_Backspace) {
        if (commandBuffer_.length() > 1) {
            commandBuffer_.chop(1);
        } else {
            setMode(Normal);
        }
    } else if (!event->text().isEmpty()) {
        commandBuffer_ += event->text();
    }
}

void VimMode::executeCommand(const QString &command) {
    if (command == ":w") {
        // Save file (emit signal or call parent method)
        // For now, do nothing - parent should handle
    } else if (command == ":q") {
        // Quit - close editor
        if (editor_ && editor_->window()) {
            editor_->window()->close();
        }
    } else if (command == ":wq") {
        // Save and quit
        // Parent should handle
    }
}

// Navigation methods
void VimMode::moveLeft() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Left);
    editor_->setTextCursor(cursor);
}

void VimMode::moveRight() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Right);
    editor_->setTextCursor(cursor);
}

void VimMode::moveUp() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Up);
    editor_->setTextCursor(cursor);
}

void VimMode::moveDown() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Down);
    editor_->setTextCursor(cursor);
}

void VimMode::moveWordForward() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::NextWord);
    editor_->setTextCursor(cursor);
}

void VimMode::moveWordBackward() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::PreviousWord);
    editor_->setTextCursor(cursor);
}

void VimMode::moveLineStart() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    editor_->setTextCursor(cursor);
}

void VimMode::moveLineEnd() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    editor_->setTextCursor(cursor);
}

void VimMode::moveDocumentStart() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Start);
    editor_->setTextCursor(cursor);
}

void VimMode::moveDocumentEnd() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::End);
    editor_->setTextCursor(cursor);
}

// Editing methods
void VimMode::deleteLine() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor); // Include newline
    yankBuffer_ = cursor.selectedText();
    cursor.removeSelectedText();
    editor_->setTextCursor(cursor);
}

void VimMode::deleteWord() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    yankBuffer_ = cursor.selectedText();
    cursor.removeSelectedText();
    editor_->setTextCursor(cursor);
}

void VimMode::yankLine() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor); // Include newline
    yankBuffer_ = cursor.selectedText();
}

void VimMode::paste() {
    if (!editor_ || yankBuffer_.isEmpty()) {
        return;
    }

    QTextCursor cursor = editor_->textCursor();
    cursor.insertText(yankBuffer_);
    editor_->setTextCursor(cursor);
}

void VimMode::undo() {
    if (!editor_) return;
    editor_->undo();
}

void VimMode::redo() {
    if (!editor_) return;
    editor_->redo();
}

// Insert mode methods
void VimMode::enterInsertMode() {
    setMode(Insert);
}

void VimMode::enterInsertModeAfter() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::Right);
    editor_->setTextCursor(cursor);
    setMode(Insert);
}

void VimMode::enterInsertModeNewLineBelow() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::EndOfBlock);
    cursor.insertText("\n");
    editor_->setTextCursor(cursor);
    setMode(Insert);
}

void VimMode::enterInsertModeNewLineAbove() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.insertText("\n");
    cursor.movePosition(QTextCursor::Left);
    editor_->setTextCursor(cursor);
    setMode(Insert);
}

void VimMode::enterInsertModeLineStart() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    editor_->setTextCursor(cursor);
    setMode(Insert);
}

void VimMode::enterInsertModeLineEnd() {
    if (!editor_) return;
    QTextCursor cursor = editor_->textCursor();
    cursor.movePosition(QTextCursor::EndOfBlock);
    editor_->setTextCursor(cursor);
    setMode(Insert);
}
