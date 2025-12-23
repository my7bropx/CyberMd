#include "codefolding.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QRegularExpression>

CodeFolding::CodeFolding(QPlainTextEdit *editor, QObject *parent)
    : QObject(parent), editor_(editor)
{
}

CodeFolding::~CodeFolding() {
}

void CodeFolding::analyzeFoldRegions() {
    if (!editor_) return;

    foldRegions_.clear();
    QTextDocument *doc = editor_->document();
    QTextBlock block = doc->begin();
    int lineNum = 0;

    while (block.isValid()) {
        QString text = block.text();
        int headerLevel = 0;

        if (isHeader(text, headerLevel)) {
            // Find where this header's content ends
            int endLine = findHeaderEndLine(lineNum, headerLevel);
            if (endLine > lineNum) {
                FoldRegion region;
                region.startLine = lineNum;
                region.endLine = endLine;
                region.isFolded = false;
                region.foldType = "header";
                foldRegions_[lineNum] = region;
            }
        } else if (isCodeBlockDelimiter(text)) {
            // Find the closing delimiter
            int endLine = findCodeBlockEndLine(lineNum);
            if (endLine > lineNum) {
                FoldRegion region;
                region.startLine = lineNum;
                region.endLine = endLine;
                region.isFolded = false;
                region.foldType = "codeblock";
                foldRegions_[lineNum] = region;
            }
        }

        block = block.next();
        lineNum++;
    }
}

bool CodeFolding::isHeader(const QString& text, int& level) {
    QRegularExpression headerRegex(R"(^(#{1,6})\s+.+)");
    QRegularExpressionMatch match = headerRegex.match(text);
    if (match.hasMatch()) {
        level = match.captured(1).length();
        return true;
    }
    return false;
}

bool CodeFolding::isCodeBlockDelimiter(const QString& text) {
    QString trimmed = text.trimmed();
    return trimmed.startsWith("```") || trimmed.startsWith("~~~");
}

bool CodeFolding::isListItem(const QString& text) {
    QString trimmed = text.trimmed();
    return trimmed.startsWith("- ") ||
           trimmed.startsWith("* ") ||
           trimmed.startsWith("+ ") ||
           QRegularExpression(R"(^\d+\.\s)").match(trimmed).hasMatch();
}

int CodeFolding::findHeaderEndLine(int startLine, int headerLevel) {
    QTextDocument *doc = editor_->document();
    QTextBlock block = doc->findBlockByNumber(startLine);
    block = block.next();
    int lineNum = startLine + 1;

    while (block.isValid()) {
        QString text = block.text();
        int currentLevel = 0;

        // Stop if we find a header of equal or higher level
        if (isHeader(text, currentLevel) && currentLevel <= headerLevel) {
            return lineNum - 1;
        }

        block = block.next();
        lineNum++;
    }

    return lineNum - 1;
}

int CodeFolding::findCodeBlockEndLine(int startLine) {
    QTextDocument *doc = editor_->document();
    QTextBlock block = doc->findBlockByNumber(startLine);
    QString startDelimiter = block.text().trimmed();
    block = block.next();
    int lineNum = startLine + 1;

    while (block.isValid()) {
        QString text = block.text().trimmed();

        // Look for closing delimiter
        if (text.startsWith("```") || text.startsWith("~~~")) {
            return lineNum;
        }

        block = block.next();
        lineNum++;
    }

    return lineNum - 1;
}

int CodeFolding::findListEndLine(int startLine) {
    QTextDocument *doc = editor_->document();
    QTextBlock block = doc->findBlockByNumber(startLine);
    block = block.next();
    int lineNum = startLine + 1;

    while (block.isValid()) {
        QString text = block.text();

        // Stop if we encounter a non-list line (and it's not empty)
        if (!text.trimmed().isEmpty() && !isListItem(text)) {
            return lineNum - 1;
        }

        block = block.next();
        lineNum++;
    }

    return lineNum - 1;
}

bool CodeFolding::isFoldable(int line) const {
    return foldRegions_.contains(line);
}

bool CodeFolding::isFolded(int line) const {
    return foldedLines_.contains(line);
}

void CodeFolding::toggleFoldAtLine(int line) {
    if (!foldRegions_.contains(line)) {
        return;
    }

    const FoldRegion& region = foldRegions_[line];

    if (foldedLines_.contains(line)) {
        // Unfold
        unfoldRegion(line);
        foldedLines_.remove(line);
    } else {
        // Fold
        foldRegion(region.startLine, region.endLine);
        foldedLines_.insert(line);
    }
}

void CodeFolding::foldRegion(int startLine, int endLine) {
    if (!editor_) return;

    QTextDocument *doc = editor_->document();

    // Hide all blocks between startLine+1 and endLine (inclusive)
    for (int line = startLine + 1; line <= endLine; ++line) {
        QTextBlock block = doc->findBlockByNumber(line);
        if (block.isValid()) {
            block.setVisible(false);
        }
    }

    // Force document layout update
    editor_->document()->markContentsDirty(
        doc->findBlockByNumber(startLine).position(),
        doc->findBlockByNumber(endLine).position()
    );

    // Update viewport
    editor_->viewport()->update();
}

void CodeFolding::unfoldRegion(int startLine) {
    if (!foldRegions_.contains(startLine) || !editor_) {
        return;
    }

    QTextDocument *doc = editor_->document();
    const FoldRegion& region = foldRegions_[startLine];

    // Show all blocks between startLine+1 and endLine (inclusive)
    for (int line = startLine + 1; line <= region.endLine; ++line) {
        QTextBlock block = doc->findBlockByNumber(line);
        if (block.isValid()) {
            block.setVisible(true);
        }
    }

    // Force document layout update
    editor_->document()->markContentsDirty(
        doc->findBlockByNumber(startLine).position(),
        doc->findBlockByNumber(region.endLine).position()
    );

    // Update viewport
    editor_->viewport()->update();
}

void CodeFolding::foldAll() {
    for (auto it = foldRegions_.begin(); it != foldRegions_.end(); ++it) {
        int startLine = it.key();
        if (!foldedLines_.contains(startLine)) {
            const FoldRegion& region = it.value();
            foldRegion(region.startLine, region.endLine);
            foldedLines_.insert(startLine);
        }
    }
}

void CodeFolding::unfoldAll() {
    for (int line : foldedLines_) {
        unfoldRegion(line);
    }
    foldedLines_.clear();
}

void CodeFolding::updateFolding() {
    analyzeFoldRegions();
}
