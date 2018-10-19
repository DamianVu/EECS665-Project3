%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {LILC}
%define parser_class_name {LilC_Parser}
%output "lilc_parser.cc"
%token-table

%code requires{
   #include <list>
   #include "symbols.hpp"
   #include "ast.hpp"
   namespace LILC {
      class LilC_Compiler;
      class LilC_Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { LilC_Scanner  &scanner  }
%parse-param { LilC_Compiler &compiler }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for interoperation between scanner/parser */
   #include "lilc_compiler.hpp"

#undef yylex
#define yylex scanner.yylex
}

/*%define api.value.type variant*/
%union {
    /*
    int intVal;
    std::string * strVal;
    */
    LILC::SynSymbol * symbolValue;
    LILC::IDToken * idTokenValue;
    LILC::ASTNode * astNode;
    LILC::ProgramNode * programNode;
    std::list<DeclNode *> * declList;
    LILC::DeclNode * declNode;
    LILC::VarDeclNode * varDeclNode;
    LILC::TypeNode * typeNode;
    LILC::IdNode * idNode;
    /*LILC::Token * token;*/
}

%define parse.assert

%token               END    0     "end of file"
%token               NEWLINE "newline"
%token               CHAR
%token               BOOL
%token               INT
%token               VOID
%token               TRUE
%token               FALSE
%token               STRUCT
%token               INPUT
%token               OUTPUT
%token               IF
%token               ELSE
%token               WHILE
%token               RETURN
%token <idTokenValue> ID
%token               INTLITERAL
%token               STRINGLITERAL
%token               LCURLY
%token               RCURLY
%token               LPAREN
%token               RPAREN
%token               SEMICOLON
%token               COMMA
%token               DOT
%token               WRITE
%token               READ
%token               PLUSPLUS
%token               MINUSMINUS
%token               PLUS
%token               MINUS
%token               TIMES
%token               DIVIDE
%token               NOT
%token               AND
%token               OR
%token               EQUALS
%token               NOTEQUALS
%token               LESS
%token               GREATER
%token               LESSEQ
%token               GREATEREQ
%token               ASSIGN

/* Nonterminals
*  NOTE: You will need to add more nonterminals
*  to this list as you add productions to the grammar
*  below.
*/
%type <programNode> program
%type <declList> declList
%type <declNode> decl
%type <varDeclNode> varDecl
%type <typeNode> type
%type <idNode> id


/* NOTE: Make sure to add precedence and associativity
 * declarations
*/

%%

program : declList {
           //$$ = new ProgramNode(new DeclListNode($1));
           $$ = new ProgramNode(new DeclListNode($1));
           compiler.setASTRoot($$);
           }
    ;

declList : declList decl {
             $1->push_back($2);
             $$ = $1;
             }
    | /* epsilon */ {
            $$ = new std::list<DeclNode *>();
            }
    ;
// Bison adds '$$ = $1' for empty bracket definitions by default
decl : varDecl {} | structDecl {} | fnDecl {}

varDecl : type id SEMICOLON {
  $$ = new VarDeclNode($1, $2, VarDeclNode::NOT_STRUCT);
}

structDecl : STRUCT id LCURLY structBody RCURLY SEMICOLON {

}

structBody : structBody varDecl {

             }
        |    varDecl {

             }

fnDecl : type id formals fnBody {

}

formals : LPAREN RPAREN {

        }
    | LPAREN formalsList RPAREN {

        }

formalsList : formalDecl {

        }
    | formalDecl COMMA formalsList {

        }

formalDecl : type id {

}

fnBody : LCURLY varDeclList stmtList RCURLY {

}

varDeclList : varDeclList varDecl {

            }
        | /* epsilon */ {

        }

stmtList : stmtList stmt {

        }
    | /* epsilon */ {

        }

stmt : assignExp SEMICOLON { 
           }
         | loc PLUSPLUS SEMICOLON {
           }
         | loc MINUSMINUS SEMICOLON {
           }
         | INPUT READ loc SEMICOLON {
           }
         | OUTPUT WRITE exp SEMICOLON {
           }
         | IF LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY ELSE LCURLY varDeclList stmtList RCURLY {
           }
         | IF LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY {
           }
         |  WHILE LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY {
           }
         | RETURN exp SEMICOLON {
           }
         | RETURN SEMICOLON {
           }
         | fncall SEMICOLON {
           }

assignExp : loc ASSIGN exp {

}

loc : id {

    }
| loc DOT id {

    }

exp : assignExp {
       }
     | exp PLUS expt {

     }
     | exp MINUS expt {

     }
     | expt {

     }
     | NOT exp {
       }
     | exp AND exp {
       }
     | exp OR exp {
       }
     | exp EQUALS exp {
       }
     | exp NOTEQUALS exp {
       }
     | exp LESS exp {
       }
     | exp GREATER exp {
       }
     | exp LESSEQ exp {
       }
     | exp GREATEREQ exp {
       }
    }

expt : expt TIMES expf {
    
        }
    | expt DIVIDE expf {

        }
    | expf {

    }

expf : term {
    
    } 
    | MINUS term {

    }



term : loc {
       }
     | INTLITERAL {
       }
     | STRINGLITERAL {
       }
     | TRUE {
       }
     | FALSE {
       }
     | LPAREN exp RPAREN {
       }
     | fncall {
       }


fncall : id LPAREN RPAREN {
    
        }
    | id LPAREN actualList RPAREN {

        }

actualList : exp {
    
        }
    | actualList COMMA exp {
    
    }

type : INT { $$ = new IntNode(); }
     | BOOL { $$ = new BoolNode(); }
     | VOID { $$ = new VoidNode(); }

id : ID { $$ = new IdNode($1); }
%%
void
LILC::LilC_Parser::error(const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
