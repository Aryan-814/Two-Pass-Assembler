// Two Pass Assembler for Simplex Assembly
// Project of CS2206 : Computer Architecture - Prof : Dr Jimson Matthew
//Author : Aryan, Roll No : 2401CS48

#include <bits/stdc++.h>
using namespace std;

// Global Tables
map<string, pair<int, int>> opcode = {
    //opcode, no of operands
    {"ldc", {0, 1}},
    {"adc", {1, 1}},
    {"ldl", {2, 1}},
    {"stl", {3, 1}},
    {"ldnl", {4, 1}},
    {"stnl", {5, 1}},
    {"add", {6, 0}},
    {"sub", {7, 0}},
    {"shl", {8, 0}},
    {"shr", {9, 0}},
    {"adj", {10, 1}},
    {"a2sp", {11, 0}},
    {"sp2a", {12, 0}},
    {"call", {13, 1}},
    {"return", {14, 0}},
    {"brz", {15, 1}},
    {"brlz", {16, 1}},
    {"br", {17, 1}},
    {"HALT", {18, 0}},
    {"data", {-1, 1}},
    {"SET", {-2, 1}}
};

//Stores the label and PC value
map<string, int> symbol;
//Stores the errors
vector<string> errors;
//Stores the warnings
vector<string> warnings;
//Track label use for warning
map<string, bool> label_used;
//Stores instructions in internal memory
map<int, pair<string, string>> instructions; 
//Stores PC value and corresponding labels
map<int, vector<string>> pc_labels;         

// Check if a label is valid 
bool checklabel(string &label) {
    if (label.empty() || !isalpha(label[0])) return false; 
    for (char c : label) {
        if (!isalnum(c)) return false; 
    }
    return true;
}

//Check if a number is valid
bool checknumber(string &str, int &result){
    if(str.empty()) return false;

    char *endptr;
    long val = strtol(str.c_str(), &endptr, 0);
    if (*endptr != '\0') {
        return false; 
    }

    result = val;
    return true;
}


void extract(string line, string &label, string &mnemonic, string &operand, string &extra) {
    label = mnemonic = operand = extra = "";
    
    // Remove comments 
    size_t comment_pos = line.find(';');
    if (comment_pos != string::npos) {
        line = line.substr(0, comment_pos);
    }

    // Extract label 
    size_t colon_pos = line.find(':');
    string remainder = line;
    if (colon_pos != string::npos) {
        label = line.substr(0, colon_pos);
        //Removing spaces in between
        label.erase(remove_if(label.begin(), label.end(), ::isspace), label.end());
        remainder = line.substr(colon_pos + 1);
    }

    // Extract mnemonic, operand, and anything trailing
    stringstream ss(remainder);
    ss >> mnemonic >> operand >> extra;
}

void first_pass(const string &filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    //Initializing
    string line, label, mnemonic, operand, extra;
    int pc = 0, line_number = 0;

    // Processing File
    while (getline(infile, line)) {

        line_number++;
        extract(line, label, mnemonic, operand, extra);

        // Process the Label 
        if (!label.empty()) {
            if (!checklabel(label)) {
                //Marks as invalid
                errors.push_back("Error at line " + to_string(line_number) + ": bogus label name '" + label + "'");
            } else if (symbol.count(label)) {
                //Finds if a duplicate label is used 
                errors.push_back("Error at line " + to_string(line_number) + ": duplicate label definition '" + label + "'");
            } else {

                // Determine if this is a SET label or a standard PC label
                if (mnemonic == "SET" && !operand.empty()) {
                    int val;
                    //Checks Number format
                    if (checknumber(operand, val)) {
                        symbol[label] = val; 
                    } else {
                        errors.push_back("Error at line " + to_string(line_number) + ": invalid number format '" + operand + "'");
                    }
                } else {
                    symbol[label] = pc; 
                    pc_labels[pc].push_back(label);
                }

                // Label usage tracking
                label_used[label] = false;
            }
        }

        // Process the Instruction
        if (!mnemonic.empty()) {
            if (opcode.count(mnemonic) == 0) {
                errors.push_back("Error at line " + to_string(line_number) + ": bogus mnemonic '" + mnemonic + "'");
            } else {
                auto inst = opcode[mnemonic];


                // Check for syntax errors
             if (inst.second == 0 && operand.size()) {
                    //Finds no operand
                    errors.push_back("Error at line " + to_string(line_number) + ": unexpected operand");
                } else if (inst.second == 1 && operand.empty()) {
                    //No operand
                    errors.push_back("Error at line " + to_string(line_number) + ": missing operand");
                } else if (!extra.empty()) {
                    //Found extra after the operand
                    errors.push_back("Error at line " + to_string(line_number) + ": extra on end of line");
                }else if(inst.second == 1 && !isalpha(operand[0])){
                    //Checks format
                    int temp;
                    if (!checknumber(operand, temp)) {
                        errors.push_back("Error at line " + to_string(line_number) + ": invalid number format");
                    }
                }

                if (mnemonic != "SET") {
                    instructions[pc] = {mnemonic, operand};
                    pc++;
                }

            }
        }
    }

    infile.close();

}

