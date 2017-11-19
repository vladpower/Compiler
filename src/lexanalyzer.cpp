#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>
#include <string>
#include <list>
#include <unordered_set>
#include "lexanalyzer.h"
#include <iomanip>
#include "parser.h"

using namespace std;

int analyze(char* fname)
{
    ifstream fin;
    fin.open (fname);
    char ch;
    vector<Lex_attributes> recognized_lexs;
    init_state_machines();
    init_hash_table();
    while (fin >> noskipws >> ch) {
        Symbol smb(ch);
        if(recognize(smb, recognized_lexs) < 0) {
            cerr << "Invalid character "<< smb.ch << endl;
            return 0;
        }
    }
    //show_lexs(recognized_lexs);
    parse(recognized_lexs);

    fin.close();
    return 0;
}

void show_lexs(vector<Lex_attributes>& recognized_lexs)
{
    for(vector<Lex_attributes>::iterator it = recognized_lexs.begin();it != recognized_lexs.end();it++) {
        cout<<it->s_num<<'\t'<<it->token<<'\t';
        switch (it->token_type) {
            case id_type: {
                cout<<"identifier";
            }
            break;
            case reserved_type: {
                cout<<"reserved word";
            }
            break;
            case unary_type: {
                cout<<"unary type";
            }
            break;
            case assign_type: {
                cout<<"assign operation";
            }
            break;
            case arifm_op_type: {
                cout<<"arifmetic operation";
            }
            break;
            case logic_op_type: {
                cout<<"logic operation";
            }
            break;
            case comp_op_type: {
                cout<<"comparison operation";
            }
            break;
            case separator_type: {
                cout<<"separator";
            }
            break;
            case int_type: {
                cout<<"integer constant";
            }
            break;
            case float_type: {
                cout<<"float constant";
            }
            break;
            case bool_type: {
                cout<<"bool constant";
            }
            break;
        }
        cout<<'\t';
        if(it->token_type == float_type) {
            cout << it->value.d;
        } else {
            cout << it->value.i;
        }
        cout << endl;
    }
}


void init_state_machines()
{
    init_main_machine();
    init_number_machine();
    init_operation_machine();
}

State_machine main_machine(number_of_states,number_of_symtype, end_act);
void init_main_machine()
{
    main_machine.add_branch(empty_st, digit, number_st,number_act);
    main_machine.add_branch(empty_st, letter, word_st,nil);
    main_machine.add_branch(empty_st, comment, comment_st, operation_act);
    main_machine.add_branch(comment_st, comment, comment_st, operation_act);
    main_machine.add_branch(empty_st, comparison, comparison_st,operation_act);
    main_machine.add_branch(empty_st, logic, logic_st,operation_act);
    main_machine.add_branch(empty_st, math, math_st,operation_act);
    main_machine.add_branch(empty_st, separator, separator_st,nil);
    main_machine.add_branch(empty_st, wrong_symb, empty_st,nil);
    main_machine.add_branch(number_st, digit, number_st,number_act);
    main_machine.add_branch(number_st, letter, number_e_st,number_act);
    main_machine.add_branch(number_e_st, digit, number_e_st,number_act);
    main_machine.add_branch(number_e_st, math, number_e_st,number_act);
    main_machine.add_branch(word_st, digit, word_st,nil);
    main_machine.add_branch(word_st, letter, word_st,nil);
    main_machine.add_branch(comparison_st, comparison, comparison_st,operation_act);
    main_machine.add_branch(logic_st, logic, logic_st,operation_act);
    main_machine.add_branch(math_st, math, math_st,operation_act);
    main_machine.add_branch(math_st, comparison, comparison_st,operation_act);
}

State_machine number_machine(number_of_numstates,number_of_numsymtype, wrong_act);
void init_number_machine()
{
    number_machine.add_branch(empty_nst, digit_num, int_nst,nil);
    number_machine.add_branch(empty_nst, dot_num, dot_nst,nil);
    number_machine.add_branch(int_nst, digit_num, int_nst,nil);
    number_machine.add_branch(int_nst, dot_num, dot_nst,nil);
    number_machine.add_branch(dot_nst, digit_num, fraction_nst,nil);
    number_machine.add_branch(fraction_nst, digit_num, fraction_nst,nil);
    number_machine.add_branch(fraction_nst, e_num, e_nst,nil);
    number_machine.add_branch(e_nst, arifm_num, sign_nst,nil);
    number_machine.add_branch(sign_nst, digit_num, order_nst,nil);
    number_machine.add_branch(order_nst, digit_num, order_nst,nil);
}

