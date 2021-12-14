#include <iostream>
#include <iomanip>  
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <algorithm>
#include<list>
#include <unordered_set>
#include <queue>

#define AddressSize 32
using namespace std;

int convertToDec(string num)
{
    num.erase(0,2);
    
    int base = 1;
    int temp = 0;
    
    for (int i= num.size() - 1; i>=0; i--) {
        num[i] = toupper(num[i]);
        if (num[i]>='0' && num[i]<='9') {
            temp += (num[i] - 48)*base;
            base = base * 16;
        }
        else if (num[i]>='A' && num[i]<='F') {
            temp += (num[i] - 55)*base;
            base = base*16;
        }
    }

    return temp;
}
string convertToHex(int num)
{
    if(num == 0)
        return ("0x0");
    // char array to store hexadecimal number
    char hexaDeciNum[100];
    string address;
 
    // counter for hexadecimal number array
    int i = 0;
    while (num != 0) {
        // temporary variable to store remainder
        int temp = 0;
 
        // storing remainder in temp variable.
        temp = num % 16;
 
        // check if temp < 10
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
 
        num = num / 16;
    }
 
    // printing hexadecimal number array in reverse order
    address += '0';
    address += 'x';

    for (int j = i - 1; j >= 0; j--)
        address += hexaDeciNum[j];
    
    
    return address;
}
struct Instruction
{
    string address;
    char instruction;
    int value;

    Instruction(string address,char instruction, int value): address(address),instruction(instruction),value(value) {}
};
struct CacheFullyAssociative
{
    int numOfSet,numOfBytesPerLine,numOfLinesPerSet,offsetWidth,lineWidth,setSize = 0;
    int totalCount = 0;
    int hitCount = 0;
    int missCount = 0;

    list<unsigned long> cacheMem;

    CacheFullyAssociative(int numOfSet,int numOfBytesPerLine,int numOfLinesPerSet) : numOfSet(numOfSet),numOfBytesPerLine(numOfBytesPerLine),numOfLinesPerSet(numOfLinesPerSet)
    {
        offsetWidth = (int)(log(numOfBytesPerLine) / log(2));
        lineWidth = (int)(log(numOfSet) / log(2));
    }
    void GetRatioLRU(vector<Instruction> info)
    {
       for(int i = 0;i<info.size();i++){
            bitset<AddressSize> address (convertToDec(info[i].address));
            
            string offsetString = address.to_string().substr((AddressSize - offsetWidth),string::npos);
            string tagString = address.to_string().substr(0,(AddressSize - offsetWidth));
            
            bitset<AddressSize> offsetVal_bin (offsetString);
            bitset<AddressSize> tagVal_bin (tagString);

            for(auto it = cacheMem.begin();it != cacheMem.end();it++){
                if((*it) == tagVal_bin.to_ulong()){
                    hitCount++;
                    cacheMem.erase(it);
                    break;
                }
            }
            if(cacheMem.size() == numOfBytesPerLine){
                cacheMem.pop_back();
            }
            totalCount++;
            cacheMem.push_front(tagVal_bin.to_ulong());
        }
    
        cout << "Hits: " << hitCount << endl;
        cout << "Misses: "<<(totalCount-hitCount)<<endl;
        cout << "Hit Rate of Fully Associative: " << (double)(hitCount)/(double)(totalCount) << endl;
    }
    
