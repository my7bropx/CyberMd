#include "theme.h"

Theme::Theme(ThemeType type) : type_(type) { loadTheme(type); }
// Theme stylesheet implementations

QString Theme::getMainWindowStyleSheet() const {
  return QString(R"(
        QMainWindow {
            background-color: %1;
            color: %2;
        }
        QMainWindow::separator {
            background-color: %3;
            width: 1px;
            height: 1px;
        }
    )")
      .arg(m_backgroundColor.name())
      .arg(m_textColor.name())
      .arg(m_borderColor.name());
}

QString Theme::getEditorStyleSheet() const {
  return QString(R"(
        QPlainTextEdit {
            background-color: %1;
            color: %2;
            border: none;
            selection-background-color: %3;
            selection-color: %4;
            font-family: "JetBrains Mono", "Fira Code", "Consolas", monospace;
            font-size: 14px;
            line-height: 1.5;
        }
        QPlainTextEdit:focus {
            border: none;
            outline: none;
        }
    )")
      .arg(m_editorBackground.name())
      .arg(m_editorText.name())
      .arg(m_selectionBackground.name())
      .arg(m_selectionText.name());
}

QString Theme::getTabWidgetStyleSheet() const {
  return QString(R"(
        QTabWidget {
            background-color: %1;
            border: none;
        }
        QTabWidget::pane {
            background-color: %1;
            border: none;
            border-top: 1px solid %2;
        }
        QTabWidget::tab-bar {
            alignment: left;
        }
    )")
      .arg(m_backgroundColor.name())
      .arg(m_borderColor.name());
}

QString Theme::getTabBarStyleSheet() const {
  return QString(R"(
        QTabBar {
            background-color: %1;
            border: none;
        }
        QTabBar::tab {
            background-color: %1;
            color: %2;
            padding: 8px 16px;
            margin-right: 1px;
            border: none;
            border-bottom: 2px solid transparent;
            min-width: 100px;
        }
        QTabBar::tab:selected {
            background-color: %3;
            color: %4;
            border-bottom: 2px solid %5;
        }
        QTabBar::tab:hover:! selected {
            background-color: %6;
        }
        QTabBar::tab:! selected {
            margin-top: 2px;
        }
        QTabBar::close-button {
            image: url(:/icons/close.svg);
            subcontrol-position: right;
            padding: 2px;
        }
        QTabBar::close-button:hover {
            background-color: %7;
            border-radius: 2px;
        }
    )")
      .arg(m_tabBackground.name())       // %1 - inactive tab bg
      .arg(m_tabInactiveText.name())     // %2 - inactive tab text
      .arg(m_tabActiveBackground.name()) // %3 - active tab bg
      .arg(m_tabActiveText.name())       // %4 - active tab text
      .arg(m_accentColor.name())         // %5 - active indicator
      .arg(m_hoverBackground.name())     // %6 - hover bg
      .arg(m_dangerColor.name());        // %7 - close button hover
}

QString Theme::getButtonStyleSheet() const {
  return QString(R"(
        QPushButton {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: 500;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: %4;
            border-color: %5;
        }
        QPushButton:pressed {
            background-color: %6;
        }
        QPushButton:disabled {
            background-color: %7;
            color: %8;
            border-color: %7;
        }
        QPushButton:default {
            background-color: %5;
            color: %9;
            border-color: %5;
        }
        QPushButton:default:hover {
            background-color: %10;
        }
    )")
      .arg(m_buttonBackground.name())   // %1 - normal bg
      .arg(m_buttonText.name())         // %2 - text color
      .arg(m_borderColor.name())        // %3 - border
      .arg(m_hoverBackground.name())    // %4 - hover bg
      .arg(m_accentColor.name())        // %5 - accent/primary
      .arg(m_pressedBackground.name())  // %6 - pressed bg
      .arg(m_disabledBackground.name()) // %7 - disabled bg
      .arg(m_disabledText.name())       // %8 - disabled text
      .arg(m_accentText.name())         // %9 - accent text
      .arg(m_accentHover.name());       // %10 - accent hover
}

QString Theme::getToolButtonStyleSheet() const {
  return QString(R"(
        QToolButton {
            background-color: transparent;
            color: %1;
            border: none;
            border-radius: 4px;
            padding: 6px;
        }
        QToolButton:hover {
            background-color: %2;
        }
        QToolButton:pressed {
            background-color: %3;
        }
        QToolButton:checked {
            background-color: %4;
            color: %5;
        }
        QToolButton::menu-indicator {
            image: none;
        }
    )")
      .arg(m_iconColor.name())
      .arg(m_hoverBackground.name())
      .arg(m_pressedBackground.name())
      .arg(m_accentColor.name())
      .arg(m_accentText.name());
}

QString Theme::getSplitterStyleSheet() const {
  return QString(R"(
        QSplitter {
            background-color: %1;
        }
        QSplitter::handle {
            background-color: %2;
        }
        QSplitter::handle:horizontal {
            width: 1px;
        }
        QSplitter::handle:vertical {
            height: 1px;
        }
        QSplitter::handle:hover {
            background-color: %3;
        }
    )")
      .arg(m_backgroundColor.name())
      .arg(m_borderColor.name())
      .arg(m_accentColor.name());
}

QString Theme::getStatusBarStyleSheet() const {
  return QString(R"(
        QStatusBar {
            background-color: %1;
            color: %2;
            border-top: 1px solid %3;
            padding: 2px 8px;
            font-size: 12px;
        }
        QStatusBar::item {
            border: none;
        }
        QStatusBar QLabel {
            color: %2;
            padding: 0 8px;
        }
        QStatusBar QPushButton {
            background-color: transparent;
            border: none;
            color: %2;
            padding: 2px 8px;
        }
        QStatusBar QPushButton:hover {
            background-color: %4;
        }
    )")
      .arg(m_statusBarBackground.name())
      .arg(m_statusBarText.name())
      .arg(m_borderColor.name())
      .arg(m_hoverBackground.name());
}

QString Theme::getFeaturePanelStyleSheet() const {
  return QString(R"(
        QWidget#FeaturePanel {
            background-color: %1;
            border-right: 1px solid %2;
        }
        QWidget#FeaturePanel QLabel {
            color: %3;
            font-size: 11px;
            font-weight: 600;
            text-transform: uppercase;
            padding: 8px 12px;
        }
        QWidget#FeaturePanel QToolButton {
            background-color: transparent;
            color: %4;
            border: none;
            border-radius: 4px;
            padding: 8px;
            margin: 2px 4px;
        }
        QWidget#FeaturePanel QToolButton:hover {
            background-color: %5;
        }
        QWidget#FeaturePanel QToolButton:checked {
            background-color: %6;
            color: %7;
        }
    )")
      .arg(m_sidebarBackground.name())
      .arg(m_borderColor.name())
      .arg(m_dimmedText.name())
      .arg(m_iconColor.name())
      .arg(m_hoverBackground.name())
      .arg(m_accentColor.name())
      .arg(m_accentText.name());
}

QString Theme::getFileTreeStyleSheet() const {
  return QString(R"(
        QTreeView {
            background-color: %1;
            color: %2;
            border: none;
            outline: none;
            font-size: 13px;
        }
        QTreeView::item {
            padding: 4px 8px;
            border-radius: 4px;
            margin: 1px 4px;
        }
        QTreeView::item:hover {
            background-color: %3;
        }
        QTreeView::item:selected {
            background-color: %4;
            color: %5;
        }
        QTreeView::item:selected:! active {
            background-color: %6;
        }
        QTreeView::branch {
            background-color: transparent;
        }
        QTreeView::branch:has-siblings:!adjoins-item {
            border-image: none;
        }
        QTreeView::branch:has-siblings:adjoins-item {
            border-image: none;
        }
        QTreeView::branch:!has-children:!has-siblings:adjoins-item {
            border-image: none;
        }
        QTreeView::branch:has-children:! has-siblings:closed,
        QTreeView::branch:closed:has-children:has-siblings {
            image: url(:/icons/chevron-right.svg);
        }
        QTreeView::branch:open:has-children:!has-siblings,
        QTreeView::branch:open:has-children:has-siblings {
            image: url(:/icons/chevron-down.svg);
        }
        QHeaderView::section {
            background-color: %1;
            color: %7;
            border: none;
            padding: 6px 12px;
            font-weight: 600;
        }
    )")
      .arg(m_sidebarBackground.name())   // %1 - tree bg
      .arg(m_textColor.name())           // %2 - text
      .arg(m_hoverBackground.name())     // %3 - hover
      .arg(m_selectionBackground.name()) // %4 - selected bg
      .arg(m_selectionText.name())       // %5 - selected text
      .arg(m_inactiveSelection.name())   // %6 - inactive selection
      .arg(m_dimmedText.name());         // %7 - header text
}

QString Theme::getScrollBarStyleSheet() const {
  return QString(R"(
        QScrollBar:vertical {
            background-color: %1;
            width: 12px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background-color: %2;
            border-radius: 4px;
            min-height: 30px;
            margin: 2px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: %3;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
            background: none;
        }
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }
        QScrollBar:horizontal {
            background-color: %1;
            height: 12px;
            margin: 0;
        }
        QScrollBar::handle:horizontal {
            background-color: %2;
            border-radius: 4px;
            min-width: 30px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: %3;
        }
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0;
            background: none;
        }
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: none;
        }
    )")
      .arg(m_scrollbarBackground.name())
      .arg(m_scrollbarHandle.name())
      .arg(m_scrollbarHover.name());
}

QString Theme::getMenuStyleSheet() const {
  return QString(R"(
        QMenuBar {
            background-color: %1;
            color: %2;
            border-bottom: 1px solid %3;
            padding: 2px;
        }
        QMenuBar::item {
            background-color: transparent;
            padding: 6px 10px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background-color: %4;
        }
        QMenu {
            background-color: %5;
            color: %2;
            border: 1px solid %3;
            border-radius: 8px;
            padding: 4px;
        }
        QMenu::item {
            padding: 8px 32px 8px 24px;
            border-radius: 4px;
            margin: 2px 4px;
        }
        QMenu::item:selected {
            background-color: %4;
        }
        QMenu::item:disabled {
            color: %6;
        }
        QMenu::separator {
            height: 1px;
            background-color: %3;
            margin: 4px 12px;
        }
        QMenu::indicator {
            width: 16px;
            height: 16px;
            margin-left: 6px;
        }
        QMenu::icon {
            margin-left: 8px;
        }
        QMenu::right-arrow {
            image: url(:/icons/chevron-right.svg);
            margin-right: 8px;
        }
    )")
      .arg(m_menuBarBackground.name()) // %1 - menubar bg
      .arg(m_textColor.name())         // %2 - text
      .arg(m_borderColor.name())       // %3 - borders
      .arg(m_hoverBackground.name())   // %4 - hover
      .arg(m_menuBackground.name())    // %5 - dropdown bg
      .arg(m_disabledText.name());     // %6 - disabled text
}

QString Theme::getDialogStyleSheet() const {
  return QString(R"(
        QDialog {
            background-color: %1;
            color: %2;
        }
        QDialog QLabel {
            color: %2;
        }
        QDialog QGroupBox {
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: 600;
        }
        QDialog QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 8px;
            color: %4;
        }
        QDialog QCheckBox {
            color: %2;
            spacing: 8px;
        }
        QDialog QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid %3;
            border-radius: 4px;
            background-color: %5;
        }
        QDialog QCheckBox::indicator:checked {
            background-color: %6;
            border-color: %6;
            image: url(:/icons/check.svg);
        }
        QDialog QCheckBox::indicator:hover {
            border-color: %6;
        }
        QDialog QRadioButton {
            color: %2;
            spacing: 8px;
        }
        QDialog QRadioButton::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid %3;
            border-radius: 9px;
            background-color: %5;
        }
        QDialog QRadioButton::indicator:checked {
            background-color: %6;
            border-color: %6;
        }
    )")
      .arg(m_dialogBackground.name()) // %1 - dialog bg
      .arg(m_textColor.name())        // %2 - text
      .arg(m_borderColor.name())      // %3 - borders
      .arg(m_dimmedText.name())       // %4 - group title
      .arg(m_inputBackground.name())  // %5 - checkbox bg
      .arg(m_accentColor.name());     // %6 - accent
}

QString Theme::getInputStyleSheet() const {
  return QString(R"(
        QLineEdit {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            selection-background-color: %4;
        }
        QLineEdit:focus {
            border-color: %5;
        }
        QLineEdit:disabled {
            background-color: %6;
            color: %7;
        }
        QLineEdit::placeholder {
            color: %8;
        }
        QTextEdit {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 8px;
            selection-background-color: %4;
        }
        QTextEdit:focus {
            border-color: %5;
        }
        QSpinBox, QDoubleSpinBox {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 6px 8px;
        }
        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: %5;
        }
        QComboBox {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 8px 12px;
            min-width: 100px;
        }
        QComboBox:focus {
            border-color: %5;
        }
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        QComboBox::down-arrow {
            image: url(:/icons/chevron-down.svg);
            width: 12px;
            height: 12px;
        }
        QComboBox QAbstractItemView {
            background-color: %9;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            selection-background-color: %10;
        }
    )")
      .arg(m_inputBackground.name())     // %1 - input bg
      .arg(m_textColor.name())           // %2 - text
      .arg(m_borderColor.name())         // %3 - border
      .arg(m_selectionBackground.name()) // %4 - selection
      .arg(m_accentColor.name())         // %5 - focus border
      .arg(m_disabledBackground.name())  // %6 - disabled bg
      .arg(m_disabledText.name())        // %7 - disabled text
      .arg(m_placeholderText.name())     // %8 - placeholder
      .arg(m_menuBackground.name())      // %9 - dropdown bg
      .arg(m_hoverBackground.name());    // %10 - dropdown hover
}

QString Theme::getFuzzyFinderStyleSheet() const {
  return QString(R"(
        QWidget#FuzzyFinder {
            background-color: %1;
            border: 1px solid %2;
            border-radius: 12px;
        }
        QWidget#FuzzyFinder QLineEdit {
            background-color: %3;
            color: %4;
            border: none;
            border-bottom: 1px solid %2;
            border-radius: 0;
            padding: 16px 20px;
            font-size: 16px;
        }
        QWidget#FuzzyFinder QLineEdit:focus {
            border-bottom-color: %5;
        }
        QWidget#FuzzyFinder QListWidget {
            background-color: transparent;
            color: %4;
            border: none;
            outline: none;
            padding: 8px;
        }
        QWidget#FuzzyFinder QListWidget::item {
            padding: 10px 16px;
            border-radius: 6px;
            margin: 2px 4px;
        }
        QWidget#FuzzyFinder QListWidget::item:hover {
            background-color: %6;
        }
        QWidget#FuzzyFinder QListWidget::item:selected {
            background-color: %7;
            color: %8;
        }
        QWidget#FuzzyFinder QLabel#ModeLabel {
            color: %9;
            font-size: 11px;
            font-weight: 600;
            text-transform: uppercase;
            padding: 8px 20px;
            background-color: %10;
        }
        QWidget#FuzzyFinder QLabel#HintLabel {
            color: %9;
            font-size: 11px;
            padding: 8px 20px;
        }
    )")
      .arg(m_fuzzyBackground.name())     // %1 - main bg
      .arg(m_borderColor.name())         // %2 - borders
      .arg(m_inputBackground.name())     // %3 - input bg
      .arg(m_textColor.name())           // %4 - text
      .arg(m_accentColor.name())         // %5 - focus accent
      .arg(m_hoverBackground.name())     // %6 - hover
      .arg(m_selectionBackground.name()) // %7 - selected bg
      .arg(m_selectionText.name())       // %8 - selected text
      .arg(m_dimmedText.name())          // %9 - dimmed/hint text
      .arg(m_headerBackground.name());   // %10 - mode label bg
}

