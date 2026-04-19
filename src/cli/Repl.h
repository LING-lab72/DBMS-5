#pragma once

#include "Session.h"
#include "../engine/DBEngine.h"

class Repl {
public:
    /**
     * @param engine  已初始化的引擎实例
     * @param session 初始 CLI 会话状态
     */
    Repl(DBEngine& engine, CLISession& session);

    /** 进入交互式循环，直至用户输入 \quit 或 EOF */
    void run();

    /** 执行单个 SQL 文件（批处理 / source 命令入口） */
    void runFile(const std::string& filePath);

private:
    DBEngine&   engine_;
    CLISession& session_;

    void        handleInput(const std::string& sql);
    std::string prompt() const;
};
