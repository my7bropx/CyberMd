#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QMap>
#include <QString>
#include <QFileInfo>
#include <QTextDocument>

class CodeEditor;
class Theme;
class BaseSyntaxHighlighter;

// Custom TabBar with close buttons and styling
class EditorTabBar : public QTabBar {
    Q_OBJECT

public:
    explicit EditorTabBar(QWidget *parent = nullptr);
    void setTheme(Theme *theme);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize tabSizeHint(int index) const override;

private:
    Theme *theme_;
};

// Tab info structure
struct TabInfo {
    QString filePath;
    QString fileName;
    bool isModified;
    bool isUntitled;
    BaseSyntaxHighlighter *highlighter;
    
    TabInfo() : isModified(false), isUntitled(true), highlighter(nullptr) {}
};

// Main tab widget for managing multiple editor tabs
class EditorTabWidget : public QTabWidget {
    Q_OBJECT

public:
    explicit EditorTabWidget(QWidget *parent = nullptr);
    ~EditorTabWidget();

    // Tab management
    int newTab(const QString &filePath = QString());
    bool openFile(const QString &filePath);
    bool saveTab(int index = -1);
    bool saveTabAs(int index = -1);
    bool saveAllTabs();
    bool closeTab(int index);
    bool closeAllTabs();
    
    // Current editor access
    CodeEditor* currentEditor() const;
    CodeEditor* editorAt(int index) const;
    QString currentFilePath() const;
    QString filePathAt(int index) const;
    
    // Tab state
    bool isModified(int index = -1) const;
    bool hasUnsavedChanges() const;
    int findTabByPath(const QString &filePath) const;
    
    // Theme support
    void setTheme(Theme *theme);
    
    // Apply highlighter to specific tab
    void applyHighlighter(int index, const QString &filePath);

signals:
    void tabModified(int index, bool modified);
    void currentFileChanged(const QString &filePath);
    void tabCountChanged(int count);
    void editorChanged(CodeEditor *editor);

public slots:
    void updateTabTitle(int index);
    void onTextChanged();

private slots:
    void onTabCloseRequested(int index);
    void onCurrentChanged(int index);
    void onTabMoved(int from, int to);

private:
    void setupUI();
    void updateTabIcon(int index);
    QString getLanguageFromPath(const QString &filePath);
    QIcon getFileIcon(const QString &filePath);
    BaseSyntaxHighlighter* createHighlighter(const QString &filePath, QTextDocument *doc);

    EditorTabBar *tabBar_;
    QMap<int, TabInfo> tabInfoMap_;
    Theme *theme_;
    int untitledCounter_;
};

#endif // TABWIDGET_H
