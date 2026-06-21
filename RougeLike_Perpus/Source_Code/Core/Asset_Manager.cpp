
#include "Asset_Manager.h"

Asset_Manager::Asset_Manager() {
	//contruktor kosong
}

Asset_Manager::~Asset_Manager() {
	Clear_All_Asset();
}

void Asset_Manager::Add_Texture(std::string name_texture, const char* filename) {
	//cek texture sebelum di load
	if (texture.find(name_texture) == texture.end()) {
		Texture2D _tex = LoadTexture(filename);

		//set texdure agar tidak burik
		SetTextureFilter(_tex, TEXTURE_FILTER_BILINEAR);

		texture[name_texture] = _tex;
		std::cout << "INFO: [ASSET] Texture Berhasil Di Muat! " << name_texture << std::endl;
	}
}

Texture2D Asset_Manager::Get_Texture(std::string nama_texture) {
	if (texture.find(nama_texture) != texture.end()) {
		return texture[nama_texture];
	}

	std::cerr << "WARNING: [ERROR] Texture Tidak Ditemukan! " << nama_texture << std::endl;

	return { 0 };
}

void Asset_Manager::Add_Shader(std::string nama_shader, const char* vs_filename, const char* fs_filename) {
	if (shader.find(nama_shader) == shader.end()) {
		Shader _shader = LoadShader(vs_filename, fs_filename);
		shader[nama_shader] = _shader;
		std::cout << "INFO: [SHADER] Shader Berhasil Di Muat!" << std::endl;
	}
}

Shader Asset_Manager::Get_Shader(std::string nama_shader) {
	if (shader.find(nama_shader) != shader.end()) {
		return shader[nama_shader];
	}
	std::cerr << "WARNING [ERROR] Shader Tidak Ditemukan!" << nama_shader << std::endl;
	return { 0 };
}

void Asset_Manager::Clear_All_Asset() {
	//hapus semua asset yang duah tidak digunakan dari memrory
	for (auto const& _texture : texture) {
		UnloadTexture(_texture.second);
	}
	texture.clear();

	for (auto const _shader : shader) {
		UnloadShader(_shader.second);
	}
	shader.clear();

	std::cout << "INFO: [ASSET] Semua Texture  Dan Shader Berhasil Di Bersihkan!" << std::endl;

}