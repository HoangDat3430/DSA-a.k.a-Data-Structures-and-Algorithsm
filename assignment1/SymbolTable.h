#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

    bool isNumber(string str){
        for(char c : str){
            if (isdigit(c) == 0) return false;
        }
        return true;
    }    

    int convert(string c){
        if(c[0] == 'I'){
            return 1;
        }
        else if(c[0] == 'A'){return 2;}
        else if(c[0] == 'B'){return 3;}
        else if(c[0] == 'E'){return 4;}
        else if(c[0] == 'L'){return 5;}
        else if(c[0] == 'P'){return 6;}
        return 7;
    }

    bool Checkvalidcommand(string s){
        regex insert    ("(INSERT)\\s([a-z]\\w*)\\s(string|number)");
        regex assign    ("(ASSIGN)\\s([a-z]\\w*)\\s(.*)");
        regex command   ("(BEGIN|END|PRINT|RPRINT)");
        regex find      ("(LOOKUP)\\s([a-z]\\w*)"); 

        if(regex_match(s, insert)) return true;
        else if(regex_match(s, assign)) return true;
        else if(regex_match(s, command)) return true;
        else if(regex_match(s, find)) return true;
        else return false;
    }
    
    bool Checkvalidvalue(string s){
        regex val       ("\'(\\w|\\d|\\s)*\'");
        if(regex_match(s, val)) return true;
        else return false;
    }

class node{
public:
    string identifier;
    string type;
    string value;
    int level;
    bool activated;
    int duplicated;
    node *prev;
    node *next;

    node(string ide, string typ, int lv){
        identifier = ide;
        type = typ;
        level = lv;
        activated = true; 
        duplicated = 0;   
        next = NULL;
        prev = NULL;
    }
    node(){    
        level = 0;
        activated = true;
        duplicated = 0;
        next = NULL;
        prev = NULL;
    }
};

class DLL{
public: 
    node *head;
    node *tail;

    DLL(){
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        head->prev = NULL;
        tail->next = NULL;
    }  

    ~DLL(){
        node *temp = head->next;
        while (temp != tail){
            node *pre = temp;
            temp = temp->next;
            delete pre;
        }
        delete head;
        delete tail;
    }

    node *GetDeactivatedNode(string ide){
        node *temp = head->next;
        while (temp != tail){
            if(ide == temp->identifier && temp->activated == false) return temp;
            temp = temp->next;
        }
        return NULL;
    }

    node *GetActiveNode(string ide){
        node *temp = head->next;
        while (temp != tail){
            if(ide == temp->identifier && temp->activated == true) return temp;    
            temp = temp->next;
        }
        return NULL;
    }

    void DeactivateBlock(int lv){
        node *temp = EndOfBlock(lv-1);
        while(temp != tail){
            temp->activated = false;
            temp = temp->next;
        }
    }

    node *EndOfBlock(int lv){
        node *temp = head->next;
        while (temp->level <= lv && temp != tail){
            temp = temp->next;
        }
        return temp;
    }

    node *GetDuplicated(string ide, int dupli){
        node *temp = head->next;
        while(temp != tail){
            if(temp->identifier == ide && temp->duplicated == dupli) return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void Insert(string ide, string typ, int lv){
        node *temp = new node(ide, typ, lv);
        if(head->next == tail){
            temp->prev = head;
            temp->next = tail;
            head->next = temp;
            tail->prev = temp;
        }
        else{
            node *temp1 = tail->prev;
            temp->next = tail;
            temp->prev = temp1;
            temp1->next = temp;
            tail->prev = temp;
        }  
    }

    int Assign(string ide, string val){
        node *temp = GetActiveNode(ide);
        node *temp2 = GetActiveNode(val);  
        if(temp2 != NULL){
            if(temp->type == temp2->type) return 1;
            else return 2;
        }
        if((temp->type[0] == 'n' && isNumber(val) == true) ||
        (temp->type[0] == 's' && Checkvalidvalue(val) == true)) return 1;

        if((temp->type[0] == 'n' && Checkvalidvalue(val) == true) ||
        (temp->type[0] == 's' && isNumber(val) == true)) return 2;
        return 0;
    }

    void Print(){
        if(head->next == tail) return;
        node *temp = head->next;
        while (temp != tail->prev){
            if(temp->activated == true){
                cout << temp->identifier << "//" << temp->level << " ";
            }
            temp = temp->next;
        }
        cout << temp->identifier << "//" << temp->level << endl;
    }

    void Rprint(){
        if (tail->prev == head) return;
        node *temp = tail->prev;
        node *temp2 = head->next;
        while(temp2->activated != true){
            temp2 = temp2->next;
        }
        while (temp != temp2){
            if(temp->activated == true){
                cout << temp->identifier << "//" << temp->level << " ";
            }
            temp = temp->prev;
        }
        cout << temp->identifier << "//" << temp->level << endl;
    }

    void EraseBlock(int lv){
        node *temp2 = EndOfBlock(lv-1);
        if(temp2 == tail) return;
        while(temp2->next != tail){
            EraseNode(temp2->next->identifier);
        }
        EraseNode(temp2->identifier);
    }

    void EraseNode(string ide){
        node *temp = GetDeactivatedNode(ide);
        node *pre = temp->prev;
        node *nex = temp->next;
        pre->next = nex;
        nex->prev = pre;
        temp->next = NULL;
        temp->prev = NULL;
        delete temp;
    }
};

class SymbolTable {
public:
    SymbolTable() {}
    DLL Chain;
    void run(string filename);
};
#endif