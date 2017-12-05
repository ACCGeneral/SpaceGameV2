#pragma once

#include "glm\gtc\matrix_transform.hpp"
#include <gtc/type_ptr.hpp>
#include <memory>
#include "shader.h"
#include "particleExplotionData.h"


#define NUM_PARTICLE_ATTRIBUTES 6
#define MAX_PARTICLES_ON_SCENE 100000

#define PARTICLE_TYPE_GENERATOR 0

struct Particles
{
	glm::vec3 vPosition;
	glm::vec3 vVelocity;
	float fLifeTime;
	float fSize;
	int iType;
	glm::vec3 vRandSeed;
};


//reference to http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

class ParticleSphereGen
{
private:

	bool bInitialized;

	GLuint uiTransformFeedbackBuffer;

	GLuint uiParticleBuffer[2];
	GLuint uiVAO[2];

	GLuint uiQuery;
	GLuint uiTexture;

	GLuint iCurReadBuffer;
	int iNumParticles;

	glm::mat4 matProjection, matView;
	glm::vec3 vQuad1, vQuad2;

	float fElapsedTime;
	float fNextGenerationTime;

	glm::vec3 vGenPosition;
	float radious;

	float fGenLifeMin, fGenLifeRange;
	float fGenSize;

	int iNumToGenerate;

	std::shared_ptr<shader> rendershader;
	std::shared_ptr<shader> updateshader;

	int maxcycles;

	int currentcycles;

	void RenderParticles();
	void UpdateParticles(float dt);

	int generators;

	bool startgen;

public:

	void updateall(float dt);

	bool InitalizeParticleSystem(GLuint texture, int genamount);

	void SetGeneratorProperties(particleExplotionData mydata)
	{
		vGenPosition = mydata.pos;
		fGenLifeMin = mydata.minLife;
		fGenLifeRange = mydata.maxAddedLife;
		fGenSize = mydata.particleSize;
		fNextGenerationTime = mydata.spawnEvery;
		iNumToGenerate = mydata.numberToGen;
		radious = mydata.radious;
		maxcycles = mydata.maxCycles;
	}

	void ClearAllParticles();
	bool ReleaseParticleSystem();

	int GetNumParticles();

	void SetMatrices(glm::mat4 &a_matProjection, glm::vec3 vEye, glm::vec3 vView, glm::vec3 vUpVector)
	{
		matProjection = a_matProjection;
		matView = glm::lookAt(vEye, vView, vUpVector);

		vView = vView - vEye;
		vView = glm::normalize(vView);
		vQuad1 = glm::cross(vView, vUpVector);

		vQuad1 = glm::normalize(vQuad1);
		vQuad2 = glm::cross(vView, vQuad1);
		vQuad2 = glm::normalize(vQuad2);
	}

	ParticleSphereGen();

	bool getfinished()
	{
		if (startgen == true && iNumParticles == generators)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void getshader(std::shared_ptr<shader> ren)
	{
		rendershader = ren;
	}


};