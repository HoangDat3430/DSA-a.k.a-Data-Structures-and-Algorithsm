#include "SymbolTable.h"

void SymbolTable::run(string filename){
    ifstream ifs;
    ifs.open(filename);
    int lv = 0;
    while(!ifs.eof()){
        string command[3];
        int i = 0;
        string temp = "";
        getline(ifs, temp);
        if(Checkvalidcommand(temp) == true){
            stringstream ss(temp);
            string str;
            while(getline(ss, str, ' ') && i < 2){
                command[i] = str;
                i++;
            }
            if(command[0] == "INSERT" || command[0] == "ASSIGN"){
                command[2] = temp.substr(command[0].length() + command[1].length() + 2);
            }
            switch(convert(command[0])){
                case 1:
                {
                    if(Chain.GetActiveNode(command[1]) != NULL){
                        if(Chain.GetActiveNode(command[1])->level == lv || lv == 0) throw Redeclared(temp);
                        else if(Chain.GetActiveNode(command[1])->level < lv){
                            int dup = Chain.GetActiveNode(command[1])->duplicated + 1;
                            Chain.GetActiveNode(command[1])->activated = false;
                            Chain.Insert(command[1], command[2], lv);
                            Chain.GetActiveNode(command[1])->duplicated = dup;
                            cout << "success" << endl;
                        }
                    }
                    else{
                        Chain.Insert(command[1], command[2], lv);
                        cout << "success" << endl;
                    }
                    break;
                }
                case 2:
                {   
                    if (Chain.GetActiveNode(command[1]) == NULL) throw Undeclared(temp); 
                    if (Chain.Assign(command[1], command[2]) == 1) cout << "success" << endl;
                    else if (Chain.Assign(command[1], command[2]) == 0) throw Undeclared(temp);
                    else throw TypeMismatch(temp);
                    break;
                }
                case 3:
                {   
                    lv++;
                    break;
                }
                case 4:
                {   
                    if(lv <= 0) throw UnknownBlock();
                    if(lv > 0){
                        node *temp2 = Chain.tail;
                        node *temp = Chain.EndOfBlock(lv-1);
                        while(temp != temp2){;
                            if(temp->duplicated > 0){
                                Chain.GetDuplicated(temp->identifier, temp->duplicated-1)->activated = 1;
                            }
                            temp = temp->next;
                        }
                    }
                    Chain.DeactivateBlock(lv);
                    Chain.EraseBlock(lv);
                    lv--;
                    break;
                }
                case 5:
                {
                    if(Chain.GetActiveNode(command[1]) != NULL){
                        cout << Chain.GetActiveNode(command[1])->level << endl;
                    }
                    else throw Undeclared(temp);
                    break;
                }
                case 6:
                {   
                    Chain.Print();
                    break;
                }
                case 7:
                {
                    Chain.Rprint();
                    break;
                }
            }
        }
        else throw InvalidInstruction(temp);
    }
    if(lv > 0)throw UnclosedBlock(lv);
    ifs.close();
}

