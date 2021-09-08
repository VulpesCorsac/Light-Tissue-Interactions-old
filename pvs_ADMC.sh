pvs-studio-analyzer trace -- g++ -std=c++17 -O2 main_admc.cpp -o main_admc -pthread
pvs-studio-analyzer analyze -o log
plog-converter -a GA:1,2,3 -t fullhtml -o tasks log