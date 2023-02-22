#ifndef PASS2_H
#define PASS2_H

#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <iomanip>
#include <string>

#include "utilities.h"
#include <ctype.h>

void generateOpcode(vector<vector<string> >code, vector<int>&objectCodeInt, map<string,string>opTable, map<string,string> symbolTable, int &baseAddress, int lines){
    for(int i = 1; i < lines; i++){
        if(code[i].size() == 1){
            if(code[i][0][0]=='+'){
                objectCodeInt.push_back(handleFormat4(code[i], code[i][0], opTable));
            }
            else{
                objectCodeInt.push_back(handleFormat3(code[i], code[i][0], opTable));
            }
        }
        else if(code[i].size() == 2){
            if(code[i][0][0] == '+'){
                objectCodeInt.push_back(handleFormat4(code[i], code[i][0], opTable));
            }
            else if(code[i][0] == "ADDR" || code[i][0]=="CLEAR" || code[i][0]=="COMPR" || code[i][0]=="TIXR"){
                objectCodeInt.push_back(handleFormat2(code[i], code[i][0], opTable));
            }
            else if(code[i][0] == "BASE"){
                objectCodeInt.push_back(-1);
                baseAddress = hexStringToDec(symbolTable.find(code[i][1]) -> second);
            }
            else{
                objectCodeInt.push_back(handleFormat3(code[i], code[i][0] ,opTable));
            }
        }
        else if(code[i].size() == 3){
            if(code[i][1][0] == '+'){
                objectCodeInt.push_back(handleFormat4(code[i], code[i][1], opTable));
            }
            else if(code[i][1] == "ADDR" || code[i][1] == "CLEAR" || code[i][1] == "COMPR" || code[i][1]=="TIXR"){
                objectCodeInt.push_back(handleFormat2(code[i], code[i][1] ,opTable));
            }
            else if(code[i][1] == "BASE"){
                objectCodeInt.push_back(-1);
                baseAddress = hexStringToDec(symbolTable.find(code[i][2]) -> second);
            }
            else if(code[i][1] == "RESW" || code[i][1] == "RESB"){
                objectCodeInt.push_back(-2);
            }
            else if((code[i][2][0] == 'X' || code[i][2][0] == 'x') && code[i][2].size() >= 3){
                string temp, declaredValue;
                int stringLength = code[i][2].substr(2,code[i][2].size() - 3).size();
                declaredValue = code[i][2].substr(2,code[i][2].size() - 3);
                if(stringLength % 2 == 0){
                    for(int i = 0; i < stringLength / 2; i++){
                        objectCodeInt.push_back(-3);
                    }
                }
            }
            else if((code[i][2][0] == 'C' || code[i][2][0] == 'c') && code[i][2].size() >= 3){
                stringstream stream;
                string temp, declaredValue;
                int stringLength = code[i][2].substr(2, code[i][2].size() - 3).size();
                declaredValue = code[i][2].substr(2, code[i][2].size() - 3);
                for(int i = 0; i < stringLength; i++){
                    objectCodeInt.push_back(-4);
                }
            }
            else{
                objectCodeInt.push_back(handleFormat3(code[i], code[i][1], opTable));
            }
        }
    }
}

