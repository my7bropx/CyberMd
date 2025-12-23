#include "markdownpreview.h"
#include <QVBoxLayout>
#include <QScrollBar>

MarkdownPreview::MarkdownPreview(QWidget *parent)
    : QWidget(parent),
      browser_(nullptr),
      syncScroll_(true)
{
    setupBrowser();
}

MarkdownPreview::~MarkdownPreview() {
}

void MarkdownPreview::setupBrowser() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    browser_ = new QTextBrowser(this);
    browser_->setOpenExternalLinks(true);
    browser_->setReadOnly(true);

    layout->addWidget(browser_);

    // Connect scroll event
    connect(browser_->verticalScrollBar(), &QScrollBar::valueChanged,
            this, [this]() {
        if (syncScroll_) {
            emit scrolled(scrollPercentage());
        }
    });
}

void MarkdownPreview::setHtml(const QString& html) {
    // Save scroll position
    double scrollPos = scrollPercentage();

    browser_->setHtml(wrapHtml(html));

    // Restore scroll position
    setScrollPercentage(scrollPos);
}

void MarkdownPreview::clear() {
    browser_->clear();
}

int MarkdownPreview::scrollPosition() const {
    return browser_->verticalScrollBar()->value();
}

void MarkdownPreview::setScrollPosition(int position) {
    browser_->verticalScrollBar()->setValue(position);
}

double MarkdownPreview::scrollPercentage() const {
    QScrollBar *scrollBar = browser_->verticalScrollBar();
    int max = scrollBar->maximum();
    if (max == 0) return 0.0;
    return static_cast<double>(scrollBar->value()) / max;
}

void MarkdownPreview::setScrollPercentage(double percentage) {
    syncScroll_ = false;
    QScrollBar *scrollBar = browser_->verticalScrollBar();
    int position = static_cast<int>(percentage * scrollBar->maximum());
    scrollBar->setValue(position);
    syncScroll_ = true;
}

QString MarkdownPreview::wrapHtml(const QString& content) {
    return QString(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">

    <!-- Highlight.js for code syntax highlighting -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/vs2015.min.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>

    <!-- KaTeX for math equations -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.css">
    <script src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/contrib/auto-render.min.js"></script>

    <!-- Mermaid for diagrams -->
    <script src="https://cdn.jsdelivr.net/npm/mermaid@10.6.1/dist/mermaid.min.js"></script>

    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif;
            font-size: 14px;
            line-height: 1.6;
            color: #d4d4d4;
            background-color: #1e1e1e;
            padding: 20px;
            max-width: 900px;
            margin: 0 auto;
        }

        h1, h2, h3, h4, h5, h6 {
            margin-top: 24px;
            margin-bottom: 16px;
            font-weight: 600;
            line-height: 1.25;
        }

        h1 { font-size: 2em; color: #569CD6; border-bottom: 1px solid #404040; padding-bottom: 0.3em; }
        h2 { font-size: 1.5em; color: #4EC9B0; border-bottom: 1px solid #404040; padding-bottom: 0.3em; }
        h3 { font-size: 1.25em; color: #DCDCAA; }
        h4 { font-size: 1em; color: #9CDCFE; }
        h5 { font-size: 0.875em; color: #C586C0; }
        h6 { font-size: 0.85em; color: #CE9178; }

        p { margin-top: 0; margin-bottom: 16px; }

        a { color: #4EC9B0; text-decoration: none; }
        a:hover { text-decoration: underline; }

        code {
            background-color: #2d2d2d;
            color: #CE9178;
            padding: 0.2em 0.4em;
            border-radius: 3px;
            font-family: 'Consolas', 'Monaco', 'Courier New', monospace;
            font-size: 0.9em;
        }

        pre {
            background-color: #1e1e1e;
            padding: 16px;
            border-radius: 6px;
            overflow: auto;
            line-height: 1.45;
            border: 1px solid #404040;
        }

        pre code {
            background-color: transparent;
            padding: 0;
            color: #d4d4d4;
        }

        /* Highlight.js overrides */
        pre code.hljs {
            background-color: transparent;
        }

        blockquote {
            margin: 0;
            padding: 0 1em;
            color: #858585;
            border-left: 0.25em solid #404040;
        }

        ul, ol {
            margin-top: 0;
            margin-bottom: 16px;
            padding-left: 2em;
        }

        li { margin-bottom: 0.25em; }

        table {
            border-collapse: collapse;
            width: 100%;
            margin-bottom: 16px;
        }

        table th, table td {
            padding: 6px 13px;
            border: 1px solid #404040;
        }

        table th {
            font-weight: 600;
            background-color: #2d2d2d;
        }

        table tr:nth-child(2n) {
            background-color: #252525;
        }

        hr {
            height: 0.25em;
            padding: 0;
            margin: 24px 0;
            background-color: #404040;
            border: 0;
        }

        img {
            max-width: 100%;
            height: auto;
        }

        input[type="checkbox"] {
            margin-right: 0.5em;
        }

        .task-list-item {
            list-style-type: none;
        }

        .task-list-item input {
            margin: 0 0.2em 0.25em -1.6em;
            vertical-align: middle;
        }

        /* Math equation styling */
        .katex {
            font-size: 1.1em;
        }

        /* Mermaid diagram styling */
        .mermaid {
            background-color: #2d2d2d;
            padding: 16px;
            border-radius: 6px;
            margin: 16px 0;
        }
    </style>

    <script>
        // Initialize Mermaid
        mermaid.initialize({
            startOnLoad: true,
            theme: 'dark',
            themeVariables: {
                darkMode: true,
                background: '#2d2d2d',
                primaryColor: '#569CD6',
                primaryTextColor: '#d4d4d4',
                primaryBorderColor: '#404040',
                lineColor: '#4EC9B0',
                secondaryColor: '#4EC9B0',
                tertiaryColor: '#C586C0'
            }
        });

        // Initialize on load
        document.addEventListener('DOMContentLoaded', function() {
            // Highlight code blocks
            document.querySelectorAll('pre code').forEach((block) => {
                hljs.highlightElement(block);
            });

            // Render math equations
            renderMathInElement(document.body, {
                delimiters: [
                    {left: '$$', right: '$$', display: true},
                    {left: '$', right: '$', display: false},
                    {left: '\\[', right: '\\]', display: true},
                    {left: '\\(', right: '\\)', display: false}
                ],
                throwOnError: false
            });
        });
    </script>
</head>
<body>
%1
</body>
</html>
    )").arg(content);
}
