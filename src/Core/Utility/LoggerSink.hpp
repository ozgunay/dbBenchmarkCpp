/* LoggerSink.hpp */

#ifndef _DBBENCHMARK_LOGGER_SINK_HPP_
#define _DBBENCHMARK_LOGGER_SINK_HPP_

#include <syslog.h>
#include <iostream>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/logmessage.hpp>
#include <g3log/loglevels.hpp>

/**
 * Custom log level
 * Values with a +/-1 than their closest equivalent or they could have the same
 * value as well.
 */

const LEVELS ERROR {WARNING.value + 1, {"ERROR"}};

namespace dbbenchmark {
namespace utility {
namespace logger {

/**
 * @brief This is a sink class for cinar G3logger.
 * @details This class will print log messages with color in xterm
 * @author Ozgun AY
 * @version 1.2.0
 * @date 06/03/2019
 * @bug There is no bug so far.
 */
struct ColorCoutSink
{
    /// Linux xterm color
    // http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    enum FG_Color {YELLOW = 33, RED = 31, GREEN=32, WHITE = 37};

    FG_Color GetColor(const LEVELS level) const {
        if (level.value == WARNING.value) { return YELLOW; }
        if (level.value == DEBUG.value) { return GREEN; }
        if (g3::internal::wasFatal(level)) { return RED; }

        return WHITE;
    }

    void ReceiveLogMessage(g3::LogMessageMover logEntry) {
        auto level = logEntry.get()._level;
        auto color = GetColor(level);

        std::cout << "\033[" << color << "m" 
            << logEntry.get().toString() << "\033[m" << std::endl;
    }

    static std::string formatToString(const g3::LogMessage& msg) {
        std::string out;
        out.append(msg.timestamp() + "    "
                    + msg.level()
                    + " ["
                    + msg.threadID()
                    + " "
                    + msg.file()
                    + "->"+ msg.function()
                    + ":" + msg.line() + "]    ");
        return out;
    }
};


/**
 * @brief This is a wrapper class for cinar G3logger.
 * @details This class create a one logger object that log to a file, console
 *   and also syslog.
 * @author Ozgun AY
 * @version 1.2.0
 * @date 06/03/2019
 * @pre The logger object instance must be initialized to "0"
 * @bug There is no bug so far.
 * @warning The looger will flush for every log input
 */
class Initializer final
{
public:
    Initializer(const Initializer&) =delete;
    Initializer& operator=(const Initializer&) =delete;
    Initializer(Initializer&&) =delete;
    Initializer& operator=(Initializer&&) =delete;

private:
    Initializer() =default;
    ~Initializer() =default;

    std::string directory;
    std::string fileName;
    std::string defaultId;
    std::string logFileNamewithPath;
    std::unique_ptr<g3::FileSinkHandle> handle;
    /**
     * Constructor
     * @param fileName Logger filename default is main program name.
     * @param directory Directory where logger filename will be saved. Default is "./logs".
     * @param defaultId ID for log entries.  Default is "".
     * @param writeToCerr Flag to echo console with cerr.
     */
    Initializer(const std::string fileName, const std::string directory ="./logs", const std::string defaultId ="",
        const bool writeToCerr = false) 
    {
        static auto worker = g3::LogWorker::createLogWorker();  // https://github.com/KjellKod/g3log/issues/35
        this->directory = directory;
        this->defaultId = defaultId;
        this->fileName = fileName;

        handle = worker->addDefaultLogger(this->fileName, this->directory, this->defaultId);
        auto changeFormatting = handle->call(&g3::FileSink::overrideLogDetails, ColorCoutSink::formatToString);
        const std::string newHeader = "\t\tLOG format: [YYYY/MM/DD hh:mm:ss uuu* LEVEL THREAD_ID FILE->FUNCTION:LINE] message\n\t\t(uuu*: microseconds fractions of the seconds value)\n\n";
        auto changeHeader = handle->call(&g3::FileSink::overrideLogHeader, newHeader);
        changeFormatting.wait();
        changeHeader.wait();
        auto logFileNamefuture = handle->call(&g3::FileSink::fileName);

        auto sinkHandle = worker->addSink(std::make_unique<ColorCoutSink>(), &ColorCoutSink::ReceiveLogMessage);
        this->logFileNamewithPath = logFileNamefuture.get();

        g3::initializeLogging(worker.get());
        g3::only_change_at_initialization::addLogLevel(ERROR, true);
    }

public:
    /**
     * Singleton instance
     * @param fileName Logger filename default is main program name.
     * @param directory Directory where logger filename will be saved. Default is "./logs".
     * @param defaultId ID for log entries. Default is "".
     * @param writeToCerr Flag to echo console with cerr.
     */
    static Initializer& instance(const std::string fileName, const std::string directory ="./logs",
        const std::string defaultId ="", const bool writeToCerr = false)
    {
        static Initializer sInstance(fileName, directory, defaultId, writeToCerr);
        return sInstance;
    }

    /**
     * Get the current file name of log file.
     * @return The current filename
     */
    const std::string getLogFileName() {
    	auto logFileNamefuture = handle->call(&g3::FileSink::fileName);
    	this->logFileNamewithPath = logFileNamefuture.get();
        return this->logFileNamewithPath;
    }

    void startNewLogFile() {
        this->handle->call(&g3::FileSink::changeLogFile, this->directory, "");
    }
};

} // namesapce logger
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_LOGGER_SINK_HPP_
