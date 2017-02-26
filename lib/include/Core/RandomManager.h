#pragma once
#include <random>
class RandomManager {
public:

	~RandomManager();
	static RandomManager& getInstance();
	int uniform_int_distribution(int a, int b);
	float uniform_real_distribution(float a, float b);
private:
	RandomManager();
	static RandomManager _instance;
	 std::random_device r;
};
