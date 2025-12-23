#include "regexhelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

RegexHelper::RegexHelper(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    addExamples();
    setWindowTitle("Regular Expression Helper");
    resize(800, 600);
}

RegexHelper::~RegexHelper() {
}

void RegexHelper::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Pattern input
    QHBoxLayout *patternLayout = new QHBoxLayout();
    patternLayout->addWidget(new QLabel("Pattern:"));
    patternEdit_ = new QLineEdit(this);
    patternEdit_->setPlaceholderText("Enter regex pattern...");
    patternLayout->addWidget(patternEdit_);
    testBtn_ = new QPushButton("Test", this);
    patternLayout->addWidget(testBtn_);
    mainLayout->addLayout(patternLayout);

    // Options
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    caseSensitiveCheck_ = new QCheckBox("Case Sensitive", this);
    multilineCheck_ = new QCheckBox("Multiline", this);
    optionsLayout->addWidget(caseSensitiveCheck_);
    optionsLayout->addWidget(multilineCheck_);
    optionsLayout->addStretch();
    mainLayout->addLayout(optionsLayout);

    // Test text
    QGroupBox *testGroup = new QGroupBox("Test Text", this);
    QVBoxLayout *testLayout = new QVBoxLayout(testGroup);
    testTextEdit_ = new QTextEdit(this);
    testTextEdit_->setPlaceholderText("Enter text to test against...");
    testLayout->addWidget(testTextEdit_);
    mainLayout->addWidget(testGroup);

    // Results
    QGroupBox *resultGroup = new QGroupBox("Matches", this);
    QVBoxLayout *resultLayout = new QVBoxLayout(resultGroup);
    resultEdit_ = new QTextEdit(this);
    resultEdit_->setReadOnly(true);
    resultEdit_->setStyleSheet("QTextEdit { background-color: #2d2d2d; color: #4EC9B0; }");
    resultLayout->addWidget(resultEdit_);
    mainLayout->addWidget(resultGroup);

    // Examples sidebar
    QGroupBox *examplesGroup = new QGroupBox("Common Patterns", this);
    QVBoxLayout *examplesLayout = new QVBoxLayout(examplesGroup);
    examplesList_ = new QListWidget(this);
    examplesLayout->addWidget(examplesList_);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(examplesGroup, 1);
    bottomLayout->addWidget(resultGroup, 2);

    mainLayout->removeWidget(resultGroup);
    mainLayout->addLayout(bottomLayout);

    // Connections
    connect(testBtn_, &QPushButton::clicked, this, &RegexHelper::testRegex);
    connect(patternEdit_, &QLineEdit::returnPressed, this, &RegexHelper::testRegex);
    connect(examplesList_, &QListWidget::currentRowChanged, this, &RegexHelper::loadExample);
}

void RegexHelper::addExamples() {
    struct Example {
        QString name;
        QString pattern;
        QString description;
    };

    QList<Example> examples = {
        {"Email", R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})", "Matches email addresses"},
        {"URL", R"(https?://[^\s]+)", "Matches HTTP/HTTPS URLs"},
        {"IP Address", R"(\b(?:\d{1,3}\.){3}\d{1,3}\b)", "Matches IPv4 addresses"},
        {"Phone (US)", R"(\(?\d{3}\)?[-.\s]?\d{3}[-.\s]?\d{4})", "US phone numbers"},
        {"Date (YYYY-MM-DD)", R"(\d{4}-\d{2}-\d{2})", "ISO date format"},
        {"Hex Color", R"(#[0-9A-Fa-f]{6})", "Hex color codes"},
        {"Username", R"([a-zA-Z0-9_]{3,16})", "Alphanumeric usernames"},
        {"Password (Strong)", R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z\d]{8,}$)", "Min 8 chars, 1 upper, 1 lower, 1 digit"},
        {"Markdown Link", R"(\[([^\]]+)\]\(([^\)]+)\))", "Markdown link syntax"},
        {"Markdown Heading", R"(^#{1,6}\s+(.+)$)", "Markdown headings"},
        {"Code Block", R"(```(\w+)?\n([\s\S]*?)```)", "Fenced code blocks"},
        {"HTML Tag", R"(<([a-z]+)([^>]*)>)", "HTML opening tags"},
        {"Word Boundary", R"(\b\w+\b)", "Individual words"},
        {"Digits Only", R"(^\d+$)", "Only numeric characters"},
        {"Whitespace", R"(\s+)", "One or more whitespace chars"}
    };

    for (const auto& ex : examples) {
        examplesList_->addItem(ex.name);
    }

    // Store patterns as item data
    for (int i = 0; i < examples.size(); ++i) {
        examplesList_->item(i)->setData(Qt::UserRole, examples[i].pattern);
        examplesList_->item(i)->setData(Qt::UserRole + 1, examples[i].description);
    }
}

void RegexHelper::loadExample(int index) {
    if (index < 0) return;

    QListWidgetItem *item = examplesList_->item(index);
    if (item) {
        QString pattern = item->data(Qt::UserRole).toString();
        QString description = item->data(Qt::UserRole + 1).toString();

        patternEdit_->setText(pattern);
        resultEdit_->setPlainText(description);
    }
}

void RegexHelper::testRegex() {
    QString pattern = patternEdit_->text();
    QString testText = testTextEdit_->toPlainText();

    if (pattern.isEmpty()) {
        resultEdit_->setPlainText("Please enter a pattern.");
        return;
    }

    QRegularExpression regex(pattern);

    // Set options
    QRegularExpression::PatternOptions options;
    if (!caseSensitiveCheck_->isChecked()) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    if (multilineCheck_->isChecked()) {
        options |= QRegularExpression::MultilineOption;
    }
    regex.setPatternOptions(options);

    // Check if pattern is valid
    if (!regex.isValid()) {
        resultEdit_->setPlainText(QString("Invalid pattern: %1\nError: %2")
                                 .arg(pattern)
                                 .arg(regex.errorString()));
        return;
    }

    // Find all matches
    QRegularExpressionMatchIterator it = regex.globalMatch(testText);
    QStringList results;
    int matchCount = 0;

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        matchCount++;

        QString matchInfo = QString("Match #%1:\n").arg(matchCount);
        matchInfo += QString("  Full match: \"%1\"\n").arg(match.captured(0));
        matchInfo += QString("  Position: %1-%2\n")
                    .arg(match.capturedStart(0))
                    .arg(match.capturedEnd(0));

        // Show captured groups
        for (int i = 1; i <= match.lastCapturedIndex(); ++i) {
            matchInfo += QString("  Group %1: \"%2\"\n")
                        .arg(i)
                        .arg(match.captured(i));
        }

        results << matchInfo;
    }

    if (matchCount == 0) {
        resultEdit_->setPlainText("No matches found.");
    } else {
        QString summary = QString("Found %1 match(es):\n\n").arg(matchCount);
        resultEdit_->setPlainText(summary + results.join("\n"));
    }
}
