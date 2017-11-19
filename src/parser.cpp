#include "parser.h"
#include <iostream>

int parse(vector<Lex_attributes> recognized_lexs)
{
    init_parser_machine();
    init_resword_pars_machine();
    init_separator_pars_machine();
    stack<int> buf;
    buf.push(get_pt(empty_pt));
    Token_value val;
    val.i = 0;
    recognized_lexs.push_back(Lex_attributes(0,"",end_type,val));
    vector<Lex_attributes>::iterator it;
    for(it = recognized_lexs.begin();it!=recognized_lexs.end();it++) {
        if(parse_lex(*it, buf)<0) {
            cout << "Syntax error" << endl;
            return -1;
        }
    }
    if(buf.size() == 1 && buf.top() == get_pt(statements_pt)) {
        cout << "Syntax right" << endl;
    } else {
        cout << "Syntax error " << buf.size() <<' '<< buf.top() << endl;
    }
    return 0;
}

State_machine parser_machine(get_pt(number_of_pars),number_of_pars, wrong_act);
void init_parser_machine()
{
    parser_machine.add_branch(get_pt(empty_pt), reserved_type, get_pt(error_pt), resword_act);
    parser_machine.add_branch(get_pt(empty_pt), separator_type, get_pt(error_pt), separator_act);
    parser_machine.add_branch(get_pt(empty_pt), id_type, get_pt(id_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), int_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), float_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), bool_type, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), unary_type, get_pt(unary_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), id_pt, get_pt(operand_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), operand_pt, get_pt(unary_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), unary_exp_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), additive_exp_pt, get_pt(log_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), log_exp_pt, get_pt(expression_pt), reduce_act);
    //parser_machine.add_branch(get_pt(empty_pt), expression_pt, get_pt(statement_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), if_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), else_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), local_var_decl_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), var_decl_init_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), statement_pt, get_pt(statements_pt), reduce_act);

    parser_machine.add_branch(get_pt(id_pt), assign_type, get_pt(statement_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(unary_op_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(unary_op_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(unary_op_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(unary_op_pt), operand_pt, get_pt(unary_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(unary_exp_pt), arifm_op_type, get_pt(additive_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(additive_op_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(additive_op_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(additive_op_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(additive_op_pt), unary_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(additive_op_pt), unary_exp_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(additive_op_pt), additive_exp_pt, get_pt(additive_exp_pt), reduce_act);
    //parser_machine.add_branch(get_pt(additive_op_pt), operand_pt, get_pt(additive_exp_pt), reduce_act);
    //parser_machine.add_branch(get_pt(additive_op_pt), id_pt, get_pt(additive_exp_pt), reduce_act);

    //parser_machine.add_branch(get_pt(additive_exp_pt), arifm_op_type , get_pt(additive_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(log_op_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(log_op_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(log_op_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(log_op_pt), unary_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(log_op_pt), additive_exp_pt, get_pt(log_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(log_exp_pt), comp_op_type , get_pt(log_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(expression_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(expression_op_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(expression_op_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(expression_op_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(expression_op_pt), unary_type, get_pt(error_pt), shift_act);

    parser_machine.add_branch(get_pt(expression_op_pt), expression_pt, get_pt(expression_pt), reduce_act);

    parser_machine.add_branch(get_pt(statement_op_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), unary_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_op_pt), expression_pt, get_pt(statement_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(statement_exp_pt), separator_type, get_pt(error_pt), separator_act); // ;


    parser_machine.add_branch(get_pt(type_pt), id_type, get_pt(local_var_decl_pt), reduce_act);
    parser_machine.add_branch(get_pt(local_var_decl_pt), assign_type, get_pt(var_assign_pt), reduce_act);
    parser_machine.add_branch(get_pt(var_assign_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(var_assign_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(var_assign_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(var_assign_pt), expression_pt, get_pt(var_decl_init_pt), reduce_act);
    parser_machine.add_branch(get_pt(local_var_decl_pt), separator_type, get_pt(empty_pt), separator_act); // , ;
    parser_machine.add_branch(get_pt(var_decl_init_pt), separator_type, get_pt(empty_pt), separator_act); // ,
    parser_machine.add_branch(get_pt(var_comma_pt), id_type, get_pt(local_var_decl_pt), reduce_act);


    parser_machine.add_branch(get_pt(if_1_pt), separator_type, get_pt(error_pt), separator_act); // (
    parser_machine.add_branch(get_pt(if_2_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_2_pt), expression_pt, get_pt(if_3_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_3_pt), separator_type, get_pt(error_pt), separator_act); // )
    parser_machine.add_branch(get_pt(if_4_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_4_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_4_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_4_pt), statement_pt, get_pt(if_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_pt), reserved_type, get_pt(error_pt), resword_act);
    parser_machine.add_branch(get_pt(else_1_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(else_1_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(else_1_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(else_1_pt), statement_pt, get_pt(else_pt), reduce_act);

    parser_machine.add_branch(get_pt(statement_pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statement_pt), statement_pt, get_pt(statements_pt), reduce_act);
    parser_machine.add_branch(get_pt(statements_pt), statement_pt, get_pt(statements_pt), reduce_act);
    parser_machine.add_branch(get_pt(statements_pt), separator_type, get_pt(error_pt), separator_act);
    parser_machine.add_branch(get_pt(statements_pt), end_type, get_pt(statements_pt), reduce_act);
}

State_machine resword_pars_machine(get_pt(number_of_pars),number_of_keys, wrong_act);
void init_resword_pars_machine()
{
    resword_pars_machine.add_branch(get_pt(empty_pt), if_key, get_pt(if_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(if_pt), else_key, get_pt(else_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), int_key, get_pt(type_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), double_key, get_pt(type_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), for_key, get_pt(for_1_pt), reduce_act);
}

State_machine separator_pars_machine(get_pt(number_of_pars),number_of_separators, wrong_act);
void init_separator_pars_machine()
{
    separator_pars_machine.add_branch(get_pt(empty_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(statements_pt), semicolon, get_pt(error_pt), shift_act);
    separator_pars_machine.add_branch(get_pt(if_1_pt), left_parenthesis, get_pt(if_2_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(if_3_pt), right_parenthesis, get_pt(if_4_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(local_var_decl_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(var_decl_init_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(statement_exp_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(var_decl_init_pt), comma, get_pt(var_comma_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(local_var_decl_pt), comma, get_pt(var_comma_pt), reduce_act);

}

int parse_lex(Lex_attributes lex,stack<int>& buf)
{
    int last = buf.top();
    int token_type = lex.token_type;
    int token_val = lex.value.i;
    int act;
    int next = parse_pt(buf.top(), token_type, act,token_val);
    cout << "act " << act << " last " << buf.top() << " token " << token_type << " next " << next << endl;
    switch (act) {
        case reduce_act: {
            cout<<"pop "<<buf.top()<<", push "<<next<<endl;
            buf.pop();
            buf.push(next);


        }
        break;
        case shift_act: {
            int next_act;
            next = parse_pt(get_pt(empty_pt) ,token_type, next_act,token_val);
            if(next_act == reduce_act) {
                cout<<"push "<<next<<endl;
                buf.push(next);
            } else {
                return -1;
            }

        }
        break;
        case wrong_act: {
            int next_act;

            if(buf.size()>=2) {
                buf.pop();
                int penult = buf.top();
                next = parse_pt(penult ,last + number_of_tokens, next_act);
                cout << "act " << next_act << " last " << penult << " pt " << last << " next " << next << endl;
                if(next_act != wrong_act && next != error_pt) {
                    cout<<"pop "<<penult<<", pop "<<last<<", push "<<next<<endl;
                    buf.pop();
                    buf.push(next);
                    return parse_lex(lex,buf);
                } else {
                    buf.push(last);
                }
            }
            next = parse_pt(get_pt(empty_pt), last + number_of_tokens, next_act);
            if(next_act == reduce_act) {
                cout<<"pop " << buf.top() << " push "<<next<<endl;
                buf.pop();
                buf.push(next);
                if(parse_lex(lex,buf)<0) {
                    return -1;
                }
            } else {
                return -1;
            }
        }
        default: {

        }
    }
    return 0;


}

int parse_pt(int last, int pt, int& act, int val_lex)
{
    State_act state_act = parser_machine.transitions[last][pt];
    switch(state_act.act) {
        case resword_act: {
            State_act resword_state_act = resword_pars_machine.transitions[last][val_lex];
            act = resword_state_act.act;
            return resword_state_act.next;
        }
        break;
        case separator_act: {
            State_act separator_state_act = separator_pars_machine.transitions[last][val_lex];
            act = separator_state_act.act;
            return separator_state_act.next;
        }
        break;
        default: {
            act = state_act.act;
            return state_act.next;
        }
    }
}

int get_pt(int type)
{
    return type - empty_pt;
}
