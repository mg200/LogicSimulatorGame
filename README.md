# LogicSimulatorGame
Note: for a better understanding of the game requirements and how to play, read the full document.

A boolean logic "simulator" game using x86 assembly.
Two players can add/remove gates (AND,OR,NAND,NOR,XOR,XNOR) and wires to a board of a total 100 nodes. A player can only add gates to the next consecutive column(i.e. both inputs must be of 
the same column and ouput of the consecutive column), and wires to the next two consecutive columns. There is no limit on the number of additions or removals to the board.

The first ten nodes (column 0) can only be input nodes, while the last ten (column 9) can only be output nodes.

Possible logic values are 1 (high), 0 (low), or U (undefined). All nodes except column 0 are initialized to logic 0. Since the nodes of column 0 are the seed input nodes, they are randomized to random logic values (1 or 0) at the beginning of the game. 

To add any gate, the user types a command of a specific format; each gate is identified by a special character:
AND->A, OR->O, NOR->R, NAND->D, XOR->X, XNOR->R, Wire->W, M->Remove.
The program is not case-sensitive--meaning entering "A" or "a" for AND gate will both work.
Example:
A010318
The above command adds and AND gate, with input1 being the node in column 0, row 1, and input2 being in column 0,row 3, and output at the node in column 1, row 8.

A command like   x011217 is INVALID since both inputs must be at the same column. 

Example of a wire command: W0023...a wire is conncected from the node in column 0, row 0 to the node in column 2, row 3.

To remove the connection (wire/gate) made at any node, you can use the Remove Command

Example: m23 : this command will remove the connection that is at the node in column 2, row 3.

The game in two levels:
1. Level-1: the game ends when the player connects logic 1 to any of his nodes (the top 5 nodes in the last column) AND logic 0 to any of his opponent's nodes (the lower 5 nodes in the last column)
2. Level-2: a player wins when he connects logic 1 to all his nodes AND logic 0 to all his opponent's nodes. 


The game can run on either one PC with two players, or using a serial connection to run it on two different PCs or two different Dosbox terminals on the same PC.
This version of the game is configured to run only run on one PC. To run the game via the serial connection and have fun on two PCs, uncomment and comment the lines mentioned in the MAIN proceducre as explained there.   

To play serially, you first need to enter your names simulataneously and send. Then to go to the game directly, one needs to send a game invitation to the other by pressing F2 or a chat invitation by pressing F4; either way the other user must accept by pressing the same key. For the game the host (the one who sent the game invitation) would need to choose the level.

The game also supports in-game chatting. 
