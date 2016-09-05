#include "SpriteBatch.h"

using namespace fm;
SpriteBatch::SpriteBatch(Texture &texture) {
	this->texture = texture;
}
SpriteBatch::~SpriteBatch() {}

void SpriteBatch::draw(Shader &shader) {

   		//shader.Use();
   		//glActiveTexture(GL_TEXTURE0);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
		//texture.bind();
        glBindVertexArray(quadVAO);
        //glDrawElementsInstanced(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0, sprites.size());
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, sprites.size()); // 100 triangles of 6 vertices each
        glBindVertexArray(0);
}

void SpriteBatch::addSprite(InfoSprite &inf) {
	sprites.push_back(inf);
}
void SpriteBatch::init() {
	//unsigned int indices[] = { 0,1,2,0,3,1 };
	    GLfloat quadVertices[] = {
        // Positions   // Colors
        0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  0.0f,  0.0f, 1.0f, 0.0f,


        0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f	
    };

    std::vector<glm::mat4> v;

    for(InfoSprite s : sprites) {

   		glm::mat4 model;
    	float scaleX = s.scale.x;
    	float scaleY = s.scale.y;
    	float posX = s.pos.x;
    	float posY = s.pos.y;
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(posX, posY,  0.0f));
	
	
		model = glm::translate(model, glm::vec3(0.5f * scaleX, 0.5f * scaleY, 0.0f));
			//test[0] = glm::rotate(test[0], rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * scaleX, -0.5f * scaleY, 0.0f));
	
		model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
		v.push_back(model);

	}
    //test[0] = glm::mat4(1);

	glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));


    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * v.size(), v.data(), GL_STATIC_DRAW);
    

    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

const std::string SpriteBatch::getNameShader() const {
	return shaderName;
} 