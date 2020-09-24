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
  cout << "\n\nMemory address \t" << "Numerical Value" << endl;
  for(map<int,int>::iterator it = mem_value.begin(); it != mem_value.end(); it++){
    cout << it->first << "\t\t\t" << it->second << endl;
  }
}

// Prints out the instruction simulation map
void printInstruction_Simulation(map<string, vector<string>> instruction_simulation){
  for(map<string ,vector<string>>::iterator it = instruction_simulation.begin(); it != instruction_simulation.end(); it++){
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
  map<string, vector<string>> instruction_simulation;

  // Base address to start at
  int address = 256;
  // String used to get the category categorybits
  string categorybits;
  // String used to get the opcode after categorybits
  string opcode;
  bool breakhit = false;

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
    cout << "Error with file" << endl;

  // Store the data values into the memory->values map
  mem_value = calculateDataValues(mem_data);

  //cout << "MIPS Code \t\t\t" << "  Category \t" << "Opcode \t" << "Memory address" << endl;
  cout << "MIPS Code \t\t\t" << "  Memory \t" <<  "Instruction" << endl;
  for(map<int,string>::iterator it = mem_instruction.begin(); it != mem_instruction.end(); it++){
    // Makes a substring out of the line to find out if the instruction is in category 1 or category 2
    categorybits = it->second.substr(0,2);
    opcode = it->second.substr(2,4);

    if(categorybits == "01"){
      // J Instruction. Shifted right 2 on the instruction bits so shifted left twice to account for that.
      if(opcode == "0000"){
        int jumpaddr = 0;
        string instruction = "";
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

        // Create new map for the simulation output <instruction binary | stringified memory address -> instructions -> >
        instruction_simulation.insert(pair<string, vector<string>>(to_string(it->first), vector<string>()));
        instruction_simulation[to_string(it->first)].push_back(it->second);
        instruction_simulation[to_string(it->first)].push_back(instruction);
      }
      // JR Instruction
      if(opcode == "0001"){

      }
      // BEQ Instruction
      if(opcode == "0010"){

      }
      // BLTZ Instruction
      if(opcode == "0011"){

      }
      // BGTZ Instruction
      if(opcode == "0100"){

      }
      // BREAK Instruction ** Prolly dont need this here? **
      if(opcode == "0101"){

      }
      // SW Instruction
      if(opcode == "0110"){

      }
      // LW Instruction
      if(opcode == "0111"){

      }
      // SLL Instruction
      if(opcode == "1000"){

      }
      // SRL Instruction
      if(opcode == "1001"){

      }
      // SRA Instruction
      if(opcode == "1010"){

      }
      // NOP Instruction
      if(opcode == "1011"){

      }
    }
    else if(categorybits == "11"){
      // ADD Instruction
      if(opcode == "0000"){

      }
      // SUB Instruction
      if(opcode == "0001"){

      }
      // MUL Instruction
      if(opcode == "0010"){

      }
      // AND Instruction
      if(opcode == "0011"){

      }
      // OR Instruction
      if(opcode == "0100"){

      }
      // XOR Instruction
      if(opcode == "0101"){

      }
      // NOR Instruction
      if(opcode == "0110"){

      }
      // SLT Instruction
      if(opcode == "0111"){

      }
      // ADDI Instruction
      if(opcode == "1000"){

      }
      // ANDI Instruction
      if(opcode == "1001"){

      }
      // ORI Instruction
      if(opcode == "1010"){

      }
      // XORI Instruction
      if(opcode == "1011"){

      }
    }

    //cout << it->second << "\t" << categorybits << "\t" << opcode << "\t" << it->first << endl;
  }
  printInstruction_Simulation(instruction_simulation);
  printDataValues(mem_value);
  //cout << "\nData Values\t\t\t\t\t" << "Memory address \t" << endl;


  return 0;
}
