 #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

    out vec2 TexCoords;
    
    uniform bool shake;
    uniform float time;

    void main()
    {
        gl_Position = vec4(vertex.xy, 0.0f, 1.0f); 
        vec2 texture = vertex.zw;
        TexCoords = texture;

        if (shake)
        {
            float strength = 0.02;
            gl_Position.x += cos(time * 10) * strength;        
            gl_Position.y += cos(time * 15) * strength;        
        }
    }  