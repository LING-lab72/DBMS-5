#pragma once

#include "../../types.h"
#include <string>
#include <vector>
#include <optional>

class TableManager {
public:
    explicit TableManager(const std::string& dataDir);

    std::vector<std::string>       listTables(const std::string& database);
    std::optional<TableDefinition> describeTable(const std::string& database,
                                                  const std::string& table);
    void createTable(const std::string& database, const TableDefinition& def);
    void dropTable  (const std::string& database, const std::string& table);

    void addColumn   (const std::string& database, const std::string& table,
                      const ColumnDefinition& col);
    void modifyColumn(const std::string& database, const std::string& table,
                      const std::string& colName, const ColumnDefinition& newDef);
    void dropColumn  (const std::string& database, const std::string& table,
                      const std::string& colName);

private:
    std::string dataDir_;
};
