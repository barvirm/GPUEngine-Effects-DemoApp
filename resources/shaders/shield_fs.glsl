#version 450

struct PulseWave {
    vec3 origin;
    float t;
};

struct IP {
    vec3 point;
    float t;
};


layout(std430, binding = 0) readonly buffer PulseWaveInfo { PulseWave pulsewave[]; };
layout(std430, binding = 1) readonly buffer PrefixSumIntersections { int PSI[]; };
layout(std430, binding = 2) readonly buffer IntersectionPoints { IP intersectPoints[]; };
layout(std430, binding = 3) readonly buffer PrefixSumPulseWaves { int PSPW[]; };
layout(std430, binding = 4) readonly buffer ModelMatrixies { mat4 modelMatrix[]; };
//136 str. openGL core

uniform float time;

in vec3 vPosition;
flat in int glInstanceID;

out vec4 fragColor;

#define PI 3.1415695
#define TWO_PI 6.28318530718

// posun inverzni matici
float sphereDistance(vec3 a, vec3 b, float radius, mat4 inverseMatrix) {
    a = vec3(inverseMatrix * vec4(a,1.0));
    b = vec3(inverseMatrix * vec4(b,1.0));
    return radius * atan( length(cross(a,b)), dot(a,b) );
}

float cubicPulse( float c, float w, float x) {
    x = abs(x - c);
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}

vec2 LissajousCurve() {
    float A = 1.0;
    float a = 7.0;
    float f = PI/2.0;
    float c = 1.0;
    float B = 1.5;
    float b = 5.0;
    float D = 0.5;
    float tm = mod(time*0.03 + glInstanceID,TWO_PI);
    return vec2(A * sin(a*tm+f)+c, B * sin(b*tm)+ D);
}

vec3 ring(vec3 position, vec3 beginPosition, vec3 colorB, vec3 colorE, float gradientWidth, float sphereRadius, float t) {
    float d = sphereDistance(position, beginPosition, sphereRadius, inverse(modelMatrix[glInstanceID]));
    t *= PI*sphereRadius*1.25;
    if ( t > d  && t < d+gradientWidth ) {
        return mix(colorE, colorB ,1.0*cubicPulse(d+gradientWidth*0.5, gradientWidth*0.7,t));
    }
    return vec3(0);
}

vec3 hitting(vec3 position, vec3 hit, vec3 begin, vec3 end, float width, float sphereRadius) {
    float d = sphereDistance(position, hit, sphereRadius, inverse(modelMatrix[glInstanceID]));
    if ( d < width ) {
        return mix(begin, end, d/width);
    }
    return vec3(0);
}

float field(in vec3 position) {

    const float weight[20] = {
        1.0,
        0.8337529180751806,
        0.6951439283988787,
        0.5795782787848095,
        0.48322508118982543,
        0.402890321529133,
        0.3359109812391624,
        0.2800667608216489,
        0.2335064790909134,
        0.19468670833151017,
        0.1623206111818482,
        0.1353352832366127,
        0.11283618731705693,
        0.09407750044007386,
        0.07843739051713068,
        0.06539740322986021,
        0.05452527577743518,
        0.045460607788290544,
        0.03790291440095851,
        0.031601665485352946,
    };
    const float strength = 15.0;
    const float radius = modelMatrix[glInstanceID][0][0];

    float sum = 0.0;
    float weightSum = 1.4;
    float previous = 0.0;
    float intesity = 1.8;


    vec3 modelPosition = vec3(modelMatrix[glInstanceID] * vec4(vPosition,1.0));

    for (int i = PSI[glInstanceID]; i < PSI[glInstanceID+1]; i++) {
        intesity +=  5*hitting(modelPosition, intersectPoints[i].point, vec3(1), vec3(0), 0.5*radius*cubicPulse(0.5,0.5,intersectPoints[i].t), radius).z;
    }

    for (int i = PSPW[glInstanceID]; i < PSPW[glInstanceID+1]; i++) {
        float pulsefade = (2.2 - pulsewave[i].t*2);
        intesity +=  3 * pulsefade * ring(modelPosition, pulsewave[i].origin, vec3(1), vec3(0), 3.0, radius, pulsewave[i].t).x;
    }
    position += 0.5 * vec3(LissajousCurve(),0);

    for (int i = 0; i < 20; ++i) {
        float mag = dot(position, position);
        position = abs(position) / mag + vec3(-0.5, -0.4, -1.4);
        sum += weight[i] * exp(-strength * pow(abs(mag - previous), intesity ));
        weightSum += weight[i];
        previous = mag;
    }

    return max(0.0, 4.0 * sum / weightSum - 0.7);
}

void main() {
    const vec3 white = vec3(1);
    const vec3 black = vec3(0);
    const float radius = modelMatrix[glInstanceID][0][0];

    float t = field(vPosition);
    vec3 color =  vec3(0.2*t*t*t, 0.9*t*t, 1.9*t*t);
    fragColor = vec4(color,1.0);
}
