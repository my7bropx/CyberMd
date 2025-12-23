#include "fuzzyfinder.h"
#include "theme.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QTextStream>
#include <QApplication>
#include <QScreen>
#include <algorithm>

// ==================== FuzzyListItem ====================

FuzzyListItem::FuzzyListItem(const FuzzyMatch &match, QListWidget *parent)
    : QListWidgetItem(parent)
    , filePath_(match.filePath)
    , score_(match.score)
{
    setText(match.text);
    setData(Qt::UserRole, match.filePath);
    setData(Qt::UserRole + 1, QVariant::fromValue(match.matchPositions));
}

// ==================== FuzzyLineEdit ====================

FuzzyLineEdit::FuzzyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setPlaceholderText("Type to search...");
}

void FuzzyLineEdit::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:
            emit moveUp();
            return;
        case Qt::Key_Down:
            emit moveDown();
            return;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            emit selectCurrent();
            return;
        default:
            break;
    }
    
    QLineEdit::keyPressEvent(event);
}

// ==================== FuzzyFinder ====================

FuzzyFinder::FuzzyFinder(QWidget *parent)
    : QDialog(parent, Qt::Popup | Qt::FramelessWindowHint)
    , searchInput_(nullptr)
    , resultsList_(nullptr)
    , statusLabel_(nullptr)
    , modeLabel_(nullptr)
    , currentMode_(FileMode)
    , theme_(nullptr)
    , searchTimer_(new QTimer(this))
{
    setupUI();
    
    // Setup search debounce timer
    searchTimer_->setSingleShot(true);
    searchTimer_->setInterval(50);  // 50ms debounce
    connect(searchTimer_, &QTimer::timeout, this, &FuzzyFinder::performSearch);
    
    // Initialize commands for command mode
    commands_ = {
        {"New File", "newFile"},
        {"Open File", "openFile"},
        {"Save File", "saveFile"},
        {"Save As", "saveFileAs"},
        {"Close Tab", "closeTab"},
        {"Find", "showFindDialog"},
        {"Replace", "showReplaceDialog"},
        {"Go to Line", "showGoToLineDialog"},
        {"Toggle Preview", "toggleViewMode"},
        {"Toggle File Tree", "toggleFileTree"},
        {"Zoom In", "zoomIn"},
        {"Zoom Out", "zoomOut"},
        {"Reset Zoom", "resetZoom"},
        {"Fold All", "foldAll"},
        {"Unfold All", "unfoldAll"},
        {"Toggle VIM Mode", "toggleVimMode"},
        {"Regex Helper", "showRegexHelper"},
        {"Command Helper", "showCommandHelper"},
        {"Shell Checker", "showShellChecker"},
        {"Preferences", "showPreferences"},
        {"About", "about"},
        {"Theme: Dark", "themeDefault"},
        {"Theme: Light", "themeLight"},
        {"Theme: Dracula", "themeDracula"},
        {"Theme: Monokai", "themeMonokai"},
        {"Theme: Nord", "themeNord"},
        {"Theme: One Dark", "themeOneDark"},
        {"Theme: CyberPunk", "themeCyberPunk"},
        {"Theme: Matrix", "themeMatrix"}
    };
}

FuzzyFinder::~FuzzyFinder()
{
}

void FuzzyFinder::setupUI()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumSize(600, 400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Container widget with styling
    QWidget *container = new QWidget(this);
    container->setObjectName("fuzzyContainer");
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(12, 12, 12, 12);
    containerLayout->setSpacing(8);
    
    // Mode label at top
    modeLabel_ = new QLabel("Files", container);
    modeLabel_->setObjectName("fuzzyModeLabel");
    containerLayout->addWidget(modeLabel_);
    
    // Search input
    searchInput_ = new FuzzyLineEdit(container);
    searchInput_->setObjectName("fuzzySearchInput");
    searchInput_->setMinimumHeight(36);
    containerLayout->addWidget(searchInput_);
    
    // Results list
    resultsList_ = new QListWidget(container);
    resultsList_->setObjectName("fuzzyResultsList");
    resultsList_->setUniformItemSizes(true);
    resultsList_->setSelectionMode(QAbstractItemView::SingleSelection);
    containerLayout->addWidget(resultsList_, 1);
    
    // Status label at bottom
    statusLabel_ = new QLabel("0 results", container);
    statusLabel_->setObjectName("fuzzyStatusLabel");
    containerLayout->addWidget(statusLabel_);
    
    mainLayout->addWidget(container);
    
    // Connections
    connect(searchInput_, &FuzzyLineEdit::textChanged, this, &FuzzyFinder::onTextChanged);
    connect(searchInput_, &FuzzyLineEdit::moveUp, this, &FuzzyFinder::onMoveUp);
    connect(searchInput_, &FuzzyLineEdit::moveDown, this, &FuzzyFinder::onMoveDown);
    connect(searchInput_, &FuzzyLineEdit::selectCurrent, this, &FuzzyFinder::onSelectCurrent);
    connect(resultsList_, &QListWidget::itemActivated, this, &FuzzyFinder::onItemActivated);
    connect(resultsList_, &QListWidget::itemDoubleClicked, this, &FuzzyFinder::onItemActivated);
}

