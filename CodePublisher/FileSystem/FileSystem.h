#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

typedef char Byte;

class Block
{
    public:
        Block(size_t size = 0) : bytes_(size) {}
        Block(Byte* beg, Byte* end);
        void push_back(Byte b);
        Byte& operator[](size_t i);
        Byte operator[](size_t i) const;
        bool operator==(const Block&) const;
        bool operator!=(const Block&) const;
        size_t size() const;
    private:
        std::vector<Byte> bytes_;
    };

class File
{
    public:
        using byte = char;
        enum direction { in, out };
        enum type { text, binary };
        File(const std::string& filespec);
        bool open(direction dirn, type typ = File::text);
        ~File();
        std::string name();
        std::string getLine(bool keepNewLine = false);
        std::string readAll(bool KeepNewLines = false);
        void putLine(const std::string& line, bool wantReturn = true);
        Block getBlock(size_t size);
        void putBlock(const Block&);
        size_t getBuffer(size_t bufLen, byte* buffer);
        void putBuffer(size_t bufLen, byte* buffer);
        bool isGood();
        void clear();
        void flush();
        void close();
        static bool exists(const std::string& file);
        static bool copy(const std::string& src, const std::string& dst, bool failIfExists = false);
        static bool remove(const std::string& filespec);
    private:
        std::string name_;
        std::ifstream* pIStream;
        std::ofstream* pOStream;
        direction dirn_;
        type typ_;
        bool good_;
    };

    inline std::string File::name() { return name_; }


class FileInfo
    {
    public:
        enum dateFormat { fullformat, timeformat, dateformat };
        FileInfo(const std::string& fileSpec);
        ~FileInfo();
        bool good();
        std::string name() const;
        std::string date(dateFormat df = fullformat) const;
        size_t size() const;

        bool isArchive() const;
        bool isCompressed() const;
        bool isDirectory() const;
        bool isEncrypted() const;
        bool isHidden() const;
        bool isNormal() const;
        bool isOffLine() const;
        bool isReadOnly() const;
        bool isSystem() const;
        bool isTemporary() const;

        bool operator<(const FileInfo& fi) const;
        bool operator==(const FileInfo& fi) const;
        bool operator>(const FileInfo& fi) const;
        bool earlier(const FileInfo& fi) const;
        bool later(const FileInfo& fi) const;
        bool smaller(const FileInfo& fi) const;
        bool larger(const FileInfo& fi) const;
    private:
        bool good_;
        static std::string intToString(long i);
        WIN32_FIND_DATAA data;
        HANDLE hFindFile;
    };

class Path
    {
    public:
        static std::string getFullFileSpec(const std::string& fileSpec);
        static std::string getPath(const std::string& fileSpec);
        static std::string getName(const std::string& fileSpec, bool withExt = true);
        static std::string getExt(const std::string& fileSpec);
        static std::string fileSpec(const std::string& path, const std::string& name);
        static std::string toLower(const std::string& src);
        static std::string toUpper(const std::string& src);
 };

class Directory
    {
    public:
        static bool create(const std::string& path);
        static bool remove(const std::string& path);
        static bool exists(const std::string& path);
        static std::string getCurrentDirectory();
        static bool setCurrentDirectory(const std::string& path);
        static std::vector<std::string> getFiles(const std::string& path = ".", const std::string& pattern = "*.*");
        static std::vector<std::string> getDirectories(const std::string& path = ".", const std::string& pattern = "*.*");
    private:
};