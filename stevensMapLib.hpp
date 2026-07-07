/**
 * stevensMapLib.hpp
 *
 * A library used to expand the functionality of std::map and std::unordered_map
 */

#pragma once

#include <iterator>
#include <functional>
#include <algorithm>
#include <string>
#include <stevensMathLib.h>


namespace stevensMapLib
{
    /*** Helper Functions (Internal) ***/

    /**
     * @brief Internal random number generator for stevensMapLib
     *
     * Uses modern C++ random engine (Mersenne Twister) with thread-local storage.
     * This keeps the library header-only and portable.
     */
    namespace detail
    {
        inline size_t randomIndex(size_t max)
        {
            if (max == 0) return 0;
            std::uniform_int_distribution<size_t> distribution(0, max - 1);
            return distribution(stevensMathLib::getRandomEngine());
        }
    }

    /**
     * @brief Helper template to get a comparison function for pairs
     */
    namespace detail
    {
        template<typename K, typename V>
        using PairComparison = std::function<bool(std::pair<K,V>, std::pair<K,V>)>;

        template<typename Pair, typename Extractor>
        auto makeLessComparator(Extractor extract)
        {
            return [extract](const Pair& a, const Pair& b) {
                return extract(a) < extract(b);
            };
        }

        template<typename Pair, typename Extractor>
        auto makeGreaterComparator(Extractor extract)
        {
            return [extract](const Pair& a, const Pair& b) {
                return extract(a) > extract(b);
            };
        }
    }

    /*** Map Combination Functions ***/

    /**
     * @brief Takes two maplike structures and performs the + operation on their shared keys.
     *
     * NOTE: M is deduced as the full concrete map type (e.g. std::unordered_map<K,V>), rather
     * than deducing K/V separately via a template template parameter — MSVC's template template
     * argument deduction doesn't reliably handle the variadic-Args pattern these functions used
     * to use (std::unordered_map's trailing Hash/KeyEqual/Allocator template arguments), even
     * though GCC/Clang accept it. Use typename M::key_type / typename M::mapped_type internally.
     *
     * @param mapA One of the maplike objects we are adding together.
     * @param mapB The other maplike object we are adding to the first.
     * @param addOperationTarget Target of the + operator: "values" or "keys and values"
     * @param omitKeysNotShared Whether to omit pairs from A and B that they do not share
     *
     * @return M The result of adding the two maps
     */
    template <typename M>
    M addMaps(M mapA, M mapB,
                   std::string addOperationTarget = "keys and values",
                   bool omitKeysNotShared = false)
    {
        M result;
        const bool addValues = (addOperationTarget == "values");

        // Process pairs from map A
        for (const auto& [key, value] : mapA) {
            if (mapB.find(key) == mapB.end()) {
                if (!omitKeysNotShared) {
                    result[key] = value;
                }
                continue;
            }

            // Key exists in both maps - add them
            if (addValues) {
                result[key] = mapA.at(key) + mapB.at(key);
            } else {
                result[key + key] = mapA.at(key) + mapB.at(key);
            }
        }

        // Add unmatched keys from map B (only if not omitting)
        if (!omitKeysNotShared) {
            for (const auto& [key, value] : mapB) {
                if (result.find(key) == result.end()) {
                    result[key] = value;
                }
            }
        }

        return result;
    }

    /**
     * @brief Copies the elements of a source map to a destination map.
     *
     * @param sourceMap The map from which we are copying keys and values.
     * @param destinationMap The map to which keys and values are being copied.
     * @param overwrite If true, sourceMap's value is used for commonly-held keys.
     *                  If false, destinationMap's value is preserved.
     *
     * @return A maplike object with the combined keys and values.
     */
    template <typename M>
    M combineMaps(M sourceMap, M destinationMap, bool overwrite = false)
    {
        if (!overwrite) {
            destinationMap.insert(sourceMap.begin(), sourceMap.end());
            return destinationMap;
        }

        for (const auto& [key, value] : sourceMap) {
            destinationMap[key] = value;
        }
        return destinationMap;
    }

    /*** Conversion Functions ***/

    /**
     * @brief Converts a maplike object to a vector of pairs.
     *
     * @param map The maplike object to convert to a vector.
     * @return std::vector<std::pair<K,V>> A vector containing all key-value pairs.
     */
    template <typename M>
    std::vector<std::pair<typename M::key_type, typename M::mapped_type>> mapToVector(const M& map)
    {
        using K = typename M::key_type;
        using V = typename M::mapped_type;
        std::vector<std::pair<K,V>> result;
        result.reserve(map.size());

        for (const auto& [key, value] : map) {
            result.emplace_back(key, value);
        }
        return result;
    }