void FuzzyFinder::setMode(Mode mode)
{
    currentMode_ = mode;
    
    QString modeText;
    QString placeholder;
    
    switch (mode) {
        case FileMode:
            modeText = "📁 Files";
            placeholder = "Search files...";
            break;
        case ContentMode:
            modeText = "🔍 Search in Files";
            placeholder = "Search content...";
            break;
        case CommandMode:
            modeText = "⌘ Commands";
            placeholder = "Search commands...";
            break;
        case BufferMode:
            modeText = "📋 Open Buffers";
            placeholder = "Search open files...";
            break;
    }
    
    modeLabel_->setText(modeText);
    searchInput_->setPlaceholderText(placeholder);
    searchInput_->clear();
    clearResults();
}

void FuzzyFinder::setRootPath(const QString &path)
{
    rootPath_ = path;
    indexedFiles_.clear();
}

void FuzzyFinder::setOpenFiles(const QStringList &files)
{
    openFiles_ = files;
}

void FuzzyFinder::setTheme(Theme *theme)
{
    theme_ = theme;
    applyTheme();
}

void FuzzyFinder::showFileSearch(const QString &rootPath)
{
    setRootPath(rootPath);
    setMode(FileMode);
    indexFiles();
    show();
}

void FuzzyFinder::showContentSearch(const QString &rootPath)
{
    setRootPath(rootPath);
    setMode(ContentMode);
    indexFiles();
    show();
}

void FuzzyFinder::showBufferSearch(const QStringList &openFiles)
{
    setOpenFiles(openFiles);
    setMode(BufferMode);
    show();
}

void FuzzyFinder::showCommandSearch()
{
    setMode(CommandMode);
    show();
}

void FuzzyFinder::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    
    // Center on parent or screen
    QWidget *parent = parentWidget();
    if (parent) {
        QPoint parentCenter = parent->mapToGlobal(parent->rect().center());
        move(parentCenter.x() - width() / 2, parentCenter.y() - height() / 2 - 100);
    } else {
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeom = screen->availableGeometry();
            move(screenGeom.center().x() - width() / 2, 
                 screenGeom.center().y() - height() / 2 - 100);
        }
    }
    
    searchInput_->setFocus();
    searchInput_->selectAll();
    
    // Show initial results
    performSearch();
}

void FuzzyFinder::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
        return;
    }
    
    QDialog::keyPressEvent(event);
}

void FuzzyFinder::onTextChanged(const QString &text)
{
    lastPattern_ = text;
    searchTimer_->start();
}

void FuzzyFinder::onItemActivated(QListWidgetItem *item)
{
    if (!item) return;
    
    QString filePath = item->data(Qt::UserRole).toString();
    int lineNumber = item->data(Qt::UserRole + 2).toInt();
    
    switch (currentMode_) {
        case FileMode:
            emit fileSelected(filePath);
            break;
        case ContentMode:
            emit contentSelected(filePath, lineNumber);
            break;
        case CommandMode:
            emit commandSelected(filePath);  // filePath contains command name
            break;
        case BufferMode:
            emit bufferSelected(filePath);
            break;
    }
    
    hide();
}

void FuzzyFinder::onMoveUp()
{
    int current = resultsList_->currentRow();
    if (current > 0) {
        resultsList_->setCurrentRow(current - 1);
    }
}

void FuzzyFinder::onMoveDown()
{
    int current = resultsList_->currentRow();
    if (current < resultsList_->count() - 1) {
        resultsList_->setCurrentRow(current + 1);
    }
}

void FuzzyFinder::onSelectCurrent()
{
    QListWidgetItem *item = resultsList_->currentItem();
    if (item) {
        onItemActivated(item);
    }
}

