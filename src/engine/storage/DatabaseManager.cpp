#include "DatabaseManager.h"
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

// 校验库名：只允许字母、数字、下划线
static void validateDbName(const std::string& name) {
    if (name.empty())
        throw DBException(ErrorCode::DB_NAME_INVALID, "Database name cannot be empty");
    for (char c : name)
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_')
            throw DBException(ErrorCode::DB_NAME_INVALID,
                              "Invalid character in database name: '" + name + "'");
}

DatabaseManager::DatabaseManager(const std::string& dataDir) : dataDir_(dataDir) {}

void DatabaseManager::init() {
    try {
        if (!fs::exists(dataDir_))
            fs::create_directories(dataDir_);
    } catch (const fs::filesystem_error& e) {
        throw DBException(ErrorCode::FILE_IO_ERROR, e.what());
    }
}

std::vector<std::string> DatabaseManager::listDatabases() {
    std::vector<std::string> dbs;
    try {
        for (const auto& entry : fs::directory_iterator(dataDir_))
            if (entry.is_directory())
                dbs.push_back(entry.path().filename().string());
    } catch (...) {}
    std::sort(dbs.begin(), dbs.end());
    return dbs;
}

void DatabaseManager::createDatabase(const std::string& name) {
    validateDbName(name);
    auto path = fs::path(dataDir_) / name;
    if (fs::exists(path))
        throw DBException(ErrorCode::DB_ALREADY_EXISTS,
                          "Database '" + name + "' already exists");
    try {
        fs::create_directory(path);
    } catch (const fs::filesystem_error& e) {
        throw DBException(ErrorCode::FILE_IO_ERROR, e.what());
    }
}

void DatabaseManager::dropDatabase(const std::string& name) {
    auto path = fs::path(dataDir_) / name;
    if (!fs::exists(path))
        throw DBException(ErrorCode::DB_NOT_FOUND,
                          "Unknown database '" + name + "'");
    try {
        fs::remove_all(path);
    } catch (const fs::filesystem_error& e) {
        throw DBException(ErrorCode::FILE_IO_ERROR, e.what());
    }
}

bool DatabaseManager::databaseExists(const std::string& name) {
    return fs::is_directory(fs::path(dataDir_) / name);
}
