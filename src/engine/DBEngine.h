#pragma once

#include "types.h"
#include <string>

// 引擎会话上下文（贯穿整个执行链）
struct Session {
    std::string sessionId;
    std::string currentDatabase;  // 空串表示未 USE 任何库
    std::string transactionId;    // 空串表示无活跃事务
    std::string user;             // 当前登录用户，空串表示匿名
};

class DBEngine {
public:
    /** @param dataDir 数据文件根目录，不存在时自动创建 */
    explicit DBEngine(const std::string& dataDir);
    ~DBEngine();

    /** 执行任意单条 SQL（不含末尾分号） */
    QueryResult execute(const std::string& sql, Session& session);

    /** 显式开始事务 */
    void beginTransaction(Session& session);

    /** 提交当前事务 */
    void commit(Session& session);

    /** 回滚当前事务 */
    void rollback(Session& session);

private:
    std::string dataDir_;

    // 前向声明内部子模块（实现文件中引入具体头文件）
    struct Impl;
    Impl* impl_;
};
