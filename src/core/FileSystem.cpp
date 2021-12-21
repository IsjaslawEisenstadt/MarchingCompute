#include "FileSystem.h"

#include <iostream>
#include <sstream>

namespace FileSystem
{
std::optional<std::string> ReadStringFromFile(const std::string &file_path)
{
	const std::ifstream input_stream(file_path, std::ios_base::binary);

	if (input_stream.fail())
		return std::nullopt;

	std::stringstream buffer;
	buffer << input_stream.rdbuf();

	return buffer.str();
}

std::vector<char> ReadBytesFromFile(const std::string &path, std::size_t bytes)
{
	std::ifstream ifs;
	ifs.open(path, std::ios::binary | std::ios::in);
	if (!ifs)
	{
		std::cerr << "Error reading File from " << path << '\n';
		abort();
	}
	std::vector<char> data(bytes);
	ifs.read(data.data(), bytes);
	return data;
}
} // namespace FileSystem
