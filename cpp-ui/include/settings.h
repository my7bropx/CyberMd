#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QFont>
#include <QStringList>

class Settings {
public:
    enum class Theme {
        Dark,
        Light
    };

    Settings();
    ~Settings();

    // Theme settings
    Theme theme() const;
    void setTheme(Theme theme);

    // Editor settings
    QString fontFamily() const;
    void setFontFamily(const QString& family);

    int fontSize() const;
    void setFontSize(int size);

    int tabSize() const;
    void setTabSize(int size);

    // Recent files
    QStringList recentFiles() const;
    void addRecentFile(const QString& filePath);
    void clearRecentFiles();

    // Window settings
    QByteArray windowGeometry() const;
    void setWindowGeometry(const QByteArray& geometry);

    QByteArray windowState() const;
    void setWindowState(const QByteArray& state);

private:
    QSettings settings_;

    static const int MAX_RECENT_FILES = 10;
};

#endif // SETTINGS_H
