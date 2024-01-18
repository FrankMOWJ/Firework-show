#include"Particle.h"

Particle::Particle() {

	Scale = 1;
	EndScale = 1.5;
	Age = 0;
	IsFake = false;


}

Particle::~Particle()
{

}

void Particle::update(float dt) {

	if (Age == 0) {

		_startColour = Colour;
		_startScale = Scale;
	}
	// ¸üÐÂÊÙÃü
	Age += dt;

	Scale = (std::min)(_startScale + (EndScale - _startScale) * (Age / MaxAge), _startScale);

	Position = Position + (MoveVector * dt);

	Colour = RGBA(_startColour, EndColour, (Age / MaxAge));
}
