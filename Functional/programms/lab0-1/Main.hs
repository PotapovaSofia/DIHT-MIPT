import Data.List

flipV = map reverse
flipH = reverse
rotateHalfPICounterClockWise = transpose . reverse
rotateHalfPIClockWise = reverse . transpose
rotatePI = rotateHalfPIClockWise . rotateHalfPIClockWise
stack = (++)
glue = zipWith (++)
inverse = map $ map $ \c -> if c == '.' then '@' else '.'

art  = ["@..@..................",
        "..@..@................",
        "....@..@..@@@@@@@@@@@@",
        "......@..@............",
        "........@..@..........",
        "......@..@...@..@@@@@@",
        "....@..@.......@......",
        "..@..@...........@....",
        "@..@...............@.."]
out lst = if null lst then return()
	else do
		print $ head lst
		out $ tail lst

printIm = putStrLn . intercalate "\n"

main = out $ rotateHalfPIClockWise art
--printIm $ glue art $ inverse art
