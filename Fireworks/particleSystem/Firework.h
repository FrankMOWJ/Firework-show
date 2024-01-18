#ifndef FireworkH
#define FireworkH


#include "ParticleSystem.h"
#include"../utils/boom.h"
#include"light.h"
#include<algorithm>

enum FireworkStates { kReady, kLaunching, kPrepExplode, kExploding, kFinished };

class Firework : public ParticleSystem {
public:
	float LaunchSpeed;	//发射速度
	Vector3 LaunchVector;	//发射方向
	
	float SparkSpeed;	//火花移动的速度
	float SparkAngle;
	float SparkAngleVar;
	// 爆炸时的颜色属性
	RGBA ExplodeColour;
	float ExplodeParticleCount;
	// 爆炸的速度
	float ExplodeSpeed;
	float ExplodeSpeedVar;
	Vector3 ExplodeGravity;

	bool ShouldEmitTrail;
	int UpdatesPerTrail;
	float TrailLifeModifier;

	FireworkStates State;

	Shader* shader;

	Light* light;

protected:
	float _timeSinceLaunch;
	float _launchDuration;
	int _tailUpdatesCount;
	FireworkStates _previousState;

public:
	Firework(GLuint texture, Shader* shader);
	virtual void loadDefaultValues();
	virtual void launchWithDuration(float timeToExplosion);
	virtual void update(float dt);
	virtual void draw(
		const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
		glm::vec3 ambient, glm::vec3 specular, glm::vec3 diffusion
	);
	~Firework();

protected:
	virtual bool hasFinishedExplosion();
	virtual void explodePrep();
	virtual void explode();
};
#endif