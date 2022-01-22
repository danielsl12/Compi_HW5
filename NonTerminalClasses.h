#ifndef HW3_NONTERMINALCLASSES_H
#define HW3_NONTERMINALCLASSES_H

#include <string>
#include <vector>
#include <utility>
#include "bp.hpp"

class Object {
public:
    virtual bool isTypeAnnotation() const { return false; }
    virtual bool isFunc() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isByte() const { return false; }
    virtual bool isBool() const { return false; }
    virtual bool isString() const { return false; }
    virtual bool isVoid() const { return false; }
    virtual bool isExpList() const { return false; }
    virtual bool isFormalDecl() const { return false; }
    virtual bool isFormalsList() const { return false; }
    virtual bool isFormals() const{ return false; }
    virtual bool isCall() const { return false; }
    virtual bool isId() const { return false; }
    virtual bool isNum() const { return false; }
    virtual bool isStringVal() const { return false; }
};

class TypeAnnotation : public Object {
private:
    bool isConst;
public:
//    TypeAnnotation() : isConst(false) {}
    explicit TypeAnnotation(bool isConst=false) : isConst(isConst) {}

    bool getIsConst() const {return this->isConst;}
    virtual bool isTypeAnnotation() const override { return true; }
};

class ProtoType : public Object {
protected:
    bool isConst;
//    bool isFunc, isInt, isByte, isBool;

    std::string value;

public:
//    enum Type { INT, BYTE, BOOL };
    ProtoType() : isConst(false), value("") {}
    explicit ProtoType(TypeAnnotation typeAnnotation);
//    ProtoType(Type type);
//    ProtoType(const TypeAnnotation& typeAnnotation, bool isFunc, bool,);
    ProtoType(const ProtoType& type);
    ProtoType& operator=(const ProtoType& type);

    virtual ~ProtoType() = default;
    virtual ProtoType* clone() const = 0;

    virtual std::string typeToString() const = 0;
    virtual bool compareArgs() const;
//    virtual bool isFunc() const;
//    virtual bool isInt() const;
//    virtual bool isByte() const;
//    virtual bool isBool() const;
//    virtual bool isString() const;
//    virtual bool isVoid() const;
    void setConst(bool c);
    bool getIsConst() { return this->isConst; }
    bool operator==(const ProtoType& other) const;

    //Only Func should implement, the others have empty implementaion
    virtual ProtoType& getReturnType() const;
    virtual bool compareArgs(const std::vector<ProtoType*>& args);
    std::string getValue() { return this->value; }
    void setValue(std::string newValue) { this->value = newValue; }
};

class Void : public ProtoType {
public:
    Void();
    Void(const Void& other);
    Void& operator=(const Void& other);
    ~Void() override = default;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isVoid() const override;
};

class Int : public ProtoType {
public:
    Int() : ProtoType() {}
    explicit Int(TypeAnnotation typeAnnotation);
    Int(const Int& other);
    Int& operator=(const Int& other);
    ~Int() override = default;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isInt() const override;
};

class Byte : public ProtoType {
public:
    Byte() : ProtoType() {}
    explicit Byte(TypeAnnotation typeAnnotation);
    Byte(const Byte& other);
    Byte& operator=(const Byte& other);
    ~Byte() override = default;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isByte() const override;
};

class Func : public ProtoType {
private:
    ProtoType* retType;
    std::vector<std::pair<std::string, ProtoType*> > args;
public:
    Func(const ProtoType& retType, const std::string& name, const ProtoType& type);
    Func(const ProtoType& retType, const std::vector<std::string>& names, const std::vector<ProtoType*>& types);
    Func(const Func& other);
    Func& operator=(const Func& other);
    ~Func() override;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isFunc() const override;

    ProtoType& getReturnType() const override;
    bool compareArgs(const std::vector<ProtoType*>& list) override;
    bool compareArgs() const override;
    std::vector<std::string> getStrList() const;
};

class Bool : public ProtoType {
private:
    std::vector<std::pair<int,BranchLabelIndex>> trueList;
    std::vector<std::pair<int,BranchLabelIndex>> falseList;
public:
    Bool() : ProtoType() {}
    explicit Bool(const TypeAnnotation& typeAnnotation);
    Bool(const Bool& other);
    Bool& operator=(const Bool& other);
    ~Bool() override = default;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isBool() const override;

