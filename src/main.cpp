#include "config.h" 
#include "triangle_mesh.h"

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

unsigned int make_module(const std::string& filepath, unsigned int module_type);

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

    unsigned int shader = make_shader(
		"../src/shaders/vertex.txt", 
		"../src/shaders/fragment.txt"
	);

    TriangleMesh triangleMesh = TriangleMesh();

    while (!glfwWindowShouldClose(window)) {  // Main loop: continue while window is not closed
        
        glfwPollEvents();  // Process all pending window events

        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
        glUseProgram(shader); // Use the shader program

        triangleMesh.draw();

        glfwSwapBuffers(window); 
    }

    glDeleteProgram(shader); // Delete the shader program

    glfwTerminate();  // Clean up and terminate GLFW library

    return 0;  // Exit program with success code
}


unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line)) {
		//std::cout << line << std::endl;
		bufferedLines << line << '\n';
	}

	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, 
                NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" 
            << errorLog << std::endl;
	}

	return shaderModule;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) { // Return the shader program ID

    //To  store all the shader modules
    std::vector<unsigned int> modules;

    //Add a vertex shader module
    modules.push_back(make_module(vertex_filepath, 
                GL_VERTEX_SHADER));

    //Add a fragment shader module
    modules.push_back(make_module(fragment_filepath, 
                GL_FRAGMENT_SHADER));

    //Attach all the modules then link the program
    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    //Check the linking worked
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, 
                NULL, errorLog);
        std::cout << "Shader linking error:\n" << errorLog << '\n';
    }

    //Modules are now unneeded and can be freed
    for (unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shader;
}