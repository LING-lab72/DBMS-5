#include "Repl.h"
#include "Formatter.h"
#include "MetaCommands.h"
#include <iostream>
#include <fstream>
#include <sstream>

Repl::Repl(DBEngine& engine, CLISession& session)
    : engine_(engine), session_(session) {}

std::string Repl::prompt() const {
    const auto& db = session_.engineSession.currentDatabase;
    return db.empty() ? "dbms> " : "dbms [" + db + "]> ";
}

void Repl::handleInput(const std::string& sql) {
    // TODO(成员B): 元命令分流 + 引擎调用 + Formatter 输出
    Formatter fmt;
    MetaCommandHandler meta;
    auto mr = meta.handle(sql, session_);
    if (mr.handled) { std::cout << mr.output; return; }

    auto result = engine_.execute(sql, session_.engineSession);
    fmt.formatPaged(result, session_.pageSize);
}

void Repl::run() {
    // TODO(成员B): 完整 REPL 主循环（多行输入、分号检测、历史记录）
    std::string line, buf;
    while (true) {
        std::cout << prompt();
        if (!std::getline(std::cin, line)) break;
        buf += line;
        if (!buf.empty() && buf.back() == ';') {
            buf.pop_back();
            handleInput(buf);
            buf.clear();
        } else {
            buf += ' ';
        }
    }
}

void Repl::runFile(const std::string& filePath) {
    // TODO(成员B): 读取 SQL 文件逐条执行
    std::ifstream f(filePath);
    if (!f) { std::cerr << "Cannot open: " << filePath << "\n"; return; }
    std::string sql, line;
    while (std::getline(f, line)) {
        sql += line;
        if (!sql.empty() && sql.back() == ';') {
            sql.pop_back();
            handleInput(sql);
            sql.clear();
        } else {
            sql += ' ';
        }
    }
}
