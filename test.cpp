/* On my honor, I have neither given nor received unauthorized aid on this assignment */
//  ************************************************************** Take out all print statements before submitting *******************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <math.h>
#include <vector>
#include <utility>


using namespace std;

// Creates the memory to data map
map<int,int> calculateDataValues(map<int,string> mem_data){
  int value;
  map<int,int> mem_value;
  for(map<int,string>::iterator it = mem_data.begin(); it != mem_data.end(); it++){
    // Determine if twos complement
    if(it->second.front() == '1'){
      //cout << "\n" << "___________________" << "\n" << "original: " << it->second << "\t";
      value = 0;
      // Logic to convert the twos complemeented number back into normal binary
      // Subtract one first. Logic: turn rightmost 1 to 0 and then convert all the rest of the rightmost 0's to 1
      for(int i = it->second.length()-1; i > 0; i--){
        if(it->second.at(i) == '1'){
          it->second.at(i) = '0';
          for(int j = i+1; j < it->second.length(); j++){
            it->second.at(j) = '1';
          }
          //cout << "step 1: " << it->second << "\t\n";
          break;
        }
      }
      // Complement the number back
      for(int i = it->second.length()-1; i > 0; i--){
        if(it->second.at(i) == '0'){
          it->second.at(i) = '1';
        }
        else if(it->second.at(i) == '1'){
          it->second.at(i) = '0';
        }
      }
      // Calculate values of the 1's and just multiply it by -1 and add to value.
      for(int i = it->second.length()-1; i> 0; i--){
        if(it->second.at(i) == '1'){
          //cout << "step 3: " << it->second << "\t\n";
          //cout << "prevalue " << value;
          value = value + -1 * pow(2, it->second.length() - i-1);
        //  cout << it->first << "|" << value;
        }
        //cout << "twos\t";
      }
      mem_value.insert(pair<int,int>(it->first, value));
    }
    // Determine if normal representation
    else if(it->second.front() == '0'){
      value = 0;
      // For each bit in the data value, check if it is 1 to add to the sum of the value. Start from right most end and work towards left.
      for(int i = it->second.length()-1; i > 0; i--){
        if(it->second.at(i) == '1'){
          // Make the value equal to previous value + 2^(total length - current length). I.e first value will be 2^(32-31-1) = 2^(0) = 1.
          value = value + pow(2, it->second.length() - i-1);
        }
      }
      mem_value.insert(pair<int, int>(it->first, value));
      //cout << "normal\t";
    }
    //cout << it->second << "\t" << it->first << endl;
  }

  return mem_value;
}

// Prints out the memory data map
void printDataValues(map<int,int> mem_value){
  cout << "\n\nMemory \t\t" << "Numerical Value" << endl;
  for(map<int,int>::iterator it = mem_value.begin(); it != mem_value.end(); it++){
    cout << it->first << "\t\t\t" << it->second << endl;
  }
}

// Prints out the instruction disassembly map
void printinstruction_disassembly(map<string, vector<string>> instruction_disassembly){
  int xx = 0;
  for(map<string ,vector<string>>::iterator it = instruction_disassembly.begin(); it != instruction_disassembly.end(); it++){
    // printed value to make sure that on second iteration through vector list to print instruction, the memory address is not printed as well. Refreshes to false after each row
    bool printed = false;
    for(vector<string>::iterator itr = it->second.begin(); itr != it->second.end(); itr++){
      cout << *itr << "\t";
      if(printed == false){
        cout << it->first << "\t";
        printed = true;
      }
    }
    cout << "\n";
  }
}

