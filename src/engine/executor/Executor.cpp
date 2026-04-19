#include "Executor.h"

Executor::Executor(const std::string& dataDir) : dataDir_(dataDir) {}

QueryResult Executor::execute(const ASTNode& /*ast*/, Session& /*session*/) {
    // TODO(成员E): 按 ast.type 分发到各执行逻辑
    return QueryResult::ok("stub: executor not implemented");
}
