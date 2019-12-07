#pragma once
#include "Core/Bounds.h"
#include "Core/Math/Vector2.h"
#include <vector>

namespace fm {

template <typename T> struct Data {
    math::vec2 pos;
    T* load;

    Data(math::vec2 pos = math::vec2(), T* data = nullptr)
        : pos(pos)
        , load(data){};
};

template <class T> class QuadTree {
private:
    // 4 children
    QuadTree* nw;
    QuadTree* ne;
    QuadTree* sw;
    QuadTree* se;

    Bounds boundary;

    std::vector<Data<T> > objects;

    unsigned int CAPACITY;
    void subdivide();

public:
    QuadTree<T>();
    QuadTree<T>(Bounds boundary);

    ~QuadTree();

    bool insert(Data<T> d);

    std::vector<Data<T> > queryRange(Bounds range);
};
}

namespace fm {
template <class T> QuadTree<T>::QuadTree() {
    CAPACITY = 4;
    nw = nullptr;
    ne = nullptr;
    sw = nullptr;
    se = nullptr;
    boundary = Bounds();
    objects = std::vector<Data<T> >();
}

template <class T> QuadTree<T>::QuadTree(Bounds boundary) {
    objects = std::vector<Data<T> >();
    CAPACITY = 4;
    nw = nullptr;
    ne = nullptr;
    sw = nullptr;
    se = nullptr;
    this->boundary = boundary;
}

template <class T> QuadTree<T>::~QuadTree() {
    delete nw;
    delete sw;
    delete ne;
    delete se;
}

template <class T> void QuadTree<T>::subdivide() {
    math::vec2 qSize = math::vec2(boundary.getFinalSize().x / 2.0f, boundary.getFinalSize().y / 2.0f);
    math::vec2 qCentre = math::vec2(boundary.center.x - qSize.x, boundary.center.y - qSize.y);
    nw = new QuadTree(Bounds(qCentre, qSize));

    qCentre = math::vec2(boundary.center.x + qSize.x, boundary.center.y - qSize.y);
    ne = new QuadTree(Bounds(qCentre, qSize));

    qCentre = math::vec2(boundary.center.x - qSize.x, boundary.center.y + qSize.y);
    sw = new QuadTree(Bounds(qCentre, qSize));

    qCentre = math::vec2(boundary.center.x + qSize.x, boundary.center.y + qSize.y);
    se = new QuadTree(Bounds(qCentre, qSize));
}

template <class T> bool QuadTree<T>::insert(Data<T> d) {
    // std::cout << d.pos << std::endl;
    if(!boundary.isInside(d.pos)) {

        return false;
    }

    if(objects.size() < CAPACITY) {
        objects.push_back(d);
        return true;
    } 

    if(nw == nullptr) {
        subdivide();
    }

    if(nw->insert(d)) {
        return true;
    }
    if(ne->insert(d)) {
        return true;
    }
    if(sw->insert(d)) {
        return true;
    }
    if(se->insert(d)) {
        return true;
    }

    return false;
}

template <class T> std::vector<Data<T> > QuadTree<T>::queryRange(Bounds range) {
    std::vector<Data<T> > pInRange = std::vector<Data<T> >();

    if(!boundary.intersects2D(range)) {
        return pInRange;
    }

    for(auto&& object : objects) {
        if(range.isInside(object.pos)) {
            pInRange.push_back(object);
        }
    }

    if(nw == nullptr) {
        return pInRange;
    }

    std::vector<Data<T> > temp = nw->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = ne->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = sw->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    temp = se->queryRange(range);
    pInRange.insert(pInRange.end(), temp.begin(), temp.end());

    return pInRange;
}
}