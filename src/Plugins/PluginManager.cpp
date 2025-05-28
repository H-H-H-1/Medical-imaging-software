#include "PluginManager.h"
#include "PluginInterface.h"
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include <QLibrary>

namespace MedicalImaging {

struct PluginManager::Impl {
    QMap<QString, QPluginLoader*> loadedPlugins;
    QMap<QString, PluginInterface*> pluginInterfaces;
    QStringList pluginDirectories;
    QString lastError;
    
    Impl() {
        // 默认插件目录
        pluginDirectories << QCoreApplication::applicationDirPath() + "/plugins"
                         << QCoreApplication::applicationDirPath() + "/extensions";
    }
    
    ~Impl() {
        // 清理所有加载的插件
        for (auto loader : loadedPlugins) {
            if (loader) {
                loader->unload();
                delete loader;
            }
        }
    }
};

PluginManager& PluginManager::instance() {
    static PluginManager instance;
    return instance;
}

PluginManager::PluginManager(QObject* parent) 
    : QObject(parent), d(std::make_unique<Impl>()) {
}

PluginManager::~PluginManager() = default;

bool PluginManager::loadPlugin(const QString& filePath) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        d->lastError = QString("插件文件不存在: %1").arg(filePath);
        return false;
    }

    QString pluginName = fileInfo.baseName();
    
    // 检查是否已经加载
    if (d->loadedPlugins.contains(pluginName)) {
        d->lastError = QString("插件已经加载: %1").arg(pluginName);
        return false;
    }

    auto loader = new QPluginLoader(filePath, this);
    
    if (!validatePlugin(loader)) {
        delete loader;
        return false;
    }

    // 加载插件
    QObject* pluginObject = loader->instance();
    if (!pluginObject) {
        d->lastError = QString("无法实例化插件: %1 - %2")
                      .arg(pluginName)
                      .arg(loader->errorString());
        delete loader;
        return false;
    }

    // 尝试转换为插件接口
    auto plugin = qobject_cast<PluginInterface*>(pluginObject);
    if (!plugin) {
        d->lastError = QString("插件不实现PluginInterface接口: %1").arg(pluginName);
        loader->unload();
        delete loader;
        return false;
    }

    // 初始化插件
    if (!plugin->initialize()) {
        d->lastError = QString("插件初始化失败: %1 - %2")
                      .arg(pluginName)
                      .arg(plugin->getLastError());
        loader->unload();
        delete loader;
        return false;
    }

    // 注册插件
    d->loadedPlugins[pluginName] = loader;
    d->pluginInterfaces[pluginName] = plugin;
    
    setupPluginInterface(plugin);
    
    emit pluginLoaded(pluginName);
    
    qDebug() << "插件加载成功:" << pluginName;
    return true;
}

bool PluginManager::loadPluginsFromDirectory(const QString& directory) {
    QDir dir(directory);
    if (!dir.exists()) {
        d->lastError = QString("插件目录不存在: %1").arg(directory);
        return false;
    }

    QStringList filters;
#ifdef Q_OS_WIN
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    filters << "*.dylib";
#else
    filters << "*.so";
#endif

    dir.setNameFilters(filters);
    QStringList pluginFiles = dir.entryList(QDir::Files);
    
    bool hasSuccess = false;
    for (const QString& fileName : pluginFiles) {
        QString fullPath = dir.absoluteFilePath(fileName);
        if (loadPlugin(fullPath)) {
            hasSuccess = true;
        }
    }

    return hasSuccess;
}

void PluginManager::unloadPlugin(const QString& pluginName) {
    if (!d->loadedPlugins.contains(pluginName)) {
        d->lastError = QString("插件未加载: %1").arg(pluginName);
        return;
    }

    // 清理插件
    cleanupPlugin(pluginName);
    
    // 卸载插件
    auto loader = d->loadedPlugins[pluginName];
    if (loader) {
        loader->unload();
        delete loader;
    }
    
    d->loadedPlugins.remove(pluginName);
    d->pluginInterfaces.remove(pluginName);
    
    emit pluginUnloaded(pluginName);
    
    qDebug() << "插件卸载:" << pluginName;
}

void PluginManager::unloadAllPlugins() {
    QStringList pluginNames = d->loadedPlugins.keys();
    for (const QString& name : pluginNames) {
        unloadPlugin(name);
    }
}

