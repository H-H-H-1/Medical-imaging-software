#include "Config.h"
#include <QStandardPaths>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>

namespace MedicalImaging {

struct Config::Impl {
    std::unique_ptr<QSettings> settings;
    QMutex mutex;
    
    Impl() {
        // 创建配置文件路径
        QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir().mkpath(configDir);
        QString configFile = configDir + "/MedicalImaging.ini";
        
        settings = std::make_unique<QSettings>(configFile, QSettings::IniFormat);
    }
};

Config::Config() : d(std::make_unique<Impl>()) {
    ensureDefaults();
}

Config::~Config() = default;

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::setValue(const QString& key, const QVariant& value) {
    QMutexLocker locker(&d->mutex);
    d->settings->setValue(key, value);
    d->settings->sync();
}

QVariant Config::getValue(const QString& key, const QVariant& defaultValue) const {
    QMutexLocker locker(&d->mutex);
    return d->settings->value(key, defaultValue);
}

bool Config::contains(const QString& key) const {
    QMutexLocker locker(&d->mutex);
    return d->settings->contains(key);
}

void Config::remove(const QString& key) {
    QMutexLocker locker(&d->mutex);
    d->settings->remove(key);
    d->settings->sync();
}

void Config::clear() {
    QMutexLocker locker(&d->mutex);
    d->settings->clear();
    d->settings->sync();
}

QString Config::getString(const QString& key, const QString& defaultValue) const {
    return getValue(key, defaultValue).toString();
}

int Config::getInt(const QString& key, int defaultValue) const {
    return getValue(key, defaultValue).toInt();
}

double Config::getDouble(const QString& key, double defaultValue) const {
    return getValue(key, defaultValue).toDouble();
}

bool Config::getBool(const QString& key, bool defaultValue) const {
    return getValue(key, defaultValue).toBool();
}

Config::WindowSettings Config::getWindowSettings() const {
    WindowSettings settings;
    settings.width = getInt("window/width", settings.width);
    settings.height = getInt("window/height", settings.height);
    settings.x = getInt("window/x", settings.x);
    settings.y = getInt("window/y", settings.y);
    settings.maximized = getBool("window/maximized", settings.maximized);
    return settings;
}

void Config::setWindowSettings(const WindowSettings& settings) {
    setValue("window/width", settings.width);
    setValue("window/height", settings.height);
    setValue("window/x", settings.x);
    setValue("window/y", settings.y);
    setValue("window/maximized", settings.maximized);
}

Config::RenderingSettings Config::getRenderingSettings() const {
    RenderingSettings settings;
    settings.renderer = getString("rendering/renderer", settings.renderer);
    settings.antialiasing = getBool("rendering/antialiasing", settings.antialiasing);
    settings.maxTextureSize = getInt("rendering/maxTextureSize", settings.maxTextureSize);
    settings.useVTKInteraction = getBool("rendering/useVTKInteraction", settings.useVTKInteraction);
    return settings;
}

void Config::setRenderingSettings(const RenderingSettings& settings) {
    setValue("rendering/renderer", settings.renderer);
    setValue("rendering/antialiasing", settings.antialiasing);
    setValue("rendering/maxTextureSize", settings.maxTextureSize);
    setValue("rendering/useVTKInteraction", settings.useVTKInteraction);
}

Config::ImageProcessingSettings Config::getImageProcessingSettings() const {
    ImageProcessingSettings settings;
    settings.defaultWindowWidth = getInt("imageProcessing/defaultWindowWidth", settings.defaultWindowWidth);
    settings.defaultWindowLevel = getInt("imageProcessing/defaultWindowLevel", settings.defaultWindowLevel);
    settings.defaultColormap = getString("imageProcessing/defaultColormap", settings.defaultColormap);
    settings.enableGPUProcessing = getBool("imageProcessing/enableGPUProcessing", settings.enableGPUProcessing);
    return settings;
}

void Config::setImageProcessingSettings(const ImageProcessingSettings& settings) {
    setValue("imageProcessing/defaultWindowWidth", settings.defaultWindowWidth);
    setValue("imageProcessing/defaultWindowLevel", settings.defaultWindowLevel);
    setValue("imageProcessing/defaultColormap", settings.defaultColormap);
    setValue("imageProcessing/enableGPUProcessing", settings.enableGPUProcessing);
}

bool Config::loadFromFile(const QString& filename) {
    QMutexLocker locker(&d->mutex);
    
    if (!QFile::exists(filename)) {
        return false;
    }
    
    d->settings.reset();
    d->settings = std::make_unique<QSettings>(filename, QSettings::IniFormat);
    return true;
}

bool Config::saveToFile(const QString& filename) const {
    QMutexLocker locker(&d->mutex);
    
    QSettings exportSettings(filename, QSettings::IniFormat);
    
    // 复制所有键值对
    QStringList keys = d->settings->allKeys();
    for (const QString& key : keys) {
        exportSettings.setValue(key, d->settings->value(key));
    }
    
    exportSettings.sync();
    return exportSettings.status() == QSettings::NoError;
}

void Config::loadDefaults() {
    clear();
    ensureDefaults();
}

QStringList Config::getRecentFiles() const {
    return getValue("recentFiles", QStringList()).toStringList();
}

void Config::addRecentFile(const QString& filepath) {
    QStringList recentFiles = getRecentFiles();
    
    // 移除已存在的条目
    recentFiles.removeAll(filepath);
    
    // 添加到开头
    recentFiles.prepend(filepath);
    
    // 限制数量
    while (recentFiles.size() > MAX_RECENT_FILES) {
        recentFiles.removeLast();
    }
    
    setValue("recentFiles", recentFiles);
}

void Config::clearRecentFiles() {
    setValue("recentFiles", QStringList());
}

void Config::ensureDefaults() {
    // 确保默认窗口设置
    if (!contains("window/width")) {
        WindowSettings defaultWindow;
        setWindowSettings(defaultWindow);
    }
    
    // 确保默认渲染设置
    if (!contains("rendering/renderer")) {
        RenderingSettings defaultRendering;
        setRenderingSettings(defaultRendering);
    }
    
    // 确保默认图像处理设置
    if (!contains("imageProcessing/defaultWindowWidth")) {
        ImageProcessingSettings defaultImageProcessing;
        setImageProcessingSettings(defaultImageProcessing);
    }
}

} // namespace MedicalImaging
