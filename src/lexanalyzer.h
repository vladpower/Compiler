#ifndef LEXANALYZER_H
#define LEXANALYZER_H
#include <string>
#include <vector>
using namespace std;
enum Symbol_type {
    digit, // 0..9
    letter, // a..z _
    comment, // / *
    comparison, // < >
    logic, // & |
    math, // + - = !
    separator, // , ; ( ) { } \n
    wrong_symb,
    number_of_symtype
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
    separator_st,
    number_of_states
};

enum Number_symbol_type {
    digit_num,
    dot_num,
    e_num,
    arifm_num,
    number_of_numsymtype
};

enum Number_states {
    empty_nst,
    dot_nst,
    int_nst,
    fraction_nst,
    e_nst,
    sign_nst,
    order_nst,
    number_of_numstates
};

enum comment_states {
    no_comment,
    line_comment,
    multiple_comment
};

enum Operation_symbol_type {
    plus_op, // +
    minus_op, // -
    exclamation_op, // !
    equal_op, // =
    comp_op, // < >
    slash_op, // /
    asterisk_op, // *
    ampersand_op, // &
    stick_op, // |
    number_of_opsymtype
};

enum Operation_state {
    empty_ost,
    assign_ost,
    plus_ost,
    minus_ost,
    plus_plus_ost,
    minus_minus_ost,
    exclamation_ost,
    one_sign_comp_ost,
    two_sign_comp_ost,
    ampersand_ost,
    stick_ost,
    two_ampersand_ost,
    two_stick_ost,
    slash_ost,
    two_slash_ost,
    asterisk_ost,
    comment_open_ost,
    comment_close_ost,
    number_of_opstates

};

enum Act{
    nil,
    number_act,
    operation_act,
    resword_act,
    separator_act,
    shift_act,
    reduce_act,
    wrong_act,
    end_act
};

enum Keys {
    if_key,
    else_key,
    for_key,
    while_key,
    break_key,
    continue_key,
    bool_key,
    int_key,
    double_key,
    false_key,
    true_key,
    number_of_keys

};

enum Token_type {
    space_type,
    wrong_type,
    id_type,
    reserved_type,
    assign_type, // =
    unary_type, // ! -- ++
    arifm_op_type, // + -
    logic_op_type, // && ||
    comp_op_type, // < > == != <= >=
    separator_type,
    int_type,
    float_type,
    bool_type,
    end_type,
    number_of_tokens
};

enum Num_separator {
    comma, // ,
    semicolon, // ;
    left_parenthesis, // (
    right_parenthesis, // )
    left_brace, // {
    right_brace, // }
    number_of_separators
};

enum Num_arifmetic {
    plus_val,
    minus_val
};

enum Num_unary {
    plus_plus_val,
    minus_minus_val,
    exclamation_val
};

enum Num_comp {
    more_val,
    less_val,
    equal_val,
    not_equal_val,
    more_equal_val,
    less_equal_val
};

enum Num_logic {
    and_val,
    or_val
};

struct Keyword {
    Keyword(string word, Keys key);
    string word;
    Keys key;
};

struct Symbol {
    Symbol(char c);
    char ch;
    Symbol_type type;
};

struct State_act {
    State_act(int next, Act act);
    int next;
    Act act;
};


struct State_machine {
    State_machine(int state_num,int smb_num, Act act);
    void add_branch(int old_state, int smb, int next_state, Act call_back);
    //void add_non_final_state(int state);
    ~State_machine();
    int state_num;
    int smb_num;
    int current_state = 0;
    State_act** transitions;
};

union Token_value {
    long long  i;
    double d;
};

struct Lex_attributes {
    Lex_attributes(int s_num, string token, Token_type token_type, Token_value value);
    int s_num;
    string token;
    Token_type token_type;
    Token_value value;
};



struct State {
    char main_state = empty_st;
    char num_state = empty_nst;
    char op_state = empty_ost;
    char comment_state = no_comment;
    int s_num = 1;
    Token_value value;
};



int analyze(char* fname);
int recognize(Symbol& smb, vector<Lex_attributes> &recognized_lexs);
int recognize_lex(string& buf, State& current_state, vector<Lex_attributes> &recognized_lexs);
int recognize_num(Symbol smb, State& current_state);
int recognize_op(Symbol smb, State& current_state);
Number_symbol_type get_num_symol_type(char ch);
Operation_symbol_type get_op_symol_type(char ch);
void init_state_machines();
void init_main_machine();
void init_number_machine();
void init_operation_machine();
void init_hash_table();
void add_hash(string word, Keys key);
Token_type categorize(string str, State& state);
int find_key_word(string word);
int get_num_separator(char ch);
int get_num_comparison(string str); void show_lexs(vector<Lex_attributes>& recognized_lexs);

#endif
