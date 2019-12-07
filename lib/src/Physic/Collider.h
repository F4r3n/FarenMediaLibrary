#pragma once
#include <cstddef>
enum EVENT_COLLISION {BEGIN, END};

 struct Collider {
     Collider(size_t idA, size_t idB, EVENT_COLLISION event) {
         this->idA = idA;
         this->idB = idB;
         this->event = event;
     }
        size_t idA;
        size_t idB;
        EVENT_COLLISION event;
    };
    
    struct ColliderInfo {
        size_t idOther;
    };
