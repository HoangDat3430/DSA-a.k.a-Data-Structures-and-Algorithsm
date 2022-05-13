#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

bool checkInstruction(string s){
    regex insert    ("(INSERT)\\s(([a-z]*\\w+\\s[0-9]+)|([a-z]*\\w+))");
    regex assign    ("(ASSIGN)\\s[a-z]+\\w*\\s(\\d+|(\'(\\w|\\s|\\d)*\')|([a-z]+\\w*\\((([a-z]+\\w*|\\d*|(\'(\\w|\\d|\\s)*\')|\\,)+)\\))|[a-z]+\\w*)");
    regex one_word_command   ("(BEGIN|END|PRINT)");
    regex lookup    ("(LOOKUP)\\s([a-z]*\\w+)");
    regex linear    ("(LINEAR)\\s([0-9]+)\\s([0-9]+)");
    regex quadratic ("(QUADRATIC)\\s([0-9]+)\\s([0-9]+\\s([0-9]+))");
    regex Double    ("(DOUBLE)\\s([0-9]+)\\s([0-9]+)");
    regex call      ("(CALL)\\s[a-z]+\\w*\\((([a-z]+\\w*|\\d*|(\'(\\w|\\d|\\s)*\')|\\,)+)\\)");

    if(regex_match(s, insert)) return true;
    else if(regex_match(s, assign)) return true;
    else if(regex_match(s, one_word_command)) return true;
    else if(regex_match(s, lookup)) return true;
    else if(regex_match(s, linear)) return true;
    else if(regex_match(s, quadratic)) return true;
    else if(regex_match(s, Double)) return true;
    else if(regex_match(s, call)) return true;
    else return false;
}

bool isNumber(string str){
    for(char c : str){
        if (isdigit(c) == 0) return false;
    }
    return true;
} 

bool isValid(string s){
    regex val       ("\'(\\w|\\d|\\s)*\'");
    if(regex_match(s, val)) return true;
    else return false;
}

int convertCommand(string s){
    if(s == "LINEAR") return 0;
    else if(s == "QUADRATIC") return 1;
    else if(s == "DOUBLE") return 2;
    else if(s == "INSERT") return 3;
    else if(s == "ASSIGN") return 4;
    else if(s == "BEGIN") return 5;
    else if(s == "END") return 6;
    else if(s == "LOOKUP") return 7;
    else if(s == "PRINT") return 8;
    else if(s == "CALL") return 9;
    else return 10;
}

long long convertValue(int lv, string id){
    string temp = to_string(lv);
    for(unsigned int i = 0; i < id.size(); i++){
        temp += to_string(id[i]-48);
    }
    return stoll(temp);
}

class HashTable{
public:
class Data{
public:
    string identifier;
    int level_of_block;
    string type;
    int num_para;
    string type_para;
    Data(){
        this->identifier = '\0';
        this->level_of_block = -1;
        this->num_para = 0;
        this->type = '\0';
        this->type_para = '\0';
    }
    Data(string ide, int lv){
        this->identifier = ide;
        this->level_of_block = lv;
        this->type = "";
        this->type_para = "";
    }
};
    Data **Table;
    int Size;
    string Pattern = "";
    int counter = 0;
    int total = 0;
    int c1, c2;
    HashTable(){
        Table = new Data * [1000];
        for(int i = 0; i < 1000; i++){
            Table[i] = NULL;
        }
    }

    ~HashTable(){
        for(int i = 0; i < 1000; i++){
            delete Table[i];
        }
    }

    int hash_func(long long val){
        int h1 = val % Size;
        if (Pattern == "linear") return (h1 + c1*counter);
        if (Pattern == "quadratic") return (h1 + c1*counter + c2*counter*counter) % Size;
        if (Pattern == "double"){
            int h2 = 1 + (val % (Size-2));
            return (h1 + c1*counter*h2) % Size;
        }
        return 0;
    }

