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
 * @brief 插件管理器类
 * 
 * 负责加载、管理和协调所有插件的生命周期
 */
class PluginManager : public QObject {
    Q_OBJECT

public:
    static PluginManager& instance();
    ~PluginManager();

    // 插件加载和卸载
    bool loadPlugin(const QString& filePath);
    bool loadPluginsFromDirectory(const QString& directory);
    void unloadPlugin(const QString& pluginName);
    void unloadAllPlugins();

    // 插件查询
    QStringList getLoadedPlugins() const;
    QStringList getAvailablePlugins(const QString& directory) const;
    PluginInterface* getPlugin(const QString& name) const;
    
    // 类型化插件访问
    QList<ImageProcessingPlugin*> getImageProcessingPlugins() const;
    QList<VisualizationPlugin*> getVisualizationPlugins() const;
    QList<MeasurementPlugin*> getMeasurementPlugins() const;

    // 插件信息
    QString getPluginInfo(const QString& name) const;
    bool isPluginLoaded(const QString& name) const;
    QString getPluginVersion(const QString& name) const;

    // 插件目录管理
    void addPluginDirectory(const QString& directory);
    void removePluginDirectory(const QString& directory);
    QStringList getPluginDirectories() const;

    // 错误处理
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