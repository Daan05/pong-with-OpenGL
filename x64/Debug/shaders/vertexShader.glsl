#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float p1_offset;
uniform float p2_offset;
uniform float p1_points;
uniform float p2_points;
uniform float ball_x;
uniform float ball_y;

void main()
{
	ourColor = aColor;

	if (aColor.r == 1.0 && aColor.g == 0.0 && aColor.b == 0.0)	// p1 handle - red
	{
		gl_Position = vec4(aPos.x, aPos.y + p1_offset, aPos.z, 1.0);
	}
	else if (aColor.r == 0.0 && aColor.g == 0.0 && aColor.b == 1.0)	// p2 handle - blue
	{
		gl_Position = vec4(aPos.x, aPos.y + p2_offset, aPos.z, 1.0);
	}
	else if (aColor.r == 1.0 && aColor.g == 1.0 && aColor.b == 1.0)	// ball - white
	{
		gl_Position = vec4(aPos.x + ball_x, aPos.y + ball_y, aPos.z, 1.0);
	}
	else if (aColor.r == 1 && aColor.b == 0.4) // points for p1
	{
		gl_Position = vec4(aPos.x + p1_points * 0.06, aPos.y, aPos.z, 1.0);
	}
	else if (aColor.r == 0.4 && aColor.b == 1) // points for p2
	{
		gl_Position = vec4(aPos.x + p2_points * 0.06, aPos.y, aPos.z, 1.0);
	}
	else
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
}
