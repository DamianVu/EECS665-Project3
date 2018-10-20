#ifndef LILC_AST_HPP
#define LILC_AST_HPP

#include <ostream>
#include <list>
#include "symbols.hpp"

//Here is a suggestion for all the different kinds of AST nodes
// and what kinds
// of children they have. All of these kinds of AST nodes are
// subclasses of ASTNode.
// Indentation indicates further subclassing:
//
//     Subclass		Children
//     --------		------
//     ProgramNode	DeclListNode
//     DeclListNode	linked list of DeclNode
//     DeclNode
//       VarDeclNode	TypeNode, IdNode, int
//       FnDeclNode	TypeNode, IdNode, FormalsListNode, FnBodyNode
//       FormalDeclNode    TypeNode, IdNode
//       StructDeclNode    IdNode, DeclListNode
//
//     FormalsListNode     linked list of FormalDeclNode
//     FnBodyNode          DeclListNode, StmtListNode
//     StmtListNode        linked list of StmtNode
//     ExpListNode         linked list of ExpNode
//
//     TypeNode:
//       IntNode           -- none --
//       BoolNode          -- none --
//       VoidNode          -- none --
//       StructNode        IdNode
//
//     StmtNode:
//       AssignStmtNode      AssignNode
//       PostIncStmtNode     ExpNode
//       PostDecStmtNode     ExpNode
//       ReadStmtNode        ExpNode
//       WriteStmtNode       ExpNode
//       IfStmtNode          ExpNode, DeclListNode, StmtListNode
//       IfElseStmtNode      ExpNode, DeclListNode, StmtListNode,
//                                    DeclListNode, StmtListNode
//       WhileStmtNode       ExpNode, DeclListNode, StmtListNode
//       CallStmtNode        CallExpNode
//       ReturnStmtNode      ExpNode
//
//     ExpNode:
//       IntLitNode          -- none --
//       StrLitNode          -- none --
//       TrueNode            -- none --
//       FalseNode           -- none --
//       IdNode              -- none --
//       DotAccessNode       ExpNode, IdNode
//       AssignNode          ExpNode, ExpNode
//       CallExpNode         IdNode, ExpListNode
//       UnaryExpNode        ExpNode
//         UnaryMinusNode
//         NotNode
//       BinaryExpNode       ExpNode ExpNode
//         PlusNode
//         MinusNode
//         TimesNode
//         DivideNode
//         AndNode
//         OrNode
//         EqualsNode
//         NotEqualsNode
//         LessNode
//         GreaterNode
//         LessEqNode
//         GreaterEqNode
//
//
// Here are the different kinds of AST nodes again, organized according to
// whether they are leaves, internal nodes with linked lists of kids, or
// internal nodes with a fixed number of kids:
//
// (1) Leaf nodes:
//        IntNode,   BoolNode,  VoidNode,  IntLitNode,  StrLitNode,
//        TrueNode,  FalseNode, IdNode
//
// (2) Internal nodes with (possibly empty) linked lists of children:
//        DeclListNode, FormalsListNode, StmtListNode, ExpListNode
//
// (3) Internal nodes with fixed numbers of kids:
//        ProgramNode,     VarDeclNode,     FnDeclNode,     FormalDeclNode,
//        StructDeclNode,  FnBodyNode,      StructNode,     AssignStmtNode,
//        PostIncStmtNode, PostDecStmtNode, ReadStmtNode,   WriteStmtNode
//        IfStmtNode,      IfElseStmtNode,  WhileStmtNode,  CallStmtNode
//        ReturnStmtNode,  DotAccessNode,   CallExpNode,
//        UnaryExpNode,    BinaryExpNode,   UnaryMinusNode, NotNode,
//        PlusNode,        MinusNode,       TimesNode,      DivideNode,
//        AndNode,         OrNode,          EqualsNode,     NotEqualsNode,
//        LessNode,        GreaterNode,     LessEqNode,     GreaterEqNode
//
// **********************************************************************

// **********************************************************************
// ASTnode class (base class for all other kinds of nodes)
// **********************************************************************

namespace LILC{

class SymSymbol;
class DeclListNode;
class DeclNode;
class TypeNode;
class IdNode;
class StructDeclNode;
class StmtNode;
class FormalDeclNode;
class ExpNode;
class StmtNode;
class AssignNode;
class AssignStmtNode;
class CallExpNode;
class VarDeclListNode;
class ExpListNode;

class ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
	void doIndent(std::ostream& out, int indent){
		for (int k = 0 ; k < indent; k++){ out << " "; }
	}
};

