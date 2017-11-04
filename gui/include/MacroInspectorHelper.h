#pragma once

#define DECLARE_INSPECTOR_CLASS(name, typeTarget)  \
    class name##Inspector : public Inspector {     \
       public:                                     \
        void draw();                               \
        name##Inspector();                         \
        name##Inspector(BaseComponent* component); \
        void setTarget(BaseComponent* compo);      \
        typeTarget* target;                        \
    };

#define DEFINE_INSPECTOR_FUNCTIONS(name, typeTarget)             \
    name##Inspector::name##Inspector() {                         \
        target = dynamic_cast<typeTarget*>(_component);          \
    }                                                            \
    name##Inspector::name##Inspector(BaseComponent* component) { \
        _component = component;                                  \
        target = dynamic_cast<typeTarget*>(_component);          \
    }                                                            \
    void name##Inspector::setTarget(BaseComponent* compo) {      \
        target = dynamic_cast<typeTarget*>(_component);          \
    }