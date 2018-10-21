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
%expect 4

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
    LILC::FnBodyNode * fnBodyNode;
    LILC::FnDeclNode * fnDeclNode;
    LILC::StmtListNode * stmtListNode;
    LILC::StructDeclNode * structDeclNode;
    LILC::FormalsListNode * formalsListNode;
    LILC::FormalDeclNode * formalDeclNode;
    LILC::StmtNode * stmtNode;
    LILC::ExpNode * expNode;
    LILC::AssignNode * assignNode;
    LILC::AssignStmtNode * assignStmtNode;
    LILC::VarDeclListNode * varDeclList;
    LILC::ExpListNode * expListNode;
    LILC::CallExpNode * callExpNode;
    LILC::IfStmtNode * ifStmtNode;
    LILC::IfElseStmtNode * ifElseStmtNode;
    /*LILC::Token * token;*/
    LILC::IntLitToken * intLit;
    LILC::StringLitToken * stringLit;
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
%token <intLit>      INTLITERAL
%token <stringLit>   STRINGLITERAL
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
%type <fnDeclNode> fnDecl
%type <formalsListNode> formals
%type <formalsListNode> formalsList
%type <formalDeclNode> formalDecl
%type <fnBodyNode> fnBody
%type <stmtListNode> stmtList
%type <varDeclList> varDeclList structBody
%type <structDeclNode> structDecl
%type <expNode> exp loc expt expf term
%type <assignNode> assignExp
%type <stmtNode> stmt
%type <callExpNode> fncall
%type <expListNode> actualList

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
  $$ = new StructDeclNode($2, $4, 0);
}

structBody : structBody varDecl {
  $1->add($2);
  $$ = $1;
             }
        |    varDecl {
            $$ = new VarDeclListNode(new std::list<VarDeclNode *>());
            $$->add($1);
             }

fnDecl : type id formals fnBody {
    $$ = new FnDeclNode($1, $2, $3, $4, 0);
}

formals : LPAREN RPAREN {
            $$ = nullptr;
        }
    | LPAREN formalsList RPAREN {
            $$ = $2;
        }

formalsList : formalDecl {
            $$ = new FormalsListNode(new std::list<FormalDeclNode *>());
            $$->add($1);
        }
    | formalsList COMMA formalDecl {
            $1->add($3);
            $$ = $1;
        }

formalDecl : type id {
            $$ = new FormalDeclNode($1, $2, 0);
}

fnBody : LCURLY varDeclList stmtList RCURLY {
    $$ = new FnBodyNode($2, $3);
}

varDeclList : varDeclList varDecl {
          $1->add($2);
          $$ = $1;
            }
        | /* epsilon */ {
          $$ = new VarDeclListNode(new std::list<VarDeclNode *>());
        }

stmtList : stmtList stmt {
            $1->add($2);
            $$ = $1;
        }
    | /* epsilon */ {
            $$ = new StmtListNode(new std::list<StmtNode *>());
        }

stmt : assignExp SEMICOLON {
            $$ = new AssignStmtNode($1);
        }
         | loc PLUSPLUS SEMICOLON {
            $$ = new PostIncStmtNode($1);
           }
         | loc MINUSMINUS SEMICOLON {
            $$ = new PostDecStmtNode($1);
           }
         | INPUT READ loc SEMICOLON {
            $$ = new ReadStmtNode($3);
           }
         | OUTPUT WRITE exp SEMICOLON {
            $$ = new WriteStmtNode($3);
           }
         | IF LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY ELSE LCURLY varDeclList stmtList RCURLY {
            $$ = new IfElseStmtNode($3, $6, $7, $11, $12);
           }
         | IF LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY {
            $$ = new IfStmtNode($3, $6, $7);
           }
         |  WHILE LPAREN exp RPAREN LCURLY varDeclList stmtList RCURLY {
            $$ = new WhileStmtNode($3, $6, $7);
           }
         | RETURN exp SEMICOLON {
            $$ = new ReturnStmtNode($2);
           }
         | RETURN SEMICOLON {
            $$ = new ReturnStmtNode(nullptr);
           }
         | fncall SEMICOLON {
            $$ = new CallStmtNode($1);
           }

assignExp : loc ASSIGN exp {
    $$ = new AssignNode($1, $3);
}

loc : id {} | loc DOT id {
        $$ = new DotAccessNode($1, $3);
    }

exp : assignExp {}
     | exp PLUS expt {
        $$ = new PlusNode($1, $3);
     }
     | exp MINUS expt {
        $$ = new MinusNode($1, $3);
     }
     | expt {}
     | NOT exp {
        $$ = new NotNode($2);
       }
     | term AND term {
        $$ = new AndNode($1, $3);
       }
     | term OR term {
        $$ = new OrNode($1, $3);
       }
     | term EQUALS term {
        $$ = new EqualsNode($1, $3);
       }
     | term NOTEQUALS term {
        $$ = new NotEqualsNode($1, $3);
       }
     | term LESS term {
        $$ = new LessNode($1, $3);
       }
     | term GREATER term {
        $$ = new GreaterNode($1, $3);
       }
     | term LESSEQ term {
        $$ = new LessEqNode($1, $3);
       }
     | term GREATEREQ term {
        $$ = new GreaterEqNode($1, $3);
       }

expt : expt TIMES expf {
            $$ = new TimesNode($1, $3);
        }
    | expt DIVIDE expf {
            $$ =  new DivideNode($1, $3);
        }
    | expf {}

expf : term {}
    | MINUS term {
        $$ = new UnaryMinusNode($2);
    }

term : loc {
       }
     | INTLITERAL {
        $$ = new IntLitNode($1);
       }
     | STRINGLITERAL {
        $$ = new StringLitNode($1);
       }
     | TRUE {
        $$ = new TrueNode();
       }
     | FALSE {
        $$ = new FalseNode();
       }
     | LPAREN exp RPAREN {
        $$ = $2;
       }
     | fncall {}

fncall : id LPAREN RPAREN {
            $$ = new CallExpNode($1, nullptr);
        }
    | id LPAREN actualList RPAREN {
            $$ = new CallExpNode($1, $3);
        }

actualList : exp {
            $$ = new ExpListNode(new std::list<ExpNode *>());
            $$->add($1);
        }
    | actualList COMMA exp {
        $1->add($3);
        $$ = $1;
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
