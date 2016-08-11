#include "stdafx.h"
#include "ParticleGenerator.h"
#include "ResourcesManager.h"
using namespace fm;
ParticleGenerator::ParticleGenerator(float posX, float posY, unsigned int numberParticles, Texture &texture)
{
	nameShader = "particle";
	this->positionX = posX;
	this->positionY = posY;
	this->numberParticles = numberParticles;
	this->textureName = textureName;
	this->texture = texture;
//	ResourcesManager::loadTexture(textureName, textureName);
	//particles = std::vector<Particle>{ numberParticles };
	init();
}

void ParticleGenerator::init() {
	GLuint VBO;
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

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->numberParticles; ++i) {
		Particle p;
		p.color = { 1,1,1,1 };
		p.life = 1;
		p.position = { positionX, positionY };
		p.velocity = i;
		p.offset = { 0,0 };
		this->particles.push_back(p);
	}
}

const std::string ParticleGenerator::getNameShader() const{
	return nameShader;
}

void ParticleGenerator::update() {
	for (Particle &p : particles) {
		if (p.life > 0.0f) {
			p.position.x += p.velocity/100;
			p.position.y += p.velocity/100;
		}
	}
}

void ParticleGenerator::draw(Shader &shader) {
	//std::cout << "Called" << std::endl;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	for (Particle p : particles) {
		if (p.life > 0.0f) {
			//std::cout << "life " << p.life << std::endl;
			shader.setFloat("scale", 10);
			shader.setVector2f("offset", p.position);
			shader.setVector4f("particleColor", p.color);
			texture.bind();

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


ParticleGenerator::~ParticleGenerator()
{
}
