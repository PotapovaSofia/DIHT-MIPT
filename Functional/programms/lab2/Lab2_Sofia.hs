{-# LANGUAGE OverloadedStrings #-}

import System.IO
import System.Environment
import System.Directory
import Control.Monad
import Data.Text.Encoding
import qualified Data.List as L
import Network.HTTP.Conduit
import qualified Data.Text as T
import Text.HTML.DOM (parseLBS)
import Text.XML.Cursor (Cursor, attributeIs, attribute, content, element, fromDocument, child, ($//), (&|), (&//), (&/), (>=>)) 
import Network (withSocketsDo)
import Data.Maybe

email = ""

cursorFor :: String -> IO Cursor
cursorFor u = do
     page <- withSocketsDo $ simpleHttp u
     return $ fromDocument $ parseLBS page

execute_api_query query = do
    cursor <- cursorFor $ "http://api.vk.com/method/groups.getMembers.xml?group_id=" ++ query
    return $ cursor $// element "uid" &| T.concat . content

lab2 = do
    list <- liftM2 (++) (execute_api_query "fivtmipt") (execute_api_query "fivtmipt&offset=1000")
    kvn_list <- liftM2 (++) (execute_api_query "ligakvnmfti") (execute_api_query "ligakvnmfti&offset=1000")
    return list `L.intersect` kvn_list

--main :: IO()

--main = do


{-main :: IO()
main = withSocketsDo $ do
  nodes <- lab2
  dir <- getCurrentDirectory
  initReq <- parseUrl "http://mipt.eu01.aws.af.cm/lab2"
  handle <- openFile (dir ++ "/Lab2.hs") ReadMode
  hSetEncoding handle utf8_bom
  content <- hGetContents handle
  let req = urlEncodedBody [("email", email), ("result", encodeUtf8 $ T.pack $ show nodes), ("content", encodeUtf8 $ T.pack content) ] $ initReq { method = "POST" }
  response <- withManager $ httpLbs req
  hClose handle
  L.putStrLn $ responseBody response
  -}
