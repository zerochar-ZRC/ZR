#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <numeric>

#define ZRM_UNSAFE true

namespace zr {
	enum Type {
		Read,
		Write,
		Any //used only for isAvaibleFor! Do not use in others - zr::start would return false always
	};
	/* Inits zrm. Could be used again inside another start to change crypt key - other will be ignored.
	1: type of process
	2: path without extension
	3: crypt key for resolving
	*/
	bool start(Type,std::string_view,std::string_view);

	//Writes size_t data in pointer to zrm buffer.
	void write(const void*,size_t);
	void writeString(std::string_view);
	
	//Zrm tries read next object and fully rewrites provided pointer.
	//If safe mode - will require size of reading object to compare it. If it fails - returns false.
	bool read(void*,size_t);
	std::string readString();

	//Is read object was final. Returns false in write mode
	bool isEOF();
	//Checks stream state, mode and eof if in read mode.
	bool isAvaibleFor(zr::Type);


	//Flush zrm buffers and close streams.
	void end();
	
}