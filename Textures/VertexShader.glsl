#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0); // set ourColor to the input color we got from the vertex data
    ourColor = aColor;
    TexCoord = aTexCoord;
}

