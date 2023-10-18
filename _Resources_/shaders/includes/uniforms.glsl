
#ifdef VULKAN
#define INSTANCE_INDEX gl_InstanceIndex
#define UNIFORM(inSet, inBinding) layout(set = inSet, binding = inBinding) uniform
#define READ_ONLY_BUFFER(inSet, inBinding) layout(std140,set = inSet, binding = inBinding) readonly buffer
#define UNIFORM_SAMPLER(inSet, inBinding) layout(set = inSet, binding = inBinding) uniform sampler2D
#else
#define INSTANCE_INDEX gl_BaseInstance + gl_InstanceID
#define UNIFORM(inSet, inBinding) layout(std140, binding = inBinding) uniform
#define READ_ONLY_BUFFER(inSet, inBinding) layout(std430, binding = inBinding) readonly buffer
#define UNIFORM_SAMPLER(inSet, inBinding) uniform sampler2D
#endif