//
// Created by Saikat Chakraborty on 9/11/20.
//
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>    // std::find
#include "Lexer.h"
#include<stdio.h>
#include<ctype.h> //isalpha()

//create global variables for whitespace_list and stop_list
char whitespace_array [] = {' ','\t','\n'};
char stoplist_array [] = {' ','\t','\n','{','(','}',')'};
char num_stoplist_array [] = {' ','\t','\n',';','+','-','=',')','}'}; //stop list specificlly for numbers
char keyword_stoplist_array [] = {' ','\t','\n','(','{'}; //stop list specificlly for potential keywords
char id_stoplist_array [] = {' ','\t','\n',';',',','+','-','*','=','<','>','(',')','.'}; //stop list specificlly for potential keywords
char num_error_array [] = {'_','{','('};
char str_stop_array [] = {';',')',',',' ','\t','\n'};

std::list<char> whitespace_list (whitespace_array,whitespace_array + sizeof(whitespace_array) / sizeof(char));
std::list<char> stop_list (stoplist_array, stoplist_array + sizeof(stoplist_array) / sizeof(char));
std::list<std::string> keyword_list = {"if","while","for","else","extern","asm"};
std::list<char> num_stoplist (num_stoplist_array, num_stoplist_array + sizeof(num_stoplist_array) / sizeof(char));
std::list<char> keyword_stoplist (keyword_stoplist_array, keyword_stoplist_array + sizeof(keyword_stoplist_array) / sizeof(char));
std::list<char> id_stoplist (id_stoplist_array, id_stoplist_array + sizeof(id_stoplist_array) / sizeof(char));
std::list<char> num_error_list (num_error_array, num_error_array + sizeof(num_error_array) / sizeof(char));
std::list<char> str_stop_list (str_stop_array, str_stop_array + sizeof(str_stop_array) / sizeof(char));

//global variable buf representing a buffer when scanning through characters
char buf[1024] = {0}; //initialize buffer to null

//global variable representing a pointer pointing where the latest scanned 
//char is located in the buffer
int pt = 0;

bool token_accepted = false;

//function inList() to check whether target is included in list
template <class T> bool inList(std::list<T> list, T target){
    //std::_List_iterator<T> it = std::find(list.begin(),list.end(),target);
    auto it = std::find(list.begin(),list.end(),target);
    if (it != list.end())
    {
        return true;
    }else{
        return false;
    }
}

//function strAlpha() to check whether an entire string is alphabet only
bool strAlpha(std::string &target){
    for (char &c : target)
    {
        if (!isalpha(c))
        {
            return false;
        }
    }
    return true;
}

/*
//look_ahead function to look ahead by 1 position
std::string look_ahead(char ch){
    if (inList(whitespace_list,ch))
    {
        token_accepted = true;
        return "";
    }
    else
    {
        //restore buffer and pointer to original 
        for (int i=0; i<pt; i++){
            buf[i] = 0;
        }
        pt = 0;
        buf[0] = ch;
        return stateTransition("", ch); 
    }
}
*/

/*
        if (look_ahead == true)
        {
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                pt = 0;
                buf[0] = current_character;
                next_state = stateTransition("", current_character);
            }
            look_ahead = false;
        }
*/
/*
 * We simulate the state transitions with a DFA here.
 * The next state should be constructed based on the current state and the
 * input character.
 * Gotcha note!
 *   1. Be careful about whitespaces.
 *      Whitespaces usually finish a token, except for strings and comments.
 *      For example, `hello world ` contains 2 identifier tokens, each terminated
 *      with a whitespace.
 *      However, `"Hello world "` contains 1 string token, and both spaces are
 *      included in the token lexeme.
 *   2. Whitespaces (i.e. ' ', '\t', '\n') usually end sequences of characters
 *      in a token. However, other characters can also end a token stream.
 *      For example,
 *        a. print ( ) -> the tokens here are <ID, "print">, <LPAR, "(">, and
 *           <RPAR, ")">
 *        b. print() -> the tokens here are also <ID, "print">, <LPAR, "(">, and
 *           <RPAR, ")">
 *      Note that, in the former case, the space, ' ', after the token print
 *      ended the "print" token.
 *      However, in the latter case, "print" is followed by a left parenthesis,
 *      '(', which ends the "print" token.
 *      Your code should be able to handle both these scenarios.
 *   3. For comments, you should discard the newline character ('\n') that ends
 *      the comments. See LexerTest.cpp for the relevant test case.
 */
