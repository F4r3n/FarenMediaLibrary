    #pragma once
 namespace fm {   
         enum RESOURCE_TYPE {
        TEXTURE,
        FONT,
        MESH,
        SHADER,
        MATERIAL,
        LAST_RESOURCE,
        NONE
    };
    
    class BaseResource {
    public: 
      BaseResource() {};
      virtual ~BaseResource() {}
    };
    
    
    class Resource {
    public:
        Resource() {}
        virtual ~Resource() {}

    protected:
    };
    

    
 }
