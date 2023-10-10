#include <iostream>
#include <bitset>
#include <fstream>
#include "State.h"

int main()
{
    std::cout << "hello World\n";

    std::bitset<196> encoding(1);
    encoding <<= 2;




    std::fstream file("brain.txt");
    if(!file.is_open()) { std::cerr << "file failed to open\n"; throw; }
    file << encoding;
    file.close();
    return 0;
}