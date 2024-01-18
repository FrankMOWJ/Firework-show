#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include "../utils/CoordSystems.h"
#include <glad/glad.h>
#include <vector>
#include "../utils/ColourSystems.h"
#include <algorithm>
#include <iostream>
#include"../utils/shader.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Particle.h"

using namespace std;

class ParticleSystem {
public:
	/* ��������������ӷ�������ǿ����� */
	Vector3 Origin;			//����ϵͳ�����ķ���
	Vector3 OriginVariance;
	// ��������
	float ParticleLife;
	float ParticleLifeVariance;
	// ��С�����Լ���������
	float ParticleSize;
	float StartScale;
	float EndScale;
	float StartScaleVariance;
	float EndScaleVariance;
	// ��ɫ����
	RGBA StartColour;
	RGBA EndColour;
	RGBA StartColourVariance;
	RGBA EndColourVariance;
	// �ٶ������Լ�ƫת�����ԣ���Ҫ������Z���
	float Angle;
	float AngleVariance;
	float MoveSpeed;
	float MoveVariance;
	
	Vector3 Gravity;		// �������ԣ�ÿ�θ������Ӿͻ�ʩ��һ��Gravity*dt���ٶ�
	float MaxParticles;		// ���������
	float EmissionRate;		// �������ӵ�����
	bool ShouldRegenParticles;	// �Ƿ���������

	float BankAngle;		// z����б��

	GLuint VAO, VBO;		// �����������Ͷ��㻺�����
	Shader* particleShader;

protected:
	GLuint _texture;
	vector<Particle*> _activeParticles;
	int _totalActiveParticles;	//_activeParticles.size()
	float _emissionCountdown;
public:
	ParticleSystem(GLuint texture, int maxParticles, Shader* shader);
	~ParticleSystem();
	virtual void loadDefaultValues();
	virtual void update(float dt);
	virtual void spawnParticle();
	virtual void draw(
		const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
		glm::vec3 ambient, glm::vec3 specular, glm::vec3 diffusion
	) = 0;

protected:
	virtual void replenishParticles(float dt);
	virtual void removeParticle(Particle* particleToRemove);
};

#endif // !PARTICLE_SYSTEM_H

