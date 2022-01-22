#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H

#include <deque>
#include <vector>
#include <string>
#include "hw3_output.hpp"
#include "NonTerminalClasses.h"

class Symbol {
private:
    const std::string name;
    ProtoType* type;
    int offset;
public:
    Symbol(const std::string& name, const ProtoType& type, int offset);
    Symbol(const Symbol& sym);
    ~Symbol();

    const std::string& getName() const;
    ProtoType& getType() const ;
    int getOffset() const;
};

std::ostream& operator<< (std::ostream &os, const Symbol &symbol);

class SymbolTableEntry : public std::vector<Symbol> {
private:
    int lastParameter, lastOffset;
public:
    SymbolTableEntry() : std::vector<Symbol>(), lastParameter(0), lastOffset(-1) {}
    explicit SymbolTableEntry(int startOffset) : std::vector<Symbol>(), lastParameter(0), lastOffset(startOffset) {}
    SymbolTableEntry(const SymbolTableEntry& other);

    int getLastOffset() const;
    int getLastParameter() const;
    void lastParameterInc();
    void lastOffsetInc();
};

std::ostream& operator<< (std::ostream &os, const SymbolTableEntry &symbolTableEntry);

class SymbolTable : public std::deque<SymbolTableEntry> {
public:
    explicit SymbolTable();

    void pop();
    void push(const SymbolTableEntry& entry);
    void push(SymbolTableEntry& entry);
    void pushNewEntry();
    SymbolTableEntry& top();

    void insertAtTop(const std::string& name, const ProtoType& type, bool isParameter=false);
    bool isMainExists() const;
    bool findId(const std::string& id) const;
    ProtoType& getIdType(const std::string& id) const;
    bool addParameters(const Formals& parameters, int lineno);
    int getAbsoluteOffset(const std::string& id);
    void setValueById(const std::string& id);
    string getValueById(const std::string& id);

};

#endif //HW3_SYMBOLTABLE_H

#pragma clang diagnostic pop