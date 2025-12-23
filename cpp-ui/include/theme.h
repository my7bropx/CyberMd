#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QString>

class Theme {
public:
  enum ThemeType {
    DarkDefault,
    Light,
    Dracula,
    Monokai,
    Nord,
    SolarizedDark,
    SolarizedLight,
    GruvboxDark,
    GruvboxLight,
    OneDark,
    MaterialDark,
    TokyoNight,
    Catppuccin,
    CyberPunk,  // Custom cybersecurity theme
    MatrixGreen // Hacker-style green on black
  };

  // Constructor
  explicit Theme(ThemeType type = DarkDefault);

  // Getters
  QString name() const { return name_; }
  ThemeType type() const { return type_; }
  //========== BASE COLORS ===========
  // Base colors
  QColor m_backgroundColor;
  QColor m_textColor;
  QColor m_borderColor;
  QColor m_accentColor;
  QColor m_accentText;
  QColor m_accentHover;

  // Editor colors
  QColor m_editorBackground;
  QColor m_editorText;
  QColor m_selectionBackground;
  QColor m_selectionText;
  QColor m_inactiveSelection;

  // UI state colors
  QColor m_hoverBackground;
  QColor m_pressedBackground;
  QColor m_disabledBackground;
  QColor m_disabledText;
  QColor m_dimmedText;
  QColor m_placeholderText;

  // Tab colors
  QColor m_tabBackground;
  QColor m_tabActiveBackground;
  QColor m_tabActiveText;
  QColor m_tabInactiveText;

  // Button colors
  QColor m_buttonBackground;
  QColor m_buttonText;

  // Sidebar colors
  QColor m_sidebarBackground;
  QColor m_iconColor;

  // Statusbar colors
  QColor m_statusBarBackground;
  QColor m_statusBarText;

  // Menu colors
  QColor m_menuBarBackground;
  QColor m_menuBackground;

  // Dialog colors
  QColor m_dialogBackground;

  // Input colors
  QColor m_inputBackground;

  // Scrollbar colors
  QColor m_scrollbarBackground;
  QColor m_scrollbarHandle;
  QColor m_scrollbarHover;

  // FuzzyFinder colors
  QColor m_fuzzyBackground;
  QColor m_headerBackground;

  // Preview colors
  QColor m_previewBackground;
  QColor m_previewText;
  QColor m_linkColor;

  // Tooltip colors
  QColor m_tooltipBackground;
  QColor m_tooltipText;

  // Special colors
  QColor m_dangerColor;

  // ========== EDITOR COLORS ==========
  QColor editorBackground() const { return editorBackground_; }
  QColor editorForeground() const { return editorForeground_; }
  QColor editorCurrentLine() const { return editorCurrentLine_; }
  QColor editorSelection() const { return editorSelection_; }
  QColor editorSelectionForeground() const {
    return editorSelectionForeground_;
  }
  QColor editorCursor() const { return editorCursor_; }
  QColor editorWhitespace() const { return editorWhitespace_; }
  QColor editorIndentGuide() const { return editorIndentGuide_; }

  // ========== LINE NUMBERS ==========
  QColor lineNumberForeground() const { return lineNumberForeground_; }
  QColor lineNumberBackground() const { return lineNumberBackground_; }
  QColor lineNumberActiveForeground() const {
    return lineNumberActiveForeground_;
  }

  // ========== FOLDING AREA ==========
  QColor foldingBackground() const { return foldingBackground_; }
  QColor foldingMarker() const { return foldingMarker_; }
  QColor foldingMarkerHover() const { return foldingMarkerHover_; }
  QColor foldedLineBackground() const { return foldedLineBackground_; }

  // ========== TAB BAR ==========
  QColor tabBackground() const { return tabBackground_; }
  QColor tabForeground() const { return tabForeground_; }
  QColor tabActiveBackground() const { return tabActiveBackground_; }
  QColor tabActiveForeground() const { return tabActiveForeground_; }
  QColor tabHoverBackground() const { return tabHoverBackground_; }
  QColor tabBorder() const { return tabBorder_; }
  QColor tabModifiedIndicator() const { return tabModifiedIndicator_; }
  QColor tabCloseButton() const { return tabCloseButton_; }
  QColor tabCloseButtonHover() const { return tabCloseButtonHover_; }

