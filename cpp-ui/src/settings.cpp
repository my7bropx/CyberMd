#include "settings.h"

Settings::Settings()
    : settings_("CyberMD", "CyberMD")
{
}

Settings::~Settings() {
}

// Theme settings
Settings::Theme Settings::theme() const {
    QString themeStr = settings_.value("theme", "Dark").toString();
    return (themeStr == "Light") ? Theme::Light : Theme::Dark;
}

void Settings::setTheme(Theme theme) {
    settings_.setValue("theme", (theme == Theme::Light) ? "Light" : "Dark");
}

// Editor settings
QString Settings::fontFamily() const {
    return settings_.value("editor/fontFamily", "Consolas").toString();
}

void Settings::setFontFamily(const QString& family) {
    settings_.setValue("editor/fontFamily", family);
}

int Settings::fontSize() const {
    return settings_.value("editor/fontSize", 11).toInt();
}

void Settings::setFontSize(int size) {
    settings_.setValue("editor/fontSize", size);
}

int Settings::tabSize() const {
    return settings_.value("editor/tabSize", 4).toInt();
}

void Settings::setTabSize(int size) {
    settings_.setValue("editor/tabSize", size);
}

// Recent files
QStringList Settings::recentFiles() const {
    return settings_.value("recentFiles").toStringList();
}

void Settings::addRecentFile(const QString& filePath) {
    QStringList files = recentFiles();

    // Remove if already exists
    files.removeAll(filePath);

    // Add to front
    files.prepend(filePath);

    // Keep only MAX_RECENT_FILES
    while (files.size() > MAX_RECENT_FILES) {
        files.removeLast();
    }

    settings_.setValue("recentFiles", files);
}

void Settings::clearRecentFiles() {
    settings_.setValue("recentFiles", QStringList());
}

// Window settings
QByteArray Settings::windowGeometry() const {
    return settings_.value("window/geometry").toByteArray();
}

void Settings::setWindowGeometry(const QByteArray& geometry) {
    settings_.setValue("window/geometry", geometry);
}

QByteArray Settings::windowState() const {
    return settings_.value("window/state").toByteArray();
}

void Settings::setWindowState(const QByteArray& state) {
    settings_.setValue("window/state", state);
}
