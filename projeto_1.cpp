#include <cg_window.hpp>
#include <cmath>

#define DIST 0.02f


// Constantes que determinam a cor dos triângulos que formam o catavento.
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.2f, 0.2f, 0.5f);\n"
    "}\n\0";



// A seguir serão apresentadas as variáveis importantes que serão utilizadas ao longo do código.

// Variáveis que coordenam a velocidade do catavento.
double velocidade; // Valor que representara a velocidade, e quando a velocidade aumentar, a variavel RAD, apresentada a seguir, aumetara.
double angulo; // Variável que contém o valor do angulo que determinará a rotação e, portanto, a velocidade do catavento.
int divisorAngulo; // Variável que será utilizada para regular o valor do angulo da velocidade

// Variáveis auxiliares para o momento em que a tecla "espaco" for acionada.
int vTempEspaco; // Velocidade temporária, variável que auxiliará quando o espaço for acionado.
int flagEspaco; // Variável que determina se se pausa ou se retoma a rotacao.

// Vértices dos triângulos que formam a figura do catavento em sua posição inicial.
//Utilizamos um vetor de double, para tentar obter maior precisao no calculo da rotacao.
double vertices[] = {	// Triângulo 1.    
   						0.0f, 0.0f, 0.0f, 
   						0.45f, -0.15f, 0.0f, 
						0.4f,  -0.3f, 0.0f, 

						// Triângulo 2.
						-0.45f, 0.15f, 0.0f,
						-0.4f,  0.3f, 0.0f, 

						// Triângulo 3.
						0.15f, 0.45f, 0.0f, 
						0.3f,  0.4f, 0.0f,  

						// Triângulo 4.
						-0.15f, -0.45f, 0.0f, 
						-0.3f,  -0.4f, 0.0f,  

						// Base do Catavento.
						0.05f, -0.9f, 0.0f,
						-0.05f, -0.9f, 0.0f  };

unsigned int indices[] = { 	0, 1, 2,  // Triângulo 1.
    						4, 0, 3,  // Triângulo 2.
    						5, 0, 6,  // Triângulo 3.
    						7, 0, 8,  // Triângulo 4.
   							9, 0, 10  }; // Base do Catavento.



// A seguir, serão apresentadas as funções que modificam e manipulam as variáveis acima.

// Vertex Buffer Objects (VBO) é a variável que armazenará o ID do buffer dos vértices que serão trabalhados.
// Vertex Array Object (VAO)
// Element Buffer Object (EBO) é a variável que será o buffer dos índices dos vértices.
unsigned int VBO, VAO, EBO;

// Função que inicializa e manipula as variáveis VBO, VAO e EBO para que seja possível imprimir o catavento na tela.
void inicializaVar() 
{
    glGenBuffers(1, &VBO); // VBO inicializado.
    glGenBuffers(1, &EBO); // EBO inicializado.	
    glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Determina-se o tipo de buffer da variável VBO e liga-se VBO com GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // São copiados os vértices para o buffer que está ligado a GL_BUFFER, ou seja, VBO.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Determina-se o tipo de buffer de EBO e liga-se EBO com GL_ELEMENT_ARRAY_BUFFER.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // São copiados os indices para o buffer que está ligado a GL_ELEMENT_ARRAY_BUFFER, ou seja, EBO.

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0); // Ponteiros inicializados.
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 


    glBindVertexArray(0); 
}

// Função que manipulará a velocidade.
void inicializaVelocidade()
{
	velocidade = 1; // Velocidade inicial.
	divisorAngulo = 22;
	angulo = M_PI/divisorAngulo; // Ângulo inicial.
}

// Funções responsáveis pelos movimentos do catavento, chamadas na função processInput(), que sera apresentada a seguir.
void MoveCima()
{
    for(int i = 1; i <= 31; i += 3)
    	vertices[i] += DIST;
}

void MoveEsq()
{
    for(int i = 0; i <= 30; i += 3)	
    	vertices[i] -= DIST;
}

