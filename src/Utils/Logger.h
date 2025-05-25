#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <memory>

namespace MedicalImaging {

/**
 * @brief 日志记录器类
 * 
 * 提供统一的日志记录功能，支持不同级别的日志输出
 */
class Logger {
public:
    enum LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

    static Logger& getInstance();
    
    // 日志记录方法
    void debug(const QString& message);
    void info(const QString& message);
    void warning(const QString& message);
    void error(const QString& message);
    void critical(const QString& message);
    
    // 配置方法
    void setLogLevel(LogLevel level);
    void setLogFile(const QString& filename);
    void setConsoleOutput(bool enabled);
    void setFileOutput(bool enabled);
    
    // 静态便捷方法
    static void logDebug(const QString& message);
    static void logInfo(const QString& message);
    static void logWarning(const QString& message);
    static void logError(const QString& message);
    static void logCritical(const QString& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void writeLog(LogLevel level, const QString& message);
    QString formatMessage(LogLevel level, const QString& message);
    QString levelToString(LogLevel level);
    
    struct Impl;
    std::unique_ptr<Impl> d;
};

// 便捷宏定义
#define LOG_DEBUG(msg) MedicalImaging::Logger::logDebug(msg)
#define LOG_INFO(msg) MedicalImaging::Logger::logInfo(msg)
#define LOG_WARNING(msg) MedicalImaging::Logger::logWarning(msg)
#define LOG_ERROR(msg) MedicalImaging::Logger::logError(msg)
#define LOG_CRITICAL(msg) MedicalImaging::Logger::logCritical(msg)

} // namespace MedicalImaging

#endif // LOGGER_H
