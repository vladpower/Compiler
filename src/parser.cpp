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
            cout << "Line " << it->s_num << ": Invalid token " << it->token << endl;
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
    parser_machine.add_branch(get_pt(empty_pt), alt_id_pt, get_pt(var_init_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), operand_pt, get_pt(unary_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), unary_exp_pt, get_pt(additive_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), additive_exp_pt, get_pt(log_exp_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), log_exp_pt, get_pt(expression_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), if_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), else_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), for_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), while_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), block_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), local_var_decl_pt, get_pt(for_opt_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), for_opt_pt, get_pt(prestatement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), var_init_pt, get_pt(var_list_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), statement_exp_pt, get_pt(for_opt_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), statement_pt, get_pt(statements_pt), reduce_act);

    parser_machine.add_branch(get_pt(alt_pt), id_type, get_pt(alt_id_pt), reduce_act);

    parser_machine.add_branch(get_pt(id_pt), assign_type, get_pt(statement_op_pt), reduce_act);
    parser_machine.add_branch(get_pt(alt_id_pt), assign_type, get_pt(var_assign_pt), reduce_act);

    add_default_shifts(unary_op_pt);
    parser_machine.add_branch(get_pt(unary_op_pt), operand_pt, get_pt(unary_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(unary_exp_pt), arifm_op_type, get_pt(additive_op_pt), reduce_act);

    add_default_shifts(additive_op_pt);
    parser_machine.add_branch(get_pt(additive_op_pt), additive_exp_pt, get_pt(additive_exp_pt), reduce_act);

    add_default_shifts(log_op_pt);
    parser_machine.add_branch(get_pt(log_op_pt), additive_exp_pt, get_pt(log_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(log_exp_pt), comp_op_type , get_pt(log_op_pt), reduce_act);

    parser_machine.add_branch(get_pt(log_exp_pt), logic_op_type , get_pt(expression_op_pt), reduce_act);

    add_default_shifts(expression_op_pt);
    parser_machine.add_branch(get_pt(expression_op_pt), expression_pt, get_pt(expression_pt), reduce_act);

    //parser_machine.add_branch(get_pt(expression_pt), separator_type, get_pt(error_pt), separator_act); // ;

    add_default_shifts(statement_op_pt);
    parser_machine.add_branch(get_pt(statement_op_pt), expression_pt, get_pt(statement_exp_pt), reduce_act);

    parser_machine.add_branch(get_pt(statement_exp_pt), separator_type, get_pt(error_pt), separator_act); // ;
    //parser_machine.add_branch(get_pt(local_var_statement_pt), separator_type, get_pt(error_pt), separator_act); // ;

    parser_machine.add_branch(get_pt(type_pt), id_type, get_pt(error_pt), alt_shift_act);
    parser_machine.add_branch(get_pt(type_pt), var_list_pt, get_pt(local_var_decl_pt), reduce_act);

    parser_machine.add_branch(get_pt(prestatement_pt), separator_type, get_pt(error_pt), separator_act);


    add_default_shifts(var_assign_pt);
    parser_machine.add_branch(get_pt(var_assign_pt), expression_pt, get_pt(var_init_pt), reduce_act);
    parser_machine.add_branch(get_pt(var_init_pt), separator_type, get_pt(empty_pt), separator_act); // , ;
    parser_machine.add_branch(get_pt(var_comma_pt), id_type, get_pt(error_pt), alt_shift_act);
    parser_machine.add_branch(get_pt(var_comma_pt), var_list_pt, get_pt(var_list_pt), reduce_act);


    parser_machine.add_branch(get_pt(if_1_pt), separator_type, get_pt(error_pt), separator_act); // (
    add_default_shifts(if_2_pt);
    parser_machine.add_branch(get_pt(if_2_pt), expression_pt, get_pt(if_3_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_3_pt), separator_type, get_pt(error_pt), separator_act); // )
    add_default_shifts(if_4_pt);
    parser_machine.add_branch(get_pt(if_4_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(if_4_pt), statement_pt, get_pt(if_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_pt), reserved_type, get_pt(error_pt), resword_act);
    add_default_shifts(else_1_pt);
    parser_machine.add_branch(get_pt(else_1_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(else_1_pt), statement_pt, get_pt(else_pt), reduce_act);

    parser_machine.add_branch(get_pt(for_1_pt), separator_type, get_pt(error_pt), separator_act); // ()
    parser_machine.add_branch(get_pt(for_2_pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(for_2_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(for_2_pt), for_opt_pt, get_pt(for_3_pt), reduce_act);
    parser_machine.add_branch(get_pt(for_3_pt), separator_type, get_pt(error_pt), separator_act); // ;
    add_default_shifts(for_4_pt);
    parser_machine.add_branch(get_pt(for_4_pt), expression_pt, get_pt(for_5_pt), reduce_act);
    parser_machine.add_branch(get_pt(for_5_pt), separator_type, get_pt(error_pt), separator_act); // ;
    add_default_shifts(for_6_pt);
    parser_machine.add_branch(get_pt(for_6_pt), statement_exp_pt, get_pt(for_7_pt), reduce_act);
    parser_machine.add_branch(get_pt(for_6_pt), expression_pt, get_pt(for_7_pt), reduce_act);
    parser_machine.add_branch(get_pt(for_7_pt), separator_type, get_pt(error_pt), separator_act); // )
    add_default_shifts(for_8_pt);
    parser_machine.add_branch(get_pt(for_8_pt), separator_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(for_8_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(for_8_pt), statement_pt, get_pt(for_pt), reduce_act);

    parser_machine.add_branch(get_pt(while_1_pt), separator_type, get_pt(error_pt), separator_act);
    add_default_shifts(while_2_pt);
    parser_machine.add_branch(get_pt(while_2_pt), expression_pt, get_pt(while_3_pt), reduce_act);
    parser_machine.add_branch(get_pt(while_3_pt), separator_type, get_pt(error_pt), separator_act);
    add_default_shifts(while_4_pt);
    parser_machine.add_branch(get_pt(while_4_pt), separator_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(while_4_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(while_4_pt), statement_pt, get_pt(while_pt), reduce_act);

    parser_machine.add_branch(get_pt(break_pt), separator_type, get_pt(error_pt), separator_act);
    parser_machine.add_branch(get_pt(continue_pt), separator_type, get_pt(error_pt), separator_act);

    add_default_shifts(block_1_pt);
    parser_machine.add_branch(get_pt(block_1_pt), separator_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(block_1_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(block_1_pt), statements_pt, get_pt(block_2_pt), reduce_act);
    parser_machine.add_branch(get_pt(block_2_pt), separator_type, get_pt(error_pt), separator_act);

    add_default_shifts(statements_pt);
    parser_machine.add_branch(get_pt(statements_pt), reserved_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(statements_pt), statement_pt, get_pt(statements_pt), reduce_act);
    parser_machine.add_branch(get_pt(statements_pt), separator_type, get_pt(error_pt), separator_act);
    parser_machine.add_branch(get_pt(statements_pt), end_type, get_pt(statements_pt), reduce_act);
}

State_machine resword_pars_machine(get_pt(number_of_pars),number_of_keys, wrong_act);
void init_resword_pars_machine()
{
    resword_pars_machine.add_branch(get_pt(empty_pt), if_key, get_pt(if_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(if_pt), else_key, get_pt(else_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), bool_key, get_pt(type_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), int_key, get_pt(type_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), double_key, get_pt(type_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), for_key, get_pt(for_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), while_key, get_pt(while_1_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), continue_key, get_pt(continue_pt), reduce_act);
    resword_pars_machine.add_branch(get_pt(empty_pt), break_key, get_pt(break_pt), reduce_act);
}

State_machine separator_pars_machine(get_pt(number_of_pars),number_of_separators, wrong_act);
void init_separator_pars_machine()
{
    separator_pars_machine.add_branch(get_pt(empty_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(empty_pt), left_brace, get_pt(block_1_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(block_2_pt), right_brace, get_pt(block_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(statements_pt), semicolon, get_pt(error_pt), shift_act);
    separator_pars_machine.add_branch(get_pt(if_1_pt), left_parenthesis, get_pt(if_2_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(if_3_pt), right_parenthesis, get_pt(if_4_pt), reduce_act);
    //separator_pars_machine.add_branch(get_pt(local_var_statement_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(statement_exp_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(prestatement_pt), semicolon, get_pt(statement_pt), reduce_act);

    separator_pars_machine.add_branch(get_pt(var_init_pt), comma, get_pt(var_comma_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(local_var_decl_pt), comma, get_pt(var_comma_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(for_1_pt), left_parenthesis, get_pt(for_2_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(for_3_pt), semicolon, get_pt(for_4_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(for_5_pt), semicolon, get_pt(for_6_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(for_7_pt), right_parenthesis, get_pt(for_8_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(while_1_pt), left_parenthesis, get_pt(while_2_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(while_3_pt), right_parenthesis, get_pt(while_4_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(continue_pt), semicolon, get_pt(statement_pt), reduce_act);
    separator_pars_machine.add_branch(get_pt(break_pt), semicolon, get_pt(statement_pt), reduce_act);

}

void add_default_shifts(int pt)
{
    parser_machine.add_branch(get_pt(pt), int_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(pt), float_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(pt), bool_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(pt), id_type, get_pt(error_pt), shift_act);
    parser_machine.add_branch(get_pt(pt), unary_type, get_pt(error_pt), shift_act);
}

int parse_lex(Lex_attributes lex,stack<int>& buf)
{
    int last = buf.top();
    int token_type = lex.token_type;
    int token_val = lex.value.i;
    int act;
    int next = parse_pt(buf.top(), token_type, act,token_val);
    //cout << "act " << act << " last " << buf.top() << " token " << token_type << " next " << next << endl;
    switch (act) {
        case reduce_act: {
            cout<<"pop \t"<<get_name_pt(buf.top())<<"\t push \t"<<get_name_pt(next)<<endl;
            buf.pop();
            buf.push(next);
            if(buf.size()>=2) {
                int next_act;
                buf.pop();
                int penult = buf.top();
                int new_next = parse_pt(penult ,last + number_of_tokens, next_act);
                //cout << "act " << next_act << " last " << penult << " pt " << last << " next " << next << endl;
                if(next_act == reduce_act && new_next != error_pt) {
                    cout<<"pop \t"<<get_name_pt(penult)<<"\t pop \t"<<get_name_pt(last)<<",\t push \t"<<get_name_pt(new_next)<<endl;
                    buf.pop();
                    buf.push(new_next);
                } else {
                    buf.push(next);
                }
            }


        }
        break;
        case shift_act: {
            int next_act;
            next = parse_pt(get_pt(empty_pt) ,token_type, next_act,token_val);
            if(next_act == reduce_act) {
                cout<<"push \t"<<get_name_pt(next)<<endl;
                buf.push(next);
            } else {
                return -1;
            }
        }
        break;
        case alt_shift_act: {
            int next_act;
            next = parse_pt(get_pt(alt_pt) ,token_type, next_act,token_val);
            if(next_act == reduce_act) {
                cout<<"push \t"<<get_name_pt(next)<<endl;
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
                //cout << "act " << next_act << " last " << penult << " pt " << last << " next " << next << endl;
                if(next_act != wrong_act && next != error_pt) {
                    cout<<"pop \t"<<get_name_pt(penult)<<"\t pop \t"<<get_name_pt(last)<<"\t push \t"<<get_name_pt(next)<<endl;
                    buf.pop();
                    buf.push(next);
                    return parse_lex(lex,buf);
                } else {
                    buf.push(last);
                }
            }
            next = parse_pt(get_pt(empty_pt), last + number_of_tokens, next_act);
            if(next_act == reduce_act) {
                cout<<"pop \t" << get_name_pt(buf.top()) << "\t push \t"<<get_name_pt(next)<<endl;
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

string get_name_pt(int pt)
{
    switch(pt+number_of_tokens) {
        // case id_type:
        // return "id_type";
        // case reserved_type:
        // return "reserved_type";
        // case assign_type:
        // return "assign_type";
        // case unary_type:
        // return "unary_type";
        // case arifm_op_type:
        // return "arifm_op_type";
        // case logic_op_type:
        // return "logic_op_type";
        // case comp_op_type:
        // return "comp_op_type";
        // case separator_type:
        // return "separator_type";
        // case int_type:
        // return "int_type";
        // case float_type:
        // return "float_type";
        // case bool_type:
        // return "bool_type";
        // case end_type:
        // return "end_type";
        case empty_pt:
        return "empty_pt";
        case operand_pt:
        return "operand_pt";
        case id_pt:
        return "id_pt";
        case alt_id_pt:
        return "alt_id_pt";
        case unary_op_pt:
        return "unary_op_pt";
        case unary_exp_pt:
        return "unary_exp_pt";
        case additive_op_pt:
        return "additive_op_pt";
        case additive_exp_pt:
        return "additive_exp_pt";
        case log_op_pt:
        return "log_op_pt";
        case log_exp_pt:
        return "log_exp_pt";
        case expression_op_pt:
        return "expression_op_pt";
        case expression_pt:
        return "expression_pt";
        case statement_op_pt:
        return "statement_op_pt";
        case statement_exp_pt:
        return "statement_exp_pt";
        case local_var_decl_pt:
        return "local_var_decl_pt";
        case var_comma_pt:
        return "var_comma_pt";
        case var_assign_pt:
        return "var_assign_pt";
        case var_list_pt:
        return "var_list_pt";
        case var_init_pt:
        return "var_init_pt";
        case type_pt:
        return "type_pt";
        case statement_pt:
        return "statement_pt";
        case statements_pt:
        return "statements_pt";
        case for_opt_pt:
        return "for_opt_pt";
        case if_1_pt:
        return "if_1_pt";
        case if_2_pt:
        return "if_2_pt";
        case if_3_pt:
        return "if_3_pt";
        case if_4_pt:
        return "if_4_pt";
        case if_pt:
        return "if_pt";
        case else_1_pt:
        return "else_1_pt";
        case else_pt:
        return "else_pt";
        case for_1_pt:
        return "for_1_pt";
        case for_2_pt:
        return "for_2_pt";
        case for_3_pt:
        return "for_3_pt";
        case for_4_pt:
        return "for_4_pt";
        case for_5_pt:
        return "for_5_pt";
        case for_6_pt:
        return "for_6_pt";
        case for_7_pt:
        return "for_7_pt";
        case for_8_pt:
        return "for_8_pt";
        case for_pt:
        return "for_pt";
        case while_1_pt:
        return "while_1_pt";
        case while_2_pt:
        return "while_2_pt";
        case while_3_pt:
        return "while_3_pt";
        case while_4_pt:
        return "while_4_pt";
        case while_pt:
        return "while_pt";
        case break_pt:
        return "break_pt";
        case continue_pt:
        return "continue_pt";
        case block_1_pt:
        return "block_1_pt";
        case block_2_pt:
        return "block_2_pt";
        case block_pt:
        return "block_pt";
        case error_pt:
        return "error_pt";
        case alt_pt:
        return "alt_pt";
        default:
        return to_string(pt);
    }
}
