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
	for (std::list<VarDeclNode *>::iterator it=myDecls.begin();
		it != myDecls.end(); ++it){
	    DeclNode * elt = *it;
	    elt->unparse(out, indent + 1);
	}

	//myStmts->unparse(out, indent + 1);
	for (std::list<StmtNode *>::iterator it=myStmts.begin();
		it != myStmts.end(); ++it){
	    StmtNode * elt = *it;
	    elt->unparse(out, indent + 1);
	}
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
	out << "AHHHH";
}

void StructDeclNode::unparse(std::ostream& out, int indent) {
	doIndent(out, indent);
	out << "struct ";
	myId->unparse(out, 0);
	out << " {\n";
	for (std::list<VarDeclNode *>::iterator it=myDecls.begin();
		it != myDecls.end(); ++it){
	    DeclNode * elt = *it;
	    elt->unparse(out, indent + 1);
	}
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

// End Exp Nodes

} // End namespace LIL' C
