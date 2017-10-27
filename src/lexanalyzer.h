#ifndef LEXANALYZER_H
#define LEXANALYZER_H
#include <string>
enum Symbol_type {
    digit, // 0..9
    letter, // a..z _
    comment, // / *
    comparison, // = < > !
    logic, // & |
    math, // + -
    separator, // , ; ( ) { } \n
    wrong_symb
};
enum States {
    end_st,
    wrong_st,
    empty_st,
    number_st,
    word_st,
    comparison_st, // == != > <
    logic_st, // && ||
    math_st, // ++ -- + - = !
    separator_st
};

enum Number_symbol_type {
    digit_num,
    dot_num,
    e_num,
    plus_num,
    minus_num
};

enum Number_states {
    empty_nst,
    minus_nst,
    dot_nst,
    int_nst,
    fraction_nst,
    e_nst,
    sign_nst,
    order_nst
};

enum Act{
    nil,
    number_act,
    end_act
};

enum Keys {
    if_key = 1,
    else_key,
    for_key,
    while_key,
    break_key,
    continue_key,
    bool_key,
    int_key,
    double_key,
    true_key,
    false_key
};

enum Lex_class {
    id_class,
    int_class,
    float_class,
    bool_class,
    logic_class
};

struct Keyword {
    Keyword(std::string word, Keys key);
    std::string word;
    Keys key;
};

struct Symbol {
    Symbol(char c);
    char ch;
    Symbol_type type;
};

struct State_act {
    State_act();
    State_act(int next, Act act);
    int next;
    Act act;
};

struct Lexem {
    std::string name;

};

struct State_machine {
    State_machine(int state_num,int smb_num);
    void add_branch(int old_state, int smb, int next_state, Act call_back);
    ~State_machine();
    int state_num;
    int smb_num;
    int current_state = 0;
    State_act** transitions;
};



int analyze(char* fname);
int recognize(Symbol& smb);
void init_state_machines();
void init_main_machine();
void init_number_machine();
void init_hash_table();
void add_hash(std::string word, Keys key);
void categorize(std::string str, char state);
int find_key_word(std::string word);
#endif
