//CacheSetAssociative.h
#ifndef SET_ASSOCIATIVE_H // include guard
#define SET_ASSOCIATIVE_H
#include <unordered_map>
#include<list>
#include "Instruction.h"


using namespace std;

struct CacheSetAssociative
{
  
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

#endif /* SET_ASSOCIATIVE_H */