{-# LANGUAGE OverloadedStrings #-}

import Control.Applicative hiding ((<|>), many)
import System.IO
import System.Environment
import System.Directory
import System.Random
import Control.Monad
import Control.Monad.State
import Control.Concurrent

import Network.HTTP.Conduit
import Network (withSocketsDo)
import Data.Maybe
import Data.Either

import Data.Text.Encoding
import Data.Ord (comparing)
import Data.Function (on)
import Data.List.Split (chunksOf)
import GHC.Conc (numCapabilities)

import qualified Data.Text as T
import qualified Data.Map as M
import qualified Data.Set as S
import qualified Data.List as L
import qualified Data.Graph as G
import qualified Control.Exception as X
import qualified Data.ByteString.Lazy.Char8 as BLC
import qualified Data.ByteString.Lazy as BL
import Text.HTML.DOM (parseLBS)
import Text.XML.Cursor (Cursor, attribute, attributeIs, content, element, fromDocument, child, ($//), (&|), (&//), (&/), (>=>)) 
import Network.URI
import Numeric
import Text.Parsec

email = "sofiapotapova95@mail.ru"

data Node = Null
            | Boolean Bool
            | String String
            | Number Integer
            | Array [Node]
            | Object (M.Map String Node) deriving (Eq, Ord)

instance Show Node where
    show Null = "Null"
    show (Object m) = "{" ++ L.intercalate "," (map (\ (x,y) -> show x ++ ":" ++ show y) $ M.toList m) ++ "}"
    show (Boolean b) = show b
    show (String s) = show s
    show (Number n) = show n
    show (Array l) = show l

decode = attempt . parse input "JSON"
    where attempt (Right (success, "")) = success
          attempt (Right (_, trail)) = error $ "trailing " ++ show trail
          attempt (Left failure) = error $ "invalid " ++ show failure

input = (,) <$> value <*> getInput where
    value = spaced (null' <|> boolean <|> string' <|> array <|> object <|> number)
    null'    = Null <$  string "Null"
    boolean = Boolean <$> (True <$ string "True" <|> False <$ string "False")
    number  = Number  <$> (read <$> (many1 $ oneOf "0123456789"))
    stringn = between (char '"') (char '"') (many $ noneOf "\"")
    string' = String  <$> stringn
    array   = Array   <$> between (char '[') (char ']') (listOf value)
    object  = Object  <$> between (char '{') (char '}') (M.fromList <$> listOf pair)
    pair = (,) <$> (spaced stringn) <*> (spaced (char ':') *> value)
    listOf  = (`sepBy` char ',')
    spaced = between ws ws where ws = many (oneOf " \t\r\n")

instance Read Node where
    readsPrec _ x = [(decode x, "")]

randInt :: IO Integer
randInt = abs <$> randomIO

genCase num len
    | num < 12 = return Null
    | num < 24 = return $ Boolean False
    | num < 36 = return $ Boolean True
    | num < 48 = Number <$> randInt
    | num < 60 = String <$> (show <$> randInt)
    | num < 80 = Array <$> replicateM len generateIO
    | otherwise = Object <$> (M.fromList
            <$> replicateM len ((,) <$> (show <$> randInt) <*> generateIO))


generateIO = do
    num <- (flip mod 100) <$> randInt
    len <- randomRIO (0, 5) :: IO Int
    genCase num len


vertexCnt (Boolean _) = 0
vertexCnt (Null) = 0
vertexCnt (Number _) = 0
vertexCnt (String _) = 0
vertexCnt (Array l) = 1 + (sum $ map vertexCnt l)
vertexCnt (Object m) = 1 + (sum $ M.elems $ M.map vertexCnt m)


tests = [("True", 0),
    ("189", 0),
    ("[True, False, Null]", 1),
    ("{\"sofia\":[74, [204]], \"dima\":[True, Null, [52,159]]}", 5),
    ("[[[[[1,2,3], [3,10]]]], [[[[9]]]]]", 10) ]

runTests = and $ map (\(test, res) -> res == (vertexCnt $ decode test)) tests

runParserTest = do
    test <- generateIO
    return $ test == (read $ show test)
main :: IO()
main = withSocketsDo $ do
  dir <- getCurrentDirectory
  initReq <- parseUrl "http://mipt.eu01.aws.af.cm/lab3"
  handle <- openFile (dir ++ "/Lab3.hs") ReadMode
  hSetEncoding handle utf8_bom
  content <- hGetContents handle
  let req = urlEncodedBody [("email", email), ("content", encodeUtf8 $ T.pack content) ] $ initReq { method = "POST" }
  response <- withManager $ httpLbs req
  hClose handle
  BLC.putStrLn $ responseBody response