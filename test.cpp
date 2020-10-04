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
void printDataValues(map<int,int> mem_value);

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
void print_Disassembly(map<string, vector<string>> instruction_disassembly){
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

void print_Simulation(map<int,int> mem_value, int regNum, int cycle, map<int,string> register_values, map<string, vector<string>> instruction_disassembly, map<int,string>::iterator it, string instruction){
  cout << "-------\n";
  // Prints each cycle with format |cycle # | memory address | instruction in english | .......... last part got from instruction dissasembly finding the 2nd vector element at that memory address
  cout << "Cycle: " << cycle << "\t" << it->first << "\t" << instruction_disassembly.find(to_string(it->first))->second.at(1) << "\n\n";
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

// Have to pass in the output file to allow changes every iteration as opposed to writing all at once like the disassembly
void write_Simulation(ofstream &simulation, map<int,int> mem_value, int regNum, int cycle, map<int,string> register_values, map<string, vector<string>> instruction_disassembly, map<int,string>::iterator it, string instruction){
  simulation << "-------\n";
  // Prints each cycle with format |cycle # | memory address | instruction in english | .......... last part got from instruction dissasembly finding the 2nd vector element at that memory address
  simulation << "Cycle: " << cycle << "\t" << it->first << "\t" << instruction_disassembly.find(to_string(it->first))->second.at(1) << "\n\n";
  simulation << "Registers";
  for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
    // For printing out the register row names
    if(regNum % 8 == 0){
      if(regNum == 0)
        simulation << "\nR00:\t";
      else if(regNum == 8)
        simulation << "\nR08:\t";
      else
        simulation << "\nR" << regNum << ":\t";
    }
    simulation << regItr->second << "\t";
    regNum++;
  }
  simulation << "\n";
  simulation << "Data";
  int dataCounter = 0;
  for(map<int, int>::iterator it = mem_value.begin(); it != mem_value.end(); it++){
    if(dataCounter == 0){
      simulation << "\n" << it->first << ":\t" << it->second << "\t";
      dataCounter = 8;
    }
    else
      simulation << it->second << "\t";
    dataCounter--;
  }
  simulation << "\n";
}
// References instruction_disassembly to add instruction to it. Takes in mem_instruction iterator and instruction for key / values
void addto_instruction_disassembly(map<string, vector<string>> &instruction_disassembly, map<int,string>::iterator it, string instruction){
  // Create new map for the simulation output <stringified memory address | instruction binary -> instructions -> >
  instruction_disassembly.insert(pair<string, vector<string>>(to_string(it->first), vector<string>()));
  instruction_disassembly[to_string(it->first)].push_back(it->second);
  instruction_disassembly[to_string(it->first)].push_back(instruction);
}

void addto_cycles_simulation(map<int, vector<string>> &cycle_instructions, int cycle, map<int, string>::iterator it, string instruction){
  cycle_instructions.insert(pair<int, vector<string>>(cycle, vector<string>()));
  cycle_instructions[cycle].push_back(to_string(it->first));
  cycle_instructions[cycle].push_back(instruction);
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
  // open up output channel
  ofstream simulation("simulation.txt");
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
    int cycle = 0;
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
      map<int, string>::iterator jumper;

      //cout << "\n" << it->first << "\t" << it->second << " ::";
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
            //  cout << "jumpaddr:" << jumpaddr;
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            // finds the iterator that has the jump addr as the key.
            jumper = mem_instruction.find(jumpaddr-4);
            //cout << "it: " << it->first << "\t" << it->second << "\n";

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
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
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
            //cout << "rs:" << rsReg << ", " << stoi(register_values.find(rsReg)->second) << "\n";
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            if(register_values.find(rsReg)->second == register_values.find(rtReg)->second){
              //cout << "MEMORY ADDRESS:" << (it->first + offset + 4) << "\n";
              jumper = mem_instruction.find(it->first + offset);
            }
            else{
              jumper = it;
            }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            //cout << "rs:" << rsReg << ", " << stoi(register_values.find(rsReg)->second) << "\n";
            if(stoi(register_values.find(rsReg)->second) < 0){
              jumper = mem_instruction.find((it->first + offset));
            }
            else{
              jumper = it;
            }
          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            //cout << "rs:" << rsReg << ", " << stoi(register_values.find(rsReg)->second) << "\n";
            if(stoi(register_values.find(rsReg)->second) > 0){
              jumper = mem_instruction.find((it->first + offset));
            }
            else{
              jumper = it;
            }
          }
        }
        // BREAK Instruction **functionality other than denoting data is next??
        if(opcode == "0101"){
          instruction = "BREAK";
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            cycle++;
          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            int addr = (offset + stoi(register_values.find(base)->second));
            for(map<int,int>::iterator dataItr = mem_value.begin(); dataItr != mem_value.end(); dataItr++){
              if(addr == dataItr->first){
                //cout << "addr:" << addr;
                //cout << "value: " << register_values.find(rtReg)->second;
                mem_value.at(addr) = stoi(register_values.find(rtReg)->second);
              }
            }
            cycle++;
          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            int addr = (offset + stoi(register_values.find(base)->second));
            int memoryval = mem_value.find(addr)->second;
            //cout << "addr:" << addr << " |memval:" << memoryval;
            register_values.erase(rtReg);
            register_values.insert(pair<int,string>(rtReg, to_string(memoryval)));
          }
          cycle++;
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
          shftedRegAmt = stoi(register_values.find(rtReg)->second);
          // For each shift amount, shift left bits 1.
          for(int i = sa; i > 0; i--){
            shftedRegAmt = shftedRegAmt << 1;
          }
          instruction = "SLL R" + to_string(rdReg) + ", R" + to_string(rtReg) + ", #" + to_string(sa);
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            // Remove current register value and replace with the new one
            register_values.erase(rdReg);
            register_values.insert(pair<int, string>(rdReg, to_string(shftedRegAmt)));
            // cout << "rdreg:" << rdReg << "|amt: " << to_string(shftedRegAmt);
            cycle++;
          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            // Remove current register value and replace with the new one
            if(register_values.count(rdReg) == 1)
              register_values.erase(rdReg);
            register_values.insert(pair<int, string>(rdReg, to_string(shftedRegAmt)));
            cycle++;
          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            // Remove current register value and replace with the new one
            if(register_values.count(rdReg) == 1)
              register_values.erase(rdReg);
            register_values.insert(pair<int, string>(rdReg, to_string(shftedRegAmt)));
            cycle++;
          }
         }
        // NOP Instruction
        if(opcode == "1011"){
          instruction = "NOP";
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            cycle++;
          }
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
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
              if(regItr->first == rdReg){
                x = stoi(register_values.at(rsReg));
                y = stoi(register_values.at(rtReg));
                // result of the two registers
                z = x - y;

                register_values.at(rdReg) = to_string(z);
              }
            }
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            cycle++;

          }
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
          if(iteration == 1)
            addto_instruction_disassembly(instruction_disassembly, it, instruction);
          else if(iteration == 2){
            for(map<int,string>::iterator regItr = register_values.begin(); regItr != register_values.end(); regItr++){
              if(regItr->first == rdReg){
                x = stoi(register_values.at(rsReg));
                y = stoi(register_values.at(rtReg));
                // cout << "x:" << x << "  | y:" << y;
                // result of the two registers
                z = x * y;

                register_values.at(rdReg) = to_string(z);
              }
            }
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            cycle++;

          }
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
          else if(iteration == 2){
            addto_cycles_simulation(cycle_instructions, cycle, it, instruction);
            // Remove current register value and replace with the new one
            x = stoi(register_values.find(rsReg)->second);
            z = x + immediate;
            register_values.at(rtReg) = to_string(z);
            cycle++;
          }
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
      if(iteration == 2){
        int regNum = 0;
        // If it's a jump, I have to increase the cycle number so that it'll print out its jump state before changing the iterator to the one right before current iterator??
        if( (opcode == "0000" || opcode == "0010" || opcode == "0011" || opcode == "0100") && (categorybits == "01") ){
          cycle++;
        }
        //print_Simulation(mem_value, regNum, cycle, register_values, instruction_disassembly, it, instruction);
        write_Simulation(simulation, mem_value, regNum, cycle, register_values, instruction_disassembly, it, instruction);
        // If jump, set the current iterator to be equal to the instruction right before where we want so that when it loops around it'll be the actual one we want
        if( (opcode == "0000" || opcode == "0010" || opcode == "0011" || opcode == "0100") && (categorybits == "01") ){
          it = jumper;
        }

      }
      //cout << it->second << "\t" << categorybits << "\t" << opcode << "\t" << it->first << endl;
    }
    // The iterations are first iteration for the disassembly to be finished and then second iteration for actual register work
    iteration++;
  }

  //register_values.insert(pair<int, string>(0, "10"));
  //register_values.at(0) = "fifty";    // This works as valid way of inserting register values

  // Print out number of cycles
  //cout << cycle_instructions.size();

  write_Disassembly(instruction_disassembly, mem_value, mem_data);
  // print_Disassembly(instruction_disassembly);
  // printDataValues(mem_value);
  // print_DataReg(mem_value);

  return 0;
}
