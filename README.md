Author: Ryan Phung
Date: April 14, 2022

** Using recursive backtracking to schedule works given an availability matrix (described below)
** Constraints: 
    -Workers cannot work twice in a day (have 2 shifts on the same day)
    -Workers cannot exceed the max number of shifts allowed (given by user)

            W   W   W   W
            O   O   O   O
            R   R   R   R
            K   K   K   K
            E   E   E   E
            R   R   R   R
            0   1   2   3

            |   |   |   |
            |   |   |   |
            V   V   V   V

Day 0 -->   1   1   1   1   
Day 1 -->   1   0   1   0   
Day 2 -->   1   1   0   1
Day 3 -->   1   0   0   1

** 1 = available to work, 0 = unavailable to work

