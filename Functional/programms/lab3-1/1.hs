{-# LANGUAGE OverloadedStrings #-}

import System.IO
import System.Environment
import System.Directory
import Control.Monad
import Data.Text.Encoding
import Network.HTTP.Conduit
import qualified Data.Text as T
import qualified Data.Map as M
import qualified Data.Set as S
import qualified Data.List as L
import qualified Data.Graph as G
import GHC.Conc (numCapabilities)
import Control.Concurrent
import Text.HTML.DOM (parseLBS)
import Text.XML.Cursor (Cursor, attribute, attributeIs, content, element, fromDocument, child, ($//), (&|), (&//), (&/), (>=>)) 
import Network (withSocketsDo)
import Network.URI
import Data.Maybe
import Data.List.Split (chunksOf)

get_domain_by_uri uri = 
	let x = T.pack $ uriRegName $ fromJust $ uriAuthority uri {-до домена-}
    	in if "www." `T.isPrefixOf` x then T.drop 4 x else x

has_domain uri = not $ isNothing $ uriAuthority uri {-проверка на корректную строку-}

get_page_by_uri uri = T.pack $ uriPath uri ++ (uriQuery uri) {-после слеша, адрес страницы-}

cursorFor :: T.Text -> IO Cursor
cursorFor u = do
     page <- withSocketsDo $ simpleHttp $ T.unpack u
     return $ fromDocument $ parseLBS page

get_linked_pages domain page = do {-возвраает неповторяющиеся ссылки с нашей страницы в наш домен-}
    url <- return $ T.concat ["http://", domain, page]
    uri <- return $ fromJust $ parseURIReference {-из строки в юри-} $ T.unpack  {-из текста в строку-} url
    cursor <- cursorFor url
    return $ L.nub {-выкидывает повторяющиеся из списка-} $ filter (\x -> x /= "" && x /= page) {-удаляем пустые ссылки и на саму себя-} $ map get_page_by_uri
        $ filter (\ x -> get_domain_by_uri x == domain) $ filter has_domain
        $ map (`relativeTo` uri {-преобразует относительную ссылку к абсолютной-}) $ catMaybes $ map  (parseURIReference . T.unpack)
        $ cursor $//{-применяет все ко всем вершинам дерева-} element "a" {-ищем теги а-} &| T.concat . (attribute {-принимает название аттрибута и возвр массив текстов-}"href")
        {-массив предполагаемых тегов-}
nThreads = 10

build_website_graph domain queue graph = do {-граф - мэп: ключи - адрес, значение - массив адресов всех страниц, доступных с данной-}
    var <- newEmptyMVar
    queue_parts <- return $ chunksOf ((nThreads - 1 + length queue) `div` nThreads) queue
    
    let get_edges_for_list list = mapM (\y -> (get_linked_pages domain y) >>= return . (\a -> (y,a))) list {-список пар (адрес, список доступных адресов)-}
        in mapM_ (\x -> forkIO $ (get_edges_for_list x) >>= putMVar var) queue_parts
    
    new_edges <- replicateM (length queue_parts) (takeMVar var) >>= return . M.fromList . concat
    new_graph <- return $ M.union graph new_edges
    new_queue <- return $ filter (\x -> not $ M.member x new_graph) $ L.nub $ concat $ M.elems new_edges
    if new_queue == [] then return new_graph else build_website_graph domain new_queue new_graph


spider :: T.Text -> T.Text -> IO (M.Map T.Text [T.Text])
spider domain page = build_website_graph domain [page] M.empty {-упрощение синтаксиса-}

transpose_map mp = foldl (\m v -> M.insertWith (++) v [] m) {-транспонирует граф-}
        (M.foldWithKey (\k v r ->
            foldl (\nr nv -> M.insertWith (++) nv  [k] nr) r v) M.empty mp)
        $ M.keys mp

d :: Float
d = 0.85

update_page_rank :: M.Map T.Text Float -> M.Map T.Text Float -> [T.Text] -> T.Text -> Float
update_page_rank out_degrees old_rank edges vertex = (1 - d)/(fromIntegral $ M.size out_degrees) + d * (sum [ (old_rank M.! x) / (out_degrees M.! x) | x <- edges]) +
        if out_degrees M.! vertex == 0 then d * (old_rank M.! vertex) else 0

build_page_rank it graph = 
    let emptyMap = M.fromSet (const (1.0 / (fromIntegral $ M.size graph))) $ M.keysSet graph {-список (вершинаб начальный п-р = 1/кол-во вершин -}
        transposed_graph = transpose_map graph
        vertex_out_degrees = M.map (fromIntegral.length) graph {-для каждой вершины храним ее степень-}
    in foldl (\m _ -> M.mapWithKey (\k _ -> update_page_rank vertex_out_degrees m (transposed_graph M.! k) k) m) emptyMap [1..it]
    {-it раз перессчитываем пэйдж ранг-}
    
main = do
    args <- getArgs
    putStrLn $ show  args
    res <- spider (T.pack $ head args) (T.pack $ last args)
    putStrLn $ show $ build_page_rank 20 res

