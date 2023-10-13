#include <iostream>
#include <bitset>
#include <fstream>
#include <set>
#include "State.h"
#include "Agent.h"

int main()
{
    std::cout << "hello World\n";

    // std::bitset<196> encoding(1);
    // encoding <<= 2;




    // std::fstream file("brain.txt");
    // if(!file.is_open()) { std::cerr << "file failed to open\n"; throw; }
    // file << encoding;
    // file.close();


    std::bitset<ENCODINGSIZE> seven(7);
    std::bitset<ENCODINGSIZE> eight(8);

    std::set<std::bitset<ENCODINGSIZE>, EncodingCompare> mySet;

    mySet.insert(seven);
    mySet.insert(eight);

    std::cout << mySet.size() << "\n";

    return 0;
}