all:
	ghc ./1.hs -O2 -threaded -XBangPatterns

run: all
	./1 179.ru / +RTS -N2

clean:
	rm ./1
	rm ./1.hi
	rm ./1.o	