class StmtNode : public ASTNode {
public:
	StmtNode() : ASTNode(){}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class ExpNode : public ASTNode {
public:
	ExpNode() : ASTNode() {}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class UnaryExpNode : public ExpNode {
public:
	UnaryExpNode() : ExpNode() {}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class BinaryExpNode : public ExpNode {
public:
	BinaryExpNode() : ExpNode() {}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class ProgramNode : public ASTNode{
public:
	ProgramNode(DeclListNode * L) : ASTNode(){
		myDeclList = L;
	}
	void unparse(std::ostream& out, int indent);
private:
	DeclListNode * myDeclList;

};

class DeclListNode : public ASTNode{
public:
	DeclListNode(std::list<DeclNode *> * decls) : ASTNode(){
		myDecls = *decls;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<DeclNode *> myDecls;
};

class FormalsListNode : public ASTNode {
public:
	FormalsListNode(std::list<FormalDeclNode *> * decls) : ASTNode() {
		myFormals = *decls;
		count = 0;
	}
	void unparse(std::ostream& out, int indent);
	void add(FormalDeclNode * formal) {
		myFormals.push_back(formal);
		count++;
	}
private:
	std::list<FormalDeclNode *> myFormals;
	int count;
};

class DeclNode : public ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class VarDeclNode : public DeclNode{
public:
	VarDeclNode(TypeNode * type, IdNode * id, int size) : DeclNode(){
		myType = type;
		myId = id;
		mySize = size;
	}
	void unparse(std::ostream& out, int indent);
	static const int NOT_STRUCT = -1; //Use this value for mySize
					  // if this is not a struct type
private:
	TypeNode * myType;
	IdNode * myId;
	int mySize;
};

class FormalDeclNode : public DeclNode {
public:
	FormalDeclNode(TypeNode * type, IdNode * id, int size) : DeclNode() {
		myType = type;
		myId = id;
		mySize = size;
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IdNode * myId;
	int mySize;
};

class StmtListNode : public ASTNode {
public:
	StmtListNode(std::list<StmtNode *> * list) : ASTNode() {
		myList = *list;
	}
	void unparse(std::ostream& out, int indent);
	void add(StmtNode * stmt) {
		myList.push_back(stmt);
	}
private:
	std::list<StmtNode *> myList;
};

class FnBodyNode : public ASTNode {
public:
	FnBodyNode(VarDeclListNode * varDeclList, StmtListNode * stmtList) : ASTNode() {
		myDecls = varDeclList;
		myStmts = stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	VarDeclListNode * myDecls;
	StmtListNode * myStmts;
};

class FnDeclNode : public DeclNode {
public:
	FnDeclNode(TypeNode * type, IdNode * id, FormalsListNode * formals, FnBodyNode * body, int size) : DeclNode() {
		myType = type;
		myId = id;
		myFormals = formals;
		myBody = body;
		mySize = size;	}
	void unparse(std::ostream& out, int indent);

private:
	TypeNode * myType;
	IdNode * myId;
	FormalsListNode * myFormals;
	FnBodyNode * myBody;
	int mySize;
};
class VarDeclListNode : public ASTNode{
public:
	VarDeclListNode(std::list<VarDeclNode *> * varDecls) : ASTNode(){
		myVarDecls = *varDecls;
	}
	void unparse(std::ostream& out, int indent);
	void add(VarDeclNode * decl) {
		myVarDecls.push_back(decl);
	}
private:
	std::list<VarDeclNode *> myVarDecls;
};

class StructDeclNode : public DeclNode {
public:
	StructDeclNode(IdNode * id, VarDeclListNode * varDecls, int size) : DeclNode(){
		myId = id;
		mySize = size;
		myDecls = varDecls;
	}
	void unparse(std::ostream& out, int indent);
private:
	IdNode * myId;
	int mySize;
	VarDeclListNode * myDecls;
};
// Stmt Nodes

class AssignStmtNode : public StmtNode {
public:
	AssignStmtNode(AssignNode * assign) : StmtNode() {
		myAssign = assign;
	}
	void unparse(std::ostream& out, int indent);
private:
	AssignNode * myAssign;
};

class PostIncStmtNode : public StmtNode {
public:
	PostIncStmtNode(ExpNode * loc) : StmtNode() {
		myLoc = loc;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLoc;
};

class PostDecStmtNode : public StmtNode {
public:
	PostDecStmtNode(ExpNode * loc) : StmtNode() {
		myLoc = loc;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLoc;
};

class ReadStmtNode : public StmtNode {
public:
	ReadStmtNode(ExpNode * loc) : StmtNode() {
		myLoc = loc;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLoc;
};

class WriteStmtNode : public StmtNode {
public:
	WriteStmtNode(ExpNode * loc) : StmtNode() {
		myLoc = loc;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLoc;
};

class ReturnStmtNode : public StmtNode {
public:
	ReturnStmtNode(ExpNode * loc) : StmtNode() {
		myLoc = loc;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLoc;
};

class CallStmtNode : public StmtNode {
public:
	CallStmtNode(CallExpNode * call) : StmtNode() {
		myCall = call;
	}
	void unparse(std::ostream& out, int indent);
private:
	CallExpNode * myCall;
};

class IfStmtNode : public StmtNode {
public:
	IfStmtNode(ExpNode * exp, VarDeclListNode * varList, StmtListNode * stmtList) : StmtNode() {
		myExp = exp;
		myVarList = varList;
		myStmtList = stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myExp;
	VarDeclListNode * myVarList;
	StmtListNode * myStmtList;
};

class IfElseStmtNode : public StmtNode {
public:
	IfElseStmtNode(ExpNode * exp, VarDeclListNode * varList, StmtListNode * stmtList, VarDeclListNode * elseVarList, StmtListNode * elseStmtList) : StmtNode() {
		myExp = exp;
		myVarList = varList;
		myStmtList = stmtList;
		myElseVarList = elseVarList;
		myElseStmtList = elseStmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myExp;
	VarDeclListNode * myVarList;
	StmtListNode * myStmtList;
	VarDeclListNode * myElseVarList;
	StmtListNode * myElseStmtList;
};

class WhileStmtNode : public StmtNode {
public:
	WhileStmtNode(ExpNode * exp, VarDeclListNode * varList, StmtListNode * stmtList) : StmtNode() {
		myExp = exp;
		myVarList = varList;
		myStmtList = stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myExp;
	VarDeclListNode * myVarList;
	StmtListNode * myStmtList;
};

// End Stmt Nodes

// Exp Nodes

class AssignNode : public ExpNode {
public:
	AssignNode(ExpNode * left, ExpNode * right) : ExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class DotAccessNode : public ExpNode {
public:
	DotAccessNode(ExpNode * left, IdNode * right) : ExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	IdNode * myRight;
};

class CallExpNode : public ExpNode {
public:
	CallExpNode(IdNode * loc, ExpListNode * list) : ExpNode() {
		myLoc = loc;
		myList = list;
	}
	void unparse(std::ostream& out, int indent);
private:
	IdNode * myLoc;
	ExpListNode * myList;
};

class ExpListNode : public ExpNode {
public:
	ExpListNode(std::list<ExpNode *> * list) : ExpNode() {
		myList = *list;
	}
	void unparse(std::ostream& out, int indent);
	void add(ExpNode * exp) {
		myList.push_back(exp);
	}
private:
	std::list<ExpNode *> myList;
};

class PlusNode : public BinaryExpNode {
public:
	PlusNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class MinusNode : public BinaryExpNode {
public:
	MinusNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class TimesNode : public BinaryExpNode {
public:
	TimesNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class DivideNode : public BinaryExpNode {
public:
	DivideNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class UnaryMinusNode : public UnaryExpNode {
public:
	UnaryMinusNode(ExpNode * node) : UnaryExpNode() {
		myNode = node;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myNode;
};

class NotNode : public UnaryExpNode {
public:
	NotNode(ExpNode * node) : UnaryExpNode() {
		myNode = node;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myNode;
};

class AndNode : public BinaryExpNode {
public:
	AndNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class OrNode : public BinaryExpNode {
public:
	OrNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class EqualsNode : public BinaryExpNode {
public:
	EqualsNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class NotEqualsNode : public BinaryExpNode {
public:
	NotEqualsNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class LessNode : public BinaryExpNode {
public:
	LessNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class GreaterNode : public BinaryExpNode {
public:
	GreaterNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class LessEqNode : public BinaryExpNode {
public:
	LessEqNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class GreaterEqNode : public BinaryExpNode {
public:
	GreaterEqNode(ExpNode * left, ExpNode * right) : BinaryExpNode() {
		myLeft = left;
		myRight = right;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myLeft;
	ExpNode * myRight;
};

class TrueNode : public ExpNode {
public:
	TrueNode() : ExpNode() {}
	void unparse(std::ostream& out, int indent);
};

class FalseNode : public ExpNode {
public:
	FalseNode() : ExpNode() {}
	void unparse(std::ostream& out, int indent);
};



// LANE! Please finish these. I don't know how the memory works for getting the values in lilc.yy

class IntLitNode : public ExpNode {
public:
	IntLitNode(int val) : ExpNode() {
		myVal = val;
	}
	void unparse(std::ostream& out, int indent);
private:
	int myVal;
};

class StringLitNode : public ExpNode {
public:
	StringLitNode(std::string * val) : ExpNode() {
		myVal = val;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::string * myVal;
};

//       IntLitNode          -- none --
//       StrLitNode          -- none --
//       TrueNode            -- none --
//       FalseNode           -- none --
//       IdNode              -- none --
// End Exp Nodes
//       BinaryExpNode       ExpNode ExpNode
//         PlusNode
//         MinusNode
//         TimesNode
//         DivideNode
//         AndNode
//         OrNode
//         EqualsNode
//         NotEqualsNode
//         LessNode
//         GreaterNode
//         LessEqNode
//         GreaterEqNode


class TypeNode : public ASTNode{
public:
	TypeNode() : ASTNode(){
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IdNode : public ExpNode{
public:
	IdNode(IDToken * token) : ExpNode(){
		myStrVal = token->value();
	}
	void unparse(std::ostream& out, int indent);
private:
	std::string myStrVal;
};

class IntNode : public TypeNode{
public:
	IntNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

class BoolNode : public TypeNode{
public:
	BoolNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

class VoidNode : public TypeNode{
public:
	VoidNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

} //End namespace LIL' C

#endif
