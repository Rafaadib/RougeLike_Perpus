
#version 330

// Mengambil input koordinat tekstur langsung dari DrawTextureRec (0.0 sampai 1.0)
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform vec2 uResolution;
uniform float uTime;

// Kombinasi palet warna merah-biru Balatro
#define COLOR_1 vec4(0.04, 0.32, 0.24, 1.0) // Hijau blebih cerah)
#define COLOR_2 vec4(0.12, 0.52, 0.38, 1.0) 
#define COLOR_3 vec4(15.0/255.0, 99.0/255.0, 76.0/255.0, 1.0) 

#define PIXEL_FILTER 1000.0 
#define CONTRAST 3.5

void main()
{
    // Ambil koordinat pixel murni layar
    vec2 fragCoord = fragTexCoord * uResolution;

    //Efek Pikselasi Retro (Diselaraskan dengan tinggi layar agar aspek rasio pas)
    float pixel_size = length(uResolution.xy) / PIXEL_FILTER;
    vec2 uv = (floor(fragCoord.xy * (1.0 / pixel_size)) * pixel_size - 0.5 * uResolution.xy) / uResolution.y;
    
    // Sistem Gerakan In-Game (Iterative Domain Warping Plasma)
    uv *= 15.0; 
    
    // Mengatur tempo aliran lambat (0.4)
    float speed = uTime * 3.0 + 280.2; 
    vec2 uv2 = vec2(uv.x + uv.y);
    
    // Loop 5x untuk mengacak lekukan marmer cairan
    for(int i = 0; i < 5; i++) {
        uv2 += uv + cos(length(uv));
        uv += 0.5 * vec2(
            cos(5.11 + 0.35 * uv2.y + speed * 0.13),
            sin(uv2.x - 0.11 * speed)
        );
        uv -= 1.0 * cos(uv.x + uv.y) - 1.0 * sin(uv.x * 0.71 - uv.y);
    }
    
    // Kalkulasi Pembagian 3 Lapisan Warna Asli (Koreksi Rumus Math)
    float contrast_mod = (0.25 * CONTRAST + 1.2);
    float paint_res = min(2.0, max(0.0, length(uv) * (0.035) * contrast_mod));
    
    float c1p = max(0.0, 1.0 - contrast_mod * abs(1.0 - paint_res));
    float c2p = max(0.0, 1.0 - contrast_mod * abs(paint_res));
    float c3p = 1.0 - min(1.0, c1p + c2p);
    
    //Blending Akhir Warna Cairan Meja Judi
    vec4 ret_col = (0.3 / CONTRAST) * COLOR_1 + 
                   (1.0 - 0.3 / CONTRAST) * (COLOR_1 * c1p + COLOR_2 * c2p + vec4(c3p * COLOR_3.rgb, c3p * COLOR_1.a));
    
    finalColor = ret_col;
}