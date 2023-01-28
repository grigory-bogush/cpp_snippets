

run_thread_pool:
	mkdir -p build && g++ -std=c++17 -pthread -o build/thread_pool thread_pool.cpp && ./build/thread_pool
