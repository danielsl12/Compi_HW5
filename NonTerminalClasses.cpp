#include "NonTerminalClasses.h"
#include "hw3_output.hpp"
#include <iostream>

ProtoType::ProtoType(TypeAnnotation typeAnnotation) : isConst(typeAnnotation.getIsConst()) {}

ProtoType::ProtoType(const ProtoType& type) : isConst(type.isConst) {}

ProtoType& ProtoType::operator=(const ProtoType& type) {
    if (this == &type) {
        return *this;
    }

    this->isConst = type.isConst;
    return *this;
}

//bool ProtoType::isFunc() const {return false;}
//bool ProtoType::isInt() const {return false;}
//bool ProtoType::isByte() const {return false;}
//bool ProtoType::isBool() const {return false;}
//bool ProtoType::isString() const {return false;}
//bool ProtoType::isVoid() const {return false;}

void ProtoType::setConst(bool c) {this->isConst = c;}

ProtoType& ProtoType::getReturnType() const {}
bool ProtoType::compareArgs(const std::vector<ProtoType*>& args) { return false;}

bool ProtoType::operator==(const ProtoType& other) const {
    if(this->isConst != other.isConst)
        return false;

    if(this->isBool() && other.isBool())
        return true;

    if(this->isByte() && other.isByte())
        return true;

    if(this->isInt() && other.isInt())
        return true;

    //TODO: maybe need to add more types to check???

    return false;
}

bool ProtoType::compareArgs() const { return false;}

/* ******************** */

Void::Void() : ProtoType(TypeAnnotation()) {}

Void::Void(const Void& other) : ProtoType(TypeAnnotation(other.isConst)) {}

Void& Void::operator=(const Void& other) {
    if (this == &other) {
        return *this;
    }

    this->isConst = other.isConst;
    return *this;
}

ProtoType* Void::clone() const {
    return new Void();
}

std::string Void::typeToString() const {
    return "VOID";
}

bool Void::isVoid() const {
    return true;
}

/* ******************** */

Int::Int(TypeAnnotation typeAnnotation) : ProtoType(typeAnnotation) {}

Int::Int(const Int& other) : ProtoType(TypeAnnotation(other.isConst)) {}

Int& Int::operator=(const Int& other) {
    if (this == &other) {
        return *this;
    }

    this->isConst = other.isConst;
    return *this;
}

ProtoType* Int::clone() const {
    return new Int(TypeAnnotation(this->isConst));
}

std::string Int::typeToString() const {
    return "INT";
}

bool Int::isInt() const {
    return true;
}

/* ******************** */

Byte::Byte(TypeAnnotation typeAnnotation) : ProtoType(typeAnnotation) {}

Byte::Byte(const Byte& other) : ProtoType(TypeAnnotation(other.isConst)) {}

Byte& Byte::operator=(const Byte& other) {
    if (this == &other) {
        return *this;
    }

    this->isConst = other.isConst;
    return *this;
}

ProtoType* Byte::clone() const {
    return new Byte(TypeAnnotation(this->isConst));
}

std::string Byte::typeToString() const {
    return "BYTE";
}

bool Byte::isByte() const {
    return true;
}

/* ******************** */

Func::Func(const ProtoType& retType, const std::string& name, const ProtoType& type) : ProtoType(TypeAnnotation()) {
    this->retType = retType.clone();
    std::pair<std::string, ProtoType*> pair = std::make_pair(name, type.clone());
    this->args.push_back(pair);
}

Func::Func(const ProtoType &retType, const std::vector<std::string> &names, const std::vector<ProtoType*> &types) : ProtoType(TypeAnnotation()){
    this->retType = retType.clone();

    //Assume that names and types are of same length
    for(int i = 0; i < names.size(); i++) {
        std::pair<std::string, ProtoType*> pair = std::make_pair(names[i], types[i]->clone()); // maybe clone types[i]
        this->args.push_back(pair);
    }
}

Func::Func(const Func& other) : ProtoType(TypeAnnotation(other.isConst)) {
    this->retType = other.retType->clone();

    for(int i = 0; i < other.args.size(); i++) {
        std::pair<std::string, ProtoType*> pair = std::make_pair(other.args[i].first, other.args[i].second->clone());
        this->args.push_back(pair);
    }
}

