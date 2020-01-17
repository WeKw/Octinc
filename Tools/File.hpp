/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#if !defined(OCTINC_FILE_HPP) && defined(_WIN32)
#  define OCTINC_FILE_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  ifndef STD_WINABLE_H
#    define STD_WINABLE_H
#    include <Winable.h>
#  endif
#  ifndef STD_WINDOWS_H
#    define STD_WINDOWS_H
#    include <Windows.h>
#  endif
#  ifndef STD_DIRECT_H
#    define STD_DIRECT_H
#    include <direct.h>
#  endif
#  ifndef STD_IO_H
#    define STD_IO_H
#    include <io.h>
#  endif

namespace Octinc {
class File {
   public:
    struct timer {
        int year, month, day, hour, minute, second;

        timer(int y = 0, int m = 0, int d = 0, int h = 0, int mi = 0, int s = 0)
            : year(y), month(m), day(d), hour(h), minute(mi), second(s) {}
    };

   private:
    static const int BUFSIZE = 4096, EXIST = 0, READABLE = 2, WRITABLE = 4,
                     EXECUTABLE = 6;
    _finddata_t fileInfo;
    std::string absolute_path, path, name;
    void Update(std::string);
    timer getTime(time_t tm);

   public:
    File(std::string pathname);
    std::string getName();          // Get the filename.
    std::string getParent();        // Get the parent path.
    File getParentFile();           // Get a File object from the parent path.
    std::string getPath();          // Get the path.
    bool isAbsolute();              // Determine whether it is an absolute path.
    std::string getAbsolutePath();  // Get the absolute path.
    File getAbsoluteFile();         // Get a File object from the absolute path.
    timer getCreateTime();          // Get the create time.
    timer getAccessTime();          // Get the last access time.
    timer getWriteTime();           // Get the last modify time.
    bool canRead();                 // Determine whether it is readable.
    bool canWrite();                // Determine whether it is writable.
    bool canRun();                  // Determine whether it is runnable.
    bool exists();                  // Determine whether it exists.
    bool isDirectory();             // Determine whether it is a directory.
    bool isFile();                  // Determine whether is is a text.
    bool isHidden();                // Detetmine whether it is hidden.
    long long length();             // Get the file byte size.
    bool Delete();                  // Delete the file.
    std::vector<std::string> list();  // Get a list of subfiles.
    std::vector<File> listFile();  // Get a list of File objects for subfiles.
    bool mkdir();                  // Create a new directory..
    bool renameTo(std::string newName);  // Remane.
};

File::File(std::string pathname) { Update(pathname); }

void File::Update(std::string pathname) {
    path = pathname;
    TCHAR buffer[BUFSIZE] = TEXT("");
    TCHAR* lppPart = {NULL};
    GetFullPathName((LPCWSTR)pathname.c_str(), BUFSIZE, buffer, &lppPart);

    if (lppPart != NULL) name = (char *)lppPart;

    absolute_path = (char *)buffer;
    long hFile = _findfirst(absolute_path.c_str(), &fileInfo);
    _findclose(hFile);
}

std::string File::getName() { return name; }

std::string File::getAbsolutePath() { return absolute_path; }

std::string File::getPath() { return path; }

std::string File::getParent() {
    return absolute_path.substr(0, absolute_path.rfind("\\"));
}

bool File::isAbsolute() { return path == absolute_path; }

bool File::isDirectory() {
    if (exists()) return fileInfo.attrib == _A_SUBDIR;

    return false;
}

bool File::isHidden() {
    if (exists()) return fileInfo.attrib == _A_HIDDEN;

    return false;
}

bool File::exists() { return !_access(path.c_str(), EXIST); }

bool File::canRead() {
    if (exists()) return !_access(path.c_str(), READABLE);

    return false;
}

bool File::canWrite() {
    if (exists()) return !_access(path.c_str(), WRITABLE);

    return false;
}

bool File::canRun() {
    if (exists()) return !_access(path.c_str(), EXECUTABLE);

    return false;
}

bool File::isFile() {
    if (exists()) return fileInfo.attrib == _A_ARCH;

    return false;
}

bool File::mkdir() {
    if (_mkdir(absolute_path.c_str())) {
        Update(absolute_path);
        return true;
    }

    return false;
}

bool File::renameTo(std::string newName) {
    std::string newPath = getParent() + "\\" + newName;

    if (exists() && rename(absolute_path.c_str(), newPath.c_str())) {
        Update(newPath);
        return true;
    }

    return false;
}

bool File::Delete() {
    if (exists() && remove(absolute_path.c_str())) return true;

    return false;
}

long long File::length() { return fileInfo.size; }

File::timer File::getTime(time_t tt) {
    tm t = *localtime(&tt);
    return timer(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min,
                 t.tm_sec);
}

File::timer File::getCreateTime() {
    if (exists()) return getTime(fileInfo.time_create);

    return timer();
}

File::timer File::getAccessTime() {
    if (exists()) return getTime(fileInfo.time_access);

    return timer();
}

File::timer File::getWriteTime() {
    if (exists()) return getTime(fileInfo.time_write);

    return timer();
}

File File::getParentFile() { return File(getParent()); }

File File::getAbsoluteFile() { return File(getAbsoluteFile()); }

std::vector<std::string> File::list() {
    std::vector<std::string> lst;

    if (isDirectory()) {
        _finddata_t temp;
        long find = _findfirst((absolute_path + "\\*").c_str(), &temp);
        _findnext(find, &temp);
        _findnext(find, &temp);

        do {
            lst.push_back(temp.name);
        } while (_findnext(find, &temp) == 0);

        _findclose(find);
    }

    return lst;
}

std::vector<File> File::listFile() {
    std::vector<File> lstf;

    if (isDirectory())
        for (auto& it : list()) lstf.push_back(File(absolute_path + "\\" + it));

    return lstf;
}
}  // namespace Octinc

#endif
