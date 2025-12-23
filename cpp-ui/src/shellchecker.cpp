#include "shellchecker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QRegularExpression>

ShellChecker::ShellChecker(QWidget *parent)
    : QDialog(parent), process_(nullptr)
{
    setupUI();
    setWindowTitle("Shell Script Checker");
    resize(900, 700);
}

ShellChecker::~ShellChecker() {
    if (process_) {
        process_->kill();
        process_->waitForFinished();
        delete process_;
    }
}

void ShellChecker::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Shell type selector
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *shellLabel = new QLabel("Shell Type:", this);
    shellTypeCombo_ = new QComboBox(this);
    shellTypeCombo_->addItem("bash");
    shellTypeCombo_->addItem("sh");
    shellTypeCombo_->addItem("zsh");
    shellTypeCombo_->addItem("ksh");
    topLayout->addWidget(shellLabel);
    topLayout->addWidget(shellTypeCombo_);
    topLayout->addStretch();

    checkButton_ = new QPushButton("Check Script", this);
    connect(checkButton_, &QPushButton::clicked, this, &ShellChecker::onCheckClicked);
    topLayout->addWidget(checkButton_);

    mainLayout->addLayout(topLayout);

    // Script input
    QLabel *inputLabel = new QLabel("Script Content:", this);
    mainLayout->addWidget(inputLabel);

    scriptInput_ = new QTextEdit(this);
    scriptInput_->setPlaceholderText("Paste your shell script here...");
    mainLayout->addWidget(scriptInput_, 1);

    // Results output
    QLabel *resultsLabel = new QLabel("Check Results:", this);
    mainLayout->addWidget(resultsLabel);

    resultsOutput_ = new QTextEdit(this);
    resultsOutput_->setReadOnly(true);
    mainLayout->addWidget(resultsOutput_, 1);

    // Style
    QString style = R"(
        QTextEdit {
            background-color: #1e1e1e;
            color: #d4d4d4;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 12px;
            border: 1px solid #3e3e3e;
        }
        QPushButton {
            background-color: #0e639c;
            color: white;
            border: none;
            padding: 8px 16px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #1177bb;
        }
        QComboBox {
            background-color: #3c3c3c;
            color: #d4d4d4;
            border: 1px solid #3e3e3e;
            padding: 4px;
        }
    )";
    setStyleSheet(style);
}

void ShellChecker::checkScript(const QString& scriptContent) {
    scriptInput_->setPlainText(scriptContent);
    onCheckClicked();
}

void ShellChecker::onCheckClicked() {
    QString scriptContent = scriptInput_->toPlainText();

    if (scriptContent.trimmed().isEmpty()) {
        resultsOutput_->setPlainText("No script content to check.");
        return;
    }

    resultsOutput_->setPlainText("Checking script...\n");

    // Try to run shellcheck first
    runShellcheck(scriptContent);
}

void ShellChecker::runShellcheck(const QString& scriptContent) {
    // Check if shellcheck is available
    QProcess checkProcess;
    checkProcess.start("which", QStringList() << "shellcheck");
    checkProcess.waitForFinished();

    if (checkProcess.exitCode() != 0) {
        // shellcheck not found, use manual checks
        resultsOutput_->append("Note: shellcheck not found, using basic validation.\n");
        resultsOutput_->append("Install shellcheck for comprehensive analysis:\n");
        resultsOutput_->append("  - Debian/Ubuntu: sudo apt install shellcheck\n");
        resultsOutput_->append("  - Arch: sudo pacman -S shellcheck\n");
        resultsOutput_->append("  - macOS: brew install shellcheck\n\n");
        performManualChecks(scriptContent);
        return;
    }

    // Create temporary file for shellcheck
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(true);

    if (!tempFile.open()) {
        resultsOutput_->append("Error: Could not create temporary file.\n");
        performManualChecks(scriptContent);
        return;
    }

    tempFile.write(scriptContent.toUtf8());
    tempFile.flush();

    // Run shellcheck
    if (process_) {
        delete process_;
    }

    process_ = new QProcess(this);
    connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ShellChecker::onProcessFinished);
    connect(process_, &QProcess::errorOccurred, this, &ShellChecker::onProcessError);

    QStringList args;
    args << "-f" << "gcc";  // GCC-style output
    args << "-s" << shellTypeCombo_->currentText();
    args << tempFile.fileName();

    process_->start("shellcheck", args);
}

void ShellChecker::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit) {
        QString output = process_->readAllStandardOutput();
        QString errors = process_->readAllStandardError();

        if (exitCode == 0) {
            resultsOutput_->append("✓ No issues found!\n");
            resultsOutput_->append("Script follows shell scripting best practices.\n");
        } else {
            resultsOutput_->append("Issues found:\n\n");

            if (!output.isEmpty()) {
                // Parse and format shellcheck output
                QStringList lines = output.split('\n', Qt::SkipEmptyParts);
                for (const QString& line : lines) {
                    // shellcheck output format: file:line:column: level: message [SCxxxx]
                    QRegularExpression re(R"(.*:(\d+):(\d+):\s+(\w+):\s+(.+)\s+\[SC(\d+)\])");
                    QRegularExpressionMatch match = re.match(line);

                    if (match.hasMatch()) {
                        QString lineNum = match.captured(1);
                        QString col = match.captured(2);
                        QString level = match.captured(3);
                        QString message = match.captured(4);
                        QString code = match.captured(5);

                        QString icon = "•";
                        if (level == "error") icon = "✗";
                        else if (level == "warning") icon = "⚠";
                        else if (level == "note") icon = "ℹ";

                        resultsOutput_->append(QString("%1 Line %2:%3 [%4] %5\n")
                            .arg(icon).arg(lineNum).arg(col).arg(level).arg(message));
                        resultsOutput_->append(QString("   Code: SC%1\n").arg(code));
                    } else {
                        resultsOutput_->append(line + "\n");
                    }
                }
            }

            if (!errors.isEmpty()) {
                resultsOutput_->append("\nErrors:\n" + errors + "\n");
            }
        }

        // Also run manual checks
        resultsOutput_->append("\n--- Additional Checks ---\n");
        performManualChecks(scriptInput_->toPlainText());
    }
}

