/**
 * stevensMapLib.hpp
 *
 * A library used to expand the functionality of std::map and std::unordered_map
 */

#include <iterator>
#include <functional>
#include <algorithm>
#include <string>

#ifndef STEVENSMAPLIB
#define STEVENSMAPLIB

namespace stevensMapLib
{
    /*** Helper Functions (Internal) ***/

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
     * @param mapA One of the maplike objects we are adding together.
     * @param mapB The other maplike object we are adding to the first.
     * @param addOperationTarget Target of the + operator: "values" or "keys and values"
     * @param omitKeysNotShared Whether to omit pairs from A and B that they do not share
     *
     * @return M<K,V> The result of adding the two maps
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> addMaps(M<K,V> mapA, M<K,V> mapB,
                   std::string addOperationTarget = "keys and values",
                   bool omitKeysNotShared = false)
    {
        M<K,V> result;
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> combineMaps(M<K,V> sourceMap, M<K,V> destinationMap, bool overwrite = false)
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<std::pair<K,V>> mapToVector(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<std::tuple<K,V>> mapToVecOfTuples(M<K,V> map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<std::pair<K,V>> mapToSortedVector(const M<K,V>& map,
                                                   const std::string& sortBy = "value",
                                                   const std::string& order = "ascending")
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<K> getKeyVector(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<V> getValueVector(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K getFirstKey(const M<K,V>& map)
    {
        return map.begin()->first;
    }

    /**
     * @brief Returns the value from the first pair in the maplike object.
     *
     * @param map The map object from which to get the first value.
     * @return The first value from the map.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    V getFirstValue(const M<K,V>& map)
    {
        return map.begin()->second;
    }

    /**
     * @brief Gets a random key from a maplike object.
     *
     * @param map The map object from which to pick a random key.
     * @return A random key found in map.
     * @throws std::invalid_argument if the map is empty.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K getRandomKey(const M<K,V>& map)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::getRandomKey() cannot get a random key from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, rand() % map.size());
        return iterator->first;
    }

    /**
     * @brief Gets a random value from a maplike object.
     *
     * @param map The map object from which to pick a random value.
     * @return A random value found in map.
     * @throws std::invalid_argument if the map is empty.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    V getRandomValue(const M<K,V>& map)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::getRandomValue() cannot get a random value from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, rand() % map.size());
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
    template <template <typename, typename, typename...> class M, typename K, typename V,
              typename... Args, typename Compare, typename Extract>
    std::pair<K,V> findExtremePair(const M<K,V>& map, Compare compare, Extract extractValue,
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::pair<K,V> getPairWithMaxValue(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::pair<K,V> getPairWithMinValue(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::pair<K,V> getPairWithMaxKey(const M<K,V>& map)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    V sumAllValues(M<K,V> map, V initialValue = 0)
    {
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> multiplyWithValues(M<K,V> map, long double factor)
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    bool containsOnly(const M<K,V>& map, const K& key)
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> erase(M<K,V> map, const std::vector<K>& keysToErase)
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
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::pair<K,V> popRandom(M<K,V>& map)
    {
        if (map.empty()) {
            throw std::invalid_argument(
                "stevensMapLib::popRandom() cannot pop a random pair from an empty map");
        }

        auto iterator = map.begin();
        std::advance(iterator, rand() % map.size());

        std::pair<K,V> randomPair(iterator->first, iterator->second);
        map.erase(iterator);
        return randomPair;
    }

    /**
     * @brief Inserts a range of integers as string keys into the map.
     *
     * @param map The maplike object to insert keys into.
     * @param rangeStart The first number to insert as a key.
     * @param rangeEnd The last number to insert as a key.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    void insertNumericRangeAsStringKeys(M<K,V>& map,
                                        const long long int& rangeStart,
                                        const long long int& rangeEnd)
    {
        for (long long int i = rangeStart; i <= rangeEnd; ++i) {
            map[std::to_string(i)];
        }
    }

    /*** String Key Operations ***/

    /**
     * @brief Helper to filter pairs based on a string key predicate.
     *
     * @param map The maplike object to filter.
     * @param predicate Function returning true for pairs to include.
     * @return Filtered map.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V,
              typename... Args, typename Predicate>
    M<K,V> filterPairsByKeyPredicate(const M<K,V>& map, Predicate predicate)
    {
        M<K,V> result;
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
     * @param prefix The prefix string to match.
     * @return A map containing only pairs with keys starting with prefix.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> getPairsWhereKeysStartWith(const M<K,V>& map, const K& prefix)
    {
        return filterPairsByKeyPredicate(map,
            [&prefix](const K& key) { return key.starts_with(prefix); });
    }

    /**
     * @brief Returns a map with pairs whose keys start with a given string removed.
     *
     * @param map The maplike object to filter.
     * @param prefix The prefix string to match for removal.
     * @return A map with matching pairs removed.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> erasePairsWhereKeysStartWith(const M<K,V> map, const K& prefix)
    {
        return filterPairsByKeyPredicate(map,
            [&prefix](const K& key) { return !key.starts_with(prefix); });
    }

    /**
     * @brief Erases all instances of a substring from all keys in the map.
     *
     * @param map The maplike object with string keys to modify.
     * @param substring The substring to erase from all keys.
     * @return A map with the substring erased from all keys.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> eraseStringFromKeys(const M<K,V>& map, const K& substring)
    {
        M<K,V> result;
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
     * @brief Creates a unique key string for the map.
     *
     * @param map The maplike object for which to create a unique key.
     * @param keyString The base key string to make unique.
     * @param algorithm The method to use for making the key unique.
     * @return A unique key string not present in the map.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K createUniqueKeyString(const M<K,V>& map, K keyString = "",
                           const std::string& algorithm = "integer concatenation")
    {
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

#endif
