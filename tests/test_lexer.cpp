#include "../src/engine/lexer/Lexer.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

// 极简测试辅助
static int passed = 0, failed = 0;

#define ASSERT_EQ(a, b) do { \
    auto _a = (a); auto _b = (b); \
    if (_a == _b) { ++passed; } \
    else { ++failed; std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "\n"; } \
} while(0)

#define ASSERT_TRUE(expr) ASSERT_EQ((expr), true)

static std::vector<Token> lex(const std::string& sql) {
    Lexer l;
    auto toks = l.tokenize(sql);
    // 去掉最后的 EOF
    if (!toks.empty() && toks.back().type == TokenType::EOF_TOKEN)
        toks.pop_back();
    return toks;
}

// ============================================================
// 测试：DDL 关键字
// ============================================================
static void test_keywords() {
    auto t = lex("CREATE DATABASE mydb");
    ASSERT_EQ(t.size(), (size_t)3);
    ASSERT_EQ(t[0].type, TokenType::CREATE);
    ASSERT_EQ(t[1].type, TokenType::DATABASE);
    ASSERT_EQ(t[2].type, TokenType::IDENTIFIER);
    ASSERT_EQ(t[2].value, std::string("mydb"));
}

static void test_all_keywords_are_tokenized() {
    const std::vector<std::pair<std::string, TokenType>> cases = {
        {"CREATE", TokenType::CREATE}, {"DROP", TokenType::DROP},
        {"DATABASE", TokenType::DATABASE}, {"TABLE", TokenType::TABLE},
        {"INDEX", TokenType::INDEX}, {"SHOW", TokenType::SHOW},
        {"DATABASES", TokenType::DATABASES}, {"TABLES", TokenType::TABLES},
        {"USE", TokenType::USE}, {"DESCRIBE", TokenType::DESCRIBE},
        {"ALTER", TokenType::ALTER}, {"ADD", TokenType::ADD},
        {"MODIFY", TokenType::MODIFY}, {"COLUMN", TokenType::COLUMN},
        {"INSERT", TokenType::INSERT}, {"INTO", TokenType::INTO},
        {"VALUES", TokenType::VALUES}, {"SELECT", TokenType::SELECT},
        {"DISTINCT", TokenType::DISTINCT}, {"FROM", TokenType::FROM},
        {"WHERE", TokenType::WHERE}, {"UPDATE", TokenType::UPDATE},
        {"SET", TokenType::SET}, {"DELETE", TokenType::DELETE},
        {"PRIMARY", TokenType::PRIMARY}, {"KEY", TokenType::KEY},
        {"FOREIGN", TokenType::FOREIGN}, {"REFERENCES", TokenType::REFERENCES},
        {"UNIQUE", TokenType::UNIQUE}, {"NOT", TokenType::NOT},
        {"NULL", TokenType::NULL_KW}, {"DEFAULT", TokenType::DEFAULT},
        {"AUTO_INCREMENT", TokenType::AUTO_INCREMENT},
        {"INT", TokenType::INT_KW}, {"INTEGER", TokenType::INTEGER_KW},
        {"DOUBLE", TokenType::DOUBLE_KW}, {"FLOAT", TokenType::FLOAT_KW},
        {"VARCHAR", TokenType::VARCHAR_KW}, {"BOOL", TokenType::BOOL_KW},
        {"BOOLEAN", TokenType::BOOL_KW}, {"DATETIME", TokenType::DATETIME_KW},
        {"CONSTRAINT", TokenType::CONSTRAINT}, {"ORDER", TokenType::ORDER},
        {"BY", TokenType::BY}, {"ASC", TokenType::ASC},
        {"DESC", TokenType::DESC}, {"LIMIT", TokenType::LIMIT},
        {"OFFSET", TokenType::OFFSET}, {"GROUP", TokenType::GROUP},
        {"HAVING", TokenType::HAVING}, {"AS", TokenType::AS},
        {"AND", TokenType::AND}, {"OR", TokenType::OR},
        {"IN", TokenType::IN}, {"LIKE", TokenType::LIKE},
        {"IS", TokenType::IS}, {"BETWEEN", TokenType::BETWEEN},
        {"COUNT", TokenType::COUNT}, {"SUM", TokenType::SUM},
        {"MAX", TokenType::MAX}, {"MIN", TokenType::MIN},
        {"AVG", TokenType::AVG}, {"IF", TokenType::IF},
        {"EXISTS", TokenType::EXISTS}, {"JOIN", TokenType::JOIN},
        {"INNER", TokenType::INNER}, {"LEFT", TokenType::LEFT},
        {"RIGHT", TokenType::RIGHT}, {"OUTER", TokenType::OUTER},
        {"CROSS", TokenType::CROSS}, {"TRUE", TokenType::BOOL_LITERAL},
        {"FALSE", TokenType::BOOL_LITERAL}, {"BEGIN", TokenType::BEGIN},
        {"COMMIT", TokenType::COMMIT}, {"ROLLBACK", TokenType::ROLLBACK},
        {"TRANSACTION", TokenType::TRANSACTION}, {"GRANT", TokenType::GRANT},
        {"REVOKE", TokenType::REVOKE}, {"ON", TokenType::ON},
        {"TO", TokenType::TO}, {"WITH", TokenType::WITH},
        {"OPTION", TokenType::OPTION}, {"ALL", TokenType::ALL},
        {"PRIVILEGES", TokenType::PRIVILEGES}, {"USER", TokenType::USER},
        {"PASSWORD", TokenType::PASSWORD}, {"CONNECT", TokenType::CONNECT},
        {"IDENTIFIED", TokenType::IDENTIFIED}, {"BACKUP", TokenType::BACKUP},
        {"RESTORE", TokenType::RESTORE},
    };

    for (const auto& [word, type] : cases) {
        auto t = lex(word);
        ASSERT_EQ(t.size(), (size_t)1);
        ASSERT_EQ(t[0].type, type);
    }
}

