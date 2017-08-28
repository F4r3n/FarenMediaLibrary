#include "gtest/gtest.h"
#include "ECS.h"
int size = 1000;
class AllocTest : public ::testing::Test {
public:
    std::vector<Entity*> entities;

    void SetUp() {
        for(int i = 0; i < size; ++i) {
            entities.push_back(EntityManager::get().createEntity());
        }
        EntityManager::get().make();
    }
    
    void TearDown() {
        
        EntityManager::get().killAll();
    }
   
};



TEST_F(AllocTest, Destroy) {

    for(int i = 0; i < size; ++i) {
        entities[i]->destroy();
    }
}

TEST_F(AllocTest, SecondAlloc) {
   for(int i = 0; i < size; ++i) {
        entities[i]->destroy();
    }
    for(int i = 0; i < size; ++i) {
       entities.push_back(EntityManager::get().createEntity());
    }
    EntityManager::get().make();
   
}



TEST_F(AllocTest, AllocDestroy) {

    for(int i = 0; i < size/2; ++i) {
        entities[i]->destroy();
    }
    for(int i = 0; i < size/2; ++i) {
       entities.push_back(EntityManager::get().createEntity());
    }
    for(int i = 0; i < size/2; ++i) {
        entities[i]->destroy();
    }
}
