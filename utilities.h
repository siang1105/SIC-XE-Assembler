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