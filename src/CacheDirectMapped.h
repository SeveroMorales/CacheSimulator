//CacheDirectMapped.h

#ifndef DIRECT_MAPPED_H // include guard
#define DIRECT_MAPPED_H
#include "Instruction.h"

using namespace std;
struct CacheDirectMapped
{ 
    unordered_map<unsigned long,unsigned long> cacheMem; 
    int numOfSets, numBlocksPerSet,numOfBytesInBlock,cacheSize;
   
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

#endif /*DIRECT_MAPPED_H*/
