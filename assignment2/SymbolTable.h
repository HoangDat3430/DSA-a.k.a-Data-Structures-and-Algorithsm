#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct Data{
    string identifier;
    string type;
    int level;
};

bool checkInstruction(string s){
    regex insert    ("(INSERT)\\s([a-z]\\w*)\\s(string|number|(((\\((number|string)(,number|,string)*\\))|(\\(\\)))->(string|number)))\\s(true|false)");
    regex assign    ("(ASSIGN)\\s([a-z]\\w*)\\s(.*)");
    regex one_word_command   ("(BEGIN|END|PRINT)");
    regex lookup    ("(LOOKUP)\\s([a-z]\\w*)");

    if(regex_match(s, insert)) return true;
    else if(regex_match(s, assign)) return true;
    else if(regex_match(s, one_word_command)) return true;
    else if(regex_match(s, lookup)) return true;
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
    if(s[0] == 'I') return 0;
    else if(s[0] == 'A') return 1;
    else if(s[0] == 'B') return 2;
    else if(s[0] == 'E') return 3;
    else if(s[0] == 'L') return 4;
    else return 5;
}

int compareData(Data data1, Data data2){
    if (data1.level > data2.level) return 1;
    if (data1.level < data2.level) return 0;
    if (data1.identifier.compare(data2.identifier) == 0) return 2;
    else{
        for(unsigned int i = 0; i < max(data1.identifier.length(), data2.identifier.length()); i++){
            if(data1.identifier[i] > data2.identifier[i]) return 1;
            else if(data1.identifier[i] < data2.identifier[i]) return 0;
        }
    }
    return 0;
}

class SplayTree{
public:
    class Node{
    public:
        friend class SplayTree;
        Data data;
        Node *Parent;
        Node *Left;
        Node *Right;  
        Node(string iden_input, string type_input, int lv_input){
            data.identifier = iden_input;
            data.type = type_input;
            data.level = lv_input;
            Left = NULL;
            Right = NULL;
            Parent = NULL;
        }
    };
    Node *root;
    int num_comp = 0;
    int num_splay = 0;
    bool isSplay = 0;
    SplayTree(){root = NULL;}
    ~SplayTree(){clear(root);}
    
