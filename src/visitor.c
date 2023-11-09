#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>


AST_T* visitor_visit(AST_T* node)
{   
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(node); break;
        case AST_VARIABLE: return visitor_visit_variable(node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(node); break;
        case AST_STRING: return visitor_visit_string(node); break;
        case AST_COMPOUND: return visitor_visit_compound(node); break;
        case AST_NOOP: return node; break;
    }

    printf("Uncaught statement of type %d\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_variable_definition(AST_T* node)
{
    
}

AST_T* visitor_visit_variable(AST_T* node)
{

}

AST_T* visitor_visit_function_call(AST_T* node)
{

}

AST_T* visitor_visit_string(AST_T* node)
{

}

AST_T* visitor_visit_compound(AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        printf("%d\n", i );
        AST_T* child_node = node->compound_value[i];
        visitor_visit(child_node);
    }
    
    return init_ast(AST_NOOP);
}