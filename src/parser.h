#ifndef PARSER_H
#define PARSER_H
#include "lexanalyzer.h"
#include <stack>
#include <fstream>
using namespace std;

struct Var_attributes
{
  Var_attributes();
  Var_attributes(int reg_num, int type);
  int reg_num;
  int type;
};

struct Code_attributes
{
  Code_attributes();
  Code_attributes(Var_attributes var_attr, string code_str);
  Var_attributes var_attr;
  string code_str;
};

int parse(vector<Lex_attributes> recognized_lexs);
string get_name_pt(int pt);
void add_default_shifts(int pt);
int parse_lex(Lex_attributes lex,stack<int>& buf, ofstream& fout);
int parse_pt(int last, int next, int& act, int val_lex = 0);
int get_pt(int type);
void reduce_next(stack<int>& buf, int next,Lex_attributes lex, ofstream& fout);
int reduce_last(stack<int>& buf,Lex_attributes lex,ofstream& fout);
void reduce_one(stack<int>& buf, int next,Lex_attributes lex,ofstream& fout);
Var_attributes  find_var(string var_name);

void init_parser_machine();
void init_resword_pars_machine();
void init_separator_pars_machine();


enum Parser_nonterminals {
    empty_pt = number_of_tokens,
    operand_pt,
    id_pt,
    alt_id_pt,
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
    prestatement_pt,
    var_comma_pt,
    var_assign_pt,
    var_list_pt,
    var_init_pt,
    type_pt,
    statement_pt,
    statements_pt,
    for_opt_pt,
    if_1_pt,
    if_2_pt,
    if_3_pt,
    if_4_pt,
    if_5_pt,
    if_pt,
    else_1_pt,
    else_pt,
    for_1_pt,
    for_2_pt,
    for_3_pt,
    for_4_pt,
    for_5_pt,
    for_6_pt,
    for_7_pt,
    for_8_pt,
    for_pt,
    while_1_pt,
    while_2_pt,
    while_3_pt,
    while_4_pt,
    while_pt,
    break_pt,
    continue_pt,
    block_1_pt,
    block_2_pt,
    block_pt,
    error_pt,
    alt_pt,
    number_of_pars
};

enum Error_codes {
  undefined_ref_err = -99,
  already_exist_err
};


Code_attributes get_code(int next_pt, Lex_attributes lex, Code_attributes l_code, Code_attributes r_code);

#endif
