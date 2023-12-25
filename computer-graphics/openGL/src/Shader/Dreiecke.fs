#version 450 core  // 420, 330 core , compatibility

in vec2 textureC;
in vec3 f_normals;
in vec3 f_position;

out vec4 FragColor;
uniform sampler2D textureImg;


void main() {

	vec3 view	= normalize(- f_position);	
	vec3 light	= normalize(vec3(1.0f, 1.0f, 1.0f));
	vec3 normal = normalize(f_normals);
	vec3 color	= vec3(texture(textureImg, textureC));
	vec3 reflec = reflect(-light, normal);

	vec3 ambient = color * 0.7;
	vec3 diffuse = max(dot(normal, light), 0.0) * color;
	vec3 specul  = pow(max(dot(reflec, view), 0.0), 10.0) * color * 0;

	FragColor	= vec4(ambient + diffuse + specul, 1.0);

	//FragColor = texture(textureImg, textureC);
}

