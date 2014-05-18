varying vec3 normal, lightDir, eyeDir;

uniform sampler2D colorTexture0;
uniform sampler2D colorTexture1;
uniform sampler2D colorTexture2;

void main (void)
{
	// normalize normal vector range from [0, 1] -> [-1, 1]
	vec3 normal = texture2D(colorTexture0, gl_TexCoord[0].xy).rgb * float(2) - float(1);
	vec4 final_color = (gl_LightSource[0].ambient * gl_FrontMaterial.ambient)
	;
							
	vec3 N = normalize(gl_NormalMatrix * normal);
	vec3 L = normalize(lightDir);

	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
		               texture2D(colorTexture1, gl_TexCoord[1].xy).rgba
		               * lambertTerm;	
		
		vec3 E = normalize(eyeDir);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );

		final_color += gl_LightSource[0].specular * 
		               texture2D(colorTexture2, gl_TexCoord[2].xy).rgba 
		               * specular;	
	}

	gl_FragColor = final_color;	
}