void FuzzyFinder::performSearch()
{
    QString pattern = lastPattern_.trimmed();
    
    switch (currentMode_) {
        case FileMode:
            searchFiles(pattern);
            break;
        case ContentMode:
            searchContent(pattern);
            break;
        case CommandMode:
            searchCommands(pattern);
            break;
        case BufferMode:
            searchBuffers(pattern);
            break;
    }
}

void FuzzyFinder::applyTheme()
{
    if (!theme_) return;
    
    setStyleSheet(theme_->getFuzzyFinderStyleSheet());
}

void FuzzyFinder::clearResults()
{
    resultsList_->clear();
    statusLabel_->setText("0 results");
}

// ==================== Fuzzy Matching Algorithm ====================

int FuzzyFinder::fuzzyScore(const QString &pattern, const QString &text, QVector<int> &positions)
{
    positions.clear();
    
    if (pattern.isEmpty()) {
        return 1;  // Empty pattern matches everything with low score
    }
    
    int patternLen = pattern.length();
    int textLen = text.length();
    
    int patternIdx = 0;
    int score = 0;
    int consecutive = 0;
    int lastMatchIdx = -1;
    
    QString lowerPattern = pattern.toLower();
    QString lowerText = text.toLower();
    
    for (int i = 0; i < textLen && patternIdx < patternLen; ++i) {
        if (lowerText[i] == lowerPattern[patternIdx]) {
            positions.append(i);
            
            // Bonus for consecutive matches
            if (lastMatchIdx >= 0 && i == lastMatchIdx + 1) {
                consecutive++;
                score += consecutive * 10;
            } else {
                consecutive = 0;
            }
            
            // Bonus for match at start
            if (i == 0) {
                score += 50;
            }
            
            // Bonus for match after separator
            if (i > 0 && (text[i-1] == '/' || text[i-1] == '_' || text[i-1] == '-' || text[i-1] == '.')) {
                score += 30;
            }
            
            // Bonus for case match
            if (text[i] == pattern[patternIdx]) {
                score += 5;
            }
            
            // Bonus for match at word start (camelCase)
            if (i > 0 && text[i].isUpper() && text[i-1].isLower()) {
                score += 20;
            }
            
            score += 10;  // Base score for each match
            lastMatchIdx = i;
            patternIdx++;
        }
    }
    
    // Did we match all pattern characters?
    if (patternIdx < patternLen) {
        return 0;  // No match
    }
    
    // Penalty for longer strings
    score -= (textLen - patternLen);
    
    // Bonus for shorter strings (exact or near matches)
    if (textLen == patternLen) {
        score += 100;
    }
    
    return qMax(1, score);
}

bool FuzzyFinder::fuzzyMatch(const QString &pattern, const QString &text)
{
    QVector<int> positions;
    return fuzzyScore(pattern, text, positions) > 0;
}

// ==================== Search Functions ====================

void FuzzyFinder::searchFiles(const QString &pattern)
{
    QVector<FuzzyMatch> matches;
    
    for (const QString &filePath : indexedFiles_) {
        QString fileName = QFileInfo(filePath).fileName();
        QString relativePath = filePath;
        if (filePath.startsWith(rootPath_)) {
            relativePath = filePath.mid(rootPath_.length() + 1);
        }
        
        QVector<int> positions;
        int score = fuzzyScore(pattern, fileName, positions);
        
        if (score > 0) {
            FuzzyMatch match;
            match.text = relativePath;
            match.filePath = filePath;
            match.score = score;
            match.matchPositions = positions;
            matches.append(match);
        }
    }
    
    // Sort by score
    std::sort(matches.begin(), matches.end());
    
    // Limit results
    if (matches.size() > 100) {
        matches.resize(100);
    }
    
    displayResults(matches);
}

void FuzzyFinder::searchContent(const QString &pattern)
{
    if (pattern.isEmpty()) {
        clearResults();
        return;
    }
    
    QVector<FuzzyMatch> matches;
    
    for (const QString &filePath : indexedFiles_) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;
        
        QTextStream stream(&file);
        int lineNumber = 0;
        
        while (!stream.atEnd() && matches.size() < 200) {
            QString line = stream.readLine();
            lineNumber++;
            
            if (line.contains(pattern, Qt::CaseInsensitive)) {
                FuzzyMatch match;
                QString relativePath = filePath;
                if (filePath.startsWith(rootPath_)) {
                    relativePath = filePath.mid(rootPath_.length() + 1);
                }
                match.text = QString("%1:%2: %3").arg(relativePath).arg(lineNumber).arg(line.trimmed().left(80));
                match.filePath = filePath;
                match.score = 100;  // All content matches have same base score
                
                // Store line number in a special way
                QListWidgetItem *item = new QListWidgetItem();
                item->setText(match.text);
                item->setData(Qt::UserRole, filePath);
                item->setData(Qt::UserRole + 2, lineNumber);
                resultsList_->addItem(item);
            }
        }
        
        file.close();
    }
    
    statusLabel_->setText(QString("%1 results").arg(resultsList_->count()));
    
    if (resultsList_->count() > 0) {
        resultsList_->setCurrentRow(0);
    }
}

