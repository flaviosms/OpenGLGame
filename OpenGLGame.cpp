#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "           \n\
#version 330										  \n\
layout(location=0) in vec3 pos;\n\
														\n\
void main(){														\n\
	gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z,1.0);														\n\
}														\n\
";

//Fragment Shader
static const char* fShader ="           \n\
#version 330										  \n\
out vec4 colour;											\n\
														\n\
void main(){														\n\
	colour = vec4(1.0,0.0,0.0,1.0);														\n\
}														\n\
";

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode,GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLenght[1];
	codeLenght[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLenght);
	glCompileShader(theShader);


	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Erro Compilando o %d shader: '%s'\n",shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Erro Criando Programa do shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader,GL_LINK_STATUS,&result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Erro linkando o programa: '%s'\n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Erro validando o programa: '%s'\n", eLog);
		return;
	}
}

int main()
{
	//inicializando GLFW
	if (!glfwInit()) {
		printf("Inicialização do GLWF falhou");
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tirando Retro-Compatibilidade
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Setando Compatibilidade com novas versões
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Teste",NULL,NULL);
	if (!mainWindow) {
		printf("Criação da janela do GLFW falhou");
		glfwTerminate();
		return -2;
	}

	//Pegar Informação do tamanho do buffer 
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Setando o contexto do GLEW
	glfwMakeContextCurrent(mainWindow);

	//Permitindo Extensões modernas 
	glewExperimental = GL_TRUE;

	if (glewInit()!=GLEW_OK) {
		printf("Inicializacao do GLEW falhou");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -3;
	}

	//Setando Tamanho da Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Criando Triangulo
	CreateTriangle();
	CompileShaders(); 

	//Loop Principal, até a janela fechar
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		//Obtendo e lidando com eventos de input do usuario
		glfwPollEvents();

		//Limpar Janela com cor vermelha
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES,0,3);


			glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}






	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
