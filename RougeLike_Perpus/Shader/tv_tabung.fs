
#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0; // Kanvas virtual yang berisi seluruh gambar game

void main()
{
    vec2 uv = fragTexCoord;

    //BARREL DISTORTION (Efek Layar Cembung)
    // Geser titik tengah UV dari (0.5, 0.5) ke (0.0, 0.0) agar lengkungan berpusat di tengah
    uv = uv * 2.0 - 1.0;
    
    //makin jauh dari tengah, semakin ditarik melengkung
    float curvature = 0.001; // Besaran lengkungan. Ubah jika terasa terlalu cembung
    uv *= 1.0 + (uv.yx * uv.yx) * curvature;
    
    // Kembalikan UV ke rentang normal (0.0 sampai 1.0)
    uv = uv * 0.5 + 0.5;

    // Jika setelah dilengkungkan UV keluar dari batas, warnai menjadi hitam polos (bingkai TV)
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Ambil warna asli layar dari kanvas virtual
    vec4 texColor = texture(texture0, uv);

    //SCANLINES (Garis horizontal)
    // Kita gunakan gelombang sinus yang sangat rapat pada sumbu Y
    float scanline = sin(uv.y * 900.0) * 0.04; 
    texColor.rgb -= scanline; // Kurangi kecerahan warna asli untuk membuat garis gelap

    //VIGNETTE (Sudut Gelap)
    // Hitung jarak pixel saat ini ke titik tengah layar (0.5, 0.5)
    float distanceToCenter = distance(uv, vec2(0.5, 0.5));
    
    // Smoothstep membuat gradasi mulus: dari jarak 0.4 (terang penuh) hingga 0.8 (gelap penuh)
    float vignette = smoothstep(0.8, 0.5, distanceToCenter);
    texColor.rgb *= vignette;

    // PHOSPHOR GLOW TWEAK (Opsional: Membuat warna sedikit lebih 'retro')
    texColor.g *= 1.05; // Naikkan warna hijau sedikit ala 
    texColor.b *= 1.02; // Naikkan warna biru sedikit

    finalColor = texColor * fragColor;
}