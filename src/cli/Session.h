#pragma once

#include "../engine/DBEngine.h"
#include <string>
#include <vector>

struct CLISession {
    Session     engineSession;
    std::string historyFile;
    int         pageSize = 40;   // 分页行数

    // 内存历史记录（环形缓冲，\history 命令使用）
    std::vector<std::string> history;
    static constexpr int     MAX_HISTORY = 100;

    void addHistory(const std::string& sql) {
        if (history.size() >= MAX_HISTORY) history.erase(history.begin());
        history.push_back(sql);
    }
};
