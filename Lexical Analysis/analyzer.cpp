#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Forward declarations
void checkToken();
void checkSingleToken();
void tokensCalculation();
void symbolTableCalculation();
bool checkKeyword(const std::string& str);
bool checkOperator(const std::string& str);
bool checkSymbol(const std::string& str);
bool isNumber(const std::string& str);

// Use unordered_set for O(1) lookup instead of arrays
const std::unordered_set<std::string> keywords = {
    "bool", "char", "int", "long", "float", "double", "void", "return", 
    "if", "else", "while", "for", "break", "switch", "case", "cin", "cout", "string"
};

const std::unordered_set<std::string> operators = {
    "+", "-", "*", "/", "++", "--", "=", "+=", "-=", "*=", "/=", 
    "==", "!=", ">", "<", ">=", "<=", "&&", "||", "<<", ">>"
};

const std::unordered_set<std::string> symbols = {
    "(", ")", "{", "}", ";", ","
};

// Operator attribute mapping
const std::unordered_map<std::string, std::string> operatorAttributes = {
    {"+", "addition"}, {"-", "subtraction"}, {"*", "multiplication"}, {"/", "division"},
    {"++", "increment"}, {"--", "decrement"}, {"=", "assignment"},
    {"+=", "addition and assignment"}, {"-=", "subtraction and assignment"},
    {"*=", "multiplication and assignment"}, {"/=", "division and assignment"},
    {"==", "Relation Operator"}, {"!=", "Relation Operator"}, {">", "Relation Operator"},
    {"<", "Relation Operator"}, {">=", "Relation Operator"}, {"<=", "Relation Operator"},
    {"&&", "Relation Operator"}, {"||", "Relation Operator"},
    {"<<", "Stream extraction operator"}, {">>", "Stream insertion operator"}
};

// Symbol attribute mapping
const std::unordered_map<std::string, std::string> symbolAttributes = {
    {"(", "opening braces"}, {")", "closing braces"},
    {"{", "left curly braces"}, {"}", "right curly braces"},
    {";", "semicolon"}, {",", "comma"}
};

// Global variables
std::string program;
std::string lexeme;
std::vector<std::string> lexemes;
std::vector<std::string> tokenName;
std::vector<std::string> attributeValue;
std::vector<std::string> symbolTableSymbol;
std::vector<std::string> symbolTableDataType;

int lexemeFound = 0;
std::string tempLexeme[2];
std::string tempId;
std::string singleCharLexeme;
bool flag = false;

int main() {
    std::cout << "Source Program:" << std::endl;
    
    // Input processing with comment removal
    std::string x;
    while (std::getline(std::cin, x)) {
        if (x == "eof") break;
        
        size_t commentIndex = x.find("//");
        size_t commentStartIndex = x.find("/*");
        
        // Handle // comments
        if (commentIndex != std::string::npos) {
            program += x.substr(0, commentIndex);
        }
        // Handle /* */ comments
        else if (commentStartIndex != std::string::npos) {
            program += x.substr(0, commentStartIndex);
            size_t commentEndIndex = x.find("*/", commentStartIndex);
            
            if (commentEndIndex != std::string::npos) {
                program += x.substr(commentEndIndex + 2);
            } else {
                std::cout << "Compile Error!";
                return 1;
            }
        }
        // No comment
        else {
            program += x;
        }
        program += " ";
    }
    
    // Token calculation
    for (size_t i = 0; i < program.length(); i++) {
        // Skip extra spaces
        if (program[i] == ' ') {
            while (i < program.length() && program[i] == ' ') {
                i++;
            }
            i--;
        } else {
            singleCharLexeme = program[i];
            lexeme += program[i];
        }
        
        // Check for identifier separation
        if ((checkKeyword(singleCharLexeme) || checkOperator(singleCharLexeme) || 
             checkSymbol(singleCharLexeme)) && lexeme.size() > 1) {
            tempId = lexeme.substr(0, lexeme.size() - 1);
            flag = true;
            i--;
            lexeme = tempId;
            tempId.clear();
        }
        
        singleCharLexeme.clear();
        
        // Process lexeme if found
        if (checkKeyword(lexeme) || checkOperator(lexeme) || checkSymbol(lexeme) || flag) {
            flag = false;
            
            if (lexemeFound < 2) {
                tempLexeme[lexemeFound] = lexeme;
                lexemeFound++;
            } else {
                checkToken();
            }
            lexeme.clear();
        }
    }
    
    // Process remaining lexemes
    if (lexemeFound > 0) {
        if (lexemeFound == 1) {
            checkSingleToken();
        } else if (lexemeFound == 2) {
            checkToken();
            checkSingleToken();
        }
    }
    
    symbolTableCalculation();
    tokensCalculation();
    
    // Output
    std::cout << "\n\nTokens:" << std::endl;
    std::cout << "\nLexemes-Token Name-Attribute Value\n" << std::endl;
    for (size_t i = 0; i < lexemes.size(); i++) {
        std::cout << lexemes[i] << " - " << tokenName[i] << " - " << attributeValue[i] << std::endl;
    }
    
    std::cout << "\n\nSymbol Table:" << std::endl;
    std::cout << "\nSymbol-Token-DataType-Pointer to symbol table entry\n" << std::endl;
    for (size_t i = 0; i < symbolTableSymbol.size(); i++) {
        std::cout << symbolTableSymbol[i] << " - id - " << symbolTableDataType[i] 
                  << " - " << i << std::endl;
    }
    
    return 0;
}

