The management operations of an order statistics tree:
● BUILD_TREE(n)
	○ builds a balanced BST containing the keys 1,2,...n (hint: use a divide and conquer
	approach)
	○ make sure you initialize the size field in each tree node
● OS­SELECT(tree, i)
	○ selects the element with the ith smallest key
● OS­DELETE(tree, i)
	○ you may use the deletion from a BST, without increasing the height of the tree
	(why don’t you need to rebalance the tree?)
	○ keep the size information consistent after subsequent deletes
	○ there are several alternatives to update the size field without increasing the
	complexity of the algorithm (it is up to you to figure this out).
