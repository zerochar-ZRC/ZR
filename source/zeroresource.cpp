#include "zeroresource.h"
#include <format>
#include <filesystem>
#include <iostream>
using byte_buffer = std::vector<char>;

//ZeroResourceManager
struct ZRM {
	std::fstream file;
	std::filesystem::path path;
	zr::Type mode;
	byte_buffer crypt_key; //later
	
	size_t element = 0;
	size_t max_elements = 0;

	void crypt(byte_buffer& buff){	
		std::size_t i = 0;
		for (char& c : buff) {
			c ^= crypt_key[i];
			i = (i + 1) % crypt_key.size();
		}
	}
};
static ZRM zrm{};

bool zr::start(Type type, std::string_view file_name,std::string_view key) {
	size_t len = key.size();
	zrm.crypt_key.clear();
	zrm.crypt_key.reserve(len);
	for(size_t i = 0;i!=len;i++) zrm.crypt_key.push_back(key[i]);
	
	if (zrm.file.is_open()) return false;
	zrm.mode = type; 
	if (zrm.mode == zr::Type::Any) return false;

	zrm.path = std::format("./{}.zrc", file_name);
	zrm.file.open(
		zrm.path, 
		std::ios::binary | (zrm.mode == zr::Type::Read ? std::ios::in : std::ios::out)
	);
	if (!zrm.file.is_open()) return false;

	zrm.element = 0;
	zrm.max_elements = 0;
	if (zrm.mode == zr::Type::Read) {
		zrm.file.seekg(-static_cast<long long>(sizeof(zrm.max_elements)), std::ios::end);
		zrm.file.read(reinterpret_cast<char*>(&zrm.max_elements),sizeof(zrm.max_elements));
		zrm.file.seekg(0, std::ios::beg);
	}
}
bool zr::isAvaibleFor(zr::Type type) {
	return zrm.file.is_open() && (zrm.mode == type || type == zr::Type::Any) && !zr::isEOF();
}
bool zr::isEOF(){
	if (zrm.mode == zr::Type::Write) return false;
	return zrm.element == zrm.max_elements;
}

void zr::write(const void* obj,size_t size)
{
	if (!isAvaibleFor(zr::Type::Write)) return; 
	zrm.file.write(reinterpret_cast<char*>(&size),sizeof(size));
	zrm.file.write(static_cast<const char*>(obj), size);
	zrm.max_elements++;
}

void zr::writeString(std::string_view text) 
{
	zr::write(static_cast<const void*>(text.data()), text.length());
}

bool zr::read(void* buff, size_t excepted)
{
	if (!isAvaibleFor(zr::Type::Read)) return false;

	size_t elem_size{};
	zrm.file.read(reinterpret_cast<char*>(&elem_size),sizeof(elem_size));
#if not ZRM_UNSAFE
	if (elem_size != excepted) return false;
#endif
	zrm.file.read(static_cast<char*>(buff), elem_size);
	zrm.element++;
	return true;
}
std::string zr::readString() 
{
	if (!isAvaibleFor(zr::Type::Read)) return "";
	size_t elem_size{};
	zrm.file.read(reinterpret_cast<char*>(&elem_size), sizeof(elem_size));
#if not ZRM_UNSAFE
	if (elem_size != excepted) return false;
#endif
	byte_buffer buff{};
	buff.resize(elem_size+1,'\0');
	zrm.file.read(buff.data(), elem_size);
	zrm.element++;
	return std::string(buff.data());
}

void zr::end() 
{
	if (!isAvaibleFor(zr::Type::Any)) return;
	if (zrm.mode == zr::Type::Write) { // no needs to check avability if passed upp
		zrm.file.write(reinterpret_cast<char*>(&zrm.max_elements),sizeof(zrm.max_elements));
	}
	zrm.file.close();
}	