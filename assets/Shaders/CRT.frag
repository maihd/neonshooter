#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float uAmount; 
uniform float uTime;   
uniform float uBloomIntensity;

out vec4 finalColor;

const float curvature = 4.0;          // Lower = more curve
const float scanlineWeight = 0.15;    
const float scanlineFrequency = 3.00;

vec2 curve(vec2 uv) {
    uv = (uv - 0.5) * 2.0;
    uv.x *= 1.0 + pow((abs(uv.y) / curvature), 2.0);
    uv.y *= 1.0 + pow((abs(uv.x) / curvature), 2.0);
    uv = (uv / 2.0) + 0.5;
    return uv;
}

vec4 textureSharpBilinear(sampler2D tex, vec2 uv, vec2 offset) {
    vec2 texSize = vec2(textureSize(tex, 0));
    vec2 texels = (uv + offset) * texSize;
    
    vec2 alpha = fract(texels);
    vec2 grid = floor(texels) + 0.5;
    
    alpha = clamp((alpha - 0.5) / fwidth(texels) + 0.5, 0.0, 1.0);
    
    vec2 correctedUV = (grid + alpha) / texSize;
    return texture(tex, correctedUV);
}

vec4 getDynamicGlow(sampler2D tex, vec2 uv, float edgeDist) {
    vec4 glow = vec4(0.0);
    
    // Độ mờ tăng dần dựa trên khoảng cách tới rìa màn hình (edgeDist càng gần 0 tức là càng ra sát rìa)
    float blurScale = mix(0.006, 0.002, edgeDist); 
    
    vec2 offsets[] = {
        vec2(-blurScale, -blurScale), vec2(0.0, -blurScale), vec2(blurScale, -blurScale),
        vec2(-blurScale,  0.0),       vec2(0.0,  0.0),       vec2(blurScale,  0.0),
        vec2(-blurScale,  blurScale),  vec2(0.0,  blurScale), vec2(blurScale,  blurScale)
    };
    
    for(int i = 0; i < 9; i++) {
        glow += textureSharpBilinear(tex, uv, offsets[i]);
    }
    
    return (glow / 9.0) * uBloomIntensity;
}

void main() {
    vec2 crtUV = curve(fragTexCoord);
    
    if (crtUV.x < 0.0 || crtUV.x > 1.0 || crtUV.y < 0.0 || crtUV.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float vignetteFactor = crtUV.x * crtUV.y * (1.0 - crtUV.x) * (1.0 - crtUV.y);
    float edgeDist = clamp(pow(16.0 * vignetteFactor, 0.5), 0.0, 1.0); 
    float vignetteDarkness = clamp(pow(16.0 * vignetteFactor, 0.25), 0.0, 1.0);

    float dynamicTwitch = sin(uTime * 80.0) * cos(uTime * 45.0) * uAmount;
    float currentSplit = uAmount + dynamicTwitch;

    vec4 sharpSample = textureSharpBilinear(texture0, crtUV, vec2(0.0));
    vec4 blurrySample = texture(texture0, crtUV);
    vec4 coreColor = mix(blurrySample, sharpSample, edgeDist);

    float r = mix(texture(texture0, vec2(crtUV.x - currentSplit, crtUV.y)).r, textureSharpBilinear(texture0, crtUV, vec2(-currentSplit, 0.0)).r, edgeDist);
    float b = mix(texture(texture0, vec2(crtUV.x + currentSplit, crtUV.y)).b, textureSharpBilinear(texture0, crtUV, vec2(currentSplit, 0.0)).b, edgeDist);
    vec4 finalSharpColor = vec4(r, coreColor.g, b, coreColor.a);

    vec4 glowColor = getDynamicGlow(texture0, crtUV, edgeDist);

    vec4 baseColor = finalSharpColor;

    vec2 texSize = vec2(textureSize(texture0, 0));
    float pixelY = floor(crtUV.y * texSize.y);
    float scanline = 1.0;
    if (mod(pixelY, scanlineFrequency) < 1.0) {
        scanline = 1.0 - scanlineWeight;
    }

    finalColor = baseColor * vec4(vec3(scanline * vignetteDarkness), 1.0) * fragColor;
}
