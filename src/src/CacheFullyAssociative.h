
#ifndef FULLY_ASSOCIATIVE_H // include guard
#define FULLY_ASSOCIATIVE_H
#include<list>
#include "Instruction.h"

using namespace std;


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

#endif /* FULLY_ASSOCIATIVE_H */