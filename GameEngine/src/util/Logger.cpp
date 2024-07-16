#include "GameEngine/util/Logger.h"
#include "GameEngine/util/Singleton.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <fmt/chrono.h>

class ILoggerOutput
{
    Gen::LogLevel minimumLogLevel = Gen::LogLevel::disabled;
protected:
    constexpr ILoggerOutput(Gen::LogLevel level) : minimumLogLevel(level) {}

    constexpr bool isLevelSuitable(Gen::LogLevel level) const noexcept
    {
        return level >= minimumLogLevel;
    }
public:
    virtual void push(Gen::LogLevel level, const std::string_view& text) = 0;
    virtual void flush() = 0;
    virtual ~ILoggerOutput() = default;
};

class FileLoggerOutput : public ILoggerOutput
{
    std::ofstream stream{};
public:
    FileLoggerOutput(const Gen::LogLevel minimumLogLevel, const std::filesystem::path& path, bool truncate, bool required) : ILoggerOutput(minimumLogLevel)
    {
        stream.open(path, truncate ? std::ios_base::out : std::ios_base::app);
        if (required && !stream.is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    void push(Gen::LogLevel level, const std::string_view& text) override
    {
        if (isLevelSuitable(level))
            stream << text << std::endl;
    }

    void flush() override
    {
        stream.flush();
    }

    ~FileLoggerOutput()
    {
        flush();
        stream.close();
    }
};

std::unordered_map<Gen::LogLevel, std::string> g_logLevelToString {
    { Gen::LogLevel::debug, "debug" },
    { Gen::LogLevel::info, "info" },
    { Gen::LogLevel::warning, "warning" },
    { Gen::LogLevel::error, "error" },
    { Gen::LogLevel::critical, "critical" },
    { Gen::LogLevel::disabled, "disabled" }
};

std::unordered_map<Gen::LogLevel, std::string> g_logLevelToColor {
    { Gen::LogLevel::debug, "\033[38;5;8m" },
    { Gen::LogLevel::info, "\033[38;5;7m" },
    { Gen::LogLevel::warning, "\033[38;5;214m" },
    { Gen::LogLevel::error, "\033[38;5;9m" },
    { Gen::LogLevel::critical, "\033[38;5;15;48;5;196m" },
    { Gen::LogLevel::disabled, "\033[0m" }
};

class StderrLoggerOutput : public ILoggerOutput
{
    bool colorizeOutput = false;
public:
    StderrLoggerOutput(const Gen::LogLevel minimumLogLevel, bool colorize = false) : ILoggerOutput(minimumLogLevel), colorizeOutput(colorize)
    {
    }

    void push(Gen::LogLevel level, const std::string_view& text) override
    {
        if (isLevelSuitable(level)) {
            if (colorizeOutput) {
                std::cerr << g_logLevelToColor[level] << text << g_logLevelToColor[Gen::LogLevel::disabled] << std::endl;
            }
            else {
                std::cerr << text << std::endl;
            }
        }
    }

    void flush() override
    {
        std::cerr.flush();
    }

    ~StderrLoggerOutput()
    {
        flush();
    }
};


class OutputTargetsStorage : public Gen::Singleton<OutputTargetsStorage>
{
    std::list<ILoggerOutput*> m_targets{};

public:

    OutputTargetsStorage()
    {
        std::cerr.tie(); std::ios_base::sync_with_stdio(0);
    }

    void addTarget(ILoggerOutput* target)
    {
        m_targets.push_back(target);
    }

    void push(Gen::LogLevel level, const std::string_view message)
    {
        for (auto& target : m_targets) {
            target->push(level, message);
        }
    }


    void flush()
    {
        for (auto& target : m_targets) {
            target->flush();
        }
    }

    ~OutputTargetsStorage()
    {
        for (auto& target : m_targets)
        {
            delete target;
        }
    }
};

void Gen::AddLoggerFileOutput(const LogLevel minimumLogLevel, const std::filesystem::path& path, bool truncate, bool required)
{
    auto* pTarget = new FileLoggerOutput(minimumLogLevel, path, truncate, required);
    OutputTargetsStorage::getInstance()->addTarget(pTarget);
}

void Gen::AddLoggerStderrOutput(const LogLevel minimumLogLevel, bool colorize)
{
    auto* pTarget = new StderrLoggerOutput(minimumLogLevel, colorize);
    OutputTargetsStorage::getInstance()->addTarget(pTarget);
}

void Gen::BasicLoggingSetup()
{
    AddLoggerFileOutput(Gen::LogLevel::info, "engine.log", false, false);
    AddLoggerStderrOutput(Gen::LogLevel::debug, true);
}

void Gen::FlushLoggerOutput()
{
    OutputTargetsStorage::getInstance()->flush();
}

std::hash<std::thread::id> g_threadIdHasher{};

void Gen::Logger::pushMessage(const LogLevel level, const std::string_view message, const std::source_location& location) const
{
    auto now = std::chrono::system_clock::now();
    auto sse = now.time_since_epoch();
    auto threadId = g_threadIdHasher(std::this_thread::get_id());
    auto& levelName = g_logLevelToString[level];
    auto finalMessage =
        fmt::format("[{:%F:%H:%M:}{:%S}] [{:^16}] [{:^8}] [{:^8}] [{}:{}@{}] {}",
            now,
            sse,
            m_name,
            levelName,
            threadId,
            location.file_name(),
            location.line(),
            location.function_name(),
            message);
    OutputTargetsStorage::getInstance()->push(level, finalMessage);
}