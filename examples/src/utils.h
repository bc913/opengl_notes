#ifndef _UTILS_H
#define _UTILS_H

namespace Utility
{
	bool parse_file_into_str(const char* file_name, char* shader_str, int max_len);

	namespace texture
	{
		unsigned int load(const char* texture_path);
	}
}

#endif