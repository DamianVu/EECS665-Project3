#include "ast.hpp"

namespace LILC{

void ProgramNode::unparse(std::ostream& out, int indent){
	myDeclList->unparse(out, indent);
}

void DeclListNode::unparse(std::ostream& out, int indent){
	for (std::list<DeclNode *>::iterator it=myDecls.begin();
		it != myDecls.end(); ++it){
	    DeclNode * elt = *it;
	    elt->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myType->unparse(out, 0);
	out << " ";
	myId->unparse(out, 0);
	out << ";\n";
}

void VarDeclListNode::unparse(std::ostream& out, int indent) {
	for (std::list<VarDeclNode *>::iterator it=myVarDecls.begin();
		it != myVarDecls.end(); ++it){
	    VarDeclNode * elt = *it;
	    elt->unparse(out, indent + 1);
	}
}

void FnDeclNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	myType->unparse(out, 0);
	out << " ";
	myId->unparse(out, 0);
	out << "(";
	if (myFormals != nullptr) {
		myFormals->unparse(out, 0);
	}
	out << ") {\n";
	myBody->unparse(out, indent);
	doIndent(out, indent);
	out << "}\n";
}

void FnBodyNode::unparse(std::ostream& out, int indent) {
	myDecls->unparse(out, indent + 1);

	myStmts->unparse(out, indent + 1);
}

void FormalsListNode::unparse(std::ostream& out, int indent) {
	for (std::list<FormalDeclNode *>::iterator it=myFormals.begin();
		it != myFormals.end(); ++it){
	    FormalDeclNode * elt = *it;
	    elt->unparse(out, 0);
	    if (elt != myFormals.back()) {
	    	out << ", ";
	    }
	}
}

void FormalDeclNode::unparse(std::ostream& out, int indent) {
	myType->unparse(out, 0);
	out << " ";
	myId->unparse(out, 0);
}

void StmtListNode::unparse(std::ostream& out, int indent) {
	for (std::list<StmtNode *>::iterator it=myList.begin();
		it != myList.end(); ++it){
	    StmtNode * elt = *it;
	    elt->unparse(out, indent + 1);
	}
}

void StructDeclNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "struct ";
	myId->unparse(out, 0);
	out << " {\n";
	myDecls->unparse(out, indent + 1);
	out << "};\n";
}



void IdNode::unparse(std::ostream& out, int indent){
	out << myStrVal;
}

void IntNode::unparse(std::ostream& out, int indent){
	out << "int";
}

void BoolNode::unparse(std::ostream& out, int indent){
	out << "bool";
}

void VoidNode::unparse(std::ostream& out, int indent){
	out << "void";
}

// Begin Statement Nodes

void AssignStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	myAssign->unparse(out, 0);
}

void PostIncStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	myLoc->unparse(out, 0);
	out << "++;\n";
}

void PostDecStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	myLoc->unparse(out, 0);
	out << "--;\n";
}

void ReadStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "cin >> ";
	myLoc->unparse(out, 0);
	out << ";\n";
}

void WriteStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "cout << ";
	myLoc->unparse(out, 0);
	out << ";\n";
}

void ReturnStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "return";
	if (myLoc != nullptr) {
		out << " ";
		myLoc->unparse(out, 0);
	}
	out << ";\n";
}

void CallStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	myCall->unparse(out, 0);
	out << ";\n";
}

void IfStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "if (";
	myExp->unparse(out, 0);
	out << ") {\n";
	myVarList->unparse(out, indent+1);
	myStmtList->unparse(out, indent+1);
	doIndent(out, indent);
	out << "}\n";
}

void IfElseStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "if (";
	myExp->unparse(out, 0);
	out << ") {\n";
	myVarList->unparse(out, indent+1);
	myStmtList->unparse(out, indent+1);
	doIndent(out, indent);
	out << "} else {\n";
	myElseVarList->unparse(out, indent+1);
	myElseStmtList->unparse(out, indent+1);
	doIndent(out, indent);
	out << "}\n";
}

void WhileStmtNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "while (";
	myExp->unparse(out, 0);
	out << ") {\n";
	myVarList->unparse(out, indent+1);
	myStmtList->unparse(out, indent+1);
	doIndent(out, indent);
	out << "}\n";
}

// End Statement Nodes

// Begin Exp Nodes

void AssignNode::unparse(std::ostream& out, int indent) {
	myLeft->unparse(out, 0);
	out << " = ";
	myRight->unparse(out, 0);
	out << ";\n";
}

void DotAccessNode::unparse(std::ostream& out, int indent) {
	myLeft->unparse(out, 0);
	out << ".";
	myRight->unparse(out, 0);
}

void CallExpNode::unparse(std::ostream& out, int indent) {
	myLoc->unparse(out, 0);
	out << "(";
	if (myList != nullptr)
		myList->unparse(out, 0);
	out << ")";
}

void ExpListNode::unparse(std::ostream& out, int indent) {
	for (std::list<ExpNode *>::iterator it=myList.begin();
		it != myList.end(); ++it){
	    ExpNode * elt = *it;
	    elt->unparse(out, 0);
	    if (elt != myList.back()) {
	    	out << ", ";
	    }
	}
}

void PlusNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " + ";
	myRight->unparse(out, 0);
	out << ")";
}

void MinusNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " - ";
	myRight->unparse(out, 0);
	out << ")";
}

void TimesNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " * ";
	myRight->unparse(out, 0);
	out << ")";
}

void DivideNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " / ";
	myRight->unparse(out, 0);
	out << ")";
}

void UnaryMinusNode::unparse(std::ostream& out, int indent) {
	out << "(-";
	myNode->unparse(out, 0);
	out << ")";
}

void NotNode::unparse(std::ostream& out, int indent) {
	out << "(!";
	myNode->unparse(out, 0);
	out << ")";
}

void AndNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " && ";
	myRight->unparse(out, 0);
	out << ")";
}

void OrNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " || ";
	myRight->unparse(out, 0);
	out << ")";
}

void EqualsNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " == ";
	myRight->unparse(out, 0);
	out << ")";
}

void NotEqualsNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " != ";
	myRight->unparse(out, 0);
	out << ")";
}

void LessNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " < ";
	myRight->unparse(out, 0);
	out << ")";
}

void GreaterNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " > ";
	myRight->unparse(out, 0);
	out << ")";
}

void LessEqNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " <= ";
	myRight->unparse(out, 0);
	out << ")";
}

void GreaterEqNode::unparse(std::ostream& out, int indent) {
	out << "(";
	myLeft->unparse(out, 0);
	out << " >= ";
	myRight->unparse(out, 0);
	out << ")";
}

void TrueNode::unparse(std::ostream& out, int indent) {
	out << "true";
}

void FalseNode::unparse(std::ostream& out, int indent) {
	out << "false";
}

void IntLitNode::unparse(std::ostream& out, int indent) {
	out << myVal;
}

void StringLitNode::unparse(std::ostream& out, int indent) {
	out << myVal;
}


// End Exp Nodes

} // End namespace LIL' C
