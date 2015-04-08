{-# LANGUAGE OverloadedStrings #-}

import Control.Monad
import Network.HTTP.Conduit
import Data.Text.Encoding
import qualified Data.ByteString.Lazy as L
import qualified Data.ByteString.Char8 as C
import Network (withSocketsDo)

(email, name) = ("sofiapotapova95@mail.ru", encodeUtf8 "Potapova S. G.") -- адрес почты и фамилия с инициалами

pascal :: Int -> Int -> Int
pascal k n = product [n-k+1 .. n] `div` product [1 .. k]

printIt :: Int -> C.ByteString
printIt n = C.pack $ show $ [pascal y x | x <- [0..n], y <- [0..x]]

main :: IO()
main =
  withSocketsDo $ do
  initReq <- parseUrl "http://mipt.eu01.aws.af.cm/lab0"
  let req = urlEncodedBody [("email", email), ("name", name), ("lang", "haskell"), ("content", printIt 20)] $ initReq { method = "POST" }
  response <- withManager $ httpLbs req
  L.putStr $ responseBody response
