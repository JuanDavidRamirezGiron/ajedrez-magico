#include"Mesh.h"

class Model {
public:
	Model(const char* filename);
	Model();
	void Draw(Shader& shader, Camera& camera, glm::vec3 translation, GLfloat rdeg, glm::vec3 raxis, glm::vec3 escalation);
	int loadModel(const char* filename);
private:
	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);
	
	std::vector<Mesh> meshes;
};