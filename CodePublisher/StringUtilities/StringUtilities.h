#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <locale>

namespace Utilities
{
    inline void Title(const std::string& text, std::ostream& out = std::cout, char underline = '=')
    {
        out << "\n  " << text;
        out << "\n " << std::string(text.size() + 2, underline);
    }

    inline void title(const std::string& text, std::ostream& out = std::cout, char underline = '-')
    {
        out << "\n  " << text;
        out << "\n " << std::string(text.size() + 2, underline);
    }

    inline void putline(size_t j = 1, std::ostream& out = std::cout)
    {
        for (size_t i = 0; i < j; ++i)
            out << "\n";
    }
    
    template <typename T>
    inline std::basic_string<T> trim(const std::basic_string<T>& toTrim)
    {
        if (toTrim.size() == 0)
            return toTrim;
        std::basic_string<T> temp;
        std::locale loc;
        typename std::basic_string<T>::const_iterator iter = toTrim.begin();
        while (isspace(*iter, loc) && *iter != '\n')
        {
            if (++iter == toTrim.end())
            {
                break;
            }
        }
        for (; iter != toTrim.end(); ++iter)
        {
            temp += *iter;
        }
        typename std::basic_string<T>::reverse_iterator riter;
        size_t pos = temp.size();
        for (riter = temp.rbegin(); riter != temp.rend(); ++riter)
        {
            --pos;
            if (!isspace(*riter, loc) || *riter == '\n')
            {
                break;
            }
        }
        if (0 <= pos && pos < temp.size())
            temp.erase(++pos);
        return temp;
    }

    template <typename T>
    inline std::vector<std::basic_string<T>> split(const std::basic_string<T>& toSplit, T splitOn = ',')
    {
        std::vector<std::basic_string<T>> splits;
        std::basic_string<T> temp;
        typename std::basic_string<T>::const_iterator iter;
        for (iter = toSplit.begin(); iter != toSplit.end(); ++iter)
        {
            if (*iter != splitOn)
            {
                temp += *iter;
            }
            else
            {
                splits.push_back(trim(temp));
                temp.clear();
            }
        }
        if (temp.length() > 0)
            splits.push_back(trim(temp));
        return splits;
    }

    template <typename T>
    inline void showSplits(const std::vector<std::basic_string<T>>& splits, std::ostream& out = std::cout)
    {
        out << "\n";
        for (auto item : splits)
        {
            if (item == "\n")
                out << "\n--" << "newline";
            else
                out << "\n--" << item;
        }
        out << "\n";
    }

    inline std::string wToS(const std::wstring& ws)
    {
        std::string conv;
        for (size_t i = 0; i < ws.size() - 1; i++)
        {
            conv.push_back(static_cast<char>(ws[i]));
        }
        return conv;
    }

    inline std::wstring sToW(const std::string& s)
    {
        std::wstring conv;
        for (auto ch : s)
        {
            conv.push_back(static_cast<wchar_t>(ch));
        }
        return conv;
    }
}