  // ========== UI COLORS ==========
  QColor uiBackground() const { return uiBackground_; }
  QColor uiForeground() const { return uiForeground_; }
  QColor uiBorder() const { return uiBorder_; }
  QColor uiAccent() const { return uiAccent_; }
  QColor uiAccentHover() const { return uiAccentHover_; }
  QColor uiButtonBackground() const { return uiButtonBackground_; }
  QColor uiButtonForeground() const { return uiButtonForeground_; }
  QColor uiButtonHover() const { return uiButtonHover_; }
  QColor uiButtonPressed() const { return uiButtonPressed_; }
  QColor uiInputBackground() const { return uiInputBackground_; }
  QColor uiInputForeground() const { return uiInputForeground_; }
  QColor uiInputBorder() const { return uiInputBorder_; }
  QColor uiInputFocusBorder() const { return uiInputFocusBorder_; }

  // ========== SCROLLBAR ==========
  QColor scrollbarBackground() const { return scrollbarBackground_; }
  QColor scrollbarHandle() const { return scrollbarHandle_; }
  QColor scrollbarHandleHover() const { return scrollbarHandleHover_; }

  // ========== FILE TREE ==========
  QColor treeBackground() const { return treeBackground_; }
  QColor treeForeground() const { return treeForeground_; }
  QColor treeSelectedBackground() const { return treeSelectedBackground_; }
  QColor treeSelectedForeground() const { return treeSelectedForeground_; }
  QColor treeHoverBackground() const { return treeHoverBackground_; }
  QColor treeFolderIcon() const { return treeFolderIcon_; }
  QColor treeFileIcon() const { return treeFileIcon_; }

  // ========== STATUS BAR ==========
  QColor statusBarBackground() const { return statusBarBackground_; }
  QColor statusBarForeground() const { return statusBarForeground_; }
  QColor statusBarBorder() const { return statusBarBorder_; }

  // ========== MARKDOWN SYNTAX ==========
  QColor syntaxHeading1() const { return syntaxHeading1_; }
  QColor syntaxHeading2() const { return syntaxHeading2_; }
  QColor syntaxHeading3() const { return syntaxHeading3_; }
  QColor syntaxHeading4() const { return syntaxHeading4_; }
  QColor syntaxHeading5() const { return syntaxHeading5_; }
  QColor syntaxHeading6() const { return syntaxHeading6_; }
  QColor syntaxParagraph() const { return syntaxParagraph_; }
  QColor syntaxCode() const { return syntaxCode_; }
  QColor syntaxCodeBackground() const { return syntaxCodeBackground_; }
  QColor syntaxCodeBlockBackground() const {
    return syntaxCodeBlockBackground_;
  }
  QColor syntaxCodeBlockBorder() const { return syntaxCodeBlockBorder_; }
  QColor syntaxBold() const { return syntaxBold_; }
  QColor syntaxItalic() const { return syntaxItalic_; }
  QColor syntaxStrikethrough() const { return syntaxStrikethrough_; }
  QColor syntaxLink() const { return syntaxLink_; }
  QColor syntaxLinkUrl() const { return syntaxLinkUrl_; }
  QColor syntaxImage() const { return syntaxImage_; }
  QColor syntaxListMarker() const { return syntaxListMarker_; }
  QColor syntaxBlockQuote() const { return syntaxBlockQuote_; }
  QColor syntaxBlockQuoteBorder() const { return syntaxBlockQuoteBorder_; }
  QColor syntaxHorizontalRule() const { return syntaxHorizontalRule_; }
  QColor syntaxTable() const { return syntaxTable_; }
  QColor syntaxTableBorder() const { return syntaxTableBorder_; }
  QColor syntaxComment() const { return syntaxComment_; }

  // ========== CODE SYNTAX ==========
  QColor codeKeyword() const { return codeKeyword_; }
  QColor codeKeyword2() const { return codeKeyword2_; } // Secondary keywords
  QColor codeClass() const { return codeClass_; }
  QColor codeFunction() const { return codeFunction_; }
  QColor codeFunctionCall() const { return codeFunctionCall_; }
  QColor codeVariable() const { return codeVariable_; }
  QColor codeParameter() const { return codeParameter_; }
  QColor codeString() const { return codeString_; }
  QColor codeStringEscape() const { return codeStringEscape_; }
  QColor codeNumber() const { return codeNumber_; }
  QColor codeOperator() const { return codeOperator_; }
  QColor codePreprocessor() const { return codePreprocessor_; }
  QColor codeMacro() const { return codeMacro_; }
  QColor codeComment() const { return codeComment_; }
  QColor codeDocComment() const { return codeDocComment_; }
  QColor codeType() const { return codeType_; }
  QColor codeConstant() const { return codeConstant_; }
  QColor codeAttribute() const { return codeAttribute_; }
  QColor codeNamespace() const { return codeNamespace_; }
  QColor codeBracket() const { return codeBracket_; }
  QColor codeBracketMatch() const { return codeBracketMatch_; }
  QColor codeRegex() const { return codeRegex_; }

