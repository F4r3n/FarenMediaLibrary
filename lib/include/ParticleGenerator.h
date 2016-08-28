#pragma once
#include "Drawable.h"
#include <vector>
#include "Texture.h"
#include "Vector2.h"
#include <random>

namespace fm {
	struct Particle {
		float life;
		glm::vec2 offset;
		glm::vec2 position;
		glm::vec4 color;
		float scale;
		glm::vec2 velocity;
		float alpha = 1;
		float lifeMax;
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

		template <typename T>
		void setVelocity(Vector2<T> min, Vector2<T> max) {
			velocityMin = min;
			velocityMax = max;
		}
		template <typename T>
		void setLife(Vector2<T> min_max ) {
			life = min_max;
		}
		void setShape(pa::SHAPE shape);
		void setLifeGenerator(float lifeGenerator);
		void setLifeParticle(float life);
		void initParticles();
		void reset();
		void setFading(bool value);
	private:
		void resetParticle(Particle &p, int indice);


		GLuint VAO;
		Texture texture;
		unsigned int numberParticles;
		std::vector<Particle> particles;
		float lifeGenerator = 10;
		float lifeGeneratorMax = 10;

		Vector2<float> velocityMax;
		Vector2<float> velocityMin;
		
		Vector2<float> life;
		Vector2<float> position;
		Vector2<float> gravity;

		pa::SHAPE shape = pa::SHAPE::NONE;

		std::string nameShader;
		std::string textureName;

		std::random_device seeder;

		bool over = false;
		bool fading = false;

	};
}

//TODO instanciation