# SquareSumChains

Solver for the Square Sum Chains problem describe in this Numberphile video: https://youtu.be/G1m7goLCJDY.

The program reads the order of the chain to be created on standard input, then builds a graph, an edge between 2 vertices representing a valid square sum of 2 values. Then a DFS is performed on the graph, at every node the current value is marked as used, then the list of their valid neighbours is sorted by ascending valid edges count and tried in this order. The search continues until an hamiltonian path is found (all values visited once).

An explicit stack is implemented to manage high level of recursions. It takes 9 minutes to find a square sum chain of length 500000 on my computer (Windows 7 Professional, Intel Core I3 3.50GHz).
