import numpy as np
import time as tm

robotPos = []

def rollOneDie():
    state = 0
    flips = 0
    die = 0
    print("Rolling the die!\nStarting from state", state)
    print("Tossing coin: ")
 #   tm.sleep(0.3)
    state = np.random.choice([1, 2], p=[0.5, 0.5])
    if state == 1:
        print("..heads")
    elif state == 2:
        print("..tails")
    flips += 1
    print("Current state: ", state)
    while (state != 7) and (die == 0) :
        if state == 1:
            flips += 1
    #        tm.sleep(0.3)
            print("Tossing coin:")
     #       tm.sleep(0.3)
            state = np.random.choice([3, 4], p=[0.5, 0.5])
            if state == 3 :
                print("..heads")
            elif state == 4 :
                print("..tails")
            print("Current state: ", state)
        if state == 2 :
            flips += 1
      #      tm.sleep(0.3)
            print("Tossing coin:")
       #     tm.sleep(0.3)
            state = np.random.choice([5, 6], p=[0.5, 0.5])
            if state == 5 :
                print("..heads")
            elif state == 6 :
                print("..tails")
            print("Current state: ", state)
        if state == 3 :
            flips += 1
         #   tm.sleep(0.3)
            print("Tossing coin:")
        #    tm.sleep(0.3)
            state = np.random.choice([1, 7], p=[0.5, 0.5])
            if state == 1:
                print("..heads")
            elif state == 7:
                print("..tails")
                die = 1
            print("Current state: ", state)
        if state == 4 :
            flips += 1
            state = 7
         #   tm.sleep(0.3)
            print("Tossing coin:")
         #   tm.sleep(0.3)
            die = np.random.choice([2, 3], p=[0.5, 0.5])
            if die == 2:
                print("..heads")
            elif die == 3:
                print("..tails")
            print("Current state: ", state)
        if state == 5 :
            flips += 1
            state = 7
         #   tm.sleep(0.3)
            print("Tossing coin:")
        #    tm.sleep(0.3)
            die = np.random.choice([4, 5], p=[0.5, 0.5])
            if die == 4:
                print("..heads")
            elif die == 5:
                print("..tails")
            print("Current state: ", state)
        if state == 6 :
            flips += 1
         #   tm.sleep(0.3)
            print("Tossing coin:")
         #   tm.sleep(0.3)
            state = np.random.choice([2, 7], p=[0.5, 0.5])
            if state == 2:
                print("..heads")
                die = 6
            elif state == 7:
                print("..tails")
            print("Current state: ", state)
    if 0<die<7 :
        print("\n\tCoin flips: ", flips)
        print("\n\tNumber of moves: ", die,"\n")
        return die
    else :
        return 1


