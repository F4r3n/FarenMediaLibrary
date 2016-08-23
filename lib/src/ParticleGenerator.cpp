
#include "ParticleGenerator.h"
#include "ResourcesManager.h"
using namespace fm;
using namespace fm::pa;
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

void ParticleGenerator::setShape(pa::SHAPE shape) {
	this->shape = shape;
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
	
}

void ParticleGenerator::initParticles() {
	for (GLuint i = 0; i < this->numberParticles; ++i) {
		Particle p;
		p.color = { 1,1,1,1 };
		p.life = 100;
		p.position = { positionX, positionY };

		if(shape == SHAPE::CIRCLE) {
			p.velocity.x = velocityMaxX*glm::cos(i*2*glm::pi<float>()/this->numberParticles);
			p.velocity.y = velocityMaxY*glm::sin(i*2*glm::pi<float>()/this->numberParticles);
		}

		p.offset = { 0,0 };
		p.scale = 100;
		this->particles.push_back(p);
	}
}

void ParticleGenerator::reset() {
	int i = 0;
	for (Particle &p : particles) {

		p.color = { 1,1,1,1 };
		p.life = 100;
		p.position = { positionX, positionY };

		if(shape == SHAPE::CIRCLE) {
			p.velocity.x = velocityMaxX*glm::cos(i*2*glm::pi<float>()/this->numberParticles);
			p.velocity.y = velocityMaxY*glm::sin(i*2*glm::pi<float>()/this->numberParticles);
		}

		p.offset = { 0,0 };
		p.scale = 100;
		i++;
	}
}

const std::string ParticleGenerator::getNameShader() const{
	return nameShader;
}

void ParticleGenerator::update(float dt) {
	for (Particle &p : particles) {
		if (p.life > 0.0f) {
			p.velocity.x += gravityX*dt*10;
			p.velocity.y += gravityY*dt*10;

			p.position.x += p.velocity.x*dt;
			p.position.y += p.velocity.y*dt;

			if(p.scale > 0)
				p.scale -= 1;
		}
	}
}

void ParticleGenerator::setVelocity(float fx, float fy) {
	velocityMaxX = fx;
	velocityMaxY = fy;
}

void ParticleGenerator::setGravity(float fx, float fy) {
	gravityX = fx;
	gravityY = fy;
}

void ParticleGenerator::draw(Shader &shader) {
	//std::cout << "Called" << std::endl;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	texture.bind();
	for (Particle p : particles) {
		if (p.life > 0.0f) {
			//std::cout << "life " << p.life << std::endl;
			shader.setFloat("scale", p.scale);
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
}
