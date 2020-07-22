#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <clocale>
#include <locale>
#include "FileSystem.h"

class FileSystemSearch
{
public:
    FileSystemSearch();
    ~FileSystemSearch();
    std::string firstFile(const std::string& path = ".", const std::string& pattern = "*.*");
    std::string nextFile();
    std::string firstDirectory(const std::string& path = ".", const std::string& pattern = "*.*");
    std::string nextDirectory();
    void close();
private:
    HANDLE hFindFile;
    WIN32_FIND_DATAA FindFileData;
    WIN32_FIND_DATAA* pFindFileData;
};

FileSystemSearch::FileSystemSearch() : pFindFileData(&FindFileData) {}
FileSystemSearch::~FileSystemSearch() { ::FindClose(hFindFile); }
void FileSystemSearch::close() { ::FindClose(hFindFile); }

Block::Block(Byte* beg, Byte* end) : bytes_(beg, end) {}

void Block::push_back(Byte b)
{
    bytes_.push_back(b);
}

Byte& Block::operator[](size_t i)
{
    if (i < 0 || bytes_.size() <= i)
        throw std::runtime_error("index out of range in Block");
    return bytes_[i];
}

Byte Block::operator[](size_t i) const
{
    if (i < 0 || bytes_.size() <= i)
        throw std::runtime_error("index out of range in Block");
    return bytes_[i];
}

bool Block::operator==(const Block& block) const
{
    return bytes_ == block.bytes_;
}

bool Block::operator!=(const Block& block) const
{
    return bytes_ != block.bytes_;
}

size_t Block::size() const
{
    return bytes_.size();
}

File::File(const std::string& filespec)
    : name_(filespec), pIStream(nullptr), pOStream(nullptr), dirn_(in), typ_(text), good_(true)
{
}

File::~File()
{
    if (pIStream)
    {
        pIStream->close();
        delete pIStream;
        pIStream = nullptr;
        good_ = false;
    }
    if (pOStream)
    {
        pOStream->close();
        delete pOStream;
        pOStream = nullptr;
        good_ = false;
    }
}

bool File::open(direction dirn, type typ)
{
    dirn_ = dirn;
    typ_ = typ;
    good_ = true;
    if (dirn == in)
    {
        pIStream = new std::ifstream;
        if (typ == binary)
            pIStream->open(name_.c_str(), std::ios::in | std::ios::binary);
        else
            pIStream->open(name_.c_str(), std::ios::in);
        if (!(*pIStream).good())
        {
            good_ = false;
            pIStream = nullptr;
        }
    }
    else
    {
        pOStream = new std::ofstream;
        if (typ == binary)
            pOStream->open(name_.c_str(), std::ios::out | std::ios::binary);
        else
            pOStream->open(name_.c_str(), std::ios::out);
        if (!(*pOStream).good())
        {
            good_ = false;
            pOStream = nullptr;
        }
    }
    return good_;
}

std::string File::getLine(bool keepNewLines)
{
    if (pIStream == nullptr || !pIStream->good())
        throw std::runtime_error("input stream not open");
    if (typ_ == binary)
        throw std::runtime_error("getting text line from binary file");
    if (dirn_ == out)
        throw std::runtime_error("reading output file");

    std::string store;
    while (true)
    {
        char ch = pIStream->get();
        if (!isGood())
            return store;
        if (ch == '\n')
        {
            if (keepNewLines)
                store += ch;
            return store;
        }
        store += ch;
    }
}

std::string File::readAll(bool keepNewLines)
{
    std::string store;
    while (true)
    {
        if (!isGood())
            return store;
        store += getLine(keepNewLines);
        std::locale loc;
        if (store.size() > 0 && !std::isspace(store[store.size() - 1], loc))
            store += ' ';
    }
    return store;
}