    int find_func(long long val){
        int hp = hash_func(val);
        while(hp < Size){
            if(Table[hp] && convertValue(Table[hp]->level_of_block, Table[hp]->identifier) == val){
                total += counter;
                counter = 0;
                return hp;
            }
            else{
                counter++;
                hp = hash_func(val);
                while(hp >= Size) hp = hp - Size;
            }
            if(counter > Size){
                counter = 0;
                return -1;
            }
        }
        return -1;
    }

    int checkFunction(string parameters,string typeofPara, int num_para, int lv, int idx){
        string para[100];
        string types[100];
        int counter_para = 0;
        if(parameters == "()") counter_para = 0;
        else{
            counter_para = 1;
            for(unsigned int i = 1; i < parameters.length()-1; i++){
                if(parameters[i] != ','){
                    para[counter_para-1].push_back(parameters[i]);
                }
                else{
                    if(para[counter_para-1].length() != 0) counter_para++;
                    else return 3;
                }
            }
            if(para[counter_para-1].length() == 0) return 3;
        }

        if(typeofPara != ""){
            for(unsigned int i = 0; i < typeofPara.length()/6; i++){
                types[i] = typeofPara.substr(i*6, 6);
            }
        }

        if(counter_para != num_para) return 2;
        else{
            if(counter_para == 0) return 1;
            else{
                for(int i = 0; i <= counter_para-1; i++){
                    string temp = para[i];
                    if(!isNumber(temp) && !isValid(temp)){
                        int find = search(temp, lv);
                        if(find != -1){
                            if(Table[find]->num_para != -1) return 2;
                            if(Table[find]->type == "" && types[i] == "") return 4;
                            if(Table[find]->type == "" || types[i] == ""){
                                if(Table[find]->type == "") Table[find]->type = types[i];
                                else{
                                    types[i] = Table[find]->type;
                                    Table[idx]->type_para += Table[find]->type;
                                }
                            }
                            if(Table[find]->type != types[i]) return 2;
                            find_func(convertValue(Table[find]->level_of_block, temp)); 
                        }
                        else throw Undeclared(temp);
                    }
                    else if(isNumber(temp)){
                        if(types[i] == "string") return 2;
                        if(types[i] == "") Table[idx]->type_para += "number"; 
                    }
                    else if(isValid(temp)){
                        if(types[i] == "number") return 2;
                        if(types[i] == "") Table[idx]->type_para += "string";
                    }
                }
            }
            return 1;
        }
        return 3;
    }

    int insertion(string id, int lv, long long val, string para){
        bool hollow = false;
        for(int i = 0; i < Size; i++){
            if(Table[i] && Table[i]->identifier == id){
                if(Table[i]->level_of_block == lv) return 0;
            }
            if(!Table[i]) hollow = true;
        }
        if(!hollow) return 2;
        Data *newNode = new Data(id, lv);
        int hp = hash_func(val);
        while(hp < Size){
            if(!Table[hp]){
                Table[hp] = newNode;
                if(para == "") Table[hp]->num_para = -1;
                else Table[hp]->num_para = stoi(para);
                return 1;
            }
            if(Table[hp]){
                counter++;
                hp = hash_func(val);
                while(hp >= Size) hp = hp - Size;
            }
            if(counter > Size) return 2;
        }
        delete newNode;
        return 2;
    }

