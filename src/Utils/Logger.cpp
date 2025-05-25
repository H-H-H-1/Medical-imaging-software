#include "Logger.h"
#include <iostream>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>

namespace MedicalImaging {

struct Logger::Impl {
    LogLevel currentLevel = INFO;
    bool consoleOutput = true;
    bool fileOutput = false;
    QString logFileName;
    QFile logFile;
    QTextStream logStream;
    QMutex mutex;
    
    Impl() = default;
    
    bool openLogFile(const QString& filename) {
        if (logFile.isOpen()) {
            logFile.close();
        }
        
        logFile.setFileName(filename);
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            logStream.setDevice(&logFile);
            return true;
        }
        return false;
    }
};

Logger::Logger() : d(std::make_unique<Impl>()) {
}

Logger::~Logger() = default;

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::debug(const QString& message) {
    writeLog(DEBUG, message);
}

void Logger::info(const QString& message) {
    writeLog(INFO, message);
}

void Logger::warning(const QString& message) {
    writeLog(WARNING, message);
}

void Logger::error(const QString& message) {
    writeLog(ERROR, message);
}

void Logger::critical(const QString& message) {
    writeLog(CRITICAL, message);
}

void Logger::setLogLevel(LogLevel level) {
    QMutexLocker locker(&d->mutex);
    d->currentLevel = level;
}

void Logger::setLogFile(const QString& filename) {
    QMutexLocker locker(&d->mutex);
    d->logFileName = filename;
    if (!filename.isEmpty()) {
        d->fileOutput = d->openLogFile(filename);
    } else {
        d->fileOutput = false;
        if (d->logFile.isOpen()) {
            d->logFile.close();
        }
    }
}

void Logger::setConsoleOutput(bool enabled) {
    QMutexLocker locker(&d->mutex);
    d->consoleOutput = enabled;
}

void Logger::setFileOutput(bool enabled) {
    QMutexLocker locker(&d->mutex);
    if (enabled && !d->logFileName.isEmpty()) {
        d->fileOutput = d->openLogFile(d->logFileName);
    } else {
        d->fileOutput = false;
    }
}

void Logger::writeLog(LogLevel level, const QString& message) {
    if (level < d->currentLevel) {
        return;
    }
    
    QMutexLocker locker(&d->mutex);
    QString formattedMessage = formatMessage(level, message);
    
    // 控制台输出
    if (d->consoleOutput) {
        if (level >= ERROR) {
            std::cerr << formattedMessage.toStdString() << std::endl;
        } else {
            std::cout << formattedMessage.toStdString() << std::endl;
        }
    }
      // 文件输出
    if (d->fileOutput && d->logFile.isOpen()) {
        d->logStream << formattedMessage << endl;
        d->logStream.flush();
    }
}

QString Logger::formatMessage(LogLevel level, const QString& message) {
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString levelStr = levelToString(level);
    
    return QString("[%1] [%2] %3")
           .arg(timestamp)
           .arg(levelStr)
           .arg(message);
}

QString Logger::levelToString(LogLevel level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        case CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// 静态便捷方法
void Logger::logDebug(const QString& message) {
    getInstance().debug(message);
}

void Logger::logInfo(const QString& message) {
    getInstance().info(message);
}

void Logger::logWarning(const QString& message) {
    getInstance().warning(message);
}

void Logger::logError(const QString& message) {
    getInstance().error(message);
}

void Logger::logCritical(const QString& message) {
    getInstance().critical(message);
}

} // namespace MedicalImaging
