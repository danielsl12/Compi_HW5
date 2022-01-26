#include "SymbolTable.h"
#include "hw3_output.hpp"
#include <iostream>

#define MAIN "main"

using namespace std;


Symbol::Symbol(const std::string& name, const ProtoType& type, int offset) : name(name), type(type.clone()), offset(offset) {} //check that string and type dont die after scope ends

Symbol::~Symbol() {
    delete this->type;
}

const std::string& Symbol::getName() const {
    return this->name;
}

ProtoType& Symbol::getType() const {
    return *(this->type);
}

int Symbol::getOffset() const {
    return this->offset;
}

Symbol::Symbol(const Symbol &sym) : name(sym.getName()), type(sym.type->clone()), offset(sym.offset) {}

void Symbol::setValue(std::string value) {
    this->value = value;
}

std::string Symbol::getValue() {
    return this->value;
}

std::ostream& operator<< (std::ostream &os, const Symbol &symbol) {
    std::string space(" ");
    std::string type = symbol.getType().typeToString();
    return os << symbol.getName() << space << type << space << std::to_string(symbol.getOffset());
}

std::ostream& operator<< (std::ostream &os, const SymbolTableEntry &symbolTableEntry) {
    std::string carriage_return("\n");
    for(const Symbol& entry : symbolTableEntry) {
        os << entry << carriage_return;
    }
    return os;
}

SymbolTableEntry::SymbolTableEntry(const SymbolTableEntry& other) : std::vector<Symbol>(), lastParameter(other.lastParameter), lastOffset(other.lastOffset) {
    for(unsigned long i = 0; i < other.size(); i++) {
        this->push_back(other[i]);
    }
}

void SymbolTable::pop() {
    this->pop_front();
}

void SymbolTable::push(const SymbolTableEntry &entry) {
    this->push_front(entry);
}

void SymbolTable::push(SymbolTableEntry &entry) {
    this->push_front(entry);
}

SymbolTableEntry &SymbolTable::top() {
    return this->front();
}

void SymbolTable::pushNewEntry() {
//    cout << "pushed new Entry" << endl;
    if(!this->empty()) {
//        cout << this->top().getLastOffset() << endl;
        this->push(SymbolTableEntry(this->top().getLastOffset()));  //check if new is needed
    } else {
        this->push(SymbolTableEntry());  //check if new is needed
    }

}

void SymbolTable::insertAtTop(const std::string &name, const ProtoType &type, bool isParameter) {
    int offset = 0;
//    cout << name << endl;
    if (!type.isFunc()) {  //check what happens when entry is empty
        SymbolTableEntry& entry = this->top();
        if (isParameter) {
            offset = entry.getLastParameter() - 1;
            entry.lastParameterInc();
        } else {
            offset = this->top().getLastOffset() + 1;
            entry.lastOffsetInc();
//            cout << this->top();
        }
    }
    this->top().push_back(Symbol(name, type, offset));  //check if new is needed
}

SymbolTable::SymbolTable() : std::deque<SymbolTableEntry>() {
    this->pushNewEntry();
}


bool SymbolTable::findId(const string &id) const {
//    cout << id << endl;
    for (const SymbolTableEntry& entry : *this) {
//        cout << entry;
        for (const Symbol& sym : entry) {
//            cout << sym << endl;
//            cout << sym.getName() << endl;
            if (sym.getName() == id) {
                return true;
            }
        }
    }
    return false;
}

bool SymbolTable::isMainExists() const {
    if(this->findId(MAIN)) {
        ProtoType& mainType = this->getIdType(MAIN);
        if (mainType.isFunc()) {
            Func& main = dynamic_cast<Func&>(mainType);
            if (main.getReturnType().isVoid() && main.getStrList().empty()) {
                return true;
            }
        }
    }
    return false;
}

ProtoType &SymbolTable::getIdType(const string &id) const {
    for (const SymbolTableEntry& entry : *this) {
        for (const Symbol& sym : entry) {
            if (sym.getName() == id) {
                return sym.getType();
            }
        }
    }
    throw Void(); // very very stupid.
}

bool SymbolTable::addParameters(const Formals &parameters, int lineno) {
    std::vector<std::string> names = parameters.getNames();
    std::vector<ProtoType*> types = parameters.getTypes();
    unsigned long len = names.size();
    int count = 0;
    for (unsigned long i = 0; i < len; ++i) {
        if(this->findId(names[i])) {
            output::errorDef(lineno, names[i]);
            return true;
        }
        
        types[i]->setConst(true);
        types[i]->setIsLiteral(true);
        this->insertAtTop(names[i], *(types[i]), true);
        std::string val = "%";
        val += std::to_string(count++);
        this->setValueById(names[i], val);
    }
    return false;
}

int SymbolTable::getAbsoluteOffset(const string &id) {
    int offset = 0;
    bool isFound = false;
    for (const SymbolTableEntry& entry : *this) {
        if(!isFound) {
            for (const Symbol &sym : entry) {
                if (sym.getName() == id) {
                    isFound = true;
                    offset += sym.getOffset();
                }
            }
        } else {
            offset += entry.getLastOffset() == -1 ? 0 : entry.getLastOffset();
        }
    }
    return offset;
}

void SymbolTable::setValueById(const string &id, std::string& value) {
    for (SymbolTableEntry& entry: *this) {
        for (Symbol& sym: entry) {
            if(sym.getName() == id) {
                cout << "found id " << id << " and gave it value " << value << endl;
                sym.setValue(value);
                cout << "new value of sym: " << sym.getValue() << endl;
                return;
            }
        }
    }
}

string SymbolTable::getValueById(const string &id) {
    for (SymbolTableEntry& entry: *this) {
        for (Symbol& sym: entry) {
            if(sym.getName() == id) {
                return sym.getValue();
            }
        }
    }
    return ""; //
}


int SymbolTableEntry::getLastOffset() const {
    return this->lastOffset;
}

int SymbolTableEntry::getLastParameter() const {
    return this->lastParameter;
}

void SymbolTableEntry::lastParameterInc() {
    this->lastParameter--;
}

void SymbolTableEntry::lastOffsetInc() {
    this->lastOffset++;
}