void FuzzyFinder::searchBuffers(const QString &pattern)
{
    QVector<FuzzyMatch> matches;
    
    for (const QString &filePath : openFiles_) {
        QString fileName = QFileInfo(filePath).fileName();
        
        QVector<int> positions;
        int score = fuzzyScore(pattern, fileName, positions);
        
        if (score > 0) {
            FuzzyMatch match;
            match.text = filePath;
            match.filePath = filePath;
            match.score = score;
            match.matchPositions = positions;
            matches.append(match);
        }
    }
    
    std::sort(matches.begin(), matches.end());
    displayResults(matches);
}

void FuzzyFinder::searchCommands(const QString &pattern)
{
    QVector<FuzzyMatch> matches;
    
    for (auto it = commands_.begin(); it != commands_.end(); ++it) {
        QVector<int> positions;
        int score = fuzzyScore(pattern, it.key(), positions);
        
        if (score > 0) {
            FuzzyMatch match;
            match.text = it.key();
            match.filePath = it.value();  // Store command name as "filePath"
            match.score = score;
            match.matchPositions = positions;
            matches.append(match);
        }
    }
    
    std::sort(matches.begin(), matches.end());
    displayResults(matches);
}

void FuzzyFinder::scanDirectory(const QString &path, QStringList &files, int maxDepth)
{
    if (maxDepth <= 0) return;
    
    QDir dir(path);
    
    // Skip certain directories
    static const QStringList skipDirs = {
        "node_modules", ".git", ".svn", ".hg", "__pycache__",
        "target", "build", "dist", ".cache", "vendor"
    };
    
    QString dirName = dir.dirName();
    if (skipDirs.contains(dirName) || dirName.startsWith('.')) {
        return;
    }
    
    // Get files
    QStringList nameFilters;
    nameFilters << "*.md" << "*.txt" << "*.cpp" << "*.h" << "*.hpp" << "*.c"
                << "*.py" << "*.rs" << "*.js" << "*.ts" << "*.jsx" << "*.tsx"
                << "*.json" << "*.yaml" << "*.yml" << "*.toml" << "*.html"
                << "*.css" << "*.scss" << "*.sh" << "*.bash" << "*.zsh"
                << "CMakeLists.txt" << "Makefile" << "Cargo.toml";
    
    QFileInfoList fileInfos = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable);
    for (const QFileInfo &info : fileInfos) {
        files.append(info.absoluteFilePath());
    }
    
    // Recurse into subdirectories
    QFileInfoList dirInfos = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &info : dirInfos) {
        scanDirectory(info.absoluteFilePath(), files, maxDepth - 1);
    }
}

void FuzzyFinder::indexFiles()
{
    indexedFiles_.clear();
    
    if (rootPath_.isEmpty()) {
        rootPath_ = QDir::currentPath();
    }
    
    scanDirectory(rootPath_, indexedFiles_, 8);
    
    statusLabel_->setText(QString("Indexed %1 files").arg(indexedFiles_.size()));
}

void FuzzyFinder::displayResults(const QVector<FuzzyMatch> &matches)
{
    resultsList_->clear();
    
    for (const FuzzyMatch &match : matches) {
        FuzzyListItem *item = new FuzzyListItem(match, resultsList_);
        resultsList_->addItem(item);
    }
    
    statusLabel_->setText(QString("%1 results").arg(matches.size()));
    
    if (resultsList_->count() > 0) {
        resultsList_->setCurrentRow(0);
    }
}

QString FuzzyFinder::highlightMatches(const QString &text, const QVector<int> &positions)
{
    if (positions.isEmpty()) return text;
    
    QString result;
    int lastPos = 0;
    
    for (int pos : positions) {
        if (pos >= 0 && pos < text.length()) {
            result += text.mid(lastPos, pos - lastPos);
            result += QString("<b>%1</b>").arg(text[pos]);
            lastPos = pos + 1;
        }
    }
    
    result += text.mid(lastPos);
    return result;
}
