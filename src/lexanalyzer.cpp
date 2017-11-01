#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>
#include <string>
#include <list>
#include <unordered_set>
#include "lexanalyzer.h"
#include <iomanip>

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
        }
    }

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
            case assign_type: {
                cout<<"reserved word";
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
        cout<<endl;
    }

    fin.close();
    return 0;
}


void init_state_machines()
{
    init_main_machine();
    init_number_machine();
}

State_machine main_machine(10,8);
void init_main_machine()
{
    main_machine.add_branch(empty_st, digit, number_st,number_act);
    main_machine.add_branch(empty_st, letter, word_st,nil);
    main_machine.add_branch(empty_st, comment, empty_st,nil);
    main_machine.add_branch(empty_st, comparison, comparison_st,nil);
    main_machine.add_branch(empty_st, logic, logic_st,nil);
    main_machine.add_branch(empty_st, math, math_st,nil);
    main_machine.add_branch(empty_st, separator, separator_st,nil);
    main_machine.add_branch(empty_st, wrong_symb, empty_st,nil);
    main_machine.add_branch(number_st, digit, number_st,number_act);
    main_machine.add_branch(number_st, letter, number_e_st,number_act);
    main_machine.add_branch(number_e_st, digit, number_e_st,number_act);
    main_machine.add_branch(number_e_st, math, number_e_st,number_act);
    main_machine.add_branch(word_st, digit, word_st,nil);
    main_machine.add_branch(word_st, letter, word_st,nil);
    main_machine.add_branch(comparison_st, comparison, comparison_st,nil);
    main_machine.add_branch(logic_st, logic, logic_st,nil);
    main_machine.add_branch(math_st, math, math_st,nil);
}

State_machine number_machine(7,4);
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

int recognize(Symbol& smb,vector<Lex_attributes> &recognized_lexs)
{
    static State current_state;
    static string val;
    if(smb.type == wrong_symb)
        return -1;
    State_act  state_act = main_machine.transitions[current_state.main_state][smb.type];
    switch(state_act.act) {
        case number_act: {
            if(recognize_num(smb.ch, current_state) < 0) {
                return -1;
            }

        }
        break;
        case end_act: {
            Token_type token_type = categorize(val, current_state);
            switch(token_type) {
                case space_type: {

                }
                break;
                case wrong_type: {
                    return -1;
                }
                break;
                default: {
                    recognized_lexs.push_back(Lex_attributes(current_state.s_num,val,token_type));
                }
            }

            val.clear();
            current_state.main_state = state_act.next;
            recognize(smb, recognized_lexs);
            return 0;
        }
        break;
    }
    val+=smb.ch;
    current_state.main_state = state_act.next;

    return 0;
}

int recognize_num(Symbol smb, State& current_state)
{
        Number_symbol_type type_symbol = get_num_symol_type(smb.ch);
        State_act  state_act = number_machine.transitions[current_state.num_state][type_symbol];
        if(state_act.act == end_act) {
            return -1;
        }
        current_state.num_state = state_act.next;

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

Token_type categorize(string str, State& state)
{
    switch(state.main_state) {
        case word_st: {
            int key = find_key_word(str);
            if(key>=true_key) {
                return bool_type;
            } else if(key>=if_key) {
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
            return separator_type;
        }
        case number_st: {
            int num_state = state.num_state;
            state.num_state = empty_nst;
            switch (num_state) {
                case int_nst: {
                    return int_type;
                }
                case fraction_nst: {
                    return float_type;
                }
                default: {
                    return wrong_type;
                }
            }
        }
        case number_e_st: {
            if(state.num_state == order_nst) {
                return float_type;
            } else {
                return wrong_type;
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
    add_hash("bool",while_key);
    add_hash("int",break_key);
    add_hash("double",continue_key);
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
    return 0;
}

Symbol::Symbol(char c) { // transliterator
    ch = c;
    static map<char,Symbol_type> defenition = {
        {'.',digit},
        {'/',comment},
        {'*',comment},
        {'=',comparison},
        {'<',comparison},
        {'>',comparison},
        {'!',comparison},
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

State_act::State_act(){
   this->next = empty_st;
   this->act = end_act;
}

State_act::State_act(int next, Act act){
   this->next = next;
   this->act = act;
}

Keyword::Keyword(string word, Keys key)
{
    this->word = word;
    this->key = key;
}

State_machine::State_machine(int state_num,int smb_num)
{
    this->state_num = state_num;
    this->smb_num = smb_num;
    transitions = new State_act*[state_num];
    for(int i=0;i<state_num;i++)
        transitions[i] = new State_act[smb_num];
}

void State_machine::add_branch(int old_state, int smb, int next_state, Act call_back)
{
    transitions[old_state][smb] = State_act(next_state,call_back);
}

// void State_machine::add_non_final_state(int state)
// {
//     for(int i =0;i<smb_num;i++) {
//         if(transitions[state][i].act == end_act)
//             transitions[state][i].act = invalid_act;
//     }
// }

State_machine::~State_machine()
{
    for(int i=0;i<state_num;i++)
        delete transitions[i];
    delete transitions;
}

Lex_attributes::Lex_attributes(int s_num, std::string token, Token_type token_type) {
    this->s_num = s_num;
    this-> token = token;
    this->token_type = token_type;
}
