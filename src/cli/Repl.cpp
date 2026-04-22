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

// 去掉末尾空白
static std::string rtrim(const std::string& s) {
    size_t e = s.find_last_not_of(" \t\r\n");
    return e == std::string::npos ? "" : s.substr(0, e + 1);
}

// 去掉首部空白
static std::string ltrim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    return a == std::string::npos ? "" : s.substr(a);
}

// 检测并去掉末尾 ASCII 分号或中文全角分号（UTF-8: EF BC 9B），返回是否匹配
static bool stripSemicolon(std::string& s) {
    if (!s.empty() && s.back() == ';') {
        s.pop_back();
        return true;
    }
    // 中文全角分号 UTF-8 = 0xEF 0xBC 0x9B
    if (s.size() >= 3 &&
        (unsigned char)s[s.size()-3] == 0xEF &&
        (unsigned char)s[s.size()-2] == 0xBC &&
        (unsigned char)s[s.size()-1] == 0x9B) {
        s.resize(s.size() - 3);
        return true;
    }
    return false;
}

void Repl::run() {
    std::string line, buf;
    while (true) {
        std::cout << prompt() << std::flush;
        if (!std::getline(std::cin, line)) break;

        // 元命令（以 \ 开头）：立即执行，不需要分号
        std::string trimmed = ltrim(line);
        if (!trimmed.empty() && trimmed[0] == '\\') {
            buf.clear();
            handleInput(trimmed);
            continue;
        }

        buf += line;

        // 检测分号（ASCII 或中文全角）
        std::string candidate = rtrim(buf);
        if (stripSemicolon(candidate)) {
            candidate = rtrim(candidate);
            handleInput(candidate);
            buf.clear();
        } else {
            buf += ' ';
        }
    }
}

void Repl::runFile(const std::string& filePath) {
    std::ifstream f(filePath);
    if (!f) { std::cerr << "Cannot open: " << filePath << "\n"; return; }
    std::string buf, line;
    while (std::getline(f, line)) {
        std::string trimmed = ltrim(line);
        if (!trimmed.empty() && trimmed[0] == '\\') {
            buf.clear();
            handleInput(trimmed);
            continue;
        }
        buf += line;
        std::string candidate = rtrim(buf);
        if (stripSemicolon(candidate)) {
            candidate = rtrim(candidate);
            handleInput(candidate);
            buf.clear();
        } else {
            buf += ' ';
        }
    }
}