Func& Func::operator=(const Func& other) {
    if(this == &other) {
        return *this;
    }

    //deleting prev data
    delete this->retType;
    for (int i = 0; i < this->args.size(); i++)
        delete (this->args[i].second);

    this->args.clear();

    //inserting new data
    this->retType = other.retType->clone();
    for(int i = 0; i < other.args.size(); i++) {
        std::pair<std::string, ProtoType*> pair = std::make_pair(other.args[i].first, other.args[i].second->clone());
        this->args.push_back(pair);
    }

    return *this;
}

Func::~Func() {
    delete this->retType;
    for (int i = 0; i < this->args.size(); i++)
        delete (this->args[i].second);

    this->args.clear();
}

ProtoType* Func::clone() const {
    //return new Func(this->retType, this-)

    return new Func(*this); //TODO: check if this is right
}

std::vector<std::string> Func::getStrList() const {
    std::vector<std::string> argsList;
    for(const auto & arg : this->args)
        argsList.push_back(arg.second->typeToString());
    return argsList;
}

std::string Func::typeToString() const {
    std::vector<std::string> argsList = this->getStrList();
//    for(int i = 0; i < this->args.size(); i++)
//        argsList.push_back(this->args[i].second->typeToString());
    return output::makeFunctionType(this->retType->typeToString(), argsList);
}

bool Func::isFunc() const {return true;}

ProtoType& Func::getReturnType() const {
    return *(this->retType);
}

bool Func::compareArgs(const std::vector<ProtoType*>& list) {
//    cout << list.size() << endl;
//    cout << this->args.size() << endl;
//    cout << this->args[0].second->typeToString() << endl;
//    cout << list[0]->typeToString() << endl;
//    cout << "not this" << endl;
    if(this->args.size() != list.size())
        return false;
    //cout << "check1 " << (i < len) << endl, cout << "check2 " << endl,
    unsigned long len = this->args.size();
//    int counter = 0;
    for(unsigned long i = 0;  i < len; ++i) {
//        cout << i << endl;
//        bool b1 = !(this->args[i].second->isInt() && list[i]->isByte());
//        bool b2 = !(this->args[i].second->typeToString() != list[i]->typeToString());
//        cout << !(this->args[i].second->isInt() && list[i]->isByte()) << endl;
//        cout << "IDK1" << endl;
//        cout << (this->args[i].second->typeToString() != list[i]->typeToString()) << endl;
//        cout << "IDK2" << endl;
//        cout << (!(this->args[i].second->isInt() && list[i]->isByte()) && this->args[i].second->typeToString() != list[i]->typeToString()) << endl;
//        cout << b1 && b2;
//        cout << "IDK";
        if(!(this->args[i].second->isInt() && list[i]->isByte()) && this->args[i].second->typeToString() != list[i]->typeToString()) {
//            cout << "return false" << endl;
            return false;
        }
//        bool b1 = !(i->second->isInt() && list[counter]->isByte());
//        bool b2 = !(i->second->typeToString() != list[counter]->typeToString());
//        cout << "here" << endl;
//        cout << b1 << endl;
//        cout << b2 << endl;
//        if (b1) {
//            cout << "why" << endl;
//            if(!b2) {
//                cout << "not" << endl;
//                return false;
//            }
//        }
//        cout << "check" << endl;
    }
//    cout << "return true";
    return true;
}

bool Func::compareArgs() const {
    return (this->args.empty());
}

/* ******************** */

Bool::Bool(const TypeAnnotation& typeAnnotation) : ProtoType(typeAnnotation) {}

Bool::Bool(const Bool& other) : ProtoType(TypeAnnotation(other.isConst)) {}

Bool& Bool::operator=(const Bool& other) {
    if (this == &other) {
        return *this;
    }

    this->isConst = other.isConst;
    return *this;
}

ProtoType* Bool::clone() const {
    return new Bool(TypeAnnotation(this->isConst));
}

std::string Bool::typeToString() const {
    return "BOOL";
}