  // ========== VIM MODE ==========
  QColor vimNormal() const { return vimNormal_; }
  QColor vimInsert() const { return vimInsert_; }
  QColor vimVisual() const { return vimVisual_; }
  QColor vimCommand() const { return vimCommand_; }
  QColor vimReplace() const { return vimReplace_; }

  // ========== DIAGNOSTICS ==========
  QColor errorColor() const { return errorColor_; }
  QColor errorBackground() const { return errorBackground_; }
  QColor warningColor() const { return warningColor_; }
  QColor warningBackground() const { return warningBackground_; }
  QColor infoColor() const { return infoColor_; }
  QColor infoBackground() const { return infoBackground_; }
  QColor hintColor() const { return hintColor_; }
  QColor successColor() const { return successColor_; }

  // ========== DIFF COLORS ==========
  QColor diffAdded() const { return diffAdded_; }
  QColor diffRemoved() const { return diffRemoved_; }
  QColor diffModified() const { return diffModified_; }
  QColor diffAddedBackground() const { return diffAddedBackground_; }
  QColor diffRemovedBackground() const { return diffRemovedBackground_; }

  // ========== SEARCH ==========
  QColor searchMatch() const { return searchMatch_; }
  QColor searchMatchBackground() const { return searchMatchBackground_; }
  QColor searchCurrentMatch() const { return searchCurrentMatch_; }
  QColor searchCurrentMatchBackground() const {
    return searchCurrentMatchBackground_;
  }
  //=========GUIDE COLOR ===========
  QColor indentGuideColor() const { return indentGuideColor_; }

  // ========== FUZZY FINDER ==========
  QColor fuzzyBackground() const { return fuzzyBackground_; }
  QColor fuzzyForeground() const { return fuzzyForeground_; }
  QColor fuzzyMatch() const { return fuzzyMatch_; }
  QColor fuzzySelected() const { return fuzzySelected_; }
  QColor fuzzySelectedBackground() const { return fuzzySelectedBackground_; }
  QColor fuzzyBorder() const { return fuzzyBorder_; }

  // ========== PREVIEW (VIEW MODE) ==========
  QColor previewBackground() const { return previewBackground_; }
  QColor previewForeground() const { return previewForeground_; }
  QColor previewHeading() const { return previewHeading_; }
  QColor previewLink() const { return previewLink_; }
  QColor previewCodeBackground() const { return previewCodeBackground_; }
  QColor previewBlockquoteBackground() const {
    return previewBlockquoteBackground_;
  }
  QColor previewBlockquoteBorder() const { return previewBlockquoteBorder_; }

  // ========== STATIC HELPERS ==========
  static QMap<ThemeType, QString> availableThemes();
  static ThemeType themeFromName(const QString &name);

  // ========== STYLESHEETS ==========
  QString getMainWindowStyleSheet() const;
  QString getEditorStyleSheet() const;
  QString getTabWidgetStyleSheet() const;
  QString getTabBarStyleSheet() const;
  QString getButtonStyleSheet() const;
  QString getToolButtonStyleSheet() const;
  QString getSplitterStyleSheet() const;
  QString getStatusBarStyleSheet() const;
  QString getFeaturePanelStyleSheet() const;
  QString getFileTreeStyleSheet() const;
  QString getScrollBarStyleSheet() const;
  QString getMenuStyleSheet() const;
  QString getDialogStyleSheet() const;
  QString getInputStyleSheet() const;
  QString getFuzzyFinderStyleSheet() const;
  QString getPreviewStyleSheet() const;
  QString getTooltipStyleSheet() const;

  // Combined stylesheet for entire application
  QString getFullStyleSheet() const;

private:
  void loadTheme(ThemeType type);
  void loadDarkDefaultTheme();
  void loadLightTheme();
  void loadDraculaTheme();
  void loadMonokaiTheme();
  void loadNordTheme();
  void loadSolarizedDarkTheme();
  void loadSolarizedLightTheme();
  void loadGruvboxDarkTheme();
  void loadGruvboxLightTheme();
  void loadOneDarkTheme();
  void loadMaterialDarkTheme();
  void loadTokyoNightTheme();
  void loadCatppuccinTheme();
  void loadCyberPunkTheme();
  void loadMatrixGreenTheme();
  void loadIndentGuideColor();

  ThemeType type_;
  QString name_;

