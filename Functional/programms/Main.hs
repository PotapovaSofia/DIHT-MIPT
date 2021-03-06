import Data.List

flipVertical = map reverse
flipHorizontal = reverse
rotate90counterClockWise = transpose $ reverse
rotate90clockWise = reverse $ transpose
rotate180 = rotate90clockWise $ rotate90clockWise
{-
art = 
[
['@','.','.','@','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.']
['.','.','@','.','.','@','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.']
['.','.','.','.','@','.','.','@','.','.','@','@','@','@','@','@','@','@','@','@','@','@']
['.','.','.','.','.','.','@','.','.','@','.','.','.','.','.','.','.','.','.','.','.','.']
['.','.','.','.','.','.','.','.','@','.','.','@','.','.','.','.','.','.','.','.','.','.']
['.','.','.','.','.','.','@','.','.','@','.','.','.','@','.','.','@','@','@','@','@','@']
['.','.','.','.','@','.','.','@','.','.','.','.','.','.','.','@','.','.','.','.','.','.']
['.','.','@','.','.','@','.','.','.','.','.','.','.','.','.','.','.','@','.','.','.','.']
['@','.','.','@','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','@','.','.']
]
-}

art = [[1,2],[3,4]]
main = print $ rotate180 art
