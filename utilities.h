#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <sstream>
using namespace std;

int hexToDec(string str){
    int address;
    stringstream stream;
    stream << str;
    stream >> hex >> address;

    return address;
}

int stringToDec(string str){
    int output;
    stringstream stream;
    stream << str;
    stream >> output;
    return output;
}

string intToHexString(int input){
    stringstream ss;
    ss<<hex<<input;
    return ss.str();
}

int hexStringToDec(string str){
    int output;
    stringstream stream;
    stream <<hex<<str;
    stream >> output;
    return output;
}

int handleFormat4(vector<string> line, string instruction ,map<string,string>opTable){
    string cleanedInstruction = instruction.substr(1,instruction.size() - 1);
    int opcode = hexStringToDec(opTable.find(cleanedInstruction) -> second);
    opcode &= 252;
    if(line.size() == 2){
        if(line[1][0] == '@') opcode |= 2;
        else if(line[1][0] == '#'){
            opcode |= 1;
            opcode = opcode << 4;
            opcode |= 1;
            return opcode;
        }
        else opcode |= 3;
    }
    else if(line.size() == 3){
        if(line[2][0] == '@') opcode |= 2;
        else if(line[2][0] == '#'){
            opcode |= 1;
            opcode = opcode << 4;
            opcode |= 1;
            return opcode;
        }
        else opcode|=3;
    }

    opcode = opcode << 4;

    if(line.size() == 2){
        if(line[1][line[1].size() - 1] == 'x' || line[1][line[1].size() - 1] == 'X') opcode |= 9;
        else opcode |= 1;
    }
    else if(line.size() == 3){
        if(line[2][line[2].size() - 1] == 'x' || line[2][line[2].size() - 1] == 'X') opcode |= 9;
        else opcode |= 1;
    }

    return opcode;

}

int handleFormat3(vector<string> line, string instruction ,map<string,string>opTable){
    
    int opcode = hexStringToDec(opTable.find(instruction) -> second);
    opcode &= 252; //和111111"00"做AND 為了先將n, i設成0 

    if(line.size() == 1){ //RSUB
        opcode |= 3;
        opcode = opcode << 4;
        return opcode;
    }
    else if(line.size() == 2){
        if(line[1][0] == '@') opcode |= 2;//indirect(n = 1, i = 0)
        else if(line[1][0] == '#'){
            opcode |= 1; // immediate (n = 0, i = 1)
            opcode = opcode << 4;//左移四位 x, b, p, e = 0, 0, 0, 0
            return opcode;
        }
        else opcode |= 3;//和11做OR設定n, i (simple n = 1, i = 1)
    }
    else if(line.size() == 3){
        if(line[2][0] == '@') opcode |= 2;
        else if(line[2][0]=='#'){
            opcode |= 1;
            opcode = opcode << 4;
            return opcode;
        }
        else opcode |= 3;
    }

    opcode = opcode << 4; //左移四位
    if(line.size() == 2){
        if(line[1][line[1].size() - 1] == 'x' || line[1][line[1].size() - 1] == 'X') opcode |= 10; // x, b, p, e = 1, 0, 1, 0
        else opcode |= 2;// x, b, p, e = 0, 0, 1, 0
    }
    else if(line.size() == 3){
        if(line[2][line[2].size() - 1] == 'x' || line[2][line[2].size() - 1] == 'X') opcode |= 10;
        else opcode |= 2; //和10做OR 將p設為1
    }
    return opcode;
}

int handleFormat2(vector<string> line, string instruction ,map<string,string>opTable){
    string r1,r2;
    if(line.size() == 2){
        int commaIndex = line[1].find(',');// 找不到回傳-1
        if(line[1].find(',') == std::string::npos){
            r1 = line[1].substr(0, commaIndex);
            r2 = "-1";
        }
        else{
            r1 = line[1].substr(0, commaIndex);
            r2 = line[1].substr(commaIndex + 1, line[1].size() - r1.size() - 1);
        }
    }
    else if(line.size() == 3){
        int commaIndex = line[2].find(',');
        if(line[2].find(',') == std::string::npos){
            r1 = line[2].substr(0, commaIndex);
            r2 = "-1"; 
        }
        else{
            r1 = line[2].substr(0, commaIndex);
            r2 = line[2].substr(commaIndex + 1, line[2].size() - r1.size() - 1);
        }

    }

    int opcode = hexStringToDec(opTable.find(instruction) -> second);

    opcode = opcode << 4;

    if(r1 == "A") opcode |= 0;
    else if(r1  ==  "X") opcode |= 1;
    else if(r1  ==  "L") opcode |= 2;
    else if(r1  ==  "B") opcode |= 3;
    else if(r1  ==  "S") opcode |= 4;
    else if(r1  ==  "T") opcode |= 5;
    else if(r1  ==  "F") opcode |= 6;
    else if(r1  ==  "PC") opcode |= 8;
    else if(r1  ==  "SW") opcode |= 9;

    opcode = opcode<<4;

    if(r2 == "-1") return opcode;
    if(r2 == "A") opcode |= 0;
    else if(r2  ==  "X") opcode |= 1;
    else if(r2  ==  "L") opcode |= 2;
    else if(r2  ==  "B") opcode |= 3;
    else if(r2  ==  "S") opcode |= 4;
    else if(r2  ==  "T") opcode |= 5;
    else if(r2  ==  "F") opcode |= 6;
    else if(r2  ==  "PC") opcode |= 8;
    else if(r2  ==  "SW") opcode |= 9;

    return opcode;

}
#endif