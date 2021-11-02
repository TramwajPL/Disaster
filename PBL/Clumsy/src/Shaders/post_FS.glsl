 #version 330 core
    in  vec2  TexCoords;
    out vec4  color;

    uniform sampler2D scene;

    uniform bool grey;
    uniform bool shake;

    void main()
    {
        if(grey)
        {
            color = texture(scene, TexCoords);
            float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
            color = vec4(average, average, average, 1.0);
        }
        else
        {
            color =  texture(scene, TexCoords);
        }

        // gamma correction
        float gamma = 2.2;
        color.rgb = pow(color.rgb, vec3(1.0/gamma));
        color = vec4(color.rgb, 1.0);        

    }