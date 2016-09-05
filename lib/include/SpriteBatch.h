#pragma once
#include "Drawable.h"
#include <vector>
#include "Rect.h"
#include "Vector2.h"
#include "Texture.h"
namespace fm {

	struct InfoSprite {
		Vector2f pos = {0,0};
		//glm::vec3 color = {1,1,1};
		//Vector2 size = {100,100}
	};

	class SpriteBatch : public Drawable {
		public:
			SpriteBatch(Texture &texture);
			~SpriteBatch();
			void draw(Shader &shader);
			const std::string getNameShader() const;
			void addSprite(InfoSprite &inf);
			void init();
		private:
			std::string shaderName = "instancing";
			std::vector<InfoSprite> sprites;
			Texture texture;

			GLuint quadVAO, quadVBO;

	};
}