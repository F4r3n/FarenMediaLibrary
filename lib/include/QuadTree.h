#pragma once
#include "Rect.h"
#include <memory>
#include <array>
#include <vector>
#include <iostream>
#include <memory>
#define MAX_LEVELS 1000
#define MAX_OBJECTS 100
namespace fm {
struct Object {
    size_t idObject;
    Recti rect;
};

class QuadTree {
public:
    QuadTree() {
    }
    QuadTree(unsigned int level, const Recti& rect)
        : rect(rect)
        , level(level) {
        setRect(rect);
        //std::cout << "Init quadTree" << std::endl;
        for(int i = 0; i < 4; ++i) {
            nodes[i] = std::make_unique<QuadTree>();
            nodes[i]->setDataQuadTree(level + 1, rects[i]);
        }
    }

    void setLevel(unsigned int level) {
        this->level = level;
    }
    void setRect(const Recti& rect) {
        rects[0] = Recti(rect.x, rect.y, rect.w / 2, rect.h / 2);
        rects[1] = Recti(rect.x + rect.w / 2, rect.y, rect.w / 2, rect.h / 2);
        rects[2] = Recti(rect.x, rect.y + rect.h / 2, rect.w / 2, rect.h / 2);
        rects[3] = Recti(rect.x + rect.w / 2, rect.y + rect.h / 2, rect.w / 2, rect.h / 2);
        
        //for(int i = 0;i <4; i++) {
        //    std::cout << "Level " << level << " " << rects[i] << std::endl;
        //}
        this->rect = rect;
    }

    void setDataQuadTree(unsigned int level, const Recti& rect) {
        setLevel(level);
        setRect(rect);
    }

    bool insert(const Object& object) {
       // std::cout << object.rect.x << " " << object.idObject << " " << level << std::endl;
        if(level == MAX_LEVELS)
            return false;
        for(int i = 0; i < rects.size(); ++i) {
            if(rects[i].contains(object.rect)) {
                if(level == 0) {
                    return nodes[i]->insert(object);
                }
                else if(level != 0 && getNumberElements() < MAX_OBJECTS) {
                  //  std::cout << "Rect " << i << " " << rects[i].x << " " << rects[i].w << " " << std::endl;
                    data.push_back(object);
                    return true;
                }
                else if(getNumberElements() >= MAX_OBJECTS) {
                    nodes[i] = std::make_unique<QuadTree>(level + 1, this->rect);
                    nodes[i]->insert(object);

                    int j = 0;
                    for(auto o : data) {
                        nodes[i]->insert(o);
                        j++;
                    }
                    data.clear();
                    return true;
                }
            }
        }
        return false;
    }

    int getNumberElements() const {
        return data.size();
    }

    const std::vector<Object>* getData(Recti rect) {
        for(int i = 0; i < rects.size(); ++i) {

            if(rects[i].contains(rect)) {
                if(level == 0) {
                    return nodes[i]->getData(rect);
                } else if(!nodes[i]) {
                    return &data;
                }
            }
        }
        return nullptr;
    }

    void clear() {
        data.clear();
        for(int i = 0; i < 4; i++) {
            if(nodes[i]) {
                nodes[i]->clear();
                if(level != 0)
                nodes[i] = nullptr;
            }
        }
    }

private:
    Recti rect;
    unsigned int level;
    std::array<std::unique_ptr<QuadTree>, 4> nodes;
    std::array<Recti, 4> rects;
    std::vector<Object> data;
};
}