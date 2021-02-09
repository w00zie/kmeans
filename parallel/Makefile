all: clean release 

debug: par.cpp
	g++ -std=c++17 -g -Wall -Wextra -pedantic -O0 -pthread par.cpp -o kmeans_par_db

release: par.cpp
	g++ -std=c++17 -Wall -Wextra -pedantic -O3 -pthread par.cpp -o kmeans_par

clean:
	rm -f kmeans_par*