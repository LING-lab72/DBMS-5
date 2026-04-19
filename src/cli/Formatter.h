#pragma once

#include "../types.h"
#include <string>

class Formatter {
public:
    /** 将 QueryResult 渲染为终端字符串（表格 / 受影响行数 / 错误信息） */
    std::string format(const QueryResult& result);

    /** 分页输出：超过 pageSize 行时暂停等待回车继续 */
    void formatPaged(const QueryResult& result, int pageSize);

    // ANSI 颜色辅助（Windows 下需提前 enableAnsi()）
    static void        enableAnsi();          // 开启 Windows 虚拟终端
    static std::string red   (const std::string& s);
    static std::string green (const std::string& s);
    static std::string yellow(const std::string& s);
    static std::string bold  (const std::string& s);
};
