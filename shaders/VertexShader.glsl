#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform vec3 object_color;
uniform bool water;

out vec3 frag_color;
out vec3 fragWorldPos;

void main()
{
    vec3 pos = v_position;
    
    if (water) {
        if (pos.z < -10) {
            pos.y += 2 * sin(pos.x + time) * cos(pos.z + time);
        } else {
            pos.y += 0.7 * sin(pos.x + time) * cos(pos.z + time);
        }
    }

    frag_color = object_color;
    fragWorldPos = vec3(Model * vec4(pos, 1.0));
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
