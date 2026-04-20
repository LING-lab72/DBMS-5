#include "TableManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

namespace fs = std::filesystem;

// ============================================================
// .tdf 格式工具函数
// ============================================================

static std::string fieldTypeName(FieldType t) {
    switch (t) {
        case FieldType::INTEGER:  return "INTEGER";
        case FieldType::DOUBLE:   return "DOUBLE";
        case FieldType::BOOL:     return "BOOL";
        case FieldType::VARCHAR:  return "VARCHAR";
        case FieldType::DATETIME: return "DATETIME";
    }
    return "INTEGER";
}

static FieldType parseFieldTypeStr(const std::string& s) {
    if (s == "DOUBLE")   return FieldType::DOUBLE;
    if (s == "BOOL")     return FieldType::BOOL;
    if (s == "VARCHAR")  return FieldType::VARCHAR;
    if (s == "DATETIME") return FieldType::DATETIME;
    return FieldType::INTEGER;
}

static std::map<std::string, std::string> readIni(const std::string& path) {
    std::map<std::string, std::string> m;
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        m[line.substr(0, eq)] = line.substr(eq + 1);
    }
    return m;
}

static std::string tdfPath(const std::string& dataDir,
                            const std::string& db,
                            const std::string& table) {
    return (fs::path(dataDir) / db / (table + ".tdf")).string();
}

static void saveDef(const std::string& dataDir, const std::string& db,
                    const TableDefinition& def, int64_t aiCounter = 0) {
    std::ofstream f(tdfPath(dataDir, db, def.name), std::ios::trunc);
    f << "col_count=" << def.columns.size() << "\n";
    for (size_t i = 0; i < def.columns.size(); ++i) {
        const auto& c = def.columns[i];
        std::string px = "col." + std::to_string(i) + ".";
        f << px << "name="     << c.name                       << "\n";
        f << px << "type="     << fieldTypeName(c.type)         << "\n";
        f << px << "length="   << c.length                      << "\n";
        f << px << "nullable=" << (c.nullable      ? "1" : "0") << "\n";
        f << px << "default="  << c.defaultValue                << "\n";
        f << px << "pk="       << (c.primaryKey    ? "1" : "0") << "\n";
        f << px << "ai="       << (c.autoIncrement ? "1" : "0") << "\n";
        f << px << "unique="   << (c.unique        ? "1" : "0") << "\n";
    }
    f << "fk_count=" << def.foreignKeys.size() << "\n";
    for (size_t i = 0; i < def.foreignKeys.size(); ++i) {
        const auto& fk = def.foreignKeys[i];
        std::string px = "fk." + std::to_string(i) + ".";
        std::string cols, rcols;
        for (size_t j = 0; j < fk.columns.size(); ++j)
            cols += (j ? "," : "") + fk.columns[j];
        for (size_t j = 0; j < fk.refColumns.size(); ++j)
            rcols += (j ? "," : "") + fk.refColumns[j];
        f << px << "name="     << fk.constraintName << "\n";
        f << px << "cols="     << cols               << "\n";
        f << px << "reftable=" << fk.refTable        << "\n";
        f << px << "refcols="  << rcols              << "\n";
    }
    f << "ai_counter=" << aiCounter << "\n";
}

static std::optional<TableDefinition> loadDef(const std::string& dataDir,
                                               const std::string& db,
                                               const std::string& table) {
    auto path = tdfPath(dataDir, db, table);
    if (!fs::exists(path)) return std::nullopt;
    auto m = readIni(path);
    TableDefinition def;
    def.name = table;
    int colCount = std::stoi(m.count("col_count") ? m["col_count"] : "0");
    def.columns.resize(colCount);
    for (int i = 0; i < colCount; ++i) {
        std::string px = "col." + std::to_string(i) + ".";
        auto& c = def.columns[i];
        c.name          = m.count(px + "name")     ? m[px + "name"]     : "";
        c.type          = parseFieldTypeStr(m.count(px + "type") ? m[px + "type"] : "INTEGER");
        c.length        = std::stoi(m.count(px + "length")   ? m[px + "length"]   : "0");
        c.nullable      = (m.count(px + "nullable") ? m[px + "nullable"] : "1") == "1";
        c.defaultValue  = m.count(px + "default")  ? m[px + "default"] : "";
        c.primaryKey    = (m.count(px + "pk")     ? m[px + "pk"]     : "0") == "1";
        c.autoIncrement = (m.count(px + "ai")     ? m[px + "ai"]     : "0") == "1";
        c.unique        = (m.count(px + "unique") ? m[px + "unique"] : "0") == "1";
    }
    int fkCount = std::stoi(m.count("fk_count") ? m["fk_count"] : "0");
    for (int i = 0; i < fkCount; ++i) {
        std::string px = "fk." + std::to_string(i) + ".";
        ForeignKeyDefinition fk;
        fk.constraintName = m.count(px + "name")     ? m[px + "name"]     : "";
        fk.refTable       = m.count(px + "reftable") ? m[px + "reftable"] : "";
        auto splitComma = [](const std::string& s) {
            std::vector<std::string> v;
            std::istringstream ss(s);
            std::string tok;
            while (std::getline(ss, tok, ','))
                if (!tok.empty()) v.push_back(tok);
            return v;
        };
        fk.columns    = splitComma(m.count(px + "cols")    ? m[px + "cols"]    : "");
        fk.refColumns = splitComma(m.count(px + "refcols") ? m[px + "refcols"] : "");
        def.foreignKeys.push_back(fk);
    }
    return def;
}

