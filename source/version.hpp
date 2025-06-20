/************************************************************\
Filename:   version.hpp
Author:     Adrian Padin (padin.adrian@gmail.com)
Description:
    This header contains definitions for the version strings.

\************************************************************/


#ifndef  VERSION_HPP_
#define  VERSION_HPP_


/* ===== Includes ===== */
#include <string>


namespace version {

    // For this project there are four parts to the version number:
    // * Major - Denotes major release, contains significant or breaking changes
    // * Minor - Denotes minor release, contains possible interface changes
    // * Bugfix - Denotes bugfix release, changes are internal only and do not break interface
    // * Commit hash - Included to differentiate internal builds before release

    // The version major number
    extern const std::string VERSION_MAJOR;

    // The version minor number
    extern const std::string VERSION_MINOR;

    // The version bugfix number
    extern const std::string VERSION_BUGFIX;

    // The SHA1 hash of the HEAD git commit
    extern const std::string VERSION_GIT_SHA1;

    // The complete version string
    // Format: MAJOR.MINOR.BUGFIX.SHA1_SHORT
    // Note that SHA1_SHORT contains only the first 7 characters of the hash
    // This makes the version string much shorter and more readable
    extern const std::string VERSION_FULL;
}


#endif  // VERSION_HPP_