#pragma once
#include "Rect.h"
#include <memory>
#include <array>
#include <vector>
#define MAX_LEVELS 10
#define MAX_OBJECTS 100
namespace fm
{
template <typename U>
class Object {
    public:
    U object;
    Recti rect;
};    
    
    
template <class T> class QuadTree
{
public:
    QuadTree()
    {
    }
    QuadTree(unsigned int level, const Recti& rect)
        : rect(rect)
        , level(level)
    {

        std::shared_ptr<QuadTree> n0 = std::make_shared<QuadTree<T> >();
        n0->setDataQuadTree(level + 1, rects[0]);
        nodes[0] = n0;

        std::shared_ptr<QuadTree> n1 = std::make_shared<QuadTree<T> >();
        n1->setDataQuadTree(level + 1, rects[1]);
        nodes[1] = n1;

        std::shared_ptr<QuadTree> n2 = std::make_shared<QuadTree<T> >();
        n2->setDataQuadTree(level + 1, rects[2]);
        nodes[2] = n2;

        std::shared_ptr<QuadTree> n3 = std::make_shared<QuadTree<T> >();
        n3->setDataQuadTree(level + 1, rects[3]);
        nodes[3] = n3;
    }

    void setLevel(unsigned int level)
    {
        this->level = level;
    }
    void setRect(const Recti& rect)
    {
        rects[0] = Recti(rect.x, rect.y, rect.w / 2, rect.h / 2);
        rects[1] = Recti(rect.x + rect.w / 2, rect.y, rect.w / 2, rect.h / 2);
        rects[2] = Recti(rect.x, rect.y + rect.h / 2, rect.w / 2, rect.h / 2);
        rects[3] = Recti(rect.x + rect.w / 2, rect.y + rect.h / 2, rect.w / 2, rect.h / 2);
        this->rect = rect;
    }

    void setDataQuadTree(unsigned int level, const Recti& rect)
    {
        setLevel(level);
        setRect(rect);
    }
    void insert(std::shared_ptr<T> object, const Recti& rect)
    {
        for(int i = 0; i < rects.size(); ++i) {
            if(rects[i].contains(rect)) {
                if(nodes[i] != nullptr) {
                    nodes[i]->insert(object, rect);
                } else if(nodes[i] == nullptr && getNumberElements() < MAX_OBJECTS) {
                    data.push_back(object);
                    return;
                } else if(nodes[i] == nullptr && getNumberElements() >= MAX_OBJECTS) {
                    nodes[i] = std::make_shared<QuadTree<T> >();
                    nodes[i]->setDataQuadTree(level + 1, this->rect);
                    nodes[i]->insert(object, rect);
                    
                    for(Object<T> o : data) {
                        insert(o->object, o->rect);
                    }
                }
            }
        }
    }

    int getNumberElements() const
    {
        return data.size();
    }

private:
    Recti rect;
    unsigned int level;
    std::array<std::shared_ptr<QuadTree<T> >, 4> nodes;
    std::array<Recti, 4> rects;
    std::vector<std::shared_ptr<Object<T>> > data;
};
}