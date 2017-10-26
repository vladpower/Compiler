#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>
#include "lexanalyzer.h"

using namespace std;

int analyze(char* fname)
{
    ifstream fin;
    fin.open (fname);
    char ch;


    while (fin >> noskipws >> ch) {
        Symbol smb(ch);
        recognize(smb);


    }
    fin.close();
    return 0;
}

int recognize(Symbol& smb)
{
    static char transitions[7][8] = {
    //   digit     letter   comment  comparison    logic    math    separator    wrong_symb
        {number_st,word_st, empty_st,comparison_st,logic_st,math_st,separator_st,wrong_st}, // empty_st
        {number_st,number_st,end_st,  end_st,      end_st,  end_st, end_st,      wrong_st}, // number_st
        {word_st,  word_st, end_st,   end_st,      end_st,  end_st, end_st,      wrong_st}, // word_st
        {end_st,   end_st,  end_st,  comparison_st,end_st,  end_st, end_st,      wrong_st}, // comparison_st
        {end_st,   end_st,  end_st,   end_st,      logic_st,end_st, end_st,      wrong_st}, // logic_st
        {end_st,   end_st,  end_st,   end_st,      end_st,  math_st,end_st,      wrong_st}, // math_st
        {end_st,   end_st,  end_st,   end_st,      end_st,  end_st, end_st,      wrong_st} // separator_st
    };
    static map<char,map<char,pair<char,char>>> = {
        {digit,map<char,pair<char,char>>(empty_st,pair<char,char>(number_st,0))};
    }
    static char current_state = empty_st;
    static string val;
    current_state = transitions[current_state][smb.type];
    cout << (int)current_state;
    if(current_state < end_st) {
        val+=smb.ch;
    } else if(current_state == end_st) {
        cout << val <<endl;
        val.clear();
        current_state = empty_st;
        recognize(smb);
    } else {
        cerr << "wrong symbol" <<endl;
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