static void test_case_insensitive() {
    auto t = lex("select * From users where id = 1");
    ASSERT_EQ(t[0].type, TokenType::SELECT);
    ASSERT_EQ(t[1].type, TokenType::STAR);
    ASSERT_EQ(t[2].type, TokenType::FROM);
    ASSERT_EQ(t[3].type, TokenType::IDENTIFIER);
    ASSERT_EQ(t[4].type, TokenType::WHERE);
    ASSERT_EQ(t[5].type, TokenType::IDENTIFIER);
    ASSERT_EQ(t[6].type, TokenType::EQ);
    ASSERT_EQ(t[7].type, TokenType::INT_LITERAL);
    ASSERT_EQ(t[7].value, std::string("1"));
}

// ============================================================
// 测试：字符串字面量
// ============================================================
static void test_string_literal() {
    auto t = lex("'hello world'");
    ASSERT_EQ(t.size(), (size_t)1);
    ASSERT_EQ(t[0].type, TokenType::STRING_LITERAL);
    ASSERT_EQ(t[0].value, std::string("hello world"));
}

static void test_string_escape() {
    auto t = lex("'it\\'s'");
    ASSERT_EQ(t[0].type, TokenType::STRING_LITERAL);
    ASSERT_EQ(t[0].value, std::string("it's"));
}

static void test_double_quote_string() {
    auto t = lex("\"name\"");
    ASSERT_EQ(t[0].type, TokenType::STRING_LITERAL);
    ASSERT_EQ(t[0].value, std::string("name"));
}

// ============================================================
// 测试：数字字面量
// ============================================================
static void test_integers() {
    auto t = lex("0 42 -1");
    ASSERT_EQ(t[0].type, TokenType::INT_LITERAL);
    ASSERT_EQ(t[0].value, std::string("0"));
    ASSERT_EQ(t[1].type, TokenType::INT_LITERAL);
    ASSERT_EQ(t[1].value, std::string("42"));
    ASSERT_EQ(t[2].type, TokenType::MINUS);   // '-' is separate token
    ASSERT_EQ(t[3].type, TokenType::INT_LITERAL);
}

static void test_float() {
    auto t = lex("3.14  2.5e10");
    ASSERT_EQ(t[0].type, TokenType::DOUBLE_LITERAL);
    ASSERT_EQ(t[0].value, std::string("3.14"));
    ASSERT_EQ(t[1].type, TokenType::DOUBLE_LITERAL);
    ASSERT_EQ(t[1].value, std::string("2.5e10"));
}