std::string stateTransition(std::string current_state, char ch) {
    /*
    if (current_state.empty()) {
        // When the state is empty, the input character will represent the
        // current state.
        current_state.push_back(ch);
        return current_state;
    }
    */
    // TODO: implement the rest of the state transitions.
    /*
    //keyword: if
    else if (current_state == "i" && ch == 'f')
    {
        return "if";
    }
    */

    //handle whitespaces at the beginning
    //eg.     extern
    if (current_state == "" && inList(whitespace_list,ch))
    {
        //std::cout << "return whitespace \n";
        return "whitespace";
    }

    //next states for comments
    if (current_state == "" && ch == '/')
    {
        return "comment_1_slash";
    }
    else if (current_state == "comment_1_slash" && ch == '/')
    {
        return "comment_continue";
    }
    else if (current_state == "comment_continue" && ch != '\n' )
    {
        return "comment_continue";
    }
    else if (current_state == "comment_continue" && ch == '\n' )
    {
        return "comment";
    }
    //next states for strings
    else if (current_state == "" && ch == '"') 
    {
        return "string_continue";
    }
    else if (current_state == "string_continue" && ch !='"' && ch != '\n')
    {
        return "string_continue";
    }
    else if (current_state == "string_continue" && ch == '"')
    {
        return "string_pending";
    }
    else if (current_state == "string_pending" && !inList(str_stop_list,ch))
    {
        return "str_error_pending";
    }
    else if (current_state == "str_error_pending" && !inList(str_stop_list,ch))
    {
        return "str_error_pending";
    }
    else if (current_state == "str_error_pending" && inList(str_stop_list,ch))
    {
        return "str_error";
    }
    else if (current_state == "string_pending" && inList(str_stop_list,ch))
    {
        return "string";
    }
    else if (current_state == "string_continue" && ch == '\n')
    {
        return "str_error";
    }

    //next states for numbers:
    else if (current_state == "" && ch == '.')
    {
        return "num_decimal_last";
    }
    else if (current_state == "" && std::isdigit(ch))
    {
        return "num_no_decimal";
    }
    else if (current_state == "num_no_decimal" && std::isdigit(ch))
    {
        return "num_no_decimal";
    }
    else if (current_state == "num_no_decimal" && ch == '.')
    {
        return "num_decimal_last";
    }
    else if (current_state == "num_no_decimal" && inList(num_stoplist,ch))
    {
        return "number";
    }
    else if (current_state == "num_decimal_last" && std::isdigit(ch))
    {
        return "num_decimal";
    }
    else if (current_state == "num_decimal" && std::isdigit(ch))
    {
        //std::cout << "2. return num_decimal\n";
        return "num_decimal";
    }
    else if (current_state == "num_decimal" && inList(num_stoplist,ch))
    {
        //std::cout << "1. return number\n";
        return "number";
    }

    //errors about numbers
    else if (current_state == "num_no_decimal" && inList(num_error_list,ch))
    {
        //std::cout << "error after num_no_decimal\n";
        return "error_number_pending";
    }
    else if (current_state == "num_no_decimal" && std::isalpha(ch))
    {
        //std::cout << "error after num_no_decimal\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal_last" && ch == '.')
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal_last" && std::isalpha(ch))
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal_last" && inList(num_error_list,ch))
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal_last" && inList(num_stoplist,ch))
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal" && std::isalpha(ch)) 
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal" && ch == '.') 
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "num_decimal" && inList(num_error_list,ch)) 
    {
        //std::cout << "error after num_decimal_last\n";
        return "error_number_pending";
    }
    else if (current_state == "error_number_pending" && !inList(num_stoplist,ch))
    {
        return "error_number_pending";
    }
    else if (current_state == "error_number_pending" && inList(num_stoplist,ch))
    {
        //std::cout << "stop list after error number \n";
        return "error_number";
    }


    //next states for identifiers and keywords
    else if (current_state == "" && std::isalpha(ch))
    {
        //std::cout << "return 1. maybe_id_alpha \n";
        return "maybe_id_alpha";
    }
    else if (current_state == "" && ch == '_')
    {
        //std::cout << "return 2. maybe_id \n";
        return "maybe_id";
    }
    else if (current_state == "maybe_id_alpha" && (std::isdigit(ch) || ch == '_'))
    {
        //std::cout << "return 3. maybe_id \n";
        return "maybe_id";
    }
    else if (current_state == "maybe_id" && (std::isdigit(ch) || ch == '_' || std::isalpha(ch)))
    {
        //std::cout << "return 4. maybe_id \n";
        return "maybe_id";
    }
    else if (current_state == "maybe_id_alpha" && std::isalpha(ch))
    {
        //std::cout << "return 5. maybe_id_keyword \n";
        return "maybe_id_keyword";
    }
    else if (current_state == "maybe_id_keyword" && (std::isdigit(ch) || ch == '_'))
    {
        //std::cout << "return 6. maybe_id \n";
        return "maybe_id";
    }
    else if (current_state == "keyword_pending" && inList(keyword_stoplist,ch))
    {
        //std::cout << "return 7. keyword \n";
        return "keyword";
    }
    else if (current_state == "keyword_pending" && std::isalpha(ch))
    {
        //std::cout << "return 8. maybe_id_keyword \n";
        return "maybe_id_keyword";
    }
    else if (current_state == "keyword_pending" && (std::isdigit(ch) || ch == '_'))
    {
        //std::cout << "return 9. maybe_id \n";
        return "maybe_id";
    }
    else if (current_state == "maybe_id" && inList(id_stoplist,ch))
    {
        //std::cout << "return 10. identifier \n";
        return "identifier";
    }
    else if (current_state == "maybe_id_alpha" && inList(id_stoplist,ch))
    {
        //std::cout << "return 11. identifier \n";
        return "identifier";
    }

    //states for operators
    else if (current_state == "" && ch == '<')
    {
        return "< pending";
    }
    else if (current_state == "< pending" && ch == '=')
    {
        return "<=";
    }
    else if (current_state == "< pending" && ch != '=')
    {
        return "<";
    }

    else if (current_state == "" && ch == '>')
    {
        return "> pending";
    }
    else if (current_state == "> pending" && ch == '=')
    {
        return ">=";
    }
    else if (current_state == "> pending" && ch != '=')
    {
        return ">";
    }
    else if (current_state == "=" && ch == '=')
    {
        //std::cout <<" return ==" <<"\n";
        return "==";
    }
    else if (current_state == "" && ch == '+')
    {
        return "+ pending";
    }
    else if (current_state == "+ pending" && ch == '+')
    {
        return "++";
    }
    else if (current_state == "+ pending" && ch != '+')
    {
        return "+";
    }
    else if (current_state == "" && ch == '-')
    {
        return "- pending";
    }
    else if (current_state == "- pending" && ch == '-')
    {
        return "--";
    }  
    else if (current_state == "- pending" && ch != '-')
    {
        return "-";
    } 
    else if (current_state == "" && ch == '=')
    {
        //std::cout << "return =pending\n"; 
        return "= pending";
    }
    else if (current_state == "= pending" && ch == '=')
    {
        return "==";
    }
    else if (current_state == "= pending" && ch != '=')
    {
        //std::cout << "return =\n";
        return "=";
    }
    else if (current_state.empty()) {
        // When the state is empty, the input character will represent the
        // current state.
        current_state.push_back(ch);
        return current_state;
    }

    else {
        return "Not implemented error!";
    }
}

