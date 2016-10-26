
#include "Window.h"
#include "Time.h"
#include <chrono>
#include <thread>
#include "Renderer.h"
#include "InputManager.h"
#include "ResourcesManager.h"
#include "CMesh.h"
#include "RFont.h"
using namespace fm;

Window::Window(int width, int height, const std::string& name)
    : width(width)
    , height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    init(window);

    initFrameBuffer();

    createQuadScreen();
    createShaders();
    // glEnable (GL_BLEND);

    fm::InputManager::getInstance().init(this->window);
}

void Window::createShaders() {
    std::string text_vertex = "#version 330 core\n"
                              "layout (location = 0) in vec4 vertex;\n"
                              "out vec2 TexCoords;\n"
                              "uniform mat4 projection;\n"
                              "void main(){\n"
                              "gl_Position = projection*vec4(vertex.xy, 1.0, 1.0);\n"
                              "TexCoords = vertex.zw;}\n";

    std::string text_fragment =
        "#version 330 core\n"
        "layout (location = 0) out vec4 FragColor;\n"
        "layout (location = 1) out vec4 BrightColor;\n"
        "layout (location = 2) out vec4 posTexture;\n"

        "in vec2 TexCoords;\n"
        "uniform sampler2D text;\n"
        "uniform vec4 textColor;\n"
        "uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );\n"
        "uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703);\n"

        "uniform bool outline;\n"
        "uniform vec2 outline_min;\n"
        "uniform vec2 outline_max;\n"
        "uniform vec3 outline_color;\n"

        "uniform bool soft_edges;\n"
        "uniform vec2 soft_edge_values;\n"

        "void main(){\n"
        "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"

        "vec4 color = textColor * sampled;"

        "vec2 tex_offset = 1.0 / textureSize(text, 0); // gets size of single texel\n"
        "float result = texture(text, TexCoords).r * weight[0];\n"
        "if(soft_edges || outline){\n"
        "for(int i = 1; i < 3; ++i)\n"
        "{\n"
        "   result += texture(text, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];\n"
        "   result += texture(text, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];\n"

        "    result += texture(text, TexCoords + vec2(0.0, tex_offset.y * offset[i])).r * weight[i];\n"
        "    result += texture(text, TexCoords - vec2(0.0, tex_offset.y * offset[i])).r * weight[i];\n"
        "}}\n"
        "float oFactor = 1.0f;"

        "if(outline && result >= outline_min.x && result <= outline_max.y) {\n"
        "    if(result <= outline_min.y) {\n"
        "        oFactor = smoothstep(outline_min.x, outline_min.y, result);\n"
        "\n"
        "    } else {\n"
        "        oFactor = smoothstep(outline_max.x, outline_max.y, result);\n"
        "    }\n"
        "color = mix(color, vec4(outline_color,1), oFactor);\n"
        "}\n"
        "if(soft_edges)\n{"
        "color.a += smoothstep(soft_edge_values.x,soft_edge_values.y, result);}\n "

        "FragColor = color;\n"
        "}";

    std::string instancing_vertex = "#version 330 core\n"
                                    "layout (location = 0) in vec2 position;\n"
                                    "layout (location = 1) in vec3 color;\n"
                                    "layout (location = 3) in mat4 instanceMatrix;\n"
                                    "out vec2 TexCoords;\n"
                                    "out vec3 Color;\n"
                                    "uniform mat4 view;\n"
                                    "uniform mat4 projection;\n"
                                    "void main(){\n"
                                    "vec2 colorTest = vec2(1);\n"
                                    "gl_Position = projection*view*instanceMatrix*vec4(position, 0.0f, 1.0f);\n"
                                    "Color = color;\n"
                                    "TexCoords = colorTest;}\n";

    std::string instancing_fragment = "#version 330 core\n"
                                      "in vec2 TexCoords;\n"
                                      "in vec3 Color;"
                                      "out vec4 color;\n"
                                      "void main(){\n"
                                      "color = vec4(1);}";

    std::string blur_fragment =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoords;\n"
        "\n"
        "uniform sampler2D image;\n"
        "uniform int horizontal;\n"
        "\n"
        "uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );\n"
        "uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703); \n"
        "void main()\n"
        "{             \n"
        "     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel\n"
        "     vec3 result = texture(image, TexCoords).rgb * weight[0];\n"
        "     if(horizontal == 1)\n"
        "     {\n"
        "         for(int i = 1; i < 3; ++i)\n"
        "         {\n"
        "            result += texture(image, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];\n"
        "            result += texture(image, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];\n"
        "         }\n"
        "     }\n"
        "     else\n"
        "     {\n"
        "         for(int i = 1; i < 3; ++i)\n"
        "         {\n"
        "             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];\n"
        "             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];\n"
        "         }\n"
        "     }\n"
        "     FragColor = vec4(result, 1.0);\n"
        "}";

    std::string simple_vertex = "#version 330 core\n"
                                "layout(location = 0) in vec2 position;\n"
                                "layout(location = 1) in vec2 texCoords;\n"
                                "out vec2 TexCoords;\n"
                                "void main(){\n"
                                "gl_Position = vec4(position, 0.0f, 1.0f);\n"
                                "TexCoords = texCoords;}";

    std::string light_fragment = "#version 330 core\n"
                                 "layout (location = 0) out vec4 FragColor;\n"
                                 "layout (location = 1) out vec4 BrightColor;\n"

                                 "in vec2 TexCoords;\n"
                                 "uniform sampler2D screenTexture;\n"
                                 "uniform sampler2D posTexture;\n"

                                 "uniform vec2 screenSize;\n"
                                 "uniform vec2 viewPos;\n"
                                 
                                 "struct PointLight {\n"
                                 "vec3 position;\n"
                                 "vec4 color;\n"
                                 "int ready;\n"
                                 "float radius;\n};\n"
                                 
                                 "struct DirectionalLight {\n"
                                 "vec4 color;\n};\n"

                                 "const int MAX_LIGHTS = 32;\n"

                                 "uniform PointLight light[MAX_LIGHTS];\n"
                                 "uniform DirectionalLight dlight;\n"
                                 "void main(){\n"

                                 "vec4 hdrColor = texture(screenTexture, TexCoords);\n"
                                 "vec4 pos = texture(posTexture, TexCoords);\n"
                                 "vec3 result = dlight.color.rgb*hdrColor.rgb;\n"
                                 "for(int i = 0; i < MAX_LIGHTS; i++){\n"
                                 "vec3 dir = vec3(1);\n"
                                 "if(light[i].ready == 1) {\n"
                                 "dir = normalize(light[i].position - pos.rgb);\n"
                                 "}\n"
                                 "float ambientStrength = 0.2f;\n"
                                 "vec3 ambient = ambientStrength * light[i].color.rgb;\n"

                                 "vec3 norm = normalize(vec3(0,0,1));\n"

                                 "float diff = max(dot(norm, dir), 0.0);\n"
                                 "vec3 diffuse = diff * light[i].color.rgb;\n"
                                 "BrightColor = vec4(0,0,0,1);"
                                 "float distance    = length(light[i].position - pos.rgb);\n"
                                 "float attenuation = 1.0f / (0.9 * distance + 0.0032 * (distance * distance));\n"
                                 "result += (ambient*attenuation*255 + diffuse*attenuation*255)*hdrColor.rgb;}\n"
                                 "FragColor = vec4(result, 1);\n"
                                 "float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));\n"
                                 "if(brightness >= 0.5)\n"
                                 "BrightColor = vec4(FragColor.rgb, 1.0);\n"
                                 "}";

    std::string simple_fragment = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "in vec2 TexCoords;\n"
                                  "uniform sampler2D screenTexture;\n"
                                  "uniform sampler2D bloomBlur;\n"

                                  "uniform vec2 screenSize;\n"
                                  "uniform vec2 viewPos;\n"
                                  "void main(){\n"
                                  "const float gamma = 2.2;\n"
                                  "const float exposure = 1;"
                                  "vec4 hdrColor = texture(screenTexture, TexCoords);\n"
                                  "vec4 bloomColor = texture(bloomBlur, TexCoords);\n"
                                  //"hdrColor.rgb += bloomColor.rgb;\n"
                                  "vec4 result = hdrColor - bloomColor;\n"
                                  "FragColor = vec4(hdrColor.rgb, 1);\n"
                                  "}";

    std::string default_vertex = "#version 330 core\n"
                                 "layout(location = 0) in vec2 position;\n"
                                 "layout(location = 1) in vec2 texCoords;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 projection;\n"
                                 "uniform mat4 FM_PV;\n"

                                 "out vec3 ourPosition;\n"
                                 "void main(){\n"
                                 "vec4 worldPos = model*vec4(position, 1.0f, 1.0f);\n"
                                 "gl_Position = FM_PV*worldPos;\n"
                                 "ourPosition = worldPos.xyz;\n"
                                 "}";

    std::string default_fragment = "#version 330 core\n"
                                   "layout (location = 0) out vec4 FragColor;\n"
                                   "layout (location = 1) out vec4 BrightColor;\n"
                                   "layout (location = 2) out vec4 posTexture;\n"

                                   "uniform vec4 mainColor;\n"
                                   "uniform int BloomEffect;\n"
                                   "in vec3 ourPosition;\n"

                                   "void main(){\n"
                                   "vec4 color = mainColor;\n"
                                   "posTexture = vec4(ourPosition, 1);\n"
                                   "BrightColor = vec4(0,0,0,1);FragColor = vec4(0);\n"
                                   //"Color = vec4(1)\n;"
                                   "float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));\n"
                                   //"if(BloomEffect == 1) {\n"
                                   //"BrightColor = vec4(FragColor.rgb, 1.0);}\n"
                                   "if(BloomEffect <= 0) FragColor = color;\n"
                                   "}";

    std::string default_vertex_sprite = "#version 330 core\n"
                                        "layout(location = 0) in vec2 position;\n"
                                        "layout(location = 1) in vec2 texCoords;\n"

                                        "uniform mat4 view;\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 projection;\n"
                                        "uniform float bloomEffect;\n"
                                        "uniform mat4 FM_PV;\n"
                                        "out vec4 ourColor;\n"
                                        "out vec2 ourTexCoord;\n"
                                        "out vec3 ourPosition;\n"

                                        "void main(){\n"
                                        "vec4 worldPos = model*vec4(position, 1.0f, 1.0f);\n"
                                        "gl_Position = FM_PV*worldPos;\n"
                                        "ourPosition = worldPos.xyz;\n"
                                        "ourTexCoord = texCoords;"
                                        "}";

    std::string default_fragment_sprite = "#version 330 core\n"
                                          "layout (location = 0) out vec4 FragColor;\n"
                                          "layout (location = 1) out vec4 BrightColor;\n"
                                          "layout (location = 2) out vec4 posTexture;\n"
                                          "uniform vec4 mainColor;"
                                          "in vec2 ourTexCoord;\n"
                                          "in vec3 ourPosition;\n"
                                          "uniform float BloomEffect;"
                                          "uniform sampler2D texture2d;\n"
                                          //"out vec4 Color;\n"
                                          "void main(){\n"
                                          "vec4 color = texture(texture2d, ourTexCoord)*mainColor;\n"
                                          "BrightColor = vec4(0,0,0,1);"
                                          "posTexture = vec4(ourPosition, 0);\n"
                                          "if(color.w == 0.0f) discard;\n"
                                          //"Color = color;"
                                          "BrightColor = vec4(0,0,0,1);FragColor = vec4(0);\n"
                                          "if(BloomEffect <= 0) {FragColor = color; }\n"
                                          "else {BrightColor = color;}\n"
                                          "}";

    std::string default_vertex_particle =
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "uniform float scale;\n"
        "uniform vec2 offset;\n"
        "uniform vec4 particleColor;\n"
        "out vec4 ourColor;\n"
        "out vec2 ourTexCoord;\n"
        "void main(){\n"
        "gl_Position = projection*view*vec4((vertex.xy*scale) + offset, 0.0f, 1.0f);\n"
        "ourTexCoord = vertex.zw;"
        "ourColor = particleColor;}";

    std::string default_fragment_particle = "#version 330 core\n"
                                            "in vec4 ourColor;\n"
                                            "in vec2 ourTexCoord;\n"
                                            "uniform sampler2D texture2d;\n"
                                            "layout (location = 0) out vec4 FragColor;\n"
                                            "layout (location = 1) out vec4 BrightColor;\n"
                                            "void main(){\n"
                                            "FragColor = texture(texture2d, ourTexCoord)*ourColor;\n"
                                            "}";

    ResourcesManager::get().loadShader("light", simple_vertex, light_fragment);
    ResourcesManager::get().loadShader("blur", simple_vertex, blur_fragment);
    ResourcesManager::get().loadShader("text", text_vertex, text_fragment);
    ResourcesManager::get().loadShader("instancing", instancing_vertex, instancing_fragment);
    ResourcesManager::get().loadShader("default", default_vertex, default_fragment);
    ResourcesManager::get().loadShader("simple", simple_vertex, simple_fragment);
    ResourcesManager::get().loadShader("sprite", default_vertex_sprite, default_fragment_sprite);
    ResourcesManager::get().loadShader("particle", default_vertex_particle, default_fragment_particle);

    std::shared_ptr<Shader> s = ResourcesManager::get().getShader("simple");
    s->Use()->setInt("screenTexture", 0)->setInt("bloomBlur", 1);

    std::shared_ptr<Shader> light = ResourcesManager::get().getShader("light");
    light->Use()->setInt("screenTexture", 0)->setInt("posTexture", 1);

    // Initialize all the shapes
    for(int i = 0; i < fmc::SHAPE::LAST_SHAPE; ++i) {
        fmc::CMesh rect;
        rect.setShape(i);
    }

#ifdef __linux__
    ResourcesManager::get().load("dejavu",
                                 std::make_unique<RFont>("assets/fonts/dejavu/DejaVuSansMono.ttf"));
#endif
}

