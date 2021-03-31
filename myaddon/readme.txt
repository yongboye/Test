export LD_LIBRARY_PATH=./src/statics/Hardware/linux/arm:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
valgrind --tool=memcheck --leak-check=full node index.js