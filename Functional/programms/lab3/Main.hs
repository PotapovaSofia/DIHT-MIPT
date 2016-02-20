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

-- почтовый адрес
email = "sofiapotapova9@mail.ru"

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


newtype JSON = Object [(String, JSON)]

-- добавим сответствующие классы типов для JSON
instance Show JSON where
  show = stringify
instance Read JSON where
  readsPrec _ x = parse x

parse :: ReadS (JSON)
parse "{}" = [(Object [], "")]

lab3 (Object list) = 0

stringify (Object list) = "{}"

-- вариант с монадой IO
generateIO :: IO JSON
generateIO = do
  num <- randomRIO (1, 2) :: IO Int
  let json = case num of
               1 -> Object [];
               2 -> Object [("io", Object [])]
  return json

-- чистый вариант с генератором, заключённым в состояние
-- мы храним в состоянии генератор, каждый раз используя
-- его, возвращаем в состояние новый

type GeneratorState = State StdGen

generate' :: GeneratorState JSON
generate' = do
  gen <- get
  let (num, newGen) = randomR (1, 2) gen :: (Int, StdGen)
  let json = case num of
               1 -> Object [];
               2 -> Object [("pure", Object [])]
  put newGen
  return json

generate :: JSON
generate = evalState generate' (mkStdGen 0)