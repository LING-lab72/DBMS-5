#pragma once

#include "../../types.h"
#include "../parser/AST.h"
#include "../../engine/DBEngine.h"

class Executor {
public:
    /**
     * 接收 AST 节点，分发到对应执行逻辑，返回查询结果
     * @param dataDir 数据文件根目录，透传给各 Manager
     */
    explicit Executor(const std::string& dataDir);

    QueryResult execute(const ASTNode& ast, Session& session);

private:
    std::string dataDir_;
};