  // All color members
  QColor editorBackground_;
  QColor editorForeground_;
  QColor editorCurrentLine_;
  QColor editorSelection_;
  QColor editorSelectionForeground_;
  QColor editorCursor_;
  QColor editorWhitespace_;
  QColor editorIndentGuide_;

  QColor lineNumberForeground_;
  QColor lineNumberBackground_;
  QColor lineNumberActiveForeground_;

  QColor foldingBackground_;
  QColor foldingMarker_;
  QColor foldingMarkerHover_;
  QColor foldedLineBackground_;

  QColor tabBackground_;
  QColor tabForeground_;
  QColor tabActiveBackground_;
  QColor tabActiveForeground_;
  QColor tabHoverBackground_;
  QColor tabBorder_;
  QColor tabModifiedIndicator_;
  QColor tabCloseButton_;
  QColor tabCloseButtonHover_;

  QColor uiBackground_;
  QColor uiForeground_;
  QColor uiBorder_;
  QColor uiAccent_;
  QColor uiAccentHover_;
  QColor uiButtonBackground_;
  QColor uiButtonForeground_;
  QColor uiButtonHover_;
  QColor uiButtonPressed_;
  QColor uiInputBackground_;
  QColor uiInputForeground_;
  QColor uiInputBorder_;
  QColor uiInputFocusBorder_;

  QColor scrollbarBackground_;
  QColor scrollbarHandle_;
  QColor scrollbarHandleHover_;

  QColor treeBackground_;
  QColor treeForeground_;
  QColor treeSelectedBackground_;
  QColor treeSelectedForeground_;
  QColor treeHoverBackground_;
  QColor treeFolderIcon_;
  QColor treeFileIcon_;

  QColor statusBarBackground_;
  QColor statusBarForeground_;
  QColor statusBarBorder_;

  QColor syntaxHeading1_;
  QColor syntaxHeading2_;
  QColor syntaxHeading3_;
  QColor syntaxHeading4_;
  QColor syntaxHeading5_;
  QColor syntaxHeading6_;
  QColor syntaxParagraph_;
  QColor syntaxCode_;
  QColor syntaxCodeBackground_;
  QColor syntaxCodeBlockBackground_;
  QColor syntaxCodeBlockBorder_;
  QColor syntaxBold_;
  QColor syntaxItalic_;
  QColor syntaxStrikethrough_;
  QColor syntaxLink_;
  QColor syntaxLinkUrl_;
  QColor syntaxImage_;
  QColor syntaxListMarker_;
  QColor syntaxBlockQuote_;
  QColor syntaxBlockQuoteBorder_;
  QColor syntaxHorizontalRule_;
  QColor syntaxTable_;
  QColor syntaxTableBorder_;
  QColor syntaxComment_;

  QColor codeKeyword_;
  QColor codeKeyword2_;
  QColor codeClass_;
  QColor codeFunction_;
  QColor codeFunctionCall_;
  QColor codeVariable_;
  QColor codeParameter_;
  QColor codeString_;
  QColor codeStringEscape_;
  QColor codeNumber_;
  QColor codeOperator_;
  QColor codePreprocessor_;
  QColor codeMacro_;
  QColor codeComment_;
  QColor codeDocComment_;
  QColor codeType_;
  QColor codeConstant_;
  QColor codeAttribute_;
  QColor codeNamespace_;
  QColor codeBracket_;
  QColor codeBracketMatch_;
  QColor codeRegex_;

  QColor vimNormal_;
  QColor vimInsert_;
  QColor vimVisual_;
  QColor vimCommand_;
  QColor vimReplace_;

  QColor errorColor_;
  QColor errorBackground_;
  QColor warningColor_;
  QColor warningBackground_;
  QColor infoColor_;
  QColor infoBackground_;
  QColor hintColor_;
  QColor successColor_;

  QColor diffAdded_;
  QColor diffRemoved_;
  QColor diffModified_;
  QColor diffAddedBackground_;
  QColor diffRemovedBackground_;

  QColor searchMatch_;
  QColor searchMatchBackground_;
  QColor searchCurrentMatch_;
  QColor searchCurrentMatchBackground_;

  QColor fuzzyBackground_;
  QColor fuzzyForeground_;
  QColor fuzzyMatch_;
  QColor fuzzySelected_;
  QColor fuzzySelectedBackground_;
  QColor fuzzyBorder_;

  QColor previewBackground_;
  QColor previewForeground_;
  QColor previewHeading_;
  QColor previewLink_;
  QColor previewCodeBackground_;
  QColor previewBlockquoteBackground_;
  QColor previewBlockquoteBorder_;
  QColor indentGuideColor_;
};

#endif // THEME_H
