#ifndef PATH_H
#define PATH_H

#include <string>

/* Struct to take care of deviding the path
 into directory, filename and extention */ 
struct Path {
	//The complete path. Works just like a nomal string. 
	std::string completepath;
	std::string dir();
	std::string filename();
	std::string ext();
	Path();
	Path(const std::string _path);
};

#endif // PATH_H