// Effectively same as print instruction simulation but it just writes it to text file instead.
void write_Disassembly(map<string, vector<string>> instruction_disassembly, map<int,int> mem_value, map<int,string> mem_data){
  ofstream disassembly("disassembly.txt");
  for(map<string ,vector<string>>::iterator it = instruction_disassembly.begin(); it != instruction_disassembly.end(); it++){
    // printed value to make sure that on second iteration through vector list to print instruction, the memory address is not printed as well. Refreshes to false after each row
    bool printed = false;
    for(vector<string>::iterator itr = it->second.begin(); itr != it->second.end(); itr++){
      disassembly << *itr << "\t";
      if(printed == false){
        disassembly << it->first << "\t";
        printed = true;
      }
    }
    disassembly << "\n";
  }
  // If the memory in mem-value is equal to the memory in mem-instructionbinary then write to disassembly the format: BinaryInstruction | MemoryAddr | Integer value
  for(map<int,int>::iterator it = mem_value.begin(); it != mem_value.end(); it++){
    for(map<int, string>::iterator itr = mem_data.begin(); itr != mem_data.end(); itr++){
      if(it->first == itr->first){
        disassembly << itr->second << "\t" << to_string(it->first) << "\t" << it->second << "\n";
      }
    }
  }
}

/*
void write_Simulation(map<string, vector<string>> instruction_simulation, map<int, string> mem_instruction){
  ofstream simulation("simulation.txt");
  int cycle = 0;
  for(map<int,string>::iterator it = mem_instruction.begin(); it != mem_instruction.end(); it++){
    cout << "--------------------\n";
    cout << "Cycle: " << cycle << "\t" << it->first;
  }
}
*/

// References instruction_disassembly to add instruction to it. Takes in mem_instruction iterator and instruction for key / values
void addto_instruction_disassembly(map<string, vector<string>> &instruction_disassembly, map<int,string>::iterator &it, string instruction){
  // Create new map for the simulation output <stringified memory address | instruction binary -> instructions -> >
  instruction_disassembly.insert(pair<string, vector<string>>(to_string(it->first), vector<string>()));
  instruction_disassembly[to_string(it->first)].push_back(it->second);
  instruction_disassembly[to_string(it->first)].push_back(instruction);
}

void print_DataReg(map<int,int> mem_value){
  cout << "Data";
  int dataCounter = 0;
  for(map<int, int>::iterator it = mem_value.begin(); it != mem_value.end(); it++){
    if(dataCounter == 0){
      cout << "\n" << it->first << ":\t" << it->second << "\t";
      dataCounter = 8;
    }
    else
      cout << it->second << "\t";
    dataCounter--;
  }
  cout << "\n";
}

// Populate the register values with 0.
void create_registers(map<int, string> &register_values){
  for(int i = 0; i <= 31; i++){
    register_values.insert(pair<int, string>(i, "0"));
  }
}