QString Theme::getPreviewStyleSheet() const {
  return QString(R"(
        QWidget#PreviewPanel {
            background-color: %1;
            border-left: 1px solid %2;
        }
        QWidget#PreviewPanel QLabel#Title {
            color: %3;
            font-size: 14px;
            font-weight: 600;
            padding: 12px 16px;
            background-color: %4;
            border-bottom: 1px solid %2;
        }
        QWidget#PreviewPanel QTextBrowser {
            background-color: %1;
            color: %5;
            border: none;
            padding: 16px;
        }
        QWidget#PreviewPanel QTextBrowser a {
            color: %6;
        }
    )")
      .arg(m_previewBackground.name()) // %1 - preview bg
      .arg(m_borderColor.name())       // %2 - border
      .arg(m_textColor.name())         // %3 - title text
      .arg(m_headerBackground.name())  // %4 - header bg
      .arg(m_previewText.name())       // %5 - content text
      .arg(m_linkColor.name());        // %6 - links
}

QString Theme::getTooltipStyleSheet() const {
  return QString(R"(
        QToolTip {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 12px;
        }
    )")
      .arg(m_tooltipBackground.name())
      .arg(m_tooltipText.name())
      .arg(m_borderColor.name());
}
QMap<Theme::ThemeType, QString> Theme::availableThemes() {
  QMap<ThemeType, QString> themes;
  themes[DarkDefault] = "Dark (Default)";
  themes[Light] = "Light";
  themes[Dracula] = "Dracula";
  themes[Monokai] = "Monokai";
  themes[Nord] = "Nord";
  themes[SolarizedDark] = "Solarized Dark";
  themes[SolarizedLight] = "Solarized Light";
  themes[GruvboxDark] = "Gruvbox Dark";
  themes[GruvboxLight] = "Gruvbox Light";
  themes[OneDark] = "One Dark";
  themes[MaterialDark] = "Material Dark";
  themes[TokyoNight] = "Tokyo Night";
  themes[Catppuccin] = "Catppuccin";
  themes[CyberPunk] = "CyberPunk";
  themes[MatrixGreen] = "Matrix Green";
  return themes;
}

Theme::ThemeType Theme::themeFromName(const QString &name) {
  auto themes = availableThemes();
  for (auto it = themes.begin(); it != themes.end(); ++it) {
    if (it.value() == name)
      return it.key();
  }
  return DarkDefault;
}

void Theme::loadTheme(ThemeType type) {
  switch (type) {
  case Light:
    loadLightTheme();
    break;
  case Dracula:
    loadDraculaTheme();
    break;
  case Monokai:
    loadMonokaiTheme();
    break;
  case Nord:
    loadNordTheme();
    break;
  case SolarizedDark:
    loadSolarizedDarkTheme();
    break;
  case SolarizedLight:
    loadSolarizedLightTheme();
    break;
  case GruvboxDark:
    loadGruvboxDarkTheme();
    break;
  case GruvboxLight:
    loadGruvboxLightTheme();
    break;
  case OneDark:
    loadOneDarkTheme();
    break;
  case MaterialDark:
    loadMaterialDarkTheme();
    break;
  case TokyoNight:
    loadTokyoNightTheme();
    break;
  case Catppuccin:
    loadCatppuccinTheme();
    break;
  case CyberPunk:
    loadCyberPunkTheme();
    break;
  case MatrixGreen:
    loadMatrixGreenTheme();
    break;
  default:
    loadDarkDefaultTheme();
    break;
  }
}

void Theme::loadDarkDefaultTheme() {
  name_ = "Dark (Default)";

  // Editor
  editorBackground_ = QColor("#1e1e1e");
  editorForeground_ = QColor("#d4d4d4");
  editorCurrentLine_ = QColor("#2a2d2e");
  editorSelection_ = QColor("#264f78");
  editorSelectionForeground_ = QColor("#ffffff");
  editorCursor_ = QColor("#ffffff");
  editorWhitespace_ = QColor("#3b3b3b");
  editorIndentGuide_ = QColor("#404040");

  // Line numbers
  lineNumberForeground_ = QColor("#858585");
  lineNumberBackground_ = QColor("#1e1e1e");
  lineNumberActiveForeground_ = QColor("#c6c6c6");

  // Folding
  foldingBackground_ = QColor("#1e1e1e");
  foldingMarker_ = QColor("#808080");
  foldingMarkerHover_ = QColor("#d4d4d4");
  foldedLineBackground_ = QColor("#2d2d30");

  // Tabs
  tabBackground_ = QColor("#252526");
  tabForeground_ = QColor("#969696");
  tabActiveBackground_ = QColor("#1e1e1e");
  tabActiveForeground_ = QColor("#ffffff");
  tabHoverBackground_ = QColor("#2d2d30");
  tabBorder_ = QColor("#3c3c3c");
  tabModifiedIndicator_ = QColor("#e2c08d");
  tabCloseButton_ = QColor("#808080");
  tabCloseButtonHover_ = QColor("#ffffff");

  // UI
  uiBackground_ = QColor("#252526");
  uiForeground_ = QColor("#cccccc");
  uiBorder_ = QColor("#3c3c3c");
  uiAccent_ = QColor("#007acc");
  uiAccentHover_ = QColor("#1c97ea");
  uiButtonBackground_ = QColor("#3c3c3c");
  uiButtonForeground_ = QColor("#ffffff");
  uiButtonHover_ = QColor("#4e4e4e");
  uiButtonPressed_ = QColor("#383838");
  uiInputBackground_ = QColor("#3c3c3c");
  uiInputForeground_ = QColor("#cccccc");
  uiInputBorder_ = QColor("#3c3c3c");
  uiInputFocusBorder_ = QColor("#007acc");

  // Scrollbar
  scrollbarBackground_ = QColor("#1e1e1e");
  scrollbarHandle_ = QColor("#424242");
  scrollbarHandleHover_ = QColor("#4f4f4f");

  // File tree
  treeBackground_ = QColor("#252526");
  treeForeground_ = QColor("#cccccc");
  treeSelectedBackground_ = QColor("#094771");
  treeSelectedForeground_ = QColor("#ffffff");
  treeHoverBackground_ = QColor("#2a2d2e");
  treeFolderIcon_ = QColor("#dcb67a");
  treeFileIcon_ = QColor("#808080");

  // Status bar
  statusBarBackground_ = QColor("#007acc");
  statusBarForeground_ = QColor("#ffffff");
  statusBarBorder_ = QColor("#007acc");

  // Markdown syntax
  syntaxHeading1_ = QColor("#569cd6");
  syntaxHeading2_ = QColor("#4ec9b0");
  syntaxHeading3_ = QColor("#9cdcfe");
  syntaxHeading4_ = QColor("#b5cea8");
  syntaxHeading5_ = QColor("#c586c0");
  syntaxHeading6_ = QColor("#ce9178");
  syntaxParagraph_ = QColor("#d4d4d4");
  syntaxCode_ = QColor("#ce9178");
  syntaxCodeBackground_ = QColor("#2d2d30");
  syntaxCodeBlockBackground_ = QColor("#1e1e1e");
  syntaxCodeBlockBorder_ = QColor("#3c3c3c");
  syntaxBold_ = QColor("#569cd6");
  syntaxItalic_ = QColor("#9cdcfe");
  syntaxStrikethrough_ = QColor("#808080");
  syntaxLink_ = QColor("#4ec9b0");
  syntaxLinkUrl_ = QColor("#3794ff");
  syntaxImage_ = QColor("#c586c0");
  syntaxListMarker_ = QColor("#6a9955");
  syntaxBlockQuote_ = QColor("#608b4e");
  syntaxBlockQuoteBorder_ = QColor("#608b4e");
  syntaxHorizontalRule_ = QColor("#808080");
  syntaxTable_ = QColor("#d4d4d4");
  syntaxTableBorder_ = QColor("#808080");
  syntaxComment_ = QColor("#6a9955");

  // Code syntax
  codeKeyword_ = QColor("#569cd6");
  codeKeyword2_ = QColor("#c586c0");
  codeClass_ = QColor("#4ec9b0");
  codeFunction_ = QColor("#dcdcaa");
  codeFunctionCall_ = QColor("#dcdcaa");
  codeVariable_ = QColor("#9cdcfe");
  codeParameter_ = QColor("#9cdcfe");
  codeString_ = QColor("#ce9178");
  codeStringEscape_ = QColor("#d7ba7d");
  codeNumber_ = QColor("#b5cea8");
  codeOperator_ = QColor("#d4d4d4");
  codePreprocessor_ = QColor("#c586c0");
  codeMacro_ = QColor("#4fc1ff");
  codeComment_ = QColor("#6a9955");
  codeDocComment_ = QColor("#608b4e");
  codeType_ = QColor("#4ec9b0");
  codeConstant_ = QColor("#4fc1ff");
  codeAttribute_ = QColor("#9cdcfe");
  codeNamespace_ = QColor("#4ec9b0");
  codeBracket_ = QColor("#ffd700");
  codeBracketMatch_ = QColor("#515050");
  codeRegex_ = QColor("#d16969");

  // VIM mode
  vimNormal_ = QColor("#007acc");
  vimInsert_ = QColor("#4caf50");
  vimVisual_ = QColor("#9c27b0");
  vimCommand_ = QColor("#ff9800");
  vimReplace_ = QColor("#f44336");

  // Diagnostics
  errorColor_ = QColor("#f44747");
  errorBackground_ = QColor("#5a1d1d");
  warningColor_ = QColor("#cca700");
  warningBackground_ = QColor("#4d3800");
  infoColor_ = QColor("#3794ff");
  infoBackground_ = QColor("#063b49");
  hintColor_ = QColor("#2db7b5");
  successColor_ = QColor("#89d185");

  // Diff
  diffAdded_ = QColor("#4caf50");
  diffRemoved_ = QColor("#f44336");
  diffModified_ = QColor("#ff9800");
  diffAddedBackground_ = QColor("#0d3d0d");
  diffRemovedBackground_ = QColor("#3d0d0d");

  // Search
  searchMatch_ = QColor("#f0f000");
  searchMatchBackground_ = QColor("#613214");
  searchCurrentMatch_ = QColor("#ffff00");
  searchCurrentMatchBackground_ = QColor("#515c6a");

  // Fuzzy finder
  fuzzyBackground_ = QColor("#252526");
  fuzzyForeground_ = QColor("#cccccc");
  fuzzyMatch_ = QColor("#ffcc00");
  fuzzySelected_ = QColor("#ffffff");
  fuzzySelectedBackground_ = QColor("#094771");
  fuzzyBorder_ = QColor("#007acc");

  // Preview
  previewBackground_ = QColor("#1e1e1e");
  previewForeground_ = QColor("#d4d4d4");
  previewHeading_ = QColor("#569cd6");
  previewLink_ = QColor("#4ec9b0");
  previewCodeBackground_ = QColor("#2d2d30");
  previewBlockquoteBackground_ = QColor("#252526");
  previewBlockquoteBorder_ = QColor("#608b4e");
}

void Theme::loadLightTheme() {
  name_ = "Light";

  editorBackground_ = QColor("#ffffff");
  editorForeground_ = QColor("#000000");
  editorCurrentLine_ = QColor("#f3f3f3");
  editorSelection_ = QColor("#add6ff");
  editorSelectionForeground_ = QColor("#000000");
  editorCursor_ = QColor("#000000");
  editorWhitespace_ = QColor("#d3d3d3");
  editorIndentGuide_ = QColor("#e0e0e0");

  lineNumberForeground_ = QColor("#237893");
  lineNumberBackground_ = QColor("#ffffff");
  lineNumberActiveForeground_ = QColor("#0b216f");

  foldingBackground_ = QColor("#ffffff");
  foldingMarker_ = QColor("#808080");
  foldingMarkerHover_ = QColor("#000000");
  foldedLineBackground_ = QColor("#f0f0f0");

  tabBackground_ = QColor("#f3f3f3");
  tabForeground_ = QColor("#6e6e6e");
  tabActiveBackground_ = QColor("#ffffff");
  tabActiveForeground_ = QColor("#333333");
  tabHoverBackground_ = QColor("#e8e8e8");
  tabBorder_ = QColor("#d4d4d4");
  tabModifiedIndicator_ = QColor("#c27d1a");
  tabCloseButton_ = QColor("#808080");
  tabCloseButtonHover_ = QColor("#000000");

  uiBackground_ = QColor("#f3f3f3");
  uiForeground_ = QColor("#616161");
  uiBorder_ = QColor("#d4d4d4");
  uiAccent_ = QColor("#0066b8");
  uiAccentHover_ = QColor("#005a9e");
  uiButtonBackground_ = QColor("#e0e0e0");
  uiButtonForeground_ = QColor("#333333");
  uiButtonHover_ = QColor("#d0d0d0");
  uiButtonPressed_ = QColor("#c0c0c0");
  uiInputBackground_ = QColor("#ffffff");
  uiInputForeground_ = QColor("#000000");
  uiInputBorder_ = QColor("#cecece");
  uiInputFocusBorder_ = QColor("#0066b8");

  scrollbarBackground_ = QColor("#f3f3f3");
  scrollbarHandle_ = QColor("#c1c1c1");
  scrollbarHandleHover_ = QColor("#929292");

  treeBackground_ = QColor("#f3f3f3");
  treeForeground_ = QColor("#616161");
  treeSelectedBackground_ = QColor("#0060c0");
  treeSelectedForeground_ = QColor("#ffffff");
  treeHoverBackground_ = QColor("#e8e8e8");
  treeFolderIcon_ = QColor("#dcb67a");
  treeFileIcon_ = QColor("#616161");

  statusBarBackground_ = QColor("#0066b8");
  statusBarForeground_ = QColor("#ffffff");
  statusBarBorder_ = QColor("#0066b8");

  syntaxHeading1_ = QColor("#000080");
  syntaxHeading2_ = QColor("#267f99");
  syntaxHeading3_ = QColor("#001080");
  syntaxHeading4_ = QColor("#098658");
  syntaxHeading5_ = QColor("#af00db");
  syntaxHeading6_ = QColor("#a31515");
  syntaxParagraph_ = QColor("#000000");
  syntaxCode_ = QColor("#a31515");
  syntaxCodeBackground_ = QColor("#f0f0f0");
  syntaxCodeBlockBackground_ = QColor("#f5f5f5");
  syntaxCodeBlockBorder_ = QColor("#d4d4d4");
  syntaxBold_ = QColor("#000080");
  syntaxItalic_ = QColor("#001080");
  syntaxStrikethrough_ = QColor("#808080");
  syntaxLink_ = QColor("#267f99");
  syntaxLinkUrl_ = QColor("#0000ff");
  syntaxImage_ = QColor("#af00db");
  syntaxListMarker_ = QColor("#098658");
  syntaxBlockQuote_ = QColor("#008000");
  syntaxBlockQuoteBorder_ = QColor("#098658");
  syntaxHorizontalRule_ = QColor("#808080");
  syntaxTable_ = QColor("#000000");
  syntaxTableBorder_ = QColor("#808080");
  syntaxComment_ = QColor("#008000");

  codeKeyword_ = QColor("#0000ff");
  codeKeyword2_ = QColor("#af00db");
  codeClass_ = QColor("#267f99");
  codeFunction_ = QColor("#795e26");
  codeFunctionCall_ = QColor("#795e26");
  codeVariable_ = QColor("#001080");
  codeParameter_ = QColor("#001080");
  codeString_ = QColor("#a31515");
  codeStringEscape_ = QColor("#ee0000");
  codeNumber_ = QColor("#098658");
  codeOperator_ = QColor("#000000");
  codePreprocessor_ = QColor("#af00db");
  codeMacro_ = QColor("#0070c1");
  codeComment_ = QColor("#008000");
  codeDocComment_ = QColor("#008000");
  codeType_ = QColor("#267f99");
  codeConstant_ = QColor("#0070c1");
  codeAttribute_ = QColor("#001080");
  codeNamespace_ = QColor("#267f99");
  codeBracket_ = QColor("#000000");
  codeBracketMatch_ = QColor("#add6ff");
  codeRegex_ = QColor("#811f3f");

  vimNormal_ = QColor("#0066b8");
  vimInsert_ = QColor("#388e3c");
  vimVisual_ = QColor("#7b1fa2");
  vimCommand_ = QColor("#f57c00");
  vimReplace_ = QColor("#d32f2f");

  errorColor_ = QColor("#d32f2f");
  errorBackground_ = QColor("#ffebee");
  warningColor_ = QColor("#f57c00");
  warningBackground_ = QColor("#fff3e0");
  infoColor_ = QColor("#1976d2");
  infoBackground_ = QColor("#e3f2fd");
  hintColor_ = QColor("#00897b");
  successColor_ = QColor("#388e3c");

  diffAdded_ = QColor("#388e3c");
  diffRemoved_ = QColor("#d32f2f");
  diffModified_ = QColor("#f57c00");
  diffAddedBackground_ = QColor("#e8f5e9");
  diffRemovedBackground_ = QColor("#ffebee");

  searchMatch_ = QColor("#000000");
  searchMatchBackground_ = QColor("#ffeb3b");
  searchCurrentMatch_ = QColor("#000000");
  searchCurrentMatchBackground_ = QColor("#ff9800");

  fuzzyBackground_ = QColor("#ffffff");
  fuzzyForeground_ = QColor("#616161");
  fuzzyMatch_ = QColor("#e65100");
  fuzzySelected_ = QColor("#ffffff");
  fuzzySelectedBackground_ = QColor("#0066b8");
  fuzzyBorder_ = QColor("#0066b8");

  previewBackground_ = QColor("#ffffff");
  previewForeground_ = QColor("#000000");
  previewHeading_ = QColor("#000080");
  previewLink_ = QColor("#267f99");
  previewCodeBackground_ = QColor("#f0f0f0");
  previewBlockquoteBackground_ = QColor("#f5f5f5");
  previewBlockquoteBorder_ = QColor("#098658");
}