bool Bool::isBool() const {return true;}

/* ******************** */

String::String(const TypeAnnotation& typeAnnotation) : ProtoType(typeAnnotation) {}

String::String(const String& other) : ProtoType(TypeAnnotation(other.isConst)) {}

String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }

    this->isConst = other.isConst;
    return *this;
}

ProtoType* String::clone() const {
    return new String(TypeAnnotation(this->isConst));
}

std::string String::typeToString() const {
    return "STRING";
}

bool String::isString() const {
    return true;
}

/* ******************** */

ExpList::ExpList(const ProtoType& firstExp, const std::vector<ProtoType*>& list) {
    this->expList.push_back(firstExp.clone());
    for(int i = 0; i < list.size(); i++)
        this->expList.push_back(list[i]->clone());
}

ExpList::ExpList(const ProtoType& exp) {
    this->expList.push_back(exp.clone());
}

ExpList::~ExpList() {
    for(int i = 0; i < this->expList.size(); i++)
        delete this->expList[i];
}

std::vector<ProtoType*>& ExpList::getExpList() {
    return this->expList;
}

std::vector<std::string> ExpList::getStrList() {
    std::vector<std::string> list;
    for(auto & i : this->expList) {
        list.push_back(i->typeToString());
    }

    return list;
}

/* ******************** */

Call::Call(const Call& other) : funcRetType(other.funcRetType->clone()) {}

Call& Call::operator=(const Call& other) {
    if(this == &other) {
        return *this;
    }

    delete this->funcRetType;
    this->funcRetType = other.funcRetType->clone();

    return *this;
}

Call::~Call() {
    delete this->funcRetType;
}

ProtoType* Call::getReturnType() const {
    return this->funcRetType;
}

/* ******************** */

FormalDecl::FormalDecl(const ProtoType& type, std::string name) : type(type.clone()), id(name) {}

FormalDecl::FormalDecl(const FormalDecl& other) : type(other.type->clone()), id(other.id) {}

FormalDecl& FormalDecl::operator=(const FormalDecl& other) {
    if(this == &other) {
        return *this;
    }

    delete this->type;
    this->type = other.type->clone();
    this->id = other.id;

    return *this;
}

FormalDecl::~FormalDecl() {
    delete this->type;
}

const ProtoType& FormalDecl::getType() const {
    return *(this->type);
}

std::string FormalDecl::getId() const {
    return this->id;
}

/* ******************** */

FormalsList::FormalsList(FormalDecl& decl, std::vector<FormalDecl>& vec) {
    this->formals.push_back(decl);
    this->formals.insert(this->formals.end(), vec.begin(), vec.end());
}

FormalsList::FormalsList(FormalDecl& decl) {
    this->formals.push_back(decl);
}

FormalsList::FormalsList(const FormalsList& other) : formals(other.formals) {}

FormalsList& FormalsList::operator=(const FormalsList& other) {
    if (this == &other) {
        return *this;
    }

    this->formals = other.formals;
    return *this;
}

std::vector<FormalDecl>& FormalsList::getList() {
    return this->formals;
}

/* ******************** */

Formals::Formals() : formalsList() {}

Formals::Formals(FormalsList& list) : formalsList(list.getList()) {}

Formals::Formals(const Formals& other) : formalsList(other.formalsList) {}

Formals& Formals::operator=(const Formals& other) {
    if(this == &other) {
        return *this;
    }

    this->formalsList = other.formalsList;
    return *this;
}

std::vector<std::string> Formals::getNames() const {
    std::vector<std::string> names;

    for(const auto & i : this->formalsList) {
        names.push_back(i.getId());
    }
    return names;
}

std::vector<ProtoType*> Formals::getTypes() const {
    std::vector<ProtoType*> types;

    for(const auto & i: this->formalsList) {
        types.push_back(i.getType().clone()); //Daniel, where are we deleting this memory?
    }

    return types;
}

/* ******************** */

/*
FuncDecl::FuncDecl(const ProtoType& retType, const std::string& name, std::vector<FormalDecl>& args)
: retType(retType.clone()), funcName(name), args(args) {}
*/
