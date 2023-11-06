#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

int main(int argc, char* argv[])
{
    lexer_T* lexer = init_lexer(
        "var name = \"John Doe\";"
        "print(name);"
    );
                        
    parser_T* parser = init_parser(lexer);
    AST_T* root =  parser_parse(parser);

    printf("%d\n", root->type);
    printf("%ld\n", root->compound_size);
    //printf("%d\n", root->compound_value[0]);

    
    return 0;
}