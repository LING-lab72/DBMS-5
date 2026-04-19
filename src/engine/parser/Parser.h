#pragma once

#include "../../types.h"
#include "AST.h"
#include "../lexer/Lexer.h"

class Parser {
public:
    /**
     * 将 Token 序列解析为 AST 节点树
     * @throws DBException(SQL_SYNTAX_ERROR) 语法不合法时
     */
    ASTNodePtr parse(const std::vector<Token>& tokens);
};
