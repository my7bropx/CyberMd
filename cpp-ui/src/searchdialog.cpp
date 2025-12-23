#include "searchdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextDocument>
#include <QRegularExpression>

SearchDialog::SearchDialog(QPlainTextEdit *editor, QWidget *parent)
    : QDialog(parent),
      editor_(editor),
      findLineEdit_(nullptr),
      replaceLineEdit_(nullptr),
      replaceMode_(false)
{
    setupUI();
    setWindowTitle("Find");
    resize(400, 150);
}

SearchDialog::~SearchDialog() {
}

void SearchDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Find row
    QHBoxLayout *findLayout = new QHBoxLayout();
    findLayout->addWidget(new QLabel("Find:"));
    findLineEdit_ = new QLineEdit(this);
    findLayout->addWidget(findLineEdit_);
    mainLayout->addLayout(findLayout);

    // Replace row
    QHBoxLayout *replaceLayout = new QHBoxLayout();
    replaceLayout->addWidget(new QLabel("Replace:"));
    replaceLineEdit_ = new QLineEdit(this);
    replaceLayout->addWidget(replaceLineEdit_);
    mainLayout->addLayout(replaceLayout);

    // Options
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    caseSensitiveCheck_ = new QCheckBox("Case sensitive", this);
    wholeWordsCheck_ = new QCheckBox("Whole words", this);
    regexCheck_ = new QCheckBox("Regex", this);
    optionsLayout->addWidget(caseSensitiveCheck_);
    optionsLayout->addWidget(wholeWordsCheck_);
    optionsLayout->addWidget(regexCheck_);
    optionsLayout->addStretch();
    mainLayout->addLayout(optionsLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    findPrevBtn_ = new QPushButton("Find Previous", this);
    findNextBtn_ = new QPushButton("Find Next", this);
    replaceBtn_ = new QPushButton("Replace", this);
    replaceAllBtn_ = new QPushButton("Replace All", this);

    buttonLayout->addWidget(findPrevBtn_);
    buttonLayout->addWidget(findNextBtn_);
    buttonLayout->addWidget(replaceBtn_);
    buttonLayout->addWidget(replaceAllBtn_);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(findPrevBtn_, &QPushButton::clicked, this, &SearchDialog::findPrevious);
    connect(findNextBtn_, &QPushButton::clicked, this, &SearchDialog::findNext);
    connect(replaceBtn_, &QPushButton::clicked, this, &SearchDialog::replace);
    connect(replaceAllBtn_, &QPushButton::clicked, this, &SearchDialog::replaceAll);
    connect(findLineEdit_, &QLineEdit::returnPressed, this, &SearchDialog::findNext);

    // Initially hide replace widgets
    showFind();
}

void SearchDialog::showFind() {
    replaceMode_ = false;
    replaceLineEdit_->hide();
    replaceBtn_->hide();
    replaceAllBtn_->hide();
    setWindowTitle("Find");
    findLineEdit_->setFocus();
}

void SearchDialog::showReplace() {
    replaceMode_ = true;
    replaceLineEdit_->show();
    replaceBtn_->show();
    replaceAllBtn_->show();
    setWindowTitle("Find and Replace");
    findLineEdit_->setFocus();
}

bool SearchDialog::find(bool forward) {
    QString searchText = findLineEdit_->text();
    if (searchText.isEmpty()) {
        return false;
    }

    QTextDocument::FindFlags flags;
    if (!forward) {
        flags |= QTextDocument::FindBackward;
    }
    if (caseSensitiveCheck_->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (wholeWordsCheck_->isChecked()) {
        flags |= QTextDocument::FindWholeWords;
    }

    bool found = false;
    if (regexCheck_->isChecked()) {
        QRegularExpression regex(searchText);
        if (!caseSensitiveCheck_->isChecked()) {
            regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
        found = editor_->find(regex, flags);
    } else {
        found = editor_->find(searchText, flags);
    }

    if (!found) {
        // Wrap around
        QTextCursor cursor = editor_->textCursor();
        if (forward) {
            cursor.movePosition(QTextCursor::Start);
        } else {
            cursor.movePosition(QTextCursor::End);
        }
        editor_->setTextCursor(cursor);

        if (regexCheck_->isChecked()) {
            QRegularExpression regex(searchText);
            if (!caseSensitiveCheck_->isChecked()) {
                regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
            }
            found = editor_->find(regex, flags);
        } else {
            found = editor_->find(searchText, flags);
        }
    }

    return found;
}

void SearchDialog::findNext() {
    if (!find(true)) {
        QMessageBox::information(this, "Find", "No matches found.");
    }
}

void SearchDialog::findPrevious() {
    if (!find(false)) {
        QMessageBox::information(this, "Find", "No matches found.");
    }
}

void SearchDialog::replace() {
    QTextCursor cursor = editor_->textCursor();
    if (cursor.hasSelection()) {
        cursor.insertText(replaceLineEdit_->text());
    }
    findNext();
}

void SearchDialog::replaceAll() {
    QString searchText = findLineEdit_->text();
    QString replaceText = replaceLineEdit_->text();

    if (searchText.isEmpty()) {
        return;
    }

    QTextCursor cursor = editor_->textCursor();
    cursor.beginEditBlock();

    // Move to start
    cursor.movePosition(QTextCursor::Start);
    editor_->setTextCursor(cursor);

    int count = 0;
    while (find(true)) {
        QTextCursor current = editor_->textCursor();
        current.insertText(replaceText);
        count++;
    }

    cursor.endEditBlock();

    QMessageBox::information(this, "Replace All",
                            QString("Replaced %1 occurrence(s).").arg(count));
}
