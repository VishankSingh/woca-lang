#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->previous_token = parser->current_token;
    return parser;
}

void parser_eat(parser_T* parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->previous_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf("Unexpected token '%s' with type '%d' expected '%d'\n", 
                parser->current_token->value, 
                parser->current_token->type, token_type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser)
{
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser)
{
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);
        AST_T* ast_statement = parser_parse_statement(parser);
        if (ast_statement != NULL)
        {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value, 
                compound->compound_size * sizeof(struct AST_STRUCT*)
            );
            compound->compound_value[compound->compound_size - 1] = ast_statement;
        }
    }
    
    return compound;
}

AST_T* parser_parse_expr(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser);
        case TOKEN_ID: return parser_parse_id(parser);
        default:
            printf("Unexpected token '%s' with type '%d'\n", 
                    parser->current_token->value, 
                    parser->current_token->type
            );
            exit(1);
    }
}

AST_T* parser_parse_factor(parser_T* parser)
{

}

AST_T* parser_parse_term(parser_T* parser)
{

}

AST_T* parser_parse_function_call(parser_T* parser)
{
    parser_eat(parser, TOKEN_LPAREN);
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);
    function_call->function_call_name = parser->previous_token->value;
    function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_expr = parser_parse_expr(parser);
    function_call->function_call_arguments[0] = ast_expr;
    function_call->function_call_arguments_size += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        AST_T* ast_expr = parser_parse_expr(parser);
        if (ast_expr != NULL)
        {
            function_call->function_call_arguments_size += 1;
            function_call->function_call_arguments = realloc(
                function_call->function_call_arguments, 
                function_call->function_call_arguments_size * sizeof(struct AST_STRUCT*)
            );
            function_call->function_call_arguments[function_call->function_call_arguments_size - 1] = ast_expr;
        }
    }

    parser_eat(parser, TOKEN_RPAREN);

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID);
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);
    parser_eat(parser, TOKEN_EQUALS);
    AST_T* variable_definition_value = parser_parse_expr(parser);

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    return variable_definition;
}

AST_T* parser_parse_variable(parser_T* parser)
{
    char* token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN)
    {
        return parser_parse_function_call(parser);
    }

    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    return ast_string;
}

AST_T* parser_parse_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "var") == 0)
    {
        return parser_parse_variable_definition(parser);
    }
    else if (strcmp(parser->current_token->value, "string") == 0)
    {
        return parser_parse_string(parser);
    }

    
    else
    {
        return parser_parse_variable(parser);
    }
}











