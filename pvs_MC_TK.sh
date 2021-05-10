pvs-studio-analyzer trace -- g++ -std=c++17 -O2 main_mc.cpp -o main_mc -pthread
pvs-studio-analyzer analyze -o log
plog-converter -a GA:1,2,3 -t fullhtml -o tasks log