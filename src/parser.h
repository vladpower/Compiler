#ifndef PARSER_H
#define PARSER_H
#include "lexanalyzer.h"
#include <stack>
using namespace std;

int parse(vector<Lex_attributes> recognized_lexs);
int parse_lex(Lex_attributes lex,stack<int>& buf);
int parse_pt(int last, int next, int& act, int val_lex = 0);
int get_pt(int type);
void init_parser_machine();
void init_resword_pars_machine();
void init_separator_pars_machine();


enum Parser_nonterminals {
    empty_pt = number_of_tokens,
    operand_pt,
    id_pt,
    unary_op_pt,
    unary_exp_pt,
    additive_op_pt,
    additive_exp_pt,
    log_op_pt,
    log_exp_pt,
    expression_op_pt,
    expression_pt,
    statement_op_pt,
    statement_exp_pt,
    local_var_decl_pt,
    var_comma_pt,
    var_decl_init_pt,
    var_assign_pt,
    type_pt,
    statement_pt,
    statements_pt,
    if_1_pt,
    if_2_pt,
    if_3_pt,
    if_4_pt,
    if_pt,
    else_1_pt,
    else_pt,
    for_1_pt,
    error_pt,
    number_of_pars
};


#endif
