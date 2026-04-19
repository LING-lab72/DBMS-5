#pragma once

#include "../../types.h"
#include <string>
#include <vector>

class IndexManager {
public:
    explicit IndexManager(const std::string& dataDir);

    /** 构建 B+Tree 索引并写入 .tid 文件 */
    void createIndex(const std::string& database, const std::string& table,
                     const std::string& indexName,
                     const std::vector<std::string>& columns, bool unique);

    void dropIndex(const std::string& database, const std::string& table,
                   const std::string& indexName);

    /**
     * 通过索引查找满足等值条件的记录物理偏移列表
     * （范围查询由调用方多次调用或后续迭代器扩展）
     */
    std::vector<int64_t> lookup(const std::string& database,
                                 const std::string& table,
                                 const std::string& indexName,
                                 const FieldValue&  key);

    std::vector<IndexDefinition> listIndexes(const std::string& database,
                                              const std::string& table);

private:
    std::string dataDir_;
};
