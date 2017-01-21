#include "ShaderLibrary.h"
#define SHADER_VERSION "#version 330 core\n"
#define S(...) C(SHADER_VERSION, #__VA_ARGS__)
#define C(v, s) v s 
ShaderLibrary::ShaderLibrary() {
    
}

ShaderLibrary::~ShaderLibrary() {
    
}

void ShaderLibrary::loadShaders() {
    std::string text_vertex = S(
                              layout (location = 0) in vec4 vertex;
                              out vec2 TexCoords;
                              uniform mat4 projection;
                                                               layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    };
                              void main(){
                                gl_Position = projection*vec4(vertex.xy, 1.0, 1.0);
                                TexCoords = vertex.zw;
                              });

    std::string text_fragment =S(
        
        layout (location = 0) out vec4 FragColor;
        layout (location = 1) out vec4 BrightColor;
        layout (location = 2) out vec4 posTexture;

        in vec2 TexCoords;
        uniform sampler2D text;
        uniform vec4 textColor;
        uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
        uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703);

        uniform bool outline;
        uniform vec2 outline_min;
        uniform vec2 outline_max;
        uniform vec3 outline_color;

        uniform bool soft_edges;
        uniform vec2 soft_edge_values;

        void main(){
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);

        vec4 color = textColor * sampled;

        vec2 tex_offset = 1.0 / textureSize(text, 0); 
        float result = texture(text, TexCoords).r * weight[0];
        if(soft_edges || outline){
            for(int i = 1; i < 3; ++i)
            {
                result += texture(text, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
                result += texture(text, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
                
                result += texture(text, TexCoords + vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
                result += texture(text, TexCoords - vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
            }
        }
        float oFactor = 1.0f;
        
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

    std::string instancing_vertex = S(
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

    std::string instancing_fragment = "#version 330 core\n"
                                      "in vec2 TexCoords;\n"
                                      "in vec3 Color;"
                                      "out vec4 color;\n"
                                      "void main(){\n"
                                      "color = vec4(1);}";

    std::string blur_fragment =S(

        out vec4 FragColor;
        in vec2 TexCoords;
        
        uniform sampler2D image;
        uniform int horizontal;
        
        uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
        uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703);
        void main()
        {             
             vec2 tex_offset = 1.0 / textureSize(image, 0); 
             vec3 result = texture(image, TexCoords).rgb * weight[0];
             if(horizontal == 1)
             {
                 for(int i = 1; i < 3; ++i)
                 {
                    result += texture(image, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
                    result += texture(image, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
                 }
             }
             else
             {
                 for(int i = 1; i < 3; ++i)
                 {
                     result += texture(image, TexCoords + vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
                     result += texture(image, TexCoords - vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
                 }
             }
             FragColor = vec4(result, 1.0);
        });

    std::string simple_vertex = S(
                                layout(location = 0) in vec2 position;
                                layout(location = 1) in vec2 texCoords;
                                out vec2 TexCoords;
                                 layout (std140) uniform shader_data
                                { 
                                    mat4 FM_V;
                                    mat4 FM_P;
                                    mat4 FM_VP;
                                    };
                                void main(){
                                    gl_Position = vec4(position, 0.0f, 1.0f);
                                    TexCoords = texCoords;
                                });

    std::string light_fragment = S(
                                 layout (location = 0) out vec4 FragColor;
                                 layout (location = 1) out vec4 BrightColor;

                                 in vec2 TexCoords;
                                 uniform sampler2D screenTexture;
                                 uniform sampler2D posTexture;

                                 uniform vec2 screenSize;
                                 uniform vec2 viewPos;

                                 struct PointLight {
                                 vec3 position;
                                 vec4 color;
                                 int ready;
                                 float radius;};

                                 struct DirectionalLight {
                                 vec4 color;
                                 };

                                 const int MAX_LIGHTS = 32;

                                 uniform PointLight light[MAX_LIGHTS];
                                 uniform DirectionalLight dlight;
                                 void main(){

                                 vec4 hdrColor = texture(screenTexture, TexCoords);
                                 vec4 pos = texture(posTexture, TexCoords);
                                 vec3 result = dlight.color.rgb*hdrColor.rgb;
                                 for(int i = 0; i < MAX_LIGHTS; i++){
                                    vec3 dir = vec3(1);
                                    if(light[i].ready == 1) {
                                    dir = normalize(light[i].position - pos.rgb);
                                    }
                                    float ambientStrength = 0.2f;
                                    vec3 ambient = ambientStrength * light[i].color.rgb;
    
                                    vec3 norm = normalize(vec3(0,0,1));
    
                                    float diff = max(dot(norm, dir), 0.0);
                                    vec3 diffuse = diff * light[i].color.rgb;
                                    BrightColor = vec4(0,0,0,1);
                                    float distance    = length(light[i].position - pos.rgb);
                                    float attenuation = 1.0f / (0.9 * distance + 0.0032 * (distance * distance));
                                    result += (ambient*attenuation*255 + diffuse*attenuation*255)*hdrColor.rgb;
                                 }
                                 FragColor = vec4(result, 1);
                                 //FragColor = vec4(1);

                                 float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                                 if(brightness >= 0.5)
                                 BrightColor = vec4(FragColor.rgb, 1.0);
                                 });

    std::string simple_fragment = S(
                                  out vec4 FragColor;
                                  in vec2 TexCoords;
                                  uniform sampler2D screenTexture;
                                  uniform sampler2D bloomBlur;

                                  uniform vec2 screenSize;
                                  uniform vec2 viewPos;
                                  void main(){
                                  const float gamma = 2.2;
                                  const float exposure = 1;
                                  vec4 hdrColor = texture(screenTexture, TexCoords);
                                  vec4 bloomColor = texture(bloomBlur, TexCoords);
                                  vec4 result = hdrColor - bloomColor;
                                  FragColor = vec4(hdrColor.rgb, 1);
                                  });

    std::string default_vertex = S(
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

    std::string default_fragment = S(
                                   layout (location = 0) out vec4 FragColor;
                                   layout (location = 1) out vec4 BrightColor;
                                   layout (location = 2) out vec4 posTexture;

                                   uniform vec4 mainColor;
                                   uniform int BloomEffect;
                                   in vec3 ourPosition;
                                    
                                   void main(){
                                   vec4 color = mainColor;
                                   posTexture = vec4(ourPosition, 1);
                                   BrightColor = vec4(0,0,0,1);FragColor = vec4(0);
                                   float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                               
                                   if(BloomEffect <= 0) FragColor = color;
                                   });

    std::string default_vertex_sprite = S(
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

    std::string default_fragment_sprite = S(
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

    std::string default_vertex_particle = S(
        
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

    std::string default_fragment_particle = S(
                                            in vec4 ourColor;
                                            in vec2 ourTexCoord;
                                            uniform sampler2D texture2d;
                                            layout (location = 0) out vec4 FragColor;
                                            layout (location = 1) out vec4 BrightColor;
                                            void main(){
                                            FragColor = texture(texture2d, ourTexCoord)*ourColor;
                                            });

    fm::ResourcesManager::get().loadShader("light", simple_vertex, light_fragment);
    fm::ResourcesManager::get().loadShader("blur", simple_vertex, blur_fragment);
    fm::ResourcesManager::get().loadShader("text", text_vertex, text_fragment);
    fm::ResourcesManager::get().loadShader("instancing", instancing_vertex, instancing_fragment);
    fm::ResourcesManager::get().loadShader("default", default_vertex, default_fragment);
    fm::ResourcesManager::get().loadShader("simple", simple_vertex, simple_fragment);
    fm::ResourcesManager::get().loadShader("sprite", default_vertex_sprite, default_fragment_sprite);
    fm::ResourcesManager::get().loadShader("particle", default_vertex_particle, default_fragment_particle);
}