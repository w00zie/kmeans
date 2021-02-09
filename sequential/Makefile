all: clean release

debug: seq.cpp
	g++ -std=c++17 -g -Wall -Wextra -pedantic -O0 seq.cpp -o kmeans_seq_db

profile: seq.cpp
	g++ -std=c++17 -g -p -Wall -Wextra -pedantic -O0 seq.cpp -o kmeans_seq_prof

release: seq.cpp
	g++ -std=c++17 -Wall -Wextra -pedantic -O3 seq.cpp -o kmeans_seq

clean:
	rm -f kmeans_seq*
