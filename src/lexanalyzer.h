#ifndef LEXANALYZER_H
#define LEXANALYZER_H
enum Symbol_type{
    digit, // 0..9
    letter, // a..z _
    comment, // / *
    comparison, // = < > !
    logic, // & |
    math, // + -
    separator, // , ; ( ) { } \n
    wrong_symb
};
enum States{
    empty_st,
    number_st,
    word_st,
    comparison_st, // == != > <
    logic_st, // && ||
    math_st, // ++ -- + - = !
    separator_st,
    end_st,
    wrong_st
};
struct Symbol {
    Symbol(char c);
    char ch;
    Symbol_type type;
};


int analyze(char* fname);
int recognize(Symbol& smb);
#endif
