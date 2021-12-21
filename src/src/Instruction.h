#ifndef INSTRUCTION_H // include guard
#define INSTRUCTION_H

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

#endif /* INSTRUCTION_H */