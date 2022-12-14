#include "Board.hpp"

/**
 * Board class definition, describes how the game board is organized as well as
 * some gameplay logic related to the board
 */

// board constructor
Board::Board() {
	initNormalBoard();
}

/**
 * method to move piece from one space to another, this method handles all
 * movement logic when player methods confirm moves are valid
 * @param m - the move to make
 */
void Board::movePiece(Move m) {
	bool color = (*this)(m.getOrigC(), m.getOrigR()).getPiece().getColor();
	Tile destMove = (*this)(m.getDestC(), m.getDestR());
	if (destMove && destMove.getPiece().getColor() != color) {
		/**
		 * Tile::reset() is an explicit destructor as we want specific logic for
		 * when a tile is destroyed but also to only be able to call it
		 * explicitly rather than implicit default
		 */
		(*this)(m.getDestC(), m.getDestR()).reset();
	}
	// make a new copy of the origin tile and insert into destination tile
	switch((*this)(m.getOrigC(), m.getOrigR()).getPiece().getType()) {
		
		case 'F': case 'f': // if fugitive
			(*this)(m.getDestC(), m.getDestR()) = new Fugitive(color); break;
		case 'A': case 'a': // if agent
		//short circuit in case tile is empty
			char idealColor = ((color) ? 'R' : 'r');
			if (destMove && destMove.getPiece().getType() == idealColor) {
				// init new pieces in correct positions
				(*this)(m.getDestC(), m.getDestR()) = new Agent(color);
				(*this)(m.getOrigC(), m.getOrigR()) = new Helper(color);
				// set those pieces as moved
				(*this)(m.getDestC(), m.getDestR()).getPiece().setMoved(true);
				(*this)(m.getOrigC(), m.getOrigR()).getPiece().setMoved(true);
				// we do not need the destructor below, so exit method
				return;
			} else {
				// otherwise it is a normal move
				(*this)(m.getDestC(), m.getDestR()) = new Agent(color);
			} break;
	}
	// mark the piece as moved
	(*this)(m.getDestC(), m.getDestR()).getPiece().setMoved(true);
	// destroy the origin to complete the move
	(*this)(m.getOrigC(), m.getOrigR()).reset();
}

// method to create a standard positioned chess board
void Board::initNormalBoard() {
	// back row Fugitive
	(*this)(D, 2-1) = new Fugitive(BLACK);
	for (unsigned int i = 0; i < COLS; i++){
		//put an agent every other cell
		if (i % 2 == 0){
			(*this)(i, 1-1) = new Agent(WHITE);
		}
	}
}

/**
 * method to determine if a player is in checkmate
 * @param c - the player to check
 * @return - whether that player is checkmated
 */
bool Board::determineCheckmate(bool c) {
	if (determineCheck(c)) {
		/**
		 * must go through every next-step board state to determine if a
		 * checkmate has occurred. Find every move the proposed player can make
		 * and see if any of those moves leaves check
		 */
		std::vector<Move> moveList = getAllMoves(!c);
		// find all next-step board states
		for (unsigned int i = 0; i < moveList.size(); i++) {
			Board copy(*this);
			copy.movePiece(moveList[i]);
			// if successfully able to leave check
			if (!copy.determineCheck(c)) { return false; }
			/**
			 * as copy is not a pointer, it has implicit destruction upon
			 * leaving scope here. No destructor or handler is necessary, thus
			 * no memory leak
			 */
		}
		// if you get here, no move gets out of check
		return true;
	}
	return false; // if not in check, can't be checkmate
}

/**
 * method to determine if a color is in check
 * @param c - the color to check
 * @return - whether that player is in check
 */
bool Board::determineCheck(bool c) {
	// get all moves of opponent
	std::vector<Move> moveList = getAllNonAgentMoves(c);
	// for each of those moves
	for (unsigned int i = 0; i < moveList.size(); i++) {
		// compare the destination to see if it coincides with agent
		Tile t = (*this)(moveList[i].getDestC(), moveList[i].getDestR());
		if (t && t.getPiece().getType() == ((c) ? 'K' : 'k')) {
			return true;
		}
	}
	return false;
}

