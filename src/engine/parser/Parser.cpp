#include "Parser.h"

ASTNodePtr Parser::parse(const std::vector<Token>& /*tokens*/) {
    // TODO(成员C): 实现递归下降语法分析
    auto node = std::make_unique<ShowDatabasesNode>();
    node->type = NodeType::SHOW_DATABASES;
    return node;
}
