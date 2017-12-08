#include "parser.h"
#include <iostream>
#include <map>
#include <list>
#include <string>


list<map<string, Var_attributes> > var_table;
stack<Code_attributes> code_stack;

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
    ofstream fout;
    fout.open ("syntax.log");
    map<string, Var_attributes>  vars;
    var_table.push_back(vars);
    for(it = recognized_lexs.begin();it!=recognized_lexs.end();it++) {
        int ret = parse_lex(*it, buf,fout);
        if(ret < 0) {

            switch(ret) {
                case -1: {
                    cout << "Syntax error" << endl;
                    cout << "Line " << it->s_num;
                    cerr << ": Invalid token " << it->token << endl;
                }
                break;
                case undefined_ref_err: {
                    cerr << "Semantic error" << endl;
                    cerr << "Line " << it->s_num;
                    cerr << ": Undefined reference " << it->token << endl;
                }
                break;
                case already_exist_err: {
                    cerr << "Semantic error" << endl;
                    cerr << "Line " << it->s_num;
                    cerr << ": Conflicting declaration " << it->token << endl;
                }
                break;
            }

            return -1;
        }
    }
    if(buf.size() == 1 && buf.top() == get_pt(statements_pt)) {
        cout << "Compilation completed successfuly." << endl;
        ofstream code_out;
        code_out.open ("code.asm");
        code_out << code_stack.top().code_str;

    } else {
        cerr << "Syntax error."<<endl;
        cerr << "Invalid end of file!"<< endl;
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
    parser_machine.add_branch(get_pt(empty_pt), if_5_pt, get_pt(if_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), if_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), else_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), for_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), while_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), block_pt, get_pt(statement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), local_var_decl_pt, get_pt(for_opt_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), for_opt_pt, get_pt(prestatement_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), var_init_pt, get_pt(var_list_pt), reduce_act);
    parser_machine.add_branch(get_pt(empty_pt), expression_pt, get_pt(statement_exp_pt), reduce_act);
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
    parser_machine.add_branch(get_pt(if_4_pt), statement_pt, get_pt(if_5_pt), reduce_act);
    parser_machine.add_branch(get_pt(if_5_pt), reserved_type, get_pt(error_pt), resword_act);
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
    resword_pars_machine.add_branch(get_pt(if_5_pt), else_key, get_pt(else_1_pt), reduce_act);
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

