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
	out << "\n";
}

void FnDeclNode::unparse(std::ostream& out, int indent) {

}

void FormalsListNode::unparse(std::ostream& out, int indent) {

}

void FormalDeclNode::unparse(std::ostream& out, int indent) {
	
}

void StmtListNode::unparse(std::ostream& out, int indent) {
	
}

void FnBodyNode::unparse(std::ostream& out, int indent) {
	
}

void StructDeclNode::unparse(std::ostream& out, int indent) {
	
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

} // End namespace LIL' C
