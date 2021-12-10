#include"Model.h"

Model::Model(const char* filename){
	loadModel(filename);
}
Model::Model() {
}

void Model::Draw(Shader& shader, Camera& camera, glm::vec3 translation, GLfloat rdeg, glm::vec3 raxis, glm::vec3 escalation){
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, translation, rdeg, raxis, escalation);
	}
}


int Model::loadModel(const char* filename) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	
	std::vector < glm::vec3 > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normals;
	std::vector<Texture> textures;
	char mtl_file[128];

	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return -1;
	}
	while (1) {
		
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		//MTL
		if (strcmp(lineHeader, "mtllib") == 0) {
			fscanf(file, "%s\n", &mtl_file);
		}
		//COORDS
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		//UV TEXTURAS
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		//NORMALES
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		//CARAS
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	
	//Abrimos el mtl para buscar donde se encuentra la textura
	std::string  mtl_file_dir = mtl_file;
	std::string mtl = "models/" + mtl_file_dir;
	FILE* file2 = fopen(mtl.c_str(), "r");
	if (file2 == NULL) {
		printf("Impossible to open the file !\n");
		return -1;
	}
	while (1) {

		char lineHeader2[128];
		int res = fscanf(file2, "%s", lineHeader2);
		if (res == EOF)
			break;

			if (strcmp(lineHeader2, "map_Kd") == 0) {
				char texturename[128];
				fscanf(file2, "%s\n", &texturename);
				std::string  png_file_dir = texturename;
				std::string png = "models/" + png_file_dir;
				Texture tex(png.c_str(), "diffuse", 0);
				textures.push_back(tex);
			}
		
		
	}
	///////////////////////////////////////////////////////////////////////////////

	//map_Kd
	

	// Combine all the vertex components and also get the indices and textures
	//out_vertices = positions
	std::vector<Vertex> vertices = assembleVertices(out_vertices, out_normals, out_uvs);
	std::vector<GLuint> indices; //= getIndices(JSON["accessors"][indAccInd]);

	// Combine the vertices, indices, and textures into a mesh
	meshes.push_back(Mesh(vertices, indices, textures));

}


std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals,std::vector<glm::vec2> texUVs){
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}