    void GetRatioFIFO(vector<Instruction> info)
    {
       for(int i = 0;i<info.size();i++){
            bitset<AddressSize> address (convertToDec(info[i].address));
            
            string offsetString = address.to_string().substr((AddressSize - offsetWidth),string::npos);
            string tagString = address.to_string().substr(0,(AddressSize - offsetWidth));
            
            bitset<AddressSize> offsetVal_bin (offsetString);
            bitset<AddressSize> tagVal_bin (tagString);
    
            bool hit = false;
            for(auto it = cacheMem.begin();it != cacheMem.end();it++){
                if((*it) == tagVal_bin.to_ulong()){
                    hit = true;
                    hitCount++;
                    break;
                }
            }
            if(hit){
                totalCount++;
                continue;
            }
            if(cacheMem.size() == numOfBytesPerLine){
                cacheMem.pop_back();
            }
            totalCount++;
            cacheMem.push_front(tagVal_bin.to_ulong());
        }
        cout << "Hits: " << hitCount << endl;
        cout << "Misses: "<<(totalCount-hitCount)<<endl;
        cout << "Hit Rate of Fully Associative: " << (double)(hitCount)/(double)(totalCount) << endl;
    }

};
struct CacheSetAssociative
{
    /*
    Parameters:
    1.- How may ways is the cache. 2-way, 4-way or 8-way?
    2.- Number of Sets in the cache -> this should not be a paremeter we need to calc this 
    3.- Number of bytes per Line!! (or a block )
    4.- num of lines per set (this is unused but whatever)  // 

    after that we will calc this things
    - 
    */
    int numOfSet,numOfBytesPerLine,numOfLinesPerSet,offsetWidth,lineWidth,setSize = 0;
    int totalCount = 0;
    int hitCount = 0;
    
    //The capacity of my list would be numOfLinesPerSet
    unordered_map<unsigned long, list<unsigned long>> cacheMem;

    //Key is index and the list is the where the values are
    CacheSetAssociative(int numOfSet,int numOfBytesPerLine,int numOfLinesPerSet) : numOfSet(numOfSet),numOfBytesPerLine(numOfBytesPerLine),numOfLinesPerSet(numOfLinesPerSet)
    {
        offsetWidth = (int)(log(numOfBytesPerLine) / log(2));
        lineWidth = (int)(log(numOfSet) / log(2));

    }
    void GetRatioLRU(vector<Instruction> info)
    {
        for(int i = 0;i<info.size();i++){
            bitset<AddressSize> address (convertToDec(info[i].address));
            
            string offsetString = address.to_string().substr((AddressSize - offsetWidth),string::npos);
            string indexString = address.to_string().substr((AddressSize - offsetWidth- lineWidth),lineWidth);
            string tagString = address.to_string().substr(0,(AddressSize - offsetWidth - lineWidth));
            
            bitset<AddressSize> offsetVal_bin (offsetString);
            bitset<AddressSize> indexVal_bin (indexString);
            bitset<AddressSize> tagVal_bin (tagString);
            
            if(cacheMem.find(indexVal_bin.to_ulong()) != cacheMem.end()){
                for(auto it = cacheMem[indexVal_bin.to_ulong()].begin();it != cacheMem[indexVal_bin.to_ulong()].end();it++)
                    if((*it) == tagVal_bin.to_ulong()){
                        hitCount++;
                        //if we got a hit we will erase it and then we will push it on the front
                        cacheMem[indexVal_bin.to_ulong()].erase((it));
                        break;
                    }
                
                if(cacheMem[indexVal_bin.to_ulong()].size() == numOfLinesPerSet){
                    cacheMem[indexVal_bin.to_ulong()].pop_back();
                }
            }
            totalCount++;
            cacheMem[indexVal_bin.to_ulong()].push_front(tagVal_bin.to_ulong());
        }
        cout << "Hits: " << hitCount << endl;
        cout << "Misses: "<<(totalCount - hitCount)<<endl;
        cout << "Hit Rate of Set Associative: " << (double)(hitCount)/(double)(totalCount) << endl;
    }
    