def rollTwoDice():
    state = 0
    flips = 0
    dice = 0
    print("Rolling the dice!\nStarting from state", state)
    while (state != 34) and (dice == 0):
        if state == 0:
            print("Tossing coin:")
            state = np.random.choice([1, 2], p=[0.5, 0.5])
            if state == 1 :
                print("..heads")
            elif state == 2 :
                print("..tails")
            print("Current state: ", state)
            flips += 1
        if state == 1:
            print("Tossing coin:")
            state = np.random.choice([3, 4], p=[0.5, 0.5])
            if state == 3 :
                print("..heads")
            elif state == 4 :
                print("..tails")
            print("Current state: ", state)
            flips += 1
        if state == 2:
            print("Tossing coin:")
            state = np.random.choice([5, 6], p=[0.5, 0.5])
            flips += 1
            if state == 5 :
                print("..heads")
            elif state == 6 :
                print("..tails")
            print("Current state: ", state)
        if state == 3:
            print("Tossing coin:")
            state = np.random.choice([7, 34], p=[0.5, 0.5])
            flips += 1
            if state == 7:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 6
            print("Current state: ", state)
        if state == 4:
            print("Tossing coin:")
            state = np.random.choice([8, 34], p=[0.5, 0.5])
            flips += 1
            if state == 8:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 7
        if state == 5:
            print("Tossing coin:")
            state = np.random.choice([9, 34], p=[0.5, 0.5])
            flips += 1
            if state == 9:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 8
            print("Current state: ", state)
        if state == 6:
            print("Tossing coin:")
            state = np.random.choice([10,11], p=[0.5, 0.5])
            flips += 1
            if state == 10 :
                print("..heads")
            elif state == 11 :
                print("..tails")
            print("Current state: ", state)
        if state == 7:
            print("Tossing coin:")
            state = np.random.choice([12, 34], p=[0.5, 0.5])
            flips += 1
            if state == 12:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 4
            print("Current state: ", state)
        if state == 8:
            print("Tossing coin:")
            state = np.random.choice([13, 34], p=[0.5, 0.5])
            flips += 1
            if state == 13:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 5
            print("Current state: ", state)
        if state == 9:
            print("Tossing coin:")
            state = np.random.choice([14, 34], p=[0.5, 0.5])
            flips += 1
            if state == 14:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 9
            print("Current state: ", state)
        if state == 10:
            print("Tossing coin:")
            state = np.random.choice([15, 34], p=[0.5, 0.5])
            flips += 1
            if state == 15:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 10
            print("Current state: ", state)
        if state == 11:
            print("Tossing coin:")
            state = np.random.choice([16, 17], p=[0.5, 0.5])
            if state == 16 :
                print("..heads")
            elif state == 17 :
                print("..tails")
            print("Current state: ", state)
            flips += 1
        if state == 12:
            print("Tossing coin:")
            state = np.random.choice([18, 34], p=[0.5, 0.5])
            flips += 1
            if state == 18:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 3
            print("Current state: ", state)
        if state == 13:
            print("Tossing coin:")
            state = np.random.choice([19, 34], p=[0.5, 0.5])
            flips += 1
            if state == 19:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 5
            print("Current state: ", state)
        if state == 14:
            print("Tossing coin:")
            state = np.random.choice([20, 34], p=[0.5, 0.5])
            flips += 1
            if state == 20:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 7
            print("Current state: ", state)
        if state == 15:
            print("Tossing coin:")
            state = np.random.choice([21, 34], p=[0.5, 0.5])
            flips += 1
            if state == 21:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 9
            print("Current state: ", state)
        if state == 16:
            print("Tossing coin:")
            state = np.random.choice([22, 34], p=[0.5, 0.5])
            flips += 1
            if state == 22:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 11
            print("Current state: ", state)
        if state == 17:
            print("Tossing coin:")
            state = np.random.choice([23, 24], p=[0.5, 0.5])
            if state == 23 :
                print("..heads")
            elif state == 24 :
                print("..tails")
            print("Current state: ", state)
            flips += 1
        if state == 18:
            print("Tossing coin:")
            state = np.random.choice([34, 25], p=[0.5, 0.5])
            flips += 1
            if state == 34:
                print("..heads")
                dice = 2
            elif state == 25:
                print("..tails")
            print("Current state: ", state)
        if state == 19:
            print("Tossing coin:")
            state = np.random.choice([34, 26], p=[0.5, 0.5])
            flips += 1
            if state == 34:
                print("..heads")
                dice = 3
            elif state == 26:
                print("..tails")
            print("Current state: ", state)
        if state == 20:
            print("Tossing coin:")
            state = np.random.choice([34, 27], p=[0.5, 0.5])
            flips += 1
            if state == 34:
                print("..heads")
                dice = 4
            elif state == 27:
                print("..tails")
            print("Current state: ", state)
        if state == 21:
            print("Tossing coin:")
            dice = np.random.choice([5, 9], p=[0.5, 0.5])
            flips += 1
            if dice == 5:
                print("..heads")
            elif dice == 9:
                print("..tails")
            state = 34
            print("Current state: ", state)
        if state == 22:
            print("Tossing coin:")
            state = np.random.choice([28, 34], p=[0.5, 0.5])
            flips += 1
            if state == 28:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 10
            print("Current state: ", state)
        if state == 23:
            print("Tossing coin:")
            state = np.random.choice([29, 34], p=[0.5, 0.5])
            flips += 1
            if state == 29:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 11
            print("Current state: ", state)
        if state == 24:
            print("Tossing coin:")
            state = np.random.choice([30, 34], p=[0.5, 0.5])
            flips += 1
            if state == 30:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 12
            print("Current state: ", state)
        if state == 25:
            print("Tossing coin:")
            state = np.random.choice([1, 34], p=[0.5, 0.5])
            flips += 1
            if state == 1:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 2
            print("Current state: ", state)
        if state == 26:
            print("Tossing coin:")
            state = np.random.choice([31, 34], p=[0.5, 0.5])
            flips += 1
            if state == 31:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 3
            print("Current state: ", state)
        if state == 27:
            print("Tossing coin:")
            state = np.random.choice([34, 32], p=[0.5, 0.5])
            flips += 1
            if state == 34:
                print("..heads")
                dice = 6
            elif state == 32:
                print("..tails")
            print("Current state: ", state)
        if state == 28:
            print("Tossing coin:")
            dice = np.random.choice([7, 8], p=[0.5, 0.5])
            flips += 1
            if dice == 7:
                print("..heads")
            elif dice == 8:
                print("..tails")
            state = 34
            print("Current state: ", state)
        if state == 29:
            print("Tossing coin:")
            state = np.random.choice([33, 34], p=[0.5, 0.5])
            flips += 1
            if state == 33:
                print("..heads")
            elif state == 34:
                print("..tails")
                dice = 11
            print("Current state: ", state)
        if state == 30:
            print("Tossing coin:")
            state = np.random.choice([34, 2], p=[0.5, 0.5])
            flips += 1
            if state == 34:
                print("..heads")
                dice = 12
            elif state == 2:
                print("..tails")
            print("Current state: ", state)
        if state == 31:
            print("Tossing coin:")
            dice = np.random.choice([2, 4], p=[0.5, 0.5])
            state = 34
            flips += 1
            if dice == 2:
                print("..heads")
            elif dice == 4:
                print("..tails")
            print("Current state: ", state)
        if state == 32:
            print("Tossing coin:")
            dice = np.random.choice([6, 8], p=[0.5, 0.5])
            flips += 1
            if dice == 6:
                print("..heads")
            elif dice == 8:
                print("..tails")
            state = 34
            print("Current state: ", state)
        if state == 33:
            print("Tossing coin:")
            dice = np.random.choice([10, 12], p=[0.5, 0.5])
            flips += 1
            if dice == 10:
                print("..heads")
            elif dice == 12:
                print("..tails")
            state = 34
            print("Current state: ", state)
        if state == 34:
            break
    if 0<dice<13 :
        print("\n\tCoin flips: ", flips)
        print("\n\tNumber of moves: ", dice,"\n")
        return dice
    else :
        return 1

