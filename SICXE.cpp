#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "pass1.h"
#include "pass2.h"
#include "optable.h"
#include "objProgram.h"

using namespace std;

int main(){
    vector<vector<string> > code(1005);
    vector<int> location;
    vector<int> locationForText;
    map<string,string> symbolTableMap;
    vector<pair<string,string> > symbolTable;
    map<string,string> opTable;
    vector<int> objectCodeInt;
    vector<string>objectCode;
    vector<string>htmeRecord;

    int lines = readFile(code);
    int baseAddress;
    // int address = getStartAddress(code);
    addressCounter(code,location,lines-1, locationForText);
    createSymbolTable(code,location,symbolTable, symbolTableMap, lines-1);
    printSymbolTable(symbolTable);

    initOpTable(opTable);
    generateOpcode(code,objectCodeInt,opTable,symbolTableMap,baseAddress,lines-1);
    generateAddresses(code,objectCode,objectCodeInt,symbolTableMap,location,baseAddress,lines-1);
    generateObjectCode(objectCode);
    createHead(code,location,htmeRecord);
    createText(code,locationForText,htmeRecord,objectCode,lines-1);
    createModification(locationForText,htmeRecord,code);
    createEnd(location,htmeRecord);
    generateHTMEFile(htmeRecord);
    
    // cout<<"-------------------------------"<<endl;
    // for(int i=0;i<location.size();i++){
    //     cout<<i<<" "<<std::setfill('0')<<std::setw(4)<<intToHexString(location[i])<<" "<<location[i]<<endl;
    // }
    // cout<<"-------------------------------"<<endl;
    // for(int i = 0; i < objectCode.size(); i++){
    //     cout<<objectCode[i]<<endl;
    // }
    // cout<<"-------------------------------"<<endl;
    // for(int i = 0; i < locationForText.size(); i++){
    //     cout<<i<<" "<<std::setfill('0')<<std::setw(4)<<intToHexString(locationForText[i])<<" "<<locationForText[i]<<endl;
    // }

    cout<<"SIX-XE-Assembler success !"<<endl;
    

    return 0;
}