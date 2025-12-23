#include "syntaxhighlighter.h"

// ============================================================================
// BaseSyntaxHighlighter
// ============================================================================
#include "syntaxhighlighter.h"
#include "theme.h"
#include <QColor>
#include <QDebug>
#include <QFileInfo>
#include <QTextDocument>
BaseSyntaxHighlighter::BaseSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), theme_(nullptr), enabled_(true) {
  qDebug() << "BaseSyntaxHighlighter constructor called";
  setupFormats();
}

void BaseSyntaxHighlighter::setTheme(Theme *theme) {
  qDebug() << "BaseSyntaxHighlighter::setTheme called, theme is"
           << (theme ? "valid" : "null");
  theme_ = theme;
  setupFormats();
  rehighlight();
}

void BaseSyntaxHighlighter::setEnabled(bool enabled) {
  enabled_ = enabled;
  if (enabled_) {
    rehighlight();
  }
}

void BaseSyntaxHighlighter::rehighlightDocument() { rehighlight(); }

void BaseSyntaxHighlighter::setupFormats() {
  qDebug() << "BaseSyntaxHighlighter::setupFormats called, theme is"
           << (theme_ ? "valid" : "null");

  // Use theme colors if available, otherwise use defaults
  QColor keywordColor = theme_ ? theme_->codeKeyword() : QColor("#569cd6");
  QColor classColor = theme_ ? theme_->codeClass() : QColor("#4ec9b0");
  QColor commentColor = theme_ ? theme_->syntaxComment() : QColor("#6a9955");
  QColor stringColor = theme_ ? theme_->codeString() : QColor("#ce9178");
  QColor functionColor = theme_ ? theme_->codeFunction() : QColor("#dcdcaa");
  QColor numberColor = theme_ ? theme_->codeNumber() : QColor("#b5cea8");
  QColor operatorColor = theme_ ? theme_->codeOperator() : QColor("#d4d4d4");
  QColor preprocessorColor =
      theme_ ? theme_->codePreprocessor() : QColor("#c586c0");

  qDebug() << "Keyword color:" << keywordColor.name();
  qDebug() << "String color:" << stringColor.name();
  qDebug() << "Comment color:" << commentColor.name();

  // Keyword format
  keywordFormat_.setForeground(keywordColor);
  keywordFormat_.setFontWeight(QFont::Bold);

  // Class format
  classFormat_.setForeground(classColor);
  classFormat_.setFontWeight(QFont::Bold);

  // Comment format
  commentFormat_.setForeground(commentColor);
  commentFormat_.setFontItalic(true);

  // String format
  stringFormat_.setForeground(stringColor);

  // Function format
  functionFormat_.setForeground(functionColor);

  // Number format
  numberFormat_.setForeground(numberColor);

  // Operator format
  operatorFormat_.setForeground(operatorColor);

  // Preprocessor format
  preprocessorFormat_.setForeground(preprocessorColor);
}

// ============================================================================
// CppHighlighter
// ============================================================================

CppHighlighter::CppHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  qDebug() << "CppHighlighter constructor called";
  setupRules();

  commentStartExpression_ = QRegularExpression("/\\*");
  commentEndExpression_ = QRegularExpression("\\*/");

  multiLineCommentFormat_.setForeground(commentFormat_.foreground().color());
  multiLineCommentFormat_.setFontItalic(true);
}

