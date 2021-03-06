module Main where

import Data.List  
import Data.List.Split (splitOneOf)
import Data.String
import Data.Map (toList, fromListWith)
import Data.Char

type Text       = [Char]
type Sentences  = [[Char]]
type Sentence   = [Char]
type Words      = [[Char]]
type Name       = [Char]
type Count      = Int

sentences :: Text -> Sentences
sentences text = splitOneOf ".!?" text

wordsInText :: Text -> Words
wordsInText text = splitOneOf ".,;!?-: " text

wordsInSentence :: Sentence -> Words
wordsInSentence sentence = splitOneOf ",;:- " sentence

filterNames :: Words -> Words
filterNames words =
    let lowWords = filter (\word -> (toUpper (head word) /= (head word))) words
    in filter (\word -> toUpper (head word) == (head word) && (map toLower word) `notElem` lowWords) words

{-
 -Delete only capsed words from list of names.
 -}
names :: Text -> [Name]
names = filterNames .  nub .  filter (not . null) . wordsInText

{-
 -In each sentences finds pairs of names.
 -}
namePairs :: Text -> [(Name, Name)]
namePairs text = 
    concat $ map (\sentence ->
                        let namesInSentence = intersect (wordsInSentence sentence) (names text)
                        in [(a, b) | a <- namesInSentence,
                                             b <- namesInSentence,
                                             a < b]) (sentences text)

{-
 -Сonsiders the number of matching pairs.
 -}
nameRelationships :: Text -> [((Name, Name), Count)]
nameRelationships text = 
    let pairs = namePairs text
    in toList $ fromListWith (+) [(p, 1) | p <- pairs]

printResult lst = putStrLn ( intercalate "\n" (map (\((a, b), c) -> a ++ " " ++ b ++ " " ++ (show c)) lst)) 

main::IO()
main = do  
    text <- readFile "Tale.txt"  
    printResult $ sortBy (\a b -> compare (snd b) (snd a)) $ nameRelationships text