    Node *searchLast(string ide, int lv){
        Node *temp;
        Data key;
        key.identifier = ide;
        key.level = lv;
        while(lv >= 0){
            temp = root;
            while(temp){
                if(compareData(temp->data, key) == 1) temp = temp->Left;
                else if(compareData(temp->data, key) == 0) temp = temp->Right;
                else return temp;
            }
            key.level--;
            lv--;
        }
        return NULL;
    }
    Node *search(string ide, int lv){
        Node *lastNode = searchLast(ide, lv);
        Node *temp = root;
        if(lastNode){
            while(temp){
                num_comp++;
                int comp = compareData(temp->data, lastNode->data);
                if(comp == 2) return temp;
                if(comp == 1) temp = temp->Left;
                else if(comp == 0) temp = temp->Right;
            }
        }
        return lastNode;
    }
    Node *search_not_count(string ide, int lv){
        Node *lastNode = searchLast(ide, lv);
        Node *temp = root;
        if(lastNode){
            while(temp){
                int comp = compareData(temp->data, lastNode->data);
                if(comp == 2) return temp;
                if(comp == 1) temp = temp->Left;
                else if(comp == 0) temp = temp->Right;
            }
        }
        return lastNode;
    }
    bool addtNode(string ide, string typ, int lv, string locked){
        if(locked == "true") lv = 0;
        if(!root){ 
            root = new Node(ide, typ, lv);
            return 1;
        }
        Node *temp = root;
        Node *new_Node = new Node(ide, typ, lv);
        while(temp){
            num_comp++;
            int comp = compareData(temp->data, new_Node->data);
            if(comp == 2){
                delete(new_Node);
                return 0;
            }
            if(comp == 1){
                if(!temp->Left){
                    new_Node->Parent = temp;
                    temp->Left = new_Node;
                    if(splay(new_Node)) num_splay++;
                    return 1;
                }
                temp = temp->Left;
            }
            else{
                if(!temp->Right){
                    new_Node->Parent = temp;
                    temp->Right = new_Node;
                    if(splay(new_Node)) num_splay++;              
                    return 1;
                }
                temp = temp->Right;
            }
        }
        delete(new_Node);
        return 0;
    }
    int checkFuncion(string parameters, string func_types, int lv){
        string para[100];
        string fnc[100];
        int counter_para = 0;
        int counter_fnc = 0; 
        for(unsigned int i = 1; i < parameters.length()-1; i++){
            if(parameters[i] != ','){
                para[counter_para].push_back(parameters[i]);
            }
            else{
                if(para[counter_para].length() != 0) counter_para++;
            }
        }
        for(unsigned int i = 1; i < func_types.length()-1; i++){
            if(func_types[i] != ','){
                fnc[counter_fnc].push_back(func_types[i]);
            }
            else{
                if(fnc[counter_fnc].length() != 0) counter_fnc++;
            }
        }
        if(counter_fnc == counter_para && counter_para >= 0){
            if(counter_fnc == 0 && (para[0] == "" || fnc[0] == "")){
                if(para[0] == "" && fnc[0] == ""){
                    return 1;
                }
                else return 2;
            }
            for(int i = 0; i <= counter_para; i++){
                if((!isNumber(para[i]) && !isValid(para[i])) || para[i] == "" || fnc[i] == ""){
                    if((para[i] == "" && fnc[i] != "") || (fnc[i] == "" && para[i] != "")) return 2;
                    if(para[i].find('(') != string::npos && para[i].substr(para[i].length()-1, para[i].length()) == ")"){
                        string name = para[i].substr(0, para[i].find('('));
                        Node *temp = search_not_count(name, lv);
                        if(temp){
                            temp = search(name, lv);
                            if(splay(temp)) num_splay++;
                            int len_name = temp->data.type.length();
                            string para_input = para[i].substr(para[i].find('('), para[i].length());
                            string fnc_types = temp->data.type.substr(0, len_name-8);
                            int checkParam = checkFuncion(para_input, fnc_types, lv);
                            if(checkParam == 2) return 2;
                            if(checkParam == 0) return 0;
                        }
                        else return 0;
                    }
                    else{  
                        Node *temp = search_not_count(para[i], lv);
                        if(temp){                           
                            temp = search(para[i], lv);
                            if(splay(temp)) num_splay++;
                            if(temp->data.type != "number" && temp->data.type != "string"){
                                return 2;
                            }
                            int len_temp = temp->data.type.length();
                            if(temp->data.type.substr(len_temp-6, len_temp) != fnc[i]) return 2;
                        }
                        else{
                            return 0;
                        }
                    }
                }
                else{
                    if((fnc[i] == "number" && !isNumber(para[i])) 
                    || (fnc[i] == "string" && !isValid(para[i]))){
                        return 2;
                    }                    
                }
            }
            return 1;
        }
        return 2;
    }
    int assign(string ide, string typ, int lv){
        if(typ.find('(') != string :: npos){
            Node *temp2 = search_not_count(typ.substr(0, typ.find('(')), lv);
            if(temp2){
                temp2 = search(typ.substr(0, typ.find('(')), lv);
                if(splay(temp2)) num_splay++;                             
                int len_temp2 = temp2->data.type.length();
                string para_input = typ.substr(typ.find('('), typ.length());
                string fnc_types = temp2->data.type.substr(0, len_temp2 - 8);
                int checkParam = checkFuncion(para_input, fnc_types, lv);
                if(checkParam == 2) return 2;
                if(checkParam == 0) return 0;
                else{                    
                    Node *temp = search(ide, lv);
                    if(!temp) return 0;
                    if(temp->data.type != "number" && temp->data.type != "string") return 2;
                    else{                     
                        if(splay(temp)) num_splay++;  
                        int len_temp = temp->data.type.length();
                        if(temp->data.type.substr(len_temp-6, len_temp) == temp2->data.type.substr(len_temp2-6, len_temp2))
                        {
                            return 1;
                        }
                        else return 2; 
                    }
                }
            }
            else return 0;
        }
        else{
            if(!isNumber(typ) && !isValid(typ)){
                Node *temp2 = search(typ, lv);
                if(!temp2) return 0;
                else{
                    if(splay(temp2)) num_splay++;
                    Node *temp = search(ide, lv);
                    if(!temp) return 0;
                    if(temp->data.type != "string" && temp->data.type != "number") return 2;
                    else{  
                        if(splay(temp)) num_splay++;                   
                        int len_temp = temp->data.type.length();
                        int len_temp2 = temp2->data.type.length();
                        if(temp->data.type.substr(len_temp-6, len_temp) == temp2->data.type.substr(len_temp2-6, len_temp2)){
                            return 1;
                        }  
                        else return 2; 
                    }
                }
            }
            else{
                Node *temp = search(ide, lv);
                if(!temp) return 0;
                if(temp->data.type != "string" && temp->data.type != "number") return 2;
                if(splay(temp)) num_splay++;
                int len_temp = temp->data.type.length();
                if((temp->data.type.substr(len_temp-6, len_temp) == "number" && isNumber(typ)) 
                || (temp->data.type.substr(len_temp-6, len_temp) == "string" && isValid(typ))){  
                    return 1;
                }
                return 2;
            }
        }
        return 0;
    }
    void rotateLeft(Node *node){
        Node *temp = node->Right;
		node->Right = temp->Left;
		if(temp->Left != NULL){
			temp->Left->Parent = node;
		}
		temp->Parent = node->Parent;
		if(node->Parent == NULL){
			root = temp;
		} 
        else if(node == node->Parent->Left){
			node->Parent->Left = temp;
		} 
        else{
			node->Parent->Right = temp;
		}
		temp->Left = node;
		node->Parent = temp;
    }
    void rotateRight(Node *node){
        Node *temp = node->Left;
		node->Left = temp->Right;
		if(temp->Right != NULL){
			temp->Right->Parent = node;
		}
		temp->Parent = node->Parent;
		if (node->Parent == NULL){
			root = temp;
		} 
        else if(node == node->Parent->Right){
			node->Parent->Right = temp;
		} 
        else{
			node->Parent->Left = temp;
		}
		temp->Right = node;
		node->Parent = temp;
    }
    bool splay(Node *node){
        while(node->Parent != NULL){
			if(!node->Parent->Parent){
				if(node == node->Parent->Left) rotateRight(node->Parent);
                else rotateLeft(node->Parent);
			} 
            else if(node == node->Parent->Left && node->Parent == node->Parent->Parent->Left){
				rotateRight(node->Parent->Parent);
				rotateRight(node->Parent);
			}
            else if(node == node->Parent->Right && node->Parent == node->Parent->Parent->Right){
				rotateLeft(node->Parent->Parent);
				rotateLeft(node->Parent);
			} 
            else if(node == node->Parent->Right && node->Parent == node->Parent->Parent->Left){
				rotateLeft(node->Parent);
				rotateRight(node->Parent);
			} 
            else{
				rotateRight(node->Parent);
				rotateLeft(node->Parent);
			}
            isSplay = true;
		}
        if(!isSplay) return 0;
        isSplay = 0;
        return 1;       
    }
    void printPre(Node *temp){
        if(temp == root){
            printPre(temp->Left);
            printPre(temp->Right);
        }
        if(temp && temp != root){
            cout << " " << temp->data.identifier << "//" << temp->data.level;
            printPre(temp->Left);
            printPre(temp->Right);
        }
    }
    void deleteBlock(int lv){
        while(searchLevel(lv)){
            Node *temp = searchLevel(lv);
            deleteNode(temp);
        }
    }
    Node *searchLevel(int lv){
        Node *temp = root;
        while(temp){
            if(temp->data.level == lv) return temp;
            if(temp->data.level > lv) temp = temp->Left;
            else temp = temp->Right;
        }
        return temp;
    }
    void deleteNode(Node *node){
        Node *temp = root;
		Node *x = NULL;
		Node *t, *s;
		while (temp){
            int comp = compareData(temp->data, node->data);
			if(comp == 2) x = temp;
			if(comp == 0) temp = temp->Right;
            else temp = temp->Left; 
		}

		if(!x) return;
		split(x, s, t);
		if (s->Left) s->Left->Parent = NULL;
		root = join(s->Left, t);
		delete(s);
	}
    void split(Node *&x, Node *&s, Node *&t){
		splay(x);
		if(x->Right){
			t = x->Right;
			t->Parent = NULL;
		} 
        else t = NULL;
		s = x;
		s->Right = NULL;
		x = NULL;
	} 
    Node *join(Node *s, Node *t){
		if (!s) return t;
		if (!t) return s;
		Node *x = maximum(s);
		splay(x);
		x->Right = t;
		t->Parent = x;
		return x;
	}
    Node *maximum(Node *node){
		while (node->Right != NULL) node = node->Right;		
		return node;
	}
    void clear(Node *&eraser){
        if (eraser && eraser->Left) clear(eraser->Left);
        if (eraser && eraser->Right) clear(eraser->Right);
        delete eraser;
    }

};


class SymbolTable
{
public:
    SymbolTable() {}
    SplayTree myTree;
    void run(string filename);
};
#endif