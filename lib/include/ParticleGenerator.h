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
		float scale;
		glm::vec2 velocity;
	};

	namespace pa {

		enum SHAPE
		{
			CIRCLE,
			CONE,
			NONE
				
		};
	}

	class ParticleGenerator : public Drawable
	{
	public:
		ParticleGenerator(float posX, float posY, unsigned int numberParticles, Texture &texture);
		~ParticleGenerator();
		void draw(Shader &shader);
		void init();
		const std::string getNameShader() const;
		void update(float dt);
		void setGravity(float fx, float fy);
		void setVelocity(float fx, float fy);
		void setShape(pa::SHAPE shape);
		void initParticles();
	private:
		GLuint VAO;
		Texture texture;
		unsigned int numberParticles;
		std::vector<Particle> particles;

		float velocityMaxX, velocityMaxY;
		float lifeMax;
		float positionX, positionY;
		float gravityX = 0, gravityY = 0;

		pa::SHAPE shape = pa::SHAPE::NONE;

		std::string nameShader;
		std::string textureName;

	};
}

//TODO instanciation