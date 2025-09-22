# ZeroResources
Simple vars loader/saver from file.

# Usage:
### Start:
[zr::start](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L16) 
-  Requires type (mode of stream) and path (without extension!).
-  Return is success of opening stream or not. [zr::Type::ANY](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L13) always return false.
-  Before restarting - must be called [zr::end](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L27) function!

### Write:
[zr::write](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L18)
-  will read n-bytes and then write it into file.
<!-- end of the list -->
[zr::writeString](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L19)
-  internaly uses [zr::write](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L18) but more efficient with std::string.

### Read:
[zr::read](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L21)
-  writes n-bytes from file in given buff (any type).
<!-- end of the list -->
[zr::readString](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L22)
-  same as [zr::read](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L21) but compatible with zr::writeString.

### Handling:
[zr::isEOF](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L24)
-  returns if current read elements reached max elements. Will return false if write mode.
<!-- end of the list -->
[zr::isAvaibleFor](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L25)
-  checks if stream open, does current type match provided (or skipped if [zr::Type::ANY](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L13)) and [zr::isEOF](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L24).
<!-- end of the list -->
[ZRM_SAFE](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L7)
-  should [zr::read](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L21) checks if provided size and written inside zrc file are same. Only for [zr::read](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L21).

### End:
[zr::end](https://github.com/zerochar-ZRC/ZR/blob/main/headers/zeroresource.h#L27)\
- closes file stream and if write mode - writes how much elements passed.


# Example:
```cpp
#include "zeroresource.h"
int main() {
	size_t w_lvl = 50;
	std::string w_name = "zerochar";
	zr::start(zr::Type::WRITE, "data", "");
	zr::write(&w_lvl,sizeof(w_lvl));
	zr::writeString(w_name);
	zr::end();
	
	size_t lvl = 0;
	std::string name;
	zr::start(zr::Type::READ, "data", "");
	zr::read(&lvl, sizeof(lvl));
	name = zr::readString();
	zr::end();
	std::cout << '\n' << lvl << ' ' << name;
	return 0;
}
```

