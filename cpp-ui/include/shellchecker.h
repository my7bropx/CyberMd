#ifndef SHELLCHECKER_H
#define SHELLCHECKER_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QProcess>

class ShellChecker : public QDialog {
    Q_OBJECT

public:
    explicit ShellChecker(QWidget *parent = nullptr);
    ~ShellChecker();

    void checkScript(const QString& scriptContent);

private slots:
    void onCheckClicked();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

private:
    void setupUI();
    void runShellcheck(const QString& scriptContent);
    void performManualChecks(const QString& scriptContent);

    QTextEdit *scriptInput_;
    QTextEdit *resultsOutput_;
    QPushButton *checkButton_;
    QComboBox *shellTypeCombo_;
    QProcess *process_;
};

#endif // SHELLCHECKER_H