    /**
     * @brief Converts a maplike object to a vector of tuples.
     *
     * @param map The map to convert to a vector of tuples.
     * @return std::vector<std::tuple<K,V>> A vector containing tuples of key-value pairs.
     */
    template <typename M>
    std::vector<std::tuple<typename M::key_type, typename M::mapped_type>> mapToVecOfTuples(M map)
    {
        using K = typename M::key_type;
        using V = typename M::mapped_type;
        std::vector<std::tuple<K,V>> result;
        result.reserve(map.size());

        for (const auto& [key, value] : map) {
            result.emplace_back(key, value);
        }
        return result;
    }

    /**
     * @brief Converts a maplike object to a sorted vector of pairs.
     *
     * @param map The maplike object to convert.
     * @param sortBy The element to sort by: "key" or "value"
     * @param order The sort order: "ascending" or "descending"
     *
     * @return A vector containing all key-value pairs, sorted as requested.
     */
    template <typename M>
    std::vector<std::pair<typename M::key_type, typename M::mapped_type>> mapToSortedVector(const M& map,
                                                   const std::string& sortBy = "value",
                                                   const std::string& order = "ascending")
    {
        using K = typename M::key_type;
        using V = typename M::mapped_type;
        std::vector<std::pair<K,V>> result = mapToVector(map);

        // Validate parameters
        if (sortBy != "key" && sortBy != "value") {
            throw std::invalid_argument(
                "stevensMapLib::mapToSortedVector() \"" + sortBy +
                "\" is not a valid sort target. Valid values are \"key\" and \"value\"");
        }

        if (order != "ascending" && order != "descending") {
            throw std::invalid_argument(
                "stevensMapLib::mapToSortedVector() \"" + order +
                "\" is not a valid sorting order. Valid values are \"ascending\" and \"descending\"");
        }

        // Sort based on parameters
        if (sortBy == "value") {
            if (order == "ascending") {
                std::sort(result.begin(), result.end(),
                    [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
                        return a.second < b.second;
                    });
            } else {
                std::sort(result.begin(), result.end(),
                    [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
                        return a.second > b.second;
                    });
            }
        } else {
            if (order == "ascending") {
                std::sort(result.begin(), result.end(),
                    [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
                        return a.first < b.first;
                    });
            } else {
                std::sort(result.begin(), result.end(),
                    [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
                        return a.first > b.first;
                    });
            }
        }

        return result;
    }

    /*** Key Extraction Functions ***/

    /**
     * @brief Gets a vector containing all keys from the map.
     *
     * @param map The maplike object from which to extract keys.
     * @return std::vector<K> A vector containing all keys.
     */
    template <typename M>
    std::vector<typename M::key_type> getKeyVector(const M& map)
    {
        using K = typename M::key_type;
        std::vector<K> keys;
        keys.reserve(map.size());

        for (const auto& [key, value] : map) {
            keys.push_back(key);
        }
        return keys;
    }

    /**
     * @brief Gets a vector containing all values from the map.
     *
     * @param map The maplike object from which to extract values.
     * @return std::vector<V> A vector containing all values.
     */
    template <typename M>
    std::vector<typename M::mapped_type> getValueVector(const M& map)
    {
        using V = typename M::mapped_type;
        std::vector<V> values;
        values.reserve(map.size());

        for (const auto& [key, value] : map) {
            values.push_back(value);
        }
        return values;
    }

    /**
     * @brief Returns the first key in the maplike object.
     *
     * @param map The map object from which to get the first key.
     * @return The first key from the map.
     */
    template <typename M>
    typename M::key_type getFirstKey(const M& map)
    {
        return map.begin()->first;
    }

    /**
     * @brief Returns the value from the first pair in the maplike object.
     *
     * @param map The map object from which to get the first value.
     * @return The first value from the map.
     */
    template <typename M>
    typename M::mapped_type getFirstValue(const M& map)
    {
        return map.begin()->second;
    }

    /**
     * @brief Finds the first key associated with a given value (reverse lookup).
     *
     * @param map The maplike object to search.
     * @param value The value to search for.
     * @return The first key associated with the value, or a default-constructed K if not found.
     */
    template <typename M>
    typename M::key_type getKeyByValue(const M& map, const typename M::mapped_type& value)
    {
        using K = typename M::key_type;
        for (const auto& [key, val] : map) {
            if (val == value) {
                return key;
            }
        }

        // Value not found - return default-constructed key
        return K{};
    }