void File::putLine(const std::string& s, bool wantReturn)
{
    if (pOStream == nullptr || !pOStream->good())
        throw std::runtime_error("output stream not open");
    if (typ_ == binary)
        throw std::runtime_error("writing text line to binary file");
    if (dirn_ == in)
        throw std::runtime_error("writing input file");
    for (size_t i = 0; i < s.size(); ++i)
        pOStream->put(s[i]);
    if (wantReturn)
        pOStream->put('\n');
    pOStream->flush();
}

Block File::getBlock(size_t size)
{
    if (pIStream == nullptr || !pIStream->good())
        throw std::runtime_error("input stream not open");
    if (typ_ != binary)
        throw std::runtime_error("reading binary from text file");
    if (dirn_ == out)
        throw std::runtime_error("reading output file");
    Block blk;
    if (pIStream)
    {
        for (size_t i = 0; i < size; ++i)
        {
            Byte b;
            pIStream->get(b);
            if (pIStream->good())
                blk.push_back(b);
            else
                break;
        }
    }
    return blk;
}

void File::putBlock(const Block& blk)
{
    if (pOStream == nullptr || !pOStream->good())
        throw std::runtime_error("output stream not open");
    if (typ_ != binary)
        throw std::runtime_error("writing binary to text file");
    if (dirn_ == in)
        throw std::runtime_error("writing input file");
    if (!pOStream->good())
        return;
    for (size_t i = 0; i < blk.size(); ++i)
    {
        pOStream->put(blk[i]);
    }
}

size_t File::getBuffer(size_t bufLen, File::byte* buffer)
{
    if (pIStream == nullptr || !pIStream->good())
        throw std::runtime_error("input stream not open");
    if (typ_ != binary)
        throw std::runtime_error("reading binary from text file");
    if (dirn_ == out)
        throw std::runtime_error("reading output file");
    size_t count = 0;
    while (pIStream->good())
    {
        buffer[count++] = pIStream->get();
        if (count == bufLen)
            break;
    }
    if (!pIStream->good())
        --count;
    return count;
}

void File::putBuffer(size_t bufLen, File::byte* buffer)
{
    if (pOStream == nullptr || !pOStream->good())
        throw std::runtime_error("output stream not open");
    if (typ_ != binary)
        throw std::runtime_error("writing binary to text file");
    if (dirn_ == in)
        throw std::runtime_error("writing input file");
    if (!pOStream->good())
        return;
    size_t count = 0;
    while (pOStream->good())
    {
        pOStream->put(buffer[count++]);
        if (count == bufLen)
            break;
    }
}

bool File::isGood()
{
    if (!good_)
        return false;
    if (pIStream != nullptr)
        return (good_ = pIStream->good());
    if (pOStream != nullptr)
        return (good_ = pOStream->good());
    return (good_ = false);
}

void File::flush()
{
    if (pOStream != nullptr && pOStream->good())
        pOStream->flush();
}

void File::clear()
{
    if (pIStream != nullptr)
        pIStream->clear();
    if (pOStream != nullptr)
        pOStream->clear();
}

void File::close()
{
    File::flush();
    if (pIStream != nullptr)
    {
        pIStream->close();
        pIStream = nullptr;
        good_ = false;
    }
    if (pOStream)
    {
        pOStream->close();
        pOStream = nullptr;
        good_ = false;
    }
}

