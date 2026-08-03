#include "model.h"

const unsigned int width = 900;				//width of the screen
const unsigned int height = 900;			//height of the screen


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


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	glm::vec4 lightColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	glEnable(GL_DEPTH_TEST);			// Enables the Depth Buffer

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); // Creates camera object

	model _model("F:/Rahul30.12.2020/openGL/Graphics/models/bunny/scene.gltf");

	//MAIN WHILE LOOP
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);					//color of background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clean the back buffer and asign new color to it

		camera.inputs(window);										 // Handles camera inputs
		camera.updateMatrix(45.0f, 0.1f, 100.0f);					 // Updates and exports the camera matrix to the Vertex Shader
		
		_model.Draw(shaderProgram, camera);

		glfwSwapBuffers(window);														 //swap back buffer with front buffer
		glfwPollEvents();																 //taking care of all GLFW events
	}


	shaderProgram.Delete();	

	glfwDestroyWindow(window);		//delete window before close
	glfwTerminate();				//terminate GLFW 
	return 0;
}

