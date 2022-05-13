#include "SymbolTable.h"

    
void SymbolTable::run(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    int block = 0;
    while(!ifs.eof()){
        string instruction[4];
        int i = 0;
        string temp;
        getline(ifs, temp);
        if(checkInstruction(temp) == true){
            stringstream ss(temp);
            string str;
            while(i < 2){
                getline(ss, str, ' ');
                instruction[i] = str;
                i++;
            }
            if(instruction[0] == "INSERT"){
                getline(ss, str, ' ');
                instruction[2] = str;
                getline(ss, str, ' ');
                instruction[3] = str;
            }
            if(instruction[0] == "ASSIGN"){
                instruction[2] = temp.substr(instruction[0].length() + instruction[1].length() + 2);
            }
            switch (convertCommand(instruction[0]))
            {
                case 0:
                {
                    if (instruction[2] != "number" && instruction[2] != "string" && (block != 0 && instruction[3] == "false")) throw InvalidDeclaration(temp);
                    else{
                        if(myTree.addtNode(instruction[1], instruction[2], block, instruction[3]) == false) throw Redeclared(temp);
                        else{
                            cout << myTree.num_comp << " " << myTree.num_splay << endl;       
                        }
                    }
                    myTree.num_comp = myTree.num_splay = 0;
                    break;
                }
                case 1:
                {
                    int assign = myTree.assign(instruction[1], instruction[2], block);
                    if(assign == 0) throw Undeclared(temp);
                    else if(assign == 2) throw TypeMismatch(temp);
                    else{
                        cout << myTree.num_comp << " " << myTree.num_splay << endl;
                    }
                    myTree.num_splay = myTree.num_comp = 0;
                    break;
                }
                case 2:
                {
                    block++;
                    break;
                }
                case 3:
                {   
                    if(block == 0) throw UnknownBlock();
                    myTree.deleteBlock(block);
                    myTree.num_comp = myTree.num_splay = 0;
                    block--;
                    break;
                }
                case 4:
                {
                    if(myTree.search(instruction[1], block)){
                        cout << myTree.search(instruction[1], block)->data.level << endl;
                        myTree.splay(myTree.search(instruction[1], block));
                        myTree.num_splay = myTree.num_comp = 0;
                    }
                    else{
                        myTree.num_splay = myTree.num_comp = 0;
                        throw Undeclared(temp);
                    }
                    break;
                }
                case 5:
                {
                    if(myTree.root){
                        cout << myTree.root->data.identifier << "//" << myTree.root->data.level;
                        myTree.printPre(myTree.root);
                        cout << endl;
                    }
                    break;
                }
            }
        }
        else{
            throw InvalidInstruction(temp);
        }
    }
    if(block != 0) throw UnclosedBlock(block);
    ifs.close();
}