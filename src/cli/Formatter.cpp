#include "Formatter.h"
#include <iostream>
#include <sstream>

#ifdef DBMS_WINDOWS
#include <windows.h>
#endif

void Formatter::enableAnsi() {
#ifdef DBMS_WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

std::string Formatter::red   (const std::string& s) { return "\033[31m" + s + "\033[0m"; }
std::string Formatter::green (const std::string& s) { return "\033[32m" + s + "\033[0m"; }
std::string Formatter::yellow(const std::string& s) { return "\033[33m" + s + "\033[0m"; }
std::string Formatter::bold  (const std::string& s) { return "\033[1m"  + s + "\033[0m"; }

std::string Formatter::format(const QueryResult& result) {
    // TODO(成员B): 实现表格渲染
    if (result.error) return red("ERROR: " + result.error->message) + "\n";
    return result.message + "\n";
}

void Formatter::formatPaged(const QueryResult& result, int /*pageSize*/) {
    // TODO(成员B): 实现分页输出
    std::cout << format(result);
}
