#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


float vertices[] = {
	-0.5f, -0.5f, 0.0f, // left  
	 0.5f, -0.5f, 0.0f, // right 
	 0.0f,  0.5f, 0.0f  // top   
};
unsigned int indices[] = {
	0,1,2
};

const char simpleVertexShaderPath[] = "./resources/shaders/SimpleVertexShader.vert";
const char simpleFragmentShaderPath[] = "./resources/shaders/SimpleFragmentShader.frag";

int main() {
	std::cout << "GL Tuotrial Start" << std::endl;

	std::cout << LEapsGL::ReadFile(simpleVertexShaderPath) << "\n";
	std::cout << LEapsGL::ReadFile(simpleFragmentShaderPath);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_WIDTH, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "window create error" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 현재 쓰레드의 주 컨텍스트로 지정하겠다고 알려줌
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader

	//★★★ 일시 함수 호출 안되게 생성자 호출 후 chaning하기
	auto shaderProgramObject = LEapsGL::ShaderProgram();
	shaderProgramObject.CompileFromFile(  GL_VERTEX_SHADER, simpleVertexShaderPath)
		.CompileFromFile(GL_FRAGMENT_SHADER, simpleFragmentShaderPath)
		.MakeProgram();
	shaderProgramObject.DeleteShaderAll();

	//	shaderProgramObject.setID(shaderProgram);
	//auto shaderProgramObject = LEapsGL::ShaderProgram();
	//shaderProgramObject.setID(shaderProgram);
	
	

	// vertex buffer objet
	// vertex array obbject
	unsigned int VBO, VAO, EBO;
	// 현재 사용가능한 유니크한 buffer name을 리턴해 줌
	// 실제 메모리를 할당하지 않음
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Vetex array를 바인드한다. 현재 컨텍스트에서 Bind wvertex array는 해당 VAO에서 동작하게 된다.
	glBindVertexArray(VAO);

	// 사용할 buffer object를 VBO id로 설정한다.
	// 현재 사용할 VBO 객체의 아이디를 GL_ARRAY_BUFFER의 ARRAY_BUFFER state
	// GL_ARRAY_BUFFER는 State의 종류이며, VBO는 그 스테이트에 할당할 내 opengl 객체의 id를 의미한다.
	// 바인드는 아무일도 하지 않는다. 단지 현재 state에 id를 채원허어준다.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// CPU to GPU로 옮긴다. 데이터를 할당하고 복사한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// gl vertex attribute 초기화. VAO에 이미 바인드 되어있기 때문에
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// unbind 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	float deltaTime = 0;

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // state setting
		glClear(GL_COLOR_BUFFER_BIT); // stage using

		// DRAW
		glUseProgram(shaderProgramObject.getID());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		deltaTime += 0.001;
	}

	glfwTerminate();
	return 0;
}