    void GetRatioFIFO(vector<Instruction> info)
    {
        for(int i = 0;i<info.size();i++){
            bitset<AddressSize> address (convertToDec(info[i].address));

            string offsetString = address.to_string().substr((AddressSize - offsetWidth),string::npos);
            string indexString = address.to_string().substr((AddressSize - offsetWidth- lineWidth),lineWidth);
            string tagString = address.to_string().substr(0,(AddressSize - offsetWidth - lineWidth));

            bitset<AddressSize> offsetVal_bin (offsetString);
            bitset<AddressSize> indexVal_bin (indexString);
            bitset<AddressSize> tagVal_bin (tagString);

            bool hit = false;
            if(cacheMem.find(indexVal_bin.to_ulong()) != cacheMem.end()){
                for(auto it = cacheMem[indexVal_bin.to_ulong()].begin();it != cacheMem[indexVal_bin.to_ulong()].end();it++)
                    if((*it) == tagVal_bin.to_ulong()){
                        hitCount++;
                        hit = true;
                        break;
                    }
                //if there is a hit we will not push the value and neither try to erase it
                if(hit){
                    totalCount++;
                    continue;
                }
                if(cacheMem[indexVal_bin.to_ulong()].size() == numOfLinesPerSet){
                    cacheMem[indexVal_bin.to_ulong()].pop_back();
                }
            }
            totalCount++;
            cacheMem[indexVal_bin.to_ulong()].push_front(tagVal_bin.to_ulong());
        }
        cout << "Hits: " << hitCount << endl;
        cout << "Misses: "<<(totalCount - hitCount)<<endl;
        cout << "Hit Rate of Set Associative: " << (double)(hitCount)/(double)(totalCount) << endl;
    }

};
struct CacheDirectMapped
{ 
    unordered_map<unsigned long,unsigned long> cacheMem; 
    int numOfSets, numBlocksPerSet,numOfBytesInBlock,cacheSize;
    //I Think that you need to specify then how you can to be if fully accesitve or 8 4 2 way associtive 
    // Maybe I would do that in other class
    int offsetWidth = 0;
    int lineIDWidth = 0;
    int missCount = 0;
    int hitCount = 0;
    bitset<AddressSize> offsetGetVal;

CacheDirectMapped(int numOfSets,int numBlocksPerSet,int numOfBytesInBlock):numOfSets(numOfSets),numBlocksPerSet(numBlocksPerSet),numOfBytesInBlock(numOfBytesInBlock) 
    {
        cacheSize = (numOfSets * numOfBytesInBlock * numBlocksPerSet);
        offsetWidth = (int)(log(numOfBytesInBlock) / log(2));
        lineIDWidth = (int)(log(numOfSets) / log(2));
            //offsetGetVal will return the value neccesary to bit and the address 
            //it will give a bin num to get the first (offset + lineID) of the address
            // for example if a have address: 0x1FFFFF50 and the offset is 4 and lineID is 4 
            // offsetGetVal will be 0xFF that is 8 bits, if we bit and these to value will get the line
            // that is 0x50

        for(int i = 0; i<(offsetWidth+lineIDWidth);i++)
            offsetGetVal.set(i);
    }
    double GetHitRate(vector<Instruction> instructions)
    {

        for(int i = 0; i<instructions.size();i++){
            bitset<AddressSize> address (convertToDec(instructions[i].address));

            string offsetString = address.to_string().substr((AddressSize - offsetWidth),string::npos);
            string lineString = address.to_string().substr((AddressSize - offsetWidth- lineIDWidth),lineIDWidth);
            string tagString = address.to_string().substr(0,(AddressSize - offsetWidth - lineIDWidth));

            bitset<AddressSize> offsetVal_bin (offsetString);
            bitset<AddressSize> lineVal_bin (lineString);
            bitset<AddressSize> tagVal_bin (tagString);
            
            if(cacheMem.count(lineVal_bin.to_ulong()) != 0 && cacheMem[lineVal_bin.to_ulong()] == tagVal_bin.to_ulong())
                hitCount++;
            else
                missCount++;
        
        cacheMem[lineVal_bin.to_ulong()] = tagVal_bin.to_ulong();
        }
        cout << "Hits: " << hitCount << endl;
        cout << "Misses: " << missCount << endl;
        cout << "Hit Rate of Direct Mapped: ";
        return (double)(hitCount)/(double)(missCount + hitCount);
    }
};
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