void Theme::loadDraculaTheme() {
  name_ = "Dracula";

  editorBackground_ = QColor("#282a36");
  editorForeground_ = QColor("#f8f8f2");
  editorCurrentLine_ = QColor("#44475a");
  editorSelection_ = QColor("#44475a");
  editorSelectionForeground_ = QColor("#f8f8f2");
  editorCursor_ = QColor("#f8f8f2");
  editorWhitespace_ = QColor("#424450");
  editorIndentGuide_ = QColor("#424450");

  lineNumberForeground_ = QColor("#6272a4");
  lineNumberBackground_ = QColor("#282a36");
  lineNumberActiveForeground_ = QColor("#f8f8f2");

  foldingBackground_ = QColor("#282a36");
  foldingMarker_ = QColor("#6272a4");
  foldingMarkerHover_ = QColor("#f8f8f2");
  foldedLineBackground_ = QColor("#44475a");

  tabBackground_ = QColor("#21222c");
  tabForeground_ = QColor("#6272a4");
  tabActiveBackground_ = QColor("#282a36");
  tabActiveForeground_ = QColor("#f8f8f2");
  tabHoverBackground_ = QColor("#343746");
  tabBorder_ = QColor("#191a21");
  tabModifiedIndicator_ = QColor("#ffb86c");
  tabCloseButton_ = QColor("#6272a4");
  tabCloseButtonHover_ = QColor("#ff5555");

  uiBackground_ = QColor("#21222c");
  uiForeground_ = QColor("#f8f8f2");
  uiBorder_ = QColor("#191a21");
  uiAccent_ = QColor("#bd93f9");
  uiAccentHover_ = QColor("#ff79c6");
  uiButtonBackground_ = QColor("#44475a");
  uiButtonForeground_ = QColor("#f8f8f2");
  uiButtonHover_ = QColor("#6272a4");
  uiButtonPressed_ = QColor("#343746");
  uiInputBackground_ = QColor("#44475a");
  uiInputForeground_ = QColor("#f8f8f2");
  uiInputBorder_ = QColor("#6272a4");
  uiInputFocusBorder_ = QColor("#bd93f9");

  scrollbarBackground_ = QColor("#282a36");
  scrollbarHandle_ = QColor("#44475a");
  scrollbarHandleHover_ = QColor("#6272a4");

  treeBackground_ = QColor("#21222c");
  treeForeground_ = QColor("#f8f8f2");
  treeSelectedBackground_ = QColor("#44475a");
  treeSelectedForeground_ = QColor("#f8f8f2");
  treeHoverBackground_ = QColor("#343746");
  treeFolderIcon_ = QColor("#ffb86c");
  treeFileIcon_ = QColor("#6272a4");

  statusBarBackground_ = QColor("#191a21");
  statusBarForeground_ = QColor("#f8f8f2");
  statusBarBorder_ = QColor("#191a21");

  syntaxHeading1_ = QColor("#ff79c6");
  syntaxHeading2_ = QColor("#bd93f9");
  syntaxHeading3_ = QColor("#8be9fd");
  syntaxHeading4_ = QColor("#50fa7b");
  syntaxHeading5_ = QColor("#ffb86c");
  syntaxHeading6_ = QColor("#ff5555");
  syntaxParagraph_ = QColor("#f8f8f2");
  syntaxCode_ = QColor("#50fa7b");
  syntaxCodeBackground_ = QColor("#44475a");
  syntaxCodeBlockBackground_ = QColor("#282a36");
  syntaxCodeBlockBorder_ = QColor("#6272a4");
  syntaxBold_ = QColor("#ffb86c");
  syntaxItalic_ = QColor("#f1fa8c");
  syntaxStrikethrough_ = QColor("#6272a4");
  syntaxLink_ = QColor("#8be9fd");
  syntaxLinkUrl_ = QColor("#bd93f9");
  syntaxImage_ = QColor("#ff79c6");
  syntaxListMarker_ = QColor("#50fa7b");
  syntaxBlockQuote_ = QColor("#f1fa8c");
  syntaxBlockQuoteBorder_ = QColor("#ffb86c");
  syntaxHorizontalRule_ = QColor("#6272a4");
  syntaxTable_ = QColor("#f8f8f2");
  syntaxTableBorder_ = QColor("#6272a4");
  syntaxComment_ = QColor("#6272a4");

  codeKeyword_ = QColor("#ff79c6");
  codeKeyword2_ = QColor("#bd93f9");
  codeClass_ = QColor("#8be9fd");
  codeFunction_ = QColor("#50fa7b");
  codeFunctionCall_ = QColor("#50fa7b");
  codeVariable_ = QColor("#f8f8f2");
  codeParameter_ = QColor("#ffb86c");
  codeString_ = QColor("#f1fa8c");
  codeStringEscape_ = QColor("#ff79c6");
  codeNumber_ = QColor("#bd93f9");
  codeOperator_ = QColor("#ff79c6");
  codePreprocessor_ = QColor("#ff79c6");
  codeMacro_ = QColor("#8be9fd");
  codeComment_ = QColor("#6272a4");
  codeDocComment_ = QColor("#6272a4");
  codeType_ = QColor("#8be9fd");
  codeConstant_ = QColor("#bd93f9");
  codeAttribute_ = QColor("#50fa7b");
  codeNamespace_ = QColor("#ff79c6");
  codeBracket_ = QColor("#f8f8f2");
  codeBracketMatch_ = QColor("#44475a");
  codeRegex_ = QColor("#ff5555");

  vimNormal_ = QColor("#bd93f9");
  vimInsert_ = QColor("#50fa7b");
  vimVisual_ = QColor("#ff79c6");
  vimCommand_ = QColor("#ffb86c");
  vimReplace_ = QColor("#ff5555");

  errorColor_ = QColor("#ff5555");
  errorBackground_ = QColor("#4d1f24");
  warningColor_ = QColor("#ffb86c");
  warningBackground_ = QColor("#4d3800");
  infoColor_ = QColor("#8be9fd");
  infoBackground_ = QColor("#1f3d4d");
  hintColor_ = QColor("#50fa7b");
  successColor_ = QColor("#50fa7b");

  diffAdded_ = QColor("#50fa7b");
  diffRemoved_ = QColor("#ff5555");
  diffModified_ = QColor("#ffb86c");
  diffAddedBackground_ = QColor("#1f4d2a");
  diffRemovedBackground_ = QColor("#4d1f24");

  searchMatch_ = QColor("#000000");
  searchMatchBackground_ = QColor("#f1fa8c");
  searchCurrentMatch_ = QColor("#000000");
  searchCurrentMatchBackground_ = QColor("#ffb86c");

  fuzzyBackground_ = QColor("#282a36");
  fuzzyForeground_ = QColor("#f8f8f2");
  fuzzyMatch_ = QColor("#f1fa8c");
  fuzzySelected_ = QColor("#f8f8f2");
  fuzzySelectedBackground_ = QColor("#44475a");
  fuzzyBorder_ = QColor("#bd93f9");

  previewBackground_ = QColor("#282a36");
  previewForeground_ = QColor("#f8f8f2");
  previewHeading_ = QColor("#ff79c6");
  previewLink_ = QColor("#8be9fd");
  previewCodeBackground_ = QColor("#44475a");
  previewBlockquoteBackground_ = QColor("#21222c");
  previewBlockquoteBorder_ = QColor("#ffb86c");
}

void Theme::loadMonokaiTheme() {
  name_ = "Monokai";

  editorBackground_ = QColor("#272822");
  editorForeground_ = QColor("#f8f8f2");
  editorCurrentLine_ = QColor("#3e3d32");
  editorSelection_ = QColor("#49483e");
  editorSelectionForeground_ = QColor("#f8f8f2");
  editorCursor_ = QColor("#f8f8f2");
  editorWhitespace_ = QColor("#464741");
  editorIndentGuide_ = QColor("#464741");

  lineNumberForeground_ = QColor("#90908a");
  lineNumberBackground_ = QColor("#272822");
  lineNumberActiveForeground_ = QColor("#c2c2bf");

  foldingBackground_ = QColor("#272822");
  foldingMarker_ = QColor("#75715e");
  foldingMarkerHover_ = QColor("#f8f8f2");
  foldedLineBackground_ = QColor("#3e3d32");

  tabBackground_ = QColor("#1e1f1c");
  tabForeground_ = QColor("#75715e");
  tabActiveBackground_ = QColor("#272822");
  tabActiveForeground_ = QColor("#f8f8f2");
  tabHoverBackground_ = QColor("#3e3d32");
  tabBorder_ = QColor("#1e1f1c");
  tabModifiedIndicator_ = QColor("#e6db74");
  tabCloseButton_ = QColor("#75715e");
  tabCloseButtonHover_ = QColor("#f92672");

  uiBackground_ = QColor("#1e1f1c");
  uiForeground_ = QColor("#f8f8f2");
  uiBorder_ = QColor("#1e1f1c");
  uiAccent_ = QColor("#a6e22e");
  uiAccentHover_ = QColor("#f92672");
  uiButtonBackground_ = QColor("#49483e");
  uiButtonForeground_ = QColor("#f8f8f2");
  uiButtonHover_ = QColor("#75715e");
  uiButtonPressed_ = QColor("#3e3d32");
  uiInputBackground_ = QColor("#49483e");
  uiInputForeground_ = QColor("#f8f8f2");
  uiInputBorder_ = QColor("#75715e");
  uiInputFocusBorder_ = QColor("#a6e22e");

  scrollbarBackground_ = QColor("#272822");
  scrollbarHandle_ = QColor("#49483e");
  scrollbarHandleHover_ = QColor("#75715e");

  treeBackground_ = QColor("#1e1f1c");
  treeForeground_ = QColor("#f8f8f2");
  treeSelectedBackground_ = QColor("#49483e");
  treeSelectedForeground_ = QColor("#f8f8f2");
  treeHoverBackground_ = QColor("#3e3d32");
  treeFolderIcon_ = QColor("#e6db74");
  treeFileIcon_ = QColor("#75715e");

  statusBarBackground_ = QColor("#1e1f1c");
  statusBarForeground_ = QColor("#f8f8f2");
  statusBarBorder_ = QColor("#1e1f1c");

  syntaxHeading1_ = QColor("#f92672");
  syntaxHeading2_ = QColor("#a6e22e");
  syntaxHeading3_ = QColor("#66d9ef");
  syntaxHeading4_ = QColor("#e6db74");
  syntaxHeading5_ = QColor("#ae81ff");
  syntaxHeading6_ = QColor("#fd971f");
  syntaxParagraph_ = QColor("#f8f8f2");
  syntaxCode_ = QColor("#a6e22e");
  syntaxCodeBackground_ = QColor("#3e3d32");
  syntaxCodeBlockBackground_ = QColor("#272822");
  syntaxCodeBlockBorder_ = QColor("#49483e");
  syntaxBold_ = QColor("#f92672");
  syntaxItalic_ = QColor("#e6db74");
  syntaxStrikethrough_ = QColor("#75715e");
  syntaxLink_ = QColor("#66d9ef");
  syntaxLinkUrl_ = QColor("#ae81ff");
  syntaxImage_ = QColor("#f92672");
  syntaxListMarker_ = QColor("#a6e22e");
  syntaxBlockQuote_ = QColor("#75715e");
  syntaxBlockQuoteBorder_ = QColor("#fd971f");
  syntaxHorizontalRule_ = QColor("#75715e");
  syntaxTable_ = QColor("#f8f8f2");
  syntaxTableBorder_ = QColor("#75715e");
  syntaxComment_ = QColor("#75715e");

  codeKeyword_ = QColor("#f92672");
  codeKeyword2_ = QColor("#66d9ef");
  codeClass_ = QColor("#a6e22e");
  codeFunction_ = QColor("#a6e22e");
  codeFunctionCall_ = QColor("#66d9ef");
  codeVariable_ = QColor("#f8f8f2");
  codeParameter_ = QColor("#fd971f");
  codeString_ = QColor("#e6db74");
  codeStringEscape_ = QColor("#ae81ff");
  codeNumber_ = QColor("#ae81ff");
  codeOperator_ = QColor("#f92672");
  codePreprocessor_ = QColor("#ae81ff");
  codeMacro_ = QColor("#66d9ef");
  codeComment_ = QColor("#75715e");
  codeDocComment_ = QColor("#75715e");
  codeType_ = QColor("#66d9ef");
  codeConstant_ = QColor("#ae81ff");
  codeAttribute_ = QColor("#a6e22e");
  codeNamespace_ = QColor("#f92672");
  codeBracket_ = QColor("#f8f8f2");
  codeBracketMatch_ = QColor("#49483e");
  codeRegex_ = QColor("#e6db74");

  vimNormal_ = QColor("#a6e22e");
  vimInsert_ = QColor("#66d9ef");
  vimVisual_ = QColor("#ae81ff");
  vimCommand_ = QColor("#fd971f");
  vimReplace_ = QColor("#f92672");

  errorColor_ = QColor("#f92672");
  errorBackground_ = QColor("#4d1f24");
  warningColor_ = QColor("#e6db74");
  warningBackground_ = QColor("#4d4400");
  infoColor_ = QColor("#66d9ef");
  infoBackground_ = QColor("#1f3d4d");
  hintColor_ = QColor("#a6e22e");
  successColor_ = QColor("#a6e22e");

  diffAdded_ = QColor("#a6e22e");
  diffRemoved_ = QColor("#f92672");
  diffModified_ = QColor("#e6db74");
  diffAddedBackground_ = QColor("#2d4a1c");
  diffRemovedBackground_ = QColor("#4d1f24");

  searchMatch_ = QColor("#000000");
  searchMatchBackground_ = QColor("#e6db74");
  searchCurrentMatch_ = QColor("#000000");
  searchCurrentMatchBackground_ = QColor("#fd971f");

  fuzzyBackground_ = QColor("#272822");
  fuzzyForeground_ = QColor("#f8f8f2");
  fuzzyMatch_ = QColor("#e6db74");
  fuzzySelected_ = QColor("#f8f8f2");
  fuzzySelectedBackground_ = QColor("#49483e");
  fuzzyBorder_ = QColor("#a6e22e");

  previewBackground_ = QColor("#272822");
  previewForeground_ = QColor("#f8f8f2");
  previewHeading_ = QColor("#f92672");
  previewLink_ = QColor("#66d9ef");
  previewCodeBackground_ = QColor("#3e3d32");
  previewBlockquoteBackground_ = QColor("#1e1f1c");
  previewBlockquoteBorder_ = QColor("#fd971f");
}

