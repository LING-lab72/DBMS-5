#include "engine/DBEngine.h"
#include "engine/lexer/Lexer.h"
#include "engine/parser/Parser.h"
#include "engine/executor/Executor.h"

#include <chrono>

struct DBEngine::Impl {
    Lexer    lexer;
    Parser   parser;
    Executor executor;

    explicit Impl(const std::string& dataDir) : executor(dataDir) {}
};

DBEngine::DBEngine(const std::string& dataDir)
    : dataDir_(dataDir), impl_(new Impl(dataDir))
{
    // 引擎启动：DatabaseManager 初始化由 Executor 内部完成
}

DBEngine::~DBEngine() {
    delete impl_;
}

QueryResult DBEngine::execute(const std::string& sql, Session& session) {
    auto t0 = std::chrono::steady_clock::now();
    try {
        auto tokens = impl_->lexer.tokenize(sql);
        auto ast    = impl_->parser.parse(tokens);
        auto result = impl_->executor.execute(*ast, session);
        auto t1     = std::chrono::steady_clock::now();
        result.elapsedMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        return result;
    } catch (const DBException& e) {
        return QueryResult::err(e.error().code, e.error().message);
    } catch (const std::exception& e) {
        return QueryResult::err(ErrorCode::UNKNOWN_ERROR, e.what());
    }
}

void DBEngine::beginTransaction(Session& session) {
    execute("BEGIN", session);
}

void DBEngine::commit(Session& session) {
    execute("COMMIT", session);
}

void DBEngine::rollback(Session& session) {
    execute("ROLLBACK", session);
}
