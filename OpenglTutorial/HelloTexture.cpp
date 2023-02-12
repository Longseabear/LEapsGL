#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ShaderManager.h>
#include <Color.h>
#include <Texture2D.h>
#include <Image.h>
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
	// ��ġ              // �÷�             // �ؽ�ó ��ǥ
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ���� ���
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ���� �ϴ�
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ���� �ϴ�
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ���� ���
};

unsigned int indices[] = {
	  0, 1, 3, // first triangle
	  1, 2, 3  // second triangle
};

const char simpleVertexShaderPath[] = "./resources/shaders/HelloTexture.vert";
const char simpleFragmentShaderPath[] = "./resources/shaders/HelloTexture.frag";

int main() {
	std::cout << "GL Tuotrial Start" << std::endl;

	std::cout << LEapsGL::ReadFile(simpleVertexShaderPath) << "\n";
	std::cout << LEapsGL::ReadFile(simpleFragmentShaderPath) << "\n";

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

	// ���� �������� �� ���ؽ�Ʈ�� �����ϰڴٰ� �˷���
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//�ڡڡ� �Ͻ� �Լ� ȣ�� �ȵǰ� ������ ȣ�� �� chaning�ϱ�
	auto shaderProgramObject = LEapsGL::ShaderProgram();
	shaderProgramObject.CompileFromFile(GL_VERTEX_SHADER, simpleVertexShaderPath)
		.CompileFromFile(GL_FRAGMENT_SHADER, simpleFragmentShaderPath)
		.MakeProgram();
	shaderProgramObject.DeleteShaderAll();


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Vetex array�� ���ε��Ѵ�. ���� ���ؽ�Ʈ���� Bind wvertex array�� �ش� VAO���� �����ϰ� �ȴ�.
	glBindVertexArray(VAO);

	// ����� buffer object�� VBO id�� �����Ѵ�.
	// ���� ����� VBO ��ü�� ���̵� GL_ARRAY_BUFFER�� ARRAY_BUFFER state
	// GL_ARRAY_BUFFER�� State�� �����̸�, VBO�� �� ������Ʈ�� �Ҵ��� �� opengl ��ü�� id�� �ǹ��Ѵ�.
	// ���ε�� �ƹ��ϵ� ���� �ʴ´�. ���� ���� state�� id�� ä������ش�.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// CPU to GPU�� �ű��. �����͸� �Ҵ��ϰ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// gl vertex attribute �ʱ�ȭ. VAO�� �̹� ���ε� �Ǿ��ֱ� ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	auto texture = LEapsGL::Texture2D(LEapsGL::Image::LoadImage("resources/textures/container.jpg"));
	//auto img = LEapsGL::Image::CreateImage<GLubyte>(100, 100, 3, LEapsGL::ImageFormat{GL_RGB, GL_UNSIGNED_BYTE});
	//GLubyte* img_raws = static_cast<GLubyte*>(img.pixels.get());
	//for (int i = 0; i < img.width; i++) img_raws[(i + 0 * img.width)*3] = 255;
	//auto img = LEapsGL::Texture2D::grayTexture;
	//auto texture = LEapsGL::Texture2D::grayTexture;
	texture.AllocateDefaultSetting();
	texture.Apply();

	float deltaTime = 0;


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // state setting
		glClear(GL_COLOR_BUFFER_BIT); // stage using

		// DRAW
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		shaderProgramObject.use();
		shaderProgramObject.setFloat("deltaTime", timeValue);

		texture.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}