#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H

#include <QDialog>
#include <QTabWidget>
#include <QTextEdit>
#include <QListWidget>

class CommandHelper : public QDialog {
    Q_OBJECT

public:
    explicit CommandHelper(QWidget *parent = nullptr);
    ~CommandHelper();

private:
    void setupUI();
    void addSedExamples();
    void addAwkExamples();
    void addGrepExamples();
    void addCutExamples();
    void addGeneralExamples();

    QTabWidget *tabWidget_;
    QTextEdit *sedHelp_;
    QTextEdit *awkHelp_;
    QTextEdit *grepHelp_;
    QTextEdit *cutHelp_;
    QTextEdit *generalHelp_;
};

#endif // COMMANDHELPER_H
