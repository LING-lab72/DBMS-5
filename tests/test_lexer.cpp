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

// ---- helpers ----
static std::string typeName(TokenType t) {
    switch(t) {
#define C(x) case TokenType::x: return #x;
        C(CREATE) C(DROP) C(DATABASE) C(TABLE) C(SHOW) C(DATABASES) C(TABLES)
        C(USE) C(DESCRIBE) C(ALTER) C(ADD) C(MODIFY) C(COLUMN)
        C(INSERT) C(INTO) C(VALUES) C(SELECT) C(FROM) C(WHERE)
        C(UPDATE) C(SET) C(DELETE) C(INDEX)
        C(PRIMARY) C(KEY) C(FOREIGN) C(REFERENCES) C(UNIQUE) C(NOT) C(NULL_KW)
        C(DEFAULT) C(AUTO_INCREMENT) C(INT_KW) C(INTEGER_KW)
        C(DOUBLE_KW) C(FLOAT_KW) C(VARCHAR_KW) C(BOOL_KW) C(DATETIME_KW) C(CONSTRAINT)
        C(ORDER) C(BY) C(ASC) C(DESC) C(LIMIT) C(OFFSET) C(GROUP) C(HAVING) C(AS)
        C(AND) C(OR) C(IN) C(LIKE) C(IS) C(BETWEEN)
        C(COUNT) C(SUM) C(MAX) C(MIN) C(AVG) C(IF) C(EXISTS)
        C(BEGIN) C(COMMIT) C(ROLLBACK) C(TRANSACTION)
        C(GRANT) C(REVOKE) C(ON) C(TO) C(WITH) C(OPTION) C(ALL) C(PRIVILEGES)
        C(USER) C(PASSWORD)
        C(EQ) C(NEQ) C(LT) C(LE) C(GT) C(GE)
        C(PLUS) C(MINUS) C(STAR) C(SLASH) C(PERCENT)
        C(LPAREN) C(RPAREN) C(COMMA) C(SEMICOLON) C(DOT) C(ASSIGN)
        C(INT_LITERAL) C(DOUBLE_LITERAL) C(STRING_LITERAL) C(BOOL_LITERAL) C(NULL_LITERAL)
        C(IDENTIFIER) C(EOF_TOKEN) C(UNKNOWN)
#undef C
        default: return "?";
    }
}

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
