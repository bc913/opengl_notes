#include "utils.h"
#include <cstdio> // sprintf_s
#include <iostream>

namespace Utility
{
	bool parse_file_into_str(const char* file_name, char* shader_str, int max_len)
	{
		/*auto file = std::filesystem::path(file_name);
		if (!std::filesystem::exists(file))
			std::cout << "Can not find the file\n";

		std::filesystem::create_directory("Osman");

		std::ifstream t(file_name);
		if (t.fail())
			std::cout << "Fail to read\n";
		std::stringstream buffer;
		buffer << t.rdbuf();
		shader_str = buffer.str();*/

		FILE* file = fopen(file_name, "r");
		if (!file) 
		{
			std::cerr << "ERROR: opening file for reading: " << file_name << "\n";
			return false;
		}
		size_t cnt = fread(shader_str, 1, max_len - 1, file);
		if ((int)cnt >= max_len - 1) 
		{
			std::cout << "WARNING: file " << file_name << " too big - truncated.\n";
		}
		if (ferror(file)) 
		{
			fclose(file);
			std::cerr << "ERROR: reading shader file: " << file_name << "\n";
			return false;
		}

		// append \0 to end of file string
		shader_str[cnt] = 0;
		fclose(file);
		return true;
	}

}