#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

void initOpTable(map<string,string> &opTable){
    opTable.insert(pair<string,string>("ADD","18"));
    opTable.insert(pair<string,string>("ADDR","90"));

    opTable.insert(pair<string,string>("CLEAR","B4"));
    opTable.insert(pair<string,string>("COMP","28"));
    opTable.insert(pair<string,string>("COMPR","A0"));

    opTable.insert(pair<string,string>("DIV","24"));
    opTable.insert(pair<string,string>("DIVR","9C"));

    opTable.insert(pair<string,string>("J","3C"));
    opTable.insert(pair<string,string>("JEQ","30"));
    opTable.insert(pair<string,string>("JLT","38"));
    opTable.insert(pair<string,string>("JSUB","48"));

    opTable.insert(pair<string,string>("LDA","00"));
    opTable.insert(pair<string,string>("LDB","68"));
    opTable.insert(pair<string,string>("LDCH","50"));
    opTable.insert(pair<string,string>("LDT","74"));
    opTable.insert(pair<string,string>("LDX","04"));
    
    opTable.insert(pair<string,string>("MUL","20"));
    opTable.insert(pair<string,string>("MULR","98"));
    opTable.insert(pair<string,string>("OR","44"));
    opTable.insert(pair<string,string>("RD","D8"));

    opTable.insert(pair<string,string>("RSUB","4C"));

    opTable.insert(pair<string,string>("STA","0C"));
    opTable.insert(pair<string,string>("STB","78"));
    opTable.insert(pair<string,string>("STCH","54"));
    opTable.insert(pair<string,string>("STX","10"));
    opTable.insert(pair<string,string>("STL","14"));

    opTable.insert(pair<string,string>("SUB","1C"));
    opTable.insert(pair<string,string>("SUBR","94"));

    opTable.insert(pair<string,string>("TD","E0"));
    opTable.insert(pair<string,string>("TIX","2C"));
    opTable.insert(pair<string,string>("TIXR","B8"));
    
    opTable.insert(pair<string,string>("WD","DC"));

}