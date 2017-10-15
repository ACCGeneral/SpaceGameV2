#include "mesh.h"

void mesh::modelsetup(GLenum drawtype) //setup opengl model data for drawing
{
	mygldata.genSetup();
	mygldata.bindstart();
	
	mygldata.bufferdata(vertices, drawtype);
	
	mygldata.Indexbuffersetup(indices, GL_STATIC_DRAW);

	mygldata.setupattrib(0, 3, sizeof(vertexinfo), (GLvoid*)0);
	mygldata.setupattrib(1, 3, sizeof(vertexinfo), (GLvoid*)offsetof(vertexinfo, vertexnormal));
	mygldata.setupattrib(2, 2, sizeof(vertexinfo), (GLvoid*)offsetof(vertexinfo, vertextexture));
	mygldata.unbind();


}

mesh::mesh(std::vector<vertexinfo> ver, std::vector<int> in, std::vector<textureinfo> tex, GLenum drawtype) //model data setup
{
	vertices = ver;
	indices = in;
	textures = tex;

	modelsetup(drawtype);
}

void mesh::drawme(std::shared_ptr<shader> myshader) //draw our mesh data 
{
	int diffuseTex = 1;
	int specularTex = 1;

	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string texturenumber;
		std::string texturetype = textures[i].textureType;
		if (texturetype == "texture_diffuse")
		{
			ss << diffuseTex++;
		}
		else if(texturetype == "texture_specular")
		{
			ss << specularTex++;
		}

		texturenumber = ss.str();

		GLint texturelink = glGetUniformLocation(myshader->returnprogram(), (texturetype + texturenumber).c_str());
		glUniform1f(texturelink, i);
		if (texturetype == "cubmap")
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i].textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		}
	}

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(mygldata.returnVAO());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);

}

