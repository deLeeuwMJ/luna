#include "map_loader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

void MapLoader::loadBeatMap(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open beat map file");
    }

    nlohmann::json jsonData;
    file >> jsonData;

    for (const auto& item : jsonData) {
        if (item.contains("beat_trigger")) {
            double triggerTime = item["beat_trigger"].get<double>();
            beatList.push_back(triggerTime);
        }
    }
}

std::vector<double> MapLoader::beats() const {
    return beatList;
}
