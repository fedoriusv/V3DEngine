float GetColorLuminance( vec3 color )
{
    return dot( color, vec3( 0.2126, 0.7152, 0.0722 ) );
}

vec4 nsaaPixelShader(vec2 posPos, sampler2D tex, vec2 rcpFrame)
{
    const float fScale = 3;

    // Offset coordinates
    vec2 upOffset = vec2( 0, rcpFrame.y ) * fScale;
    vec2 rightOffset = vec2( rcpFrame.x, 0 ) * fScale;
    
    float topHeight = GetColorLuminance( texture( tex, posPos + upOffset).rgb );
    float bottomHeight = GetColorLuminance( texture( tex, posPos - upOffset).rgb );
    float rightHeight = GetColorLuminance( texture( tex, posPos + rightOffset).rgb );
    float leftHeight = GetColorLuminance( texture( tex, posPos - rightOffset).rgb );
    float leftTopHeight = GetColorLuminance( texture( tex, posPos - rightOffset + upOffset).rgb );
    float leftBottomHeight = GetColorLuminance( texture( tex, posPos - rightOffset - upOffset).rgb );
    float rightBottomHeight = GetColorLuminance( texture( tex, posPos + rightOffset + upOffset).rgb );
    float rightTopHeight = GetColorLuminance( texture( tex, posPos + rightOffset - upOffset).rgb );
    
    // Normal map creation
    float sum0 = rightTopHeight+ topHeight + rightBottomHeight;
    float sum1 = leftTopHeight + bottomHeight + leftBottomHeight;
    float sum2 = leftTopHeight + leftHeight + rightTopHeight;
    float sum3 = leftBottomHeight + rightHeight + rightBottomHeight;
    
    float vect1 = (sum1 - sum0);
    float vect2 = (sum2 - sum3);

    // Put them together and scale.
    vec2 Normal = vec2(vect1, vect2) * rcpFrame * fScale;
    
    // Color
    vec4 Scene0 = texture( tex, posPos );
    vec4 Scene1 = texture( tex, posPos + Normal.xy );
    vec4 Scene2 = texture( tex, posPos - Normal.xy );
    vec4 Scene3 = texture( tex, posPos + vec2(Normal.x, -Normal.y) * 0.5 );
    vec4 Scene4 = texture( tex, posPos - vec2(Normal.x, -Normal.y) * 0.5 );
    
    // Final color
    return (Scene0 + Scene1 + Scene2 + Scene3 + Scene4) * 0.2;
}
