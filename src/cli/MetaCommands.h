#pragma once

#include "Session.h"
#include <string>

struct MetaResult {
    bool        handled = false;
    std::string output;   // handled=true 时有效
};

class MetaCommandHandler {
public:
    /**
     * 判断并处理元命令（以 \ 开头，或 source 命令）
     * @param input   用户原始输入（已去除首尾空白）
     * @param session 当前 CLI 会话（可能被修改，例如 \use 切库）
     * @returns       handled=false 表示不是元命令，由调用方继续解析为 SQL
     */
    MetaResult handle(const std::string& input, CLISession& session);
};
