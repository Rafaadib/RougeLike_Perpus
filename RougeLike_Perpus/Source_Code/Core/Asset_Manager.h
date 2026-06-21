
#pragma once

#include "raylib.h"

#include <iostream>
#include <string>
#include <map>

class Asset_Manager {
public:
	Asset_Manager();
	~Asset_Manager();

	//fungsi untuk load texture custom
	void Add_Texture(std::string nama_texture, const char* filename);
	Texture2D Get_Texture(std::string nama_texture);

	//fungsi load untuk shader
	void Add_Shader(std::string nama_shader, const char* vs_filename, const char* fs_filename);
	Shader Get_Shader(std::string nama_shader);

	//bersihkan texture dari memori
	void Clear_All_Asset();

private:
	//mengunakan map yang datanya terurut yang btuth key dan value
	//disini kunci berupa string dan valuenya berupa texture
	std::map<std::string, Texture2D> texture;
	std::map<std::string, Shader> shader;
};

