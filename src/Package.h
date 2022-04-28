#pragma once
#include <string>
#include <vector>

struct Package {

public:

    void setData(unsigned long packageTimeStamp, uint8_t packageID, std::vector<std::string> data) {
        this->packageTimeStamp = packageTimeStamp;
        this->packageID = packageID;
        this->data = data;
        this->isNull = false;
    }

    unsigned long packageTimeStamp;
    uint8_t packageID;
    std::vector<std::string> data;
    bool isNull = true;

private:

    //PackageType

};


static Package parseString(std::string mystring, std::string delimiter) {

    Package pkg;

    std::vector<std::string> push;

    size_t pos = 0;
    std::string token;
    while ((pos = mystring.find(delimiter)) != std::string::npos) {
        token = mystring.substr(0, pos);
        push.push_back(token);
        mystring.erase(0, pos + delimiter.length());
    }

    if (push.size() < 2) {
        return pkg;
    }

    unsigned long packageTimeStamp = std::stoul(push[0], nullptr, 0);
    uint8_t packageID = (uint8_t)std::stoi(push[1], nullptr, 0);

    push.erase(push.begin() + 1);
    push.erase(push.begin() + 0);

    pkg.setData(packageTimeStamp, packageID, push);

    return pkg;

}

static std::vector<std::string> parseStrings(std::string mystring, std::string delimiter) {

    std::vector<std::string> push;

    size_t pos = 0;
    std::string token;
    while ((pos = mystring.find(delimiter)) != std::string::npos) {
        token = mystring.substr(0, pos);
        push.push_back(token);
        mystring.erase(0, pos + delimiter.length());
    }

    return push;

}