int main(int args, char **argv){

  // #argv[1] should contain the input file
  fstream file;
  // Open file and set direction to be input
  file.open(argv[1], ios::in);
  int linenum = 0;
  map<int, string> mem_instruction;
  map<int, string> mem_data;
  map<int, int> mem_value;
  // map<memoryaddr, instruction>
  map<string, vector<string>> instruction_disassembly;
  map<int, string> register_values;
  map<int, vector<string>> cycle_instructions;

  // Base address to start at
  int address = 256;
  // String used to get the category categorybits
  string categorybits;
  // String used to get the opcode after categorybits
  string opcode;
  bool breakhit = false;
  string instruction = "";
  // Initialize cycle to start at 1
  int cycle = 1;

  if(file.good()){
    while(!file.eof()){
      string line;

      getline(file, line);
      if(linenum == 0){
        cout << "Input line length: " << line.length() << '\n' << endl;
      }
      // If there is something on the line (not empty)
      if(line != ""){
        categorybits = line.substr(0,2);
        opcode = line.substr(2,4);

        if(breakhit == false)
          mem_instruction.insert(pair<int, string>(address, line));

        if(breakhit == true)
          mem_data.insert(pair<int,string>(address, line));

        if(categorybits == "01" && opcode == "0101")
          breakhit = true;

      }

      address = address + 4;
      linenum++;
    }
  }
  else
    return 0;

  // Store the data values into the memory->values map
  mem_value = calculateDataValues(mem_data);
  create_registers(register_values);

  //cout << "MIPS Code \t\t\t" << "  Category \t" << "Opcode \t" << "Memory address" << endl;
  cout << "MIPS Code \t\t\t" << "  Memory \t" <<  "Instruction" << endl;
  int iteration = 1;
  while(iteration <= 2){
    int cycle = 1;
    for(map<int,string>::iterator it = mem_instruction.begin(); it != mem_instruction.end(); it++){
      // Makes a substring out of the line to find out if the instruction is in category 1 or category 2
      // Start at position 0 - 2 length
      categorybits = it->second.substr(0,2);
      // Start at position 2 - 4 length
      opcode = it->second.substr(2,4);
      instruction = "";
      int rsReg = 0;
      int rtReg = 0;
      int rdReg = 0;
      int immediate = 0;
      int sa = 0; // shift amount
      int shftedRegAmt = 0; // how much to shift register by
      int offset = 0;
      int base = 0; // for LW? SW
      int x = 0;
      int y = 0;
      int z = 0;
      //cout << it->first << "\t" << it->second << " ::\n";
      if(categorybits == "01"){
        // J Instruction. Shifted right 2 on the instruction bits so shifted left twice to account for that.
        if(opcode == "0000"){
          int jumpaddr = 0;
          //cout << "Jumping to memory: ";
          for(int i = it->second.length() - 1; i > 5; i--){
            if(it->second.at(i) == '1')
              jumpaddr = jumpaddr + pow(2, it->second.length() - i - 1);
          }
          jumpaddr = jumpaddr << 2;
          //cout << jumpaddr << "\t";
          // Make string and append the jump address to it so i can store it for outputting
          instruction = "J #";
          instruction += to_string(jumpaddr);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            //cout << "heree\n";
            cycle_instructions.insert(pair<int, vector<string>>(cycle, vector<string>()));
            cycle_instructions[cycle].push_back(to_string(it->first));
            cycle_instructions[cycle].push_back(instruction);
            // finds the iterator that has the jump addr as the key.
            it = mem_instruction.find(jumpaddr);
            //cout << "it: " << it->first << "\t" << it->second << "\n";
            cycle++;

          }
        }
        // JR Instruction
        if(opcode == "0001"){

          for(int i = it->second.length() - 22; i >= 6; i--){
            if(it->second.at(i) == '1')
              rsReg = rsReg + pow(2, it->second.length() - 22 - i);
          }
          instruction = "JR R";
          instruction += to_string(rsReg);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
              if(regItr->first == rsReg){
                // Finds the register value stored at the register and stores it to x
                x = stoi(register_values.at(rsReg));
              }
            }
            cycle_instructions.insert(pair<int, vector<string>>(cycle, vector<string>()));
            cycle_instructions[cycle].push_back(to_string(it->first));
            cycle_instructions[cycle].push_back(instruction);
            // make the iterator start where x's value (memory?) is
            it = mem_instruction.find(x);
            cycle++;
          }
        }
        // BEQ Instruction
        if(opcode == "0010"){
          int offset = 0;
          // Offset bits are from 31-16
          for(int i = it->second.length()-1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              offset = offset + pow(2,it->second.length() - i - 1);
            }
          }
          offset = offset << 2;
          // RT Register are from bits 15-11
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }
          // RS Register are from bits 10-6
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          // Combine everything into string for adding to instruction simulation
          instruction = "BEQ R" + to_string(rsReg) + ", R" + to_string(rtReg) + ", #" + to_string(offset);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            //cout << "reg1. :" << register_values.find(rsReg) -> second << "\n";
            //cout << "reg2. :" << register_values.find(rtReg) -> second << "\n";

            cycle_instructions.insert(pair<int, vector<string>>(cycle, vector<string>()));
            cycle_instructions[cycle].push_back(to_string(it->first));
            cycle_instructions[cycle].push_back(instruction);
            if(register_values.find(rsReg)->second == register_values.find(rtReg)->second){
              //cout << "MEMORY ADDRESS:" << (it->first + offset + 4) << "\n";
              it = mem_instruction.find(it->first + offset + 4);
            }
            cycle++;
          }
        }
        // BLTZ Instruction
        if(opcode == "0011"){
          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              offset = offset + pow(2, it->second.length() - 1 - i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }
          // offset is shifted left 2 bits
          offset = offset << 2;
          instruction = "BLTZ R" + to_string(rsReg) + ", #" + to_string(offset);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // BGTZ Instruction
        if(opcode == "0100"){
          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              offset = offset + pow(2, it->second.length() - 1 - i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }
          // offset is shifted left 2 bits
          offset = offset << 2;
          instruction = "BGTZ R" + to_string(rsReg) + ", #" + to_string(offset);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // BREAK Instruction **functionality other than denoting data is next??
        if(opcode == "0101"){
          instruction = "BREAK";
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // SW Instruction
        if(opcode == "0110"){
          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              offset = offset + pow(2, it->second.length() - i - 1);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15-i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              base = base + pow(2, 10 - i);
            }
          }
          // For the actual word to load, take the offset 340 to base R0 and every increment of 4 is the next data register.
          instruction = "SW R" + to_string(rtReg) + ", " + to_string(offset) + "(R" + to_string(base) + ")";
          addto_instruction_disassembly(instruction_disassembly, it, instruction);

        }
        // LW Instruction
        if(opcode == "0111"){

          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              offset = offset + pow(2, it->second.length() - i - 1);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15-i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              base = base + pow(2, 10 - i);
            }
          }
          // For the actual word to load, take the offset 340 to base R0 and every increment of 4 is the next data register.
          instruction = "LW R" + to_string(rtReg) + ", " + to_string(offset) + "(R" + to_string(base) + ")";
          addto_instruction_disassembly(instruction_disassembly, it, instruction);

        }
        // SLL Instruction  //31-26
        if(opcode == "1000"){
          for(int i = it->second.length() - 7; i >= 21; i--){
            if(it->second.at(i) == '1')
              sa = sa + pow(2, 25 - i);
          }
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1')
              rdReg = rdReg + pow(2, 20 - i);
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1')
              rtReg = rtReg + pow(2, 15 - i);
          }
          // Keep the new value of rdReg in another variable
          shftedRegAmt = rtReg;
          // For each shift amount, shift left bits 1.
          for(int i = sa; i > 0; i--){
            shftedRegAmt = shftedRegAmt << 1;
          }
          instruction = "SLL R" + to_string(rdReg) + ", R" + to_string(rtReg) + ", #" + to_string(sa);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);

        }
        // SRL Instruction
        if(opcode == "1001"){
          for(int i = it->second.length() - 7; i >= 21; i--){
            if(it->second.at(i) == '1')
              sa = sa + pow(2, 25 - i);
          }
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1')
              rdReg = rdReg + pow(2, 20 - i);
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1')
              rtReg = rtReg + pow(2, 15 - i);
          }
          // Keep the new value of rdReg in another variable
          shftedRegAmt = rtReg;
          // For each shift amount, shift left bits 1.
          for(int i = sa; i > 0; i--){
            shftedRegAmt = shftedRegAmt << 1;
          }
          instruction = "SRL R" + to_string(rdReg) + ", R" + to_string(rtReg) + ", #" + to_string(sa);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // SRA Instruction
        if(opcode == "1010"){

          for(int i = it->second.length() - 7; i >= 21; i--){
            if(it->second.at(i) == '1')
              sa = sa + pow(2, 25 - i);
          }
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1')
              rdReg = rdReg + pow(2, 20 - i);
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1')
              rtReg = rtReg + pow(2, 15 - i);
          }
          // Keep the new value of rdReg in another variable
          shftedRegAmt = rtReg;
          // For each shift amount, shift left bits 1.
          for(int i = sa; i > 0; i--){
            shftedRegAmt = shftedRegAmt << 1;
          }
          instruction = "SRA R" + to_string(rdReg) + ", R" + to_string(rtReg) + ", #" + to_string(sa);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // NOP Instruction
        if(opcode == "1011"){
          instruction = "NOP";
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
      }
      else if(categorybits == "11"){
        // Use struct and create function to reduce redundancy if there is extra time

        // ADD Instruction
        if(opcode == "0000"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }
          instruction = "ADD R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
              if(regItr->first == rdReg){
                x = stoi(register_values.at(rsReg));
                y = stoi(register_values.at(rtReg));
                // result of the two registers
                z = x + y;

                register_values.at(rdReg) = to_string(z);
              }
            }
            cycle_instructions.insert(pair<int, vector<string>>(cycle, vector<string>()));
            cycle_instructions[cycle].push_back(to_string(it->first));
            cycle_instructions[cycle].push_back(instruction);
            cycle++;

          }
        }
        // SUB Instruction
        if(opcode == "0001"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "SUB R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // MUL Instruction
        if(opcode == "0010"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "MUL R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // AND Instruction
        if(opcode == "0011"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "AND R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // OR Instruction
        if(opcode == "0100"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "OR R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // XOR Instruction
        if(opcode == "0101"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "XOR R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // NOR Instruction
        if(opcode == "0110"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "NOR R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // SLT Instruction
        if(opcode == "0111"){
          for(int i = 20; i >= 16; i--){
            if(it->second.at(i) == '1'){
              rdReg = rdReg + pow(2, 20 - i);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "SLT R" + to_string(rdReg) + ", R" + to_string(rsReg) + ", R" + to_string(rtReg);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // ADDI Instruction
        if(opcode == "1000"){
          // Calculate the immediate value (16 bits)
          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              immediate = immediate + pow(2, it->second.length() - i - 1);
            }
          }
          // Calculate the rt register (5 bits)
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15 - i);
            }
          }

          // Calculate the rs register (5 bits)
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "ADDI R" + to_string(rtReg) + ", R" + to_string(rsReg) + ", #" + to_string(immediate);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);

        }
        // ANDI Instruction
        if(opcode == "1001"){
          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              immediate = immediate + pow(2, it->second.length() - i - 1);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15-i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "ANDI R" + to_string(rtReg) + ", R" + to_string(rsReg) + ", #" + to_string(immediate);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // ORI Instruction
        if(opcode == "1010"){

          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              immediate = immediate + pow(2, it->second.length() - i - 1);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15-i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "ORI R" + to_string(rtReg) + ", R" + to_string(rsReg) + ", #" + to_string(immediate);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
        // XORI Instruction
        if(opcode == "1011"){

          for(int i = it->second.length() - 1; i >= 16; i--){
            if(it->second.at(i) == '1'){
              immediate = immediate + pow(2, it->second.length() - i - 1);
            }
          }
          for(int i = 15; i >= 11; i--){
            if(it->second.at(i) == '1'){
              rtReg = rtReg + pow(2, 15-i);
            }
          }
          for(int i = 10; i >= 6; i--){
            if(it->second.at(i) == '1'){
              rsReg = rsReg + pow(2, 10 - i);
            }
          }

          instruction = "XORI R" + to_string(rtReg) + ", R" + to_string(rsReg) + ", #" + to_string(immediate);
          addto_instruction_disassembly(instruction_disassembly, it, instruction);
        }
      }

      //cout << it->second << "\t" << categorybits << "\t" << opcode << "\t" << it->first << endl;
      cycle++;
    }
    // The iterations are first iteration for the disassembly to be finished and then second iteration for actual register work
    iteration++;
  }



  printinstruction_disassembly(instruction_disassembly);
  printDataValues(mem_value);
  write_Disassembly(instruction_disassembly, mem_value, mem_data);
  //print_DataReg(mem_value);

  //register_values.insert(pair<int, string>(0, "10"));
  //register_values.at(0) = "fifty";    // This works as valid way of inserting register values
  cout << cycle_instructions.size();
  int regNum = 0;
  for(map<int, vector<string>>::iterator cycleItr = cycle_instructions.begin(); cycleItr != cycle_instructions.end(); cycleItr++){
    regNum = 0;
    cout << "-------\n";
    cout << "Cycle: " << cycle << "\t" << cycleItr->first << "\t" << cycleItr->second.at(0) << "\t" << cycleItr->second.at(1) << "\n\n";
    cout << "Registers";
    for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
      // For printing out the register row names
      if(regNum % 8 == 0){
        if(regNum == 0)
          cout << "\nR00:\t";
        else if(regNum == 8)
          cout << "\nR08:\t";
        else
          cout << "\nR" << regNum << ":\t";
      }
      cout << regItr->second << "\t";
      regNum++;
    }
    cout << "\n";
    cycle++;
    regNum++;
    cout << "\n";
    print_DataReg(mem_value);
  }

  //write_Simulation(instruction_simulation);
  //cout << "\nData Values\t\t\t\t\t" << "Memory address \t" << endl;


  return 0;
}