State_machine operation_machine(number_of_opstates,number_of_opsymtype, wrong_act);
void init_operation_machine()
{
    operation_machine.add_branch(empty_ost,plus_op,plus_ost,nil);
    operation_machine.add_branch(empty_ost,minus_op,minus_ost,nil);
    operation_machine.add_branch(empty_ost,exclamation_op,exclamation_ost,nil);
    operation_machine.add_branch(empty_ost,equal_op,assign_ost,nil);
    operation_machine.add_branch(empty_ost,comp_op,one_sign_comp_ost,nil);
    operation_machine.add_branch(empty_ost,ampersand_op,ampersand_ost,nil);
    operation_machine.add_branch(empty_ost,stick_op,stick_ost,nil);
    operation_machine.add_branch(empty_ost,slash_op,slash_ost,nil);
    operation_machine.add_branch(empty_ost,asterisk_op,asterisk_ost,nil);
    operation_machine.add_branch(plus_ost,plus_op,plus_plus_ost,nil);
    operation_machine.add_branch(minus_ost,minus_op,minus_minus_ost,nil);
    operation_machine.add_branch(exclamation_ost,equal_op,two_sign_comp_ost,nil);
    operation_machine.add_branch(assign_ost,equal_op,two_sign_comp_ost,nil);
    operation_machine.add_branch(one_sign_comp_ost,equal_op,two_sign_comp_ost,nil);
    operation_machine.add_branch(ampersand_ost,ampersand_op,two_ampersand_ost,nil);
    operation_machine.add_branch(stick_ost,stick_op,two_stick_ost,nil);
    operation_machine.add_branch(slash_ost,slash_op,two_slash_ost,end_act);
    operation_machine.add_branch(slash_ost,asterisk_op,comment_open_ost,end_act);
    operation_machine.add_branch(asterisk_ost,slash_op,comment_close_ost,end_act);
}

int recognize(Symbol& smb,vector<Lex_attributes> &recognized_lexs)
{
    static State current_state;
    static string buf;
    if(smb.type == wrong_symb)
        return -1;
    if(current_state.comment_state) {
        if(smb.ch == '\n') {
            if(current_state.comment_state == line_comment)
                current_state.comment_state = no_comment;
        } else if(smb.type != comment) {
            return 0;
        }
    }
    State_act  state_act = main_machine.transitions[current_state.main_state][smb.type];
    switch(state_act.act) {
        case number_act: {
            if(recognize_num(smb.ch, current_state) < 0) {
                return -1;
            }
        }
        break;
        case operation_act: {
            switch (recognize_op(smb.ch, current_state)) {
                case end_act: {
                    buf+=smb.ch;
                    if(recognize_lex(buf,current_state,recognized_lexs)<0)
                        return -1;
                    current_state.main_state = empty_st;
                    return 0;
                }
                break;
                case nil: {

                }
                break;
                default: {
                    return -1;
                }
            }
        }
        break;
        case end_act: {
            if(recognize_lex(buf,current_state,recognized_lexs)<0)
                return -1;
            current_state.main_state = state_act.next;
            recognize(smb, recognized_lexs);
            return 0;
        }
        break;
    }
    buf+=smb.ch;
    current_state.main_state = state_act.next;

    return 0;
}

int recognize_lex(std::string& buf, State& current_state, std::vector<Lex_attributes> &recognized_lexs)
{
    Token_type token_type = categorize(buf, current_state);
    switch(token_type) {
        case space_type: {

        }
        break;
        case wrong_type: {
            return -1;
        }
        break;
        default: {
            recognized_lexs.push_back(Lex_attributes(current_state.s_num,buf,token_type,current_state.value));
        }
    }

    buf.clear();
}

