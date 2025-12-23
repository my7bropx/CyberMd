#ifndef FEATUREPANEL_H
#define FEATUREPANEL_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>

class FeaturePanel : public QWidget {
    Q_OBJECT

public:
    explicit FeaturePanel(QWidget *parent = nullptr);
    ~FeaturePanel();

    bool isFileTreeEnabled() const;
    bool isRegexHelperEnabled() const;
    bool isCommandHelperEnabled() const;
    bool isTextProcessingEnabled() const;
    bool isShellCheckerEnabled() const;
    bool isVimModeEnabled() const;

signals:
    void fileTreeToggled(bool enabled);
    void regexHelperToggled(bool enabled);
    void commandHelperToggled(bool enabled);
    void textProcessingToggled(bool enabled);
    void shellCheckerToggled(bool enabled);
    void vimModeToggled(bool enabled);

private:
    void setupUI();

    QCheckBox *fileTreeCheck_;
    QCheckBox *regexHelperCheck_;
    QCheckBox *commandHelperCheck_;
    QCheckBox *textProcessingCheck_;
    QCheckBox *shellCheckerCheck_;
    QCheckBox *vimModeCheck_;
};

#endif // FEATUREPANEL_H
