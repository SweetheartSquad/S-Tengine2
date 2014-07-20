attribute vec3 aPositionVertex;

void main()
{
    gl_Position = aPositionVertex;//gl_ModelViewProjectionMatrix * vec4(aPositionVertex, 1.0);
}
