g++ test_main.cpp /usr/local/lib/libglog.a -I/usr/local/include -std=c++11 -DDEBUG -lpthread -o sample
g++ test_main.cpp -L/usr/local/lib -lglog -I/usr/local/include -std=c++11 -DDEBUG -lpthread -o sample
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARU_PATH
