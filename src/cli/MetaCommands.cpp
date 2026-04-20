#include "MetaCommands.h"
#include "Formatter.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

MetaResult MetaCommandHandler::handle(const std::string& input, CLISession& session) {
    std::string s = trim(input);
    if (s.empty() || s[0] != '\\') return {false, ""};

    // 取命令词（去掉前导 \，小写）
    std::string cmd;
    std::string args;
    auto sp = s.find_first_of(" \t", 1);
    if (sp == std::string::npos) {
        cmd = s.substr(1);
    } else {
        cmd  = s.substr(1, sp - 1);
        args = trim(s.substr(sp + 1));
    }
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (cmd == "help" || cmd == "h" || cmd == "?") {
        return {true,
            "Available commands:\n"
            "  \\help         Show this help\n"
            "  \\quit / \\q   Exit the DBMS\n"
            "  \\status       Show session status\n"
            "  \\clear        Clear screen\n"
            "  \\history      Show command history\n"
            "  \\use <db>     Switch database (alias for USE <db>)\n"
            "  \\databases    List databases (alias for SHOW DATABASES)\n"
            "  \\tables       List tables in current DB (alias for SHOW TABLES)\n"
            "  source <file> Execute SQL file\n"
        };
    }

    if (cmd == "quit" || cmd == "q" || cmd == "exit") {
        std::cout << Formatter::green("Bye!") << "\n";
        std::exit(0);
    }

    if (cmd == "status") {
        std::string db = session.engineSession.currentDatabase;
        std::string user = session.engineSession.user;
        return {true,
            "User   : " + (user.empty() ? "(anonymous)" : user) + "\n"
            "Database: " + (db.empty()  ? "(none)"      : db)   + "\n"
        };
    }

    if (cmd == "clear") {
#ifdef DBMS_WINDOWS
        std::system("cls");
#else
        std::cout << "\033[2J\033[H";
#endif
        return {true, ""};
    }

    if (cmd == "history") {
        std::string out;
        for (size_t i = 0; i < session.history.size(); ++i)
            out += std::to_string(i + 1) + "\t" + session.history[i] + "\n";
        if (out.empty()) out = "(empty history)\n";
        return {true, out};
    }

    if (cmd == "use" && !args.empty()) {
        // 转换为 SQL：不直接处理，让引擎处理；但我们可以合成 SQL 返回 unhandled
        // 实际上返回 handled=false 让调用方当 SQL 处理效果最好，
        // 这里我们构造 USE sql 交给引擎
        return {false, ""};  // fall-through to SQL engine with "USE <args>"
    }

    if (cmd == "databases") return {false, "SHOW DATABASES"};
    if (cmd == "tables")    return {false, "SHOW TABLES"};

    // 未知元命令
    return {true, Formatter::yellow("Unknown meta-command: \\" + cmd
                                    + "  (type \\help for help)\n")};
}