def print_matrix(xJ, yJ, xR, yR):
    for i in range(n):
        for j in range(n):
            if i == xR and j == yR:
                Matrix[i][j] = "R"
            elif i == xJ and j == yJ:
                Matrix[i][j] = "J"
            else:
                Matrix[i][j] = "*"
    for row in Matrix:
        print(' '.join([str(elem) for elem in row]))
    print('')


try:
    n = int(input("Choose the n size of the grid (n x n): "))
    nrOfDice = int(input("Number of dice to be rolled: (1/2): "))
    if n==5 :
        maxTimeUnits = 100
    else :
        maxTimeUnits = n*n + (79/18)*n*4
    print("The goal is to reach the destination with maximum", (2*n), "moves in maximum", maxTimeUnits, "time units.")
    tm.sleep(4)
    Matrix = [[0] * n for i in range(n)]
    for x in range(n):
        robotPos.append((n-1, x))
    for y in range(n-2, -1, -1):
        robotPos.append((y, n-1))
    v1 = 0
    deniedMoves = 0
    timeUnits = 0
    xR, yR = robotPos[v1][0], robotPos[v1][1]
    xJ, yJ = 0, n-1
    print_matrix(xJ, yJ, xR, yR)
    while not(xR == 0 and yR == n-1):
        if nrOfDice == 1:
            numberOfMoves = rollOneDie()
        elif nrOfDice == 2:
            numberOfMoves = rollTwoDice()
        else:
            print("Invalid number of dice")
            exit(3)
        print("Janitor will move", numberOfMoves, "times")
        for i in range(0, numberOfMoves):
            timeUnits += 1
            print("Move number:",i+1)
            randomPos = np.random.choice([0, 1, 2, 3], p=[0.25, 0.25, 0.25, 0.25])
            if randomPos == 0:  # left y--
                if (yJ - 1) < 0:
                    print("Janitor cannot move left, (Out of boundary)")
                elif (yJ - 1 == yR) and (xJ == xR):
                    print("Janitor cannot move left, (Occupied by Robot)")
                else:
                    yJ = yJ - 1
                    print("Janitor moved left")
                    print_matrix(xJ, yJ, xR, yR)
            if randomPos == 1:  # up x--
                if (xJ - 1) < 0:
                    print("Janitor cannot move up, (Out of boundary)")
                elif (xJ - 1 == xR) and (yJ == yR):
                    print("Janitor cannot move up, (Occupied by Robot)")
                else:
                    xJ = xJ - 1
                    print("Janitor moved up")
                    print_matrix(xJ, yJ, xR, yR)
            if randomPos == 2:  # right y++
                if (yJ + 1) >= n:
                    print("Janitor cannot move right, (Out of boundary)")
                elif (yJ + 1 == yR) and (xJ == xR):
                    print("Janitor cannot move right, (Occupied by Robot)")
                else:
                    yJ = yJ + 1
                    print("Janitor moved right")
                    print_matrix(xJ, yJ, xR, yR)
            if randomPos == 3:  # down x++
                if (xJ + 1) >= n:
                    print("Janitor cannot move down, (Out of boundary)")
                elif (xJ + 1 == xR) and (yJ == yR):
                    print("Janitor cannot move down, (Occupied by Robot)")
                else:
                    xJ = xJ + 1
                    print("Janitor moved down")
                    print_matrix(xJ, yJ, xR, yR)
        v1 += 1
        if xJ == robotPos[v1][0] and yJ == robotPos[v1][1]:
            v1 -= 1
            print("Robot cannot move, (Occupied by Janitor)")
            deniedMoves += 1
        xR, yR = robotPos[v1][0], robotPos[v1][1]
        timeUnits += 1
        print("Time units so far:", timeUnits)
        print("The robot is moving: ")
        print_matrix(xJ, yJ, xR, yR)
        if xR == 0 and yR == (n-1) :
            print("The robot reached the destination in", v1+deniedMoves, "moves having a delay of", deniedMoves,"moves")
            if (maxTimeUnits-timeUnits) < 0 :
                print("and", maxTimeUnits-timeUnits, "time units")
            if maxTimeUnits-timeUnits < 0 :
                print("Experiment failed. Timelimit exceeded")
            else :
                print("Experiment succeeded. Final time units:", timeUnits)
except ValueError:
    print("Not a number")