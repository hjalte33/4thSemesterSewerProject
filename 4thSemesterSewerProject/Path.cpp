#include "stdafx.h"
#include "Path.h"



//returns the directory of the path
std::string Path::dir() {
	std::size_t foundslash = completepath.find_last_of("/\\");
	return completepath.substr(0, foundslash + 1);
}

//returns the filename (if any) of the path
std::string Path::filename() {
	std::size_t foundslash = completepath.find_last_of("/\\");
	std::size_t founddot = completepath.find_last_of(".");
	return completepath.substr(foundslash + 1, founddot - foundslash - 1);
}

// returns the file extention (if any) of the path
std::string Path::ext() {
	std::size_t founddot = completepath.find_last_of(".");
	return completepath.substr(founddot);
}

// default constructor
Path::Path() {
	completepath = " ";
}

/*constructor for the path struct
takes one argument which is the full path as a string */
Path::Path(std::string _path) {
	completepath = _path;
}
