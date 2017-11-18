#include "parser.h"

int parse(vector<Lex_attributes> recognized_lexs)
{
    stack<Lex_attributes> buf;
    vector<Lex_attributes>::iterator it;
    for(it = recognized_lexs.begin();it!=recognized_lexs.end();it++) {

    }
    return 0;
}

State_machine parser_machine(get_pt(number_of_pars),number_of_pars, wrong_act);
void init_parser_machine()
{
    parser_machine.add_branch(get_pt(empty_pt), reserved_type, get_pt(empty_pt), resword_act);
    parser_machine.add_branch(get_pt(empty_pt), id_type, get_pt(id_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), int_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), float_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), bool_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), unary_type, get_pt(unary_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(unary_op_pt), operand_pt, get_pt(unary_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(unary_op_pt), id_pt, get_pt(unary_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(unary_exp_pt), arifm_op_type, get_pt(additive_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_op_pt), unary_exp_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(operand_pt), arifm_op_type, get_pt(additive_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_op_pt), operand_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(id_pt), arifm_op_type, get_pt(additive_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_op_pt), id_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_exp_pt), comp_op_type , get_pt(log_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(log_op_pt), additive_exp_pt, get_pt(log_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(unary_exp_pt), comp_op_type , get_pt(log_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(log_op_pt), unary_exp_pt, get_pt(log_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(operand_pt), comp_op_type , get_pt(log_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(log_op_pt), operand_pt, get_pt(log_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(id_pt), comp_op_type , get_pt(log_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(log_op_pt), id_pt, get_pt(log_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(log_exp_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(expression_op_pt), log_exp_pt, get_pt(expression_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_exp_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(expression_op_pt), additive_exp_pt, get_pt(expression_pt), shift_act);
    parser_machine.add_branch(get_pt(unary_exp_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(expression_op_pt), unary_exp_pt, get_pt(expression_pt), shift_act);
    parser_machine.add_branch(get_pt(operand_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(expression_op_pt), operand_pt, get_pt(expression_pt), shift_act);
    parser_machine.add_branch(get_pt(id_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(expression_op_pt), id_pt, get_pt(expression_pt), shift_act);
    parser_machine.add_branch(get_pt(id_pt), assign_type, get_pt(statement_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(statement_op_pt), expression_pt, get_pt(statement_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(statement_op_pt), log_exp_pt, get_pt(statement_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), additive_exp_pt, get_pt(statement_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), unary_exp_pt, get_pt(statement_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), operand_pt, get_pt(statement_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), id_pt, get_pt(statement_exp_pt), shift_act);
    parser_machine.add_branch(get_pt(id_pt), separator_type, get_pt(empty_pt), separator_act);
    parser_machine.add_branch(get_pt(statement_exp_pt), separator_type, get_pt(empty_pt), separator_act);
    parser_machine.add_branch(get_pt(var_comma_pt), id_pt, get_pt(var_list_pt), shift_act);
    parser_machine.add_branch(get_pt(var_comma_pt), statement_exp_pt, get_pt(var_list_pt), reduce_act);
    parser_machine.add_branch(get_pt(var_list_pt), separator_type, get_pt(empty_pt), separator_act);
    parser_machine.add_branch(get_pt(type_pt), var_list_pt, get_pt(local_var_decl_pt), reduce_act);
    parser_machine.add_branch(get_pt(type_pt), id_pt, get_pt(local_var_decl_pt), shift_act);
    parser_machine.add_branch(get_pt(local_var_decl_pt), separator_type, get_pt(empty_pt), separator_act); // ; statement
    parser_machine.add_branch(get_pt(if_1_pt), separator_type, get_pt(empty_pt), separator_act); // (
    parser_machine.add_branch(get_pt(if_2_pt), expression_pt, get_pt(if_3_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_2_pt), log_exp_pt, get_pt(if_3_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), additive_exp_pt, get_pt(if_3_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), unary_exp_pt, get_pt(if_3_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), operand_pt, get_pt(if_3_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), id_pt, get_pt(if_3_pt), shift_act);
    parser_machine.add_branch(get_pt(if_3_pt), separator_type, get_pt(empty_pt), separator_act);



}

int get_pt(int type)
{
    return type - empty_pt;
}
