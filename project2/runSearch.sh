clear && make search -s && ./build/search -i datasets/nasdaq2017_LQ.csv -q test_query_120 -o outputSearch.txt -k 15 -L 7 -M 1000 -probes 2 -algorithm LSH -metric discrete