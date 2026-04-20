#pragma once

#include "../../types.h"
#include "../parser/AST.h"
#include "../../engine/DBEngine.h"
#include "../storage/DatabaseManager.h"
#include "../storage/TableManager.h"
#include "../storage/RecordManager.h"
#include "../storage/IndexManager.h"

class Executor {
public:
    explicit Executor(const std::string& dataDir);

    QueryResult execute(const ASTNode& ast, Session& session);

private:
    std::string     dataDir_;
    DatabaseManager dbMgr_;
    TableManager    tblMgr_;
    RecordManager   recMgr_;
    IndexManager    idxMgr_;

    // 解析 SQL 中指定的库名 or 使用当前会话库
    std::string resolveDb(const std::string& nodeDb, const Session& s);

    // DDL – 数据库
    QueryResult execCreateDatabase (const CreateDatabaseNode&  n, Session& s);
    QueryResult execDropDatabase   (const DropDatabaseNode&    n, Session& s);
    QueryResult execShowDatabases  (Session& s);
    QueryResult execUseDatabase    (const UseDatabaseNode&     n, Session& s);

    // DDL – 表
    QueryResult execCreateTable    (const CreateTableNode&     n, Session& s);
    QueryResult execDropTable      (const DropTableNode&       n, Session& s);
    QueryResult execShowTables     (Session& s);
    QueryResult execDescribeTable  (const DescribeTableNode&   n, Session& s);
    QueryResult execAlterTable     (const AlterTableNode&      n, Session& s);

    // DDL – 索引
    QueryResult execCreateIndex    (const CreateIndexNode&     n, Session& s);
    QueryResult execDropIndex      (const DropIndexNode&       n, Session& s);

    // DML
    QueryResult execInsert         (const InsertNode&          n, Session& s);
    QueryResult execSelect         (const SelectNode&          n, Session& s);
    QueryResult execUpdate         (const UpdateNode&          n, Session& s);
    QueryResult execDelete         (const DeleteNode&          n, Session& s);
};
