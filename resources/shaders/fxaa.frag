#version 330 core
// FBO
in vec2 uv;

uniform sampler2D fxaaSampler;

uniform float width;
uniform float height;

out vec4 fragColor;

float luma(vec3 color){
    return sqrt(dot(color, vec3(0.299, 0.587, 0.114)));
}

vec2 UV = uv;

uniform bool enableFxaa;

void main()
{
    if (!enableFxaa) {
        fragColor = texture(fxaaSampler, UV);
        return;
    }

    vec4 centerColor = texture(fxaaSampler, UV);
    vec4 upColor = texture(fxaaSampler, vec2(UV[0], UV[1] - 1));
    vec4 downColor = texture(fxaaSampler, vec2(UV[0], UV[1] + 1));
    vec4 leftColor = texture(fxaaSampler, vec2(UV[0] - 1, UV[1]));
    vec4 rightColor = texture(fxaaSampler, vec2(UV[0] + 1, UV[1]));
    vec4 topLeftColor = texture(fxaaSampler, vec2(UV[0] - 1, UV[1] - 1));
    vec4 topRightColor = texture(fxaaSampler, vec2(UV[0] + 1, UV[1] - 1));
    vec4 bottomLeftColor = texture(fxaaSampler, vec2(UV[0] - 1, UV[1] + 1));
    vec4 bottomRightColor = texture(fxaaSampler, vec2(UV[0] + 1, UV[1] + 1));

    float centerLuma = luma(vec3(centerColor));
    float upLuma = luma(vec3(upColor));
    float downLuma = luma(vec3(downColor));
    float leftLuma = luma(vec3(leftColor));
    float rightLuma = luma(vec3(rightColor));
    float topLeftLuma = luma(vec3(topLeftColor));
    float topRightLuma = luma(vec3(topRightColor));
    float bottomLeftLuma = luma(vec3(bottomLeftColor));
    float bottomRightLuma = luma(vec3(bottomRightColor));

    float minLuma = min(centerLuma, min(upLuma, min(downLuma, min(leftLuma, rightLuma))));
    float maxLuma = max(centerLuma, max(upLuma, max(downLuma, max(leftLuma, rightLuma))));

    float range = maxLuma - minLuma;

    float threshold = 0.125;

    if (range < max(0.125, maxLuma*0.0312)){
        fragColor = centerColor;
        return;
    }

    float edgeHorizontal = abs(-2.0 * leftLuma + topLeftLuma + bottomLeftLuma) + abs(-2.0 * centerLuma + upLuma + downLuma) * 2.0 + abs(-2.0 * rightLuma + topRightLuma + bottomRightLuma);
    float edgeVertical = abs(-2.0 * upLuma + topLeftLuma + topRightLuma) + abs(-2.0 * centerLuma + leftLuma + rightLuma) * 2.0 + abs(-2.0 * downLuma + bottomLeftLuma + bottomRightLuma);

    bool isHorizontal = (edgeHorizontal >= edgeVertical);

    float luma1 = isHorizontal ? downLuma : leftLuma;
    float luma2 = isHorizontal ? upLuma : rightLuma;

    float grad1 = luma1 - centerLuma;
    float grad2 = luma2 - centerLuma;
    bool is1Steepest = abs(grad1) >= abs(grad2);
    float gradientScaled = 0.25*max(abs(grad1),abs(grad2));

    float stepX = 1.f / width;
    float stepY = 1.f / height;

    float step;

    if (isHorizontal) {
        step = stepX;
    } else {
        step = stepY;
    }

    float lumaLocalAverage = 0.0;

    if (is1Steepest){
        step = -1.f * step;
        lumaLocalAverage = 0.5*(luma1 + centerLuma);
    } else {
        lumaLocalAverage = 0.5*(luma2 + centerLuma);
    }

    if (isHorizontal){
        UV[1] += 0.5 * step;
    } else {
        UV[0] += 0.5 * step;
    }

    vec2 offset = isHorizontal ? vec2(stepX,0.0) : vec2(0.0,stepY);

    vec2 UV1 = UV - offset;
    vec2 UV2 = UV + offset;

    float lumaEnd1 = luma(texture(fxaaSampler,UV1).rgb);
    float lumaEnd2 = luma(texture(fxaaSampler,UV2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if(!reached1){
        UV1 -= offset;
    }
    if(!reached2){
        UV2 += offset;
    }

    if(!reachedBoth){

        for(int i = 2; i < 8; i++){

            if(!reached1){
                lumaEnd1 = luma(texture(fxaaSampler, UV1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }

            if(!reached2){
                lumaEnd2 = luma(texture(fxaaSampler, UV2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }

            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            if(!reached1){
                UV1 -= offset;
            }
            if(!reached2){
                UV2 += offset;
            }

            if(reachedBoth){ break;}
        }
    }

    float distance1 = isHorizontal ? (UV[0] - UV1.x) : (UV[1] - UV1.y);
    float distance2 = isHorizontal ? (UV2.x - UV[0]) : (UV2.y - UV[1]);

    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    float edgeThickness = (distance1 + distance2);

    float pixelOffset = - distanceFinal / edgeThickness + 0.5;

    bool isLumaCenterSmaller = centerLuma < lumaLocalAverage;

    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

    float finalOffset = correctVariation ? pixelOffset : 0.0;

    vec2 finalUV = UV;
    if (isHorizontal){
        finalUV.y += finalOffset * step;
    } else {
        finalUV.x += finalOffset * step;
    }

    vec4 finalColor = texture(fxaaSampler,finalUV);
    fragColor = finalColor;

}
