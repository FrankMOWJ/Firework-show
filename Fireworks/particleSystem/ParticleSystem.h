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
	/* 所有粒子属性添加方差以增强随机性 */
	Vector3 Origin;			//粒子系统的中心分量
	Vector3 OriginVariance;
	// 粒子寿命
	float ParticleLife;
	float ParticleLifeVariance;
	// 大小属性以及缩放属性
	float ParticleSize;
	float StartScale;
	float EndScale;
	float StartScaleVariance;
	float EndScaleVariance;
	// 颜色属性
	RGBA StartColour;
	RGBA EndColour;
	RGBA StartColourVariance;
	RGBA EndColourVariance;
	// 速度属性以及偏转角属性，主要是用于Z轴的
	float Angle;
	float AngleVariance;
	float MoveSpeed;
	float MoveVariance;
	
	Vector3 Gravity;		// 重力属性，每次更新粒子就会施加一个Gravity*dt的速度
	float MaxParticles;		// 最大粒子数
	float EmissionRate;		// 发射粒子的速率
	bool ShouldRegenParticles;	// 是否重生粒子

	float BankAngle;		// z轴倾斜角

	GLuint VAO, VBO;		// 顶点数组对象和顶点缓冲对象
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

