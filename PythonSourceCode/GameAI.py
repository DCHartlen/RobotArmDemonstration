"""
GameAI.py

Created by: D.C. Hartlen
Date:       19-Aug-2018
Updated by: 
Date:

Contains the algorithm to allow a computer to always win or draw at a game of
tic tac toe. The entirety of the algorithm presented here was developed by 
Cecil Wobker (https://cwoebker.com/posts/tic-tac-toe). However, I have made 
changed variable names to clarify thier meaning, added documentation, and added
a function or two to allow for easier interface with other project source codes

NOTE: this source code only works if the computer plays as 'O'. I can't figure
out why, but it always plays the wrong moves if the computer plays as 'X'. I'm
its a logic issue somewhere, but I haven't had the time to fix it

FIXME: find out why computer can only play as 'O'
"""
import random

class Game:
    """
    Class contains all the gameboard informatino and utilities including 
    determining the winner and tree-search for determining the next move.
    However, this class will not actually make a prediction. That is 
    the standalone function below. 
    Inputs:
        squares: 9 entry list containing 'None','X', and 'O'. Used to preload 
            a configuration into the system. Defaults to empty and an empty
            gameboard is generated
    """
    # List of all winning combos
    winningCombos = (
        [0, 1, 2], [3, 4, 5], [6, 7, 8],
        [0, 3, 6], [1, 4, 7], [2, 5, 8],
        [0, 4, 8], [2, 4, 6])
    # Possible winners
    winners = ('X-win', 'Draw', 'O-win')

    def __init__(self, squares=[]):
        if len(squares) == 0:
            self.squares = [None for i in range(9)]
        else:
            self.squares = squares

    def showBoard(self):
        """ Pretty print the current game board"""
        for element in [self.squares[i:i + 3] for i in range(0, len(self.squares), 3)]:
            print( element)

    def availableMoves(self):
        """ Return all empty squares"""
        return [k for k, v in enumerate(self.squares) if v is None]

    def availableCombos(self, player):
        """what combos are available?"""
        return self.availableMoves() + self.getSquares(player)

    def complete(self):
        """Has game ended?"""
        if None not in [v for v in self.squares]:
            return True
        if self.winner() != None:
            return True
        return False
    """ Next three methods return winner"""
    def xWon(self):
        return self.winner() == 'X'

    def oWon(self):
        return self.winner() == 'O'

    def tied(self):
        return self.complete() == True and self.winner() is None

    def winner(self):
        """ Check if there if current player is a winner"""
        for player in ('X', 'O'):
            positions = self.getSquares(player)
            for combo in self.winningCombos:
                win = True
                for pos in combo:
                    if pos not in positions:
                        win = False
                if win:
                    return player
        return None

    def declareWinner(self):
        """ 
        Hacky way to return winner and the winning combo. Can't use winner()
        because the output of winner is altered by xWon() et al. 
        """
        for player in ('X', 'O'):
            positions = self.getSquares(player)
            for combo in self.winningCombos:
                win = True
                for pos in combo:
                    if pos not in positions:
                        win = False
                if win:
                    return player,combo
        return None

    def getSquares(self, player):
        """Return all regions which belong to the current player"""
        return [k for k, v in enumerate(self.squares) if v == player]

    def addMarker(self, position, player):
        """place on square on the board"""
        self.squares[position] = player

    def alphaBeta(self, board, player, alpha, beta):
        """ 
        The magic: binary tree search through all possible moves to find the 
        best one. Return the best move. This function runs recursively
        """    
        if board.complete():
            if board.xWon():
                return -1
            elif board.tied():
                return 0
            elif board.oWon():
                return 1
        for move in board.availableMoves():
            board.addMarker(move, player)
            val = self.alphaBeta(board, getEnemy(player), alpha, beta)
            board.addMarker(move, None)
            if player == 'O':
                if val > alpha:
                    alpha = val
                if alpha >= beta:
                    return beta
            else:
                if val < beta:
                    beta = val
                if beta <= alpha:
                    return alpha
        if player == 'O':
            return alpha
        else:
            return beta


def determineMove(board, player):
    """
    Determine the optimal move given the current gameboard configuration.
    Inputs:
        board: Board class which contains the current state of play
        player: the player for whom the move is predicted.
    Returns:
        Optimal next move for player
    """

    a = -2
    choices = []
    if len(board.availableMoves()) == 9:
        return 4
    for move in board.availableMoves():
        board.addMarker(move, player)
        val = board.alphaBeta(board, getEnemy(player), -2, 2)
        board.addMarker(move, None)
        # print( "move:", move + 1, "causes:", board.winners[val + 1])
        if val > a:
            a = val
            choices = [move]
        elif val == a:
            choices.append(move)
    return random.choice(choices)


def getEnemy(player):
    """
    Returns the other player. getEnemy('X') returns 'O'.
    """
    if player == 'X':
        return 'O'
    return 'X'

"""
Standalone exection allows one to play a game of tic tac toe by specifing
the square ID (0-9) which one wants to play. 
"""
if __name__ == "__main__":
    board = Game()
    board.showBoard()

    while not board.complete():
        player = 'X'
        playerMove = int(input("Next Move: ")) - 1
        if not playerMove in board.availableMoves():
            continue
        board.addMarker(playerMove, player)
        board.showBoard()

        if board.complete():
            break
        player = getEnemy(player)
        computerMove = determineMove(board, player)
        # Bots move is zero indexed
        board.addMarker(computerMove, player)
        board.showBoard()
    print(board.declareWinner())
    print("winner is", board.declareWinner())