#include "PointLight.h"

void pointlight::shaderlinks(GLuint program, std::string index)
{
	glUniform3f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + ambatt).c_str()), Ambient.x, Ambient.y, Ambient.z);
	glUniform3f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + lightatt).c_str()), lightcolour.x, lightcolour.y, lightcolour.z);
	glUniform3f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + Posatt).c_str()), position.x, position.y, position.z);

	glUniform1f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + constantatt).c_str()), constant);
	glUniform1f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + linearatt).c_str()), linear);
	glUniform1f(glGetUniformLocation(program, ("myPointLight[" + index + "]." + quadraticatt).c_str()), quadratic);

}
