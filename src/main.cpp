#include <iostream>
#include <iomanip>  
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <queue>
#include "CacheFullyAssociative.h"
#include "Instruction.h"
#include "CacheSetAssociative.h"
#include "CacheDirectMapped.h"

using namespace std;

void LoadTraceFile(string thePath, vector<Instruction>& instructions)
{
    //This would load the file an return 
    // I can make a class of "Instructions" 
    // The class will have a char (either s or l)
    // it will have a string with the key value or the address
    // it will have a int representing the value 
    
    ifstream file (thePath);
    if(file.is_open()){
        string lineFromFile;
       
        while(getline(file,lineFromFile)){
            istringstream stream (lineFromFile);
            
            string address;
            string instruction;
            int value;

            string tempValue;
            
            getline(stream,instruction,' ');
            getline(stream,address,' '); 
            
            for_each(address.begin()+2, address.end(), [](char & c) {
                c = ::toupper(c);
             });

           getline(stream,tempValue,' ');
           try{
            value = stoi(tempValue);
           }
           catch(const exception& e){
                cout << e.what();
           }
            
            Instruction obj(address,instruction[0],value);
            instructions.push_back(obj);
            
        }
    }
}
int main()
{
    
    vector<Instruction> instructions;
    LoadTraceFile("gcc.trace",instructions);
    //The vector will have all the instructions that we need
    for(int i = 0; i<6;i++){
        cout<<endl;
        CacheFullyAssociative theCache(1,pow(4,i),pow(4,i));
        cout<<"LRU ";
        theCache.GetRatioLRU(instructions);
        cout<<"Cache size: "<<(theCache.numOfBytesPerLine * theCache.numOfLinesPerSet * theCache.numOfSet)<<endl;
        cout<<endl;
        CacheFullyAssociative itCache(1,pow(4,i),pow(4,i));
        cout<<"FIFO ";
        itCache.GetRatioFIFO(instructions);
        cout<<"Cache size: "<<(itCache.numOfBytesPerLine * itCache.numOfLinesPerSet * itCache.numOfSet)<<endl;
        cout<<endl;
    }

    for(int i = 0; i<6;i++){
        CacheDirectMapped cache (pow(4,i),1,pow(4,i));
        cout<<cache.GetHitRate(instructions)<<endl;
        cout<<"Cache size: "<<(cache.numOfBytesInBlock * cache.numBlocksPerSet * cache.numOfSets)<<endl;
        cout<<endl;
    }
    cout<<"2-Way Set Associative Cache"<<endl;
    for(int i = 0; i<6;i++){
        
        CacheSetAssociative myCache(2,pow(4,i),pow(4,i));
        cout<<"LRU ";
        myCache.GetRatioLRU(instructions);
        cout<<"Cache size: "<<(myCache.numOfBytesPerLine * myCache.numOfLinesPerSet * myCache.numOfSet)<<endl;
        cout<<endl;
        
        CacheSetAssociative otherCache(2,pow(4,i),pow(4,i));
        cout<<"FIFO ";
        otherCache.GetRatioFIFO(instructions);
        cout<<"Cache size: "<<(otherCache.numOfBytesPerLine * otherCache.numOfLinesPerSet * otherCache.numOfSet)<<endl;
        cout<<endl;
    }
    cout<<"4-Way Set Associative Cache"<<endl;
    for(int i = 0; i<6;i++){
        
        CacheSetAssociative myCache(4,pow(4,i),pow(4,i));
        cout<<"LRU ";
        myCache.GetRatioLRU(instructions);
        cout<<"Cache size: "<<(myCache.numOfBytesPerLine * myCache.numOfLinesPerSet * myCache.numOfSet)<<endl;
        cout<<endl;
        
        CacheSetAssociative otherCache(4,pow(4,i),pow(4,i));
        cout<<"FIFO ";
        otherCache.GetRatioFIFO(instructions);
        cout<<"Cache size: "<<(otherCache.numOfBytesPerLine * otherCache.numOfLinesPerSet * otherCache.numOfSet)<<endl;
        cout<<endl;
    }
    cout<<"8-Way Set Associative Cache"<<endl;
    for(int i = 0; i<6;i++){
        
        CacheSetAssociative myCache(8,pow(4,i),pow(4,i));
        cout<<"LRU ";
        myCache.GetRatioLRU(instructions);
        cout<<"Cache size: "<<(myCache.numOfBytesPerLine * myCache.numOfLinesPerSet * myCache.numOfSet)<<endl;
        cout<<endl;
        
        CacheSetAssociative otherCache(8,pow(4,i),pow(4,i));
        cout<<"FIFO ";
        otherCache.GetRatioFIFO(instructions);
        cout<<"Cache size: "<<(otherCache.numOfBytesPerLine * otherCache.numOfLinesPerSet * otherCache.numOfSet)<<endl;
        cout<<endl;
    }

    return 0;
}