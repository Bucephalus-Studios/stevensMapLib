/**
 * stevensStringLib.h
 *
 * Minimal implementation of string utilities needed by stevensMapLib
 * Note: This is a stub for testing. For production use, link against the full stevensStringLib.
 */

#ifndef STEVENSSTRINGLIB
#define STEVENSSTRINGLIB

#include <string>
#include <algorithm>

namespace stevensStringLib
{
    /**
     * @brief Checks if a string starts with a given prefix
     *
     * @param str The string to check
     * @param prefix The prefix to look for
     * @return true if str starts with prefix, false otherwise
     */
    template<typename StringType>
    bool startsWith(const StringType& str, const StringType& prefix)
    {
        if (prefix.size() > str.size()) {
            return false;
        }
        return std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    /**
     * @brief Replaces all occurrences of a substring with another string
     *
     * @param str The source string
     * @param from The substring to replace
     * @param to The replacement string
     * @return A new string with replacements made
     */
    template<typename StringType>
    StringType replaceSubstr(StringType str, const StringType& from, const StringType& to)
    {
        if (from.empty()) {
            return str;
        }

        size_t startPos = 0;
        while ((startPos = str.find(from, startPos)) != StringType::npos) {
            str.replace(startPos, from.length(), to);
            startPos += to.length();
        }
        return str;
    }
}

#endif
