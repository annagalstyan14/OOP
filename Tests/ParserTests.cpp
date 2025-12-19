#include "TestFramework.h"
#include "../Parser/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Command/CommandFactory.h"

using namespace ppt;
using namespace test;

// ========== Tokenizer Tests ==========

TEST(Tokenizer_EmptyInput) {
    Tokenizer tokenizer("");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::END);
}

TEST(Tokenizer_SingleKeyword) {
    Tokenizer tokenizer("add");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::KEYWORD);
    ASSERT_EQ("add", token.value);
    ASSERT_TRUE(token.keyword == Keyword::ADD);
}

TEST(Tokenizer_MultipleKeywords) {
    Tokenizer tokenizer("add slide");
    
    Token t1 = tokenizer.getToken();
    ASSERT_TRUE(t1.keyword == Keyword::ADD);
    
    Token t2 = tokenizer.getToken();
    ASSERT_TRUE(t2.keyword == Keyword::SLIDE);
    
    Token t3 = tokenizer.getToken();
    ASSERT_TRUE(t3.type == TokenType::END);
}

TEST(Tokenizer_StringLiteral) {
    Tokenizer tokenizer("\"Hello World\"");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::STRING);
    ASSERT_EQ("Hello World", token.value);
}

TEST(Tokenizer_SingleQuoteString) {
    Tokenizer tokenizer("'Hello'");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::STRING);
    ASSERT_EQ("Hello", token.value);
}

TEST(Tokenizer_Number) {
    Tokenizer tokenizer("42");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::NUMBER);
    ASSERT_EQ("42", token.value);
}

TEST(Tokenizer_NegativeNumber) {
    Tokenizer tokenizer("-123");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::NUMBER);
    ASSERT_EQ("-123", token.value);
}

TEST(Tokenizer_FloatNumber) {
    Tokenizer tokenizer("3.14");
    Token token = tokenizer.getToken();
    
    ASSERT_TRUE(token.type == TokenType::NUMBER);
    ASSERT_EQ("3.14", token.value);
}

TEST(Tokenizer_ComplexCommand) {
    Tokenizer tokenizer("add slide \"My Slide\"");
    
    Token t1 = tokenizer.getToken();
    ASSERT_TRUE(t1.keyword == Keyword::ADD);
    
    Token t2 = tokenizer.getToken();
    ASSERT_TRUE(t2.keyword == Keyword::SLIDE);
    
    Token t3 = tokenizer.getToken();
    ASSERT_TRUE(t3.type == TokenType::STRING);
    ASSERT_EQ("My Slide", t3.value);
}

TEST(Tokenizer_AllKeywords) {
    Tokenizer tokenizer("add remove list new open save load slide shape text");
    
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::ADD);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::REMOVE);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::LIST);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::NEW);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::OPEN);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::SAVE);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::LOAD);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::SLIDE);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::SHAPE);
    ASSERT_TRUE(tokenizer.getToken().keyword == Keyword::TEXT);
}

TEST(Tokenizer_CaseInsensitive) {
    Tokenizer tokenizer("ADD SLIDE");
    
    Token t1 = tokenizer.getToken();
    ASSERT_TRUE(t1.keyword == Keyword::ADD);
    
    Token t2 = tokenizer.getToken();
    ASSERT_TRUE(t2.keyword == Keyword::SLIDE);
}

TEST(Tokenizer_WhitespaceHandling) {
    Tokenizer tokenizer("   add   slide   ");
    
    Token t1 = tokenizer.getToken();
    ASSERT_TRUE(t1.keyword == Keyword::ADD);
    
    Token t2 = tokenizer.getToken();
    ASSERT_TRUE(t2.keyword == Keyword::SLIDE);
}

TEST(Tokenizer_Eof) {
    Tokenizer tokenizer("add");
    ASSERT_FALSE(tokenizer.eof());
    
    tokenizer.getToken();  // consume "add"
    ASSERT_TRUE(tokenizer.eof());
}

// ========== Parser Tests ==========

TEST(Parser_EmptyCommand) {
    CommandFactory factory;
    Parser parser("", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd == nullptr);
    ASSERT_TRUE(parser.hasError());
}

TEST(Parser_UnknownCommand) {
    CommandFactory factory;
    Parser parser("unknown", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd == nullptr);
    ASSERT_TRUE(parser.hasError());
}

TEST(Parser_AddSlideCommand) {
    CommandFactory factory;
    Parser parser("add slide \"Test Slide\"", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd != nullptr);
    ASSERT_FALSE(parser.hasError());
}

TEST(Parser_AddShapeCommand) {
    CommandFactory factory;
    Parser parser("add shape \"MyRect\" rectangle blue", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd != nullptr);
}

TEST(Parser_RemoveSlideCommand) {
    CommandFactory factory;
    Parser parser("remove slide 1", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd != nullptr);
}

TEST(Parser_ListCommand) {
    CommandFactory factory;
    Parser parser("list", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd != nullptr);
}

TEST(Parser_ListSlidesCommand) {
    CommandFactory factory;
    Parser parser("list slides", factory);
    
    auto cmd = parser.parse();
    
    ASSERT_TRUE(cmd != nullptr);
}

// ========== CommandFactory Tests ==========

TEST(CommandFactory_HasCommand) {
    CommandFactory factory;
    
    ASSERT_TRUE(factory.hasCommand("add"));
    ASSERT_TRUE(factory.hasCommand("remove"));
    ASSERT_TRUE(factory.hasCommand("list"));
    ASSERT_FALSE(factory.hasCommand("unknown"));
}

TEST(CommandFactory_CreateAddCommand) {
    CommandFactory factory;
    std::vector<ArgPtr> args;
    args.push_back(std::make_unique<Argument>("Test"));
    
    auto cmd = factory.create("add", "slide", std::move(args));
    
    ASSERT_TRUE(cmd != nullptr);
}

TEST(CommandFactory_CreateListCommand) {
    CommandFactory factory;
    std::vector<ArgPtr> args;
    
    auto cmd = factory.create("list", "", std::move(args));
    
    ASSERT_TRUE(cmd != nullptr);
}

TEST(CommandFactory_UnknownCommand) {
    CommandFactory factory;
    std::vector<ArgPtr> args;
    
    ASSERT_THROW(factory.create("unknown", "", std::move(args)), std::runtime_error);
}

// ========== Argument Tests ==========

TEST(Argument_StringConstructor) {
    Argument arg("hello");
    
    ASSERT_TRUE(arg.isString());
    ASSERT_FALSE(arg.isNumber());
    ASSERT_EQ("hello", arg.asString());
}

TEST(Argument_NumberConstructor) {
    Argument arg(42.5);
    
    ASSERT_TRUE(arg.isNumber());
    ASSERT_FALSE(arg.isString());
    ASSERT_DOUBLE_EQ(42.5, arg.asNumber());
}

TEST(Argument_AsInt) {
    Argument arg(42.7);
    
    ASSERT_EQ(42, arg.asInt());
}

TEST(Argument_WrongTypeAccess) {
    Argument strArg("hello");
    Argument numArg(42.0);
    
    ASSERT_THROW(strArg.asNumber(), std::runtime_error);
    ASSERT_THROW(numArg.asString(), std::runtime_error);
}