    int assign(string id, int lv, string para){
        if(para.find('(') == string :: npos){
            int find2 = search(para, lv);
            if(find2 == -1){
                if(isNumber(para)){
                    int find1 = search(id, lv);
                    if(find1 == -1) throw Undeclared(id);
                    else{
                        int idx = find_func(convertValue(Table[find1]->level_of_block, id));
                        if(Table[idx]->type == ""){
                            Table[idx]->type = "number";
                            return 1;
                        }
                        if(Table[idx]->type == "number") return 1;
                        else return 2;
                    }
                }
                if(isValid(para)){
                    int find1 = search(id, lv);
                    if(find1 == -1) throw Undeclared(id);
                    else{
                        int idx = find_func(convertValue(Table[find1]->level_of_block, id));
                        if(Table[idx]->type == ""){
                            Table[idx]->type = "string";
                            return 1;
                        }
                        if(Table[idx]->type == "string") return 1;
                        else return 2;
                    }
                }
                throw Undeclared(para);
            }
            else{
                int idx2 = find_func(convertValue(Table[find2]->level_of_block, para));
                int find1 = search(id, lv);
                if(find1 == -1) throw Undeclared(id);
                else{
                    if(Table[find1]->num_para != -1) return 2; 
                    int idx1 = find_func(convertValue(Table[find1]->level_of_block, id));
                    if(Table[idx1]->type == "" && Table[idx2]->type == "") return 4;
                    if(Table[idx1]->type == "" || Table[idx2]->type == ""){
                        if(Table[idx1]->type == "") Table[idx1]->type = Table[idx2]->type;
                        if(Table[idx2]->type == "") Table[idx2]->type = Table[idx1]->type;
                        return 1;
                    }
                    if(Table[idx1]->type != "" && Table[idx2]->type != ""){
                        if(Table[idx1]->type == Table[idx2]->type) return 1;
                        else return 2;
                    }
                }
            }
        }
        else{
            string func = para.substr(0, para.find('('));
            para = para.substr(para.find('('));
            int find2 = search(func, lv);
            if(find2 != -1){
                if(Table[find2]->type == "void") return 2;
                int idx2 = find_func(convertValue(Table[find2]->level_of_block, func));
                int res = checkFunction(para, Table[idx2]->type_para, Table[idx2]->num_para, lv, idx2);
                if(res == 1){
                    int find1 = search(id, lv);
                    if(find1 == -1) throw Undeclared(id);
                    else{
                        int idx1 = find_func(convertValue(Table[find1]->level_of_block, id));
                        if(Table[idx1]->num_para != -1) return 2;
                        if(Table[idx1]->type == "" && Table[idx2]->type == "") return 4;
                        if(Table[idx1]->type == "" || Table[idx2]->type == ""){
                            if(Table[idx1]->type == "") Table[idx1]->type = Table[idx2]->type;
                            if(Table[idx2]->type == "") Table[idx2]->type = Table[idx1]->type;
                            return 1;
                        }
                        if(Table[idx1]->type != "" && Table[idx2]->type != ""){
                            if(Table[idx1]->type == Table[idx2]->type) return 1;
                            return 2;
                        }
                    } 
                }
                else return res;  
            }
            else throw Undeclared(func);
        }
        return 2;
    }

    int call(string instr, int lv){
        string func = instr.substr(0, instr.find('('));
        instr = instr.substr(instr.find('('));
        int find = search(func, lv);
        if(find != -1){
            if(Table[find]->num_para == -1) return 2;
            int idx = find_func(convertValue(Table[find]->level_of_block, func));
            Table[find]->type = "void";
            return checkFunction(instr, Table[idx]->type_para, Table[idx]->num_para, lv, idx); 
        }
        throw Undeclared(func);
    }

    int search(string id, int lv){
        int idx = -1;
        while(lv >= 0){
            for(int i = 0; i < Size; i++){
                if(Table[i] && Table[i]->identifier == id && Table[i]->level_of_block == lv)
                return i;
            }
            lv--;
        }
        return idx;
    }

    void deleleBlock(int lv){
        for(int i = 0; i < Size; i++){
            if(Table[i] && Table[i]->level_of_block == lv){
                Data *temp = Table[i];
                Table[i] = NULL;
                delete temp;
            }
        }
    }

    int theFirst(){
        for(int i = 0; i < Size; i++){
            if(Table[i]) return i;
        }
        return -1;
    }

    void printTable(){
        int slot1st = theFirst();
        for(int i = slot1st+1; i < Size; i++){
            if(Table[i]){
                cout << ";" << i << " " << Table[i]->identifier << "//" << Table[i]->level_of_block;
            }
        }
        if(slot1st != -1) cout << endl;
    }
};

class SymbolTable
{
public:
    HashTable hashTable;
    SymbolTable() {}
    void run(string filename);
};
#endif