int recognize_num(Symbol smb, State& current_state)
{
      Number_symbol_type type_symbol = get_num_symol_type(smb.ch);
      State_act  state_act = number_machine.transitions[current_state.num_state][type_symbol];
      if(state_act.act == wrong_act) {
          return -1;
      }
      current_state.num_state = state_act.next;

}

int recognize_op(Symbol smb, State& current_state)
{
    Operation_symbol_type type_symbol = get_op_symol_type(smb.ch);
    State_act state_act = operation_machine.transitions[current_state.op_state][type_symbol];
    if(state_act.act == wrong_act) {
        //cout<<"ERR "<<type_symbol<<' '<<smb.ch<<' '<<(int)current_state.op_state<<endl;
        return -1;
    }
    current_state.op_state = state_act.next;
    return state_act.act;
}

Number_symbol_type get_num_symol_type(char ch)
{
    if(ch == '.')
        return dot_num;
    if(isdigit(ch))
        return digit_num;
    if(toupper(ch) == 'E')
        return e_num;
    if(ch == '+' || ch == '-') {
        return arifm_num;
    }
}

Operation_symbol_type get_op_symol_type(char ch)
{
    switch(ch) {
        case '+': {
            return plus_op;
        }
        case '-': {
            return minus_op;
        }
        case '!': {
            return exclamation_op;
        }
        case '=': {
            return equal_op;
        }
        case '<':
        case '>': {
            return comp_op;
        }
        case '&': {
            return ampersand_op;
        }
        case '|': {
            return stick_op;
        }
        case '/': {
            return slash_op;
        }
        case '*': {
            return asterisk_op;
        }
    }
}

int get_num_separator(char ch)
{
    switch (ch) {
        case ',': {
            return comma;
        }
        case ';': {
            return semicolon;
        }
        case '(': {
            return left_parenthesis;
        }
        case ')': {
            return right_parenthesis;
        }
        case '{': {
            return left_brace;
        }
        case '}': {
            return right_brace;
        }
        default: {
            return -1;
        }
    }
}

int get_num_comparison(std::string str)
{
    switch (str[0]) {
        case '=': {
            return equal_val;
        }
        case '<': {
            if(str.length()>1) {
                return less_equal_val;
            } else {
                return less_val;
            }
        }
        case '>': {
            if(str.length()>1) {
                return more_equal_val;
            } else {
                return more_val;
            }
        }
        case '!': {
            return not_equal_val;
        }
    }
}

Token_type categorize(string str, State& state)
{
    state.value.i = 0;
    switch(state.main_state) {
        case word_st: {
            int key = find_key_word(str);
            if(key>=false_key) {
                state.value.i = key - false_key; // ture or false
                return bool_type;
            } else if(key>=if_key) {
                state.value.i = key;
                return reserved_type;
            } else {
                return id_type;
            }
        }
        break;
        case separator_st: {
            if(str == "\n") {
                state.s_num++;
                return space_type;
            }
            if(str == " ") {
                return space_type;
            }
            state.value.i = get_num_separator(str[0]);
            return separator_type;
        }
        case number_st: {
            int num_state = state.num_state;
            state.num_state = empty_nst;
            switch (num_state) {
                case int_nst: {
                    state.value.i = stoi(str);
                    return int_type;
                }
                case fraction_nst: {
                    state.value.d = stod(str);
                    return float_type;
                }
                default: {
                    return wrong_type;
                }
            }
        }
        case number_e_st: {
            int num_state = state.num_state;
            state.num_state = empty_nst;
            if(num_state == order_nst) {
                state.value.d = stod(str);
                return float_type;
            } else {
                return wrong_type;
            }
        }
        case comparison_st:
        case math_st: {
            int op_state = state.op_state;
            state.op_state = empty_ost;
            switch (op_state) {
                case assign_ost: {
                    return assign_type;
                }
                case plus_ost: {
                    state.value.i = plus_val;
                    return arifm_op_type;
                }
                case minus_ost: {
                    state.value.i = minus_val;
                    return arifm_op_type;
                }
                case plus_plus_ost: {
                    state.value.i = plus_plus_val;
                    return unary_type;
                }
                case minus_minus_ost: {
                    state.value.i = minus_minus_val;
                    return unary_type;
                }
                case exclamation_ost: {
                    state.value.i = exclamation_val;
                    return unary_type;
                }
                case one_sign_comp_ost: {
                    state.value.i = get_num_comparison(str);
                    return comp_op_type;
                }
                case two_sign_comp_ost: {
                    state.value.i = get_num_comparison(str);
                    return comp_op_type;
                }
                default: {
                    return wrong_type;
                }
            }
        }
        case logic_st: {
            int op_state = state.op_state;
            state.op_state = empty_nst;
            switch (op_state) {
                case two_stick_ost: {
                    state.value.i = or_val;
                    return logic_op_type;
                }
                case two_ampersand_ost: {
                    state.value.i = and_val;
                    return logic_op_type;
                }
                default: {
                    return wrong_type;
                }
            }
        }
        case comment_st: {
            int op_state = state.op_state;
            state.op_state = empty_nst;
            switch (op_state) {
                case two_slash_ost: {
                    if(state.comment_state == no_comment)
                        state.comment_state = line_comment;
                    return space_type;
                }
                case comment_open_ost: {
                    if(state.comment_state == no_comment)
                        state.comment_state = multiple_comment;
                    return space_type;
                }
                case comment_close_ost: {
                    if(state.comment_state == multiple_comment)
                        state.comment_state = no_comment;
                    return space_type;
                }
                default: {
                    return wrong_type;
                }
            }
        }
        default: {
            return space_type;
        }
    }
}

