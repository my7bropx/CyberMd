#ifndef MARKDOWNPREVIEW_H
#define MARKDOWNPREVIEW_H

#include <QWidget>
#include <QTextBrowser>
#include <QString>
#include <QScrollBar>

class MarkdownPreview : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownPreview(QWidget *parent = nullptr);
    ~MarkdownPreview();

    void setHtml(const QString& html);
    void clear();

    // Scroll synchronization
    int scrollPosition() const;
    void setScrollPosition(int position);
    double scrollPercentage() const;
    void setScrollPercentage(double percentage);

signals:
    void scrolled(double percentage);

private:
    QTextBrowser *browser_;
    void setupBrowser();
    QString wrapHtml(const QString& content);

    bool syncScroll_;
};

#endif // MARKDOWNPREVIEW_H
