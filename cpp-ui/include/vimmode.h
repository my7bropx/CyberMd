#ifndef VIMMODE_H
#define VIMMODE_H

#include <QObject>
#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QString>
#include <QTextCursor>

class VimMode : public QObject {
    Q_OBJECT

public:
    enum Mode {
        Normal,
        Insert,
        Visual,
        Command
    };

    explicit VimMode(QPlainTextEdit *editor, QObject *parent = nullptr);
    ~VimMode();

    bool eventFilter(QObject *obj, QEvent *event) override;

    Mode currentMode() const { return mode_; }
    bool isEnabled() const { return enabled_; }
    void setEnabled(bool enabled);

signals:
    void modeChanged(VimMode::Mode mode);

private:
    void handleNormalMode(QKeyEvent *event);
    void handleInsertMode(QKeyEvent *event);
    void handleVisualMode(QKeyEvent *event);
    void handleCommandMode(QKeyEvent *event);

    void setMode(Mode mode);
    void executeCommand(const QString &command);

    // VIM commands
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveWordForward();
    void moveWordBackward();
    void moveLineStart();
    void moveLineEnd();
    void moveDocumentStart();
    void moveDocumentEnd();

    void deleteLine();
    void deleteWord();
    void yankLine();
    void paste();
    void undo();
    void redo();

    void enterInsertMode();
    void enterInsertModeAfter();
    void enterInsertModeNewLineBelow();
    void enterInsertModeNewLineAbove();
    void enterInsertModeLineStart();
    void enterInsertModeLineEnd();

    QPlainTextEdit *editor_;
    Mode mode_;
    bool enabled_;
    QString yankBuffer_;
    QString commandBuffer_;
    int repeatCount_;
};

#endif // VIMMODE_H