void Theme::loadNordTheme() {
  name_ = "Nord";

  editorBackground_ = QColor("#2e3440");
  editorForeground_ = QColor("#d8dee9");
  editorCurrentLine_ = QColor("#3b4252");
  editorSelection_ = QColor("#434c5e");
  editorSelectionForeground_ = QColor("#eceff4");
  editorCursor_ = QColor("#d8dee9");
  editorWhitespace_ = QColor("#4c566a");
  editorIndentGuide_ = QColor("#4c566a");

  lineNumberForeground_ = QColor("#4c566a");
  lineNumberBackground_ = QColor("#2e3440");
  lineNumberActiveForeground_ = QColor("#d8dee9");

  foldingBackground_ = QColor("#2e3440");
  foldingMarker_ = QColor("#4c566a");
  foldingMarkerHover_ = QColor("#d8dee9");
  foldedLineBackground_ = QColor("#3b4252");

  tabBackground_ = QColor("#2e3440");
  tabForeground_ = QColor("#4c566a");
  tabActiveBackground_ = QColor("#3b4252");
  tabActiveForeground_ = QColor("#eceff4");
  tabHoverBackground_ = QColor("#434c5e");
  tabBorder_ = QColor("#2e3440");
  tabModifiedIndicator_ = QColor("#ebcb8b");
  tabCloseButton_ = QColor("#4c566a");
  tabCloseButtonHover_ = QColor("#bf616a");

  uiBackground_ = QColor("#2e3440");
  uiForeground_ = QColor("#d8dee9");
  uiBorder_ = QColor("#3b4252");
  uiAccent_ = QColor("#81a1c1");
  uiAccentHover_ = QColor("#88c0d0");
  uiButtonBackground_ = QColor("#4c566a");
  uiButtonForeground_ = QColor("#eceff4");
  uiButtonHover_ = QColor("#5e81ac");
  uiButtonPressed_ = QColor("#434c5e");
  uiInputBackground_ = QColor("#3b4252");
  uiInputForeground_ = QColor("#d8dee9");
  uiInputBorder_ = QColor("#4c566a");
  uiInputFocusBorder_ = QColor("#81a1c1");

  scrollbarBackground_ = QColor("#2e3440");
  scrollbarHandle_ = QColor("#4c566a");
  scrollbarHandleHover_ = QColor("#5e81ac");

  treeBackground_ = QColor("#2e3440");
  treeForeground_ = QColor("#d8dee9");
  treeSelectedBackground_ = QColor("#434c5e");
  treeSelectedForeground_ = QColor("#eceff4");
  treeHoverBackground_ = QColor("#3b4252");
  treeFolderIcon_ = QColor("#ebcb8b");
  treeFileIcon_ = QColor("#81a1c1");

  statusBarBackground_ = QColor("#3b4252");
  statusBarForeground_ = QColor("#d8dee9");
  statusBarBorder_ = QColor("#3b4252");

  syntaxHeading1_ = QColor("#8fbcbb");
  syntaxHeading2_ = QColor("#88c0d0");
  syntaxHeading3_ = QColor("#81a1c1");
  syntaxHeading4_ = QColor("#5e81ac");
  syntaxHeading5_ = QColor("#b48ead");
  syntaxHeading6_ = QColor("#bf616a");
  syntaxParagraph_ = QColor("#d8dee9");
  syntaxCode_ = QColor("#a3be8c");
  syntaxCodeBackground_ = QColor("#3b4252");
  syntaxCodeBlockBackground_ = QColor("#2e3440");
  syntaxCodeBlockBorder_ = QColor("#4c566a");
  syntaxBold_ = QColor("#eceff4");
  syntaxItalic_ = QColor("#e5e9f0");
  syntaxStrikethrough_ = QColor("#4c566a");
  syntaxLink_ = QColor("#88c0d0");
  syntaxLinkUrl_ = QColor("#81a1c1");
  syntaxImage_ = QColor("#b48ead");
  syntaxListMarker_ = QColor("#a3be8c");
  syntaxBlockQuote_ = QColor("#ebcb8b");
  syntaxBlockQuoteBorder_ = QColor("#d08770");
  syntaxHorizontalRule_ = QColor("#4c566a");
  syntaxTable_ = QColor("#d8dee9");
  syntaxTableBorder_ = QColor("#4c566a");
  syntaxComment_ = QColor("#616e88");

  codeKeyword_ = QColor("#81a1c1");
  codeKeyword2_ = QColor("#b48ead");
  codeClass_ = QColor("#8fbcbb");
  codeFunction_ = QColor("#88c0d0");
  codeFunctionCall_ = QColor("#88c0d0");
  codeVariable_ = QColor("#d8dee9");
  codeParameter_ = QColor("#d8dee9");
  codeString_ = QColor("#a3be8c");
  codeStringEscape_ = QColor("#ebcb8b");
  codeNumber_ = QColor("#b48ead");
  codeOperator_ = QColor("#81a1c1");
  codePreprocessor_ = QColor("#5e81ac");
  codeMacro_ = QColor("#8fbcbb");
  codeComment_ = QColor("#616e88");
  codeDocComment_ = QColor("#616e88");
  codeType_ = QColor("#8fbcbb");
  codeConstant_ = QColor("#d08770");
  codeAttribute_ = QColor("#8fbcbb");
  codeNamespace_ = QColor("#8fbcbb");
  codeBracket_ = QColor("#d8dee9");
  codeBracketMatch_ = QColor("#434c5e");
  codeRegex_ = QColor("#ebcb8b");

  vimNormal_ = QColor("#81a1c1");
  vimInsert_ = QColor("#a3be8c");
  vimVisual_ = QColor("#b48ead");
  vimCommand_ = QColor("#d08770");
  vimReplace_ = QColor("#bf616a");

  errorColor_ = QColor("#bf616a");
  errorBackground_ = QColor("#3b2228");
  warningColor_ = QColor("#d08770");
  warningBackground_ = QColor("#3b2f28");
  infoColor_ = QColor("#81a1c1");
  infoBackground_ = QColor("#283340");
  hintColor_ = QColor("#8fbcbb");
  successColor_ = QColor("#a3be8c");

  diffAdded_ = QColor("#a3be8c");
  diffRemoved_ = QColor("#bf616a");
  diffModified_ = QColor("#ebcb8b");
  diffAddedBackground_ = QColor("#354336");
  diffRemovedBackground_ = QColor("#3b2228");

  searchMatch_ = QColor("#2e3440");
  searchMatchBackground_ = QColor("#ebcb8b");
  searchCurrentMatch_ = QColor("#2e3440");
  searchCurrentMatchBackground_ = QColor("#d08770");

  fuzzyBackground_ = QColor("#2e3440");
  fuzzyForeground_ = QColor("#d8dee9");
  fuzzyMatch_ = QColor("#ebcb8b");
  fuzzySelected_ = QColor("#eceff4");
  fuzzySelectedBackground_ = QColor("#434c5e");
  fuzzyBorder_ = QColor("#81a1c1");

  previewBackground_ = QColor("#2e3440");
  previewForeground_ = QColor("#d8dee9");
  previewHeading_ = QColor("#8fbcbb");
  previewLink_ = QColor("#88c0d0");
  previewCodeBackground_ = QColor("#3b4252");
  previewBlockquoteBackground_ = QColor("#3b4252");
  previewBlockquoteBorder_ = QColor("#d08770");
}

void Theme::loadSolarizedDarkTheme() {
  name_ = "Solarized Dark";

  editorBackground_ = QColor("#002b36");
  editorForeground_ = QColor("#839496");
  editorCurrentLine_ = QColor("#073642");
  editorSelection_ = QColor("#073642");
  editorSelectionForeground_ = QColor("#93a1a1");
  editorCursor_ = QColor("#839496");
  editorWhitespace_ = QColor("#586e75");
  editorIndentGuide_ = QColor("#073642");

  lineNumberForeground_ = QColor("#586e75");
  lineNumberBackground_ = QColor("#002b36");
  lineNumberActiveForeground_ = QColor("#93a1a1");

  foldingBackground_ = QColor("#002b36");
  foldingMarker_ = QColor("#586e75");
  foldingMarkerHover_ = QColor("#93a1a1");
  foldedLineBackground_ = QColor("#073642");

  tabBackground_ = QColor("#002b36");
  tabForeground_ = QColor("#586e75");
  tabActiveBackground_ = QColor("#073642");
  tabActiveForeground_ = QColor("#93a1a1");
  tabHoverBackground_ = QColor("#073642");
  tabBorder_ = QColor("#002b36");
  tabModifiedIndicator_ = QColor("#b58900");
  tabCloseButton_ = QColor("#586e75");
  tabCloseButtonHover_ = QColor("#dc322f");

  uiBackground_ = QColor("#002b36");
  uiForeground_ = QColor("#839496");
  uiBorder_ = QColor("#073642");
  uiAccent_ = QColor("#268bd2");
  uiAccentHover_ = QColor("#2aa198");
  uiButtonBackground_ = QColor("#073642");
  uiButtonForeground_ = QColor("#93a1a1");
  uiButtonHover_ = QColor("#586e75");
  uiButtonPressed_ = QColor("#002b36");
  uiInputBackground_ = QColor("#073642");
  uiInputForeground_ = QColor("#839496");
  uiInputBorder_ = QColor("#586e75");
  uiInputFocusBorder_ = QColor("#268bd2");

  scrollbarBackground_ = QColor("#002b36");
  scrollbarHandle_ = QColor("#073642");
  scrollbarHandleHover_ = QColor("#586e75");

  treeBackground_ = QColor("#002b36");
  treeForeground_ = QColor("#839496");
  treeSelectedBackground_ = QColor("#073642");
  treeSelectedForeground_ = QColor("#93a1a1");
  treeHoverBackground_ = QColor("#073642");
  treeFolderIcon_ = QColor("#b58900");
  treeFileIcon_ = QColor("#268bd2");

  statusBarBackground_ = QColor("#073642");
  statusBarForeground_ = QColor("#839496");
  statusBarBorder_ = QColor("#073642");

  syntaxHeading1_ = QColor("#cb4b16");
  syntaxHeading2_ = QColor("#859900");
  syntaxHeading3_ = QColor("#268bd2");
  syntaxHeading4_ = QColor("#2aa198");
  syntaxHeading5_ = QColor("#6c71c4");
  syntaxHeading6_ = QColor("#d33682");
  syntaxParagraph_ = QColor("#839496");
  syntaxCode_ = QColor("#859900");
  syntaxCodeBackground_ = QColor("#073642");
  syntaxCodeBlockBackground_ = QColor("#002b36");
  syntaxCodeBlockBorder_ = QColor("#586e75");
  syntaxBold_ = QColor("#93a1a1");
  syntaxItalic_ = QColor("#93a1a1");
  syntaxStrikethrough_ = QColor("#586e75");
  syntaxLink_ = QColor("#268bd2");
  syntaxLinkUrl_ = QColor("#2aa198");
  syntaxImage_ = QColor("#d33682");
  syntaxListMarker_ = QColor("#859900");
  syntaxBlockQuote_ = QColor("#6c71c4");
  syntaxBlockQuoteBorder_ = QColor("#cb4b16");
  syntaxHorizontalRule_ = QColor("#586e75");
  syntaxTable_ = QColor("#839496");
  syntaxTableBorder_ = QColor("#586e75");
  syntaxComment_ = QColor("#586e75");

  codeKeyword_ = QColor("#859900");
  codeKeyword2_ = QColor("#cb4b16");
  codeClass_ = QColor("#b58900");
  codeFunction_ = QColor("#268bd2");
  codeFunctionCall_ = QColor("#268bd2");
  codeVariable_ = QColor("#839496");
  codeParameter_ = QColor("#839496");
  codeString_ = QColor("#2aa198");
  codeStringEscape_ = QColor("#dc322f");
  codeNumber_ = QColor("#d33682");
  codeOperator_ = QColor("#859900");
  codePreprocessor_ = QColor("#cb4b16");
  codeMacro_ = QColor("#cb4b16");
  codeComment_ = QColor("#586e75");
  codeDocComment_ = QColor("#586e75");
  codeType_ = QColor("#b58900");
  codeConstant_ = QColor("#cb4b16");
  codeAttribute_ = QColor("#b58900");
  codeNamespace_ = QColor("#b58900");
  codeBracket_ = QColor("#839496");
  codeBracketMatch_ = QColor("#073642");
  codeRegex_ = QColor("#dc322f");

  vimNormal_ = QColor("#268bd2");
  vimInsert_ = QColor("#859900");
  vimVisual_ = QColor("#6c71c4");
  vimCommand_ = QColor("#cb4b16");
  vimReplace_ = QColor("#dc322f");

  errorColor_ = QColor("#dc322f");
  errorBackground_ = QColor("#3d1a1a");
  warningColor_ = QColor("#b58900");
  warningBackground_ = QColor("#3d2e00");
  infoColor_ = QColor("#268bd2");
  infoBackground_ = QColor("#002b4d");
  hintColor_ = QColor("#2aa198");
  successColor_ = QColor("#859900");

  diffAdded_ = QColor("#859900");
  diffRemoved_ = QColor("#dc322f");
  diffModified_ = QColor("#b58900");
  diffAddedBackground_ = QColor("#002b00");
  diffRemovedBackground_ = QColor("#3d1a1a");

  searchMatch_ = QColor("#002b36");
  searchMatchBackground_ = QColor("#b58900");
  searchCurrentMatch_ = QColor("#002b36");
  searchCurrentMatchBackground_ = QColor("#cb4b16");

  fuzzyBackground_ = QColor("#002b36");
  fuzzyForeground_ = QColor("#839496");
  fuzzyMatch_ = QColor("#b58900");
  fuzzySelected_ = QColor("#93a1a1");
  fuzzySelectedBackground_ = QColor("#073642");
  fuzzyBorder_ = QColor("#268bd2");

  previewBackground_ = QColor("#002b36");
  previewForeground_ = QColor("#839496");
  previewHeading_ = QColor("#cb4b16");
  previewLink_ = QColor("#268bd2");
  previewCodeBackground_ = QColor("#073642");
  previewBlockquoteBackground_ = QColor("#073642");
  previewBlockquoteBorder_ = QColor("#cb4b16");
}

