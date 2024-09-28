main.out: src/main.cpp
	g++ include/glad/src/glad.c src/shader.cpp src/main.cpp -lglfw -o main.out