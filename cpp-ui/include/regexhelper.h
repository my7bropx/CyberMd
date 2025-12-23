#ifndef REGEXHELPER_H
#define REGEXHELPER_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QListWidget>

class RegexHelper : public QDialog {
    Q_OBJECT

public:
    explicit RegexHelper(QWidget *parent = nullptr);
    ~RegexHelper();

private slots:
    void testRegex();
    void loadExample(int index);

private:
    void setupUI();
    void addExamples();

    QLineEdit *patternEdit_;
    QTextEdit *testTextEdit_;
    QTextEdit *resultEdit_;
    QCheckBox *caseSensitiveCheck_;
    QCheckBox *multilineCheck_;
    QListWidget *examplesList_;
    QPushButton *testBtn_;
};

#endif // REGEXHELPER_H