// ============================================================
// TableManager 实现
// ============================================================

TableManager::TableManager(const std::string& dataDir) : dataDir_(dataDir) {}

std::vector<std::string> TableManager::listTables(const std::string& database) {
    std::vector<std::string> tables;
    auto dbPath = fs::path(dataDir_) / database;
    if (!fs::exists(dbPath)) return tables;
    for (const auto& entry : fs::directory_iterator(dbPath))
        if (entry.is_regular_file() && entry.path().extension() == ".tdf")
            tables.push_back(entry.path().stem().string());
    std::sort(tables.begin(), tables.end());
    return tables;
}

std::optional<TableDefinition> TableManager::describeTable(
    const std::string& database, const std::string& table) {
    return loadDef(dataDir_, database, table);
}

void TableManager::createTable(const std::string& database,
                                const TableDefinition& def) {
    if (!fs::exists(fs::path(dataDir_) / database))
        throw DBException(ErrorCode::DB_NOT_FOUND, "Unknown database '" + database + "'");
    if (fs::exists(tdfPath(dataDir_, database, def.name)))
        throw DBException(ErrorCode::TABLE_ALREADY_EXISTS,
                          "Table '" + def.name + "' already exists");
    saveDef(dataDir_, database, def, 0);
}

void TableManager::dropTable(const std::string& database,
                              const std::string& table) {
    auto path = tdfPath(dataDir_, database, table);
    if (!fs::exists(path))
        throw DBException(ErrorCode::TABLE_NOT_FOUND, "Unknown table '" + table + "'");
    fs::remove(path);
    auto trd = (fs::path(dataDir_) / database / (table + ".trd")).string();
    if (fs::exists(trd)) fs::remove(trd);
}

void TableManager::addColumn(const std::string& database, const std::string& table,
                              const ColumnDefinition& col) {
    auto def = loadDef(dataDir_, database, table);
    if (!def)
        throw DBException(ErrorCode::TABLE_NOT_FOUND, "Unknown table '" + table + "'");
    for (const auto& c : def->columns)
        if (c.name == col.name)
            throw DBException(ErrorCode::COLUMN_ALREADY_EXISTS,
                              "Column '" + col.name + "' already exists");
    def->columns.push_back(col);
    auto m = readIni(tdfPath(dataDir_, database, table));
    int64_t ai = std::stoll(m.count("ai_counter") ? m["ai_counter"] : "0");
    saveDef(dataDir_, database, *def, ai);
}

void TableManager::modifyColumn(const std::string& database, const std::string& table,
                                 const std::string& colName,
                                 const ColumnDefinition& newDef) {
    auto def = loadDef(dataDir_, database, table);
    if (!def)
        throw DBException(ErrorCode::TABLE_NOT_FOUND, "Unknown table '" + table + "'");
    bool found = false;
    for (auto& c : def->columns)
        if (c.name == colName) { c = newDef; found = true; break; }
    if (!found)
        throw DBException(ErrorCode::COLUMN_NOT_FOUND, "Unknown column '" + colName + "'");
    auto m = readIni(tdfPath(dataDir_, database, table));
    int64_t ai = std::stoll(m.count("ai_counter") ? m["ai_counter"] : "0");
    saveDef(dataDir_, database, *def, ai);
}

void TableManager::dropColumn(const std::string& database, const std::string& table,
                               const std::string& colName) {
    auto def = loadDef(dataDir_, database, table);
    if (!def)
        throw DBException(ErrorCode::TABLE_NOT_FOUND, "Unknown table '" + table + "'");
    auto it = std::remove_if(def->columns.begin(), def->columns.end(),
        [&](const ColumnDefinition& c){ return c.name == colName; });
    if (it == def->columns.end())
        throw DBException(ErrorCode::COLUMN_NOT_FOUND,
                          "Unknown column '" + colName + "'");
    def->columns.erase(it, def->columns.end());
    auto m = readIni(tdfPath(dataDir_, database, table));
    int64_t ai = std::stoll(m.count("ai_counter") ? m["ai_counter"] : "0");
    saveDef(dataDir_, database, *def, ai);
}

int64_t TableManager::nextAutoIncrement(const std::string& database,
                                         const std::string& table) {
    auto path = tdfPath(dataDir_, database, table);
    auto m = readIni(path);
    int64_t next = std::stoll(m.count("ai_counter") ? m["ai_counter"] : "0") + 1;
    std::ifstream fin(path);
    std::string content, line;
    bool done = false;
    while (std::getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.rfind("ai_counter=", 0) == 0) {
            content += "ai_counter=" + std::to_string(next) + "\n";
            done = true;
        } else {
            content += line + "\n";
        }
    }
    fin.close();
    if (!done) content += "ai_counter=" + std::to_string(next) + "\n";
    std::ofstream fout(path, std::ios::trunc);
    fout << content;
    return next;
}

bool TableManager::tableExists(const std::string& database,
                                const std::string& table) {
    return fs::exists(tdfPath(dataDir_, database, table));
}
