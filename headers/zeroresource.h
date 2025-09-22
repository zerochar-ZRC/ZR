#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <format>

#define ZRM_SAFE true

namespace zr {
	enum Type {
		READ,
		WRITE,
		ANY
	};

	bool start(zr::Type,std::string_view);

	void write(const void*,size_t);
	void writeString(std::string_view);
	
	bool read(void*,size_t);
	std::string readString();

	bool isEOF();
	bool isAvaibleFor(zr::Type);

	void end();	
}