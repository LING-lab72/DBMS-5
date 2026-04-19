#pragma once

#include "../../types.h"
#include "../parser/AST.h"
#include <map>
#include <string>

class ExprEvaluator {
public:
    /**
     * 对单条记录求值 WHERE 表达式
     * @param expr  WHERE 子句根节点
     * @param row   字段名 → 值 的映射（由 RecordManager 提供）
     * @returns     该行是否满足条件
     */
    bool evaluate(const WhereExpr& expr,
                  const std::map<std::string, FieldValue>& row);
};
