
#include "ParticleGenerator.h"
#include "ResourcesManager.h"
#include <glm/gtc/constants.hpp>
using namespace fm;
using namespace fm::pa;

ParticleGenerator::ParticleGenerator(float posX, float posY, unsigned int numberParticles, Texture &texture)
{
	nameShader = "particle";
	position.x = posX;
	position.y = posY;

	this->numberParticles = numberParticles;
	this->textureName = textureName;
	this->texture = texture;
	

	init();
}

void ParticleGenerator::setShape(pa::SHAPE shape) {
	this->shape = shape;
}

void ParticleGenerator::init() {
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	
}

void ParticleGenerator::resetParticle(Particle &p, int indice) {
		std::mt19937 engine(seeder());

		for(auto &cp : cparticles) {
			cp.second.random(engine, p.c[cp.first]);
		}
		p.color = glm::vec4(1);
		std::uniform_real_distribution<float> dist3(life.x, life.y);
		std::uniform_real_distribution<float> dist4(offsetStartTime.x, offsetStartTime.y);

		p.startTime = dist4(engine);
		p.life = dist3(engine);
		p.lifeMax = p.life;
		p.position = { position.x, position.y };

		std::uniform_real_distribution<float> dist(velocityMin.x, velocityMax.x);
		std::uniform_real_distribution<float> dist2(velocityMin.y, velocityMax.y);


		if(shape == SHAPE::CIRCLE) {
			p.velocity.x = dist(engine)*glm::cos(indice*2*glm::pi<float>()/this->numberParticles);
			p.velocity.y = dist2(engine)*glm::sin(indice*2*glm::pi<float>()/this->numberParticles);
		}

		p.offset = glm::vec2(0);
		p.time = 0;
		//p.scale = 10;
}

void ParticleGenerator::initParticles() {
	for (GLuint i = 0; i < this->numberParticles; ++i) {

		Particle p;

		resetParticle(p, i);
		this->particles.push_back(p);
	}
	over = false;
}

void ParticleGenerator::reset() {
	int i = 0;
	lifeGenerator = lifeGeneratorMax;
	for (Particle &p : particles) {
		resetParticle(p, i);
		i++;
	}
	over = false;
}

void ParticleGenerator::setLifeGenerator(float lifeGenerator) {
	this->lifeGeneratorMax = lifeGenerator;
}

void ParticleGenerator::setLifeParticle(float life) {
	this->life.x = life;
	this->life.y = life;
}

const std::string ParticleGenerator::getNameShader() const{
	return nameShader;
}

void ParticleGenerator::setStartTime(Vector2f offsetTime) {
	offsetStartTime = offsetTime;
}

void ParticleGenerator::update(float dt) {
	int i = 0;
	int j = 0;
	//std::cout << over << std::endl;
	if(over) return;

	for (Particle &p : particles) {
		p.time += dt;
		if(p.startTime > p.time) j = 1;
		//std::cout << p.time << " " << p.startTime << std::endl;

		if (p.life > 0.0f  && p.startTime < p.time) {
			p.velocity.x += gravity.x*dt*10;
			p.velocity.y += gravity.y*dt*10;

			p.position.x += p.velocity.x*dt;
			p.position.y += p.velocity.y*dt;

			for(auto &cp : cparticles) {
				cp.second.update(p.c[cp.first], dt, p.life/p.lifeMax);
			}
			p.life -= dt;

			float s = getComponentValue(pa::COMPONENT::SCALE);
			if(s != -1)
				p.scale = s;
			j++;

		} else if(p.life <= 0.0f) {
			if(lifeGenerator > 0.0) {
				resetParticle(p, i);
			}
		}
		i++;
	}
	if(j == 0) over = true;
	lifeGenerator -= dt;
}

void ParticleGenerator::setVelocity(float fx, float fy) {
	velocityMax.x = velocityMin.x = fx;
	velocityMax.y = velocityMin.y = fy;

}

void ParticleGenerator::setGravity(float fx, float fy) {
	gravity.x = fx;
	gravity.y = fy;
}

void ParticleGenerator::setFading(bool value) {
	this->fading = value;
}

void ParticleGenerator::draw(Shader &shader) {
	//std::cout << "Called" << std::endl;
	//if(lifeGenerator < 0.0) return;
	if(over) return;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	texture.bind();
	for (Particle p : particles) {
		if (p.life > 0.0f && p.startTime < p.time) {
			//std::cout << "Draw" << std::endl;
			//std::cout << "life " << p.life << std::endl;
			if(fading)
				p.color.w = p.life/p.lifeMax;
			shader.setFloat("scale", p.c[pa::COMPONENT::SCALE]);
			shader.setVector2f("offset", p.position);
			shader.setVector4f("particleColor", p.color);
			

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


ParticleGenerator::~ParticleGenerator()
{
	glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
