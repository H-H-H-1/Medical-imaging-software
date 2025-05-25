#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVariant>
#include <QSettings>
#include <memory>

namespace MedicalImaging {

/**
 * @brief 配置管理器类
 * 
 * 负责应用程序配置的读取、保存和管理
 */
class Config {
public:
    static Config& getInstance();
    
    // 基本配置操作
    void setValue(const QString& key, const QVariant& value);
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    bool contains(const QString& key) const;
    void remove(const QString& key);
    void clear();
    
    // 类型安全的getter方法
    QString getString(const QString& key, const QString& defaultValue = QString()) const;
    int getInt(const QString& key, int defaultValue = 0) const;
    double getDouble(const QString& key, double defaultValue = 0.0) const;
    bool getBool(const QString& key, bool defaultValue = false) const;
    
    // 应用程序特定配置
    struct WindowSettings {
        int width = 1200;
        int height = 800;
        int x = 100;
        int y = 100;
        bool maximized = false;
    };
    
    struct RenderingSettings {
        QString renderer = "OpenGL";
        bool antialiasing = true;
        int maxTextureSize = 2048;
        bool useVTKInteraction = true;
    };
    
    struct ImageProcessingSettings {
        int defaultWindowWidth = 400;
        int defaultWindowLevel = 40;
        QString defaultColormap = "Gray";
        bool enableGPUProcessing = true;
    };
    
    // 配置组管理
    WindowSettings getWindowSettings() const;
    void setWindowSettings(const WindowSettings& settings);
    
    RenderingSettings getRenderingSettings() const;
    void setRenderingSettings(const RenderingSettings& settings);
    
    ImageProcessingSettings getImageProcessingSettings() const;
    void setImageProcessingSettings(const ImageProcessingSettings& settings);
    
    // 配置文件操作
    bool loadFromFile(const QString& filename);
    bool saveToFile(const QString& filename) const;
    void loadDefaults();
    
    // 最近使用的文件管理
    QStringList getRecentFiles() const;
    void addRecentFile(const QString& filepath);
    void clearRecentFiles();
    static const int MAX_RECENT_FILES = 10;

private:
    Config();
    ~Config();
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    void ensureDefaults();
    
    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace MedicalImaging

#endif // CONFIG_H
