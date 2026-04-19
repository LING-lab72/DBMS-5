#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& dataDir) : dataDir_(dataDir) {}

void DatabaseManager::init() {
    // TODO(成员D): 创建 data 目录与 .mdb 系统文件
}

std::vector<std::string> DatabaseManager::listDatabases() {
    // TODO(成员D)
    return {};
}

void DatabaseManager::createDatabase(const std::string& /*name*/) {
    // TODO(成员D)
}

void DatabaseManager::dropDatabase(const std::string& /*name*/) {
    // TODO(成员D)
}

bool DatabaseManager::databaseExists(const std::string& /*name*/) {
    // TODO(成员D)
    return false;
}
