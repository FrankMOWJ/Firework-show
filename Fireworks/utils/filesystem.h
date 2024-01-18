#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>
#include <cstdlib>
#include<Windows.h>

static char const* logl_root = "D:/linzhy/Code/visualStudioProjects/Fireworks/resources";

class FileSystem
{
public:
	static std::string getPath(std::string path)
	{
		// Get the full path of the executable
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);

		// Extract the directory from the path
		wchar_t* pos = wcsrchr(buffer, L'\\');
		if (pos != NULL) {
			*pos = L'\0'; // Null-terminate the string at the last backslash
		}

		// Find the last backslash in the modified path
		wchar_t* lastBackslash = wcsrchr(buffer, L'\\');

		// Check if a backslash was found
		if (lastBackslash != NULL) {
			// Replace the last directory with a new folder name
			wcscpy(lastBackslash + 1, L"resources");
		}
		std::wstring wstrPath(buffer);
		for (wchar_t& ch : wstrPath) {
			if (ch == L'\\') {
				ch = L'/';
			}
		}
		std::string strPath(wstrPath.begin(), wstrPath.end());
		strPath += '/';

		// std::cout << strPath << std::endl;
		// std::string ret = std::string("D:/linzhy/Code/visualStudioProjects/Fireworks/resources/") + path;
		std::string ret = strPath + path;
		return ret;
	}
};

#endif  // !FILESYSTEM_H
