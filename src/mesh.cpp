// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-09-10
// ===============================

//Includes
#include "mesh.h"
#include <string>

void Mesh::draw(const Shader &shader, const std::unordered_map<std::string, Texture> &textureAtlas){
    //TODO: texture managing 
    unsigned int nDiffuse  = 0;
    unsigned int nSpecular = 0;

    for(unsigned int i = 0; i < textures.size(); ++i){
        //Activate next texture unit
        glActiveTexture(GL_TEXTURE0 + i);
        Texture currentTex = textureAtlas.at(textures[i]);

        //Check the type of hte texture to increment counter accordingly
        std::string name  = currentTex.type; 
        std::string number;
        if(name == "diffuse"){
            ++nDiffuse;
            number = std::to_string(nDiffuse);
        }
        else if (name == "specular"){
            ++nSpecular;
            number = std::to_string(nSpecular);
        }
        shader.setInt((name + number).c_str(), i);

        //Actually binding the texture now
        glBindTexture(GL_TEXTURE_2D, currentTex.textureID);
    }
    if(nSpecular == 0){
        shader.setInt("specularFlag", 0);
    }
    else{
        shader.setFloat("specularFlag", 1.0);
    }
    glActiveTexture(GL_TEXTURE0);


    //Mesh Drawing
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh(){
    //Generate Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind Vertex Array Object and VBO in correct order
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //VBO stuff 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //EBO stuff
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //Vertex position pointer init
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //Vertex normal pointer init
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //Vertex texture coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    //Unbinding VAO
    glBindVertexArray(0);
}