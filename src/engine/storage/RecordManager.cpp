#include "RecordManager.h"

RecordManager::RecordManager(const std::string& dataDir) : dataDir_(dataDir) {}

int64_t RecordManager::insert(const std::string& /*database*/,
                               const std::string& /*table*/,
                               const std::map<std::string, FieldValue>& /*record*/) {
    return -1;  // TODO(成员D)
}

std::vector<Row> RecordManager::scan(const std::string& /*database*/,
                                      const std::string& /*table*/) {
    return {};  // TODO(成员D)
}

void RecordManager::update(const std::string& /*database*/,
                            const std::string& /*table*/,
                            int64_t /*offset*/,
                            const std::map<std::string, FieldValue>& /*record*/) {
    // TODO(成员D)
}

void RecordManager::remove(const std::string& /*database*/,
                            const std::string& /*table*/,
                            int64_t /*offset*/) {
    // TODO(成员D)
}