bool File::exists(const std::string& file)
{
    return ::GetFileAttributesA(file.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool File::copy(const std::string& src, const std::string& dst, bool failIfExists)
{
    return ::CopyFileA(src.c_str(), dst.c_str(), failIfExists) != 0;
}

bool File::remove(const std::string& file)
{
    return ::DeleteFileA(file.c_str()) != 0;
}

FileInfo::FileInfo(const std::string& fileSpec)
{
    hFindFile = ::FindFirstFileA(fileSpec.c_str(), &data);
    if (hFindFile == INVALID_HANDLE_VALUE)
        good_ = false;
    else
        good_ = true;
}

FileInfo::~FileInfo()
{
    ::FindClose(hFindFile);
}

bool FileInfo::good()
{
    return good_;
}

std::string FileInfo::name() const
{
    return Path::getName(data.cFileName);
}

std::string FileInfo::intToString(long i)
{
    std::ostringstream out;
    out.fill('0');
    out << std::setw(2) << i;
    return out.str();
}

std::string FileInfo::date(dateFormat df) const
{
    std::string dateStr, timeStr;
    FILETIME ft;
    SYSTEMTIME st;
    ::FileTimeToLocalFileTime(&data.ftLastWriteTime, &ft);
    ::FileTimeToSystemTime(&ft, &st);
    dateStr = intToString(st.wMonth) + '/' + intToString(st.wDay) + '/' + intToString(st.wYear);
    timeStr = intToString(st.wHour) + ':' + intToString(st.wMinute) + ':' + intToString(st.wSecond);
    if (df == dateformat)
        return dateStr;
    if (df == timeformat)
        return timeStr;
    return dateStr + " " + timeStr;
}

size_t FileInfo::size() const
{
    return (size_t)(data.nFileSizeLow + (data.nFileSizeHigh << 8));
}

bool FileInfo::isArchive() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0;
}

bool FileInfo::isCompressed() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0;
}

bool FileInfo::isDirectory() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool FileInfo::isEncrypted() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) != 0;
}

bool FileInfo::isHidden() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
}

bool FileInfo::isNormal() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
}

bool FileInfo::isOffLine() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) != 0;
}

bool FileInfo::isReadOnly() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
}

bool FileInfo::isSystem() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0;
}

