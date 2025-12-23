#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QMap>
#include <QRegularExpression>
#include <QString>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTimer>
#include <QVector>

class Theme;

// Base class for all syntax highlighters
class BaseSyntaxHighlighter : public QSyntaxHighlighter {
  Q_OBJECT

public:
  explicit BaseSyntaxHighlighter(QTextDocument *parent = nullptr);
  virtual ~BaseSyntaxHighlighter() = default;

  void setTheme(Theme *theme);
  Theme *theme() const { return theme_; }

  // Enable/disable highlighting
  void setEnabled(bool enabled);
  bool isEnabled() const { return enabled_; }

  // Force rehighlight
  void rehighlightDocument();

signals:
  void highlightingComplete();

protected:
  struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
    int captureGroup = 0; // Which capture group to highlight (0 = entire match)
  };

  QVector<HighlightingRule> highlightingRules_;
  Theme *theme_;
  bool enabled_;

  virtual void setupFormats();
  virtual void setupRules() = 0;

  // Common formats
  QTextCharFormat keywordFormat_;
  QTextCharFormat keyword2Format_;
  QTextCharFormat classFormat_;
  QTextCharFormat functionFormat_;
  QTextCharFormat commentFormat_;
  QTextCharFormat stringFormat_;
  QTextCharFormat numberFormat_;
  QTextCharFormat operatorFormat_;
  QTextCharFormat preprocessorFormat_;
  QTextCharFormat typeFormat_;
};

// ==================== MARKDOWN HIGHLIGHTER ====================
class MarkdownHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit MarkdownHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightHeadings(const QString &text);
  void highlightCodeBlocks(const QString &text);
  void highlightInlineCode(const QString &text);
  void highlightEmphasis(const QString &text);
  void highlightLinks(const QString &text);
  void highlightLists(const QString &text);
  void highlightBlockQuotes(const QString &text);
  void highlightHorizontalRules(const QString &text);
  void highlightTables(const QString &text);

  // Multi-line state management
  enum BlockState {
    None = -1,
    InCodeBlock = 1,
    InCodeBlockWithLang = 2,
    InBlockQuote = 3
  };

  // Formats
  QTextCharFormat heading1Format_;
  QTextCharFormat heading2Format_;
  QTextCharFormat heading3Format_;
  QTextCharFormat heading4Format_;
  QTextCharFormat heading5Format_;
  QTextCharFormat heading6Format_;
  QTextCharFormat boldFormat_;
  QTextCharFormat italicFormat_;
  QTextCharFormat boldItalicFormat_;
  QTextCharFormat strikethroughFormat_;
  QTextCharFormat codeFormat_;
  QTextCharFormat codeBlockFormat_;
  QTextCharFormat linkFormat_;
  QTextCharFormat linkUrlFormat_;
  QTextCharFormat imageFormat_;
  QTextCharFormat listMarkerFormat_;
  QTextCharFormat blockQuoteFormat_;
  QTextCharFormat horizontalRuleFormat_;
  QTextCharFormat tableFormat_;

  QString currentCodeBlockLanguage_;
};

// ==================== C++ HIGHLIGHTER ====================
class CppHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit CppHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightMultiLineComments(const QString &text);
  void highlightPreprocessor(const QString &text);
  void highlightStrings(const QString &text);

  QRegularExpression commentStartExpression_;
  QRegularExpression commentEndExpression_;
  QTextCharFormat multiLineCommentFormat_;
  QTextCharFormat docCommentFormat_;
  QTextCharFormat macroFormat_;
};

// ==================== PYTHON HIGHLIGHTER ====================
class PythonHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit PythonHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightTripleQuotes(const QString &text);
  void highlightDecorators(const QString &text);
  void highlightFStrings(const QString &text);

  QRegularExpression tripleDoubleQuote_;
  QRegularExpression tripleSingleQuote_;
  QTextCharFormat docstringFormat_;
  QTextCharFormat decoratorFormat_;
  QTextCharFormat fstringFormat_;
  QTextCharFormat selfFormat_;
};

