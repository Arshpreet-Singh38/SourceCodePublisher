#include "DependencyTable.h"
#include <sstream>

DependencyTable::DependencyTable() {
}

DependencyTable::DependencyTable(const files& list)
{
    table_.reserve(list.size());
    for (auto file : list)
        table_[file];
}

DependencyTable::~DependencyTable() {
}

bool DependencyTable::has(const std::string& file)
{
    return table_.find(file) != table_.end();
}

void DependencyTable::addDependency(file filename, file dependency)
{
    DependencyTable::dependencies& deps = table_[filename];
    auto dep = std::find(deps.begin(), deps.end(), dependency);
    if (dep == deps.end())
        deps.push_back(dependency);
}

void DependencyTable::removeDependency(file filename, file dependency)
{
    DependencyTable::dependencies& deps = table_[filename];
    auto dep = std::find(deps.begin(), deps.end(), dependency);
    if (dep != deps.end())
        deps.erase(dep);
}

void DependencyTable::removeEntry(file filename)
{
    auto entry = table_.find(filename);
    if (entry != table_.end())
        table_.erase(entry);
}

const DependencyTable::dependencies& DependencyTable::getDependencies(file filename) const
{
    return table_.at(filename);
}

const DependencyTable::dependencies& DependencyTable::operator[](const file& filename) const
{
    return table_.at(filename);
}

DependencyTable::iterator DependencyTable::begin()
{
    return table_.begin();
}

DependencyTable::iterator DependencyTable::end()
{
    return table_.end();
}

DependencyTable::files DependencyTable::getFiles() const
{
    files list;
    for (auto entry : table_)
        list.push_back(entry.first);
    return list;
}

void DependencyTable::display()
{
    std::ostringstream out;
    out << "\n  Dependency table:";
    for (auto entry : table_) {
        out << "\n  -- " << entry.first;
        for (auto dep : entry.second)
            out << "\n   -> " << dep;
    }
}