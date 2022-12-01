#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "pass1.h"

using namespace std;

template<class T>
struct DisableCompare : public binary_function<T, T, bool>{
    bool operator()(T lhs, T rhs) const{
        if(lhs == rhs) return false;
        return true;
    }
};

int main(){
    vector<vector<string> > code(1000);
    vector<int> location;
    map<string,string> symbolTableMap;
    vector<pair<string,string> > symbolTable;

    int lines = readFile(code);
    // int address = getStartAddress(code);
    addressCounter(code,location,lines-1);
    createSymbolTable(code,location,symbolTable, symbolTableMap, lines-1);
    printSymbolTable(symbolTable);
    // for(int i=0;i<location.size();i++){
    //     cout<<std::setfill('0')<<std::setw(4)<<intToHexString(location[i])<<endl;
        
    // }
    // cout<<address<<endl;

    return 0;
}