/**
 * method to determine if a player is in stalemate
 * @param c - the player to check
 * @return - whether that player is in stalemate
 */
bool Board::determineStalemate(bool c) {
	// cannot be in stalemate if in check
	if (determineCheck(c)) { return false; }
	// if you cannot make any moves, it is a stalemate
	if (getAllMoves(!c).size() == 0) { return true; }
	return false; // all else, not stalemate
}

/**
 * method to determine a draw has occurred
 * @return - whether a draw has occurred
 */
bool Board::determineDraw() {
	// determine if only the two agents exist
	int countAgents = 0;
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			if ((*this)(j, i)) { // if piece exists
				countAgents += (*this)(j, i).getPiece().getValue();
			}
		}
	}
	// two agents would have board value equal to two agents
	return (countAgents == 2 * GameParams::A_VAL);
}

bool Board::determineFugitivePosition(bool c){
		// get all moves of opponent
	std::vector<Move> moveList = getAllNonAgentMoves(c);
	// for each of those moves
	for (unsigned int i = 0; i < moveList.size(); i++) {
		// compare the destination to see if it coincides with agent
		Tile t = (*this)(moveList[i].getDestC(), moveList[i].getDestR());
		std::cout << t;
		if (t && t.getPiece().getType() == ((c) ? 'F' : 'f')) {
			return true;
		}
	}
	return false;
}

/**
 * method to find all moves possible
 * @param c - the color to look for
 * @return - a list of moves for player
 */
std::vector<Move> Board::getAllMoves(bool c) {
	std::vector<Move> moveList; // master move list
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			// short circuit in case non-occupied tile or wrong color
			if (!(*this)(j, i) || (*this)(j, i).getPiece().getColor() == c) {
				continue;
			}
			// collect moves from tiled piece
			std::vector<Move> m = (*this)(j,i).getPiece().getMoves(this, j, i);
			for (unsigned int k = 0; k < m.size(); k++) {
				// append those moves to the master list
				moveList.push_back(m[k]);
			}
		}
	}
	return moveList;
}

/**
 * method to find all moves possible except by K, a separate function is needed
 * to avoid infinite recursion when >1 an agent is on the board
 * @param c - the color to look for
 * @return - a list of moves for player
 */
std::vector<Move> Board::getAllNonAgentMoves(bool c) {
	std::vector<Move> moveList; // master move list
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			// short circuit in case non-occupied tile or wrong color
			if (!(*this)(j, i) || (*this)(j, i).getPiece().getColor() == c) {
				continue;
			}
			// disregard agents as legally they cannot be next to each other
			if ((*this)(j, i).getPiece().getType() == ((c) ? 'a' : 'A')) {
				continue;
			}
			// collect moves from tiled piece
			std::vector<Move> m = (*this)(j,i).getPiece().getMoves(this, j, i);
			for (unsigned int k = 0; k < m.size(); k++) {
				// append those moves to the master list
				moveList.push_back(m[k]);
			}
		}
	}
	return moveList;
}

/**
 * method to count the piece values of all pieces
 * @param c - the color to check pieces for values
 * @return - the total value of those colored pieces
 */
int Board::getAllPieceValues(bool c) {
	int count = 0;
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			if ((*this)(j, i)) {
				// if piece exists and right color
				if ((*this)(j, i).getPiece().getColor() == c) {
					count += (*this)(j, i).getPiece().getValue();
				} else {
					count -= (*this)(j, i).getPiece().getValue();
				}
			}
		}
	}
	return count;
}

/**
 * method to find mobility value of the board
 * @param c - the color to check for
 * @return - total mobility score
 */
int Board::getAllMobilityValues(bool c) {
	int count = 0;
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			if (!(*this)(j, i)) { continue; } // only if piece exists
			// if piece exists and right color
			if ((*this)(j, i).getPiece().getColor() == c) {
				count += (*this)(j, i).getPiece().getMoves(this, j, i).size();
			} else {
				count -= (*this)(j, i).getPiece().getMoves(this, j, i).size();
			}
		}
	}
	return count;
}

/**
 * method to determine pawn control
 * @param c - the color to check for
 * @return - the total pieces control
 */
