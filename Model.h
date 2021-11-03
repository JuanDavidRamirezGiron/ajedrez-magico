#include<json/json.h>
#include"Mesh.h"

class Model {
public:
	Model(const char* filename);
	void Draw(Shader& shader, Camera& camera);
	int loadModel(const char* filename);
private:
	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);
	
	std::vector<Mesh> meshes;
};