// Check if an instruction is a branch type
bool is_branch_instruction(string &mnemonic) {

    if (mnemonic == "br" || mnemonic == "brz" || mnemonic == "brlz" || mnemonic == "call") {
        return true;
    } else {
        return false;
    }

}

bool second_pass(const string &objfile, const string &lstfile) {
    bool flag = true;
    ofstream bin_out(objfile, ios::binary);
    ofstream lst_out(lstfile);

    if (!bin_out.is_open() || !lst_out.is_open()) {
        cerr << "Error: Could not open output files for Pass 2." << endl;
        exit(1);
    }

    int total_instructions = instructions.size();
    for (int pc = 0; pc<=total_instructions; pc++) {
        //Output any labels assigned to this PC
        if (pc_labels.count(pc)) {
            for (const string &lbl : pc_labels[pc]) {
                lst_out << setw(8) << setfill('0') << hex << pc << "         " << lbl << ":" << endl;
            }
        }

        // Break if we just finished the trailing labels
        if (pc == total_instructions) break;

        // Fetch the instruction from the map
        string mnemonic = instructions[pc].first, operand = instructions[pc].second;
        int machine_code = 0, operand_val = 0;

        //Resolve the operand
        if (operand.size()) {
            if (isalpha(operand[0])) { 
                if (symbol.count(operand) == 0) {
                    //Label no found
                    errors.push_back("Error at PC " + to_string(pc) + ": no such label '" + operand + "'");
                    flag = false;
                }

                //Mark the label as used
                label_used[operand] = true;
                int target_address = symbol[operand];

                // Determining operand value
                if (is_branch_instruction(mnemonic)) {
                    //PC relative Addressing
                    operand_val = target_address - (pc + 1); 
                } else {
                    //Absolute Addressing
                    operand_val = target_address;
                }
            } else {
                operand_val = stol(operand, nullptr, 0);
            }
        }

        // Encode the Machine Code
        if (mnemonic == "data") {
            machine_code = operand_val; 
        } else {
            int opc = opcode[mnemonic].first;   
            machine_code = ((operand_val & 0xFFFFFF) << 8) | (opc & 0xFF);
        }

        //formatting
        bin_out.write(reinterpret_cast<const char*>(&machine_code), sizeof(machine_code));

        lst_out << setw(8) << setfill('0') << hex << uppercase << pc << " "
                << setw(8) << setfill('0') << hex << uppercase << machine_code << " "
                << mnemonic;

        if (operand.size()) lst_out << " " << operand;
        lst_out << endl;
    }

    bin_out.close();
    lst_out.close();
    return flag;
}

void checkwarnings(){
    for (auto &pair : label_used) {
        if (!pair.second) {
            warnings.push_back("Warning: Label '" + pair.first + "' was defined but never used.");
        }
    }
}

// Main execution function
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Use this: " << argv[0] << " <source.asm>" << endl;
        return 0;
    }

    string input_file = argv[1];
    size_t last_dot = input_file.find_last_of(".");
    string base = (last_dot == string::npos) ? input_file : input_file.substr(0, last_dot);
    
    string logfile = base + ".log";
    ofstream log_out(logfile);
    if (!log_out.is_open()) {
        cerr << "Error: Could not open log file." << endl;
        return 0;
    }

    first_pass(input_file);
    if(errors.empty()){
        bool pass = second_pass(base + ".o", base + ".lst");
        if(pass){
            log_out << "Compiled successfully." << endl;
        }
    }

    if (errors.size()) {
        for (auto &err : errors) {
            log_out << err << endl;
        }
    } 

    //Write warnings
    checkwarnings();
    //Write on the file
    for (auto &warn : warnings) {
        log_out << warn << endl;
    }
    
    log_out.close();
    return 0;
}