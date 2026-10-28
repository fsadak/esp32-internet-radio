#include "StationFavorites.h"
#include <Preferences.h>
#include <cassert>

int main() {
    Preferences::storage.clear();
    Preferences::failAfter = -1;
    const RadioStation catalog[] = {
        {"Türkçe İstasyon", "https://example.com/radio-a"},
        {"Second station", "https://example.com/radio-b"}
    };
    const RadioStation reordered[] = {catalog[1], {"Yeni ad", catalog[0].url}};
    String error;
    StationFavorites favorites;
    assert(!favorites.load());
    assert(favorites.mask(catalog, 2) == StationFavoriteMask(0));
    assert(favorites.toggle(catalog, 2, 0, error));
    StationFavorites reopened;
    assert(reopened.load());
    assert(reopened.mask(catalog, 2) == StationFavoriteMask(1));
    assert(reopened.mask(reordered, 2) == StationFavoriteMask(2)); // URL identity survives rename/reorder.
    Preferences::failAfter = 0;
    assert(!reopened.toggle(catalog, 2, 0, error));
    assert(reopened.mask(catalog, 2) == StationFavoriteMask(1)); // Failed write leaves RAM unchanged.
    StationFavorites afterFailure;
    assert(afterFailure.load() && afterFailure.mask(catalog, 2) == StationFavoriteMask(1));
    Preferences::failAfter = -1;
    assert(reopened.toggle(catalog, 2, 0, error));
    StationFavorites empty;
    assert(empty.load() && empty.mask(catalog, 2) == StationFavoriteMask(0)); // Empty list is persistent.
    assert(empty.toggle(catalog, 2, 0, error));
    assert(empty.toggle(catalog, 2, 1, error));
    const RadioStation other[] = {catalog[1]};
    assert(empty.mask(other, 1) == StationFavoriteMask(1));
    assert(empty.toggle(other, 1, 0, error)); // Explicit edit prunes absent URLs.
    assert(!empty.contains(catalog[0].url) && !empty.contains(catalog[1].url));
    assert(!empty.toggle(catalog, 2, 2, error));
    // Exercise both sides of the former 32-bit boundary and the last entry.
    std::vector<StationRecord> records;
    for (size_t i = 0; i < MAX_STATIONS; ++i)
        records.push_back({"Station " + std::to_string(i), "https://example.com/" + std::to_string(i)});
    std::vector<RadioStation> large;
    for (const auto& record : records) large.push_back({record.name.c_str(), record.url.c_str()});
    const size_t boundaries[] = {0, 31, 32, 63, 64, MAX_STATIONS - 1};
    for (size_t index : boundaries)
        assert(empty.toggle(large.data(), large.size(), index, error));
    const auto mask = empty.mask(large.data(), large.size());
    assert(mask.count() == 6);
    for (size_t index : boundaries) assert(mask.test(index));
    StationFavorites restored;
    assert(restored.load() && restored.mask(large.data(), large.size()) == mask);
    std::swap(large.front(), large.back());
    assert(restored.mask(large.data(), large.size()).test(0));
    assert(restored.toggle(large.data(), large.size(), 0, error));
    assert(!restored.mask(large.data(), large.size()).test(0));
    assert(restored.mask(large.data(), large.size()).test(MAX_STATIONS - 1));
}
