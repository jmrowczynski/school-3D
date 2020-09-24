#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 n;
out vec4 v;
out vec4 l1;
out vec4 l2;
out float d1;
out float d2;

out vec2 iTexCoord0;

void main(void) {    
    vec4 lp1 = vec4(-20.0f, 15.0f, -30.0f, 1); //pozcyja światła, przestrzeń świata
    vec4 lp2 = vec4(20.0f, 15.0f, 30.0f, 1); //pozcyja światła, przestrzeń świata
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    l1 = normalize(V * lp1 - V*M*vertex); //wektor do światła w przestrzeni oka
    l2 = normalize(V * lp2 - V*M*vertex); //wektor do światła w przestrzeni oka
    
    float tempd1;
    tempd1 = distance(V*M*vertex, lp1);
    d1 = 1 - (tempd1) / 160;

    float tempd2;
    tempd2 = distance(V*M*vertex, lp2);
    d2 = 1 - (tempd2) / 160;

    iTexCoord0 = texCoord0;

    gl_Position=P*V*M*vertex;
}
