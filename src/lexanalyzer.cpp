#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>
#include <string>
#include <list>
#include <unordered_set>
#include "lexanalyzer.h"

using namespace std;

int analyze(char* fname)
{
    ifstream fin;
    fin.open (fname);
    char ch;
    init_state_machines();
    init_hash_table();
    while (fin >> noskipws >> ch) {
        Symbol smb(ch);
        recognize(smb);


    }
    fin.close();
    return 0;
}


void init_state_machines()
{
    init_main_machine();
    init_number_machine();
}

State_machine main_machine(9,8);
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
    main_machine.add_branch(number_st, letter, number_st,number_act);
    main_machine.add_branch(word_st, digit, word_st,nil);
    main_machine.add_branch(word_st, letter, word_st,nil);
    main_machine.add_branch(comparison_st, comparison, comparison_st,nil);
    main_machine.add_branch(logic_st, logic, logic_st,nil);
    main_machine.add_branch(math_st, math, math_st,nil);
}

State_machine number_machine(8,5);
void init_number_machine()
{
    number_machine.add_branch(empty_nst, minus_num, minus_nst,nil);
    number_machine.add_branch(empty_nst, digit_num, int_nst,nil);
    number_machine.add_branch(empty_nst, dot_num, dot_nst,nil);
    number_machine.add_branch(minus_nst, digit_num, int_nst,nil);
    number_machine.add_branch(minus_nst, dot_num, dot_nst,nil);
    number_machine.add_branch(int_nst, digit_num, int_nst,nil);
    number_machine.add_branch(int_nst, dot_num, dot_nst,nil);
    number_machine.add_branch(dot_nst, digit_num, fraction_nst,nil);
    number_machine.add_branch(fraction_nst, digit_num, fraction_nst,nil);
    number_machine.add_branch(fraction_nst, e_num, e_nst,nil);
    number_machine.add_branch(e_nst, plus_num, sign_nst,nil);
    number_machine.add_branch(e_nst, minus_num, sign_nst,nil);
    number_machine.add_branch(sign_nst, digit_num, order_nst,nil);
}



int recognize(Symbol& smb)
{
    static char current_state = empty_st;
    static string val;
    State_act  state_act = main_machine.transitions[current_state][smb.type];


    switch(state_act.act) {
        case number_act: {

        }
        break;
        case end_act: {

            categorize(val,current_state);

            val.clear();
            current_state = state_act.next;
            recognize(smb);
            return 0;
        }
        break;
    }
    val+=smb.ch;
    current_state = state_act.next;

    return 0;
}

void categorize(string str, char state)
{
    switch(state) {
        case word_st: {
            int key = find_key_word(str);
            if(key>0) {
                cout<<"keyword "<<str<<' '<<key<<endl;
            }
        }
        break;
        case separator_st: {
            if(str == " " || str == "\n") {
                return;
            }
        }
    }
    cout<<str <<" "<< (int)state <<endl;
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

Symbol::Symbol(char c) {
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

State_machine::~State_machine()
{
    for(int i=0;i<state_num;i++)
        delete transitions[i];
    delete transitions;
}
