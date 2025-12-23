#include "featurepanel.h"

FeaturePanel::FeaturePanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

FeaturePanel::~FeaturePanel() {
}

void FeaturePanel::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Title
    QLabel *titleLabel = new QLabel("Feature Toggles", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Description
    QLabel *descLabel = new QLabel(
        "Enable/disable tools and panels\n"
        "Check boxes to activate features",
        this
    );
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(descLabel);

    mainLayout->addSpacing(10);

    // File/Folder Tree
    fileTreeCheck_ = new QCheckBox("File/Folder Tree", this);
    fileTreeCheck_->setChecked(false);
    fileTreeCheck_->setToolTip("Show file browser panel on the left");
    connect(fileTreeCheck_, &QCheckBox::toggled, this, &FeaturePanel::fileTreeToggled);
    mainLayout->addWidget(fileTreeCheck_);

    // REGEX Helper
    regexHelperCheck_ = new QCheckBox("REGEX Helper", this);
    regexHelperCheck_->setChecked(true);
    regexHelperCheck_->setToolTip("Enable regex pattern testing tool");
    connect(regexHelperCheck_, &QCheckBox::toggled, this, &FeaturePanel::regexHelperToggled);
    mainLayout->addWidget(regexHelperCheck_);

    // Command Helper
    commandHelperCheck_ = new QCheckBox("Command Helper", this);
    commandHelperCheck_->setChecked(true);
    commandHelperCheck_->setToolTip("Enable shell command reference");
    connect(commandHelperCheck_, &QCheckBox::toggled, this, &FeaturePanel::commandHelperToggled);
    mainLayout->addWidget(commandHelperCheck_);

    // Text Processing Tools
    textProcessingCheck_ = new QCheckBox("Text Processing Tools", this);
    textProcessingCheck_->setChecked(true);
    textProcessingCheck_->setToolTip("Enable text transformation tools");
    connect(textProcessingCheck_, &QCheckBox::toggled, this, &FeaturePanel::textProcessingToggled);
    mainLayout->addWidget(textProcessingCheck_);

    // Shell Checker
    shellCheckerCheck_ = new QCheckBox("Shell Script Checker", this);
    shellCheckerCheck_->setChecked(true);
    shellCheckerCheck_->setToolTip("Enable shell script validation");
    connect(shellCheckerCheck_, &QCheckBox::toggled, this, &FeaturePanel::shellCheckerToggled);
    mainLayout->addWidget(shellCheckerCheck_);

    // VIM Mode
    vimModeCheck_ = new QCheckBox("VIM Mode", this);
    vimModeCheck_->setChecked(false);
    vimModeCheck_->setToolTip("Enable VIM keybindings");
    connect(vimModeCheck_, &QCheckBox::toggled, this, &FeaturePanel::vimModeToggled);
    mainLayout->addWidget(vimModeCheck_);

    mainLayout->addStretch();

    // Styling
    setStyleSheet(R"(
        QWidget {
            background-color: #252526;
        }
        QLabel {
            color: #cccccc;
            padding: 5px;
        }
        QCheckBox {
            color: #d4d4d4;
            font-size: 11px;
            spacing: 8px;
            padding: 5px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #3e3e3e;
            border-radius: 3px;
            background-color: #1e1e1e;
        }
        QCheckBox::indicator:checked {
            background-color: #0e639c;
            border-color: #1177bb;
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEzIDRMNiAxMUwzIDgiIHN0cm9rZT0id2hpdGUiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIi8+Cjwvc3ZnPgo=);
        }
        QCheckBox::indicator:hover {
            border-color: #1177bb;
        }
    )");
}

bool FeaturePanel::isFileTreeEnabled() const {
    return fileTreeCheck_->isChecked();
}

bool FeaturePanel::isRegexHelperEnabled() const {
    return regexHelperCheck_->isChecked();
}

bool FeaturePanel::isCommandHelperEnabled() const {
    return commandHelperCheck_->isChecked();
}

bool FeaturePanel::isTextProcessingEnabled() const {
    return textProcessingCheck_->isChecked();
}

bool FeaturePanel::isShellCheckerEnabled() const {
    return shellCheckerCheck_->isChecked();
}

bool FeaturePanel::isVimModeEnabled() const {
    return vimModeCheck_->isChecked();
}