/***
 * The main function for lexical analysis/tokenization.
 *
 * @param   _character_stream of type std::string
 * @return  std::vector of type Token (declared above)
 *
 */

//loop count:
int loop_count=0;
std::vector<Token> tokenizeCode(std::string _character_stream) {
    std::vector<Token> tokens;
    /*
     * Note to the students:
     *   You should not make any changes to the function prototype (i.e., 
     *   function name, parameter, and return type). Any such changes cause the
     *   test suite to fail.
     *   You may define auxiliary/helper functions, which can then be called
     *   from this function.
     */

    //append "\n" to input stream so that we could implement cases where an EOF immediately follows data
    //such as "if", "n=1234";
    _character_stream.push_back('\n'); 
    // Initially, we start with a null or empty state.
    std::string current_state = ""; 
    std::string next_state = "";
    //bool token_accepted = false; --> move it to become a global variable
    // We scan character by character.
    for (std::string::iterator string_iter = _character_stream.begin();
         string_iter != _character_stream.end();
         string_iter++) {
        char current_character = *string_iter;
        //copy the scanned char into buffer
        buf[pt]=current_character;
        pt++; 

        // std::cout << "loop count is " << loop_count << "\n";
        // std::cout << "current ch: " << current_character << "\n";
        // std::cout << "pointer is at " << pt << "\n";
        // std::cout << "current state is " << current_state << "\n";
        
        next_state = stateTransition(current_state,current_character);
        //to identify comments 
        if (next_state == "comment")
        {
            buf[pt-1]=0;//exclude the \n at the end of the comment
            tokens.emplace_back(TokenType::type_comment, std::string(buf));
            token_accepted = true;
        }

        //identify string
        if (next_state == "string")
        {
            buf[pt-1] = 0;
            tokens.emplace_back(TokenType::type_string, std::string(buf));

            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            } 
        }

        //identify string errors
        if (next_state == "str_error")
        {
            buf[pt-1] = 0;
            //std::cout<< buf << " is invalid string\n";
            tokens.emplace_back(TokenType::type_error, "Invalid String");

            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            } 
        }

        //identify numbers
        if (next_state == "number")
        {   
            buf[pt-1] = 0;
            tokens.emplace_back(TokenType::type_number, std::string(buf));

            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }       
        }

        //identify identifiers or keywords
        if (next_state == "maybe_id_keyword")
        {
            if (inList(keyword_list,std::string(buf)))
            {
                next_state = "keyword_pending";
            }else{
                next_state = "maybe_id_alpha";
            }
        }

        //identify identifiers or keywords
        if (next_state == "keyword" || next_state == "identifier")
        {
            buf[pt-1] = 0;

            if (next_state == "keyword" )
            {
                tokens.emplace_back(TokenType::type_keyword, std::string(buf));
            }
            if (next_state == "identifier")
            {
                tokens.emplace_back(TokenType::type_identifier, std::string(buf));
            }

            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }   
        }


        //identify number errors
        if (next_state == "error_number")
        {
            buf[pt-1] = 0;
            tokens.emplace_back(TokenType::type_error, "Invalid Number");
            //std::cout<< buf << " is not a valid number\n";

            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            } 
        }

        //ignore whitespace; restore everything to original if there is whitespace; ready for next parse
        if (next_state == "whitespace")
        {
            token_accepted = true;
        }

        //!!!!!next_state = stateTransition(current_state, current_character);
        //std::cout << "next state: " << next_state << "\n";
        if (next_state == "(") {
            tokens.emplace_back(TokenType::type_lpar, "(");
            token_accepted = true;
        }
        else if (next_state == ")") {
            tokens.emplace_back(TokenType::type_rpar, ")");
            token_accepted = true;
        }
        else if (next_state == "{") {
            tokens.emplace_back(TokenType::type_lcurly_brace, "{");
            token_accepted = true;
        }
        else if (next_state == "}") {
            tokens.emplace_back(TokenType::type_rcurly_brace, "}");
            token_accepted = true;
        }
        else if(next_state == "++"){
            tokens.emplace_back(TokenType::type_unaryop, "++");
            token_accepted = true;
        }
        else if(next_state == "--"){
            tokens.emplace_back(TokenType::type_unaryop, "--");
            token_accepted = true;
        }
        else if(next_state == "=="){
            tokens.emplace_back(TokenType::type_unaryop, "==");
            token_accepted = true;
        }                
        else if(next_state == ">="){
            tokens.emplace_back(TokenType::type_binop, ">=");
            token_accepted = true;
        }
        else if(next_state == ">"){
            tokens.emplace_back(TokenType::type_binop, ">");
            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }   
        }        
        else if(next_state == "<="){
            tokens.emplace_back(TokenType::type_binop, "<=");
            token_accepted = true;
        }
        else if(next_state == "<"){
            tokens.emplace_back(TokenType::type_binop, "<");
            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }   
        }        
        else if(next_state == ";"){
            tokens.emplace_back(TokenType::type_semicolon, ";");
            token_accepted = true;
        }
        else if(next_state == "="){
            tokens.emplace_back(TokenType::type_assign_op, "=");
            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }   
        }
        else if(next_state == "+"){
            tokens.emplace_back(TokenType::type_binop, "+");
            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
                next_state = stateTransition("", current_character);
            }
        }
        else if(next_state == "-"){
            tokens.emplace_back(TokenType::type_binop, "-");
            // look ahead codes (look ahead by one position)
            if (inList(whitespace_list,current_character))
            {
                token_accepted = true;
            }
            else
            {
                //restore buffer and pointer to original 
                for (int i=0; i<pt; i++){
                    buf[i] = 0;
                }
                buf[0] = current_character;
                pt = 1;
            }
        }

        else {
            /* TODO: 
             *   Add the rest of the tokens. Make sure to flag the 'token_accepted'
             *   as true when you accept a token.
             * 
             *   a. Implement the rest of the keywords, i.e., else, while, 
             *      extern, asm, for, ...
             *   b. Implement dentifiers.
             *   c. Implement numbers.
             *   d. Implement strings.
             *   e. Implement comments.
             *   f. [Extra Credit] Implement a lexical error check. In the case
             *      of an error, the cause of the error ("Invalid Number" or 
             *      "Invalid String") should go to the lexeme.
             */

        }
        // If we have already accepted a token, we will start from an empty state.


        if (token_accepted){
            current_state = "";
            next_state = "";

            //restore buffer and pointer back to initial state
            for (int i=0; i<pt; i++){
                buf[i] = 0;
            }
            pt = 0;

            token_accepted = false;
        }
        else{
            // No token has been accepted by this character. Let's proceed!
            current_state = next_state;
        }

        loop_count++;
    }//end of for loop

    return tokens;
}

