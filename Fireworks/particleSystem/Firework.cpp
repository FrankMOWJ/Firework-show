#include "Firework.h"

Firework::Firework(GLuint texture, Shader* shader): ParticleSystem(texture, 20, shader) {
	loadDefaultValues();

	// boom_shooting();
}

void Firework::loadDefaultValues() {

	ParticleSystem::loadDefaultValues();

	LaunchSpeed = 1.2f;
	LaunchVector = Vector3(0, 1, 0);

	SparkSpeed = 2.6f;		// 粒子拖尾的炸开速度
	SparkAngle = 180;
	SparkAngleVar = 20;		// 粒子拖尾炸开的角度变化

	ExplodeColour = RGBA(1, 0, 0, 1);
	ExplodeParticleCount = 60;
	ExplodeSpeed = 0.6f;
	ExplodeSpeedVar = 0.05f;
	ExplodeGravity = Vector3(0, -0.5f, 0);

	ShouldEmitTrail = false;
	UpdatesPerTrail = 1;
	_tailUpdatesCount = 0;
	TrailLifeModifier = 0.4f;

	_timeSinceLaunch = 0;
	State = kReady;

	MaxParticles = 100;
	StartColour = RGBA(249 / (float)255, 216 / (float)255, 84 / (float)255, 1);
	StartColourVariance = RGBA(0, 40 / (float)255, 0, 0);
	EndColour = RGBA(-1, -1, -1, 0);
	EndColourVariance = RGBA(0, 0, 0, 0);
	EmissionRate = 0.005f;
	ParticleLife = 0.5f;

	light = nullptr;
}

void Firework::launchWithDuration(float timeToExplosion) {

	if (State == kReady) {

		State = kLaunching;
		_timeSinceLaunch = 0;
		_launchDuration = timeToExplosion;

		Angle = SparkAngle;
		AngleVariance = SparkAngleVar;
		Gravity = Vector3(0, -0.01f, 0);
		MoveSpeed = SparkSpeed;
		StartScale = 0.5f;
		EndScale = 0.1f;

		ShouldRegenParticles = true;
	}
}

void Firework::update(float dt) {

	if (State == kLaunching) {

		_timeSinceLaunch += dt;

		Origin = Origin + (LaunchVector * LaunchSpeed * dt);

		if (_timeSinceLaunch > _launchDuration)
			explodePrep();
	}

	else if (State == kPrepExplode) {
		
		explode();
		State = kExploding;
	}

	else if (State == kExploding) {

		if (hasFinishedExplosion() == true)
			State = kFinished;
	}

	if (ShouldEmitTrail == true && State == kExploding) {

		if (_tailUpdatesCount == UpdatesPerTrail) {

			_tailUpdatesCount = 0;

			for (int i = 0; i < (int)_activeParticles.size(); i++) {

				Particle * loopedParticle = _activeParticles[i];

				if (loopedParticle->IsFake == false) {

					ParticleSystem::spawnParticle();
					Particle * newParticle = _activeParticles.back();
					_totalActiveParticles--;

					//拖尾的粒子不算入粒子总数
					newParticle->IsFake = true;

					newParticle->Age = 0;
					newParticle->MaxAge = (loopedParticle->MaxAge * TrailLifeModifier) - loopedParticle->Age;
					newParticle->Colour = loopedParticle->Colour;
					newParticle->EndColour = loopedParticle->EndColour;
					newParticle->EndScale = loopedParticle->EndScale;
					newParticle->MoveVector = Vector3(0, 0, 0);
					newParticle->Position = loopedParticle->Position;
					newParticle->Scale = loopedParticle->Scale;
				}
			}
		}
		else
			_tailUpdatesCount++;
	}

	ParticleSystem::update(dt);

	_previousState = State;
}

// 因为需要传入相机位置，所以使用全局变量
extern Camera camera;

void Firework::draw(
	const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
	glm::vec3 ambient, glm::vec3 specular, glm::vec3 diffusion
) {
	for (int i = 0; i < (int)_activeParticles.size(); i++) {
		Particle loopedParticle = *_activeParticles[i];

		glm::mat4 cur_model = glm::translate(model, glm::vec3(loopedParticle.Position.x, loopedParticle.Position.y, loopedParticle.Position.z));

		float scale = (std::min)(3.0f, loopedParticle.Scale);
		if (scale < 0)scale = 1.0f;
		cur_model = glm::scale(cur_model, glm::vec3(scale, scale, scale));

		glm::vec4 particleColor(loopedParticle.Colour.r, loopedParticle.Colour.g, loopedParticle.Colour.b, loopedParticle.Colour.a);
		glBindVertexArray(this->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);

		this->particleShader->use();
		this->particleShader->setInt("particleTexture", 0);	// set texture
		this->particleShader->setMat4("model", cur_model);
		this->particleShader->setMat4("view", view);
		this->particleShader->setMat4("projection", projection);

		if (this->light != nullptr) {
			this->particleShader->setInt("hasLight", 1);
			this->particleShader->setVec3("light.position", light->position);
			this->particleShader->setVec3("light.ambient", light->ambient);
			this->particleShader->setVec3("light.diffuse", light->diffuse);
			this->particleShader->setVec3("light.specular", light->specular);
			this->particleShader->setVec3("viewPos", camera.Position);
		}
		else {
			this->particleShader->setInt("hasLight", 0);
		}
		this->particleShader->setVec4("particleColor", particleColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

	//if (this->light != nullptr) {
	//	glm::mat4 model_light = glm::mat4(1.0f);
	//	// std::cout << light->position.x << " " << light->position.y << " " << light->position.z << std::endl;
	//	model_light = glm::translate(model_light, this->light->position);

	//	this->light->render(model_light, view, projection);
	//}
}

bool Firework::hasFinishedExplosion() {

	if (_totalActiveParticles == 0)
		return true;

	else
		return false;
}

void Firework::explodePrep() {
	float moveSpeedScale = (std::max)(1.2f, ((float)rand() / ((float)RAND_MAX / 3)));
	State = kPrepExplode;
	_timeSinceLaunch = -1;

	StartColour = ExplodeColour;
	StartColourVariance = RGBA(0, 0, 0, 0);
	EndColour = RGBA(-1, -1, -1, 0);
	MoveSpeed = ExplodeSpeed* moveSpeedScale;
	MoveVariance = ExplodeSpeedVar;
	Angle = 0;
	AngleVariance = 360;
	Gravity = ExplodeGravity;
	ShouldRegenParticles = false;
	StartScale = 1.5;
	EndScale = 0.75f;
	ParticleLife = 1.3f;
}

void Firework::explode() {
	boom();
	auto rf = [](float l=0.0, float r=1.0)->float {
		return l + (r - l) * rand() / (RAND_MAX + 1);;
	};
	glm::vec3 light_color = glm::vec3(rf(), rf(), rf());
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	light = new Light(
		glm::vec3(Origin.x, Origin.y, Origin.z),
		ambient,
		light_color,
		light_color
	);

	for (int i = 0; i < ExplodeParticleCount; i++) {
		ParticleSystem::spawnParticle();
	}
}

Firework::~Firework()
{
	if (light != nullptr)delete light;
}
