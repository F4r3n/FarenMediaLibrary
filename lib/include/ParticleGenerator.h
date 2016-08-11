#pragma once
#include "Drawable.h"
#include <vector>
#include "Texture.h"
namespace fm {
	struct Particle {
		float life;
		glm::vec2 offset;
		glm::vec2 position;
		glm::vec4 color;
		float velocity;
	};

	class ParticleGenerator : public Drawable
	{
	public:
		ParticleGenerator(float posX, float posY, unsigned int numberParticles, Texture &texture);
		~ParticleGenerator();
		void draw(Shader &shader);
		void init();
		const std::string getNameShader() const;
		void update();
	private:
		GLuint VAO;
		Texture texture;
		unsigned int numberParticles;
		std::vector<Particle> particles;

		float velocityMax;
		float lifeMax;
		float positionX, positionY;

		std::string nameShader;
		std::string textureName;

	};
}

//TODO instanciation