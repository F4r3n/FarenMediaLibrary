    
 namespace fm {   
         enum RESOURCE_TYPE {
        TEXTURE,
        FONT,
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
        Resource() {};
        virtual ~Resource() {};
        static RESOURCE_TYPE type;

    protected:
    };
    

    
 }
