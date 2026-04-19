#include "IndexManager.h"

IndexManager::IndexManager(const std::string& dataDir) : dataDir_(dataDir) {}

void IndexManager::createIndex(const std::string& /*database*/,
                                const std::string& /*table*/,
                                const std::string& /*indexName*/,
                                const std::vector<std::string>& /*columns*/,
                                bool /*unique*/) {
    // TODO(成员D)
}

void IndexManager::dropIndex(const std::string& /*database*/,
                              const std::string& /*table*/,
                              const std::string& /*indexName*/) {
    // TODO(成员D)
}

std::vector<int64_t> IndexManager::lookup(const std::string& /*database*/,
                                           const std::string& /*table*/,
                                           const std::string& /*indexName*/,
                                           const FieldValue&  /*key*/) {
    return {};  // TODO(成员D)
}

std::vector<IndexDefinition> IndexManager::listIndexes(
    const std::string& /*database*/, const std::string& /*table*/) {
    return {};  // TODO(成员D)
}
