#include <iostream>
#include <bitset>
#include <fstream>
#include <set>
#include "State.h"
#include "Agent.h"

int main()
{
    std::ofstream file("brain.txt");
    if (!file.is_open()) 
    {
        std::cerr << "file failed to open\n";
        return 1;
    }

    AgentTrainer trainer(file);
    Ultimate3TState state;
    trainer.minimax(state);
    trainer.writeToOutput();

    file.close();
    return 0;
}