// ==================== RUST HIGHLIGHTER ====================
class RustHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit RustHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightAttributes(const QString &text);
  void highlightLifetimes(const QString &text);
  void highlightMacros(const QString &text);

  QTextCharFormat attributeFormat_;
  QTextCharFormat lifetimeFormat_;
  QTextCharFormat macroFormat_;
  QTextCharFormat docCommentFormat_;
};

// ==================== SHELL/BASH HIGHLIGHTER ====================
class ShellHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit ShellHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightVariables(const QString &text);
  void highlightHereDoc(const QString &text);
  void highlightCommandSubstitution(const QString &text);

  QTextCharFormat variableFormat_;
  QTextCharFormat builtinFormat_;
  QTextCharFormat optionFormat_;
  QTextCharFormat hereDocFormat_;
  QTextCharFormat commandSubstFormat_;
  QTextCharFormat shebangFormat_;
};

// ==================== JAVASCRIPT/TYPESCRIPT HIGHLIGHTER ====================
class JavaScriptHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit JavaScriptHighlighter(QTextDocument *parent = nullptr,
                                 bool typescript = false);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightTemplateStrings(const QString &text);
  void highlightRegex(const QString &text);
  void highlightJSX(const QString &text);

  bool isTypeScript_;
  QTextCharFormat templateStringFormat_;
  QTextCharFormat regexFormat_;
  QTextCharFormat jsxTagFormat_;
  QTextCharFormat jsxAttrFormat_;
};

// ==================== JSON HIGHLIGHTER ====================
class JsonHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit JsonHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  QTextCharFormat keyFormat_;
  QTextCharFormat boolFormat_;
  QTextCharFormat nullFormat_;
};

// ==================== YAML HIGHLIGHTER ====================
class YamlHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit YamlHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  QTextCharFormat keyFormat_;
  QTextCharFormat anchorFormat_;
  QTextCharFormat aliasFormat_;
  QTextCharFormat tagFormat_;
};

// ==================== HTML HIGHLIGHTER ====================
class HtmlHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit HtmlHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  void highlightTags(const QString &text);
  void highlightAttributes(const QString &text);
  void highlightEntities(const QString &text);

  QTextCharFormat tagFormat_;
  QTextCharFormat attributeFormat_;
  QTextCharFormat attributeValueFormat_;
  QTextCharFormat entityFormat_;
  QTextCharFormat doctypeFormat_;
};

// ==================== CSS HIGHLIGHTER ====================
class CssHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit CssHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  QTextCharFormat selectorFormat_;
  QTextCharFormat propertyFormat_;
  QTextCharFormat valueFormat_;
  QTextCharFormat unitFormat_;
  QTextCharFormat colorFormat_;
  QTextCharFormat importantFormat_;
  QTextCharFormat atRuleFormat_;
};

// ==================== TOML HIGHLIGHTER ====================
class TomlHighlighter : public BaseSyntaxHighlighter {
  Q_OBJECT

public:
  explicit TomlHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
  void setupRules() override;

private:
  QTextCharFormat sectionFormat_;
  QTextCharFormat keyFormat_;
  QTextCharFormat dateFormat_;
};

// ==================== HIGHLIGHTER FACTORY ====================
class HighlighterFactory {
public:
  enum Language {
    None,
    Markdown,
    Cpp,
    Python,
    Rust,
    Shell,
    JavaScript,
    TypeScript,
    Json,
    Yaml,
    Html,
    Css,
    Toml,
    Xml
  };

  static BaseSyntaxHighlighter *createHighlighter(Language lang,
                                                  QTextDocument *doc);
  static BaseSyntaxHighlighter *
  createHighlighterForFile(const QString &filePath, QTextDocument *doc);
  static Language detectLanguage(const QString &filePath);
  static QString languageName(Language lang);
};

#endif // SYNTAXHIGHLIGHTER_H
