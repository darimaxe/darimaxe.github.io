/*
 * FinalProject.cpp
 *
 *  Created on: Nov 24, 2020
 *      Author: DJ Maxey
 */

// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM include

#include "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "glm/gtc/type_ptr.hpp"


using namespace std; // Standard namespace

#define WINDOW_TITLE "Modern OpenGL" // Window title Macro

// Shader program macro

#ifndef GLSL

#define GLSL(Version, Source) "#version " #Version "\n" #Source

#endif

// Variable declarations for shader, window size initialization, buffer and array objects

GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO;

GLfloat cameraSpeed = 0.0005f; // Movement speed per frame

GLchar currentKey; // Will store key pressed

GLfloat lastMouseX = 400, lastMouseY = 300; // Locks mouse cursor at the center of the screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f; // mouse offset, yaw, and pitch variables
GLfloat sensitivity = 0.005f; // Used for mouse / camera rotation sensitivity
bool mouseDetected = true; // Initially true when mouse movement is detected

// Global vector declarations
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Initial camera position. Placed 5 units in z
glm::vec3 CameraUpY = glm::vec3(0.396f, 0.263f, 0.129f); // Temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -1.0f); // Temporary z unit vector
glm::vec3 front; // Temporary z unit vector for mouse

// Function prototypes

void UResizeWindow(int, int);

void URenderGraphics(void);

void UCreateShader(void);

void UCreateBuffers(void);

void UMouseMove(int x, int y);

void UKeyboard(unsigned char key, int x, int y);

void UKeyReleased(unsigned char key, int x, int y);



// Vertex Shader Source Code
const GLchar * vertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
	layout (location = 1) in vec3 color; // Color data from Vertex Attrib Pointer 1

	out vec3 mobileColor; // variable to transfer color data to the fragment shader

	//Global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
		mobileColor = color; // references incoming color data
	}
);



// Fragment Shader Source Code
const GLchar * fragmentShaderSource = GLSL(330,
	in vec3 mobileColor; // Variable to hold incoming color data from vertex shader

	out vec4 gpuColor; // Variable to pass color data to the GPU

	void main(){

		gpuColor = vec4(mobileColor, 1.0); // Sends color data to the GPU for rendering
	}
);



// Main Program
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return -1;
		}

	UCreateShader();

	UCreateBuffers();

	// Use the Shader program
	glUseProgram(shaderProgram);

	glClearColor(0.0f, 0.396f, 0.263f, 0.129f); // Set background color

	glutDisplayFunc(URenderGraphics);

	glutKeyboardFunc(UKeyboard); // Detects key press

	glutKeyboardUpFunc(UKeyReleased); // Detects key release

	glutPassiveMotionFunc(UMouseMove); // Detects mouse movement

	glutMainLoop();

	// Destroys Buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

// Resizes the window
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

// Renders graphics
void URenderGraphics(void)
{
	glEnable(GL_DEPTH_TEST); // Enable z-depth

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

	glBindVertexArray(VAO); // Activate the Vertex Array Object before rendering and transforming them

	//Camera Movement Logic
	if(currentKey == 'w')
		cameraPosition += cameraSpeed * CameraForwardZ;

	if(currentKey == 's')
		cameraPosition -= cameraSpeed * CameraForwardZ;

	if(currentKey == 'a')
		cameraPosition -= glm::normalize(glm::cross(CameraForwardZ, CameraUpY)) * cameraSpeed;

	if(currentKey == 'd')
		cameraPosition += glm::normalize(glm::cross(CameraForwardZ, CameraUpY)) * cameraSpeed;

	CameraForwardZ = front; // Replaces camera forward vector with Radians normalized as a unit vector

	// Transform the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Place the object at the center of the viewport

	// Rotate the object 45 degrees on the Y-axis

	model = glm::rotate(model, 45.0f, glm::vec3(0.0, 1.0f, 0.0f));

	// Increase the object size by a scale of 2

	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	// Transforms the camera
	glm::mat4 view;
	//view = glm::lookAt(cameraPosition - CameraForwardZ, cameraPosition, CameraUpY);
	view = glm::lookAt(CameraForwardZ, cameraPosition, CameraUpY);

	// Creates a perspective projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	// Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, 186);

	glBindVertexArray(0); // Deactivate the Vertex Array Object

	glutSwapBuffers(); // Flips the back buffer with the front buffer every frame

}


