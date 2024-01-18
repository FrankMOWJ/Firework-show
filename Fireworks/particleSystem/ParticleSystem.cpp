#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLuint texture, int maxParticles, Shader* shader) {
	_texture = texture;

	MaxParticles = maxParticles;

	this->particleShader = shader;
	// 设置颜色、大小、以及粒子顶点数据等等(粒子顶点数据是共用的，所以放在粒子系统中而不是粒子类中)
	loadDefaultValues();
}

ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

void ParticleSystem::loadDefaultValues() {

	StartScale = 1;
	EndScale = 1;
	StartScaleVariance = 0;
	EndScaleVariance = 0;

	Angle = 0;
	AngleVariance = 0;

	ParticleSize = 0.01f;
	ParticleLife = 2;
	ParticleLifeVariance = 0;

	MoveSpeed = 0;
	MoveVariance = 0;
	Gravity = Vector3(0, 0, 0);
	BankAngle = 30.0;

	EmissionRate = 0;
	ShouldRegenParticles = true;
	_emissionCountdown = 0;
	_totalActiveParticles = 0;

	StartColour = RGBA(1, 1, 1, 1);
	StartColourVariance = RGBA(0, 0, 0, 0);
	EndColour = RGBA(0, 0, 0, 1);
	EndColourVariance = RGBA(0, 0, 0, 0);

	GLfloat eachParticleVertices[] = {
		// vertex pos							texCoord	  Normal
		// front
		ParticleSize, ParticleSize, ParticleSize, 1.0, 1.0, 0, 0, 1,
		ParticleSize, -ParticleSize, ParticleSize,	1.0, 0.0,0, 0, 1,
		-ParticleSize, -ParticleSize, ParticleSize, 0, 0,0, 0, 1,
		ParticleSize, ParticleSize, ParticleSize, 1.0, 1.0,0, 0, 1,
		-ParticleSize, ParticleSize, ParticleSize,	0.0, 1.0,0, 0, 1,
		-ParticleSize, -ParticleSize, ParticleSize, 0.0, 0.0,0, 0, 1,
		// back
		ParticleSize, ParticleSize, -ParticleSize, 1.0, 1.0,0, 0, -1,
		ParticleSize, -ParticleSize, -ParticleSize,	1.0, 0.0,0, 0, -1,
		-ParticleSize, -ParticleSize, -ParticleSize, 0, 0,0, 0, -1,
		ParticleSize, ParticleSize, -ParticleSize, 1.0, 1.0,0, 0, -1,
		-ParticleSize, ParticleSize, -ParticleSize,	0.0, 1.0,0, 0, -1,
		-ParticleSize, -ParticleSize, -ParticleSize, 0.0, 0.0,0, 0, -1,
		// bottom
		-ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,0, -1, 0,
		ParticleSize, -ParticleSize, ParticleSize,	1.0, 0.0,0, -1, 0,
		ParticleSize, -ParticleSize, -ParticleSize, 0, 0,0, -1, 0,
		-ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,0, -1, 0,
		-ParticleSize, -ParticleSize, -ParticleSize,0.0, 1.0,0, -1, 0,
		ParticleSize, -ParticleSize, -ParticleSize, 0, 0,0, -1, 0,
		// top
		-ParticleSize, ParticleSize, ParticleSize, 1.0, 1.0,0, 1, 0,
		ParticleSize, ParticleSize, ParticleSize,	1.0, 0.0,0, 1, 0,
		ParticleSize, ParticleSize, -ParticleSize, 0, 0,0, 1, 0,
		-ParticleSize, ParticleSize, ParticleSize, 1.0, 1.0,0, 1, 0,
		-ParticleSize, ParticleSize, -ParticleSize,0.0, 1.0,0, 1, 0,
		ParticleSize, ParticleSize, -ParticleSize, 0, 0,0, 1, 0,
		// left
		-ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,-1, 0, 0,
		-ParticleSize, ParticleSize, ParticleSize,	1.0, 0.0,-1, 0, 0,
		-ParticleSize, ParticleSize, -ParticleSize, 0, 0,-1, 0, 0,
		-ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,-1, 0, 0,
		-ParticleSize, -ParticleSize, -ParticleSize,	0.0, 1.0,-1, 0, 0,
		-ParticleSize, ParticleSize, -ParticleSize, 0, 0,-1, 0, 0,
		// right
		ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,1, 0, 0,
		ParticleSize, ParticleSize, ParticleSize,	1.0, 0.0,1, 0, 0,
		ParticleSize, ParticleSize, -ParticleSize, 0, 0,1, 0, 0,
		ParticleSize, -ParticleSize, ParticleSize, 1.0, 1.0,1, 0, 0,
		ParticleSize, -ParticleSize, -ParticleSize,	0.0, 1.0,1, 0, 0,
		ParticleSize, ParticleSize, -ParticleSize, 0, 0,1, 0, 0
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eachParticleVertices), eachParticleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::update(float dt) {
	// 更新每个粒子
	for (int i = 0; i < (int)_activeParticles.size(); i++) {

		Particle* loopedParticle = _activeParticles[i];
		// 粒子寿命已到，从数组中移除
		if (loopedParticle->Age > loopedParticle->MaxAge) {
			removeParticle(loopedParticle);
			i--;		// 特别注意i--，不然会漏掉一些粒子
			continue;
		}
		_activeParticles[i]->update(dt);
		// 粒子的移动属性由粒子系统操控，以便重写实现各种效果
		_activeParticles[i]->MoveVector = _activeParticles[i]->MoveVector + (Gravity * dt);
	}

	if (ShouldRegenParticles == true) {

		if (_emissionCountdown > 0)
			_emissionCountdown -= dt;

		else
			replenishParticles(dt);
	}
}

void ParticleSystem::replenishParticles(float dt) {

	int numParticlesToSpawn;

	if (EmissionRate < dt)
		numParticlesToSpawn = dt / EmissionRate;
	else
		numParticlesToSpawn = 1;

	while ((numParticlesToSpawn > 0 || EmissionRate == 0) && _totalActiveParticles < MaxParticles) {

		numParticlesToSpawn--;
		spawnParticle();
	}
	_emissionCountdown = EmissionRate;
}

void ParticleSystem::spawnParticle() {

	Particle* newParticle = new Particle();
	_activeParticles.push_back(newParticle);

	_totalActiveParticles++;

	Vector3 randomOffset;
	randomOffset.random(2);
	randomOffset.z = 0;
	newParticle->Position = (Origin - OriginVariance) + (OriginVariance * randomOffset);

	newParticle->Scale = (StartScale - StartScaleVariance) + StartScaleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->EndScale = (EndScale - EndScaleVariance) + EndScaleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->MaxAge = (ParticleLife - ParticleLifeVariance) + ParticleLifeVariance * ((float)rand() / ((float)RAND_MAX / 2));

	RGBA randomColourOffset;
	randomColourOffset.random();
	newParticle->Colour = (StartColour - StartColourVariance) + StartColourVariance * randomColourOffset * 2;

	if (EndColour.r == -1 && EndColour.g == -1 && EndColour.b == -1) {

		newParticle->EndColour = newParticle->Colour;
		newParticle->EndColour.a = EndColour.a;
	}

	else
		newParticle->EndColour = (EndColour - EndColourVariance) + EndColourVariance * randomColourOffset;

	if (EndColour.a == -1)
		newParticle->EndColour.a = newParticle->Colour.a;

	
	float randomAngle = (Angle - AngleVariance) + AngleVariance * ((float)rand() / ((float)RAND_MAX / 2));
	
	float radians = (3.141592 * (randomAngle + 90)) / 180;
	float zSpeed = sinf(glm::radians(BankAngle));
	newParticle->MoveVector = Vector3(-cosf(radians), sinf(radians), 0);
	float randMoveSpeed = (MoveSpeed - MoveVariance) + MoveVariance * ((float)rand() / ((float)RAND_MAX / 2));
	newParticle->MoveVector = newParticle->MoveVector * randMoveSpeed;
}

void ParticleSystem::removeParticle(Particle* particleToRemove) {
	int pos = std::find(_activeParticles.begin(), _activeParticles.end(), particleToRemove) - _activeParticles.begin();

	_activeParticles.erase(_activeParticles.begin() + pos);

	if (particleToRemove->IsFake == false)
		_totalActiveParticles--;

	delete particleToRemove;
}
