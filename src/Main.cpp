#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include "Lexer.h"


void printTokenStream(std::vector<Token> tokens) {
    for (auto& token : tokens) {
        std::cout << token.get_repr() << std::endl;
    }
}

bool tokenMatch(std::vector<Token> generated_tokens, std::vector<Token> expected_tokens){
    int gen_size = generated_tokens.size();
    int expected_size = expected_tokens.size();
    if (gen_size == expected_size){
        for(int i = 0; i < gen_size; i++){
            if (generated_tokens[i] != expected_tokens[i])
                return false;
        }
        return true;
    }
    return false;
}
/*
int main() {
    std::string input = "STR = \"this is string.\";";
    std::vector<Token> expected_tokens(
            {Token(type_identifier, "STR"), Token(type_assign_op, "="),
             Token(type_string, "\"this is string.\""), Token(type_semicolon, ";")});
    std::vector<Token> generated_tokens = tokenizeCode(input);
    printTokenStream(generated_tokens);
    std::cout << tokenMatch(generated_tokens, expected_tokens);
    return 0;

}
*/
int main() {
    std::string input = "//this is a test case\nif(n > 0) {\nprint(\"Hello World\");\n}";
    std::vector<Token> expected_tokens(
            {Token(type_keyword, "else"), Token(type_lpar, "("), Token(type_rpar, ")"),
             });
    std::vector<Token> generated_tokens = tokenizeCode(input);
    printTokenStream(generated_tokens);
    std::cout << tokenMatch(generated_tokens, expected_tokens);
    return 0;

/*
for(int a=0;a<=3;a++)\n{\nextern a+1;\n}

//this is my comment\nfor(int a=0;a<=0.9;a++)\n{\nextern a;\n}

//this is a test case\nif(n > 0) {\nprint(\"Hello World\");\n}

else if(next_state == \"++\"){\nemplace_back(type_unaryop, \"luca val\")}
*/


}