void Theme::loadSolarizedLightTheme() {
  name_ = "Solarized Light";
  loadSolarizedDarkTheme();

  editorBackground_ = QColor("#fdf6e3");
  editorForeground_ = QColor("#657b83");
  editorCurrentLine_ = QColor("#eee8d5");
  editorSelection_ = QColor("#eee8d5");
  editorSelectionForeground_ = QColor("#586e75");

  lineNumberForeground_ = QColor("#93a1a1");
  lineNumberBackground_ = QColor("#fdf6e3");

  tabBackground_ = QColor("#fdf6e3");
  tabActiveBackground_ = QColor("#eee8d5");

  uiBackground_ = QColor("#fdf6e3");
  uiForeground_ = QColor("#657b83");
  uiBorder_ = QColor("#eee8d5");

  treeBackground_ = QColor("#fdf6e3");
  treeForeground_ = QColor("#657b83");
  treeSelectedBackground_ = QColor("#eee8d5");

  statusBarBackground_ = QColor("#eee8d5");
  statusBarForeground_ = QColor("#657b83");

  previewBackground_ = QColor("#fdf6e3");
  previewForeground_ = QColor("#657b83");
  previewCodeBackground_ = QColor("#eee8d5");
  previewBlockquoteBackground_ = QColor("#eee8d5");

  fuzzyBackground_ = QColor("#fdf6e3");
  fuzzyForeground_ = QColor("#657b83");

  fuzzySelectedBackground_ = QColor("#eee8d5");
}
void Theme::loadGruvboxDarkTheme() {
  name_ = "Gruvbox Dark";

  editorBackground_ = QColor("#282828");
  editorForeground_ = QColor("#ebdbb2");
  editorCurrentLine_ = QColor("#3c3836");
  editorSelection_ = QColor("#504945");
  editorSelectionForeground_ = QColor("#ebdbb2");
  editorCursor_ = QColor("#ebdbb2");
  editorWhitespace_ = QColor("#665c54");
  editorIndentGuide_ = QColor("#3c3836");

  lineNumberForeground_ = QColor("#7c6f64");
  lineNumberBackground_ = QColor("#282828");
  lineNumberActiveForeground_ = QColor("#ebdbb2");

  foldingBackground_ = QColor("#282828");
  foldingMarker_ = QColor("#7c6f64");
  foldingMarkerHover_ = QColor("#ebdbb2");
  foldedLineBackground_ = QColor("#3c3836");

  tabBackground_ = QColor("#1d2021");
  tabForeground_ = QColor("#7c6f64");
  tabActiveBackground_ = QColor("#282828");
  tabActiveForeground_ = QColor("#ebdbb2");
  tabHoverBackground_ = QColor("#3c3836");
  tabBorder_ = QColor("#1d2021");
  tabModifiedIndicator_ = QColor("#d79921");
  tabCloseButton_ = QColor("#7c6f64");
  tabCloseButtonHover_ = QColor("#fb4934");

  uiBackground_ = QColor("#282828");
  uiForeground_ = QColor("#ebdbb2");
  uiBorder_ = QColor("#3c3836");
  uiAccent_ = QColor("#83a598");
  uiAccentHover_ = QColor("#8ec07c");
  uiButtonBackground_ = QColor("#504945");
  uiButtonForeground_ = QColor("#ebdbb2");
  uiButtonHover_ = QColor("#665c54");
  uiButtonPressed_ = QColor("#3c3836");
  uiInputBackground_ = QColor("#3c3836");
  uiInputForeground_ = QColor("#ebdbb2");
  uiInputBorder_ = QColor("#665c54");
  uiInputFocusBorder_ = QColor("#83a598");

  scrollbarBackground_ = QColor("#282828");
  scrollbarHandle_ = QColor("#504945");
  scrollbarHandleHover_ = QColor("#665c54");

  treeBackground_ = QColor("#282828");
  treeForeground_ = QColor("#ebdbb2");
  treeSelectedBackground_ = QColor("#504945");
  treeSelectedForeground_ = QColor("#ebdbb2");
  treeHoverBackground_ = QColor("#3c3836");
  treeFolderIcon_ = QColor("#d79921");
  treeFileIcon_ = QColor("#83a598");

  statusBarBackground_ = QColor("#1d2021");
  statusBarForeground_ = QColor("#ebdbb2");
  statusBarBorder_ = QColor("#1d2021");

  syntaxHeading1_ = QColor("#fb4934");
  syntaxHeading2_ = QColor("#b8bb26");
  syntaxHeading3_ = QColor("#83a598");
  syntaxHeading4_ = QColor("#fabd2f");
  syntaxHeading5_ = QColor("#d3869b");
  syntaxHeading6_ = QColor("#fe8019");
  syntaxParagraph_ = QColor("#ebdbb2");
  syntaxCode_ = QColor("#b8bb26");
  syntaxCodeBackground_ = QColor("#3c3836");
  syntaxCodeBlockBackground_ = QColor("#282828");
  syntaxCodeBlockBorder_ = QColor("#504945");
  syntaxBold_ = QColor("#ebdbb2");
  syntaxItalic_ = QColor("#ebdbb2");
  syntaxStrikethrough_ = QColor("#7c6f64");
  syntaxLink_ = QColor("#83a598");
  syntaxLinkUrl_ = QColor("#8ec07c");
  syntaxImage_ = QColor("#d3869b");
  syntaxListMarker_ = QColor("#b8bb26");
  syntaxBlockQuote_ = QColor("#a89984");
  syntaxBlockQuoteBorder_ = QColor("#fe8019");
  syntaxHorizontalRule_ = QColor("#665c54");
  syntaxTable_ = QColor("#ebdbb2");
  syntaxTableBorder_ = QColor("#665c54");
  syntaxComment_ = QColor("#928374");

  codeKeyword_ = QColor("#fb4934");
  codeKeyword2_ = QColor("#fe8019");
  codeClass_ = QColor("#fabd2f");
  codeFunction_ = QColor("#b8bb26");
  codeFunctionCall_ = QColor("#8ec07c");
  codeVariable_ = QColor("#ebdbb2");
  codeParameter_ = QColor("#d3869b");
  codeString_ = QColor("#b8bb26");
  codeStringEscape_ = QColor("#fe8019");
  codeNumber_ = QColor("#d3869b");
  codeOperator_ = QColor("#fb4934");
  codePreprocessor_ = QColor("#fe8019");
  codeMacro_ = QColor("#fabd2f");
  codeComment_ = QColor("#928374");
  codeDocComment_ = QColor("#928374");
  codeType_ = QColor("#fabd2f");
  codeConstant_ = QColor("#d3869b");
  codeAttribute_ = QColor("#8ec07c");
  codeNamespace_ = QColor("#fabd2f");
  codeBracket_ = QColor("#ebdbb2");
  codeBracketMatch_ = QColor("#504945");
  codeRegex_ = QColor("#83a598");

  vimNormal_ = QColor("#83a598");
  vimInsert_ = QColor("#b8bb26");
  vimVisual_ = QColor("#d3869b");
  vimCommand_ = QColor("#fe8019");
  vimReplace_ = QColor("#fb4934");

  errorColor_ = QColor("#fb4934");
  errorBackground_ = QColor("#3c1f1e");
  warningColor_ = QColor("#fabd2f");
  warningBackground_ = QColor("#3c3220");
  infoColor_ = QColor("#83a598");
  infoBackground_ = QColor("#1f2b33");
  hintColor_ = QColor("#8ec07c");
  successColor_ = QColor("#b8bb26");

  diffAdded_ = QColor("#b8bb26");
  diffRemoved_ = QColor("#fb4934");
  diffModified_ = QColor("#fabd2f");
  diffAddedBackground_ = QColor("#2d3b1f");
  diffRemovedBackground_ = QColor("#3c1f1e");

  searchMatch_ = QColor("#282828");
  searchMatchBackground_ = QColor("#fabd2f");
  searchCurrentMatch_ = QColor("#282828");
  searchCurrentMatchBackground_ = QColor("#fe8019");

  fuzzyBackground_ = QColor("#282828");
  fuzzyForeground_ = QColor("#ebdbb2");
  fuzzyMatch_ = QColor("#fabd2f");
  fuzzySelected_ = QColor("#ebdbb2");
  fuzzySelectedBackground_ = QColor("#504945");
  fuzzyBorder_ = QColor("#83a598");

  previewBackground_ = QColor("#282828");
  previewForeground_ = QColor("#ebdbb2");
  previewHeading_ = QColor("#fb4934");
  previewLink_ = QColor("#83a598");
  previewCodeBackground_ = QColor("#3c3836");
  previewBlockquoteBackground_ = QColor("#3c3836");
  previewBlockquoteBorder_ = QColor("#fe8019");
}
void Theme::loadGruvboxLightTheme() {
  name_ = "Gruvbox Light";

  editorBackground_ = QColor("#fbf1c7");
  editorForeground_ = QColor("#3c3836");
  editorCurrentLine_ = QColor("#ebdbb2");
  editorSelection_ = QColor("#d5c4a1");
  editorSelectionForeground_ = QColor("#3c3836");
  editorCursor_ = QColor("#3c3836");
  editorWhitespace_ = QColor("#bdae93");
  editorIndentGuide_ = QColor("#ebdbb2");

  lineNumberForeground_ = QColor("#928374");
  lineNumberBackground_ = QColor("#fbf1c7");
  lineNumberActiveForeground_ = QColor("#3c3836");

  foldingBackground_ = QColor("#fbf1c7");
  foldingMarker_ = QColor("#928374");
  foldingMarkerHover_ = QColor("#3c3836");
  foldedLineBackground_ = QColor("#ebdbb2");

  tabBackground_ = QColor("#f9f5d7");
  tabForeground_ = QColor("#928374");
  tabActiveBackground_ = QColor("#fbf1c7");
  tabActiveForeground_ = QColor("#3c3836");
  tabHoverBackground_ = QColor("#ebdbb2");
  tabBorder_ = QColor("#f9f5d7");
  tabModifiedIndicator_ = QColor("#b57614");
  tabCloseButton_ = QColor("#928374");
  tabCloseButtonHover_ = QColor("#cc241d");

  uiBackground_ = QColor("#fbf1c7");
  uiForeground_ = QColor("#3c3836");
  uiBorder_ = QColor("#ebdbb2");
  uiAccent_ = QColor("#076678");
  uiAccentHover_ = QColor("#427b58");
  uiButtonBackground_ = QColor("#d5c4a1");
  uiButtonForeground_ = QColor("#3c3836");
  uiButtonHover_ = QColor("#bdae93");
  uiButtonPressed_ = QColor("#ebdbb2");
  uiInputBackground_ = QColor("#ebdbb2");
  uiInputForeground_ = QColor("#3c3836");
  uiInputBorder_ = QColor("#bdae93");
  uiInputFocusBorder_ = QColor("#076678");

  scrollbarBackground_ = QColor("#fbf1c7");
  scrollbarHandle_ = QColor("#d5c4a1");
  scrollbarHandleHover_ = QColor("#bdae93");

  treeBackground_ = QColor("#fbf1c7");
  treeForeground_ = QColor("#3c3836");
  treeSelectedBackground_ = QColor("#d5c4a1");
  treeSelectedForeground_ = QColor("#3c3836");
  treeHoverBackground_ = QColor("#ebdbb2");
  treeFolderIcon_ = QColor("#b57614");
  treeFileIcon_ = QColor("#076678");

  statusBarBackground_ = QColor("#f9f5d7");
  statusBarForeground_ = QColor("#3c3836");
  statusBarBorder_ = QColor("#f9f5d7");

  syntaxHeading1_ = QColor("#9d0006");
  syntaxHeading2_ = QColor("#79740e");
  syntaxHeading3_ = QColor("#076678");
  syntaxHeading4_ = QColor("#b57614");
  syntaxHeading5_ = QColor("#8f3f71");
  syntaxHeading6_ = QColor("#af3a03");
  syntaxParagraph_ = QColor("#3c3836");
  syntaxCode_ = QColor("#79740e");
  syntaxCodeBackground_ = QColor("#ebdbb2");
  syntaxCodeBlockBackground_ = QColor("#fbf1c7");
  syntaxCodeBlockBorder_ = QColor("#d5c4a1");
  syntaxBold_ = QColor("#3c3836");
  syntaxItalic_ = QColor("#3c3836");
  syntaxStrikethrough_ = QColor("#928374");
  syntaxLink_ = QColor("#076678");
  syntaxLinkUrl_ = QColor("#427b58");
  syntaxImage_ = QColor("#8f3f71");
  syntaxListMarker_ = QColor("#79740e");
  syntaxBlockQuote_ = QColor("#7c6f64");
  syntaxBlockQuoteBorder_ = QColor("#af3a03");
  syntaxHorizontalRule_ = QColor("#bdae93");
  syntaxTable_ = QColor("#3c3836");
  syntaxTableBorder_ = QColor("#bdae93");
  syntaxComment_ = QColor("#928374");

  codeKeyword_ = QColor("#9d0006");
  codeKeyword2_ = QColor("#af3a03");
  codeClass_ = QColor("#b57614");
  codeFunction_ = QColor("#79740e");
  codeFunctionCall_ = QColor("#427b58");
  codeVariable_ = QColor("#3c3836");
  codeParameter_ = QColor("#8f3f71");
  codeString_ = QColor("#79740e");
  codeStringEscape_ = QColor("#af3a03");
  codeNumber_ = QColor("#8f3f71");
  codeOperator_ = QColor("#9d0006");
  codePreprocessor_ = QColor("#af3a03");
  codeMacro_ = QColor("#b57614");
  codeComment_ = QColor("#928374");
  codeDocComment_ = QColor("#928374");
  codeType_ = QColor("#b57614");
  codeConstant_ = QColor("#8f3f71");
  codeAttribute_ = QColor("#427b58");
  codeNamespace_ = QColor("#b57614");
  codeBracket_ = QColor("#3c3836");
  codeBracketMatch_ = QColor("#d5c4a1");
  codeRegex_ = QColor("#076678");

  vimNormal_ = QColor("#076678");
  vimInsert_ = QColor("#79740e");
  vimVisual_ = QColor("#8f3f71");
  vimCommand_ = QColor("#af3a03");
  vimReplace_ = QColor("#9d0006");

  errorColor_ = QColor("#cc241d");
  errorBackground_ = QColor("#f9d7d9");
  warningColor_ = QColor("#d79921");
  warningBackground_ = QColor("#fae8c0");
  infoColor_ = QColor("#458588");
  infoBackground_ = QColor("#c5e2f0");
  hintColor_ = QColor("#689d6a");
  successColor_ = QColor("#98971a");

  diffAdded_ = QColor("#79740e");
  diffRemoved_ = QColor("#9d0006");
  diffModified_ = QColor("#b57614");
  diffAddedBackground_ = QColor("#d5e5c5");
  diffRemovedBackground_ = QColor("#f9d7d9");

  searchMatch_ = QColor("#3c3836");
  searchMatchBackground_ = QColor("#d79921");
  searchCurrentMatch_ = QColor("#3c3836");
  searchCurrentMatchBackground_ = QColor("#fe8019");

  fuzzyBackground_ = QColor("#fbf1c7");
  fuzzyForeground_ = QColor("#3c3836");
  fuzzyMatch_ = QColor("#b57614");
  fuzzySelected_ = QColor("#3c3836");
  fuzzySelectedBackground_ = QColor("#d5c4a1");
  fuzzyBorder_ = QColor("#076678");

  previewBackground_ = QColor("#fbf1c7");
  previewForeground_ = QColor("#3c3836");
  previewHeading_ = QColor("#9d0006");
  previewLink_ = QColor("#076678");
  previewCodeBackground_ = QColor("#ebdbb2");
  previewBlockquoteBackground_ = QColor("#ebdbb2");
  previewBlockquoteBorder_ = QColor("#af3a03");
}
void Theme::loadOneDarkTheme() {
  name_ = "One Dark";

  editorBackground_ = QColor("#282c34");
  editorForeground_ = QColor("#abb2bf");
  editorCurrentLine_ = QColor("#2c323c");
  editorSelection_ = QColor("#3e4451");
  editorSelectionForeground_ = QColor("#abb2bf");
  editorCursor_ = QColor("#528bff");
  editorWhitespace_ = QColor("#3b4048");
  editorIndentGuide_ = QColor("#3b4048");

  lineNumberForeground_ = QColor("#495162");
  lineNumberBackground_ = QColor("#282c34");
  lineNumberActiveForeground_ = QColor("#abb2bf");

  foldingBackground_ = QColor("#282c34");
  foldingMarker_ = QColor("#495162");
  foldingMarkerHover_ = QColor("#abb2bf");
  foldedLineBackground_ = QColor("#2c323c");

  tabBackground_ = QColor("#21252b");
  tabForeground_ = QColor("#5c6370");
  tabActiveBackground_ = QColor("#282c34");
  tabActiveForeground_ = QColor("#abb2bf");
  tabHoverBackground_ = QColor("#2c323c");
  tabBorder_ = QColor("#181a1f");
  tabModifiedIndicator_ = QColor("#e5c07b");
  tabCloseButton_ = QColor("#5c6370");
  tabCloseButtonHover_ = QColor("#e06c75");

  uiBackground_ = QColor("#21252b");
  uiForeground_ = QColor("#abb2bf");
  uiBorder_ = QColor("#181a1f");
  uiAccent_ = QColor("#61afef");
  uiAccentHover_ = QColor("#56b6c2");
  uiButtonBackground_ = QColor("#3e4451");
  uiButtonForeground_ = QColor("#abb2bf");
  uiButtonHover_ = QColor("#495162");
  uiButtonPressed_ = QColor("#2c323c");
  uiInputBackground_ = QColor("#3e4451");
  uiInputForeground_ = QColor("#abb2bf");
  uiInputBorder_ = QColor("#495162");
  uiInputFocusBorder_ = QColor("#61afef");

  scrollbarBackground_ = QColor("#282c34");
  scrollbarHandle_ = QColor("#3e4451");
  scrollbarHandleHover_ = QColor("#495162");

  treeBackground_ = QColor("#21252b");
  treeForeground_ = QColor("#abb2bf");
  treeSelectedBackground_ = QColor("#2c323c");
  treeSelectedForeground_ = QColor("#abb2bf");
  treeHoverBackground_ = QColor("#2c323c");
  treeFolderIcon_ = QColor("#e5c07b");
  treeFileIcon_ = QColor("#61afef");

  statusBarBackground_ = QColor("#21252b");
  statusBarForeground_ = QColor("#abb2bf");
  statusBarBorder_ = QColor("#181a1f");

  syntaxHeading1_ = QColor("#e06c75");
  syntaxHeading2_ = QColor("#98c379");
  syntaxHeading3_ = QColor("#61afef");
  syntaxHeading4_ = QColor("#e5c07b");
  syntaxHeading5_ = QColor("#c678dd");
  syntaxHeading6_ = QColor("#d19a66");
  syntaxParagraph_ = QColor("#abb2bf");
  syntaxCode_ = QColor("#98c379");
  syntaxCodeBackground_ = QColor("#2c323c");
  syntaxCodeBlockBackground_ = QColor("#282c34");
  syntaxCodeBlockBorder_ = QColor("#3e4451");
  syntaxBold_ = QColor("#e5c07b");
  syntaxItalic_ = QColor("#c678dd");
  syntaxStrikethrough_ = QColor("#5c6370");
  syntaxLink_ = QColor("#61afef");
  syntaxLinkUrl_ = QColor("#56b6c2");
  syntaxImage_ = QColor("#c678dd");
  syntaxListMarker_ = QColor("#98c379");
  syntaxBlockQuote_ = QColor("#5c6370");
  syntaxBlockQuoteBorder_ = QColor("#d19a66");
  syntaxHorizontalRule_ = QColor("#3e4451");
  syntaxTable_ = QColor("#abb2bf");
  syntaxTableBorder_ = QColor("#3e4451");
  syntaxComment_ = QColor("#5c6370");

  codeKeyword_ = QColor("#c678dd");
  codeKeyword2_ = QColor("#e06c75");
  codeClass_ = QColor("#e5c07b");
  codeFunction_ = QColor("#61afef");
  codeFunctionCall_ = QColor("#61afef");
  codeVariable_ = QColor("#e06c75");
  codeParameter_ = QColor("#d19a66");
  codeString_ = QColor("#98c379");
  codeStringEscape_ = QColor("#56b6c2");
  codeNumber_ = QColor("#d19a66");
  codeOperator_ = QColor("#56b6c2");
  codePreprocessor_ = QColor("#c678dd");
  codeMacro_ = QColor("#e5c07b");
  codeComment_ = QColor("#5c6370");
  codeDocComment_ = QColor("#5c6370");
  codeType_ = QColor("#e5c07b");
  codeConstant_ = QColor("#d19a66");
  codeAttribute_ = QColor("#e5c07b");
  codeNamespace_ = QColor("#e5c07b");
  codeBracket_ = QColor("#abb2bf");
  codeBracketMatch_ = QColor("#3e4451");
  codeRegex_ = QColor("#e06c75");

  vimNormal_ = QColor("#61afef");
  vimInsert_ = QColor("#98c379");
  vimVisual_ = QColor("#c678dd");
  vimCommand_ = QColor("#e5c07b");
  vimReplace_ = QColor("#e06c75");

  errorColor_ = QColor("#e06c75");
  errorBackground_ = QColor("#3b2526");
  warningColor_ = QColor("#e5c07b");
  warningBackground_ = QColor("#3b3520");
  infoColor_ = QColor("#61afef");
  infoBackground_ = QColor("#23323e");
  hintColor_ = QColor("#56b6c2");
  successColor_ = QColor("#98c379");

  diffAdded_ = QColor("#98c379");
  diffRemoved_ = QColor("#e06c75");
  diffModified_ = QColor("#e5c07b");
  diffAddedBackground_ = QColor("#2b3b29");
  diffRemovedBackground_ = QColor("#3b2526");

  searchMatch_ = QColor("#282c34");
  searchMatchBackground_ = QColor("#e5c07b");
  searchCurrentMatch_ = QColor("#282c34");
  searchCurrentMatchBackground_ = QColor("#d19a66");

  fuzzyBackground_ = QColor("#282c34");
  fuzzyForeground_ = QColor("#abb2bf");
  fuzzyMatch_ = QColor("#e5c07b");
  fuzzySelected_ = QColor("#abb2bf");
  fuzzySelectedBackground_ = QColor("#3e4451");
  fuzzyBorder_ = QColor("#61afef");

  previewBackground_ = QColor("#282c34");
  previewForeground_ = QColor("#abb2bf");
  previewHeading_ = QColor("#e06c75");
  previewLink_ = QColor("#61afef");
  previewCodeBackground_ = QColor("#2c323c");
  previewBlockquoteBackground_ = QColor("#2c323c");
  previewBlockquoteBorder_ = QColor("#d19a66");
}
void Theme::loadTokyoNightTheme() {
  name_ = "Tokyo Night";

  editorBackground_ = QColor("#1d1f21");
  editorForeground_ = QColor("#c5c8c6");
  editorCurrentLine_ = QColor("#282a2e");
  editorSelection_ = QColor("#373b41");
  editorSelectionForeground_ = QColor("#c5c8c6");
  editorCursor_ = QColor("#c5c8c6");
  editorWhitespace_ = QColor("#4d5157");
  editorIndentGuide_ = QColor("#282a2e");

  lineNumberForeground_ = QColor("#969896");
  lineNumberBackground_ = QColor("#1d1f21");
  lineNumberActiveForeground_ = QColor("#c5c8c6");

  foldingBackground_ = QColor("#1d1f21");
  foldingMarker_ = QColor("#969896");
  foldingMarkerHover_ = QColor("#c5c8c6");
  foldedLineBackground_ = QColor("#282a2e");

  tabBackground_ = QColor("#1d1f21");
  tabForeground_ = QColor("#969896");
  tabActiveBackground_ = QColor("#282a2e");
  tabActiveForeground_ = QColor("#c5c8c6");
  tabHoverBackground_ = QColor("#373b41");
  tabBorder_ = QColor("#1d1f21");
  tabModifiedIndicator_ = QColor("#f0c674");
  tabCloseButton_ = QColor("#969896");
  tabCloseButtonHover_ = QColor("#cc6666");

  uiBackground_ = QColor("#1d1f21");
  uiForeground_ = QColor("#c5c8c6");
  uiBorder_ = QColor("#282a2e");
  uiAccent_ = QColor("#81a2be");
  uiAccentHover_ = QColor("#8abeb7");
  uiButtonBackground_ = QColor("#373b41");
  uiButtonForeground_ = QColor("#c5c8c6");
  uiButtonHover_ = QColor("#4d5157");
  uiButtonPressed_ = QColor("#282a2e");
  uiInputBackground_ = QColor("#282a2e");
  uiInputForeground_ = QColor("#c5c8c6");
  uiInputBorder_ = QColor("#4d5157");
  uiInputFocusBorder_ = QColor("#81a2be");

  scrollbarBackground_ = QColor("#1d1f21");
  scrollbarHandle_ = QColor("#373b41");
  scrollbarHandleHover_ = QColor("#4d5157");

  treeBackground_ = QColor("#1d1f21");
  treeForeground_ = QColor("#c5c8c6");
  treeSelectedBackground_ = QColor("#373b41");
  treeSelectedForeground_ = QColor("#c5c8c6");
  treeHoverBackground_ = QColor("#282a2e");
  treeFolderIcon_ = QColor("#f0c674");
  treeFileIcon_ = QColor("#81a2be");

  statusBarBackground_ = QColor("#282a2e");
  statusBarForeground_ = QColor("#c5c8c6");
  statusBarBorder_ = QColor("#282a2e");

  syntaxHeading1_ = QColor("#cc6666");
  syntaxHeading2_ = QColor("#b5bd68");
  syntaxHeading3_ = QColor("#81a2be");
  syntaxHeading4_ = QColor("#f0c674");
  syntaxHeading5_ = QColor("#b294bb");
  syntaxHeading6_ = QColor("#de935f");
  syntaxParagraph_ = QColor("#c5c8c6");
  syntaxCode_ = QColor("#b5bd68");
  syntaxCodeBackground_ = QColor("#282a2e");
  syntaxCodeBlockBackground_ = QColor("#1d1f21");
  syntaxCodeBlockBorder_ = QColor("#373b41");
  syntaxBold_ = QColor("#f0c674");
  syntaxItalic_ = QColor("#de935f");
  syntaxStrikethrough_ = QColor("#969896");
  syntaxLink_ = QColor("#81a2be");
  syntaxLinkUrl_ = QColor("#8abeb7");
  syntaxImage_ = QColor("#b294bb");
  syntaxListMarker_ = QColor("#b5bd68");
  syntaxBlockQuote_ = QColor("#969896");
  syntaxBlockQuoteBorder_ = QColor("#de935f");
  syntaxHorizontalRule_ = QColor("#4d5157");
  syntaxTable_ = QColor("#c5c8c6");
  syntaxTableBorder_ = QColor("#4d5157");
  syntaxComment_ = QColor("#969896");

  codeKeyword_ = QColor("#b294bb");
  codeKeyword2_ = QColor("#cc6666");
  codeClass_ = QColor("#f0c674");
  codeFunction_ = QColor("#81a2be");
  codeFunctionCall_ = QColor("#81a2be");
  codeVariable_ = QColor("#cc6666");
  codeParameter_ = QColor("#de935f");
  codeString_ = QColor("#b5bd68");
  codeStringEscape_ = QColor("#8abeb7");
  codeNumber_ = QColor("#de935f");
  codeOperator_ = QColor("#8abeb7");
  codePreprocessor_ = QColor("#b294bb");
  codeMacro_ = QColor("#f0c674");
  codeComment_ = QColor("#969896");
  codeDocComment_ = QColor("#969896");
  codeType_ = QColor("#f0c674");
  codeConstant_ = QColor("#de935f");
  codeAttribute_ = QColor("#f0c674");
  codeNamespace_ = QColor("#f0c674");
  codeBracket_ = QColor("#c5c8c6");
  codeBracketMatch_ = QColor("#373b41");
  codeRegex_ = QColor("#cc6666");

  vimNormal_ = QColor("#81a2be");
  vimInsert_ = QColor("#b5bd68");
  vimVisual_ = QColor("#b294bb");
  vimCommand_ = QColor("#de935f");
  vimReplace_ = QColor("#cc6666");

  errorColor_ = QColor("#cc6666");
  errorBackground_ = QColor("#3b2626");
  warningColor_ = QColor("#f0c674");
  warningBackground_ = QColor("#3b3620");
  infoColor_ = QColor("#81a2be");
  infoBackground_ = QColor("#26313b");
  hintColor_ = QColor("#8abeb7");
  successColor_ = QColor("#b5bd68");

  diffAdded_ = QColor("#b5bd68");
  diffRemoved_ = QColor("#cc6666");
  diffModified_ = QColor("#f0c674");
  diffAddedBackground_ = QColor("#2d3b29");
  diffRemovedBackground_ = QColor("#3b2626");

  searchMatch_ = QColor("#1d1f21");
  searchMatchBackground_ = QColor("#f0c674");
  searchCurrentMatch_ = QColor("#1d1f21");
  searchCurrentMatchBackground_ = QColor("#de935f");

  fuzzyBackground_ = QColor("#1d1f21");
  fuzzyForeground_ = QColor("#c5c8c6");
  fuzzyMatch_ = QColor("#f0c674");
  fuzzySelected_ = QColor("#c5c8c6");
  fuzzySelectedBackground_ = QColor("#373b41");
  fuzzyBorder_ = QColor("#81a2be");

  previewBackground_ = QColor("#1d1f21");
  previewForeground_ = QColor("#c5c8c6");
  previewHeading_ = QColor("#cc6666");
  previewLink_ = QColor("#81a2be");
  previewCodeBackground_ = QColor("#282a2e");
  previewBlockquoteBackground_ = QColor("#282a2e");
  previewBlockquoteBorder_ = QColor("#de935f");
}

