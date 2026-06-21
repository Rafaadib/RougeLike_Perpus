
#version 330

// Mengambil input koordinat tekstur langsung dari DrawTextureRec (0.0 sampai 1.0)
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform vec2 uResolution;
uniform float uTime;

// Kombinasi palet warna merah-biru Balatro
#define COLOR_1 vec4(0.87, 0.26, 0.23, 1.0)
#define COLOR_2 vec4(0.0, 0.42, 0.70, 1.0)
#define COLOR_3 vec4(0.08, 0.13, 0.14, 1.0)

// Mengatur kerapatan piksel kotak retro. (Gunakan 1400.0 agar pixel kecil/halus)
#define PIXEL_FILTER 1400.0 
#define SPIN_AMOUNT 0.25
#define CONTRAST 3.5

void main()
{
    // Mengubah koordinat normalisasi (0-1) menjadi koordinat pixel layar murni
    vec2 fragCoord = fragTexCoord * uResolution;

    // 1. Efek Pikselasi Retro (Pixel Art Filter)
    float pixel_size = length(uResolution.xy) / PIXEL_FILTER;
    vec2 uv = (floor(fragCoord.xy * (1.0 / pixel_size)) * pixel_size - 0.5 * uResolution.xy) / length(uResolution.xy);
    
    float uv_len = length(uv);
    
    // 2. Efek Swirl / Pusaran air
    float speed = uTime * 0.1 + 302.2;
    float angle = atan(uv.y, uv.x) + speed - 20.0 * (SPIN_AMOUNT * uv_len + (1.0 - SPIN_AMOUNT));
    
    vec2 mid = vec2(0.5);
    uv = vec2(uv_len * cos(angle) + mid.x, uv_len * sin(angle) + mid.y);
    
    // 3. Mengontrol kerapatan gelombang plasma (Dikali 65.0 agar ombaknya rapat & kecil)
    uv *= 40.0; 
    
    speed = uTime * 1.5;
    vec2 uv2 = vec2(uv.x + uv.y);
    
    for(int i = 0; i < 5; i++) {
        uv2 += uv + cos(length(uv));
        uv += 0.5 * vec2(
            cos(5.11 + 0.35 * uv2.y + speed * 0.13),
            sin(uv2.x - 0.11 * speed)
        );
        uv -= 1.0 * cos(uv.x + uv.y) - 1.0 * sin(uv.x * 0.71 - uv.y);
    }
    
    // 4. Kalkulasi blending dan rendering warna cairan
    float contrast_mod = (0.25 * CONTRAST + 0.5 * SPIN_AMOUNT + 1.2);
    float paint_res = min(2.0, max(0.0, length(uv) * (0.015 * contrast_mod)));
    
    float c1p = max(0.0, 1.0 - 2.0 * abs(paint_res - 1.0));
    float c2p = max(0.0, 1.0 - 2.0 * abs(paint_res - 1.5));
    float c3p = 1.0 - min(1.0, c1p + c2p);
    
    finalColor = (0.3 / CONTRAST) * COLOR_1 + 
                 (1.0 - 0.3 / CONTRAST) * (COLOR_1 * c1p + COLOR_2 * c2p + vec4(c3p * COLOR_3.rgb, c3p * COLOR_1.a));
}