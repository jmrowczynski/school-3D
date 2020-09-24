#ifndef MODEL
#define MODEL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip> 
#include "shaderprogram.h"
#include "OBJ_Loader.h"
#include "globals.h"
#include "lodepng.h"

using namespace std;

struct Texture {
    GLuint id;
    string path;
};

class Model {
    public:
        Model(string filepath, string filename);
		void freeModel();
        void load();
        void draw();
    private:
        string filename;
        string filepath;
        vector<objl::Mesh> meshes;
        vector<vector<float>> vertices;
        vector<vector<float>> normals;
        vector<vector<float>> texCoords;
        GLuint readTexture(string filename);
        vector<vector<GLuint>> ids; // przechowywanie par id od kd i ks
		vector<Texture> paths;
};

Model::Model(string filepath, string filename) {
    this->filename = filename;
    this->filepath = filepath;
}

void Model::load() {
    objl::Loader Loader;
	
	bool loadout = Loader.LoadFile(filepath + filename);

	meshes = Loader.LoadedMeshes;

    if (loadout) {

        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

			vector<float> tempVertices;
			vector<float> tempNormals;
			vector<float> tempTexCoords;
			GLuint tempId;

			bool isInPaths = false;
			Texture temp;

            for (int j = 0; j < curMesh.Vertices.size(); j++) {
				tempVertices.push_back(curMesh.Vertices[j].Position.X);
				tempVertices.push_back(curMesh.Vertices[j].Position.Y);
				tempVertices.push_back(curMesh.Vertices[j].Position.Z);
				tempVertices.push_back(1.0f);

				tempNormals.push_back(curMesh.Vertices[j].Normal.X);
				tempNormals.push_back(curMesh.Vertices[j].Normal.Y);
				tempNormals.push_back(curMesh.Vertices[j].Normal.Z);
				tempNormals.push_back(0.0f);

				tempTexCoords.push_back(curMesh.Vertices[j].TextureCoordinate.X);
				tempTexCoords.push_back(curMesh.Vertices[j].TextureCoordinate.Y);
            }

			string nameToCompare = filepath + curMesh.MeshMaterial.map_Kd;
			vector<GLuint> tempIDs; // składowanie id_kd i id_ks
			for (int k = 0; k < paths.size(); k++) {
				if ( nameToCompare.compare(paths[k].path) == 0 ) {
					tempIDs.push_back(paths[k].id);
					tempIDs.push_back(paths[k+1].id);
					ids.push_back(tempIDs);
					isInPaths = true;
					break;
				}
			}

			if (!isInPaths) {
            	tempId = readTexture(filepath + curMesh.MeshMaterial.map_Kd);
				tempIDs.push_back(tempId);
				ids.push_back(tempIDs);
				temp.id = tempId;
				temp.path = filepath + curMesh.MeshMaterial.map_Kd;
				paths.push_back(temp);

				tempId = readTexture(filepath + curMesh.MeshMaterial.map_Ks);
				tempIDs.push_back(tempId);
				ids.push_back(tempIDs);
				temp.id = tempId;
				temp.path = filepath + curMesh.MeshMaterial.map_Ks;
				paths.push_back(temp);
			}

			vertices.push_back(tempVertices);
			normals.push_back(tempNormals);
			texCoords.push_back(tempTexCoords);

        }

    }
	else
	{
		// Output Error
		cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
	}
}

void Model::draw() {


    for (int i = 0; i < meshes.size(); i++) {

        // zmienna jednorodna przypisana do jednostki teksturującej 0
        glUniform1i(sp->u("textureMap0"), 0);
        // przypisanie tekstury do jednostki teksturującej
		// informacja, którą jednostkę teksturującą będziemy teraz używać
        glActiveTexture(GL_TEXTURE0); 
        // powiązanie jednostki z teksturą, ids[i][0] to uchwyt na teksturę
	    glBindTexture(GL_TEXTURE_2D, ids[i][0]); 

        glUniform1i(sp->u("specular0"), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ids[i][1]);

		glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,vertices[i].data()); //Wskaż mesheslicę z danymi dla atrybutu vertex

		glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals[i].data()); //Wskaż mesheslicę z danymi dla atrybutu normal

		glEnableVertexAttribArray(sp->a("texCoord0"));
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords[i].data());

    	glDrawElements(GL_TRIANGLES, meshes[i].Indices.size(), GL_UNSIGNED_INT, meshes[i].Indices.data());  //Narysuj obiekt
	
        glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
        glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
        glDisableVertexAttribArray(sp->a("texCoord0"));
    }
}

GLuint Model::readTexture(string filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	vector<unsigned char> image; 
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filename.c_str());

	glGenTextures(1,&tex); 
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return tex;
}

void Model::freeModel() {
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		glDeleteTextures(1, &paths[i].id);
	}
}

#endif