void Theme::loadCatppuccinTheme() {
  name_ = "Catppuccin";

  editorBackground_ = QColor("#0a0e14");
  editorForeground_ = QColor("#b3b1ad");
  editorCurrentLine_ = QColor("#00010a");
  editorSelection_ = QColor("#253340");
  editorSelectionForeground_ = QColor("#b3b1ad");
  editorCursor_ = QColor("#e6b450");
  editorWhitespace_ = QColor("#2d3640");
  editorIndentGuide_ = QColor("#191e2a");

  lineNumberForeground_ = QColor("#3e4b59");
  lineNumberBackground_ = QColor("#0a0e14");
  lineNumberActiveForeground_ = QColor("#b3b1ad");

  foldingBackground_ = QColor("#0a0e14");
  foldingMarker_ = QColor("#3e4b59");
  foldingMarkerHover_ = QColor("#b3b1ad");
  foldedLineBackground_ = QColor("#00010a");

  tabBackground_ = QColor("#01060e");
  tabForeground_ = QColor("#3e4b59");
  tabActiveBackground_ = QColor("#0a0e14");
  tabActiveForeground_ = QColor("#b3b1ad");
  tabHoverBackground_ = QColor("#00010a");
  tabBorder_ = QColor("#01060e");
  tabModifiedIndicator_ = QColor("#e6b450");
  tabCloseButton_ = QColor("#3e4b59");
  tabCloseButtonHover_ = QColor("#f07178");

  uiBackground_ = QColor("#0a0e14");
  uiForeground_ = QColor("#b3b1ad");
  uiBorder_ = QColor("#00010a");
  uiAccent_ = QColor("#59c2ff");
  uiAccentHover_ = QColor("#95e6cb");
  uiButtonBackground_ = QColor("#253340");
  uiButtonForeground_ = QColor("#b3b1ad");
  uiButtonHover_ = QColor("#2d3640");
  uiButtonPressed_ = QColor("#00010a");
  uiInputBackground_ = QColor("#00010a");
  uiInputForeground_ = QColor("#b3b1ad");
  uiInputBorder_ = QColor("#253340");
  uiInputFocusBorder_ = QColor("#59c2ff");

  scrollbarBackground_ = QColor("#0a0e14");
  scrollbarHandle_ = QColor("#253340");
  scrollbarHandleHover_ = QColor("#2d3640");

  treeBackground_ = QColor("#0a0e14");
  treeForeground_ = QColor("#b3b1ad");
  treeSelectedBackground_ = QColor("#253340");
  treeSelectedForeground_ = QColor("#b3b1ad");
  treeHoverBackground_ = QColor("#00010a");
  treeFolderIcon_ = QColor("#e6b450");
  treeFileIcon_ = QColor("#59c2ff");

  statusBarBackground_ = QColor("#01060e");
  statusBarForeground_ = QColor("#b3b1ad");
  statusBarBorder_ = QColor("#01060e");

  syntaxHeading1_ = QColor("#f07178");
  syntaxHeading2_ = QColor("#c2d94c");
  syntaxHeading3_ = QColor("#59c2ff");
  syntaxHeading4_ = QColor("#e6b450");
  syntaxHeading5_ = QColor("#d2a6ff");
  syntaxHeading6_ = QColor("#ffb454");
  syntaxParagraph_ = QColor("#b3b1ad");
  syntaxCode_ = QColor("#c2d94c");
  syntaxCodeBackground_ = QColor("#00010a");
  syntaxCodeBlockBackground_ = QColor("#0a0e14");
  syntaxCodeBlockBorder_ = QColor("#253340");
  syntaxBold_ = QColor("#e6b450");
  syntaxItalic_ = QColor("#ffb454");
  syntaxStrikethrough_ = QColor("#3e4b59");
  syntaxLink_ = QColor("#59c2ff");
  syntaxLinkUrl_ = QColor("#95e6cb");
  syntaxImage_ = QColor("#d2a6ff");
  syntaxListMarker_ = QColor("#c2d94c");
  syntaxBlockQuote_ = QColor("#626a73");
  syntaxBlockQuoteBorder_ = QColor("#ff8f40");
  syntaxHorizontalRule_ = QColor("#2d3640");
  syntaxTable_ = QColor("#b3b1ad");
  syntaxTableBorder_ = QColor("#2d3640");
  syntaxComment_ = QColor("#626a73");

  codeKeyword_ = QColor("#ff8f40");
  codeKeyword2_ = QColor("#f07178");
  codeClass_ = QColor("#59c2ff");
  codeFunction_ = QColor("#ffb454");
  codeFunctionCall_ = QColor("#ffb454");
  codeVariable_ = QColor("#b3b1ad");
  codeParameter_ = QColor("#d2a6ff");
  codeString_ = QColor("#c2d94c");
  codeStringEscape_ = QColor("#95e6cb");
  codeNumber_ = QColor("#ffee99");
  codeOperator_ = QColor("#f29668");
  codePreprocessor_ = QColor("#d2a6ff");
  codeMacro_ = QColor("#59c2ff");
  codeComment_ = QColor("#626a73");
  codeDocComment_ = QColor("#626a73");
  codeType_ = QColor("#59c2ff");
  codeConstant_ = QColor("#d2a6ff");
  codeAttribute_ = QColor("#ffb454");
  codeNamespace_ = QColor("#59c2ff");
  codeBracket_ = QColor("#b3b1ad");
  codeBracketMatch_ = QColor("#253340");
  codeRegex_ = QColor("#95e6cb");

  vimNormal_ = QColor("#59c2ff");
  vimInsert_ = QColor("#c2d94c");
  vimVisual_ = QColor("#d2a6ff");
  vimCommand_ = QColor("#e6b450");
  vimReplace_ = QColor("#f07178");

  errorColor_ = QColor("#f07178");
  errorBackground_ = QColor("#3b1f23");
  warningColor_ = QColor("#e6b450");
  warningBackground_ = QColor("#3b3120");
  infoColor_ = QColor("#59c2ff");
  infoBackground_ = QColor("#0f2d3b");
  hintColor_ = QColor("#95e6cb");
  successColor_ = QColor("#c2d94c");

  diffAdded_ = QColor("#c2d94c");
  diffRemoved_ = QColor("#f07178");
  diffModified_ = QColor("#e6b450");
  diffAddedBackground_ = QColor("#1f3b20");
  diffRemovedBackground_ = QColor("#3b1f23");

  searchMatch_ = QColor("#0a0e14");
  searchMatchBackground_ = QColor("#e6b450");
  searchCurrentMatch_ = QColor("#0a0e14");
  searchCurrentMatchBackground_ = QColor("#ff8f40");

  fuzzyBackground_ = QColor("#0a0e14");
  fuzzyForeground_ = QColor("#b3b1ad");
  fuzzyMatch_ = QColor("#e6b450");
  fuzzySelected_ = QColor("#b3b1ad");
  fuzzySelectedBackground_ = QColor("#253340");
  fuzzyBorder_ = QColor("#59c2ff");

  previewBackground_ = QColor("#0a0e14");
  previewForeground_ = QColor("#b3b1ad");
  previewHeading_ = QColor("#f07178");
  previewLink_ = QColor("#59c2ff");
  previewCodeBackground_ = QColor("#00010a");
  previewBlockquoteBackground_ = QColor("#00010a");
  previewBlockquoteBorder_ = QColor("#ff8f40");
}
void Theme::loadMaterialDarkTheme() {
  name_ = "Material Dark";

  editorBackground_ = QColor("#263238");
  editorForeground_ = QColor("#eeffff");
  editorCurrentLine_ = QColor("#2c3b41");
  editorSelection_ = QColor("#37474f");
  editorSelectionForeground_ = QColor("#eeffff");
  editorCursor_ = QColor("#ffcc00");
  editorWhitespace_ = QColor("#4f5b62");
  editorIndentGuide_ = QColor("#37474f");

  lineNumberForeground_ = QColor("#546e7a");
  lineNumberBackground_ = QColor("#263238");
  lineNumberActiveForeground_ = QColor("#eeffff");

  foldingBackground_ = QColor("#263238");
  foldingMarker_ = QColor("#546e7a");
  foldingMarkerHover_ = QColor("#eeffff");
  foldedLineBackground_ = QColor("#2c3b41");

  tabBackground_ = QColor("#1e272c");
  tabForeground_ = QColor("#546e7a");
  tabActiveBackground_ = QColor("#263238");
  tabActiveForeground_ = QColor("#eeffff");
  tabHoverBackground_ = QColor("#2c3b41");
  tabBorder_ = QColor("#1e272c");
  tabModifiedIndicator_ = QColor("#ffcb6b");
  tabCloseButton_ = QColor("#546e7a");
  tabCloseButtonHover_ = QColor("#f07178");

  uiBackground_ = QColor("#263238");
  uiForeground_ = QColor("#eeffff");
  uiBorder_ = QColor("#1e272c");
  uiAccent_ = QColor("#80cbc4");
  uiAccentHover_ = QColor("#89ddff");
  uiButtonBackground_ = QColor("#37474f");
  uiButtonForeground_ = QColor("#eeffff");
  uiButtonHover_ = QColor("#4f5b62");
  uiButtonPressed_ = QColor("#2c3b41");
  uiInputBackground_ = QColor("#2c3b41");
  uiInputForeground_ = QColor("#eeffff");
  uiInputBorder_ = QColor("#4f5b62");
  uiInputFocusBorder_ = QColor("#80cbc4");

  scrollbarBackground_ = QColor("#263238");
  scrollbarHandle_ = QColor("#37474f");
  scrollbarHandleHover_ = QColor("#4f5b62");

  treeBackground_ = QColor("#263238");
  treeForeground_ = QColor("#eeffff");
  treeSelectedBackground_ = QColor("#37474f");
  treeSelectedForeground_ = QColor("#eeffff");
  treeHoverBackground_ = QColor("#2c3b41");
  treeFolderIcon_ = QColor("#ffcb6b");
  treeFileIcon_ = QColor("#89ddff");

  statusBarBackground_ = QColor("#1e272c");
  statusBarForeground_ = QColor("#eeffff");
  statusBarBorder_ = QColor("#1e272c");

  syntaxHeading1_ = QColor("#f07178");
  syntaxHeading2_ = QColor("#c3e88d");
  syntaxHeading3_ = QColor("#82aaff");
  syntaxHeading4_ = QColor("#ffcb6b");
  syntaxHeading5_ = QColor("#c792ea");
  syntaxHeading6_ = QColor("#f78c6c");
  syntaxParagraph_ = QColor("#eeffff");
  syntaxCode_ = QColor("#c3e88d");
  syntaxCodeBackground_ = QColor("#2c3b41");
  syntaxCodeBlockBackground_ = QColor("#263238");
  syntaxCodeBlockBorder_ = QColor("#37474f");
  syntaxBold_ = QColor("#ffcb6b");
  syntaxItalic_ = QColor("#f78c6c");
  syntaxStrikethrough_ = QColor("#546e7a");
  syntaxLink_ = QColor("#89ddff");
  syntaxLinkUrl_ = QColor("#80cbc4");
  syntaxImage_ = QColor("#c792ea");
  syntaxListMarker_ = QColor("#c3e88d");
  syntaxBlockQuote_ = QColor("#546e7a");
  syntaxBlockQuoteBorder_ = QColor("#f78c6c");
  syntaxHorizontalRule_ = QColor("#4f5b62");
  syntaxTable_ = QColor("#eeffff");
  syntaxTableBorder_ = QColor("#4f5b62");
  syntaxComment_ = QColor("#546e7a");

  codeKeyword_ = QColor("#c792ea");
  codeKeyword2_ = QColor("#f07178");
  codeClass_ = QColor("#ffcb6b");
  codeFunction_ = QColor("#82aaff");
  codeFunctionCall_ = QColor("#82aaff");
  codeVariable_ = QColor("#eeffff");
  codeParameter_ = QColor("#f78c6c");
  codeString_ = QColor("#c3e88d");
  codeStringEscape_ = QColor("#89ddff");
  codeNumber_ = QColor("#f78c6c");
  codeOperator_ = QColor("#89ddff");
  codePreprocessor_ = QColor("#c792ea");
  codeMacro_ = QColor("#ffcb6b");
  codeComment_ = QColor("#546e7a");
  codeDocComment_ = QColor("#546e7a");
  codeType_ = QColor("#ffcb6b");
  codeConstant_ = QColor("#f78c6c");
  codeAttribute_ = QColor("#ffcb6b");
  codeNamespace_ = QColor("#ffcb6b");
  codeBracket_ = QColor("#eeffff");
  codeBracketMatch_ = QColor("#37474f");
  codeRegex_ = QColor("#89ddff");

  vimNormal_ = QColor("#82aaff");
  vimInsert_ = QColor("#c3e88d");
  vimVisual_ = QColor("#c792ea");
  vimCommand_ = QColor("#ffcb6b");
  vimReplace_ = QColor("#f07178");

  errorColor_ = QColor("#f07178");
  errorBackground_ = QColor("#3b2226");
  warningColor_ = QColor("#ffcb6b");
  warningBackground_ = QColor("#3b3420");
  infoColor_ = QColor("#82aaff");
  infoBackground_ = QColor("#1f2f3b");
  hintColor_ = QColor("#80cbc4");
  successColor_ = QColor("#c3e88d");

  diffAdded_ = QColor("#c3e88d");
  diffRemoved_ = QColor("#f07178");
  diffModified_ = QColor("#ffcb6b");
  diffAddedBackground_ = QColor("#2d3b29");
  diffRemovedBackground_ = QColor("#3b2226");

  searchMatch_ = QColor("#263238");
  searchMatchBackground_ = QColor("#ffcb6b");
  searchCurrentMatch_ = QColor("#263238");
  searchCurrentMatchBackground_ = QColor("#f78c6c");

  fuzzyBackground_ = QColor("#263238");
  fuzzyForeground_ = QColor("#eeffff");
  fuzzyMatch_ = QColor("#ffcb6b");
  fuzzySelected_ = QColor("#eeffff");
  fuzzySelectedBackground_ = QColor("#37474f");
  fuzzyBorder_ = QColor("#80cbc4");

  previewBackground_ = QColor("#263238");
  previewForeground_ = QColor("#eeffff");
  previewHeading_ = QColor("#f07178");
  previewLink_ = QColor("#89ddff");
  previewCodeBackground_ = QColor("#2c3b41");
  previewBlockquoteBackground_ = QColor("#2c3b41");
  previewBlockquoteBorder_ = QColor("#f78c6c");
}

