#version 330

uniform sampler2D textureMap0;
uniform sampler2D specular0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 v;
in vec4 l1;
in vec4 l2;
in float d1;
in float d2;
in vec2 iTexCoord0;

void main(void) {
	float d;
	//Znormalizowane interpolowane wektory
	vec4 ml1 = normalize(l1);
	vec4 ml2 = normalize(l2);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr1 = reflect(-ml1, mn);
	vec4 mr2 = reflect(-ml2, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = texture(specular0, iTexCoord0);

	//Obliczenie modelu oświetlenia
	float nl1 = clamp(dot(mn, ml1), 0, 1);
	float nl2 = clamp(dot(mn, ml2), 0, 1);
	float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 50);
	float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 50);

	vec4 color1 = vec4(kd.rgb * nl1, kd.a) + vec4(ks.rgb*rv1, 0);
	vec4 color2 = vec4(kd.rgb * nl2, kd.a) + vec4(ks.rgb*rv2, 0);

	pixelColor= vec4(color1.rgb*d1, color1.a) + vec4(color2.rgb*d2, color2.a);
}