void CppHighlighter::setupRules() {
  highlightingRules_.clear();

  // C++ keywords
  QStringList keywordPatterns = {
      "\\bchar\\b",         "\\bclass\\b",      "\\bconst\\b",
      "\\bdouble\\b",       "\\benum\\b",       "\\bexplicit\\b",
      "\\bfriend\\b",       "\\binline\\b",     "\\bint\\b",
      "\\blong\\b",         "\\bnamespace\\b",  "\\boperator\\b",
      "\\bprivate\\b",      "\\bprotected\\b",  "\\bpublic\\b",
      "\\bshort\\b",        "\\bsignals\\b",    "\\bsigned\\b",
      "\\bslots\\b",        "\\bstatic\\b",     "\\bstruct\\b",
      "\\btemplate\\b",     "\\btypedef\\b",    "\\btypename\\b",
      "\\bunion\\b",        "\\bunsigned\\b",   "\\bvirtual\\b",
      "\\bvoid\\b",         "\\bvolatile\\b",   "\\bbool\\b",
      "\\bfor\\b",          "\\bwhile\\b",      "\\bif\\b",
      "\\belse\\b",         "\\breturn\\b",     "\\bswitch\\b",
      "\\bcase\\b",         "\\bbreak\\b",      "\\bcontinue\\b",
      "\\bdefault\\b",      "\\bdo\\b",         "\\bgoto\\b",
      "\\btry\\b",          "\\bcatch\\b",      "\\bthrow\\b",
      "\\bauto\\b",         "\\bconstexpr\\b",  "\\bdecltype\\b",
      "\\bnoexcept\\b",     "\\bnullptr\\b",    "\\boverride\\b",
      "\\bfinal\\b",        "\\busing\\b",      "\\bstatic_cast\\b",
      "\\bdynamic_cast\\b", "\\bconst_cast\\b", "\\breinterpret_cast\\b"};

  for (const QString &pattern : keywordPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Class names
  HighlightingRule classRule;
  classRule.pattern = QRegularExpression("\\b[A-Z][a-zA-Z0-9_]*\\b");
  classRule.format = classFormat_;
  highlightingRules_.append(classRule);

  // Functions
  HighlightingRule functionRule;
  functionRule.pattern = QRegularExpression("\\b[a-zA-Z_][a-zA-Z0-9_]*(?=\\()");
  functionRule.format = functionFormat_;
  highlightingRules_.append(functionRule);

  // Numbers
  HighlightingRule numberRule;
  numberRule.pattern =
      QRegularExpression("\\b[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?[fFlLuU]*\\b");
  numberRule.format = numberFormat_;
  highlightingRules_.append(numberRule);

  // Preprocessor
  HighlightingRule preprocessorRule;
  preprocessorRule.pattern = QRegularExpression("^\\s*#\\s*[a-zA-Z_]+");
  preprocessorRule.format = preprocessorFormat_;
  highlightingRules_.append(preprocessorRule);

  // Quotation
  HighlightingRule quotationRule;
  quotationRule.pattern = QRegularExpression("\".*?\"|'.*?'");
  quotationRule.format = stringFormat_;
  highlightingRules_.append(quotationRule);

  // Single-line comment
  HighlightingRule singleLineCommentRule;
  singleLineCommentRule.pattern = QRegularExpression("//[^\n]*");
  singleLineCommentRule.format = commentFormat_;
  highlightingRules_.append(singleLineCommentRule);
}

void CppHighlighter::highlightBlock(const QString &text) {
  if (text.isEmpty() || !enabled_)
    return;

  // Apply single-line rules
  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }

  // Multi-line comments
  setCurrentBlockState(0);

  int startIndex = 0;
  if (previousBlockState() != 1)
    startIndex = text.indexOf(commentStartExpression_);

  while (startIndex >= 0) {
    QRegularExpressionMatch match =
        commentEndExpression_.match(text, startIndex);
    int endIndex = match.capturedStart();
    int commentLength = 0;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength = endIndex - startIndex + match.capturedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat_);
    startIndex =
        text.indexOf(commentStartExpression_, startIndex + commentLength);
  }
}

void CppHighlighter::highlightMultiLineComments(const QString &text) {
  Q_UNUSED(text);
  // Handled in highlightBlock
}

void CppHighlighter::highlightPreprocessor(const QString &text) {
  Q_UNUSED(text);
  // Handled in highlightBlock via rules
}

void CppHighlighter::highlightStrings(const QString &text) {
  Q_UNUSED(text);
  // Handled in highlightBlock via rules
}

// ============================================================================
// PythonHighlighter
// ============================================================================

PythonHighlighter::PythonHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();

  tripleDoubleQuote_ = QRegularExpression("\"\"\"");
  tripleSingleQuote_ = QRegularExpression("'''");

  docstringFormat_.setForeground(QColor("#ce9178"));
}

