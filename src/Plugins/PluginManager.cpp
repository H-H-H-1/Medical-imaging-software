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
        // 榛樿鎻掍欢鐩綍
        pluginDirectories << QCoreApplication::applicationDirPath() + "/plugins"
                         << QCoreApplication::applicationDirPath() + "/extensions";
    }
    
    ~Impl() {
        // 娓呯悊鎵€鏈夊姞杞界殑鎻掍欢
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
        d->lastError = QString("鎻掍欢鏂囦欢涓嶅瓨鍦? %1").arg(filePath);
        return false;
    }

    QString pluginName = fileInfo.baseName();
    
    // 妫€鏌ユ槸鍚﹀凡缁忓姞杞?
    if (d->loadedPlugins.contains(pluginName)) {
        d->lastError = QString("鎻掍欢宸茬粡鍔犺浇: %1").arg(pluginName);
        return false;
    }

    auto loader = new QPluginLoader(filePath, this);
    
    if (!validatePlugin(loader)) {
        delete loader;
        return false;
    }

    // 鍔犺浇鎻掍欢
    QObject* pluginObject = loader->instance();
    if (!pluginObject) {
        d->lastError = QString("鏃犳硶瀹炰緥鍖栨彃浠? %1 - %2")
                      .arg(pluginName)
                      .arg(loader->errorString());
        delete loader;
        return false;
    }

    // 灏濊瘯杞崲涓烘彃浠舵帴鍙?
    auto plugin = qobject_cast<PluginInterface*>(pluginObject);
    if (!plugin) {
        d->lastError = QString("鎻掍欢涓嶅疄鐜癙luginInterface鎺ュ彛: %1").arg(pluginName);
        loader->unload();
        delete loader;
        return false;
    }

    // 鍒濆鍖栨彃浠?
    if (!plugin->initialize()) {
        d->lastError = QString("鎻掍欢鍒濆鍖栧け璐? %1 - %2")
                      .arg(pluginName)
                      .arg(plugin->getLastError());
        loader->unload();
        delete loader;
        return false;
    }

    // 娉ㄥ唽鎻掍欢
    d->loadedPlugins[pluginName] = loader;
    d->pluginInterfaces[pluginName] = plugin;
    
    setupPluginInterface(plugin);
    
    emit pluginLoaded(pluginName);
    
    qDebug() << "鎻掍欢鍔犺浇鎴愬姛:" << pluginName;
    return true;
}

bool PluginManager::loadPluginsFromDirectory(const QString& directory) {
    QDir dir(directory);
    if (!dir.exists()) {
        d->lastError = QString("鎻掍欢鐩綍涓嶅瓨鍦? %1").arg(directory);
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
        QString filePath = dir.absoluteFilePath(fileName);
        if (loadPlugin(filePath)) {
            hasSuccess = true;
        }
    }

    return hasSuccess;
}

void PluginManager::unloadPlugin(const QString& pluginName) {
    if (!d->loadedPlugins.contains(pluginName)) {
        return;
    }

    // 娓呯悊鎻掍欢
    cleanupPlugin(pluginName);
    
    // 鍗歌浇鎻掍欢
    auto loader = d->loadedPlugins[pluginName];
    if (loader) {
        loader->unload();
        delete loader;
    }
    
    d->loadedPlugins.remove(pluginName);
    d->pluginInterfaces.remove(pluginName);
    
    emit pluginUnloaded(pluginName);
    
    qDebug() << "鎻掍欢鍗歌浇:" << pluginName;
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
        if (auto imagePlugin = qobject_cast<ImageProcessingPlugin*>(plugin)) {
            plugins.append(imagePlugin);
        }
    }
    return plugins;
}

QList<VisualizationPlugin*> PluginManager::getVisualizationPlugins() const {
    QList<VisualizationPlugin*> plugins;
    for (auto plugin : d->pluginInterfaces) {
        if (auto vizPlugin = qobject_cast<VisualizationPlugin*>(plugin)) {
            plugins.append(vizPlugin);
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
        return QString();
    }
    
    return QString("鍚嶇О: %1\n鐗堟湰: %2\n浣滆€? %3\n鎻忚堪: %4")
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
        return;
    }
    
    QString filePath = d->loadedPlugins[pluginName]->fileName();
    unloadPlugin(pluginName);
    loadPlugin(filePath);
}

bool PluginManager::validatePlugin(QPluginLoader* loader) const {
    if (!loader) {
        d->lastError = "鏃犳晥鐨勬彃浠跺姞杞藉櫒";
        return false;
    }

    if (!loader->load()) {
        d->lastError = QString("鏃犳硶鍔犺浇鎻掍欢搴? %1").arg(loader->errorString());
        return false;
    }

    return true;
}

void PluginManager::setupPluginInterface(PluginInterface* plugin) {
    if (!plugin) {
        return;
    }
    
    // 杩欓噷鍙互璁剧疆鎻掍欢鐨勪竴浜涢€氱敤灞炴€ф垨鍥炶皟
    // 渚嬪鏃ュ織璁板綍銆侀敊璇鐞嗙瓑
}

void PluginManager::cleanupPlugin(const QString& pluginName) {
    auto plugin = getPlugin(pluginName);
    if (plugin && plugin->isInitialized()) {
        plugin->finalize();
    }
}

} // namespace MedicalImaging

#include "PluginManager.moc"