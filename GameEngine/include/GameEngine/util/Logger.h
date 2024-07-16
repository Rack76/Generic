#pragma once
#include <string>
#include <string_view>
#include <source_location>
#include <filesystem>
#include <fmt/format.h>

namespace Gen
{
    /**
     * @brief Message log level for filtering
    */
    enum class LogLevel
    {
        debug,
        info,
        warning,
        error,
        critical,
        disabled
    };

    /**
     * @brief Format pattern and source location
    */
    struct FormatString
    {
        /**
         * @brief Format pattern
        */
        std::string_view            format;
        /**
         * @brief Code source location
        */
        const std::source_location& location;

        /**
         * @brief Format string from string_view
        */
        constexpr FormatString(
            std::string_view            format,
            const std::source_location& location = std::source_location::current())
            : format(format), location(location)
        {
        }

        /**
         * @brief Format string from C-string
        */
        constexpr FormatString(
            const char* format,
            const std::source_location& location = std::source_location::current())
            : format(format), location(location)
        {
        }
    };

    /**
     * @brief Logger class
    */
    class Logger
    {
        std::string m_name = "Unnamed";

    protected:
        void pushMessage(const LogLevel level, const std::string_view message, const std::source_location& location) const;

    public:
        /**
         * @brief Create unnamed logger
        */
        Logger() = default;

        /**
         * @brief Create named logger
         * @param name Name of logger
        */
        constexpr Logger(const std::string_view name) : m_name(name) {}

        /**
         * @brief Log debug message
         * @tparam ...Args Formatting arguments types
         * @param format Formatting pattern string and source location
         * @param ...args Formatting arguments
        */
        template <class... Args>
        constexpr void debug(const FormatString format, Args &&...args) const noexcept
        {
            pushMessage(LogLevel::debug, fmt::vformat( format.format, fmt::make_format_args( args... ) ), format.location);
        }

        /**
         * @brief Log info message
         * @tparam ...Args Formatting arguments types
         * @param format Formatting pattern string and source location
         * @param ...args Formatting arguments
        */
        template <class... Args>
        constexpr void info(const FormatString format, Args &&...args) const noexcept
        {
            pushMessage(LogLevel::info, fmt::vformat( format.format, fmt::make_format_args( args... ) ), format.location);
        }

        /**
         * @brief Log warning message
         * @tparam ...Args Formatting arguments types
         * @param format Formatting pattern string and source location
         * @param ...args Formatting arguments
        */
        template <class... Args>
        constexpr void warning(const FormatString format, Args &&...args) const noexcept
        {
            pushMessage(LogLevel::warning, fmt::vformat( format.format, fmt::make_format_args( args... ) ), format.location);
        }

        /**
         * @brief Log error message
         * @tparam ...Args Formatting arguments types
         * @param format Formatting pattern string and source location
         * @param ...args Formatting arguments
        */
        template <class... Args>
        constexpr void error(const FormatString format, Args &&...args) const noexcept
        {
            pushMessage(LogLevel::error, fmt::vformat( format.format, fmt::make_format_args( args... ) ), format.location);
        }

        /**
         * @brief Log critical message
         * @tparam ...Args Formatting arguments types
         * @param format Formatting pattern string and source location
         * @param ...args Formatting arguments
        */
        template <class... Args>
        constexpr void critical(const FormatString format, Args &&...args) const noexcept
        {
            pushMessage(LogLevel::critical, fmt::vformat( format.format, fmt::make_format_args( args... ) ), format.location);
        }
    };

    /**
     * @brief Add file output for loggers
     * @param path Path to log file
     * @param truncate Should truncate file content?
     * @param required Should throw exception when file cannot be opened?
    */
    void AddLoggerFileOutput(const LogLevel minimumLogLevel, const std::filesystem::path& path, bool truncate = false, bool required = false);

    /**
     * @brief Add stderr output for loggers
     * @param colored Should colorize output?
    */
    void AddLoggerStderrOutput(const LogLevel minimumLogLevel, bool colored = false);

    /**
     * @brief Add one file output and one stderr output
    */
    void BasicLoggingSetup();

    /**
     * @brief Flush all logger output targets
    */
    void FlushLoggerOutput();
} // namespace gen