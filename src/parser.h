#ifndef PARSER_H
#define PARSER_H
#include "lexanalyzer.h"
#include <stack>
using namespace std;

int parse(vector<Lex_attributes> recognized_lexs);
int get_pt(int type);

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
    var_comma_pt,
    var_list_pt,
    type_pt,
    local_var_decl_pt,
    statement_pt,
    if_1_pt,
    if_2_pt,
    if_3_pt,
    if_4_pt,
    number_of_pars
};


#endif