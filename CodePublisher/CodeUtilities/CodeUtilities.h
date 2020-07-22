#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <regex>

namespace Utilities
{
    inline void preface(const std::string& msg = "", bool doReturn = true, std::ostream& out = std::cout, const std::string& prefix = "  ")
    {
        if (doReturn) out << "\n" << prefix << msg;
    }

    inline std::string defaultUsage()
    {
        std::string usage;
        usage += "\n  Command Line: path [/option]* [/pattern]* [integer]";
        usage += "\n    path is relative or absolute path where processing begins";
        usage += "\n    [/option]* are one or more options of the form /s, /r, etc.";
        usage += "\n    [pattern]* are one or more pattern strings used for matching";
        usage += "\n    [integer] is the maximum number of items to process";
        usage += "\n";
        return usage;
    }

    class ProcessCmdLine
    {
    public:
        using Usage = std::string;
        using Path = std::string;
        using Option = std::string;
        using Options = std::vector<Option>;
        using Pattern = std::string;
        using Patterns = std::vector<Pattern>;
        using Regexes = std::vector<std::string>;
        using Number = long int;

        ProcessCmdLine(int argc, char** argv, std::ostream& out = std::cout);
        ProcessCmdLine(const ProcessCmdLine&) = delete;
        ProcessCmdLine& operator=(const ProcessCmdLine&) = delete;

        bool parseError();
        Path path();
        void path(const Path& path);
        Options options();
        void option(Option op);
        bool hasOption(Option op);
        Patterns patterns();
        Regexes regexes();
        void pattern(const Pattern& patt);
        Number maxItems();
        void maxItems(Number number);
        void usage(const Usage& usage);
        void usage();
        void showCmdLine(int argc, char** argv);
        void showCmdLine();
        void showPath();
        void showOptions();
        void showPatterns();
        void showRegexes();
        void showMaxItems();

    private:
        bool isValidRegex(const std::string& regex);
    private:
        Usage usage_;
        Path path_;
        Patterns patterns_;
        Regexes regexes_;
        Options options_;
        int maxItems_ = 0;
        bool parseError_ = false;
        std::ostream& out_;
    };

    inline void ProcessCmdLine::path(const Path& path)
    {
        path_ = path;
    }

    inline ProcessCmdLine::Path ProcessCmdLine::path()
    {
        return path_;
    }

    inline void ProcessCmdLine::showPath()
    {
        out_ << path_ << " ";
    }

    inline void ProcessCmdLine::option(Option option)
    {
        options_.push_back(option);
    }

    inline ProcessCmdLine::Options ProcessCmdLine::options()
    {
        return options_;
    }

    inline bool ProcessCmdLine::hasOption(Option op)
    {
        for (auto item : options_)
        {
            if (item == op)
            {
                return true;
            }
        }
        return false;
    }

    inline void ProcessCmdLine::showOptions()
    {
        for (auto opt : options_)
        {
            out_ << '/' << opt << " ";
        }
    }

    inline void ProcessCmdLine::pattern(const Pattern& pattern)
    {
        patterns_.push_back(pattern);
    }

    inline ProcessCmdLine::Patterns ProcessCmdLine::patterns()
    {
        return patterns_;
    }

    inline ProcessCmdLine::Regexes ProcessCmdLine::regexes()
    {
        return regexes_;
    }

    inline void ProcessCmdLine::showPatterns()
    {
        for (auto patt : patterns_)
        {
            out_ << patt << " ";
        }
    }

    inline void ProcessCmdLine::showRegexes()
    {
        for (auto regex : regexes_)
        {
            out_ << regex << " ";
        }
    }

    inline void ProcessCmdLine::maxItems(Number maxItems)
    {
        maxItems_ = maxItems;
    }

    inline ProcessCmdLine::Number ProcessCmdLine::maxItems()
    {
        return maxItems_;
    }

    inline void ProcessCmdLine::showMaxItems()
    {
        if (maxItems_ != 0)
            out_ << maxItems_ << " ";
    }

    inline bool ProcessCmdLine::isValidRegex(const std::string& regex)
    {
        bool res = true;
        try
        {
            std::regex tmp(regex);
        }
        catch (const std::regex_error& e)
        {
            (e);
            res = false;
        }
        return res;
    }

    inline bool ProcessCmdLine::parseError()
    {
        return parseError_;
    }

    inline ProcessCmdLine::ProcessCmdLine(int argc, char** argv, std::ostream& out) : out_(out)
    {
        if (argc < 2)
        {
            out << "\n  command line parse error";
            parseError_ = true;
            return;
        }

        path_ = argv[1];

        for (int i = 2; i < argc; ++i)
        {
            if (argv[i][0] == '/')
            {
                std::string opt(argv[i]);
                opt = opt.substr(1);
                options_.push_back(opt);
            }
            else
            {
                int number = atoi(argv[i]);
                if (number > 0)
                {
                    maxItems_ = number;
                }
                else if (isValidRegex(argv[i]))
                {
                    regexes_.push_back(argv[i]);
                }
                else
                {
                    patterns_.push_back(argv[i]);
                }
            }
        }
    }

    inline void ProcessCmdLine::showCmdLine(int argc, char** argv)
    {
        if (argc > 2)
        {
            out_ << "  empty";
            return;
        }
        for (int i = 0; i < argc; ++i)
        {
            if (argv[i] == "0")
                continue;
            if (argv[i][0] == '/')
            {
                out_ << "/" + char(argv[i][1]) << " ";
            }
            else
            {
                out_ << argv[i] << " ";
            }
        }
    }

    inline void ProcessCmdLine::showCmdLine()
    {
        if (path_ == "" && options_.size() == 0 && patterns_.size() == 0)
        {
            out_ << "empty";
            return;
        }
        showPath();
        preface("", false);
        showOptions();
        preface("", false);
        showPatterns();
        preface("", false);
        showRegexes();
        showMaxItems();
    }

    inline void ProcessCmdLine::usage(const Usage& usage)
    {
        usage_ = usage;
    }

    inline void ProcessCmdLine::usage()
    {
        out_ << usage_;
    }
}