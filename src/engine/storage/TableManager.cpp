#include "TableManager.h"

TableManager::TableManager(const std::string& dataDir) : dataDir_(dataDir) {}

std::vector<std::string> TableManager::listTables(const std::string& /*database*/) {
    return {};  // TODO(成员D)
}

std::optional<TableDefinition> TableManager::describeTable(
    const std::string& /*database*/, const std::string& /*table*/) {
    return std::nullopt;  // TODO(成员D)
}

void TableManager::createTable(const std::string& /*database*/,
                                const TableDefinition& /*def*/) {}

void TableManager::dropTable(const std::string& /*database*/,
                              const std::string& /*table*/) {}

void TableManager::addColumn(const std::string& /*database*/,
                              const std::string& /*table*/,
                              const ColumnDefinition& /*col*/) {}

void TableManager::modifyColumn(const std::string& /*database*/,
                                 const std::string& /*table*/,
                                 const std::string& /*colName*/,
                                 const ColumnDefinition& /*newDef*/) {}

void TableManager::dropColumn(const std::string& /*database*/,
                               const std::string& /*table*/,
                               const std::string& /*colName*/) {}
