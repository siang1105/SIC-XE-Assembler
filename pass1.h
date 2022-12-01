#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "utilities.h"

using namespace std;

int readFile(vector<vector<string> > &code){

    ifstream inFile("test.txt");
    string temp;
    string tempNoSpace = "";
    int len = 0;
    while (getline(inFile, temp)){
        for(int j = 0; j < temp.size(); j++){
            if(temp[j] != ' ' && temp[j] != '\t' && temp[j] != '\n' && temp[j] != '\0'){
                tempNoSpace += temp[j];
            }
            else{
                code[len].push_back(tempNoSpace);
                tempNoSpace.clear();
            }
        }
        code[len++].push_back(tempNoSpace);
        tempNoSpace.clear();
    }
    return len;
}

int getStartAddress(vector<vector<string> > code){
    return hexToDec(code[0][2]);
}

void addressCounter(vector<vector<string> > code, vector<int> &location, int lines){
    location.push_back(getStartAddress(code));
    int address = getStartAddress(code);

    for(int i = 1; i < lines; i++){
        if(code[i].size() == 1){
            if(code[i][0][0]== '+') address += 4;
            else address += 3;
        } 
        else if(code[i].size() == 2){
            if(code[i][0][0] == '+') address += 4;
            else if(code[i][0] == "ADDR" || code[i][0] == "CLEAR" || code[i][0] == "COMPR" || code[i][0] == "DIVR" || code[i][0] == "MULR" || code[i][0] == "SUBR" || code[i][0] == "TIXR") address += 2;
            else if(code[i][0] == "BASE"){
                location.push_back(address);
                continue;
            }
            else address += 3;
        }
        else if(code[i].size() == 3){
            if(code[i][1] == "RESW") address += stringToDec(code[i][2]) * 3;
            else if(code[i][1] == "RESB") address += stringToDec(code[i][2]);
            else if(code[i][1] == "WORD") address += 3;
            else if(code[i][1] == "BYTE"){
                if(code[i][2][0]=='X' || code[i][2][0]=='x'){
                    for(int j = 0; j < (int)ceil( (double)(code[i][2].substr(2,code[i][2].size() - 3).size() ) / 2.0 ); j++){
                        address += 1;
                        location.push_back(address);
                    }
                    continue;
                }
                else if(code[i][2][0]=='C' || code[i][2][0]=='c'){
                    for(int j = 0; j < code[i][2].substr(2, code[i][2].size() - 3).size(); j++){
                        address += 1;
                        location.push_back(address);
                    }
                    continue;
                }
            }
            else if(code[i][1][0] == '+') address += 4;
            else if(code[i][1] == "ADDR" || code[i][1] == "CLEAR" || code[i][1] == "COMPR" || code[i][1] == "DIVR" || code[i][1] == "MULR" || code[i][1] == "SUBR" || code[i][1] == "TIXR") address+=2;
            else if(code[i][1]=="EQU"){
                location.push_back(address);
                continue;
            }
            else address+=3;
        }
        location.push_back(address);
    }
    return;
}

void createSymbolTable(vector<vector<string> > code, vector<int> location, vector<pair<string,string> > &symbolTable, map<string,string> &symbolTableMap, int lines){
    int j = 0;
    for(int i = 1; i < lines; i++){
        if(code[i].size() == 3){
            symbolTableMap.insert(pair<string, string>(code[i][0],intToHexString(location[j])));
            symbolTable.push_back(pair<string, string>(code[i][0],intToHexString(location[j])));
            if((code[i][2][0] == 'X' || code[i][2][0] == 'x')&&(code[i][2] != "X")){
                j += (int)ceil( (double)(code[i][2].substr(2, code[i][2].size() - 3).size() ) / 2.0) - 1;
            }
            else if((code[i][2][0]=='C' || code[i][2][0]=='c')&&(code[i][2]!="C")){
                j += code[i][2].substr(2, code[i][2].size() - 3).size() - 1;
            }
        }
        j++;
    }
    // for( map<string,string>::const_iterator iter = symbolTable.begin(); iter != symbolTable.end(); ++iter ){
    //     string key = it -> first;
    //     string value = it -> second;

    // }
}

void printSymbolTable(vector<pair<string,string> > symbolTable){
    ofstream file;
    file.open("symboltable.txt");
    // for( map<string,string>::const_iterator iter = symbolTable.begin(); iter != symbolTable.end(); ++iter ){
    //     string key = iter -> first;
    //     string value = iter -> second;
    //     file<<key<<"\t"<<hex<< setfill('0') << setw(4) << value<<endl;
    // }
    for(int i = 0; i < symbolTable.size(); i++){
        string key = symbolTable[i].first;
        string value = symbolTable[i].second;
        file<<key<<"\t"<<hex<< setfill('0') << setw(4) << value<<endl;
    }
    file.close();
    return;
}
