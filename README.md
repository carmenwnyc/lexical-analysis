# Lexical-Analysis

## About Lexical Analyzer
A **lexical analyzer** (or **tokenizer**) converts the raw sequence of characters from the code into sequence of tokens. In order to do that, the lexical analyzer scans through the sequence of characters in the code, groups them together into lexemes, and identifies the token class for each lexeme. This project implements a basic lexical analyzer (implemented in C++) that scans streams of C code.

## Setup

1. Install `cmake` from [here](https://cmake.org/download/), or run `sudo apt-get install cmake` in your terminal/shell.
2. Run the `build.sh` script as follows: `bash build.sh`.


## Main Program
Lexical Analyzer is implemented in `src/Lexer.cpp`. Below are the token classes implemented.
* Note: Template in `src/Lexer.cpp` was originally provided by Saikat Chakraborty on 9/11/2020. Functions stateTransition() and tokenizeCode() written by Carmen Wu
* [ ] **Keywords**: any tokens from the list [_**if**_/_**else**_, _**for**_, _**while**_, _**extern**_, _**asm**_]
	* Token class: _KEYWORD_
* [ ] **Identifiers**: any tokens that begin with an **_alphabetic_** (including both capital and lowercase) character or an **_underscore_** (_), followed by at most 16 alphanumeric characters and/or underscore (EXCEPT for the keyword tokens)
	* Examples of valid identifiers: **test**, **test1**, **_id1**, and **test_1_id_2**
	* Token class: _ID_
* [ ] **Numbers**: any numerical tokens optionally containing a **_decimal point/period_** (.), _i.e._, both integers and floating-point numbers
    * Examples of valid numbers: **1**, **1.0**, **1.01**, and **.01**
    * Token class: _NUMBER_
* [ ] **Strings**: any tokens represented by a sequence of characters (including the empty sequence) that begins and ends with double quotes ("). You are **not** required to handle escape characters like `\"`.
	* Examples of strings: **"Hello"**, **""**, and **"1.01"**
	* Token class: _STRING_
* [ ] **Comments **: any tokens represented by a sequence of characters beginning with a double slash (**//**) and that ends with a newline (**\n**)
	* Examples of comments: **//Hello\n**, **//""\n**, and **//"1.01"\n**
	* Token class: _COMMENT_
* [ ] **Operators **: any tokens represented by a unary operator, binary operator, or an assignment operator, specifically from the list [_**++**_,&nbsp;_**--**_,&nbsp;_**>**_,&nbsp;_**<**_,&nbsp;_**>=**_,&nbsp;_**<=**_,&nbsp;_**==**_,&nbsp;_**=**_]
	* Token classes: _UNOP_, _BINOP_, _ASSIGNOP_
* [ ] **Parentheses, Braces, and Semicolons **: any tokens from the list [_**(**_,&nbsp;_**)**_,&nbsp;_**{**_,&nbsp;_**}**_,&nbsp;_**;**_]
	* Token classes: _LPAR_, _RPAR_, _LBRACE_, _RBRACE_, _SEMICOLON_
	
### Example

**Input:** 
```
if(n > 0) {
    print("Hello World");
}
```
**Expected Output:**
```        
<KEYWORD, if>           
<LPAR, (>
<ID, n>
<BINOP, >>
<NUMBER, 0> 
<RPAR, )>    
<L-CURLY-BRACE, {>
<ID, print>
<LPAR, (>
<STRING, "Hello World">
<RPAR, )>
<SEMICOLON, ;> 
<R-CURLY-BRACE, }>
```
**Note:** Print such representation of token sequences using [`printTokenStream`](src/Main.cpp#8) function in `src/Main.cpp` file. 

