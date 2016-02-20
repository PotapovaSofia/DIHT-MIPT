 Tree = Nil | Node a (Tree a) (Tree a)

oktree Nil = True
oktree (Node _ a b)
	| (a == Nil) && (b == Nil) = True
	| (a /= Nil) && (b /= Nil) = oktree a && oktree b
	| otherwise = False

t = Node 1 (Nil) (Nil) 
