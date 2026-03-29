#version 330

in vec3 frag_color;
in vec3 fragWorldPos;
out vec4 out_color;

uniform bool tunnel;

void main()
{
	 if (tunnel) {
        bool insideX = fragWorldPos.x >= 17.0 && fragWorldPos.x <= 23.0;
        bool insideZ = fragWorldPos.z <= -4.0 && fragWorldPos.z >= -25.0;
        bool insideY = fragWorldPos.y >= 0.0 && fragWorldPos.y <= 8.0;

        bool insideXgate = fragWorldPos.x < -8.0 && fragWorldPos.x > -26.0;
        bool insideZgate = fragWorldPos.z > 5.0 && fragWorldPos.z < 10.0;
        bool insideYgate = fragWorldPos.y > 0 && fragWorldPos.y < 1.2;

        if ((insideX && insideY && insideZ) || (insideXgate && insideYgate && insideZgate))
            discard;
    }
	out_color = vec4(frag_color, 1.0f);
}
