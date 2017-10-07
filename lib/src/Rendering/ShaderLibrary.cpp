#include "Rendering/Shader.h"
#include "Rendering/ShaderLibrary.h"
#include "Core/Config.h"
#include "Rendering/ShaderParser.h"

#if OPENGL_CORE == 1
#define SHADER_VERSION "#version 330 core\n"
#else
#define SHADER_VERSION "#version 310 es\n"
#endif

#if OPENGL_CORE == 1
#define FLOAT 
#define INT 
#elif OPENGL_ES
#define FLOAT highp
#define INT mediump
#endif

#define C(v, s) v s 
#define S(...) C(SHADER_VERSION, #__VA_ARGS__)
#define STRING(...) S(__VA_ARGS__)
ShaderLibrary::ShaderLibrary() {
    
}

ShaderLibrary::~ShaderLibrary() {
    
}

void ShaderLibrary::loadShaders() {
    std::string text_vertex = STRING(
                                layout(location = 0) in vec2 position;
                                layout(location = 1) in vec2 texCoords;
                              out vec2 TexCoords;
                              uniform mat4 projection;
                                    layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    };
                              void main(){
                                gl_Position = projection*vec4(position, 1.0, 1.0);
                                TexCoords = texCoords;
                              });

    std::string text_fragment =STRING(

        layout (location = 0) out FLOAT vec4 FragColor;
        layout (location = 1) out FLOAT vec4 BrightColor;
        layout (location = 2) out FLOAT vec4 posTexture;

        in FLOAT vec2 TexCoords;
        uniform sampler2D text;
        uniform FLOAT vec4 textColor;
        
        uniform bool outline;
        uniform FLOAT vec2 outline_min;
        uniform FLOAT vec2 outline_max;
        uniform FLOAT vec3 outline_color;

        uniform bool soft_edges;
        uniform FLOAT vec2 soft_edge_values;

        void main(){
                FLOAT vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
                FLOAT vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
                FLOAT vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                FLOAT vec4 color = textColor * sampled;

                FLOAT vec2 tex_offset = 1.0f / vec2(textureSize(text, 0)); 
                FLOAT float result = texture(text, TexCoords).r * weight[0];
                if(soft_edges || outline){
                    for(INT int i = 1; i < 3; ++i)
                    {
                        result += texture(text, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
                        result += texture(text, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
                        
                        result += texture(text, TexCoords + vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
                        result += texture(text, TexCoords - vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
                    }
                }
                FLOAT float oFactor = 1.0f;
                
                if(outline && result >= outline_min.x && result <= outline_max.y) {
                    if(result <= outline_min.y) {
                        oFactor = smoothstep(outline_min.x, outline_min.y, result);
                    } else {
                        oFactor = smoothstep(outline_max.x, outline_max.y, result);
                    }
                color = mix(color, vec4(outline_color,1), oFactor);
                }
                if(soft_edges){
                    color.a += smoothstep(soft_edge_values.x,soft_edge_values.y, result);
                }
        
                FragColor = color;
        });

    std::string instancing_vertex = STRING(
                                    layout (location = 0) in vec2 position;
                                    layout (location = 1) in vec3 color;
                                    layout (location = 3) in mat4 instanceMatrix;
                                    out vec2 TexCoords;
                                    out vec3 Color;
                                    uniform mat4 view;
                                    uniform mat4 projection;
                                    
                                layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    int render_mode;
                                    };
                                    void main(){
                                    vec2 colorTest = vec2(1);
                                    gl_Position = projection*view*instanceMatrix*vec4(position, 0.0f, 1.0f);
                                    Color = color;
                                    TexCoords = colorTest;});

    std::string instancing_fragment = STRING(
            #if OPENGL_ES == 1
        precision highp float;

        #endif
                                      in vec2 TexCoords;
                                      in vec3 Color;
                                      out vec4 color;
                                      void main(){
                                      color = vec4(1);});

    std::string blur_fragment =STRING(

        out FLOAT vec4 FragColor;
        in  FLOAT vec2 TexCoords;
        
        uniform sampler2D image;
        uniform INT int horizontal;

        void main()
        {             
             FLOAT vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
             FLOAT vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
             FLOAT vec2 tex_offset = 1.0f / vec2(textureSize(image, 0)); 
             FLOAT vec3 result = texture(image, TexCoords).rgb * weight[0];
             if(horizontal == 1)
             {
                 for(INT int i = 1; i < 3; ++i)
                 {
                    result += texture(image, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
                    result += texture(image, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
                 }
             }
             else
             {
                 for(INT int i = 1; i < 3; ++i)
                 {
                     result += texture(image, TexCoords + vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
                     result += texture(image, TexCoords - vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
                 }
             }
             FragColor = vec4(result, 1.0);
        });



    std::string light_fragment = STRING(

                                 layout (location = 0) out FLOAT vec4 FragColor;
                                 layout (location = 1) out FLOAT vec4 BrightColor;

                                 FLOAT in vec2 TexCoords;
                                 uniform sampler2D screenTexture;
                                 uniform sampler2D posTexture;

                                 INT uniform vec2 screenSize;
                                 INT uniform vec2 viewPos;

                                 struct PointLight {
                                    FLOAT vec3 position;
                                    FLOAT vec4 color;
                                    INT int ready;
                                    FLOAT float radius;
                                 };

                                 struct DirectionalLight {
                                    FLOAT vec4 color;
                                 };

                                 const INT int MAX_LIGHTS = 32;

                                 uniform PointLight light[MAX_LIGHTS];
                                 uniform DirectionalLight dlight;
                                 void main() {

                                    FLOAT vec4 hdrColor = texture(screenTexture, TexCoords);
                                    FLOAT vec4 pos = texture(posTexture, TexCoords);
                                    FLOAT vec3 result = dlight.color.rgb*hdrColor.rgb;
                                    
                                    for(INT int i = 0; i < MAX_LIGHTS; i++){
                                        FLOAT vec3 dir = vec3(1);
                                        if(light[i].ready == 1) {
                                            dir = normalize(light[i].position - pos.rgb);
                                        }
                                        FLOAT float ambientStrength = 0.2f;
                                        FLOAT vec3 ambient = ambientStrength * light[i].color.rgb;
                                        
                                        FLOAT vec3 norm = normalize(vec3(0,0,1));
                                        
                                        FLOAT float diff = max(dot(norm, dir), 0.0);
                                        FLOAT vec3 diffuse = diff * light[i].color.rgb;
                                        FLOAT float distance    = length(light[i].position - pos.rgb);
                                        FLOAT float attenuation = 1.0f / (0.9f * distance + 0.0032f * (distance * distance));
                                        result += (ambient*attenuation*255.0f + diffuse*attenuation*255.0f)*hdrColor.rgb;
                                    }
                                    BrightColor = vec4(0,0,0,1);
                                    FragColor = vec4(result ,1);
                                    //FragColor = vec4(1);
    
                                    FLOAT float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                                    if(brightness >= 0.5)
                                    BrightColor = vec4(FragColor.rgb, 1.0);
                                 });
                                 
                    std::string light_fragment_no_light = STRING(
                            #if OPENGL_ES == 1
        precision highp float;

        #endif
                                 layout (location = 0) out vec4 FragColor;
                                 layout (location = 1) out vec4 BrightColor;

                                 in vec2 TexCoords;
                                 uniform sampler2D screenTexture;
                                 uniform sampler2D posTexture;

                                 uniform vec2 screenSize;
                                 uniform vec2 viewPos;

                               
                                 void main() {

                                    vec4 hdrColor = texture(screenTexture, TexCoords);
                                    vec4 pos = texture(posTexture, TexCoords);
                                   
                                    FragColor = hdrColor;
  
                                 });     

    std::string simple_vertex = STRING(
                                layout(location = 0) in vec2 position;
                                layout(location = 1) in vec2 texCoords;
                                out vec2 TexCoords;
                                 layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    int render_mode;
                                    };
                                void main(){
                                    gl_Position = vec4(position, 0.0f, 1.0f);
                                    TexCoords = texCoords;
                                });            

    std::string simple_fragment = STRING(

                                  FLOAT out vec4 FragColor;
                                  FLOAT in vec2 TexCoords;
                                  uniform sampler2D screenTexture;
                                  uniform sampler2D bloomBlur;

                                  INT uniform vec2 screenSize;
                                  INT uniform vec2 viewPos;
                                  void main(){
                                    const FLOAT float gamma = 2.2f;
                                    const FLOAT float exposure = 1.0f;
                                    FLOAT vec4 hdrColor = texture2D(screenTexture, TexCoords);
                                    FLOAT vec4 bloomColor = texture2D(bloomBlur, TexCoords);
                                    FLOAT vec4 result = hdrColor - bloomColor;
                                    FragColor = vec4(hdrColor.rgb, 1);
                                    //FragColor = vec4(1);

                                  });

    std::string default_vertex = STRING(
                                 layout(location = 0) in vec2 position;
                                 layout(location = 1) in vec2 texCoords;
                                 uniform mat4 FM_M;
                                 uniform mat4 FM_PVM;
                                 
                                 layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_PV;
                                    int render_mode;

                                    };

                                 out vec3 ourPosition;
                                 void main(){
                                     
                                 vec4 screenPos = vec4(position, 1.0f, 1.0f);
                                 gl_Position = FM_PVM*screenPos;
                                 ourPosition = (FM_M*screenPos).xyz;
                                 });

    std::string default_fragment = STRING(
            #if OPENGL_ES == 1
        precision highp float;

        #endif
                                   layout (location = 0) out vec4 FragColor;
                                   layout (location = 1) out vec4 BrightColor;
                                   layout (location = 2) out vec4 posTexture;

                                   uniform vec4 mainColor;
                                   uniform int BloomEffect;
                                   in vec3 ourPosition;
                                   
                                   void main(){
                                   vec4 color = mainColor;
                                   posTexture = vec4(ourPosition, 1);
                                   BrightColor = vec4(0,0,0,1);
                                   FragColor = vec4(0);
                                   float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                               
                                   if(BloomEffect <= 0) FragColor = color;
                                   });
                                   /*                                       if(render_mode == 1) {
                                   FragColor = mainColor;
                                   
                                       } else {
                                           vec4 color = mainColor;
                                           posTexture = vec4(ourPosition, 1);
                                           BrightColor = vec4(0,0,0,1);FragColor = vec4(0);
                                           float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                                           
                                           if(BloomEffect <= 0) FragColor = color;
                                       }*/
                                       


    std::string default_vertex_sprite = STRING(
                                        layout(location = 0) in vec2 position;
                                        layout(location = 1) in vec2 texCoords;

                                        
                                        uniform mat4 model;
                                        

                                        uniform mat4 FM_PV;
                                        
                                        out vec4 ourColor;
                                        out vec2 ourTexCoord;
                                        out vec3 ourPosition;
                                 layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    };
                                        void main(){
                                        vec4 worldPos = model*vec4(position, 1.0f, 1.0f);
                                        gl_Position = FM_PV*worldPos;
                                        ourPosition = worldPos.xyz;
                                        ourTexCoord = texCoords;
                                        });

    std::string default_fragment_sprite = STRING(
            #if OPENGL_ES == 1
        precision highp float;

        #endif
                                          layout (location = 0) out vec4 FragColor;
                                          layout (location = 1) out vec4 BrightColor;
                                          layout (location = 2) out vec4 posTexture;
                                          
                                          uniform vec4 mainColor;
                                          in vec2 ourTexCoord;
                                          in vec3 ourPosition;
                                          uniform int BloomEffect;
                                          uniform sampler2D texture2d;
                                          out vec4 Color;
                                          
                                          void main(){
                                          vec4 color = texture(texture2d, ourTexCoord)*mainColor;
                                          BrightColor = vec4(0,0,0,1);
                                          posTexture = vec4(ourPosition, 0);
                                          
                                          if(color.w == 0.0f) discard;
                                          
                                          
                                          
                                          BrightColor = vec4(0,0,0,1);FragColor = vec4(0);
                                          if(BloomEffect <= 0) {
                                              FragColor = color; 
                                          }else {
                                              BrightColor = color;
                                          }
                                          });

    std::string default_vertex_particle = STRING(
        
        layout (location = 0) in vec4 vertex;
        uniform mat4 view;
        uniform mat4 projection;
        uniform float scale;
        uniform vec2 offset;
        uniform vec4 particleColor;
        
        out vec4 ourColor;
        out vec2 ourTexCoord;
                                         layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    };
        void main(){
            gl_Position = projection*view*vec4((vertex.xy*scale) + offset, 0.0f, 1.0f);
            ourTexCoord = vertex.zw;
            ourColor = particleColor;
        });

    std::string default_fragment_particle = STRING(
                #if OPENGL_ES == 1
        precision highp float;
        #endif
                                            in vec4 ourColor;
                                            in vec2 ourTexCoord;
                                            uniform sampler2D texture2d;
                                            layout (location = 0) out vec4 FragColor;
                                            layout (location = 1) out vec4 BrightColor;
                                            void main(){
                                            FragColor = texture(texture2d, ourTexCoord)*ourColor;
                                            });
/*#if OPENGL_ES_VERSION == 2
    fm::ResourcesManager::get().loadShader("no_light", simple_vertex, light_fragment_no_light);
    fm::ResourcesManager::get().loadShader("default", default_vertex, default_fragment);
    fm::ResourcesManager::get().loadShader("simple", simple_vertex, simple_fragment);
    
    std::shared_ptr<fm::Shader> s = fm::ResourcesManager::get().getShader("simple");
    s->Use()->setInt("screenTexture", 0)->setInt("bloomBlur", 1);
    
    std::shared_ptr<fm::Shader> light = fm::ResourcesManager::get().getShader("no_light");
    light->Use()->setInt("screenTexture", 0)->setInt("posTexture", 1);
#else*/
    
    fm::ResourcesManager::get().load<fm::Shader>("light", new fm::Shader(simple_vertex, light_fragment));
    fm::ResourcesManager::get().load<fm::Shader>("no_light", new fm::Shader(simple_vertex, light_fragment_no_light));
  
    fm::ResourcesManager::get().load<fm::Shader>("blur", new fm::Shader(simple_vertex, blur_fragment));
    fm::ResourcesManager::get().load<fm::Shader>("text", new fm::Shader(text_vertex, text_fragment));
    fm::ResourcesManager::get().load<fm::Shader>("instancing", new fm::Shader(instancing_vertex, instancing_fragment));
    fm::ResourcesManager::get().load<fm::Shader>("default", new fm::Shader(default_vertex, default_fragment));
    fm::ResourcesManager::get().load<fm::Shader>("simple", new fm::Shader(simple_vertex, simple_fragment));
    //fm::ResourcesManager::get().loadShader("sprite", default_vertex_sprite, default_fragment_sprite);
    //fm::ResourcesManager::get().loadShader("particle", default_vertex_particle, default_fragment_particle);
    for(auto shader : fm::ResourcesManager::get().getAll<fm::Shader>()) {
        fm::Shader *s = dynamic_cast<fm::Shader*>(shader.second); 
        if(s != nullptr && !s->IsReady()) {
            s->compile();
        }
    }
    
    
    fm::Shader *s = fm::ResourcesManager::get().getResource<fm::Shader>("simple");
    s->Use()->setValue("screenTexture", 0)->setValue("bloomBlur", 1);
  
    fm::Shader *light = fm::ResourcesManager::get().getResource<fm::Shader>("light");
    light->Use()->setValue("screenTexture", 0)->setValue("posTexture", 1);
    
    light = fm::ResourcesManager::get().getResource<fm::Shader>("no_light");
    light->Use()->setValue("screenTexture", 0)->setValue("posTexture", 1);
//#endif
    
}

void ShaderLibrary::loadShader(const std::string& name, const std::string &path ) {
    fm::ShaderParser parser;

    std::tuple<std::string, std::string> partShader = parser.parse(path);
    fm::ResourcesManager::get().load<fm::Shader>(name, new fm::Shader(std::get<0>(partShader), std::get<1>(partShader)));
}
