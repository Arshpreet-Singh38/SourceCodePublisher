#include <iostream>
#include "Converter.h"
#include "../FileSystem/FileSystem.h"
#include "../DependencyTable/DependencyTable.cpp"

CodeConverter::CodeConverter()
{
}

CodeConverter::CodeConverter(const DependencyTable& dt) : dt_(dt)
{
}

void CodeConverter::setDepTable(const DependencyTable& dt)
{
    dt_ = dt;
}

std::vector<std::string> CodeConverter::convert()
{
    if (!createOutputDir()) {
        return std::vector<std::string>();
    }

    for (auto entry : dt_) {
        if (convertFile(entry.first)) {
            std::cout << std::endl;
            std::cout << "Converted: ";
        }
        else {
            std::cout << "Failed";
        }
        std::cout << entry.first << std::endl;
        std::string filename = Path::getName(entry.first);
    }
    return convertedFiles_;
}

std::string CodeConverter::convert(const std::string& filepath)
{
    if (!createOutputDir()) {
        return "";
    }

    if (convertFile(filepath)) {
    }
    else {
    }

    std::string filename = Path::getName(filepath);
    return filename + ".html";
}

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
    clear();
    dt_ = DependencyTable(files);
    return convert();
}

const std::string& CodeConverter::outputDir() const
{
    return outputDir_;
}

void CodeConverter::outputDir(const std::string& dir)
{
    outputDir_ = dir;
}

bool CodeConverter::createOutputDir()
{
    if (!Directory::exists(outputDir_)) {
        return Directory::create(outputDir_);
    }
    return true;
}

const std::vector<std::string> CodeConverter::convertedFiles() const
{
    return convertedFiles_;
}

void CodeConverter::clear()
{
    convertedFiles_.clear();
    dt_.clear();
}

bool CodeConverter::convertFile(std::string file)
{
    std::ifstream in(file);
    if (!in.is_open() || !in.good()) {
        in.close();
        return false;
    }

    std::string filename = Path::getName(file);
    std::string outputPath = outputDir_ + filename + ".html";
    out_.open(outputPath, std::ofstream::out);
    if (!out_.is_open() || !out_.good()) {
        in.close();
        return false;
    }

    addPreCodeHTML(filename);
    addDependencyLinks(file);
    addPreTag();

    while (in.good()) {
        std::string line;
        while (std::getline(in, line)) {
            skipSpecialChars(line);
            out_ << line << std::endl;
        }
    }

    addClosingTags();
    out_.close();

    convertedFiles_.push_back(outputPath);
    return true;
}

void CodeConverter::addPreCodeHTML(const std::string& title)
{
    out_ << "<DOCTYPE !HTML>" << std::endl;
    out_ << "<html>" << std::endl;
    out_ << "  <head>" << std::endl;
    out_ << "    <Title>" << title << "</Title>" << std::endl;
    out_ << "    <style>" << std::endl;
    out_ << "      body {" << std::endl;
    out_ << "        padding:15px 40px;" << std::endl;
    out_ << "        font-family: Consolas;" << std::endl;
    out_ << "        font-size: 1.25em;" << std::endl;
    out_ << "        font-weight: normal;" << std::endl;
    out_ << "      }" << std::endl;
    out_ << "      </style>" << std::endl;
    out_ << "  </head>" << std::endl << std::endl;
    out_ << "  <body>" << std::endl;
}

void CodeConverter::addPreTag()
{
    out_ << "    <pre>" << std::endl;
}

void CodeConverter::addDependencyLinks(std::string file)
{
    std::string filename = Path::getName(file);
    if (!dt_.has(file)) {
        return;
    }

    if (dt_[file].size() == 0) {
        return;
    }

    out_ << "    <h3>Dependencies: " << std::endl;
    for (auto dep : dt_[file]) {
        out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
    }
    out_ << "    </h3>";
}

void CodeConverter::addClosingTags()
{
    out_ << "    </pre>" << std::endl;
    out_ << "  </body>" << std::endl;
    out_ << "</html>" << std::endl;
}

void CodeConverter::skipSpecialChars(std::string& line)
{
    size_t pos = line.npos;
    while ((pos = line.find('<')) != line.npos)
        line.replace(pos, 1, "&lt;");

    while ((pos = line.find('>')) != line.npos)
        line.replace(pos, 1, "&gt;");
}