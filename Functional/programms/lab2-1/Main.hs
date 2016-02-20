{-# LANGUAGE OverloadedStrings #-}

import qualified Data.Text as T
import qualified Data.Map as M
import Data.List
import System.IO
import Data.Ord
import Control.Concurrent
import Control.Monad


getLines :: T.Text -> [T.Text]
getLines text = filter (T.isPrefixOf "1. ") $ T.splitOn "\r\n" text

getEvents :: T.Text -> [T.Text]
getEvents line = 
	let lst = map T.words $ T.splitOn ". " line
	in concat ((map init $ init lst) ++ [last lst])

getGames :: T.Text -> [T.Text] -> [[T.Text]]
getGames str lines = map (getEvents . T.strip . head . (T.splitOn "{")) $ filter (T.isSuffixOf str) lines

getPrefix games = 
	let gamePrefix game = [(a, 1) | a <- tail . inits $ game]
	in M.fromListWith (+) $ concat $ map gamePrefix games

getLongestDebute debutes = maximumBy (comparing length) $ M.keys $ M.filter (>1) $ debutes

getMaxProbability win_g games = maximumBy (comparing snd) [(a, (win_g M.! a) / (games M.! a)) | a <- M.keys win_g, games M.! a > 1]

getWorstStep games debutes md = 
	let prob x = (M.findWithDefault 0 x games) / (M.findWithDefault 1 x debutes)
	in maximumBy (comparing snd) [(a, (prob $ init a) - prob a) | a <- M.keys games, (length a) `mod` 2 == md, length a > 0, debutes M.! a > 1]

main = do
	content <- readFile "1.txt" >>= (return . getLines . T.pack)
	[white, black, draw] <- return [getPrefix $ getGames "1-0" content, getPrefix $ getGames "0-1" content, getPrefix $ getGames "1/2-1/2" content]
	debutes <- return $ M.unionsWith (+) [white, black, draw]
	mVar <- newEmptyMVar
	forkIO $ putMVar mVar $ T.unpack "Longest Debute:\n" ++ (show $ getLongestDebute debutes)
	forkIO $ putMVar mVar $ T.unpack "Best debute for white:\n" ++ (show $ getMaxProbability white debutes)
	forkIO $ putMVar mVar $ T.unpack "Best debute for black:\n" ++ (show $ getMaxProbability black debutes)
	forkIO $ putMVar mVar $ T.unpack "Worst step for white:\n" ++ (show $ getWorstStep white debutes 1)
	forkIO $ putMVar mVar $ T.unpack "Worst step for black:\n" ++ (show $ getWorstStep black debutes 0)
	replicateM_ 5 (takeMVar mVar >>= putStrLn)
	return()		
