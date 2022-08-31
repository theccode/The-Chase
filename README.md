# The Chase
<img width="640" alt="Screenshot 2022-08-31 at 6 34 10 PM" src="https://user-images.githubusercontent.com/7442471/187756792-0bd0691f-3574-4461-964d-d89f173ec773.png">

``` FIGURE 1 ```

## MAIN FUNCTIONS
**printBoard():** prints current game board. fugitiveMove(): allows fugitive to make moves. agentMove(): allows agent to make moves. gameState(): initialize the state of the game. setGameState(): set game’s state during play time.
**getPlayerWhite():** get Agent player ready. getPlayerBlack(): get Fugitive player ready. promptMove(): method to prompt user for their move **getAllMoves():** get all available moves

## DATA STRUCTURES
The main data structures used are:
Vectors from the STL (Standard Template Library) e.g. Vector<Move> MoveList 
 
 **Main():** The main entry point of the program occur here. User instructions are issued from this
 function.
 **Play():** This is the main game playing method, the play method loops as long as the game is not
 over allowing both Agents and Fugitive to make moves in turn.
 **getGameState():** get game’s state during play time. If fugitive cannot make further moves, then
 agent wins. If fugitive is on line 1, then fugitive escapes.

## GAME DESCRIPTION
In this game, there are 2 players: a fugitive, and 4 agents. The game is played on a regular 8x8 chess board, where the first line represents a river. The goal of the fugitive is to get to the river (i.e. any position on line 1); and the goal of the agents is to block the fugitive before he gets to line 1, so they can put in back to prison. Initially, the fugitive is located on the board at position d2 (on line 2), and the 5 agents are located at positions a1, c1, e1, g1 and h1 (on line 1). See Figure 1 below.
1. Manual entry for the fugitive’s move and automatic moves for the agent In order to get to this stage you have to run ./thechase 1 2

 ![ADVERSARIAL GAME](https://user-images.githubusercontent.com/7442471/187760908-5d54183d-1e33-4841-b03c-ebe579370e5c.png)

``` FIGURE 2 ```

This stage has to do with the movement of the agent to block the fugitive
Agent cannot move back after it moves forward and also the agents can only move diagonally
2. Manual entry for the agent’s move & automatic moves for the fugitive In order to get to this stage you have to run. /thechase 1 2
 
 This has to do with the movement of the fugitive to avoid being blocked by the agents. Also with this the agents can only move forward but not backward.
 
## HEURISTICS USED
The NegaMax Algorithm is an improved version of the MiniMax Algorithm. It is just a more elegant implementation. The NegaMax merges these two into one by always considering the children of the Node N, from N’s point of view. That is we multiply values in alternate rows by value of -1, so that both players become maximizers. This concept is illustrate in the figure below, which essentially depicts the same as minimax. The resulting values are as they would be if we were to evaluate node values using minimax algorithm and then multiply blue rows by -1.
The function NegaMax returns the value of the node b, as seen by the player indicated by color. That is, the better the position is for color, the greater the value that it returns. Note that this is exactly the opposite of what we see in MiniMax. Where the value of the child is reversed (multiplied by -1). In this manner, node values are processed as in Figure below, with the exception of the root. Reversing the root, as mentioned before, is not important. In fact, it can be better if we don't reverse it. This way, the true value (the value it would have had in minimax) of a node b with color c, is returned by NegaMax (b, 0, c).
We need to be careful not to leave out things like that, or make mistakes about them. A common mistake in NegaMax is to get the positive and negative mixed up. It is so easy to make a mistake and end up with a program that always finds the worst possible move for each player. Even worse, you might end up with a program that always finds the best move for one color, but the worse move for the other color.
 
![the chase 2](https://user-images.githubusercontent.com/7442471/187761562-da043695-3902-44cb-b138-0bbd635a7884.png)
