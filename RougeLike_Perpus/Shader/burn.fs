
#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0; // Mengambil data tekstur gambar .png secara otomatis dari Raylib
uniform float uDissolve;    // Nilai progress pembakaran (0.0f s/d 1.0f)

#define BURN_SIZE 0.08      // Lebar area api yang membakar

#define PIXEL_RESOLUTION vec2(160.0, 240.0) //untuk efect sdkiti pixel

// 1. Fungsi Generator Angka Acak Dasar
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

// 2. Smooth Value Noise (Menghaluskan transisi antar titik)
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    // Menggunakan kurva Hermite (sama seperti smoothstep) untuk interpolasi
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    // Mengambil 4 sudut dari kotak grid
    float a = hash(i + vec2(0.0, 0.0));
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    // Mencampur warna (bilinear interpolation) dengan mulus
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// 3. FBM (Fractal Brownian Motion) untuk tekstur organik berlapis
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    
    // Menumpuk 4 lapis noise dengan skala berbeda
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p);
        p *= 2.0;       // Rapatkan frekuensi
        amplitude *= 0.5; // Turunkan intensitas
    }
    return value;
}

void main()
{

    // UV Quantization
    // Membulatkan koordinat UV yang mulus menjadi grid kotak-kotak kasar
    vec2 pixelatedUV = floor(fragTexCoord * PIXEL_RESOLUTION) / PIXEL_RESOLUTION;
    
    //tekstur kartu menggunakan UV asli agar gambar kartu tetap tajam
    vec4 texColor = texture(texture0, fragTexCoord) * fragColor;
    
    // Abaikan pixel transparan
    if (texColor.a < 0.1) {
        discard;
    }
    
    if (uDissolve <= 0.01) {
        finalColor = texColor;
        return;
    }
    

    //membuat efek noise dan api menjadi koatak!
    float n = fbm(pixelatedUV * 9.0);
    
    // Kikis kartu
    if (n < uDissolve) {
        discard;
    }
    
    float edgeDistance = n - uDissolve;
    
    // Warnai tepian lubang
    if (edgeDistance < BURN_SIZE) {
        float t = edgeDistance / BURN_SIZE;
        
        vec3 fireColor;
        if (t < 0.3) {
            fireColor = mix(vec3(1.0, 1.0, 0.5), vec3(1.0, 0.8, 0.0), t / 0.3);
        } else if (t < 0.7) {
            fireColor = mix(vec3(1.0, 0.8, 0.0), vec3(0.8, 0.1, 0.0), (t - 0.3) / 0.4);
        } else {
            fireColor = mix(vec3(0.8, 0.1, 0.0), vec3(0.1, 0.05, 0.05), (t - 0.7) / 0.3);
        }
        
        texColor.rgb = fireColor;
        texColor.rgb *= (1.0 - t) * 2.5 + 1.0; 
    }
    
    finalColor = texColor;
}