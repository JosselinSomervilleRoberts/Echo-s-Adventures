#version 330 core

in struct fragment_data
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
	vec3 eye;
} fragment;

layout(location=0) out vec4 FragColor;

uniform sampler2D image_texture;

uniform vec3 light = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f); // Unifor color of the object
uniform float alpha = 1.0f; // alpha coefficient
uniform float Ka = 0.4f; // Ambient coefficient
uniform float Kd = 0.2f; // Diffuse coefficient
uniform float Ks = 0.4f;// Specular coefficient
uniform float specular_exp = 64.0f; // Specular exponent
uniform bool use_texture = true;
uniform bool texture_inverse_y = false;


// Lights
uniform vec3 spotlight_color[100];
uniform vec3 spotlight_position[100];
uniform float spotlight_falloff[100];
uniform float spotlight_intensity[100];
uniform float fog_falloff;
uniform int N_lights = 5;

// Differentiation de l'affichage autour du joueur et a l exterieur
uniform vec3 posCentre = vec3(0,0,5);
uniform float R = 10.0f;
uniform float dist_zone = 6.0f;
uniform bool interieur = true;
uniform bool diffentiation = true;

// Affichage
uniform float red = 1.0f;
uniform float green = 1.0f;
uniform float blue = 1.0f;
uniform bool filter = false;
uniform float render_distance = 15.0f;




void main()
{
	float luminosite = 0.6f;
	vec3 dif_vec = fragment.position - posCentre;
    float distance = dif_vec[0]*dif_vec[0] + dif_vec[1]*dif_vec[1];// + dif_vec[2]*dif_vec[2];
		
		
	// ================= On gere ici la séparation de la boule interieure et exterieure ============== //
	bool continuer = true;
	if(diffentiation==true) {	
		if((interieur==true) && (distance > dist_zone*dist_zone)) {
			discard;
			continuer = false;
			//luminosite = 0.2f;
		}
		else if((interieur==false) && ((distance <= dist_zone*dist_zone) || (distance > render_distance*render_distance))) {
			discard;//FragColor = vec4(0,0,0,1);
			continuer = false;
		}
	}
	
	
	// ============================= Affichage en temps normal ================================== //
	if(continuer==true) {
		// Obtention de la normale
		vec3 N = normalize(fragment.normal);
		if (gl_FrontFacing == false) {
			N = -N;
		}
		
		// Gestion de la texture
		vec2 uv_image = vec2(fragment.uv.x, 1.0-fragment.uv.y);
		if(texture_inverse_y) {
			uv_image.y = 1.0-uv_image.y;
		}
		vec4 color_image_texture = texture(image_texture, uv_image);
		
		if(interieur==false) {
		float attenuation = 0.3f;
			color_image_texture = vec4(attenuation * color_image_texture[0] + 0.5f*(1-attenuation), attenuation * color_image_texture[1] + 0.5f*(1-attenuation), attenuation * color_image_texture[2] + 0.5f*(1-attenuation), color_image_texture[3]);
		}
		
		if((use_texture==false) || (filter==true)) {// || (interieur==false)) {
			color_image_texture=vec4(1.0,0.0,0.0,1.0);
		}
		
		
		
		// Gestion de la transparence (pour les feuilles d'arbres par exemple)
		if( color_image_texture.a < 0.5) {
			discard;
			continuer = false;
		}
		
		if(continuer==true) {
			vec3 color_object  = fragment.color * color * color_image_texture.rgb;
			vec3 color_shading = Ka * color_object;

			for(int k_light=0; k_light<N_lights; k_light++)
			{
				vec3 v = spotlight_position[k_light]-fragment.position;
				float dist = length(v);
				vec3 L = normalize(v);
				float diffuse = max(dot(N,L),0.0);
				float specular = 0.0;
				if(diffuse>0.0){
					vec3 R = reflect(-L,N);
					vec3 V = normalize(fragment.eye-fragment.position);
					specular = pow( max(dot(R,V),0.0), specular_exp );
				}
				
				// spotlight color
				color_shading += (Kd*diffuse*color_object + Ks * specular) * spotlight_color[k_light]*exp(-spotlight_falloff[k_light]*dist*dist) * spotlight_intensity[k_light];
			}

			// fog effect
			float depth1 = length(fragment.eye-fragment.position) / sqrt(R);
			float depth2 = sqrt(distance) / dist_zone;
			float w_depth1 = 0.5f*exp(-fog_falloff*depth1*depth1);
			float w_depth2 = 0.5f*exp(-fog_falloff*depth2*depth2);
			vec3 color_with_fog = (w_depth1+w_depth2)*color_shading +  (0.5f-w_depth1)*vec3(0.3f,0.3f,0.3f) + (0.5f-w_depth2)*vec3(0.0f,0.0f,0.0f);
			
			if(color_with_fog[0] > 0.98) {
				color_with_fog[0] = 0.98;
			}

			FragColor = vec4(color_with_fog[0] * red * luminosite, color_with_fog[1] * green * luminosite, color_with_fog[2] * blue * luminosite, alpha * color_image_texture.a);
		}
	}
}
