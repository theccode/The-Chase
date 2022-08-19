#pragma once

// various game parameters

namespace GameParams {
	/**
	 * heuristic evaluation coefficients, each controls how much import is put
	 * upon a specific heuristic, a general guideline is  C1 > C3 > C2 with C1
	 * being fairly large in comparison and C2 generally being very small; for
	 * example: C1 = 12, C2 = 1, C3 = 3 or C1 = 20, C2 = 2, C3 = 5, and so on
	 */
	const static int C1 = 12; // for material value
	const static int C2 = 1; // for mobility value
	const static int C3 = 3; // for pawn rank value

	/**
	 * move buffer for AI, prevents move reduplication within the last few
	 * moves. This should be relatively small perhaps ideally in the single
	 * digits. Much larger and the AI will tend to forfeit early
	 */
	const static unsigned int BUFFER_SIZE = 3;

	/**
	 * piece values, generally keep this the way it is, there's no real need to
	 * change them
	 */
	const static unsigned int P_VAL = 1; 
	const static unsigned int N_VAL = 3; 
	const static unsigned int F_VAL = 3; // for Fugitive
	const static unsigned int H_VAL = 5; // for helpers
	const static unsigned int A_VAL = 100; // for Agents

	/**
	 * game state values, generally keep this the way it is, there's no real
	 * need to change them
	 */
	const static int STALEMATE = 0;
	const static int DRAW = 0;
	const static int CHECK = 2; // check is multiplicand

	const static int CHECKMATE = (CHECK * ((C1*103) + (C2*215) + (C3*48))) + 1;
}
