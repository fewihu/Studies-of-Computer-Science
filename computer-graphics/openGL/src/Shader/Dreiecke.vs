#version 450 core  // 420, 330 core , compatibility
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 textureIn;
layout(location = 2) in vec3 normal;

out vec2 textureC;
out vec3 f_normals;
out vec3 f_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelView;
uniform mat4 invModelView;

void main(){ 
 gl_Position = projection * view * model * vec4(vPosition,1.0f);
 textureC	 = vec2(textureIn.x, textureIn.y);
 
 f_normals	 = mat3(invModelView) * normal;
 f_position  = vec3(modelView * vec4(vPosition, 1.0f));
}
