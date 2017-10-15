#include "spotlight.h"


void spotlight::shaderlinks(GLuint program, std::string index) //link up all of our spotlight attributes 
{
	glUniform3f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + diratt).c_str()), direction.x, direction.y, direction.z);
	glUniform3f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + ambatt).c_str()), Ambient.x, Ambient.y, Ambient.z);
	glUniform3f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + lightatt).c_str()), lightcolour.x, lightcolour.y, lightcolour.z);
	glUniform3f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + Posatt).c_str()), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + radiousatt).c_str()), radious);
	glUniform1f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + outerradatt).c_str()), outerrad);
	glUniform1f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + constantatt).c_str()), constant);
	glUniform1f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + linearatt).c_str()), linear);
	glUniform1f(glGetUniformLocation(program, ("mySpotLights[" + index + "]." + quadraticatt).c_str()), quadratic);
}