void Window::bindFrameBuffer() {
    Renderer::getInstance().bindFrameBuffer();
}

void Window::update(float fps) {

    events();
    this->fpsMax = fps;
    wait_time = 1.0f / (float)fpsMax;
    frameLimit(fps);

    bindFrameBuffer();

    clear();
}

void Window::initFrameBuffer() {
    Renderer::getInstance().initFrameBuffer(width, height);
}

void Window::createQuadScreen() {
    Renderer::getInstance().createQuadScreen();
}

void Window::frameLimit(unsigned short fps) {

    curr_frame_time = glfwGetTime() - frame_start;
    double dur = 1000.0 * (wait_time - curr_frame_time) + 0.5;
    if(dur > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
    }

    double frame_end = glfwGetTime();
    Time::dt = frame_end - frame_start;
    // here you could print to file or OutputDebugStream curr_frame_time, frame_end - frame_start and dur.
    frame_start = frame_end;
}

void Window::blur() {
    Renderer::getInstance().blur();
}

void Window::swapBuffers() {

    // clear();
    errorDisplay();
    glfwSwapBuffers(window);
}

void Window::errorDisplay() {
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << std::endl;
        exit(-1);
    }
}

void Window::postProcess(bool horizontal) {
    std::shared_ptr<Shader> s = ResourcesManager::get().getShader("simple");
    s->Use()->setVector2f("screenSize", glm::vec2(width, height));
    Renderer::getInstance().postProcess(horizontal);
}

bool Window::isClosed() {
    return glfwWindowShouldClose(window);
}

void Window::events() {
    glfwPollEvents();
}

Window::~Window() {
    glfwTerminate();
}

int Window::init(GLFWwindow* window) {
    if(window == nullptr) {

        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        return -1;
    }
    glGetError();
    errorDisplay();

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace( GL_FRONT_AND_BACK);
    return 1;
}

void Window::clear() {
    Renderer::getInstance().clear();
}