void PythonHighlighter::setupRules() {
  highlightingRules_.clear();

  // Python keywords
  QStringList keywordPatterns = {
      "\\bFalse\\b",   "\\bNone\\b",     "\\bTrue\\b",     "\\band\\b",
      "\\bas\\b",      "\\bassert\\b",   "\\basync\\b",    "\\bawait\\b",
      "\\bbreak\\b",   "\\bclass\\b",    "\\bcontinue\\b", "\\bdef\\b",
      "\\bdel\\b",     "\\belif\\b",     "\\belse\\b",     "\\bexcept\\b",
      "\\bfinally\\b", "\\bfor\\b",      "\\bfrom\\b",     "\\bglobal\\b",
      "\\bif\\b",      "\\bimport\\b",   "\\bin\\b",       "\\bis\\b",
      "\\blambda\\b",  "\\bnonlocal\\b", "\\bnot\\b",      "\\bor\\b",
      "\\bpass\\b",    "\\braise\\b",    "\\breturn\\b",   "\\btry\\b",
      "\\bwhile\\b",   "\\bwith\\b",     "\\byield\\b"};

  for (const QString &pattern : keywordPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Built-in functions
  QStringList builtinPatterns = {
      "\\babs\\b",   "\\ball\\b",       "\\bany\\b",       "\\bbin\\b",
      "\\bbool\\b",  "\\bbytearray\\b", "\\bbytes\\b",     "\\bchr\\b",
      "\\bdict\\b",  "\\bdir\\b",       "\\benumerate\\b", "\\bfilter\\b",
      "\\bfloat\\b", "\\bint\\b",       "\\blen\\b",       "\\blist\\b",
      "\\bmap\\b",   "\\bmax\\b",       "\\bmin\\b",       "\\bopen\\b",
      "\\bprint\\b", "\\brange\\b",     "\\bset\\b",       "\\bstr\\b",
      "\\bsum\\b",   "\\btuple\\b",     "\\btype\\b",      "\\bzip\\b"};

  for (const QString &pattern : builtinPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = functionFormat_;
    highlightingRules_.append(rule);
  }

  // Class names
  HighlightingRule classRule;
  classRule.pattern = QRegularExpression("\\b[A-Z][a-zA-Z0-9_]*\\b");
  classRule.format = classFormat_;
  highlightingRules_.append(classRule);

  // Functions
  HighlightingRule functionRule;
  functionRule.pattern = QRegularExpression("\\b[a-zA-Z_][a-zA-Z0-9_]*(?=\\()");
  functionRule.format = functionFormat_;
  highlightingRules_.append(functionRule);

  // Numbers
  HighlightingRule numberRule;
  numberRule.pattern =
      QRegularExpression("\\b[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?\\b");
  numberRule.format = numberFormat_;
  highlightingRules_.append(numberRule);

  // Decorators
  HighlightingRule decoratorRule;
  decoratorRule.pattern = QRegularExpression("@[a-zA-Z_][a-zA-Z0-9_]*");
  decoratorRule.format = preprocessorFormat_;
  highlightingRules_.append(decoratorRule);

  // Single-quoted strings
  HighlightingRule singleQuoteRule;
  singleQuoteRule.pattern = QRegularExpression("'[^'\\\\]*(\\\\.[^'\\\\]*)*'");
  singleQuoteRule.format = stringFormat_;
  highlightingRules_.append(singleQuoteRule);

  // Double-quoted strings
  HighlightingRule doubleQuoteRule;
  doubleQuoteRule.pattern =
      QRegularExpression("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
  doubleQuoteRule.format = stringFormat_;
  highlightingRules_.append(doubleQuoteRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("#[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void PythonHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  // Apply single-line rules
  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }

  // Triple-quoted strings (docstrings)
  highlightTripleQuotes(text);
}

void PythonHighlighter::highlightTripleQuotes(const QString &text) {
  setCurrentBlockState(0);

  int startIndex = 0;
  QRegularExpression tripleQuoteExpression("(\"\"\"|\'\'\')");

  if (previousBlockState() != 1) {
    QRegularExpressionMatch match = tripleQuoteExpression.match(text);
    startIndex = match.capturedStart();
  }

  while (startIndex >= 0) {
    QRegularExpressionMatch match =
        tripleQuoteExpression.match(text, startIndex + 3);
    int endIndex = match.capturedStart();
    int stringLength = 0;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      stringLength = text.length() - startIndex;
    } else {
      stringLength = endIndex - startIndex + match.capturedLength();
    }
    setFormat(startIndex, stringLength, docstringFormat_);
    startIndex = text.indexOf(tripleQuoteExpression, startIndex + stringLength);
  }
}

void PythonHighlighter::highlightDecorators(const QString &text) {
  Q_UNUSED(text);
  // Handled in highlightBlock via rules
}

void PythonHighlighter::highlightFStrings(const QString &text) {
  Q_UNUSED(text);
  // Could be added for f-string support
}

// ============================================================================
// RustHighlighter
// ============================================================================

RustHighlighter::RustHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void RustHighlighter::setupRules() {
  highlightingRules_.clear();

  // Rust keywords
  QStringList keywordPatterns = {
      "\\bas\\b",       "\\bbreak\\b",  "\\bconst\\b",    "\\bcontinue\\b",
      "\\bcrate\\b",    "\\belse\\b",   "\\benum\\b",     "\\bextern\\b",
      "\\bfalse\\b",    "\\bfn\\b",     "\\bfor\\b",      "\\bif\\b",
      "\\bimpl\\b",     "\\bin\\b",     "\\blet\\b",      "\\bloop\\b",
      "\\bmatch\\b",    "\\bmod\\b",    "\\bmove\\b",     "\\bmut\\b",
      "\\bpub\\b",      "\\bref\\b",    "\\breturn\\b",   "\\bself\\b",
      "\\bSelf\\b",     "\\bstatic\\b", "\\bstruct\\b",   "\\bsuper\\b",
      "\\btrait\\b",    "\\btrue\\b",   "\\btype\\b",     "\\bunsafe\\b",
      "\\buse\\b",      "\\bwhere\\b",  "\\bwhile\\b",    "\\basync\\b",
      "\\bawait\\b",    "\\bdyn\\b",    "\\babstract\\b", "\\bbecome\\b",
      "\\bbox\\b",      "\\bdo\\b",     "\\bfinal\\b",    "\\bmacro\\b",
      "\\boverride\\b", "\\bpriv\\b",   "\\btypeof\\b",   "\\bunsized\\b",
      "\\bvirtual\\b",  "\\byield\\b"};

  for (const QString &pattern : keywordPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Primitive types
  QStringList typePatterns = {
      "\\bi8\\b",    "\\bi16\\b",  "\\bi32\\b",  "\\bi64\\b",  "\\bi128\\b",
      "\\bu8\\b",    "\\bu16\\b",  "\\bu32\\b",  "\\bu64\\b",  "\\bu128\\b",
      "\\bf32\\b",   "\\bf64\\b",  "\\bbool\\b", "\\bchar\\b", "\\bstr\\b",
      "\\busize\\b", "\\bisize\\b"};

  for (const QString &pattern : typePatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = classFormat_;
    highlightingRules_.append(rule);
  }

  // Standard library types
  HighlightingRule stdTypeRule;
  stdTypeRule.pattern = QRegularExpression(
      "\\b(String|Vec|Box|Rc|Arc|Option|Result|HashMap|HashSet)\\b");
  stdTypeRule.format = classFormat_;
  highlightingRules_.append(stdTypeRule);

  // Lifetimes
  HighlightingRule lifetimeRule;
  lifetimeRule.pattern = QRegularExpression("'[a-zA-Z_][a-zA-Z0-9_]*\\b");
  lifetimeRule.format = preprocessorFormat_;
  highlightingRules_.append(lifetimeRule);

  // Macros
  HighlightingRule macroRule;
  macroRule.pattern = QRegularExpression("\\b[a-zA-Z_][a-zA-Z0-9_]*!");
  macroRule.format = functionFormat_;
  highlightingRules_.append(macroRule);

  // Functions
  HighlightingRule functionRule;
  functionRule.pattern = QRegularExpression("\\b[a-zA-Z_][a-zA-Z0-9_]*(?=\\()");
  functionRule.format = functionFormat_;
  highlightingRules_.append(functionRule);

  // Numbers
  HighlightingRule numberRule;
  numberRule.pattern = QRegularExpression(
      "\\b[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?(_[a-zA-Z0-9]+)?\\b");
  numberRule.format = numberFormat_;
  highlightingRules_.append(numberRule);

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern =
      QRegularExpression("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"|r(#+)\".*?\"\\2");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Characters
  HighlightingRule charRule;
  charRule.pattern = QRegularExpression("'([^'\\\\]|\\\\.)'");
  charRule.format = stringFormat_;
  highlightingRules_.append(charRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("//[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void RustHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  // Apply all rules
  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

void RustHighlighter::highlightAttributes(const QString &text) {
  Q_UNUSED(text);
}

void RustHighlighter::highlightLifetimes(const QString &text) {
  Q_UNUSED(text);
}

void RustHighlighter::highlightMacros(const QString &text) { Q_UNUSED(text); }

// ============================================================================
// MarkdownHighlighter
// ============================================================================

MarkdownHighlighter::MarkdownHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void MarkdownHighlighter::setupRules() {
  highlightingRules_.clear();

  // Headers - different colors for different levels
  heading1Format_.setForeground(QColor("#569cd6"));
  heading1Format_.setFontWeight(QFont::Bold);

  HighlightingRule h1Rule;
  h1Rule.pattern = QRegularExpression("^#{1}\\s.*$");
  h1Rule.format = heading1Format_;
  QFont h1Font = h1Rule.format.font();
  h1Font.setPointSize(18);
  h1Rule.format.setFont(h1Font);
  highlightingRules_.append(h1Rule);

  HighlightingRule h2Rule;
  h2Rule.pattern = QRegularExpression("^#{2}\\s.*$");
  h2Rule.format = heading1Format_;
  QFont h2Font = h2Rule.format.font();
  h2Font.setPointSize(16);
  h2Rule.format.setFont(h2Font);
  highlightingRules_.append(h2Rule);

  HighlightingRule h3Rule;
  h3Rule.pattern = QRegularExpression("^#{3,6}\\s.*$");
  h3Rule.format = heading1Format_;
  QFont h3Font = h3Rule.format.font();
  h3Font.setPointSize(14);
  h3Rule.format.setFont(h3Font);
  highlightingRules_.append(h3Rule);

  // Bold text
  boldFormat_.setForeground(QColor("#d4d4d4"));
  boldFormat_.setFontWeight(QFont::Bold);

  HighlightingRule boldRule;
  boldRule.pattern = QRegularExpression("\\*\\*[^*]+\\*\\*|__[^_]+__");
  boldRule.format = boldFormat_;
  highlightingRules_.append(boldRule);

  // Italic text
  italicFormat_.setForeground(QColor("#d4d4d4"));
  italicFormat_.setFontItalic(true);

  HighlightingRule italicRule;
  italicRule.pattern = QRegularExpression("\\*[^*]+\\*|_[^_]+_");
  italicRule.format = italicFormat_;
  highlightingRules_.append(italicRule);

  // Code (inline)
  codeFormat_.setForeground(QColor("#ce9178"));
  codeFormat_.setBackground(QColor("#2d2d2d"));

  HighlightingRule inlineCodeRule;
  inlineCodeRule.pattern = QRegularExpression("`[^`]+`");
  inlineCodeRule.format = codeFormat_;
  highlightingRules_.append(inlineCodeRule);

  // Code blocks
  HighlightingRule codeBlockRule;
  codeBlockRule.pattern = QRegularExpression("^```.*$|^~~~.*$");
  codeBlockRule.format = codeFormat_;
  highlightingRules_.append(codeBlockRule);

  // Links
  linkFormat_.setForeground(QColor("#4ec9b0"));
  linkFormat_.setFontUnderline(true);

  HighlightingRule linkRule;
  linkRule.pattern = QRegularExpression("\\[([^\\]]+)\\]\\(([^)]+)\\)");
  linkRule.format = linkFormat_;
  highlightingRules_.append(linkRule);

  // Images
  HighlightingRule imageRule;
  imageRule.pattern = QRegularExpression("!\\[([^\\]]*)\\]\\(([^)]+)\\)");
  imageRule.format = linkFormat_;
  highlightingRules_.append(imageRule);

  // Lists
  HighlightingRule listRule;
  listRule.pattern = QRegularExpression("^\\s*[-*+]\\s|^\\s*\\d+\\.\\s");
  listRule.format = keywordFormat_;
  highlightingRules_.append(listRule);

  // Blockquotes
  HighlightingRule quoteRule;
  quoteRule.pattern = QRegularExpression("^>+.*$");
  quoteRule.format = commentFormat_;
  highlightingRules_.append(quoteRule);

  // Horizontal rules
  HighlightingRule hrRule;
  hrRule.pattern = QRegularExpression("^(\\*\\*\\*|---|___)\\s*$");
  hrRule.format = operatorFormat_;
  highlightingRules_.append(hrRule);
}

void MarkdownHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  // Apply all rules
  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

void MarkdownHighlighter::highlightHeadings(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightCodeBlocks(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightInlineCode(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightEmphasis(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightLinks(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightLists(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightBlockQuotes(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightHorizontalRules(const QString &text) {
  Q_UNUSED(text);
}

void MarkdownHighlighter::highlightTables(const QString &text) {
  Q_UNUSED(text);
}

// ============================================================================
// ShellHighlighter
// ============================================================================

ShellHighlighter::ShellHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void ShellHighlighter::setupRules() {
  highlightingRules_.clear();

  // Shell keywords
  QStringList keywordPatterns = {
      "\\bif\\b",       "\\bthen\\b",   "\\belse\\b",  "\\belif\\b",
      "\\bfi\\b",       "\\bfor\\b",    "\\bwhile\\b", "\\bdo\\b",
      "\\bdone\\b",     "\\bcase\\b",   "\\besac\\b",  "\\bin\\b",
      "\\bfunction\\b", "\\breturn\\b", "\\blocal\\b", "\\bexport\\b",
      "\\bsource\\b",   "\\bexit\\b",   "\\bbreak\\b", "\\bcontinue\\b"};

  for (const QString &pattern : keywordPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Variables
  variableFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule varRule;
  varRule.pattern =
      QRegularExpression("\\$[a-zA-Z_][a-zA-Z0-9_]*|\\$\\{[^}]+\\}");
  varRule.format = variableFormat_;
  highlightingRules_.append(varRule);

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern = QRegularExpression("\"[^\"]*\"|'[^']*'");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("#[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);

  // Shebang
  shebangFormat_.setForeground(QColor("#c586c0"));
  HighlightingRule shebangRule;
  shebangRule.pattern = QRegularExpression("^#!.*$");
  shebangRule.format = shebangFormat_;
  highlightingRules_.append(shebangRule);
}

void ShellHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

void ShellHighlighter::highlightVariables(const QString &text) {
  Q_UNUSED(text);
}

void ShellHighlighter::highlightHereDoc(const QString &text) { Q_UNUSED(text); }

void ShellHighlighter::highlightCommandSubstitution(const QString &text) {
  Q_UNUSED(text);
}

// ============================================================================
// JavaScriptHighlighter
// ============================================================================

JavaScriptHighlighter::JavaScriptHighlighter(QTextDocument *parent,
                                             bool typescript)
    : BaseSyntaxHighlighter(parent), isTypeScript_(typescript) {
  setupRules();
}

void JavaScriptHighlighter::setupRules() {
  highlightingRules_.clear();

  QStringList keywordPatterns = {
      "\\bbreak\\b",    "\\bcase\\b",     "\\bcatch\\b",      "\\bconst\\b",
      "\\bcontinue\\b", "\\bdebugger\\b", "\\bdefault\\b",    "\\bdelete\\b",
      "\\bdo\\b",       "\\belse\\b",     "\\bexport\\b",     "\\bextends\\b",
      "\\bfinally\\b",  "\\bfor\\b",      "\\bfunction\\b",   "\\bif\\b",
      "\\bimport\\b",   "\\bin\\b",       "\\binstanceof\\b", "\\blet\\b",
      "\\bnew\\b",      "\\breturn\\b",   "\\bsuper\\b",      "\\bswitch\\b",
      "\\bthis\\b",     "\\bthrow\\b",    "\\btry\\b",        "\\btypeof\\b",
      "\\bvar\\b",      "\\bvoid\\b",     "\\bwhile\\b",      "\\bwith\\b",
      "\\byield\\b",    "\\bclass\\b",    "\\basync\\b",      "\\bawait\\b"};

  if (isTypeScript_) {
    keywordPatterns << "\\btype\\b" << "\\binterface\\b" << "\\benum\\b"
                    << "\\bnamespace\\b" << "\\bas\\b" << "\\bimplements\\b";
  }

  for (const QString &pattern : keywordPatterns) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern = QRegularExpression("\"[^\"]*\"|'[^']*'");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("//[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void JavaScriptHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

void JavaScriptHighlighter::highlightTemplateStrings(const QString &text) {
  Q_UNUSED(text);
}

void JavaScriptHighlighter::highlightRegex(const QString &text) {
  Q_UNUSED(text);
}

void JavaScriptHighlighter::highlightJSX(const QString &text) {
  Q_UNUSED(text);
}

// ============================================================================
// JsonHighlighter
// ============================================================================

JsonHighlighter::JsonHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void JsonHighlighter::setupRules() {
  highlightingRules_.clear();

  // Keys
  keyFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule keyRule;
  keyRule.pattern = QRegularExpression("\"[^\"]+\"\\s*:");
  keyRule.format = keyFormat_;
  highlightingRules_.append(keyRule);

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern = QRegularExpression(":\\s*\"[^\"]*\"");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Booleans
  boolFormat_.setForeground(QColor("#569cd6"));
  HighlightingRule boolRule;
  boolRule.pattern = QRegularExpression("\\b(true|false)\\b");
  boolRule.format = boolFormat_;
  highlightingRules_.append(boolRule);

  // Null
  nullFormat_.setForeground(QColor("#569cd6"));
  HighlightingRule nullRule;
  nullRule.pattern = QRegularExpression("\\bnull\\b");
  nullRule.format = nullFormat_;
  highlightingRules_.append(nullRule);

  // Numbers
  HighlightingRule numberRule;
  numberRule.pattern =
      QRegularExpression("\\b-?[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?\\b");
  numberRule.format = numberFormat_;
  highlightingRules_.append(numberRule);
}

void JsonHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

// ============================================================================
// YamlHighlighter
// ============================================================================

YamlHighlighter::YamlHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void YamlHighlighter::setupRules() {
  highlightingRules_.clear();

  // Keys
  keyFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule keyRule;
  keyRule.pattern = QRegularExpression("^[^:]+:");
  keyRule.format = keyFormat_;
  highlightingRules_.append(keyRule);

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern = QRegularExpression("\"[^\"]*\"|'[^']*'");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("#[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void YamlHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

// ============================================================================
// HtmlHighlighter
// ============================================================================

HtmlHighlighter::HtmlHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void HtmlHighlighter::setupRules() {
  highlightingRules_.clear();

  // Tags
  tagFormat_.setForeground(QColor("#569cd6"));
  HighlightingRule tagRule;
  tagRule.pattern = QRegularExpression("</?[a-zA-Z][a-zA-Z0-9]*");
  tagRule.format = tagFormat_;
  highlightingRules_.append(tagRule);

  // Attributes
  attributeFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule attrRule;
  attrRule.pattern = QRegularExpression("\\b[a-zA-Z-]+(?==)");
  attrRule.format = attributeFormat_;
  highlightingRules_.append(attrRule);

  // Attribute values
  HighlightingRule attrValueRule;
  attrValueRule.pattern = QRegularExpression("\"[^\"]*\"");
  attrValueRule.format = stringFormat_;
  highlightingRules_.append(attrValueRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("<!--.*-->");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void HtmlHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

void HtmlHighlighter::highlightTags(const QString &text) { Q_UNUSED(text); }

void HtmlHighlighter::highlightAttributes(const QString &text) {
  Q_UNUSED(text);
}

void HtmlHighlighter::highlightEntities(const QString &text) { Q_UNUSED(text); }

// ============================================================================
// CssHighlighter
// ============================================================================

CssHighlighter::CssHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void CssHighlighter::setupRules() {
  highlightingRules_.clear();

  // Selectors
  selectorFormat_.setForeground(QColor("#d7ba7d"));
  HighlightingRule selectorRule;
  selectorRule.pattern =
      QRegularExpression("[.#]?[a-zA-Z][a-zA-Z0-9_-]*(?=\\s*\\{)");
  selectorRule.format = selectorFormat_;
  highlightingRules_.append(selectorRule);

  // Properties
  propertyFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule propRule;
  propRule.pattern = QRegularExpression("[a-zA-Z-]+(?=\\s*:)");
  propRule.format = propertyFormat_;
  highlightingRules_.append(propRule);

  // Values
  HighlightingRule valueRule;
  valueRule.pattern = QRegularExpression(":\\s*[^;]+");
  valueRule.format = stringFormat_;
  highlightingRules_.append(valueRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("/\\*.*\\*/");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void CssHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

// ============================================================================
// TomlHighlighter
// ============================================================================

TomlHighlighter::TomlHighlighter(QTextDocument *parent)
    : BaseSyntaxHighlighter(parent) {
  setupRules();
}

void TomlHighlighter::setupRules() {
  highlightingRules_.clear();

  // Sections
  sectionFormat_.setForeground(QColor("#569cd6"));
  HighlightingRule sectionRule;
  sectionRule.pattern = QRegularExpression("^\\[[^\\]]+\\]");
  sectionRule.format = sectionFormat_;
  highlightingRules_.append(sectionRule);

  // Keys
  keyFormat_.setForeground(QColor("#9cdcfe"));
  HighlightingRule keyRule;
  keyRule.pattern = QRegularExpression("^[a-zA-Z_][a-zA-Z0-9_-]*(?=\\s*=)");
  keyRule.format = keyFormat_;
  highlightingRules_.append(keyRule);

  // Strings
  HighlightingRule stringRule;
  stringRule.pattern = QRegularExpression("\"[^\"]*\"");
  stringRule.format = stringFormat_;
  highlightingRules_.append(stringRule);

  // Comments
  HighlightingRule commentRule;
  commentRule.pattern = QRegularExpression("#[^\n]*");
  commentRule.format = commentFormat_;
  highlightingRules_.append(commentRule);
}

void TomlHighlighter::highlightBlock(const QString &text) {
  if (!enabled_)
    return;

  for (const HighlightingRule &rule : highlightingRules_) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}

// ============================================================================
// HighlighterFactory
// ============================================================================

BaseSyntaxHighlighter *
HighlighterFactory::createHighlighter(Language lang, QTextDocument *doc) {
  switch (lang) {
  case Markdown:
    return new MarkdownHighlighter(doc);
  case Cpp:
    return new CppHighlighter(doc);
  case Python:
    return new PythonHighlighter(doc);
  case Rust:
    return new RustHighlighter(doc);
  case Shell:
    return new ShellHighlighter(doc);
  case JavaScript:
    return new JavaScriptHighlighter(doc, false);
  case TypeScript:
    return new JavaScriptHighlighter(doc, true);
  case Json:
    return new JsonHighlighter(doc);
  case Yaml:
    return new YamlHighlighter(doc);
  case Html:
  case Xml:
    return new HtmlHighlighter(doc);
  case Css:
    return new CssHighlighter(doc);
  case Toml:
    return new TomlHighlighter(doc);
  default:
    return nullptr;
  }
}

BaseSyntaxHighlighter *
HighlighterFactory::createHighlighterForFile(const QString &filePath,
                                             QTextDocument *doc) {
  Language lang = detectLanguage(filePath);
  return createHighlighter(lang, doc);
}

HighlighterFactory::Language
HighlighterFactory::detectLanguage(const QString &filePath) {
  QFileInfo fileInfo(filePath);
  QString suffix = fileInfo.suffix().toLower();
  QString baseName = fileInfo.baseName().toLower();

  if (suffix == "md" || suffix == "markdown")
    return Markdown;
  if (suffix == "cpp" || suffix == "cc" || suffix == "cxx" || suffix == "c" ||
      suffix == "h" || suffix == "hpp" || suffix == "hxx")
    return Cpp;
  if (suffix == "py" || suffix == "pyw")
    return Python;
  if (suffix == "rs")
    return Rust;
  if (suffix == "sh" || suffix == "bash" || suffix == "zsh" ||
      baseName == "makefile" || baseName == ".bashrc" || baseName == ".zshrc")
    return Shell;
  if (suffix == "js" || suffix == "jsx" || suffix == "mjs")
    return JavaScript;
  if (suffix == "ts" || suffix == "tsx")
    return TypeScript;
  if (suffix == "json")
    return Json;
  if (suffix == "yaml" || suffix == "yml")
    return Yaml;
  if (suffix == "html" || suffix == "htm")
    return Html;
  if (suffix == "xml")
    return Xml;
  if (suffix == "css" || suffix == "scss" || suffix == "sass")
    return Css;
  if (suffix == "toml")
    return Toml;

  return None;
}

QString HighlighterFactory::languageName(Language lang) {
  switch (lang) {
  case Markdown:
    return "Markdown";
  case Cpp:
    return "C++";
  case Python:
    return "Python";
  case Rust:
    return "Rust";
  case Shell:
    return "Shell";
  case JavaScript:
    return "JavaScript";
  case TypeScript:
    return "TypeScript";
  case Json:
    return "JSON";
  case Yaml:
    return "YAML";
  case Html:
    return "HTML";
  case Xml:
    return "XML";
  case Css:
    return "CSS";
  case Toml:
    return "TOML";
  default:
    return "Plain Text";
  }
}
