{-# LANGUAGE OverloadedStrings #-}

module Lab1 (fTailor, tailor, tailorA, iter, newton, dichotomy, main) where

import System.IO
import System.Environment
import System.Directory
import Control.Monad
import Network.HTTP.Conduit
import qualified Data.ByteString.Lazy as L
import qualified Data.ByteString.Char8 as C
import Network (withSocketsDo)

-- почтовый адрес
email = "sofiapotapova95@mail.ru"
-- общий тип для возвращаемых вашими функциями значений, где первая часть кортежа это само значение функции, вторая - кол-во операций
type Result = (Float, Int)

fTailor x = (x*(3-x))/((1-x)**3)  -- функция, которую раскладываем
delta = 1e-4
(n, a, b) = (20, 0.0, 0.5) -- интервал

diff :: Float -> Int -> Float
diff x intN = 
	let n = fromIntegral intN
	in x * (n + 3) * (n + 1) / (n**2 + 2 * n)

tailor :: Float -> Int -> Float -> Float -> Result
tailor x n prev sum =
	let newItem = (*) prev $ diff x n
	in if abs newItem < delta then (sum + newItem, n) else tailor x (n + 1) newItem (sum + newItem)

tailorA :: Float -> Int -> Float -> Result
tailorA x intN sum =
	let
		n = fromIntegral intN
		newItem = n * (n + 2) * (x ** n)
	in if abs newItem < delta then (sum + newItem, intN) else tailorA x (intN + 1) (sum + newItem)

printTailor = mapM_ putStrLn $ 
	map 
		(\ x -> 
			let ((firstRes, firstCou), (secondRes, secondCou)) = (tailor x 1 (3 * x) (3 * x), tailorA x 0 0) 
			in show x ++ "\t" ++ show firstRes ++ "\t" ++ show firstCou ++ "\t" ++ show secondRes ++ "\t" ++ show secondCou ++ "\t" ++ show (fTailor x)) 
		[a, a + (b - a) / n .. b]

-- *** Вторая часть

(a1, b1) = (1, 3)
fSolve1 = \x -> 3 * (log x)**2 + 6 * log x - 5 -- функция, решение которой ищем

(a2, b2) = (2, 3)
fSolve2 = \x -> 0.6 * 3**x - 2.3 * x - 3

(a3, b3) = (2, 3)
fSolve3 = \x -> x**2 - log (1 + x) - 3

iterA :: (Float -> Float) -> Float -> Int -> Result
iterA fx prev n = 
	let x_ = fx prev
	in if abs (x_ - prev) < delta then (x_, n + 1) else iterA fx x_ (n + 1)
	
alpha = 1e-3
iter :: (Float -> Float) -> Float -> Float -> Result
iter f a b =
	let k = \x -> (f (x + alpha) - f x) / alpha
	    lambda = (/) 1 $ k ((a + b) / 2)
	    fx = \x-> (x - lambda * (f x))
	in iterA fx a 0

newton :: (Float -> Float) -> Float -> Float -> Result
newton f a b = 
	let k = \x -> (f (x + alpha) - f x) / alpha
	    fx = \prev -> prev - (f prev) / (k prev)
	in iterA fx a 0

dichotomy f a b =    
	--для функций с аккумулятором удобно ставить его в начало
	let dichotomyA i f a b = 
		if abs (a - b) < delta
			then ((a + b) / 2, i)
			else 
				let f1 = f a 
				    f3 = f ((a + b) / 2)
				in if f1 * f3 < 0
					then dichotomyA (i + 1) f a ((a + b) / 2) 
					else dichotomyA (i + 1) f ((a + b) / 2) b
	in dichotomyA 0 f a b-- чтобы воспользоваться каррированием

printSolve =
	mapM_ putStrLn $ 
	(map (\f -> 
		let (root, iterNum) = f fSolve1 a1 b1
		in show root ++ "\t" ++ show iterNum) [iter, newton, dichotomy]) ++ 
	["1.8832\t\n"] ++
	(map (\f -> 
		let (root, iterNum) = f fSolve2 a2 b2
		in show root ++ "\t" ++ show iterNum) [iter, newton, dichotomy]) ++ 
	["2.4200\t\n"] ++
	(map (\f -> 
		let (root, iterNum) = f fSolve3 a3 b3
		in show root ++ "\t" ++ show iterNum) [iter, newton, dichotomy]) ++ 
	["2.0267\t"]

main = do	
	withSocketsDo $ do
	dir <- getCurrentDirectory
	initReq <- parseUrl "http://mipt.eu01.aws.af.cm/lab1"
	handle <- openFile (dir ++ "/Lab1.hs") ReadMode
	hSetEncoding handle utf8_bom
	content <- hGetContents handle
	let req = urlEncodedBody [("email", email), ("content", C.pack content)] $ initReq { method = "POST" }
	response <- withManager $ httpLbs req
	hClose handle
	L.putStrLn $ responseBody response
