#ifndef LEXANALYZER_H
#define LEXANALYZER_H
#include <string>
#include <vector>
enum Symbol_type {
    digit, // 0..9
    letter, // a..z _
    comment, // / *
    comparison, // < >
    logic, // & |
    math, // + - = !
    separator, // , ; ( ) { } \n
    wrong_symb
};
enum States {
    end_st,
    wrong_st,
    comment_st,
    empty_st,
    number_st,
    number_e_st,
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
    arifm_num
};

enum Number_states {
    empty_nst,
    dot_nst,
    int_nst,
    fraction_nst,
    e_nst,
    sign_nst,
    order_nst
};

enum Operation_symbol_type {
    plus_op, // +
    minus_op, // -
    exp_op, // !
    equal_op, // =
    comp_op, // < >
    slash_op, // /
    star_op, // *
    ampersand_op, // &
    stick_op // |
};

enum Operation_state {
    empty_ost,
    assign_ost,
    plus_ost,
    minus_ost,
    plus_plus_ost,
    minus_minus_ost,
    exp_sign_ost,
    one_sign_comp_ost,
    two_sign_comp_ost,
    ampersand_ost,
    stick_ost,
    two_ampersand_ost,
    two_stick_ost,
    slash_ost,
    two_slash_ost,
    star_ost,
    comment_open_ost,
    comment_close_ost;

};

enum Act{
    nil,
    number_act,
    operation_act,
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
    true_key = 101,
    false_key
};

enum Token_type {
    space_type,
    wrong_type,
    id_type,
    reserved_type,
    assign_type,
    unary_type,
    arifm_op_type,
    logic_op_type,
    comp_op_type,
    separator_type,
    int_type,
    float_type,
    bool_type
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
    //void add_non_final_state(int state);
    ~State_machine();
    int state_num;
    int smb_num;
    int current_state = 0;
    State_act** transitions;
};



struct Lex_attributes {
    Lex_attributes(int s_num, std::string token, Token_type token_type);
    int s_num;
    std::string token;
    Token_type token_type;


};

struct State {
    char main_state = empty_st;
    char num_state = empty_nst;
    char op_state = empty_ost;
    int s_num = 1;
};



int analyze(char* fname);
int recognize(Symbol& smb, std::vector<Lex_attributes> &recognized_lexs);
int recognize_num(Symbol smb, State& current_state);
int recognize_op(Symbol smb, State& current_state);
Number_symbol_type get_num_symol_type(char ch);
Operation_symbol_type get_op_symol_type(char ch);
void init_state_machines();
void init_main_machine();
void init_number_machine();
void init_operation_machine();
void init_hash_table();
void add_hash(std::string word, Keys key);
Token_type categorize(std::string str, State& state);
int find_key_word(std::string word);
#endif
