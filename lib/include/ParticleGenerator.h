#pragma once
#include "Drawable.h"
#include <vector>
#include "Texture.h"
#include "Vector2.h"
#include <random>
#include <map>
#include <array>
namespace fm {


	namespace pa {

		enum SHAPE
		{
			CIRCLE,
			CONE,
			NONE
				
		};

		enum COMPONENT {
			SCALE,
			ALPHA,
			LAST_COMPONENT
		};

namespace function {
		enum FUNCTION 
		{
			LINEAR,
			SQUARE,
			NONE
		};
	}

		class ComponentParticle {
		public:
			ComponentParticle() {}
			ComponentParticle(float min, float max, pa::COMPONENT name,function::FUNCTION f = function::FUNCTION::NONE) {
				min_max.x = min;
				min_max.y = max;
				current = min;
				function = f;
				this->name = name;
			}
			void setFunction(function::FUNCTION f) {
				function = f;
			}
			void random(std::mt19937 &engine, float &current) {
				std::uniform_real_distribution<float> dist(min_max.x, min_max.y);
				this->current = dist(engine);
				current = this->current;
			}
			 ~ComponentParticle() {}
			pa::COMPONENT name;
			
			void update(float &current, float dt, float life) {//Life should be between 0 and 1
				this->current = current;
				if(function == function::FUNCTION::LINEAR) {
					this->current = (min_max.x - min_max.y)*life + min_max.y;
				}
				if(function == function::FUNCTION::NONE) {

				}
			}
			float getCurrent() { return current;}
		private:
			Vector2f min_max;
			float current;
			function::FUNCTION function;

		};
	}

		struct Particle {
		float life;
		glm::vec2 offset;
		glm::vec2 position;
		glm::vec4 color;
		float scale;
		glm::vec2 velocity;
		float alpha = 1;
		float lifeMax;
		std::array<float, pa::COMPONENT::LAST_COMPONENT> c;
	};

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

		
		void addComponent(pa::COMPONENT name, pa::ComponentParticle &&c) {
			cparticles[name] = c;
		}

		float getComponentValue(pa::COMPONENT name) {
			if(cparticles.find(name) !=  cparticles.end()) return cparticles[name].getCurrent();
			return -1;
		}
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

		std::map<pa::COMPONENT, pa::ComponentParticle> cparticles;

	};
}

//TODO instanciation