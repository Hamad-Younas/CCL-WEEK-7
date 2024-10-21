#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <map>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_STRING,
    T_BOOL,
    T_CHAR,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_EOF,
    T_TRUE,
    T_FALSE,
    T_STRING_LITERAL,
    T_CHAR_LITERAL,
    T_WHILE,
    T_FOR,
    T_BREAK,
    T_CONTINUE,
    T_CONST,
    T_VOID
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->pos = 0;
        this->line = 1;
    }

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (isspace(current))
            {
                if (current == '\n')
                    line++; // Increment line on newline
                pos++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line}); // Pass line number
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "float")
                    tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "double")
                    tokens.push_back(Token{T_DOUBLE, word, line});
                else if (word == "string")
                    tokens.push_back(Token{T_STRING, word, line});
                else if (word == "bool")
                    tokens.push_back(Token{T_BOOL, word, line});
                else if (word == "char")
                    tokens.push_back(Token{T_CHAR, word, line});
                else if (word == "true")
                    tokens.push_back(Token{T_TRUE, word, line});
                else if (word == "false")
                    tokens.push_back(Token{T_FALSE, word, line});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line}); // Recognize the while keyword
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, line}); // Also handle if keyword
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, line}); // Also handle else keyword
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line}); // Handle return keyword
                // Add more keywords as needed
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            if (current == '"')
            {
                tokens.push_back(Token{T_STRING_LITERAL, consumeStringLiteral(), line}); // Handle string literal
                continue;
            }

            if (current == '\'')
            {
                tokens.push_back(Token{T_CHAR_LITERAL, consumeCharLiteral(), line}); // Handle char literal
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line});
                break;
            default:
                cout << "Unexpected character: " << current << " on line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});

        // Print all tokens after tokenization
        for (const auto &token : tokens)
        {
            cout << "Token: " << tokenTypeToString(token.type) << " Value: " << token.value << " Line: " << token.line << endl;
        }

        return tokens;
    }

    string consumeStringLiteral()
    {
        pos++; // Skip the opening "
        size_t start = pos;
        while (pos < src.size() && src[pos] != '"')
            pos++;
        string literal = src.substr(start, pos - start);
        pos++; // Skip the closing "
        return literal;
    }

    string consumeCharLiteral()
    {
        pos++; // Skip the opening '
        char literal = src[pos];
        pos++; // Skip the char
        if (src[pos] != '\'')
        {
            cout << "Error: Invalid char literal at line " << line << endl;
            exit(1);
        }
        pos++; // Skip the closing '
        return string(1, literal);
    }

    string consumeNumber()
    {
        size_t start = pos;
        bool hasDecimalPoint = false;

        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
        {
            if (src[pos] == '.')
            {
                if (hasDecimalPoint)
                    break; // Already encountered a decimal point, so stop.
                hasDecimalPoint = true;
            }
            pos++;
        }

        string number = src.substr(start, pos - start);

        // Check if the number ends with a decimal point, which is invalid.
        if (number.back() == '.')
        {
            cout << "Error: Invalid number format at line " << line << endl;
            exit(1);
        }

        return number;
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        string word = src.substr(start, pos - start);
        return word;
    }

    string tokenTypeToString(TokenType type)
    {
        static map<TokenType, string> tokenTypeToStr = {
            {T_INT, "int"}, {T_FLOAT, "float"}, {T_DOUBLE, "double"}, {T_STRING, "string"}, {T_BOOL, "bool"}, {T_CHAR, "char"}, {T_ID, "identifier"}, {T_NUM, "number"}, {T_STRING_LITERAL, "string literal"}, {T_CHAR_LITERAL, "char literal"}, {T_IF, "if"}, {T_ELSE, "else"}, {T_RETURN, "return"}, {T_ASSIGN, "="}, {T_PLUS, "+"}, {T_MINUS, "-"}, {T_MUL, "*"}, {T_DIV, "/"}, {T_LPAREN, "("}, {T_RPAREN, ")"}, {T_LBRACE, "{"}, {T_RBRACE, "}"}, {T_SEMICOLON, ";"}, {T_GT, ">"}, {T_TRUE, "true"}, {T_FALSE, "false"}, {T_EOF, "end of file"}};
        return tokenTypeToStr[type];
    }
};

class Parser
{
public:
    Parser(const vector<Token> &tokens)
    {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_DOUBLE ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR ||
            tokens[pos].type == T_VOID || tokens[pos].type == T_CONST)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_WHILE) // Check for while statement
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else
        {
            cout << "Error: Unexpected token: " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void parseDeclaration()
    {
        // Handle variable declarations
        cout << "Parsing declaration: " << tokens[pos].value << endl;
        pos++; // Consume the type token
        if (tokens[pos].type == T_ID)
        {
            cout << "Declared variable: " << tokens[pos].value << endl;
            pos++; // Consume the identifier token
            if (tokens[pos].type == T_ASSIGN)
            {
                pos++;             // Consume the assignment token
                parseExpression(); // Parse the expression after assignment
            }
            if (tokens[pos].type == T_SEMICOLON)
            {
                pos++; // Consume the semicolon
            }
        }
        else
        {
            cout << "Error: Expected identifier after type declaration." << endl;
            exit(1);
        }
    }

    void parseAssignment()
    {
        cout << "Parsing assignment for: " << tokens[pos].value << endl;
        pos++; // Consume the identifier token
        if (tokens[pos].type == T_ASSIGN)
        {
            pos++; // Consume the assignment token
            parseExpression();
            if (tokens[pos].type == T_SEMICOLON)
            {
                pos++; // Consume the semicolon
            }
        }
        else
        {
            cout << "Error: Expected assignment operator." << endl;
            exit(1);
        }
    }

    void parseIfStatement()
    {
        cout << "Parsing if statement" << endl;
        pos++; // Consume the if token
        if (tokens[pos].type == T_LPAREN)
        {
            pos++;             // Consume the left parenthesis
            parseExpression(); // Placeholder for condition
            if (tokens[pos].type == T_RPAREN)
            {
                pos++; // Consume the right parenthesis
                if (tokens[pos].type == T_LBRACE)
                {
                    pos++; // Consume the left brace
                    while (tokens[pos].type != T_RBRACE)
                    {
                        parseStatement();
                    }
                    pos++; // Consume the right brace
                }
            }
        }
    }

    void parseWhileStatement()
    {
        cout << "Parsing while statement" << endl;
        pos++; // Consume the while token
        if (tokens[pos].type == T_LPAREN)
        {
            pos++;             // Consume the left parenthesis
            parseExpression(); // Placeholder for condition
            if (tokens[pos].type == T_RPAREN)
            {
                pos++; // Consume the right parenthesis
                if (tokens[pos].type == T_LBRACE)
                {
                    pos++; // Consume the left brace
                    while (tokens[pos].type != T_RBRACE)
                    {
                        parseStatement();
                    }
                    pos++; // Consume the right brace
                }
            }
        }
    }

    void parseReturnStatement()
    {
        cout << "Parsing return statement" << endl;
        pos++; // Consume the return token
        parseExpression();
        if (tokens[pos].type == T_SEMICOLON)
        {
            pos++; // Consume the semicolon
        }
    }

    void parseExpression()
    {
        cout << "Parsing expression: " << tokens[pos].value << endl; // Placeholder for expression parsing
        pos++;                                                       // Move to next token as a placeholder for expression handling
    }
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string input(buffer.begin(), buffer.end());

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
