#pragma once

#define DECLARE_INSPECTOR_CLASS(name, typeTarget)	\
    class name##Inspector : public Inspector {		\
       public:										\
        void Draw(bool *value, const Entity& inEntity);						\
        name##Inspector();							\
        ~name##Inspector();							\
        name##Inspector(BaseComponent* component);	\
        void SetTarget();							\
		void RemoveComponent(const Entity& inEntity); \
private:											\
        void _Init();								\
		void _DeInit();								\
        typeTarget* _target;						\
   // };

#define DEFINE_INSPECTOR_FUNCTIONS(name, typeTarget)            \
    name##Inspector::name##Inspector() {                        \
        SetTarget();										    \
        _Init();                                                \
    }															\
	name##Inspector::~name##Inspector() {						\
        _DeInit();                                              \
    }															\
																\
    name##Inspector::name##Inspector(BaseComponent* component) {\
        _component = component;                                 \
        SetTarget();										    \
        _Init();                                                \
    }                                                           \
    void name##Inspector::SetTarget() {							\
        _target = static_cast<typeTarget*>(_component);         \
    }
