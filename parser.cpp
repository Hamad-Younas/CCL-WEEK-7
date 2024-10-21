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
    T_LT,
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
                    line++;
                pos++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
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
                else if (word == "Agar") // Custom keyword for 'if'
                    tokens.push_back(Token{T_IF, word, line});
                else if (word == "warna") // Custom keyword for 'else'
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "jabtak") // Custom keyword for 'while'
                    tokens.push_back(Token{T_WHILE, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            // Handle other tokens like before
            if (current == '"')
            {
                tokens.push_back(Token{T_STRING_LITERAL, consumeStringLiteral(), line});
                continue;
            }

            if (current == '\'')
            {
                tokens.push_back(Token{T_CHAR_LITERAL, consumeCharLiteral(), line});
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
            case '<':
                tokens.push_back(Token{T_LT, "<", line});
                break;
            default:
                cout << "Unexpected character: " << current << " on line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});

        return tokens;
    }

    string consumeStringLiteral()
    {
        pos++;
        size_t start = pos;
        while (pos < src.size() && src[pos] != '"')
            pos++;
        string literal = src.substr(start, pos - start);
        pos++;
        return literal;
    }

    string consumeCharLiteral()
    {
        pos++;
        char literal = src[pos];
        pos++;
        if (src[pos] != '\'')
        {
            cout << "Error: Invalid char literal at line " << line << endl;
            exit(1);
        }
        pos++;
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
                    break;
                hasDecimalPoint = true;
            }
            pos++;
        }

        string number = src.substr(start, pos - start);
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
            {T_INT, "int"}, {T_FLOAT, "float"}, {T_DOUBLE, "double"}, {T_STRING, "string"}, {T_BOOL, "bool"}, {T_CHAR, "char"}, {T_ID, "identifier"}, {T_NUM, "number"}, {T_STRING_LITERAL, "string literal"}, {T_CHAR_LITERAL, "char literal"}, {T_IF, "if"}, {T_ELSE, "else"}, {T_RETURN, "return"}, {T_ASSIGN, "="}, {T_PLUS, "+"}, {T_MINUS, "-"}, {T_MUL, "*"}, {T_DIV, "/"}, {T_LPAREN, "("}, {T_RPAREN, ")"}, {T_LBRACE, "{"}, {T_RBRACE, "}"}, {T_SEMICOLON, ";"}, {T_GT, ">"}, {T_LT, "<"}, {T_TRUE, "true"}, {T_FALSE, "false"}, {T_EOF, "end of file"}};
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
        else if (tokens[pos].type == T_WHILE)
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
        pos++;
        if (tokens[pos].type == T_ID)
        {
            pos++;

            if (tokens[pos].type == T_ASSIGN)
            {
                pos++;
                parseExpression();
            }

            if (tokens[pos].type == T_SEMICOLON)
            {
                pos++;
            }
            else
            {
                cout << "Error: Expected semicolon after declaration." << endl;
                exit(1);
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
        pos++;
        if (tokens[pos].type == T_ASSIGN)
        {
            pos++;
            parseExpression();
        }
        if (tokens[pos].type == T_SEMICOLON)
        {
            pos++;
        }
        else
        {
            cout << "Error: Expected semicolon after assignment." << endl;
            exit(1);
        }
    }

    void parseIfStatement()
    {
        pos++; // consume 'if'
        if (tokens[pos].type == T_LPAREN)
        {
            pos++;             // consume '('
            parseExpression(); // parse condition
            if (tokens[pos].type == T_RPAREN)
            {
                pos++; // consume ')'
                if (tokens[pos].type == T_LBRACE)
                {
                    pos++; // consume '{'
                    while (tokens[pos].type != T_RBRACE)
                    {
                        parseStatement();
                    }
                    pos++; // consume '}'
                }
                else
                {
                    cout << "Error: Expected '{' after if condition." << endl;
                    exit(1);
                }
            }
            else
            {
                cout << "Error: Expected ')' after if condition." << endl;
                exit(1);
            }
        }
        else
        {
            cout << "Error: Expected '(' after if." << endl;
            exit(1);
        }

        // Optional else handling
        if (tokens[pos].type == T_ELSE)
        {
            pos++; // consume 'else'
            if (tokens[pos].type == T_LBRACE)
            {
                pos++; // consume '{'
                while (tokens[pos].type != T_RBRACE)
                {
                    parseStatement();
                }
                pos++; // consume '}'
            }
            else
            {
                cout << "Error: Expected '{' after else." << endl;
                exit(1);
            }
        }
    }

    void parseWhileStatement()
    {
        pos++; // consume 'while'
        if (tokens[pos].type == T_LPAREN)
        {
            pos++;             // consume '('
            parseExpression(); // parse condition
            if (tokens[pos].type == T_RPAREN)
            {
                pos++; // consume ')'
                if (tokens[pos].type == T_LBRACE)
                {
                    pos++; // consume '{'
                    parseStatement();
                    if (tokens[pos].type == T_RBRACE)
                    {
                        pos++; // consume '}'
                    }
                    else
                    {
                        cout << "Error: Expected '}' after while statement." << endl;
                        exit(1);
                    }
                }
                else
                {
                    cout << "Error: Expected '{' after while condition." << endl;
                    exit(1);
                }
            }
            else
            {
                cout << "Error: Expected ')' after while condition." << endl;
                exit(1);
            }
        }
    }

    void parseReturnStatement()
    {
        pos++;             // consume 'return'
        parseExpression(); // parse return expression
        if (tokens[pos].type == T_SEMICOLON)
        {
            pos++;
        }
        else
        {
            cout << "Error: Expected semicolon after return statement." << endl;
            exit(1);
        }
    }

    void parseExpression()
    {
        cout << "Parsing expression starting at token: " << tokens[pos].value << endl; // Debug

        if (tokens[pos].type == T_ID || tokens[pos].type == T_NUM)
        {
            cout << "Found identifier/number: " << tokens[pos].value << endl; // Debug
            pos++;                                                            // consume identifier or number

            if (tokens[pos].type == T_GT || tokens[pos].type == T_LT || tokens[pos].type == T_ASSIGN ||
                tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS || tokens[pos].type == T_MUL ||
                tokens[pos].type == T_DIV)
            {
                cout << "Found operator: " << tokens[pos].value << endl; // Debug
                pos++;                                                   // consume operator

                if (tokens[pos].type == T_ID || tokens[pos].type == T_NUM)
                {
                    cout << "Found identifier/number after operator: " << tokens[pos].value << endl; // Debug
                    pos++;                                                                           // consume identifier or number
                }
                else
                {
                    cout << "Error: Expected identifier or number after operator." << endl;
                    exit(1);
                }
            }
        }
        else
        {
            cout << "Error: Expected identifier or number in expression." << endl;
            exit(1);
        }
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