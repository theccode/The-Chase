#include "game/Game.hpp"
#include "player/Human.hpp"
#include "player/Computer.hpp"

const bool AGENT = true, FUGITIVE = false;

// remind user of proper compilation and execution
inline void printHelp() {
	std::cout << "\nCompile and run the program as follows:\n\n";
	std::cout << " $ g++ source.cpp */*.cpp -o thechase\n";
	std::cout << " $ ./thechase <args>\n\n";

	std::cout << "Provide two arguments like below.\n\n";

	std::cout << " Argument 1: Type of Game\n";
	std::cout << "  1. Human vs. AI\n";
	std::cout << "  2. AI vs. Human\n";
	std::cout << "  3. AI vs. AI\n";
	std::cout << " Argument 2: - Depth for Tree Search\n";
	std::cout << "  - some integer value >0\n\n";

	// std::cout << "When prompted, enter one of the below commands:\n";
	// std::cout << " - a move (in the form of a0b1, i.e. a0 to b1)\n";
	// std::cout << " - a single coordinate to see possible moves\n";
	std::cout << " - \"quit\" to quit the current game\n\n";
}

// main program driver
int main(int argc, char** argv) {
	// create two empty players, init later
	Player* agent; Player* fugitive;
	// init game board
	Board* gameboard = new Board();
	if (argc != 1 && argc != 3) { // if invalid num args
		printHelp(); return 1;
	} else { // no argument or two arguments
		if (argc > 1){ // some ai is involved
			// if invalid depth
			if (atoi(argv[2]) < 1) { printHelp(); return 1; }
			unsigned int depth = atoi(argv[2]); // depth valid, init
			switch(atoi(argv[1])) { // determine type of game
				case 1: // human vs ai
					agent = new Human(AGENT);
					fugitive = new Computer(FUGITIVE, depth);
					break;
				case 2: // ai vs human
					agent = new Computer(AGENT, depth);
					fugitive = new Human(FUGITIVE);
					break;
				case 3: // ai vs ai
					agent = new Computer(AGENT, depth);
					fugitive = new Computer(FUGITIVE, depth);
					break;
				default: printHelp(); return 1;
			}
		} else {
			std::cout << "Provide two arguments: ./thechase <arg1><arg2>.\n\n";
		}
	}
	Game g(gameboard, agent, fugitive); // init game
	g.play(); // play the game
	return 0;
}
