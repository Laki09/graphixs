//
//  lightFragShader.fs
//  graphixs
//
//  Created by Lars Höll on 04.09.25.
//
#version 330 core
out vec4 FragColor;

uniform vec3 color;
void main()
{
    FragColor = vec4(color, 1.0); // set all 4 vector values to 1.0
}