TokenType Token::getType() {
    return this->type;
}

std::string Token::getLexeme() {
    return this->lexeme;
}

bool Token::operator==(const Token& other) const{
    return this->type == other.type && this->lexeme == other.lexeme;
}

bool Token::operator!=(const Token& other) const{
    return type != other.type || lexeme != other.lexeme;
}

std::string Token::get_repr()
{
    std::string return_str = "<" + get_token_type_string(this->type) + ", " + 
                             this->lexeme + ">";
    return return_str;
}

Token::Token(TokenType _type, std::string _lexeme) {
    this->type = _type;
    this->lexeme = std::move(_lexeme);
}

std::string get_token_type_string(TokenType _type) {
    switch (_type) {
    case type_lpar:
        return "LPAR";
    case type_rpar:
        return "RPAR";
    case type_lcurly_brace:
        return "L-CURLY-BRACE";
    case type_rcurly_brace:
        return "R-CURLY-BRACE";
    case type_keyword:
        return "KEYWORD";
    case type_identifier:
        return "ID";
    case type_number:
        return "NUMBER";
    case type_string:
        return "STRING";
    case type_comment:
        return "COMMENT";
    case type_error:
        return "ERROR";
    case type_unaryop:
        return "UNOP";
    case type_binop:
        return "BINOP";
    case type_semicolon:
        return "SEMICOLON";
    case type_assign_op:
        return "ASSIGN";
    default:
        return "UNKNOWN";
    }
}
