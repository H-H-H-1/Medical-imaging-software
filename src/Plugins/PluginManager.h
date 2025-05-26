#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>
#include <memory>

namespace MedicalImaging {

class PluginInterface;
class ImageProcessingPlugin;
class VisualizationPlugin;
class MeasurementPlugin;

/**
 * @brief 鎻掍欢绠＄悊鍣ㄧ被
 * 
 * 璐熻矗鍔犺浇銆佺鐞嗗拰鍗忚皟鎵€鏈夋彃浠剁殑鐢熷懡鍛ㄦ湡
 */
class PluginManager : public QObject {
    Q_OBJECT

public:
    static PluginManager& instance();
    ~PluginManager();

    // 鎻掍欢鍔犺浇鍜屽嵏杞?
    bool loadPlugin(const QString& filePath);
    bool loadPluginsFromDirectory(const QString& directory);
    void unloadPlugin(const QString& pluginName);
    void unloadAllPlugins();

    // 鎻掍欢鏌ヨ
    QStringList getLoadedPlugins() const;
    QStringList getAvailablePlugins(const QString& directory) const;
    PluginInterface* getPlugin(const QString& name) const;
    
    // 绫诲瀷鍖栨彃浠惰闂?
    QList<ImageProcessingPlugin*> getImageProcessingPlugins() const;
    QList<VisualizationPlugin*> getVisualizationPlugins() const;
    QList<MeasurementPlugin*> getMeasurementPlugins() const;

    // 鎻掍欢淇℃伅
    QString getPluginInfo(const QString& name) const;
    bool isPluginLoaded(const QString& name) const;
    QString getPluginVersion(const QString& name) const;

    // 鎻掍欢鐩綍绠＄悊
    void addPluginDirectory(const QString& directory);
    void removePluginDirectory(const QString& directory);
    QStringList getPluginDirectories() const;

    // 閿欒澶勭悊
    QString getLastError() const;
    bool hasError() const;

signals:
    void pluginLoaded(const QString& pluginName);
    void pluginUnloaded(const QString& pluginName);
    void pluginError(const QString& pluginName, const QString& error);

public slots:
    void refreshPlugins();
    void reloadPlugin(const QString& pluginName);

private:
    PluginManager(QObject* parent = nullptr);
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    bool validatePlugin(QPluginLoader* loader) const;
    void setupPluginInterface(PluginInterface* plugin);
    void cleanupPlugin(const QString& pluginName);

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace MedicalImaging

#endif // PLUGINMANAGER_H