// ============================================================
// 测试：运算符
// ============================================================
static void test_operators() {
    auto t = lex("<= >= != <> < > =");
    ASSERT_EQ(t[0].type, TokenType::LE);
    ASSERT_EQ(t[1].type, TokenType::GE);
    ASSERT_EQ(t[2].type, TokenType::NEQ);
    ASSERT_EQ(t[3].type, TokenType::NEQ);
    ASSERT_EQ(t[4].type, TokenType::LT);
    ASSERT_EQ(t[5].type, TokenType::GT);
    ASSERT_EQ(t[6].type, TokenType::EQ);
}

// ============================================================
// 测试：注释跳过
// ============================================================
static void test_comments() {
    auto t = lex("SELECT -- this is a comment\n1");
    ASSERT_EQ(t[0].type, TokenType::SELECT);
    ASSERT_EQ(t[1].type, TokenType::INT_LITERAL);
    ASSERT_EQ(t.size(), (size_t)2);
}

static void test_block_comment() {
    auto t = lex("SELECT /* inline */ 1");
    ASSERT_EQ(t[0].type, TokenType::SELECT);
    ASSERT_EQ(t[1].type, TokenType::INT_LITERAL);
    ASSERT_EQ(t.size(), (size_t)2);
}

// ============================================================
// 测试：反引号标识符
// ============================================================
static void test_backtick_ident() {
    auto t = lex("`my table`");
    ASSERT_EQ(t[0].type, TokenType::IDENTIFIER);
    ASSERT_EQ(t[0].value, std::string("my table"));
}

// ============================================================
// 测试：完整 CREATE TABLE 语句
// ============================================================
static void test_create_table() {
    const std::string sql =
        "CREATE TABLE users ("
        "  id INT PRIMARY KEY AUTO_INCREMENT,"
        "  name VARCHAR(100) NOT NULL,"
        "  score DOUBLE DEFAULT 0.0"
        ");";
    auto t = lex(sql);
    ASSERT_EQ(t[0].type, TokenType::CREATE);
    ASSERT_EQ(t[1].type, TokenType::TABLE);
    ASSERT_EQ(t[2].type, TokenType::IDENTIFIER);  // users
    ASSERT_EQ(t[3].type, TokenType::LPAREN);
    // id INT PRIMARY KEY AUTO_INCREMENT ,
    ASSERT_EQ(t[4].type, TokenType::IDENTIFIER);   // id
    ASSERT_EQ(t[5].type, TokenType::INT_KW);
    ASSERT_EQ(t[6].type, TokenType::PRIMARY);
    ASSERT_EQ(t[7].type, TokenType::KEY);
    ASSERT_EQ(t[8].type, TokenType::AUTO_INCREMENT);
    ASSERT_EQ(t[9].type, TokenType::COMMA);
}

// ============================================================
// 测试：行列号追踪
// ============================================================
static void test_line_col() {
    Lexer l;
    auto t = l.tokenize("SELECT\n  1");
    ASSERT_EQ(t[0].line, 1);
    ASSERT_EQ(t[1].line, 2);
    ASSERT_EQ(t[1].col,  3);
}

// ============================================================
// 测试：布尔与 NULL
// ============================================================
static void test_bool_null() {
    auto t = lex("TRUE FALSE NULL");
    ASSERT_EQ(t[0].type, TokenType::BOOL_LITERAL);
    ASSERT_EQ(t[0].value, std::string("TRUE"));
    ASSERT_EQ(t[1].type, TokenType::BOOL_LITERAL);
    ASSERT_EQ(t[2].type, TokenType::NULL_KW);
}

// ============================================================
// main
// ============================================================
int main() {
    test_keywords();
    test_all_keywords_are_tokenized();
    test_case_insensitive();
    test_string_literal();
    test_string_escape();
    test_double_quote_string();
    test_integers();
    test_float();
    test_operators();
    test_comments();
    test_block_comment();
    test_backtick_ident();
    test_create_table();
    test_line_col();
    test_bool_null();

    std::cout << "\nResults: " << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
