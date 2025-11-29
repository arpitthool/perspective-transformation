#include <iostream>
#include <glad/glad.h>  // Include GLAD before GLFW (GLAD must be included before any OpenGL headers)
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int main() {
    
    glfwInit(); // Initialize GLFW

    glfwTerminate(); // Clean up GLFW when the program is done
    
    return 0;
}