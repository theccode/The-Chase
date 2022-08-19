#include "Game.hpp"

// Game class definition

/**
 * Game class constructor
 * @param b - the game board to use
 * @param agent - the agent player
 * @param fugitive - the fugitive player
 */
Game::Game(Board* b, Player* agent, Player* fugitive)
	: gameboard(b), playerA(agent), playerB(fugitive), gameState(0) {
	getPlayerWhite().setBoard(b); // allow visibility of board to white
	getPlayerBlack().setBoard(b); // same for black
}
// main game playing method

void Game::play() {
	while(true) { // continue playing while game not over
		// Fugitive move
		getBoard()->printBoard();
	
		// if( getBoard()->determineFugitivePosition(getPlayerBlack().getColor())){
		// 	setGameState(3); break;
		// }
		if (getBoard()->determineStalemate(getPlayerBlack().getColor())) {
			setGameState(1); break;
		}
		if (getBoard()->determineDraw()) { setGameState(2); break; }
		if (getBoard()->determineCheck(getPlayerBlack().getColor())) {
			std::cout << "\n Fugitive is Arrested!\n";
		}
		fugitiveMove();
		// Agent move
		getBoard()->printBoard(); // print current board
		// check if checkmate, check, draw, or stalemate occurred
		if (getBoard()->determineCheckmate(getPlayerWhite().getColor())) {
			setGameState(4); break;
		}
		if (getBoard()->determineDraw()) { setGameState(2); break; }
		if (getBoard()->determineStalemate(getPlayerWhite().getColor())) {
			setGameState(1); break;
		}
		if (getBoard()->determineCheck(getPlayerWhite().getColor())) {
			std::cout << "\nAgent wins!\n";
		}
		agentMove(); // otherwise make a move
		
	}
	switch(getGameState()) { // if game is over
		case 1: std::cout << "\nAgent wins!\n\n"; break;
		case 2: std::cout << "\nBlah blah blaaah!!!.\n\n"; break;
		case 3: std::cout << "\nFugitive Wins!!!";break;
	}
}

// agent move function
inline void Game::agentMove() {
	std::cout << "\nTurn " << moveCount << ", Agent to move...\n";
	// get a move as prompted
	Move m = getPlayerWhite().promptMove();
	std::cout << "\nAgent has moved " << m << "\n";
	makeMove(m); // make the move
	moveCount++;
}

// fugitive move function
inline void Game::fugitiveMove() {
	std::cout << "\nTurn " << moveCount << ", Fugitive to move...\n";
	// get a move as prompted or AI generatedd;
	std::cout << "Pos: "<< getBoard()->determineFugitivePosition(getPlayerBlack().getColor());
	Move m = getPlayerBlack().promptMove();
	std::cout << "\nFugitive has moved " << m << "\n";
	makeMove(m); // make the move
	moveCount++;
}

/**
 * method to make the prompted move
 * @param m - the move to make
 */
inline void Game::makeMove(Move m) {
	getBoard()->movePiece(m);
}

// accessor methods
Board* Game::getBoard() { return gameboard; }
int Game::getGameState() { return gameState; }
Player& Game::getPlayerWhite() { return *playerA; }
Player& Game::getPlayerBlack() { return *playerB; }

// mutator methods
void Game::setGameState(int s) { gameState = s; }
