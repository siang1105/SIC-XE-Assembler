#ifndef HTE_H
#define HTE_H

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "utilities.h"

void createHead(vector<vector<string> > code, vector<int> location, vector<string> &hteRecord){
    stringstream stream;
    int programLength = location[location.size() - 1] - location[0];
    string head = "H ";
    head += code[0][0];
    head += " ";
    stream<<setfill('0') << setw(6) << hex << code[0][2];
    head += stream.str();
    head += " ";

    stream.str(string());

    stream<<setfill('0') << setw(6) << hex << programLength ;
    head += stream.str();
    cout<<"head "<<head<<endl;
    hteRecord.push_back(head);
}

void createText(vector<vector<string> > code, vector<int> location, vector<string> &hteRecord, vector<string>objectCode,int lines){
    int capacity = 60;
    stringstream stream;
    vector<vector<pair<string,int> > > textPool;
    vector<pair<string,int> > textField;

    for(int i = 0; i < objectCode.size(); i++){
        if(objectCode[i] == "-") continue;
        if(objectCode[i] == "--"){
            if(textField.size() == 0) continue;
            else{
                textField.push_back(make_pair("0", location[i])); //appending the text record end address for easier calculations in the future
                textPool.push_back(textField);
                textField.clear();
                capacity = 60;
                continue;
            }
        }
        if(objectCode[i].size() <= capacity){ // 一排最多放十個
            capacity -= objectCode[i].size();
            textField.push_back(make_pair(objectCode[i], location[i]));
        }
        else{
            textField.push_back(make_pair("0", location[i]));
            textPool.push_back(textField);
            textField.clear();
            capacity = 60;
            i--;
        }
    }
    if(textField.size() > 0){
        textField.push_back(make_pair("0", location[location.size() - 1]));
        textPool.push_back(textField);
        textField.clear();
    }

    for(int i = 0; i < textPool.size(); i++){
        string text = "T ";
        stream<<setfill('0') << setw(6) << hex <<textPool[i][0].second;
        text += stream.str();
        text += " ";
        stream.str(string());
        int textLength = (textPool[i][textPool[i].size()-1].second) - (textPool[i][0].second);
        stream<<setfill('0') << setw(2) << hex << textLength ;
        text += stream.str();
        text += " ";
        stream.str(string());
        for(int j = 0; j < textPool[i].size() - 1; j++){
            text += textPool[i][j].first + " ";
        }
        cout<<"text "<<text<<endl;
        hteRecord.push_back(text);
    }
}

void createModification(vector<int> location,vector<string> &hteRecord, vector<vector<string> > code){
    string Modify = "M ";
    stringstream stream;
    int address;
    for(int i = 1; i < code.size(); i++){
        if(code[i].size() == 2){
            if(code[i][0] == "+JSUB"){
                address = location[i-1] + 1;
                stream<<setfill('0') << setw(6) << intToHexString(address);
                Modify += stream.str() + " ";
                stream.str(string());
                Modify += "05";
                hteRecord.push_back(Modify);
                Modify = "M ";
            }
        }
        else if(code[i].size() == 3){
            if(code[i][1] == "+JSUB"){
                address = location[i-1] + 1;
                stream<<setfill('0') << setw(6) << intToHexString(address);
                Modify += stream.str()+" ";
                stream.str(string());
                Modify += "05";
                cout<<"Modify "<<Modify<<endl;
                hteRecord.push_back(Modify);
                Modify = "M ";
            }
        }
        stream.str(string());
    }
}

void createEnd(vector<int> location, vector<string> &hteRecord){
    stringstream stream;
    string End = "E ";
    stream<<setfill('0') << setw(6) << hex << location[0] ;
    End += stream.str();
    cout<<"End "<<End<<endl;
    hteRecord.push_back(End);
}

void generateHTEFile(vector<string> hteRecord){
        ofstream file;
        file.open("HTE.txt");
        file<<"----------Object Program----------"<<endl;
        for(int i = 0; i < hteRecord.size(); i++){
            file<<hteRecord[i]<<endl;
        }
        
        file.close();
}

#endif