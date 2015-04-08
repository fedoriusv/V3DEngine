#version 330

//inverted color
vec4 invColor(vec4 color)
{
    return vec4(1.0, 1.0, 1.0, 1.0) - color;
}

//colors grayscale
vec4 grayScale1(vec4 color)
{
    float avg = (color.r + color.g + color.b) / 3.0;
    return vec4(avg, avg, avg, 1.0);
}

//colors grayscale
vec4 grayScale2(vec4 color)
{
    float avg = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec4(avg, avg, avg, 1.0);
}

//blur color
 vec4 blur(sampler2D sampler, vec2 coord, float blurSizeH = 1.0 / 300.0, float blurSizeV = 1.0 / 200.0)
{
    vec4 sum = vec4(0.0);
    for (int x = -4; x <= 4; x++)
        for (int y = -4; y <= 4; y++)
        {
            sum += texture(sampler, vec2(coord.x + x * blurSizeH, coord.y + y * blurSizeV)) / 81.0;
        }
    return sum;
}

//sobel color
vec4 sobel(sampler2D sampler, vec2 coord)
{
    vec4 s1 = texture(sampler, coord - 1.0 / 300.0 - 1.0 / 200.0);
    vec4 s2 = texture(sampler, coord + 1.0 / 300.0 - 1.0 / 200.0);
    vec4 s3 = texture(sampler, coord - 1.0 / 300.0 + 1.0 / 200.0);
    vec4 s4 = texture(sampler, coord + 1.0 / 300.0 + 1.0 / 200.0);
    vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
    vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
    
    return sqrt(sx * sx + sy * sy);
}

