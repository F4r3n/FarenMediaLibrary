#pragma once

#define DECLARE_INSPECTOR_CLASS(name, typeTarget)	\
    class name##Inspector : public Inspector<typeTarget> {		\
       public:										\
        virtual void Draw(bool *value, const typeTarget& inTarget, std::shared_ptr<fm::GameObject> inGameObject) override;						\
		void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override; \
private:											\
   // };


