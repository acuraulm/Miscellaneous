Disjoint sets and Kruskal`s algorithm.
● MAKE_SET (x)
	
● UNION (x, y)
	○ makes the union between the set that contains the element x and the set that
	contains the element y
	○ the heuristic union by rank takes into account the height of the two trees so as to
	make the union

● FIND_SET (x)
	○ searches for the set that contains the element x
	○ the heuristic path compression links all nodes that were found on the path to x to
	the root node