void checkToken() {
    std::string combined = tempLexeme[0] + tempLexeme[1];
    
    // Check combined lexeme
    if (checkKeyword(combined)) {
        lexemes.push_back(combined);
        tokenName.push_back("keyword");
        lexemeFound = 1;
        tempLexeme[0] = lexeme;
    } else if (checkOperator(combined)) {
        lexemes.push_back(combined);
        tokenName.push_back("operator");
        lexemeFound = 1;
        tempLexeme[0] = lexeme;
    } else if (checkSymbol(combined)) {
        lexemes.push_back(combined);
        tokenName.push_back("special symbol");
        lexemeFound = 1;
        tempLexeme[0] = lexeme;
    }
    // Check first lexeme alone
    else if (checkKeyword(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("keyword");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    } else if (checkOperator(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("operator");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    } else if (checkSymbol(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("special symbol");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    } else if (isNumber(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("number");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    } else {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("id");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    }
}

void checkSingleToken() {
    if (checkKeyword(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("keyword");
    } else if (checkOperator(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("operator");
    } else if (checkSymbol(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("special symbol");
    } else if (isNumber(tempLexeme[0])) {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("number");
    } else {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("id");
    }
}

bool checkKeyword(const std::string& str) {
    return keywords.find(str) != keywords.end();
}

bool checkOperator(const std::string& str) {
    return operators.find(str) != operators.end();
}

bool checkSymbol(const std::string& str) {
    return symbols.find(str) != symbols.end();
}

bool isNumber(const std::string& str) {
    if (str.empty()) return false;
    
    for (char c : str) {
        if (!std::isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

void symbolTableCalculation() {
    for (size_t i = 0; i < lexemes.size(); i++) {
        if (tokenName[i] == "id") {
            // Check if already in symbol table
            bool found = false;
            for (const auto& sym : symbolTableSymbol) {
                if (lexemes[i] == sym) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                symbolTableSymbol.push_back(lexemes[i]);
                
                // Determine data type
                if (i + 1 < lexemes.size() && i > 0) {
                    if (lexemes[i + 1] != "(" && tokenName[i - 1] == "keyword") {
                        symbolTableDataType.push_back(lexemes[i - 1]);
                    } else if (lexemes[i - 1] == ",") {
                        symbolTableDataType.push_back(symbolTableDataType.back());
                    } else {
                        symbolTableDataType.push_back("_");
                    }
                } else {
                    symbolTableDataType.push_back("_");
                }
            }
        }
    }
}

void tokensCalculation() {
    for (size_t i = 0; i < lexemes.size(); i++) {
        if (tokenName[i] == "keyword") {
            tokenName[i] = lexemes[i];
            attributeValue.push_back(" - ");
        } else if (tokenName[i] == "number") {
            attributeValue.push_back("constant");
        } else if (tokenName[i] == "id") {
            if (i + 1 < lexemes.size() && lexemes[i + 1] != "(") {
                attributeValue.push_back("pointer to symbol table entry");
            } else {
                attributeValue.push_back(" - ");
            }
        } else if (tokenName[i] == "operator") {
            auto it = operatorAttributes.find(lexemes[i]);
            if (it != operatorAttributes.end()) {
                attributeValue.push_back(it->second);
            }
        } else if (tokenName[i] == "special symbol") {
            auto it = symbolAttributes.find(lexemes[i]);
            if (it != symbolAttributes.end()) {
                attributeValue.push_back(it->second);
            }
        }
    }
}
