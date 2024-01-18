#pragma once
#ifndef HEART_SHAPE_FIREWORK_H
#define HEART_SHAPE_FIREWORK_H

#include "Firework.h"

class HeartShapeFirework : public Firework {
public:
	HeartShapeFirework(GLuint texture, Shader* shader): Firework(texture, shader) {}
	virtual void explode() {
		boom();
		auto rf = [](float l = 0.0, float r = 1.0)->float {
			return l + (r - l) * rand() / (RAND_MAX + 1);;
		};
		glm::vec3 light_color = glm::vec3(rf(), rf(), rf());
		light = new Light{
			glm::vec3(Origin.x, Origin.y, Origin.z),
			light_color,
			light_color,
			light_color
		};
		float snp = 40.0;
		float unit = (360.0 / snp);
		this->Gravity = Vector3(0.0f, -0.01f, 0.0f);
		float moveSpeedScale = (std::max)(1.2f, ((float)rand() / ((float)RAND_MAX / 3)));
		for (float j = 0.0; j < snp; j++) {
			//create a new particle
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

			float ang = glm::radians(j * unit);

			float x = glm::cos(ang);
			float y = glm::sin(ang);
			y = y + abs(x) * sqrt((8.0 - abs(x)) / 50.0);

			newParticle->MoveVector = Vector3(cosf(ang), sinf(ang), 0);

			float MoveSpeed = (x * x + y * y) * (0.4 / (newParticle->MaxAge)) * moveSpeedScale;
			newParticle->MoveVector = newParticle->MoveVector * MoveSpeed;
		}
	}
};
#endif // !HEART_SHAPE_FIREWORK_H
