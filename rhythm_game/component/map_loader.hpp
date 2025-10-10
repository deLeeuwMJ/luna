#pragma once

#include <string>
#include <vector>

class MapLoader
{
public:
    void loadBeatMap(const std::string& filePath);
    std::vector<double> beats() const;
private:
    std::vector<double> beatList;
};