    /**
     * @brief Gets a random key from a maplike object.
     *
     * @param map The map object from which to pick a random key.
     * @return A random key found in map.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    typename M::key_type getRandomKey(const M& map)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::getRandomKey() cannot get a random key from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, detail::randomIndex(map.size()));
        return iterator->first;
    }

    /**
     * @brief Gets a random value from a maplike object.
     *
     * @param map The map object from which to pick a random value.
     * @return A random value found in map.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    typename M::mapped_type getRandomValue(const M& map)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::getRandomValue() cannot get a random value from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, detail::randomIndex(map.size()));
        return iterator->second;
    }

    /*** Min/Max Pair Functions ***/

    /**
     * @brief Helper function to find an extreme pair (min or max) based on a comparison.
     *
     * @param map The maplike object to search.
     * @param compare Comparison function returning true if first argument is "better".
     * @param extractValue Function to extract the value to compare from a pair.
     * @param functionName Name of the calling function for error messages.
     * @return The pair with the extreme value.
     */
    template <typename M, typename Compare, typename Extract>
    std::pair<typename M::key_type, typename M::mapped_type> findExtremePair(const M& map, Compare compare, Extract extractValue,
                                    const std::string& functionName)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::" + functionName + "() cannot get a pair from an empty map");
        }

        auto extremeIter = map.begin();
        auto extremeValue = extractValue(*extremeIter);

        for (auto it = map.begin(); it != map.end(); ++it) {
            auto currentValue = extractValue(*it);
            if (compare(currentValue, extremeValue)) {
                extremeValue = currentValue;
                extremeIter = it;
            }
        }

        return std::pair(extremeIter->first, extremeIter->second);
    }

    /**
     * @brief Returns the key-value pair with the greatest value.
     *
     * @param map The maplike object with numeric values.
     * @return A std::pair<K,V> containing the greatest value.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    std::pair<typename M::key_type, typename M::mapped_type> getPairWithMaxValue(const M& map)
    {
        using V = typename M::mapped_type;
        return findExtremePair(map,
            std::greater<V>(),
            [](const auto& pair) { return pair.second; },
            "getPairWithMaxValue");
    }

    /**
     * @brief Returns the key-value pair with the smallest value.
     *
     * @param map The maplike object with numeric values.
     * @return A std::pair<K,V> containing the smallest value.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    std::pair<typename M::key_type, typename M::mapped_type> getPairWithMinValue(const M& map)
    {
        using V = typename M::mapped_type;
        return findExtremePair(map,
            std::less<V>(),
            [](const auto& pair) { return pair.second; },
            "getPairWithMinValue");
    }

    /**
     * @brief Returns the key-value pair with the greatest key.
     *
     * @param map The maplike object with comparable keys.
     * @return A std::pair<K,V> containing the greatest key.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    std::pair<typename M::key_type, typename M::mapped_type> getPairWithMaxKey(const M& map)
    {
        using K = typename M::key_type;
        return findExtremePair(map,
            std::greater<K>(),
            [](const auto& pair) { return pair.first; },
            "getPairWithMaxKey");
    }

    /*** Aggregation Functions ***/

    /**
     * @brief Sums all values in the map.
     *
     * @param map The map containing numeric values to sum.
     * @param initialValue The value to begin the sum from.
     * @return The sum of all values in the map added to the initial value.
     */
    template <typename M>
    typename M::mapped_type sumAllValues(M map, typename M::mapped_type initialValue = 0)
    {
        using V = typename M::mapped_type;
        V sum = initialValue;
        for (const auto& [key, value] : map) {
            sum += value;
        }
        return sum;
    }

    /**
     * @brief Multiplies each value in the map by a factor.
     *
     * @param map The maplike object with values to multiply.
     * @param factor The factor to multiply each value by.
     * @return The map with all values multiplied by the factor.
     */
    template <typename M>
    M multiplyWithValues(M map, long double factor)
    {
        for (auto& [key, value] : map) {
            value = value * factor;
        }
        return map;
    }

    /**
     * @brief Sets all negative values in the map to zero.
     *
     * @param map The maplike object with numeric values to modify (by reference).
     */
    template<typename M>
    void setNegativeValuesToZero(M& map)
    {
        for (auto& [key, value] : map) {
            if (value < 0) {
                value = 0;
            }
        }
    }

    /*** Query Functions ***/

    /**
     * @brief Checks if a map contains exactly one key and that key matches the parameter.
     *
     * @param map The maplike object to check.
     * @param key The key to check for.
     * @return true if map contains only the specified key, false otherwise.
     */
    template <typename M>
    bool containsOnly(const M& map, const typename M::key_type& key)
    {
        return map.size() == 1 && map.find(key) != map.end();
    }

    /*** Modification Functions ***/

    /**
     * @brief Erases multiple keys from a map.
     *
     * @param map The map from which to erase keys.
     * @param keysToErase A vector of keys to erase from the map.
     * @return The map with specified keys erased.
     */
    template <typename M>
    M erase(M map, const std::vector<typename M::key_type>& keysToErase)
    {
        for (const auto& key : keysToErase) {
            map.erase(key);
        }
        return map;
    }

    /**
     * @brief Removes and returns a random key-value pair from the map.
     *
     * @param map The maplike object from which to pop a pair.
     * @return A random pair from the map.
     * @throws std::invalid_argument if the map is empty.
     */
    template <typename M>
    std::pair<typename M::key_type, typename M::mapped_type> popRandom(M& map)
    {
        using K = typename M::key_type;
        using V = typename M::mapped_type;
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::popRandom() cannot pop a random pair from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, detail::randomIndex(map.size()));

        std::pair<K,V> randomPair(iterator->first, iterator->second);
        map.erase(iterator);
        return randomPair;
    }

    /**
     * @brief Inserts a range of integers as std::string keys into the map.
     *
     * @param map The maplike object to insert keys into.
     * @param rangeStart The first number to insert as a key.
     * @param rangeEnd The last number to insert as a key.
     */
    template <typename M>
    void insertNumericRangeAsStringKeys(M& map,
                                        const long long int& rangeStart,
                                        const long long int& rangeEnd)
    {
        for (long long int i = rangeStart; i <= rangeEnd; ++i) {
            map[std::to_string(i)];
        }
    }

    /*** String Key Operations ***/

    /**
     * @brief Helper to filter pairs based on a std::string key predicate.
     *
     * @param map The maplike object to filter.
     * @param predicate Function returning true for pairs to include.
     * @return Filtered map.
     */
    template <typename M, typename Predicate>
    M filterPairsByKeyPredicate(const M& map, Predicate predicate)
    {
        M result;
        for (const auto& [key, value] : map) {
            if (predicate(key)) {
                result.emplace(key, value);
            }
        }
        return result;
    }

    /**
     * @brief Returns a map containing only pairs whose keys start with a given string.
     *
     * @param map The maplike object to filter.
     * @param prefix The prefix std::string to match.
     * @return A map containing only pairs with keys starting with prefix.
     */
    template <typename M>
    M getPairsWhereKeysStartWith(const M& map, const typename M::key_type& prefix)
    {
        using K = typename M::key_type;
        return filterPairsByKeyPredicate(map,
            [&prefix](const K& key) { return key.starts_with(prefix); });
    }

    /**
     * @brief Returns a map with pairs whose keys start with a given std::string removed.
     *
     * @param map The maplike object to filter.
     * @param prefix The prefix std::string to match for removal.
     * @return A map with matching pairs removed.
     */
    template <typename M>
    M erasePairsWhereKeysStartWith(const M map, const typename M::key_type& prefix)
    {
        using K = typename M::key_type;
        return filterPairsByKeyPredicate(map,
            [&prefix](const K& key) { return !key.starts_with(prefix); });
    }

    /**
     * @brief Erases all instances of a substring from all keys in the map.
     *
     * @param map The maplike object with std::string keys to modify.
     * @param substring The substring to erase from all keys.
     * @return A map with the substring erased from all keys.
     */
    template <typename M>
    M eraseStringFromKeys(const M& map, const typename M::key_type& substring)
    {
        using K = typename M::key_type;
        M result;
        // If substring is empty, return a copy of the original map
        if (substring.empty()) {
            return map;
        }

        for (const auto& [key, value] : map) {
            K modifiedKey = key;
            size_t pos = 0;
            while ((pos = modifiedKey.find(substring, pos)) != K::npos) {
                modifiedKey.erase(pos, substring.length());
            }
            result.emplace(modifiedKey, value);
        }
        return result;
    }

    /**
     * @brief Creates a unique key std::string for the map.
     *
     * @param map The maplike object for which to create a unique key.
     * @param keyString The base key std::string to make unique.
     * @param algorithm The method to use for making the key unique.
     * @return A unique key std::string not present in the map.
     */
    template <typename M>
    typename M::key_type createUniqueKeyString(const M& map, typename M::key_type keyString = "",
                           const std::string& algorithm = "integer concatenation")
    {
        using K = typename M::key_type;
        if (algorithm != "integer concatenation") {
            return keyString;
        }

        K originalKeyString = keyString;
        int suffix = 0;

        while (map.find(keyString) != map.end()) {
            keyString = originalKeyString + std::to_string(suffix);
            suffix++;
        }

        return keyString;
    }
};