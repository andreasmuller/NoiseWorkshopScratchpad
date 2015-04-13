#version 120
#extension GL_EXT_gpu_shader4 : require

uniform sampler2D u_particleDataTexture;
uniform vec2 u_resolution;

// ----------------------------
void main ()
{
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	//vec2 texCoord = gl_TexCoord[0].st;
	
	// Figure out the texture coordinate our data is on from the instance ID
	vec2 texCoord;
	texCoord.x = mod(gl_InstanceID, u_resolution.x) / u_resolution.x;
	texCoord.y = floor(gl_InstanceID / u_resolution.x) / u_resolution.y;
	
	vec4 particleData = texture2D( u_particleDataTexture, texCoord );
	vec3 pos = particleData.xyz;
	
	vec3 newVertexPos = pos + gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(newVertexPos, 1.0);
	
	//gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
	gl_FrontColor = gl_Color;
}
