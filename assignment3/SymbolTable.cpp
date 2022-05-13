#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    int block = 0;
    bool table_initialized = 0;
    while(!ifs.eof()){
        string instruction[4];
        string temp;
        getline(ifs, temp); 
        if(checkInstruction(temp) == true){
            stringstream ss(temp);
            string str;
            getline(ss, str, ' ');
            instruction[0] = str;

            if(instruction[0] == "INSERT"){
                getline(ss, str, ' ');
                instruction[1] = str;
                getline(ss, str, ' ');
                instruction[2] = str;
                if(instruction[2] == instruction[1]) instruction[2] = "";
                if(instruction[2] != "" && block != 0) throw InvalidDeclaration(instruction[1]); 
            }
            if(instruction[0] == "ASSIGN" || instruction[0] == "LINEAR" || instruction[0] == "DOUBLE"){
                getline(ss, str, ' ');
                instruction[1] = str;
                instruction[2] = temp.substr(instruction[0].length() + instruction[1].length() + 2);
            }
            if(instruction[0] == "QUADRATIC"){
                getline(ss, str, ' ');
                instruction[1] = str;
                getline(ss, str, ' ');
                instruction[2] = str;
                getline(ss, str, ' ');
                instruction[3] = str;
            }
            if(instruction[1] == "number" || instruction[1] == "string" || instruction[1] == "void") throw InvalidInstruction(temp);
            if(instruction[0] == "CALL" || instruction[0] == "LOOKUP") instruction[1] = temp.substr(instruction[0].length()+1);
            switch (convertCommand(instruction[0]))
            {
                case 0:
                {
                    if(table_initialized) throw InvalidDeclaration(temp);
                    hashTable.Pattern = "linear";
                    hashTable.Size = stoi(instruction[1]);
                    hashTable.c1 = stoi(instruction[2]);
                    table_initialized = 1;
                    break;
                }
                case 1:
                {
                    if(table_initialized) throw InvalidDeclaration(temp);
                    hashTable.Pattern = "quadratic";
                    hashTable.Size = stoi(instruction[1]);
                    hashTable.c1 = stoi(instruction[2]);
                    hashTable.c2 = stoi(instruction[3]);
                    table_initialized = 1;
                    break;
                }
                case 2:
                {
                    if(table_initialized) throw InvalidDeclaration(temp);
                    hashTable.Pattern = "double";
                    hashTable.Size = stoi(instruction[1]);
                    hashTable.c1 = stoi(instruction[2]);
                    table_initialized = 1;
                    break;
                }
                case 3:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    long long Value = convertValue(block, instruction[1]);
                    int insert = hashTable.insertion(instruction[1], block, Value, instruction[2]);
                    if(insert == 2) throw Overflow(temp);
                    else if(insert == 0) throw Redeclared(instruction[1]);
                    else cout << hashTable.counter << endl;
                    hashTable.counter = 0;
                    break;
                }
                case 4:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    int assign = hashTable.assign(instruction[1], block, instruction[2]);
                    if(assign == 4) throw TypeCannotBeInferred(temp);
                    else if(assign == 2) throw TypeMismatch(temp);
                    else cout << hashTable.total << endl;
                    hashTable.total = 0;
                    break;
                }
                case 5:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    block++;
                    break;
                }
                case 6:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    if(block == 0) throw UnknownBlock();
                    hashTable.deleleBlock(block);
                    block--;
                    break;
                }
                case 7:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    int slot = hashTable.search(instruction[1], block);
                    if(slot == -1) throw Undeclared(instruction[1]);
                    cout << slot << endl;
                    break;
                }
                case 8:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    int slot1st = hashTable.theFirst();
                    if(slot1st != -1) cout << slot1st << " " << hashTable.Table[slot1st]->identifier << "//" << hashTable.Table[slot1st]->level_of_block;
                    hashTable.printTable();
                    break;
                }
                case 9:
                {
                    if(!table_initialized) throw InvalidInstruction(temp);
                    int call = hashTable.call(instruction[1], block);
                    if(call == 2) throw TypeMismatch(temp);
                    else if(call == 3) throw InvalidInstruction(temp);
                    else if(call == 4) throw TypeCannotBeInferred(temp);
                    cout << hashTable.total << endl;
                    hashTable.total = 0;
                    break;
                }
            }
        }
        else throw InvalidInstruction(temp);
    }
    if(block != 0) throw UnclosedBlock(block);
}