// Creates the shader program
void UCreateShader()
{

	// Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates the Vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attaches the Vertex shader to the source code
	glCompileShader(vertexShader); // Compiles the Vertex shader

	// Fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the Fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attaches the fragment shader to the source code
	glCompileShader(fragmentShader); // Compiles the fragment shader

	// Shader program
	shaderProgram = glCreateProgram(); // Creates the shader program and return an id
	glAttachShader(shaderProgram, vertexShader); // Attach Vertex shader to the Shader program
	glAttachShader(shaderProgram, fragmentShader); // Attach Fragment shader to the Shader program
	glLinkProgram(shaderProgram); // Link Vertex and Fragment shaders to Shader program

	// Delete the Vertex and Fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void UCreateBuffers()
{
	GLfloat vertices[] = {
			//Positions				//Color
			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,

			0.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,

			0.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,

			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,

			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,

//			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
//			0.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
//			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
//			0.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
//			0.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
//			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,

			// seat
			0.0f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,

			0.0f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,

			0.0f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,

			0.0f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,

			1.5f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.0f, -0.25f, 1.0f, 0.156f, 0.0f,

			0.0f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			1.5f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, -0.25f, 1.0f, 0.156f, 0.0f,
			0.0f, 1.1f, 1.5f, 1.0f, 0.156f, 0.0f,

			// back rest
			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,

			0.0f, 1.0f, -0.15f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.15f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.15f, 0.396f, 0.263f, 0.129f,

			0.0f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,

			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.15f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			0.0f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,

			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.15f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.65f, 0.396f, 0.263f, 0.129f,
			1.5f, 2.5f, -0.75f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,

			// second leg
			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,

			1.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,

			1.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,

			1.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 0.25f, 0.396f, 0.263f, 0.129f,

			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 0.25f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, -0.25f, 0.396f, 0.263f, 0.129f,

			// third leg
			//Positions				//Color
			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,

			0.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,

			0.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,

			0.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,

			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			0.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,

			// fourth leg
			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,

			1.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,

			1.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,

			1.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, 1.0f, 1.5f, 0.396f, 0.263f, 0.129f,

			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.5f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.5f, 0.396f, 0.263f, 0.129f,
			1.0f, -0.5f, 1.0f, 0.396f, 0.263f, 0.129f,

	};



	// Generate buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Activate the Vertex Array Objects before binding and setting any VBOs and Vertex Attribute Pointers
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

	// Set attribute pointer 0 to hold Poistion data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enables vertex attribute

	// Set attribute pointer 1 to hold Color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // Enables vertex attribute

	glBindVertexArray(0); // Deactivates the VAO which is good practice

}

//Implements the UKeyboard function
void UKeyboard(unsigned char key, GLint x, GLint y)
{
	switch(key){
		case 'w':
			currentKey = key;
			cout<<"You pressed W!"<<endl;
			break;
		case 's':
			currentKey = key;
			cout<<"You pressed S!"<<endl;
			break;
		case 'a':
			currentKey = key;
			cout<<"You pressed A!"<<endl;
			break;
		case 'd':
			currentKey = key;
			cout<<"You pressed D!"<<endl;
			break;
		default:
			cout<<"Press a key!"<<endl;
	}
}

// Implements the UKeyReleased function
void UKeyReleased(unsigned char key, GLint x, GLint y)
{
	cout<<"Key released!"<<endl;
	currentKey = 0;
}

void UMouseMove(int x, int y)
{
	// Immediately replaces center locked coordinates with new mouse coordinates
	if(mouseDetected)
	{
		lastMouseX = x;
		lastMouseY = y;
		mouseDetected = false;
	}

	// Gets the direction the mouse was moved in x and y
	mouseXOffset = x - lastMouseX;
	mouseYOffset = lastMouseY - y; // Inverted Y

	// Updates with new mouse coordinates
	lastMouseX = x;
	lastMouseY = y;

	// Applies sensitivity to mouse direction
	mouseXOffset *= sensitivity;
	mouseYOffset *= sensitivity;

	// Accumulates the yaw and pitch variables
	yaw += mouseXOffset;
	pitch += mouseYOffset;

	// Maintains a 90 degree pitch for gimbal lock
	if(pitch > 89.0f)
		pitch = 89.0f;

	if(pitch < -89.0f)
		pitch = -89.0f;

	// Converts mouse coordinates / degrees into Radians, then to vectors
//	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
//	front.y = sin(glm::radians(pitch));
//	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front.x = 10.0f * cos(yaw);
	front.y = 10.0f * sin(pitch);
	front.z = sin(yaw) * cos(pitch) * 10.0f;

}