bool FileInfo::isTemporary() const
{
    return (data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0;
}

bool FileInfo::operator<(const FileInfo& fi) const
{
    return strcmp(data.cFileName, fi.data.cFileName) == -1;
}

bool FileInfo::operator==(const FileInfo& fi) const
{
    return strcmp(data.cFileName, fi.data.cFileName) == 0;
}

bool FileInfo::operator>(const FileInfo& fi) const
{
    return strcmp(data.cFileName, fi.data.cFileName) == 1;
}

bool FileInfo::earlier(const FileInfo& fi) const
{
    FILETIME ft1 = data.ftLastWriteTime;
    FILETIME ft2 = fi.data.ftLastWriteTime;
    return ::CompareFileTime(&ft1, &ft2) == -1;
}

bool FileInfo::later(const FileInfo& fi) const
{
    FILETIME ft1 = data.ftLastWriteTime;
    FILETIME ft2 = fi.data.ftLastWriteTime;
    return ::CompareFileTime(&ft1, &ft2) == 1;
}

bool FileInfo::smaller(const FileInfo& fi) const
{
    return size() < fi.size();
}

bool FileInfo::larger(const FileInfo& fi) const
{
    return size() > fi.size();
}

std::string Path::toLower(const std::string& src)
{
    std::string temp;
    for (size_t i = 0; i < src.length(); ++i)
        temp += tolower(src[i]);
    return temp;
}

std::string Path::toUpper(const std::string& src)
{
    std::string temp;
    for (size_t i = 0; i < src.length(); ++i)
        temp += toupper(src[i]);
    return temp;
}

std::string Path::getName(const std::string& fileSpec, bool withExt)
{
    size_t pos = fileSpec.find_last_of("/");
    if (pos >= fileSpec.length())
    {
        pos = fileSpec.find_last_of("\\");
        if (pos >= fileSpec.length())
        {
            if (withExt)
                return fileSpec;
            else
            {
                size_t pos = fileSpec.find(".");
                if (pos > fileSpec.size())
                    return fileSpec;
                return fileSpec.substr(0, pos - 1);
            }
        }
    }
    if (withExt)
        return fileSpec.substr(pos + 1, fileSpec.length() - pos);
    else
    {
        size_t pos2 = fileSpec.find(".", pos);
        if (pos2 > fileSpec.size())
            return fileSpec.substr(pos + 1);
        return fileSpec.substr(pos + 1, pos2 - pos - 1);
    }
}

std::string Path::getExt(const std::string& fileSpec)
{
    size_t pos1 = fileSpec.find_last_of('/');
    size_t pos2 = fileSpec.find_last_of('\\');
    size_t pos = fileSpec.find_last_of('.');
    if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
    {
        if (pos < min(pos1, pos2))
            return std::string("");
    }
 
    if (0 <= pos && pos < fileSpec.length())
        return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
    return std::string("");
}

std::string Path::getPath(const std::string& fileSpec)
{
    size_t pos = fileSpec.find_last_of("/");
    if (pos >= fileSpec.length())
        pos = fileSpec.find_last_of("\\");
    if (pos >= fileSpec.length())
        return ".";
    if (fileSpec.find(".", pos + 1))
        return fileSpec.substr(0, pos + 1);
    return fileSpec;
}

std::string Path::getFullFileSpec(const std::string& fileSpec)
{
    const size_t BufSize = 256;
    char buffer[BufSize];
    char filebuffer[BufSize];
    char* name = filebuffer;
    ::GetFullPathNameA(fileSpec.c_str(), BufSize, buffer, &name);
    return std::string(buffer);
}

std::string Path::fileSpec(const std::string& path, const std::string& name)
{
    std::string fs;
    size_t len = path.size();
    if (path[len - 1] == '/' || path[len - 1] == '\\')
        fs = path + name;
    else
    {
        if (path.find("/") < path.size())
            fs = path + "/" + name;
        else if (path.find("\\") < path.size())
            fs = path + "\\" + name;
        else
            fs = path + "/" + name;
    }
    return fs;
}

std::string Directory::getCurrentDirectory()
{
    char buffer[MAX_PATH];
    ::GetCurrentDirectoryA(MAX_PATH, buffer);
    return std::string(buffer);
}

bool Directory::setCurrentDirectory(const std::string& path)
{
    return ::SetCurrentDirectoryA(path.c_str()) != 0;
}

std::vector<std::string> Directory::getFiles(const std::string& path, const std::string& pattern)
{
    std::vector<std::string> files;
    FileSystemSearch fss;
    std::string file = fss.firstFile(path, pattern);
    if (file.size() == 0)
        return files;
    files.push_back(file);
    while (true)
    {
        file = fss.nextFile();
        if (file.size() == 0)
            return files;
        files.push_back(file);
    }
    return files;
}

std::vector<std::string> Directory::getDirectories(const std::string& path, const std::string& pattern)
{
    std::vector<std::string> dirs;
    FileSystemSearch fss;
    std::string dir = fss.firstDirectory(path, pattern);
    if (dir.size() == 0)
        return dirs;
    dirs.push_back(dir);
    while (true)
    {
        dir = fss.nextDirectory();
        if (dir.size() == 0)
            return dirs;
        dirs.push_back(dir);
    }
    return dirs;
}

bool Directory::create(const std::string& path)
{
    return ::CreateDirectoryA(path.c_str(), NULL) != 0;
}

bool Directory::exists(const std::string& path)
{
    DWORD dwAttrib = GetFileAttributesA(path.c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Directory::remove(const std::string& path)
{
    return ::RemoveDirectoryA(path.c_str()) == 0;
}

std::string FileSystemSearch::firstFile(const std::string& path, const std::string& pattern)
{
    hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            return pFindFileData->cFileName;
        else
            while (::FindNextFileA(hFindFile, pFindFileData))
                if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    return pFindFileData->cFileName;
    }
    return "";
}

std::string FileSystemSearch::nextFile()
{
    while (::FindNextFileA(hFindFile, pFindFileData))
        if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            return pFindFileData->cFileName;
    return "";
}

std::string FileSystemSearch::firstDirectory(const std::string& path, const std::string& pattern)
{
    hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            return pFindFileData->cFileName;
        else
            while (::FindNextFileA(hFindFile, pFindFileData))
                if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    return pFindFileData->cFileName;
    }
    return "";
}

std::string FileSystemSearch::nextDirectory()
{
    while (::FindNextFileA(hFindFile, pFindFileData))
        if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            return pFindFileData->cFileName;
    return "";
}