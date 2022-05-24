#include "mesh.h"
#include "displaylist.h"
#include "core.h"

Utlop::Mesh::Mesh(const std::vector<float> vertices_, const std::vector<uint32_t> indices_, 
	std::vector<Texture> textures_, string geoPath)
{
  n_indices = indices_.size();
	textures = textures_;
	path = geoPath;
	normalMap = 0;

	glCreateVertexArrays(1, &vao);
	GLuint vbo;
	GLuint ebo;
	const GLuint vertexPosition = 0;
	const GLuint texcoord = 1;
	const GLuint normalPosition = 2;

	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferData(vbo, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
	glNamedBufferData(ebo,	indices_.size() * sizeof(GLuint), 
		indices_.data(), GL_STATIC_DRAW);


	glEnableVertexArrayAttrib(vao, vertexPosition);
	glVertexArrayAttribBinding(vao, vertexPosition, 0);
	glVertexArrayAttribFormat(vao, vertexPosition, 3, GL_FLOAT, GL_FALSE, 0);


	glEnableVertexArrayAttrib(vao, texcoord);
	glVertexArrayAttribBinding(vao, texcoord, 0);
	glVertexArrayAttribFormat(vao, texcoord, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

	glEnableVertexArrayAttrib(vao, normalPosition);
	glVertexArrayAttribBinding(vao, normalPosition, 0);
	glVertexArrayAttribFormat(vao, normalPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));



	glVertexArrayVertexBuffer(vao, 0,	vbo, 0, 8 * sizeof(float));


	glVertexArrayElementBuffer(vao,	ebo);

}

void Utlop::Mesh::Draw(GLuint& shader, CameraComponent& cameracmp, int hasLightcomponent)
{
	glUseProgram(shader);

	glBindVertexArray(vao);

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	unsigned int numNormal = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse"){
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular"){
			num = std::to_string(numSpecular++);
		}
		else if (type == "normal") {
			num = std::to_string(numNormal++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].bind();
	}
	

	glUniform1i(glGetUniformLocation(shader, "hasNormalMap"), numNormal);
	glUniform1i(glGetUniformLocation(shader, "hasLightCmp"), hasLightcomponent);

	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader, "camPosition"), cameracmp.position_.x, cameracmp.position_.y, cameracmp.position_.z);
	//camera.Matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void Utlop::Mesh::DrawMesh(Mesh m, GLuint& shader, CameraComponent& cameracmp, LocalTRComponent& localcmp, int hasLightcomponent)
{
	addSetModelViewProjection(Core::Instance()->getDisplayList(), shader, cameracmp.projection_,
		localcmp.model, cameracmp.view_);

	addDrawMeshCmd(Core::Instance()->getDisplayList(), m, shader, cameracmp, localcmp, hasLightcomponent);

}
