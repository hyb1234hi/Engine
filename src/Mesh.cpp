#include "Mesh.h"

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(Vertex vertices[], int vertSize, unsigned int indices[], int indexSize)
{
  createMesh(vertices, vertSize, indices, indexSize);
}

Mesh::Mesh(const char* file)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(file,
                                           aiProcess_Triangulate |
                                           aiProcess_GenSmoothNormals |
                                           aiProcess_FlipUVs |
                                           aiProcess_CalcTangentSpace);

  if(!scene)
  {
    std::cerr << "Failed to load mesh: " << file << std::endl;
  }

  const aiMesh* model = scene->mMeshes[0];

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
  for(unsigned int i = 0; i < model->mNumVertices; i++)
  {
    const aiVector3D* pPos = &(model->mVertices[i]);
    const aiVector3D* pNormal = &(model->mNormals[i]);
    const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
    const aiVector3D* pTangent = &(model->mTangents[i]);

    Vertex vert(glm::vec3(pPos->x, pPos->y, pPos->z),
                glm::vec2(pTexCoord->x, pTexCoord->y),
                glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                glm::vec3(pTangent->x, pTangent->y, pTangent->z));

    vertices.push_back(vert);
  }

  for(unsigned int i = 0; i < model->mNumFaces; i++)
  {
    const aiFace& face = model->mFaces[i];
    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  createMesh(&vertices[0], vertices.size(), &indices[0], indices.size());
}

Mesh::~Mesh(void)
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Mesh::createMesh(Vertex vertices[], int vertSize, unsigned int indices[], int indexSize)
{
  this->vertSize  = vertSize;
  this->indexSize = indexSize;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));

  glBindVertexArray(0);
}

void Mesh::render(void)
{
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
}