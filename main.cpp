#include "mesh.h"

const unsigned int width = 900;				//width of the screen
const unsigned int height = 900;			//height of the screen


// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, 
	0, 2, 3
 
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)}
};


GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	glfwInit();   //initalize GLFW 
	
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);    //tell GLFW version of OpenGL we are using 
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//which is 3 in our case
	
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   //tell GLFW we are using "CORE" profile 
																	//which means we are only using morden functions

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl window title", NULL, NULL);  //creatig window size 1280x720 and its name
	
	glfwMakeContextCurrent(window);												//this connects the current window to the context

	gladLoadGL();   //loading GLAD

	glViewport(0, 0, width, height);											//specify the viewport of openGL in the window


	Texture textures[]
	{
		Texture("planks.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("planksSpec.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	mesh floor(verts, ind, tex);


	//lightCube

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	mesh light(lightVerts, lightInd, tex);



	glm::vec4 lightColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	glEnable(GL_DEPTH_TEST);			// Enables the Depth Buffer

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); // Creates camera object


	//MAIN WHILE LOOP
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);					//color of background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clean the back buffer and asign new color to it

		camera.inputs(window);										 // Handles camera inputs
		camera.updateMatrix(45.0f, 0.1f, 100.0f);					 // Updates and exports the camera matrix to the Vertex Shader
		
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);

		glfwSwapBuffers(window);														 //swap back buffer with front buffer
		glfwPollEvents();																 //taking care of all GLFW events
	}


	shaderProgram.Delete();
	lightShader.Delete();
	

	glfwDestroyWindow(window);		//delete window before close
	glfwTerminate();				//terminate GLFW 
	return 0;
}