void Theme::loadCyberPunkTheme() {
  name_ = "CyberPunk";

  editorBackground_ = QColor("#0a0a12");
  editorForeground_ = QColor("#00ff9f");
  editorCurrentLine_ = QColor("#1a1a2e");
  editorSelection_ = QColor("#ff00ff40");
  editorSelectionForeground_ = QColor("#ffffff");
  editorCursor_ = QColor("#ff00ff");
  editorWhitespace_ = QColor("#2a2a4e");
  editorIndentGuide_ = QColor("#1a1a3e");

  lineNumberForeground_ = QColor("#00ff9f80");
  lineNumberBackground_ = QColor("#0a0a12");
  lineNumberActiveForeground_ = QColor("#00ff9f");

  foldingBackground_ = QColor("#0a0a12");
  foldingMarker_ = QColor("#ff00ff");
  foldingMarkerHover_ = QColor("#00ffff");
  foldedLineBackground_ = QColor("#1a1a2e");

  tabBackground_ = QColor("#0a0a12");
  tabForeground_ = QColor("#00ff9f80");
  tabActiveBackground_ = QColor("#1a1a2e");
  tabActiveForeground_ = QColor("#00ff9f");
  tabHoverBackground_ = QColor("#2a2a4e");
  tabBorder_ = QColor("#ff00ff40");
  tabModifiedIndicator_ = QColor("#ffff00");
  tabCloseButton_ = QColor("#ff00ff");
  tabCloseButtonHover_ = QColor("#ff0000");

  uiBackground_ = QColor("#0a0a12");
  uiForeground_ = QColor("#00ff9f");
  uiBorder_ = QColor("#ff00ff40");
  uiAccent_ = QColor("#ff00ff");
  uiAccentHover_ = QColor("#00ffff");
  uiButtonBackground_ = QColor("#1a1a2e");
  uiButtonForeground_ = QColor("#00ff9f");
  uiButtonHover_ = QColor("#2a2a4e");
  uiButtonPressed_ = QColor("#0a0a12");
  uiInputBackground_ = QColor("#1a1a2e");
  uiInputForeground_ = QColor("#00ff9f");
  uiInputBorder_ = QColor("#ff00ff40");
  uiInputFocusBorder_ = QColor("#ff00ff");

  scrollbarBackground_ = QColor("#0a0a12");
  scrollbarHandle_ = QColor("#ff00ff40");
  scrollbarHandleHover_ = QColor("#ff00ff");

  treeBackground_ = QColor("#0a0a12");
  treeForeground_ = QColor("#00ff9f");
  treeSelectedBackground_ = QColor("#ff00ff40");
  treeSelectedForeground_ = QColor("#ffffff");
  treeHoverBackground_ = QColor("#1a1a2e");
  treeFolderIcon_ = QColor("#ffff00");
  treeFileIcon_ = QColor("#00ffff");

  statusBarBackground_ = QColor("#0a0a12");
  statusBarForeground_ = QColor("#00ff9f");
  statusBarBorder_ = QColor("#ff00ff40");

  syntaxHeading1_ = QColor("#ff00ff");
  syntaxHeading2_ = QColor("#00ffff");
  syntaxHeading3_ = QColor("#ffff00");
  syntaxHeading4_ = QColor("#ff0080");
  syntaxHeading5_ = QColor("#00ff80");
  syntaxHeading6_ = QColor("#8000ff");
  syntaxParagraph_ = QColor("#00ff9f");
  syntaxCode_ = QColor("#ffff00");
  syntaxCodeBackground_ = QColor("#1a1a2e");
  syntaxCodeBlockBackground_ = QColor("#0a0a12");
  syntaxCodeBlockBorder_ = QColor("#ff00ff40");
  syntaxBold_ = QColor("#ff00ff");
  syntaxItalic_ = QColor("#00ffff");
  syntaxStrikethrough_ = QColor("#808080");
  syntaxLink_ = QColor("#00ffff");
  syntaxLinkUrl_ = QColor("#ff00ff");
  syntaxImage_ = QColor("#ffff00");
  syntaxListMarker_ = QColor("#ff00ff");
  syntaxBlockQuote_ = QColor("#808080");
  syntaxBlockQuoteBorder_ = QColor("#ff00ff");
  syntaxHorizontalRule_ = QColor("#ff00ff40");
  syntaxTable_ = QColor("#00ff9f");
  syntaxTableBorder_ = QColor("#ff00ff40");
  syntaxComment_ = QColor("#606080");

  codeKeyword_ = QColor("#ff00ff");
  codeKeyword2_ = QColor("#00ffff");
  codeClass_ = QColor("#ffff00");
  codeFunction_ = QColor("#00ff9f");
  codeFunctionCall_ = QColor("#00ffff");
  codeVariable_ = QColor("#ff0080");
  codeParameter_ = QColor("#ff8000");
  codeString_ = QColor("#ffff00");
  codeStringEscape_ = QColor("#ff8000");
  codeNumber_ = QColor("#ff0080");
  codeOperator_ = QColor("#00ffff");
  codePreprocessor_ = QColor("#ff00ff");
  codeMacro_ = QColor("#ffff00");
  codeComment_ = QColor("#606080");
  codeDocComment_ = QColor("#808080");
  codeType_ = QColor("#00ffff");
  codeConstant_ = QColor("#ff0080");
  codeAttribute_ = QColor("#ffff00");
  codeNamespace_ = QColor("#ff00ff");
  codeBracket_ = QColor("#00ff9f");
  codeBracketMatch_ = QColor("#ff00ff40");
  codeRegex_ = QColor("#ff8000");

  vimNormal_ = QColor("#00ff9f");
  vimInsert_ = QColor("#00ffff");
  vimVisual_ = QColor("#ff00ff");
  vimCommand_ = QColor("#ffff00");
  vimReplace_ = QColor("#ff0000");

  errorColor_ = QColor("#ff0000");
  errorBackground_ = QColor("#400000");
  warningColor_ = QColor("#ffff00");
  warningBackground_ = QColor("#404000");
  infoColor_ = QColor("#00ffff");
  infoBackground_ = QColor("#004040");
  hintColor_ = QColor("#00ff9f");
  successColor_ = QColor("#00ff00");

  diffAdded_ = QColor("#00ff00");
  diffRemoved_ = QColor("#ff0000");
  diffModified_ = QColor("#ffff00");
  diffAddedBackground_ = QColor("#004000");
  diffRemovedBackground_ = QColor("#400000");

  searchMatch_ = QColor("#0a0a12");
  searchMatchBackground_ = QColor("#ffff00");
  searchCurrentMatch_ = QColor("#0a0a12");
  searchCurrentMatchBackground_ = QColor("#ff00ff");

  fuzzyBackground_ = QColor("#0a0a12");
  fuzzyForeground_ = QColor("#00ff9f");
  fuzzyMatch_ = QColor("#ff00ff");
  fuzzySelected_ = QColor("#ffffff");
  fuzzySelectedBackground_ = QColor("#ff00ff40");
  fuzzyBorder_ = QColor("#ff00ff");

  previewBackground_ = QColor("#0a0a12");
  previewForeground_ = QColor("#00ff9f");
  previewHeading_ = QColor("#ff00ff");
  previewLink_ = QColor("#00ffff");
  previewCodeBackground_ = QColor("#1a1a2e");
  previewBlockquoteBackground_ = QColor("#1a1a2e");
  previewBlockquoteBorder_ = QColor("#ff00ff");
}

