#pragma once

#include "../../types.h"
#include <string>
#include <vector>
#include <map>

class RecordManager {
public:
    explicit RecordManager(const std::string& dataDir);

    /** 插入一行，返回该行在 .trd 文件中的物理字节偏移 */
    int64_t insert(const std::string& database, const std::string& table,
                   const std::map<std::string, FieldValue>& record);

    /** 全表顺序扫描，跳过已软删除行 */
    std::vector<Row> scan(const std::string& database, const std::string& table);

    /** 按物理偏移原地更新一行 */
    void update(const std::string& database, const std::string& table,
                int64_t offset, const std::map<std::string, FieldValue>& record);

    /** 按物理偏移标记软删除 */
    void remove(const std::string& database, const std::string& table,
                int64_t offset);

private:
    std::string dataDir_;
};
