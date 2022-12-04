#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "pass1.h"
#include "pass2.h"
#include "optable.h"
#include "hte.h"

using namespace std;

int main(){
    vector<vector<string> > code(1000);
    vector<int> location;
    vector<int> locationForText;
    map<string,string> symbolTableMap;
    vector<pair<string,string> > symbolTable;
    map<string,string> opTable;
    vector<int> objectCodeInt;
    vector<string>objectCode;
    vector<string>hteRecord;

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
    createHead(code,location,hteRecord);
    createText(code,locationForText,hteRecord,objectCode,lines-1);
    createModification(locationForText,hteRecord,code);
    createEnd(location,hteRecord);
    generateHTEFile(hteRecord);
    // cout<<"size "<<objectCodeInt.size()<<endl;
    // for(int i = 0; i < objectCodeInt.size(); i++){
    //     cout<<"i = "<<i<<" "<<intToHexString(objectCodeInt[i])<<" "<<objectCodeInt[i]<<endl;
    // }
    cout<<"-------------------------------"<<endl;
    for(int i=0;i<location.size();i++){
        cout<<i<<" "<<std::setfill('0')<<std::setw(4)<<intToHexString(location[i])<<" "<<location[i]<<endl;
    }
    cout<<"-------------------------------"<<endl;
    for(int i = 0; i < objectCode.size(); i++){
        cout<<objectCode[i]<<endl;
    }
    cout<<"-------------------------------"<<endl;
    for(int i = 0; i < locationForText.size(); i++){
        cout<<i<<" "<<std::setfill('0')<<std::setw(4)<<intToHexString(locationForText[i])<<" "<<locationForText[i]<<endl;
    }
    // cout<<address<<endl;

    return 0;
}