int Board::getAllPawnValues(bool c) {
	int count = 0;
	for (unsigned int i = 0; i < COLS; i++) {
		for (unsigned int j = 0; j < ROWS; j++) {
			// if piece exists
			if ((*this)(j, i)) {
				// and is pawn
				if ((*this)(j, i).getPiece().getValue() == GameParams::P_VAL) {
					// if matching color
					if ((*this)(j, i).getPiece().getColor() == c) {
						// white pawns are on second row initially
						count += ((c == WHITE) ? i-1 : 6-i);
					} else {
						// black pawns are on seventh row initially
						count -= ((c == WHITE) ? 6-i : i-1);
					}
				}
			}
		}
	}
	return count;
}

// method to print the game board to console
void Board::printBoard() {
	std::cout << "\n     ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << (char)(97 + i) << "   ";
	}
	std::cout << "\n   ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << "+---";
	}
	std::cout << "+ \n";
	for (unsigned int i = COLS; i > 0; i--) {
		std::cout << " " << i << " | ";
		for (unsigned int j = 0; j < ROWS; j++) {
			std::cout << (*this)(j, i-1) << " | ";
		}
		std::cout << i << "\n   ";
		for (unsigned int j = 0; j < COLS; j++) {
			std::cout << "+---";
		}
		std::cout << "+ \n";
	}
	std::cout << "     ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << (char)(97 + i) << "   ";
	}
	std::cout << "\n";
}

/**
 * method to print a modified board showing possible moves for a piece
 * @param c - the columnar coordinate of the piece
 * @param r - the row coordinate of the piece
 * @param m - the movelist from caller
 */
void Board::showMoves(unsigned int c, unsigned int r, std::vector<Move> m) {
	std::stringstream sb;
	std::vector<Move> moveList;
	// init a blank move with origin the same
	Move dummyMove(c, r, 0, 0);
	// find moves with matching origin
	for (unsigned int i = 0; i < m.size(); i++) {
		if (m[i] <= dummyMove) { moveList.push_back(m[i]); }
	}
	// for easier machine reading, make a vector of coords
	std::vector<std::vector<int> > cords(moveList.size(), std::vector<int>(2));
	for (unsigned int i = 0; i < moveList.size(); i++) {
		cords[i][0] = moveList[i].getDestC();
		cords[i][1] = moveList[i].getDestR();
	}
	std::cout << "\n     ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << (char)(97 + i) << "   ";
	}
	std::cout << "\n   ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << "+---";
	}
	std::cout << "+ \n ";
	for (unsigned int i = COLS; i > 0; i--) {
		std::cout << i << " |";
		for (unsigned int j = 0; j < ROWS; j++) {
			for (unsigned int k = 0; k < moveList.size(); k++) {
				if (cords[k][0] == (int)j && cords[k][1] == (int)(i-1)) {
					sb << "o" << (*this)(j, i-1) << "o|";
					break;
				}
			}
			if (sb.str().size() > 0) {
				std::cout << sb.str();
			} else {
				std::cout << " " << (*this)(j, i-1) << " |";
			}
			sb.str(std::string());
		}
		std::cout << " " << i << "\n   ";
		for (unsigned int j = 0; j < COLS; j++) {
			std::cout << "+---";
		}
		std::cout << "+ \n ";
	}
	std::cout << "    ";
	for (unsigned int i = 0; i < COLS; i++) {
		std::cout << (char)(97 + i) << "   ";
	}
	std::cout << "\n\n";
	std::cout << ((moveList.size() > 0) ? "Moves:\n" : "No moves.\n");
	for (unsigned int i = 0; i < moveList.size(); i++) {
		std::cout << moveList[i];
		if ((i+1) % 5 == 0) {
			std::cout << "\n";
		} else if (i < moveList.size()-1) {
			std::cout << ", ";
		} else if (i == moveList.size()-1) {
			std::cout << "\n";
		}
	}
}

// operator methods
Tile& Board::operator()(unsigned int c, unsigned int r) {
	return tiles[c][r];
}

Tile const& Board::operator()(unsigned int c, unsigned int r) const {
	return tiles[c][r];
}