    std::vector<std::pair<int,BranchLabelIndex>> getTrueList() const;
    std::vector<std::pair<int,BranchLabelIndex>> getFalseList() const;
    void setTrueList(std::vector<std::pair<int,BranchLabelIndex>>& trueList);
    void setFalseList(std::vector<std::pair<int, BranchLabelIndex>>& falseList);
};

class String : public ProtoType {
public:
    String() : ProtoType() {}
    explicit String(const TypeAnnotation& typeAnnotation);
    String(const String& other);
    String& operator=(const String& other);
    ~String() override = default;
    ProtoType* clone() const override;
    std::string typeToString() const override;
    bool isString() const override;
};

class Program {

};

/*
class Exp {
private:
    const ProtoType* type;
public:
    explicit Exp(const ProtoType& type) : type(type.clone()) {}
    explicit Exp(const Exp& other);
    virtual Exp& operator=(Exp& other);
    ~Exp();

    const ProtoType& getType() const;
};
*/

class ExpList : public Object {
private:
    std::vector<ProtoType*> expList;
public:
    ExpList() : expList(std::vector<ProtoType*>()) {}
    ExpList(const ProtoType& firstExp, const std::vector<ProtoType*>& list);
    explicit ExpList(const ProtoType& exp);
    ~ExpList();

    std::vector<ProtoType*>& getExpList(); //TODO: maybe no need for reference
    std::vector<std::string> getStrList();
    bool isExpList() const override { return true; }
};

class Call : public Object {
private:
    ProtoType* funcRetType;
public:
    Call() : funcRetType(nullptr) {}
    explicit Call(const ProtoType& retType) : funcRetType(retType.clone()) {}
    Call(const Call& other);
    Call& operator=(const Call& other);
    ~Call();

    ProtoType* getReturnType() const;
    bool isCall() const override { return true; }
};

class FormalDecl : public Object {
private:
    ProtoType* type;
    std::string id;
public:
    FormalDecl() : type(nullptr), id("") {}
    FormalDecl(const ProtoType& type, std::string name);
    FormalDecl(const FormalDecl& other);
    FormalDecl& operator=(const FormalDecl& other);
    ~FormalDecl();

    //const bool& isConstAnn() const;
    const ProtoType& getType() const;
    std::string getId() const;
    bool isFormalDecl() const override { return true; }
};

class FormalsList : public Object{
private:
    std::vector<FormalDecl> formals;
public:
    FormalsList() : formals(std::vector<FormalDecl>()) {}
    FormalsList(FormalDecl& decl, std::vector<FormalDecl>& vec);
    explicit FormalsList(FormalDecl& decl);
    FormalsList(const FormalsList& other);
    FormalsList& operator=(const FormalsList& other);
    ~FormalsList() = default;
    std::vector<FormalDecl>& getList();
    bool isFormalsList() const override {return true;}
};

class Formals : public Object {
private:
    std::vector<FormalDecl> formalsList;
public:
    Formals();
    Formals(FormalsList& list);
    Formals(const Formals& other);
    Formals& operator=(const Formals& other);
    ~Formals() = default;

    std::vector<std::string> getNames() const;
    std::vector<ProtoType*> getTypes() const;
    bool isFormals() const override {return true;}
};

class Id : public Object {
public:
    std::string id;
    explicit Id(const std::string& id) : id(id) {}

    bool isId() const override { return true; }
};

class Num : public Object {
public:
    int num;
    explicit Num(int num) : num(num) {}

    bool isNum() const override { return true; }
};

class StringVal : public Object {
public:
    std::string str;
    explicit StringVal(std::string str) : str(str) {}

    bool isStringVal() const override { return true; }
};

/*
class FuncDecl {
private:
    ProtoType* retType;
    std::string funcName;
    std::vector<FormalDecl> args;
public:
    FuncDecl(const ProtoType& retType, const std::string& name, std::vector<FormalDecl>& args);
    FuncDecl(const FuncDecl& other);
    FuncDecl& operator=(FormalDecl& other);
    ~FuncDecl();

    const ProtoType& getRetType() const;
    const std::string getFuncName() const;
    const std::vector<FormalDecl> getArgs() const;

};
 */

//class Statement {
//private:
//    ProtoType* type;
//public:
//    Statement(ProtoType& type);
//    Statement(const Statement& other);
//    Statement& operator=(const Statement& other);
//    ~Statement();
//    ProtoType& getType() const;
//};

#endif //HW3_NONTERMINALCLASSES_H