void generateAddresses(vector<vector<string> >code, vector<string>&objectCode,vector<int>objectCodeInt, map<string,string> symbolTableMap,vector<int> location, int baseAddress, int lines){
    stringstream stream;
    int getObjIndex = 0;
    int getLocationIndex = 1;

    for(int i = 1; i < lines; i++){
        if(objectCodeInt[getObjIndex] == -1){
            objectCode.push_back("-");
            getObjIndex++;
            getLocationIndex++;
            continue;
        }
        else if(objectCodeInt[getObjIndex] == -2){
            objectCode.push_back("--");
            getObjIndex++;
            getLocationIndex++;
            continue;
        }
        else if(code[i].size() == 1){
            if(code[i][0][0] == '+'){
                objectCode.push_back(intToHexString(objectCodeInt[getObjIndex] << 20));
            }
            else{
                objectCode.push_back(intToHexString(objectCodeInt[getObjIndex] << 12));
            }
            getObjIndex++;
            getLocationIndex++;
        }
        else if(code[i].size() == 2){
            if(code[i][0][0] == '+'){
                if(code[i][1][0] == '#'){
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    getLocationIndex++;
                    tmp = tmp << 20;
                    if(symbolTableMap.count(code[i][1].substr(1, code[i][1].size() - 1)) > 0){
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1, code[i][1].size() - 1)) -> second));
                        tmp = tmp | disp;
                    }
                    else{
                        int disp = stringToDec(code[i][1].substr(1,code[i][1].size() - 1));
                        tmp = tmp | disp;
                    }
                    stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                    objectCode.push_back(stream.str());
                    stream.str(string());
                }
                else if(code[i][1][0] == '@'){
                    if(symbolTableMap.count(code[i][1].substr(1, code[i][1].size() - 1))>0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 2;
                        tmp = tmp << 20; // ?
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1, code[i][1].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        disp = disp & 1048575;
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][1].substr(code[i][1].size()-2,2) == ",X"){
                    if(symbolTableMap.count(code[i][1].substr(0, code[i][1].size() - 2)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 3; // 
                        tmp = tmp << 20;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(0, code[i][1].size() - 2)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        disp = disp & 1048575;
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else{
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    getLocationIndex++;
                    tmp = tmp << 20;
                    int disp = hexStringToDec(symbolTableMap.find(code[i][1]) -> second);
                    tmp = tmp | disp;
                    stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                    objectCode.push_back(stream.str());
                    stream.str(string());
                }
            }
            else if(code[i][0]=="ADDR" || code[i][0]=="CLEAR" || code[i][0]=="COMPR" || code[i][0]=="TIXR"){
                objectCode.push_back(intToHexString(objectCodeInt[getObjIndex]));
                getObjIndex++;
                getLocationIndex++;
            }
            else{
                if(code[i][1][0] == '#'){
                    if(symbolTableMap.count(code[i][1].substr(1, code[i][1].size() - 1)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 2;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1,code[i][1].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1,code[i][1].size() - 1)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                    else{
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        getLocationIndex++;
                        tmp = tmp << 12;
                        // cout<<"hex "<<hexStringToDec(code[i][1].substr(1,code[i][1].size() - 1))<<endl;
                        // cout<<"dec "<<stringToDec(code[i][1].substr(1,code[i][1].size() - 1))<<endl;
                        // int disp = hexStringToDec(code[i][1].substr(1,code[i][1].size() - 1));
                        int disp = stringToDec(code[i][1].substr(1,code[i][1].size() - 1));
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][1][0] == '@'){
                    if(symbolTableMap.count(code[i][1].substr(1,code[i][1].size() - 1)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 2;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1,code[i][1].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(1,code[i][1].size() - 1)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][1].substr(code[i][1].size() - 2, 2) == ",X"){
                    if(symbolTableMap.count(code[i][1].substr(0, code[i][1].size() - 2)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(0,code[i][1].size() - 2)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][1].substr(0,code[i][1].size() - 2)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else{
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    tmp = tmp << 12;
                    int disp = hexStringToDec(symbolTableMap.find(code[i][1]) -> second) - location[getLocationIndex];
                    getLocationIndex++;
                    if(disp >= -2048 && disp <= 2047){
                        disp = disp & 4095;
                        tmp = tmp | disp;
                    }
                    else{
                        tmp = tmp | 16384;
                        tmp = tmp & 16764927;
                        disp = (hexStringToDec(symbolTableMap.find(code[i][1]) -> second)) - baseAddress;
                        disp = disp & 4095;
                        tmp = tmp | disp;
                    }
                    stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                    objectCode.push_back(stream.str());
                    stream.str(string());
                }
            }
        }
        else if(code[i].size() == 3){
            if(code[i][1][0]=='+'){
                if(code[i][2][0] == '#'){
                    if(symbolTableMap.count(code[i][2].substr(1, code[i][2].size() - 1)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp << 20;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1, code[i][2].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        disp = disp & 1048575;
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                    else{
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        getLocationIndex++;
                        tmp = tmp << 20;
                        // int disp = hexStringToDec(code[i][2].substr(1, code[i][2].size() - 1));
                        int disp = stringToDec(code[i][2].substr(1, code[i][2].size() - 1));
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][2][0] == '@'){
                    if(symbolTableMap.count(code[i][2].substr(1, code[i][2].size() - 1))>0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp |= 2;
                        tmp = tmp << 20;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1, code[i][2].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        disp = disp & 1048575;
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][2].substr(code[i][2].size()-2,2) == ",X"){
                    if(symbolTableMap.count(code[i][2].substr(0, code[i][2].size() - 2)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 3;
                        tmp = tmp << 20;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(0,code[i][2].size() - 2)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        disp = disp & 1048575;
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else{
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    getLocationIndex++;
                    tmp = tmp << 20;
                    int disp = hexStringToDec(symbolTableMap.find(code[i][2]) -> second);
                    tmp = tmp | disp;
                    stream<<setfill('0')<<setw(8)<<intToHexString(tmp);
                    objectCode.push_back(stream.str());
                    stream.str(string());
                }
            }
            else if(code[i][1]=="ADDR" || code[i][1]=="CLEAR" || code[i][1]=="COMPR" || code[i][1]=="TIXR"){
                objectCode.push_back(intToHexString(objectCodeInt[getObjIndex]));
                getObjIndex++;
                getLocationIndex++;
            }
            else if((code[i][2][0] == 'X' || code[i][2][0] == 'x') && code[i][2].size() >= 3){
                int strLength = code[i][2].substr(2,code[i][2].size() - 3).size();
                string str = "";
                string tmpStr = "";
                tmpStr = code[i][2].substr(2,code[i][2].size() - 3);
                for(int k = 0; k < strLength / 2; k++){
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    getLocationIndex++;
                }
                objectCode.push_back(tmpStr);
            }
            else if((code[i][2][0] == 'C' || code[i][2][0] == 'c')&& code[i][2].size() >= 3){
                int strLength = code[i][2].substr(2,code[i][2].size() - 3).size();
                string str = "";
                string tmpStr = "";
                tmpStr = code[i][2].substr(2,code[i][2].size() - 3);
                for(int k = 0; k < strLength; k++){
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    getLocationIndex++;
                    if(tmp == -4){
                        int ascii = tmpStr[k];
                        str += intToHexString(ascii);
                    }
                }
                objectCode.push_back(str);
            }
            else{
                if(code[i][2][0] == '#'){
                    if(symbolTableMap.count(code[i][2].substr(1, code[i][2].size() - 1)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 2;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1,code[i][2].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1, code[i][2].size() - 1)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                    else{
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        getLocationIndex++;
                        tmp = tmp << 12;
                        // int disp = hexStringToDec(code[i][2].substr(1, code[i][2].size() - 1));
                        int disp = stringToDec(code[i][2].substr(1, code[i][2].size() - 1));
                        tmp = tmp | disp;
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][2][0]=='@'){
                    if(symbolTableMap.count(code[i][2].substr(1, code[i][2].size() - 1)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 2;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1, code[i][2].size() - 1)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(1, code[i][2].size() - 1)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else if(code[i][2].substr(code[i][2].size()-2, 2) == ",X"){
                    if(symbolTableMap.count(code[i][2].substr(0, code[i][2].size() - 2)) > 0){
                        int tmp = objectCodeInt[getObjIndex];
                        getObjIndex++;
                        tmp = tmp | 3;
                        tmp = tmp << 12;
                        int disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(0, code[i][2].size() - 2)) -> second)) - location[getLocationIndex];
                        getLocationIndex++;
                        if(disp >= -2048 && disp <= 2047){
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        else{
                            tmp = tmp | 16384;
                            tmp = tmp & 16764927;
                            disp = (hexStringToDec(symbolTableMap.find(code[i][2].substr(0, code[i][2].size() - 2)) -> second)) - baseAddress;
                            disp = disp & 4095;
                            tmp = tmp | disp;
                        }
                        stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                        objectCode.push_back(stream.str());
                        stream.str(string());
                    }
                }
                else{
                    int tmp = objectCodeInt[getObjIndex];
                    getObjIndex++;
                    tmp = tmp << 12;
                    int disp = hexStringToDec(symbolTableMap.find(code[i][2]) -> second) - location[getLocationIndex];
                    getLocationIndex++;
                    if(disp >= -2048 && disp <= 2047){
                        disp = disp & 4095; //111111111111
                        tmp = tmp | disp;
                    }
                    else{
                        tmp = tmp | 16384;//100000000000000
                        tmp = tmp & 16764927;
                        disp = (hexStringToDec(symbolTableMap.find(code[i][2])->second))-baseAddress;
                        disp = disp & 4095;
                        tmp = tmp | disp;
                    }
                    stream<<setfill('0')<<setw(6)<<intToHexString(tmp);
                    objectCode.push_back(stream.str());
                    stream.str(string());
                }
            }
        }
    }
}

void generateObjectCode(vector<string> objectCode){
        ofstream file;
        file.open("ObjectCode.txt");
        file<<"        Object Code"<<endl;
        file<<"--------------------------------"<<endl;
        for(int i = 0; i < objectCode.size(); i++){
            file<<objectCode[i]<<endl;
        }
        file.close();
}


#endif