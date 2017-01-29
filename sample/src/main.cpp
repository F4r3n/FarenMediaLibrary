#include <iostream>
#include <Window.h>
#include <Camera.h>
#include <Rectangle.h>
#include <InputManager.h>
#include <ParticleGenerator.h>
#include <Sprite.h>
#include <LuaScript.h>
#include <Time.h>
#include <chrono>
#include <ScriptManager.h>
#include <Vector2.h>
#include <Input.h>
#include <PostProcessor.h>
#include <Text.h>

#include <Source.h>
	class CTest : public Component<CTest>{
public:
	CTest() {}
	~CTest() {}
	float test = 0;
};
int main() {

	//fm::Speaker speaker;
	fm::Source source;
	source.loadAudio("piano.wav");
	source.setVolume(0.5f);
	source.setLoop(true);
	//while(1);


	fm::Window window(800, 600, "test");
	fm::PostProcessor postProcess;
	bool result = postProcess.setShader("simple", "uniform float test; \nvec4 effect(vec4 color, vec2 TexCoords) { return color*vec4(TexCoords,test,1);}");
	postProcess.getShader("simple").Use()->setFloat("test",1);
	//postProcess.activate("simple");
	std::cout << "Post process effect " << result << std::endl;

	fm::Text text(800, 600);
	text.loadFont("Arial_Alternatives/Anke/Anke.ttf", 24);
	text.setText("salut");
	fm::Camera camera(800, 600);


	fm::Texture texture("t2.png",{0,0,-1,-1});
	//fm::Texture texture2("t2.png", true);
	fm::ParticleGenerator particle(100,100, 1000, texture);
	particle.setGravity(0,10);
	particle.setVelocity(fm::Vector2<float>(10,10), fm::Vector2<float>(100,100));
	particle.setShape(fm::pa::SHAPE::CIRCLE);
	particle.setLife(fm::Vector2f(1,5));
	particle.setFading(true);
	particle.setLifeGenerator(5);
	particle.setStartTime(fm::Vector2f(0.1,1));
	particle.addComponent(fm::pa::COMPONENT::SCALE, fm::pa::ComponentParticle(10,100, fm::pa::COMPONENT::SCALE, fm::pa::function::FUNCTION::LINEAR));
	particle.initParticles();
	fm::Sprite sprite(texture);
	sprite.scale(100,100);

	fm::ScriptManager scriptManager;
	scriptManager.registerScript("../assets/test.lua");
	scriptManager.registerComponent<CTest>("CTest", "test", &CTest::test);

	scriptManager.init();
	scriptManager.start();

	fm::Vector2d posMouse;
	source.play();
	while(!window.isClosed()) {
		fm::Input::getMousePosition(posMouse);

		//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    	window.update(60);

		particle.update(fm::Time::dt);
		//object.update(fm::Time::dt);
		scriptManager.update(fm::Time::dt);
		//CTest test = scriptManager.getComponent<CTest>("../assets/test.lua", "CTest");
		//std::cout << test.test << std::endl;
		sprite.move(scriptManager.getScript("../assets/test.lua")->getTransform().position); 
		//source.setPosition(scriptManager.getScript("../assets/test.lua")->getTransform().position);

		camera.view();
		camera.draw(sprite);
		camera.draw(particle);
		camera.draw(text);

		window.swapBuffers();


	}
	texture.clear();

	return 0;
}