list<Keyword> key_words[10];
hash<string> hash_fn;
void init_hash_table()
{
    add_hash("if",if_key);
    add_hash("else",else_key);
    add_hash("for",for_key);
    add_hash("while",while_key);
    add_hash("break",break_key);
    add_hash("continue",continue_key);
    add_hash("bool",bool_key);
    add_hash("int",int_key);
    add_hash("double",double_key);
    add_hash("true",true_key);
    add_hash("false",false_key);
}

void add_hash(string word, Keys key)
{
    key_words[hash_fn(word)%10].push_back(Keyword(word,key));
}

int find_key_word(string word)
{
    int h = hash_fn(word)%10;
    auto it = key_words[h].begin();
    while (it!= key_words[h].end()) {
        if(it->word == word) {
            return it->key;
        }
        it++;
    }
    return -1;
}

Symbol::Symbol(char c) { // transliterator
    ch = c;
    static map<char,Symbol_type> defenition = {
        {'.',digit},
        {'/',comment},
        {'*',comment},
        {'=',math},
        {'<',comparison},
        {'>',comparison},
        {'!',math},
        {'&',logic},
        {'|',logic},
        {'+',math},
        {'-',math},
        {' ',separator},
        {'\n',separator},
        {',',separator},
        {';',separator},
        {'(',separator},
        {')',separator},
        {'{',separator},
        {'}',separator}
    };
    if(isdigit(ch)) {
        type = digit;
    } else if(isalpha(ch)) {
        type = letter;
    } else {
        type = defenition.find(ch)->second;
    }
}

State_act::State_act(int next = empty_st, Act act = end_act){
   this->next = next;
   this->act = act;
}

Keyword::Keyword(string word, Keys key)
{
    this->word = word;
    this->key = key;
}

State_machine::State_machine(int state_num,int smb_num, Act act)
{
    this->state_num = state_num;
    this->smb_num = smb_num;
    transitions = new State_act*[state_num];
    for(int i=0;i<state_num;i++) {
        transitions[i] = new State_act[smb_num];
        for(int j=0;j<smb_num;j++) {
            transitions[i][j].act = act;
        }
    }
}

void State_machine::add_branch(int old_state, int smb, int next_state, Act call_back)
{
    transitions[old_state][smb] = State_act(next_state,call_back);
}

State_machine::~State_machine()
{
    for(int i=0;i<state_num;i++)
        delete transitions[i];
    delete transitions;
}

Lex_attributes::Lex_attributes(int s_num, std::string token, Token_type token_type,Token_value value) {
    this->s_num = s_num;
    this-> token = token;
    this->token_type = token_type;
    this->value = value;
}