void Theme::loadMatrixGreenTheme() {
  name_ = "Matrix Green";

  editorBackground_ = QColor("#000000");
  editorForeground_ = QColor("#00ff00");
  editorCurrentLine_ = QColor("#0a1a0a");
  editorSelection_ = QColor("#003300");
  editorSelectionForeground_ = QColor("#00ff00");
  editorCursor_ = QColor("#00ff00");
  editorWhitespace_ = QColor("#003300");
  editorIndentGuide_ = QColor("#002200");

  lineNumberForeground_ = QColor("#006600");
  lineNumberBackground_ = QColor("#000000");
  lineNumberActiveForeground_ = QColor("#00ff00");

  foldingBackground_ = QColor("#000000");
  foldingMarker_ = QColor("#00aa00");
  foldingMarkerHover_ = QColor("#00ff00");
  foldedLineBackground_ = QColor("#001100");

  tabBackground_ = QColor("#000000");
  tabForeground_ = QColor("#006600");
  tabActiveBackground_ = QColor("#001100");
  tabActiveForeground_ = QColor("#00ff00");
  tabHoverBackground_ = QColor("#002200");
  tabBorder_ = QColor("#003300");
  tabModifiedIndicator_ = QColor("#ffff00");
  tabCloseButton_ = QColor("#006600");
  tabCloseButtonHover_ = QColor("#ff0000");

  uiBackground_ = QColor("#000000");
  uiForeground_ = QColor("#00ff00");
  uiBorder_ = QColor("#003300");
  uiAccent_ = QColor("#00ff00");
  uiAccentHover_ = QColor("#00cc00");
  uiButtonBackground_ = QColor("#001100");
  uiButtonForeground_ = QColor("#00ff00");
  uiButtonHover_ = QColor("#002200");
  uiButtonPressed_ = QColor("#003300");
  uiInputBackground_ = QColor("#001100");
  uiInputForeground_ = QColor("#00ff00");
  uiInputBorder_ = QColor("#003300");
  uiInputFocusBorder_ = QColor("#00ff00");

  scrollbarBackground_ = QColor("#000000");
  scrollbarHandle_ = QColor("#003300");
  scrollbarHandleHover_ = QColor("#00aa00");

  treeBackground_ = QColor("#000000");
  treeForeground_ = QColor("#00ff00");
  treeSelectedBackground_ = QColor("#003300");
  treeSelectedForeground_ = QColor("#00ff00");
  treeHoverBackground_ = QColor("#001100");
  treeFolderIcon_ = QColor("#00cc00");
  treeFileIcon_ = QColor("#00ff00");

  statusBarBackground_ = QColor("#001100");
  statusBarForeground_ = QColor("#00ff00");
  statusBarBorder_ = QColor("#003300");

  syntaxHeading1_ = QColor("#00ff00");
  syntaxHeading2_ = QColor("#00dd00");
  syntaxHeading3_ = QColor("#00bb00");
  syntaxHeading4_ = QColor("#009900");
  syntaxHeading5_ = QColor("#007700");
  syntaxHeading6_ = QColor("#005500");
  syntaxParagraph_ = QColor("#00cc00");
  syntaxCode_ = QColor("#00ff00");
  syntaxCodeBackground_ = QColor("#001100");
  syntaxCodeBlockBackground_ = QColor("#000000");
  syntaxCodeBlockBorder_ = QColor("#003300");
  syntaxBold_ = QColor("#00ff00");
  syntaxItalic_ = QColor("#00dd00");
  syntaxStrikethrough_ = QColor("#005500");
  syntaxLink_ = QColor("#00ffff");
  syntaxLinkUrl_ = QColor("#00aaaa");
  syntaxImage_ = QColor("#00ff00");
  syntaxListMarker_ = QColor("#00ff00");
  syntaxBlockQuote_ = QColor("#006600");
  syntaxBlockQuoteBorder_ = QColor("#00aa00");
  syntaxHorizontalRule_ = QColor("#003300");
  syntaxTable_ = QColor("#00ff00");
  syntaxTableBorder_ = QColor("#003300");
  syntaxComment_ = QColor("#006600");

  codeKeyword_ = QColor("#00ff00");
  codeKeyword2_ = QColor("#00cc00");
  codeClass_ = QColor("#00ff00");
  codeFunction_ = QColor("#00ff00");
  codeFunctionCall_ = QColor("#00dd00");
  codeVariable_ = QColor("#00bb00");
  codeParameter_ = QColor("#00aa00");
  codeString_ = QColor("#88ff88");
  codeStringEscape_ = QColor("#aaffaa");
  codeNumber_ = QColor("#00ffaa");
  codeOperator_ = QColor("#00ff00");
  codePreprocessor_ = QColor("#00dd00");
  codeMacro_ = QColor("#00ff00");
  codeComment_ = QColor("#006600");
  codeDocComment_ = QColor("#008800");
  codeType_ = QColor("#00ff00");
  codeConstant_ = QColor("#00ffaa");
  codeAttribute_ = QColor("#00dd00");
  codeNamespace_ = QColor("#00ff00");
  codeBracket_ = QColor("#00cc00");
  codeBracketMatch_ = QColor("#003300");
  codeRegex_ = QColor("#00ffaa");

  vimNormal_ = QColor("#00ff00");
  vimInsert_ = QColor("#00ffff");
  vimVisual_ = QColor("#ffff00");
  vimCommand_ = QColor("#00ff00");
  vimReplace_ = QColor("#ff0000");

  errorColor_ = QColor("#ff0000");
  errorBackground_ = QColor("#330000");
  warningColor_ = QColor("#ffff00");
  warningBackground_ = QColor("#333300");
  infoColor_ = QColor("#00ffff");
  infoBackground_ = QColor("#003333");
  hintColor_ = QColor("#00ff00");
  successColor_ = QColor("#00ff00");

  diffAdded_ = QColor("#00ff00");
  diffRemoved_ = QColor("#ff0000");
  diffModified_ = QColor("#ffff00");
  diffAddedBackground_ = QColor("#003300");
  diffRemovedBackground_ = QColor("#330000");

  searchMatch_ = QColor("#000000");
  searchMatchBackground_ = QColor("#00ff00");
  searchCurrentMatch_ = QColor("#000000");
  searchCurrentMatchBackground_ = QColor("#ffff00");

  fuzzyBackground_ = QColor("#000000");
  fuzzyForeground_ = QColor("#00ff00");
  fuzzyMatch_ = QColor("#ffff00");
  fuzzySelected_ = QColor("#00ff00");
  fuzzySelectedBackground_ = QColor("#003300");
  fuzzyBorder_ = QColor("#00ff00");

  previewBackground_ = QColor("#000000");
  previewForeground_ = QColor("#00ff00");
  previewHeading_ = QColor("#00ff00");
  previewLink_ = QColor("#00ffff");
  previewCodeBackground_ = QColor("#001100");
  previewBlockquoteBackground_ = QColor("#001100");
  previewBlockquoteBorder_ = QColor("#00aa00");
}