int parse_lex(Lex_attributes lex,stack<int>& buf, ofstream& fout)
{
    int last = buf.top();
    int token_type = lex.token_type;
    int token_val = lex.value.i;
    int act;
    int next = parse_pt(buf.top(), token_type, act,token_val);
    //cout << "act " << act << " last " << buf.top() << " token " << token_type << " next " << next << endl;
    switch (act) {
        case reduce_act: {
            reduce_next(buf, next,lex,fout);
            reduce_last(buf,lex,fout);
        }
        break;
        case shift_act: {
            int next_act;
            next = parse_pt(get_pt(empty_pt) ,token_type, next_act,token_val);
            if(next_act == reduce_act) {
                reduce_one(buf,next,lex,fout);
            } else {
                return -1;
            }
        }
        break;
        case alt_shift_act: {
            int next_act;
            next = parse_pt(get_pt(alt_pt) ,token_type, next_act,token_val);
            if(next_act == reduce_act) {
                reduce_one(buf,next,lex,fout);
            } else {
                return -1;
            }
        }
        break;
        case wrong_act: {
            int next_act;
            if(reduce_last(buf, lex, fout))
            return parse_lex(lex,buf, fout);
            next = parse_pt(get_pt(empty_pt), last + number_of_tokens, next_act);
            if(next_act == reduce_act) {
                reduce_next(buf, next, lex, fout);
                return parse_lex(lex,buf, fout);
            } else {
                return -1;
            }
        }
        default: {

        }
    }
    if(code_stack.top().var_attr.type < 0) {
        return code_stack.top().var_attr.type;
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
        case prestatement_pt:
        return "prestatement_pt";
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

void reduce_next(stack<int>& buf, int next,Lex_attributes lex,ofstream& fout)
{
    fout<<"pop \t"<<get_name_pt(buf.top())<<"\t push \t"<<get_name_pt(next)<<endl;
    buf.pop();
    buf.push(next);
    Code_attributes e_code;
    if(code_stack.size() >= 1) {
        Code_attributes code_attr = get_code(next, lex, code_stack.top(), e_code );
        code_stack.pop();
        code_stack.push(code_attr);
    } else {
        Code_attributes code_attr = get_code(next, lex, e_code, e_code );
        code_stack.push(code_attr);
    }
}

int reduce_last(stack<int>& buf,Lex_attributes lex,ofstream& fout)
{
    if(buf.size()>=2) {
        int next_act;
        int last = buf.top();
        buf.pop();
        int penult = buf.top();
        int new_next = parse_pt(penult ,last + number_of_tokens, next_act);
        if(next_act == reduce_act && new_next != error_pt) {
            fout<<"pop \t"<<get_name_pt(penult)<<"\t pop \t"<<get_name_pt(last)<<",\t push \t"<<get_name_pt(new_next)<<endl;
            buf.pop();
            buf.push(new_next);
            if(code_stack.size()>=2) {
                Code_attributes r_code = code_stack.top();
                code_stack.pop();
                Code_attributes l_code = code_stack.top();
                code_stack.pop();
                Code_attributes code_attr = get_code(new_next, lex, l_code, r_code );
                code_stack.push(code_attr);
            }
            return 1;
        } else {
            buf.push(last);
            return 0;
        }
    }
}

void reduce_one(stack<int>& buf, int next,Lex_attributes lex,ofstream& fout)
{
    fout<<"push \t"<<get_name_pt(next)<<endl;
    buf.push(next);
    Code_attributes e_code;
    Code_attributes code_attr = get_code(next, lex, e_code, e_code );
    code_stack.push(code_attr);
}

Var_attributes find_var(string var_name)
{
    Var_attributes var_attr;
    auto var_table_it = var_table.begin();
    for(;var_table_it != var_table.end();var_table_it++) {
        auto var_attr_it = var_table_it->find(var_name);
        if(var_attr_it != var_table_it->end()) {
            var_attr = var_attr_it->second;
            break;
        }
    }
    return var_attr;
}

Code_attributes get_code(int next_pt, Lex_attributes lex, Code_attributes l_code, Code_attributes r_code)
{
    static int reg_num = 0;
    static int type_i = 0; // int,float, errors
    static int type_u = 0; //++, --, !
    static int type_c = 0; // < > == !=
    static int type_a = 0; // + -
    static int type_e = 0; // && ||
    static int temp_reg_num = 0;
    static int label_num = 0;
    static stack<int> loop_front_label;
    static stack<int> loop_back_label;
    static Code_attributes for_opt;
    Var_attributes var_attr;
    Code_attributes code_attr;
    switch (next_pt + number_of_tokens) {
        case if_5_pt:
        case else_pt:
        case while_pt:
        case for_7_pt:
        break;
        default: {
            code_attr.code_str += l_code.code_str + r_code.code_str;
        }
    }
    switch (next_pt + number_of_tokens) {
        case block_1_pt: {
            map<string,Var_attributes> vars;
            var_table.push_front(vars);
        }
        break;
        case block_pt: {
            reg_num -= var_table.front().size() + temp_reg_num;
            temp_reg_num;
            var_table.pop_front();
        }
        break;
        case id_pt: {
            var_attr = find_var(lex.token);
            if(var_attr.reg_num < 0) {
                var_attr.type = undefined_ref_err;
            }
        }
        break;
        case alt_id_pt: {
            var_attr = find_var(lex.token);
            if(var_attr.reg_num >= 0) {
                var_attr.type = already_exist_err;
            }
            reg_num++;
            var_attr.reg_num = reg_num;
            var_attr.type = type_i;
            var_table.front().insert(pair<string, Var_attributes> (lex.token, var_attr));
        }
        break;
        case type_pt: {
            type_i = lex.value.i;
        }
        break;
        case operand_pt: {
            if(l_code.var_attr.reg_num<=0) {
                reg_num++;
                temp_reg_num++;
                var_attr.type = lex.token_type;
                var_attr.reg_num = reg_num;
                code_attr.code_str+="mov R"+to_string(reg_num)+string(",#");
                switch(lex.token_type) {
                    case int_type: {
                        code_attr.code_str+=to_string(lex.value.i);
                    }
                    break;
                    case float_type: {
                        code_attr.code_str+=to_string(lex.value.d)+"f";
                    }
                    break;
                }
                code_attr.code_str+='\n';
            } else {
                var_attr.reg_num = l_code.var_attr.reg_num;
                var_attr.type = l_code.var_attr.type;
            }
        }
        break;
        case additive_op_pt: {
            type_a = lex.value.i;
            var_attr.reg_num = l_code.var_attr.reg_num;
        }
        break;
        case additive_exp_pt: {
            if(r_code.var_attr.reg_num > 0) {
                reg_num++;
                temp_reg_num++;
                var_attr.reg_num = reg_num;
                if(type_a==plus_val) {
                    code_attr.code_str+=string("add R");
                } else {
                    code_attr.code_str+=string("sub R");
                }
                code_attr.code_str+=to_string(l_code.var_attr.reg_num);
                code_attr.code_str+=",R" + to_string(r_code.var_attr.reg_num);
                code_attr.code_str+=" R"+ to_string(reg_num)+'\n';
            } else {
                var_attr.reg_num = l_code.var_attr.reg_num;
                var_attr.type = l_code.var_attr.type;
            }
        }
        break;
        case var_init_pt:
        case statement_exp_pt: {
            var_attr.reg_num = reg_num;
            if(r_code.var_attr.reg_num>0) {
                code_attr.code_str+=string("mov R") + to_string(l_code.var_attr.reg_num);
                code_attr.code_str+=",R" + to_string(r_code.var_attr.reg_num)+'\n';
            }
        }
        break;
        case unary_op_pt: {
            type_u = lex.value.i;
        }
        break;
        case unary_exp_pt: {
            if(r_code.var_attr.reg_num > 0) {
                if(type_u == exclamation_val) {
                    reg_num++;
                    temp_reg_num++;
                    var_attr.reg_num = reg_num;
                } else {
                    var_attr.reg_num = r_code.var_attr.reg_num;
                    if(type_u == plus_plus_val) {
                        code_attr.code_str+=string("add R");
                    } else {
                        code_attr.code_str+=string("sub R");
                    }
                    code_attr.code_str+=to_string(r_code.var_attr.reg_num);
                    code_attr.code_str+=",#1";
                    code_attr.code_str+=" R"+ to_string(r_code.var_attr.reg_num)+'\n';
                }
            } else {
                var_attr.reg_num = l_code.var_attr.reg_num;
                var_attr.type = l_code.var_attr.type;
            }
        }
        break;
        case log_op_pt: {
            type_c = lex.value.i;
            var_attr.reg_num = l_code.var_attr.reg_num;
        }
        break;
        case log_exp_pt: {
            if(r_code.var_attr.reg_num > 0) {
                reg_num++;
                temp_reg_num++;
                var_attr.reg_num = reg_num;
                code_attr.code_str+=string("cmp R") + to_string(l_code.var_attr.reg_num);
                code_attr.code_str+=string(",R") + to_string(r_code.var_attr.reg_num) + '\n';
                string s_1,s_0;
                switch (type_c) {
                    case more_val: {
                        s_1 = "am R";
                        s_0 = "nm R";
                    }
                    break;
                    case less_val: {
                        s_1 = "al R";
                        s_0 = "nl R";
                    }
                    break;
                    case equal_val: {
                        s_1 = "ae R";
                        s_0 = "ane R";
                    }
                    break;
                    case not_equal_val: {
                        s_1 = "ane R";
                        s_0 = "ae R";
                    }
                    break;
                }
                code_attr.code_str+= s_1 + to_string(reg_num);
                code_attr.code_str+= string(",#1\n");
                code_attr.code_str+= s_0 + to_string(reg_num);
                code_attr.code_str+= string(",#0\n");
            } else {
                var_attr.reg_num = l_code.var_attr.reg_num;
                var_attr.type = l_code.var_attr.type;
            }
        }
        break;
        case expression_op_pt: {
            type_e = lex.value.i;
            var_attr.reg_num = l_code.var_attr.reg_num;
        }
        break;
        case expression_pt: {
            if(r_code.var_attr.reg_num > 0) {
                reg_num++;
                temp_reg_num++;
                var_attr.reg_num = reg_num;
                switch (type_e) {
                    case and_val: {
                        code_attr.code_str+=string("and R");
                    }
                    break;
                    case or_val: {
                        code_attr.code_str+=string("or R");
                    }
                    break;
                }
                code_attr.code_str+=to_string(l_code.var_attr.reg_num);
                code_attr.code_str+=string(",R") + to_string(r_code.var_attr.reg_num);
                code_attr.code_str+=string(" R") + to_string(reg_num)+'\n';
            } else {
                var_attr.reg_num = l_code.var_attr.reg_num;
                var_attr.type = l_code.var_attr.type;
            }
        }
        break;
        case if_3_pt: {
            var_attr.reg_num = r_code.var_attr.reg_num;
            var_attr.type = r_code.var_attr.type;
        }
        break;
        case if_5_pt: {
            label_num+=3;
            code_attr.code_str+=l_code.code_str;
            code_attr.code_str+=string("cmp R") + to_string(l_code.var_attr.reg_num);
            code_attr.code_str+=string(",#0") + '\n';
            code_attr.code_str+=string("ane goto M") + to_string(label_num-2) + '\n';
            code_attr.code_str+=string("ae goto M") + to_string(label_num-1) + '\n';
            code_attr.code_str+=string("M") + to_string(label_num-2) + ":\n";
            code_attr.code_str+=r_code.code_str;
            code_attr.code_str+=string("jmp M") + to_string(label_num) + '\n';

        }
        break;
        case if_pt: {
            code_attr.code_str+=string("M") + to_string(label_num-1) + ":\n";
            code_attr.code_str+=string("M") + to_string(label_num) + ":\n";

        }
        break;
        case else_pt: {
            code_attr.code_str+=l_code.code_str;
            code_attr.code_str+=string("M") + to_string(label_num-1) + ":\n";
            code_attr.code_str+=r_code.code_str;
            code_attr.code_str+=string("M") + to_string(label_num) + ":\n";
        }
        break;
        case while_3_pt: {
            label_num+=2;
            loop_front_label.push(label_num-1);
            loop_back_label.push(label_num);
            var_attr.reg_num = r_code.var_attr.reg_num;
            var_attr.type = r_code.var_attr.type;
        }
        break;
        case while_pt: {
            int l_num = loop_back_label.top();
            code_attr.code_str+=l_code.code_str;
            code_attr.code_str+=string("M") + to_string(l_num-1) + ":\n";
            code_attr.code_str+=string("cmp R") + to_string(l_code.var_attr.reg_num);
            code_attr.code_str+=string(",#0") + '\n';
            code_attr.code_str+=string("ae goto M") + to_string(l_num) + '\n';
            code_attr.code_str+=r_code.code_str;
            code_attr.code_str+=string("jmp M") + to_string(l_num-1) + '\n';
            code_attr.code_str+=string("M") + to_string(l_num) + ":\n";
            loop_front_label.pop();
            loop_back_label.pop();
        }
        break;
        case for_5_pt: {
            label_num+=3;
            loop_front_label.push(label_num-1);
            loop_back_label.push(label_num);
            code_attr.code_str+=string("M") + to_string(label_num-2) + ":\n";
            code_attr.code_str+=string("cmp R") + to_string(r_code.var_attr.reg_num);
            code_attr.code_str+=string(",#0") + '\n';
            code_attr.code_str+=string("ae goto M") + to_string(label_num) + '\n';
        }
        break;
        case for_7_pt: {
            code_attr.code_str+=l_code.code_str;
            for_opt = r_code;
        }
        break;
        case for_pt: {
            int l_num = loop_back_label.top();
            code_attr.code_str+=string("M") + to_string(l_num-1) + ":\n";
            code_attr.code_str+=for_opt.code_str;
            code_attr.code_str+=string("jmp M") + to_string(l_num-2) + '\n';
            code_attr.code_str+=string("M") + to_string(l_num) + ":\n";
            loop_front_label.pop();
            loop_back_label.pop();

        }
        break;
        case continue_pt: {
            code_attr.code_str+=string("jmp M") + to_string(loop_front_label.top()) + "\n";
        }
        break;
        case break_pt: {
            code_attr.code_str+=string("jmp M") + to_string(loop_back_label.top()) + "\n";
        }
        break;
        default: {
            var_attr.reg_num = l_code.var_attr.reg_num;
            var_attr.type = l_code.var_attr.type;
        }
        break;
    }
    code_attr.var_attr = var_attr;
    return code_attr;
}

Var_attributes::Var_attributes(int reg_num, int type)
{
    this->reg_num = reg_num;
    this->type = type;
}

Var_attributes::Var_attributes()
{
    this->reg_num = -1;
    this->type = 0;
}

Code_attributes::Code_attributes(Var_attributes var_attr, string code_str)
{
    this->var_attr = var_attr;
    this->code_str = code_str;
}

Code_attributes::Code_attributes()
{
    Var_attributes var_attr;
    this->var_attr = var_attr;
    this->code_str = "";
}
