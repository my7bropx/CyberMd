#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

class SearchDialog : public QDialog {
  Q_OBJECT

public:
  explicit SearchDialog(QPlainTextEdit *editor, QWidget *parent = nullptr);
  ~SearchDialog();

  void showFind();
  void showReplace();
  void findNext();
  void findPrevious();

private slots:
  void replace();
  void replaceAll();

private:
  void setupUI();
  bool find(bool forward = true);

  QPlainTextEdit *editor_;
  QLineEdit *findLineEdit_;
  QLineEdit *replaceLineEdit_;
  QPushButton *findNextBtn_;
  QPushButton *findPrevBtn_;
  QPushButton *replaceBtn_;
  QPushButton *replaceAllBtn_;
  QCheckBox *caseSensitiveCheck_;
  QCheckBox *wholeWordsCheck_;
  QCheckBox *regexCheck_;

  bool replaceMode_;
};

#endif // SEARCHDIALOG_H
