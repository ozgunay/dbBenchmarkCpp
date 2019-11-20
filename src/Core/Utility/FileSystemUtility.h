// FileSystemUtility.h

#ifndef _DBBENCHMARK_FILESYSTEMUTILITY_H_
#define _DBBENCHMARK_FILESYSTEMUTILITY_H_

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

namespace dbbenchmark {
namespace utility {
/**
*   \brief General functions and const values that used in entire project.
*   \details
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/

inline bool checkIfFile(const std::string &filePath) {
    try {
        // Create a Path object from given path string
        boost::filesystem::path pathObj(filePath);
        // Check if path exists and is of a regular file
        if (boost::filesystem::exists(pathObj) && boost::filesystem::is_regular_file(pathObj))
            return true;
    } catch (boost::filesystem::filesystem_error & e) {
        LOG(INFO) << e.what() << std::endl;
    }
    return false;
}

inline bool checkIfDirectory(const std::string &filePath) {
    try {
        // Create a Path object from given path string
        boost::filesystem::path pathObj(filePath);
        // Check if path exists and is of a directory file
        if (boost::filesystem::exists(pathObj) && boost::filesystem::is_directory(pathObj))
            return true;
    }
    catch (boost::filesystem::filesystem_error & e) {
        LOG(INFO) << e.what() << std::endl;
    }
    return false;
}

} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_FILESYSTEMUTILITY_H_
