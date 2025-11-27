#include "config.h" 

int main() {

    GLFWwindow* window;  // Declare a pointer to a GLFW window object

    if (!glfwInit()) {  // Initialize GLFW library, check if initialization failed
        std::cout << "GLFW init failed\n"; 
        return -1;
    }
     
    window = glfwCreateWindow(800, 600, "my window", NULL, NULL);  // Create a window with width 800, height 600, and title "my window"

    glfwMakeContextCurrent(window); // Make the window's context current
 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Initialize GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f); // Set the clear color

    while (!glfwWindowShouldClose(window)) {  // Main loop: continue while window is not closed
        
        glfwPollEvents();  // Process all pending window events

        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        glfwSwapBuffers(window); 
    }

    glfwTerminate();  // Clean up and terminate GLFW library

    return 0;  // Exit program with success code
}