void ShellChecker::onProcessError(QProcess::ProcessError error) {
    resultsOutput_->append(QString("\nProcess error: %1\n").arg(error));
    resultsOutput_->append("Falling back to manual checks...\n\n");
    performManualChecks(scriptInput_->toPlainText());
}

void ShellChecker::performManualChecks(const QString& scriptContent) {
    QStringList issues;
    QStringList warnings;
    QStringList suggestions;

    QStringList lines = scriptContent.split('\n');

    // Check for shebang
    if (!lines.isEmpty() && !lines[0].startsWith("#!")) {
        warnings << "Missing shebang (#!/bin/bash or #!/bin/sh) at the beginning";
    }

    // Check each line
    for (int i = 0; i < lines.size(); ++i) {
        const QString& line = lines[i];
        int lineNum = i + 1;

        // Skip empty lines and comments
        if (line.trimmed().isEmpty() || line.trimmed().startsWith('#')) {
            continue;
        }

        // Check for unquoted variables
        QRegularExpression unquotedVar(R"(\$\{?[A-Za-z_][A-Za-z0-9_]*\}?(?![\"']))");
        QRegularExpressionMatchIterator it = unquotedVar.globalMatch(line);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            // Make sure it's not already in quotes
            int pos = match.capturedStart();
            bool inQuotes = false;
            for (int j = 0; j < pos; ++j) {
                if (line[j] == '"' || line[j] == '\'') {
                    inQuotes = !inQuotes;
                }
            }
            if (!inQuotes && !line.contains("case") && !line.contains("=")) {
                suggestions << QString("Line %1: Consider quoting variable: %2")
                    .arg(lineNum).arg(match.captured());
                break;
            }
        }

        // Check for [ vs [[
        if (line.contains(QRegularExpression(R"(\[\s+[^\]]+\s+\])"))) {
            suggestions << QString("Line %1: Consider using [[ ]] instead of [ ] for better error handling")
                .arg(lineNum);
        }

        // Check for missing 'set -e' or 'set -u'
        if (lineNum <= 10 && i == 0 && !scriptContent.contains("set -e") && !scriptContent.contains("set -o errexit")) {
            suggestions << "Consider adding 'set -e' to exit on errors";
        }

        // Check for == in [ ]
        if (line.contains(QRegularExpression(R"(\[\s+.*==.*\])"))) {
            warnings << QString("Line %1: Use '=' instead of '==' in [ ] tests")
                .arg(lineNum);
        }

        // Check for 'which' command
        if (line.contains(QRegularExpression(R"(\bwhich\s+)"))) {
            suggestions << QString("Line %1: Consider using 'command -v' instead of 'which'")
                .arg(lineNum);
        }

        // Check for backticks
        if (line.contains('`')) {
            suggestions << QString("Line %1: Consider using $() instead of backticks")
                .arg(lineNum);
        }

        // Check for cd without error checking
        if (line.contains(QRegularExpression(R"(\bcd\s+)"))) {
            QString nextLine = (i + 1 < lines.size()) ? lines[i + 1] : "";
            if (!nextLine.contains("||") && !scriptContent.mid(0, line.length() * lineNum).contains("set -e")) {
                suggestions << QString("Line %1: Consider checking 'cd' result: cd ... || exit 1")
                    .arg(lineNum);
            }
        }

        // Check for $?
        if (line.contains("$?")) {
            suggestions << QString("Line %1: $? is fragile, consider using 'if command; then' directly")
                .arg(lineNum);
        }
    }

    // Display results
    if (issues.isEmpty() && warnings.isEmpty() && suggestions.isEmpty()) {
        resultsOutput_->append("✓ Basic checks passed!\n");
    } else {
        if (!issues.isEmpty()) {
            resultsOutput_->append("✗ Issues:\n");
            for (const QString& issue : issues) {
                resultsOutput_->append("  " + issue + "\n");
            }
            resultsOutput_->append("\n");
        }

        if (!warnings.isEmpty()) {
            resultsOutput_->append("⚠ Warnings:\n");
            for (const QString& warning : warnings) {
                resultsOutput_->append("  " + warning + "\n");
            }
            resultsOutput_->append("\n");
        }

        if (!suggestions.isEmpty()) {
            resultsOutput_->append("ℹ Suggestions:\n");
            for (const QString& suggestion : suggestions) {
                resultsOutput_->append("  " + suggestion + "\n");
            }
        }
    }
}
