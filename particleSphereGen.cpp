#include "particleSphereGen.h"

bool ParticleSphereGen::InitalizeParticleSystem(GLuint texture, int genamount)
{
	if (bInitialized)
	{
		return false;
	}
	const char* sVaryings[NUM_PARTICLE_ATTRIBUTES] =
	{
		"vPositionOut",
		"vVelocityOut",
		"fLifeTimeOut",
		"fSizeOut",
		"iTypeOut",
		"vRandomOut"
	};

	updateshader = std::make_shared<shader>();
	rendershader = std::make_shared<shader>();

	updateshader->loadshader("shaders\\Particles\\ParticledataVertex.txt", GL_VERTEX_SHADER);
	updateshader->loadshader("shaders\\Particles\\ParticlecreationGeomarty.txt", GL_GEOMETRY_SHADER);
	for (int i = 0; i < NUM_PARTICLE_ATTRIBUTES; i++)
	{
		glTransformFeedbackVaryings(updateshader->returnprogram(), NUM_PARTICLE_ATTRIBUTES, sVaryings, GL_INTERLEAVED_ATTRIBS); // Tells OpenGL which attributes should transform feedback record
	}
	updateshader->linkshader();
	updateshader->linkcheck();

	rendershader->loadshader("shaders\\Particles\\Particledraw_Vertex.txt", GL_VERTEX_SHADER);
	rendershader->loadshader("shaders\\Particles\\Particledraw_Geomatry.txt", GL_GEOMETRY_SHADER);
	rendershader->loadshader("shaders\\Particles\\Particledraw_Fragment.txt", GL_FRAGMENT_SHADER);
	rendershader->linkshader();
	rendershader->linkcheck();


	glGenTransformFeedbacks(1, &uiTransformFeedbackBuffer); // Generates transform feedback object
	glGenQueries(1, &uiQuery); // Generates a general query object, in our case we use it to determine number of emitted particles

	glGenBuffers(2, uiParticleBuffer);
	glGenVertexArrays(2, uiVAO);

	generators = genamount;

	std::vector<Particles> mygenpars;

	for (int i = 0; i < genamount; i++)
	{
		Particles parinst;
		parinst.iType = PARTICLE_TYPE_GENERATOR;
		parinst.vRandSeed = glm::vec3(getrandomwfloat(-60.0, 120.0), getrandomwfloat(-60.0, 120.0), getrandomwfloat(-60.0, 120.0));
		mygenpars.push_back(parinst);
	}

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(uiVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, uiParticleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particles)*MAX_PARTICLES_ON_SCENE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mygenpars.size() * sizeof(Particles), &mygenpars[0]);


		for (int j = 0; j < NUM_PARTICLE_ATTRIBUTES; j++)
		{
			glEnableVertexAttribArray(j);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particles), (const GLvoid*)0); // Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particles), (const GLvoid*)12); // Velocity
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particles), (const GLvoid*)24); // life
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particles), (const GLvoid*)28); // size
		glVertexAttribPointer(4, 1, GL_INT, GL_FALSE, sizeof(Particles), (const GLvoid*)32); // type
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Particles), (const GLvoid*)36); // type
	}

	iCurReadBuffer = 0;
	iNumParticles = mygenpars.size();

	bInitialized = true;

	uiTexture = texture; 

	return true;
}

void ParticleSphereGen::RenderParticles()
{
	if (!bInitialized)
	{
		return;
	}

	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_CULL_FACE);
	glDepthMask(0);

	glDisable(GL_RASTERIZER_DISCARD);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	rendershader->useProgram();
	glUniform3f(glGetUniformLocation(rendershader->returnprogram(), "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(rendershader->returnprogram(), "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);
	glUniformMatrix4fv(glGetUniformLocation(rendershader->returnprogram(), "matrices.mProj"), 1, GL_FALSE, glm::value_ptr(matProjection));
	glUniformMatrix4fv(glGetUniformLocation(rendershader->returnprogram(), "matrices.mView"), 1, GL_FALSE, glm::value_ptr(matView));
	glUniform1f(glGetUniformLocation(rendershader->returnprogram(), "gSampler"), 0);
	glUniform1f(glGetUniformLocation(rendershader->returnprogram(), "maxlife"), fGenLifeMin + fGenLifeRange);

	glUniform1f(glGetUniformLocation(rendershader->returnprogram(), "sizechange"), fGenLifeMin + fGenLifeRange);

	glBindVertexArray(uiVAO[iCurReadBuffer]);
	glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

	glDrawArrays(GL_POINTS, 0, iNumParticles);

	glDepthMask(1);
	
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleSphereGen::UpdateParticles(float dt)
{
	if (!bInitialized)
	{
		return;
	}

	updateshader->useProgram();

	glUniform1f(glGetUniformLocation(updateshader->returnprogram(), "fTimePassed"), dt);
	glUniform3f(glGetUniformLocation(updateshader->returnprogram(), "vGenPosition"), vGenPosition.x, vGenPosition.y, vGenPosition.z);


	glUniform1f(glGetUniformLocation(updateshader->returnprogram(), "fGenLifeMin"), fGenLifeMin);
	glUniform1f(glGetUniformLocation(updateshader->returnprogram(), "fGenLifeRange"), fGenLifeRange);

	glUniform1f(glGetUniformLocation(updateshader->returnprogram(), "radious"), radious);

	glUniform1f(glGetUniformLocation(updateshader->returnprogram(), "fGenSize"), fGenSize);

	if (currentcycles < maxcycles)
	{
		glUniform1i(glGetUniformLocation(updateshader->returnprogram(), "iNumToGenerate"), iNumToGenerate);
		glm::vec3 vRandomSeed = glm::vec3(getrandomwfloat(-10.0, 20.0), getrandomwfloat(-10.0, 20.0), getrandomwfloat(-10.0, 20.0));
		glUniform3f(glGetUniformLocation(updateshader->returnprogram(), "vRandomSeed"), vRandomSeed.x, vRandomSeed.y, vRandomSeed.z);
		currentcycles++;
		startgen = true;
	}
	else
	{
		glUniform1i(glGetUniformLocation(updateshader->returnprogram(), "iNumToGenerate"), 0);
	}
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, uiTransformFeedbackBuffer); // Binds transform feedback object, if you bind 0, you cancel all transform feedbacks

	glBindVertexArray(uiVAO[iCurReadBuffer]);
	glEnableVertexAttribArray(1); // Re-enable velocity

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, uiParticleBuffer[1 - iCurReadBuffer]); // Tells where to store the results of transform feedback

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, uiQuery); // Starts query, in our case we call it with parameter GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
	glBeginTransformFeedback(GL_POINTS); // Starts recording of outputted geometry

	glDrawArrays(GL_POINTS, 0, iNumParticles);

	glEndTransformFeedback(); // Ends recording of outputted geometry

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(uiQuery, GL_QUERY_RESULT, &iNumParticles); // Gets query result, in our case number of emitted particles


	iCurReadBuffer = 1 - iCurReadBuffer;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);


}

void ParticleSphereGen::ClearAllParticles()
{



}

bool ParticleSphereGen::ReleaseParticleSystem()
{
	glDeleteBuffers(2, &uiParticleBuffer[0]);
	glDeleteVertexArrays(2, &uiVAO[0]);

	return true;
}

int ParticleSphereGen::GetNumParticles()
{
	return iNumParticles;
}

ParticleSphereGen::ParticleSphereGen()
{
	currentcycles = 0;
	startgen = false;
	bInitialized = false;
}



void ParticleSphereGen::updateall(float dt)
{
	UpdateParticles(dt);
	RenderParticles();

}