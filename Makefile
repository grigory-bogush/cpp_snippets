

run_thread_pool:
	mkdir -p build && g++ -std=c++17 -pthread -o build/thread_pool thread_pool.cpp && ./build/thread_pool

run_render_triangle_gl:
	mkdir -p build && g++ -std=c++17 -o build/gl_render_triangle gl_render_triangle.cpp -lGL -lGLU -lglut && ./build/gl_render_triangle
