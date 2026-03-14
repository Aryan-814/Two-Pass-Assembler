// Emulator for Simplex Assembly
// Project of CS2206 : Computer Architecture - Prof : Dr Jimson Matthew
//Author : Aryan, Roll No : 2401CS48

#include<bits/stdc++.h>
using namespace std;

// memory and registers
vector<int> mem((1 << 24), 0); 
int A = 0, B = 0, SP = 0, PC = 0;
int instcount = 0;
int codesz = 0;
bool flag = true;

// Global Tables
vector<string> op = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};

// formats output to 8-character hex
void printhex(int value) {
    cout << setfill('0') << setw(8) << hex << value;
}

void load_file(string filename) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cout << "Error: could not open file.\n";
        exit(1);
    }

    int instruction;
    int index = 0;
    
    // read 4 bytes at a time directly into memory
    while (infile.read(reinterpret_cast<char*>(&instruction), sizeof(instruction))) {
        mem[index] = instruction;
        index++;
    }
    codesz = index;
    infile.close();
    cout << "Successfully loaded " << dec << codesz << " instructions.\n";
}

void showreg() {
    cout << "A: "; printhex(A);
    cout << endl;
    cout << "B: "; printhex(B);
    cout << endl;
    cout << "SP: "; printhex(SP);
    cout << endl;
    cout << "PC: "; printhex(PC);
    cout << endl;
}

void printisa() {
    cout << "op   mnemonic   operand\n";
    cout << "0    ldc        value\n";
    cout << "1    adc        value\n";
    cout << "2    ldl        value\n";
    cout << "3    stl        value\n";
    cout << "4    ldnl       value\n";
    cout << "5    stnl       value\n";
    cout << "6    add        \n";
    cout << "7    sub        \n";
    cout << "8    shl        \n";
    cout << "9    shr        \n";
    cout << "10   adj        value\n";
    cout << "11   a2sp       \n";
    cout << "12   sp2a       \n";
    cout << "13   call       offset\n";
    cout << "14   return     \n";
    cout << "15   brz        offset\n";
    cout << "16   brlz       offset\n";
    cout << "17   br         offset\n";
    cout << "18   halt       \n";
}

void dumpmem() {
    for (int i = 0; i < mem.size(); i++) {
        if (i < codesz + 5) {
            printhex(i); cout << " "; printhex(mem[i]); cout << endl;
        } else if (mem[i] != 0) {
            printhex(i); cout << " "; printhex(mem[i]); cout << endl;
        }
    }
}

void execute(int trace) {
    if (PC >= codesz || PC < 0) {
        cout << "Segmentation fault: pc out of bounds.\n";
        flag = false;
        return;
    }

    // fetch
    int inst = mem[PC];
    PC++;
    int opcode = inst & 0xFF; // bottom 8 bits
    int operand = inst >> 8;  // top 24 bits

    if (trace == 1) {
        cout << "PC: "; printhex(PC); 
        cout<<endl;
        cout << "INST: " << op[opcode] << " " << dec << operand << "\n";
    }

    int memtype = 0; // 0 = none, 1 = read, 2 = write
    int memaddr = 0, memval = 0, memold = 0;

    // execute
    switch (opcode) {
        case 0:  //ldc
            B = A; 
            A = operand; 
            break;  
        case 1:  //adc
            A += operand;
            break;                                   
        case 2:  //ldl
            memaddr = SP + operand;
            B = A; 
            if (memaddr < 0 || memaddr >= mem.size()) {
                flag = false;
                cout << "Memory Fault: Attempted to access invalid address at "; printhex(memaddr); cout << "\n";
                return ;
            }
            A = mem[memaddr]; 
            memtype = 1; memval = A;
            break; 
        case 3:  //stl
            memaddr = SP + operand;
            if (memaddr < 0 || memaddr >= mem.size()) {
                flag = false;
                cout << "Memory Fault: Attempted to access invalid address at "; printhex(memaddr); cout << "\n";
                return ;
            }
            memold = mem[memaddr];
            mem[memaddr] = A; 
            A = B; 
            memtype = 2;
            memval = mem[memaddr];
            break;
        case 4:  //ldnl
            memaddr = A + operand;
            if (memaddr < 0 || memaddr >= mem.size()) {
                flag = false;
                cout << "Memory Fault: Attempted to access invalid address at "; printhex(memaddr); cout << "\n";
                return ;
            }
            A = mem[memaddr]; 
            memtype = 1; 
            memval = A;
            break;
        case 5:  //stnl
            memaddr = A + operand;
            if (memaddr < 0 || memaddr >= mem.size()) {
                flag = false;
                cout << "Memory Fault: Attempted to access invalid address at "; printhex(memaddr); cout << "\n";
                return ;
            }
            memold = mem[memaddr];
            mem[memaddr] = B; 
            memtype = 2; 
            memval = mem[memaddr];
            break;               
        case 6:  //add
            A += B; 
            break;                                     
        case 7:  //sub
            A = B - A; 
            break;                              
        case 8:  //shl
            A = B << A; 
            break;                         
        case 9:  //shr
            A = B >> A; 
            break;                        
        case 10: //adj
            SP += operand; 
            break;                                 
        case 11: //a2sp
            SP = A;
            A = B; 
            break;                    
        case 12: //sp2a
            B = A; 
            A = SP; 
            break;                      
        case 13: //call
            B = A; 
            A = PC; 
            PC += operand; 
            break;   
        case 14: //return
            PC = A; 
            A = B; 
            break;                     
        case 15: //brz
            if (A == 0) PC += operand;
            break;                 
        case 16: //brlz
            if (A < 0) PC += operand; 
            break;                  
        case 17: //br
            PC += operand; 
            break;                                  
        case 18: //halt
            flag = false; 
            break;                              
        default: 
            cout << "Error: invalid opcode detected.\n"; 
            flag = false; 
            break;
    }

    instcount++;
}

void run(int trace) {
    while (flag) {
        execute(trace);
    }

    cout << "Program halted.\n";
    cout << dec << instcount << " Instructions executed total.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Use this: " << argv[0] << " <source.o>" << endl;
        return 0;
    }

    string filename = argv[1];
    load_file(filename);

    string command;
    while (true) {

        cout << "\nEmulator command \n";
        cout << "1) -run : To run the entire program \n";
        cout << "2) -t : To run a single instruction at time\n";
        cout << "3) -reg : Show registers\n";
        cout << "4) -dump : Memory dump using instruction\n";
        cout << "5) -isa : Show instruction set using\n";
        cout << "6) -quit : Exit the Emulator\n";
        cin >> command;

        if (command == "-quit") {
            break;
        } else if (command == "-dump") {
            dumpmem();
        } else if (command == "-reg") {
            showreg();
        }else if (command == "-t") {
            if (flag){
                execute(1);
                showreg();
            }else{
                cout << "Program has already halted.\n";
            }
        } else if (command == "-run") {
            run(0);
        } else if (command == "-isa") {
            printisa();
        }else{
            cout << "Invalid command.\n";
        }
    }

    return 0;
}