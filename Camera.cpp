#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 orientation, glm::vec3 up)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	Orientation = orientation;
	Up = up;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	projectionMat = projection;

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}



void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	//Types of Camera
	//Above board
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		Position = glm::vec3(-5.34988737f, 24.6849060f, 20.1021519f);
		Orientation = glm::vec3(0.178815141f, -0.9960f, -5.80324304e-05f);
		Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
	}
	//White pieces view
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		Position = glm::vec3(-14.5739f, 14.4547f, 18.2686f);
		Orientation = glm::vec3(0.6197f, -0.79112f, -0.001f);
		Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
	}
	//Black pieces view
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		Position = glm::vec3(15.7998f, 10.9678f, 19.3397f);
		Orientation = glm::vec3(-0.7300f, -0.6906f, 0.0045f);
		Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
	}

	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	// Handles mouse inputs
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click
			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}
	}
}

void Camera::WhitePiecesCamera()
{
	Position = glm::vec3(-14.5739f, 14.4547f, 18.2686f);
	Orientation = glm::vec3(0.6197f, -0.79112f, -0.001f);
	Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
}

void Camera::BlackPiecesCamera()
{
	Position = glm::vec3(15.7998f, 10.9678f, 19.3397f);
	Orientation = glm::vec3(-0.7300f, -0.6906f, 0.0045f);
	Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
}

void Camera::CenitalCamera()
{
	Position = glm::vec3(-5.34988737f, 24.6849060f, 20.1021519f);
	Orientation = glm::vec3(0.178815141f, -0.9960f, -5.80324304e-05f);
	Up = glm::vec3(0.0000f, 1.0000f, 0.0000f);
}

glm::mat4 Camera::getView() {
	return glm::lookAt(Position, Position + Orientation, Up);
}

glm::mat4 Camera::getProjection() {
	return projectionMat;
}
