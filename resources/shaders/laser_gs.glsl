#version 450

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 viewport;
uniform vec3 _cameraPosition;

layout(lines) in;
layout(triangle_strip, max_vertices = 8) out;

layout(std140, binding = 4) buffer debug_buff {
    vec4 dbg[];
};

out vec3 g_uv;
out vec4 g_color;
out float g_depth;

vec3 project(vec3 point, mat4 viewMatrix, mat4 projectionMatrix, vec4 viewport) {
    vec4 tmp = vec4(point,1.0);
    tmp = viewMatrix * tmp;
    tmp = projectionMatrix * tmp;

    tmp /= tmp.w;
    tmp = tmp * vec4(0.5) + vec4(0.5);

    tmp[0] = tmp[0] * viewport[2] + viewport[0];
    tmp[1] = tmp[1] * viewport[3] + viewport[1];

    return vec3(tmp);
}

void CreateVertex(vec2 position, float depth, vec4 color, vec3 uvs, int i) {
    g_uv = uvs;
    g_color = color;
    g_depth = depth;
    dbg[i] = vec4(position,0,1);
	gl_Position = u_Projection * u_View  * vec4(position, 0.0, 1.0);
    EmitVertex();
}

void main() {
    const float DIST_SCALING_FACTOR = 1500.0f;
    const float EPS = 0.01f;
    const float HEAD_ON_FALLOFF = 1.0f;

    vec3 begin = gl_in[0].gl_Position.xyz;
    vec3 end   = gl_in[1].gl_Position.xyz;

    vec3 orthoP1 = project(begin, viewMatrix, projectionMatrix, viewport);
    vec3 orthoP2 = project(end  , viewMatrix, projectionMatrix, viewport);

    //orthoP1.x += EPS * ((abs(orthoP1.x - orthoP2.x) < EPS) || (abs(orthoP1.y - orthoP2.y) < EPS) );

    if (orthoP1.z < 1.0 || orthoP2.z < 1.0) {
        float begin_df = DIST_SCALING_FACTOR / length(begin - _cameraPosition);
        float end_df   = DIST_SCALING_FACTOR / length(end   - _cameraPosition);

        orthoP1 *= -sign(orthoP1.z - 1.0f);
        orthoP2 *= -sign(orthoP2.z - 1.0f);

        vec3 cameraLaserDirection = normalize(_cameraPosition - ((begin + end) / 2.0f));
        vec3 laserDirection = normalize(end - begin);
        float angle = dot(laserDirection, cameraLaserDirection);
        float headOnFactor = pow(abs(angle), HEAD_ON_FALLOFF);


        if (begin_df > end_df) {
            end_df = end_df + (begin_df - end_df) * headOnFactor;
        }
        else {
            begin_df = begin_df + (end_df - begin_df) * headOnFactor;
        }

        const float WIDTH = 2.0f;// TODO uniform WIDTH
        float beginWidth = begin_df * WIDTH; 
        float endWidth = end_df * WIDTH;

        vec2 screenDir = normalize(vec2(orthoP1 - orthoP2)); 

        vec2 beginY = screenDir * beginWidth;
        vec2 endY   = screenDir * endWidth;
        vec2 beginX = vec2(beginY.y, -beginY.x);
        vec2 endX   = vec2(endY.y  , -endY.x);

        vec4 color = vec4(0,0,1,1); // TODO COLOR UNIFORM

        CreateVertex(vec2(orthoP1) - beginX + beginY, orthoP1.z, color, vec3(0.0f      , beginWidth     , beginWidth), 0); // 0
        CreateVertex(vec2(orthoP1) + beginX + beginY, orthoP1.z, color, vec3(beginWidth, beginWidth     , beginWidth), 1); // 1
        CreateVertex(vec2(orthoP1) - beginX         , orthoP1.z, color, vec3(0.0f      , beginWidth*0.5f, beginWidth), 2); // 2
        CreateVertex(vec2(orthoP1) + beginX         , orthoP1.z, color, vec3(beginWidth, beginWidth*0.5f, beginWidth), 3); // 3

        CreateVertex(vec2(orthoP2) - endX           , orthoP2.z, color, vec3(0.0f      , endWidth * 0.5f, endWidth  ), 4);
        CreateVertex(vec2(orthoP2) + endX           , orthoP2.z, color, vec3(endWidth  , endWidth * 0.5f, endWidth  ), 5);
        CreateVertex(vec2(orthoP2) - endX   - endY  , orthoP2.z, color, vec3(0.0f      , 0.0f           , endWidth  ), 6);
        CreateVertex(vec2(orthoP2) + endX   - endY  , orthoP2.z, color, vec3(endWidth  , 0.0f           , endWidth  ), 7);

    }
}