void MoveDir()
{
    for(int i = 0; i <= 30; i += 3)
    	vertices[i] += DIST;
}

void MoveBaixo()
{
    for(int i = 1; i <= 31; i += 3)
    	vertices[i] -= DIST;
}

void Pausa()
{
	if(flagEspaco == 0) // É pressionado pela primeira vez, então salva-se a velocidade anterior e iguala-se a velocidade atual a zero. 
	{
		vTempEspaco = velocidade;
		velocidade = 0;
		flagEspaco = 1;
	}
	else if(flagEspaco == 1) // É pressionado novamente, com a flag igual a 1, portanto despausa, desenvolvendo novamente a velocidade anterior à pausa.
	{
		velocidade = vTempEspaco;
		flagEspaco = 0;
	}
}

void processInput(GLFWwindow *window) // Função que processará as entradas recebidas pelo programa, enquanto a janela estiver aberta.
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // Pausa o movimento do catavento.
	{
		Pausa();    
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Fecha a janela caso "esc" seja pressionado.
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) ==  GLFW_PRESS) // Move para cima, se a tecla W for acionada.
	{   
		MoveCima();
	}
	if (glfwGetKey(window, GLFW_KEY_D) ==  GLFW_PRESS) // Move para direita, se a tecla D for acionada.
	{
		MoveDir(); 
	}
	if (glfwGetKey(window, GLFW_KEY_S) ==  GLFW_PRESS) // Move para baixo, se a tecla S for acionada.
	{   
		MoveBaixo();
	}
	if (glfwGetKey(window, GLFW_KEY_A) ==  GLFW_PRESS) // Move para esquerda, se a tecla A for acionada.
	{
		MoveEsq();
	}
	if (glfwGetKey(window, GLFW_KEY_E) ==  GLFW_PRESS) // Aumenta a velocidade, se a tecla E for acionada.
	{
		velocidade++;
		divisorAngulo -= 2; // Divisor diminui, então a velocidade aumenta
		angulo = M_PI/divisorAngulo; 
	} 
	if (glfwGetKey(window, GLFW_KEY_Q) ==  GLFW_PRESS) // Diminui a velocidade, se a tecla Q for acionada.
	{
		velocidade--; 
		divisorAngulo += 2; // Divisor aumenta, então a velocidade diminui.
		angulo = M_PI/divisorAngulo;
	}
}



int main(int argc, char const *argv[]) 
{  
    cgicmc::Window window;
    window.createWindow(); // Criação da janela.

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    // Confere-se se há erros de compilação causados pela inicialização do shader.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shadertor.
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
 

    // Confere-se se não há erros de compilação do causados pelo shader. 
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Conectam-se os shaders.
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
       

    // Confere-se se não há erros nas conexões feitas.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
	inicializaVar(); // Inicialização dos buffers e dos atributos dos vértices.
	inicializaVelocidade(); // Inicialização da variavel velocidadeAngulo.


	while (!glfwWindowShouldClose(window.getwindow())) // Enquanto não se fecha a janela, o catavento é colocado na tela.
	{
		processInput(window.getwindow());

		double a, b, c; // Pontos da origem que serão utilizados na aplicação da fórmula.
		a = vertices[0];
		b = vertices[1];
		c = vertices[2];

		if(velocidade > 0) // Gira no sentido horario. 
		{
			for(int i = 3; i < 27; i = i+3)
			{
				vertices[i] = a + vertices[i]*cos(angulo) - a*cos(angulo) - vertices[i+1]*sin(angulo) + b*sin(angulo);
				vertices[i+1] = b + vertices[i+1]*cos(angulo) - b*cos(angulo) + vertices[i]*sin(angulo) - a*sin(angulo);
			}
		}
		if(velocidade < 0) // Gira no sentido anti-horario. 
		{
    
		}

        
        inicializaVar();
 
        // Render.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
       

        glfwSwapBuffers(window.getwindow());
        glfwPollEvents();
    }

	// Desaloca tudo da biblioteca GLFW que foi alocado.  
	glfwTerminate();


	return 0;
}