QStringList PluginManager::getLoadedPlugins() const {
    return d->loadedPlugins.keys();
}

QStringList PluginManager::getAvailablePlugins(const QString& directory) const {
    QDir dir(directory);
    if (!dir.exists()) {
        return QStringList();
    }

    QStringList filters;
#ifdef Q_OS_WIN
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    filters << "*.dylib";
#else
    filters << "*.so";
#endif

    dir.setNameFilters(filters);
    return dir.entryList(QDir::Files);
}

PluginInterface* PluginManager::getPlugin(const QString& name) const {
    return d->pluginInterfaces.value(name, nullptr);
}

QList<ImageProcessingPlugin*> PluginManager::getImageProcessingPlugins() const {
    QList<ImageProcessingPlugin*> plugins;
    for (auto plugin : d->pluginInterfaces) {
        if (auto imgPlugin = qobject_cast<ImageProcessingPlugin*>(plugin)) {
            plugins.append(imgPlugin);
        }
    }
    return plugins;
}

QList<VisualizationPlugin*> PluginManager::getVisualizationPlugins() const {
    QList<VisualizationPlugin*> plugins;
    for (auto plugin : d->pluginInterfaces) {
        if (auto visPlugin = qobject_cast<VisualizationPlugin*>(plugin)) {
            plugins.append(visPlugin);
        }
    }
    return plugins;
}

QList<MeasurementPlugin*> PluginManager::getMeasurementPlugins() const {
    QList<MeasurementPlugin*> plugins;
    for (auto plugin : d->pluginInterfaces) {
        if (auto measurePlugin = qobject_cast<MeasurementPlugin*>(plugin)) {
            plugins.append(measurePlugin);
        }
    }
    return plugins;
}

QString PluginManager::getPluginInfo(const QString& name) const {
    auto plugin = getPlugin(name);
    if (!plugin) {
        return QString("插件未找到: %1").arg(name);
    }
    
    return QString("名称: %1\n版本: %2\n作者: %3\n描述: %4")
           .arg(plugin->getName())
           .arg(plugin->getVersion())
           .arg(plugin->getAuthor())
           .arg(plugin->getDescription());
}

bool PluginManager::isPluginLoaded(const QString& name) const {
    return d->loadedPlugins.contains(name);
}

QString PluginManager::getPluginVersion(const QString& name) const {
    auto plugin = getPlugin(name);
    return plugin ? plugin->getVersion() : QString();
}

void PluginManager::addPluginDirectory(const QString& directory) {
    if (!d->pluginDirectories.contains(directory)) {
        d->pluginDirectories.append(directory);
    }
}

void PluginManager::removePluginDirectory(const QString& directory) {
    d->pluginDirectories.removeAll(directory);
}

QStringList PluginManager::getPluginDirectories() const {
    return d->pluginDirectories;
}

QString PluginManager::getLastError() const {
    return d->lastError;
}

bool PluginManager::hasError() const {
    return !d->lastError.isEmpty();
}

void PluginManager::refreshPlugins() {
    for (const QString& directory : d->pluginDirectories) {
        loadPluginsFromDirectory(directory);
    }
}

void PluginManager::reloadPlugin(const QString& pluginName) {
    if (!d->loadedPlugins.contains(pluginName)) {
        d->lastError = QString("插件未加载，无法重载: %1").arg(pluginName);
        return;
    }
    
    QString filePath = d->loadedPlugins[pluginName]->fileName();
    unloadPlugin(pluginName);
    loadPlugin(filePath);
}

bool PluginManager::validatePlugin(QPluginLoader* loader) const {
    if (!loader) {
        d->lastError = "无效的插件加载器";
        return false;
    }

    if (!loader->load()) {
        d->lastError = QString("无法加载插件库: %1").arg(loader->errorString());
        return false;
    }

    return true;
}

void PluginManager::setupPluginInterface(PluginInterface* plugin) {
    if (!plugin) {
        return;
    }
    
    // 这里可以设置插件的一些通用属性或回调
    // 例如日志记录、错误处理等
}

void PluginManager::cleanupPlugin(const QString& pluginName) {
    auto plugin = getPlugin(pluginName);
    if (plugin && plugin->isInitialized()) {
        plugin->finalize();
    }
}

} // namespace MedicalImaging

#include "PluginManager.moc"
