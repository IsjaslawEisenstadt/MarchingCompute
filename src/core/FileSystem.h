#pragma once

#include <fstream>
#include <optional>
#include <vector>

namespace FileSystem
{
	std::optional<std::string> ReadStringFromFile(const std::string& file_path);

	std::vector<char> ReadBytesFromFile(const std::string& path, std::size_t bytes);
}
