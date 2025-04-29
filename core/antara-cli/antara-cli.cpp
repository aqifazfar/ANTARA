#include <iostream>
#include <vector>
#include <algorithm>
#include "cxxopts.hpp"

int main(int argc, char *argv[])
{

    cxxopts::Options options("BUMANTARA-SDK:ANTARA", "ANTARA Command-Line Tools");

    options.add_options()("h,help", "Help page")("s,scan", "Scan for device connected")("i,ID", "Set ID to connected device (4 values)", cxxopts::value<std::vector<int>>())("p,port", "Port where device is connected", cxxopts::value<int>())("r,read", "Receive messages", cxxopts::value<std::vector<int>>())("w,write", "Send message", cxxopts::value<std::vector<int>>());

    try
    {
        auto result{options.parse(argc, argv)};

        if (result.count("help"))
        {
            std::cout << options.help() << "\n";
            return 0;
        }

        if (result.count("ID"))
        {

            std::vector<std::uint8_t> ID;

            std::vector<int> tempID = result["ID"].as<std::vector<int>>();

            if ((tempID.size() > 4) || (tempID.size() < 4))
            {
                std::cerr << "Invalid ID (4 values)\n";
                return 1;
            }

            if (std::any_of(tempID.begin(), tempID.end(), [](int i)
                            { return ((i < 0) || (i > 255)); }))
            {
                std::cerr << "Invalid ID (range: 0 - 255) for each ID\n";
                return 1;
            }

            for (auto &&val : tempID)
            {
                ID.push_back(static_cast<std::uint8_t>(val));
            }

            std::cout << "Group     ID : " << std::to_string(ID[0]) << "\nSystem    ID : " << std::to_string(ID[1]);
            return 0;
        }

        if (result.count("scan"))
        {
        }
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error pasing options: " << e.what() << "\n";
        return 1;
    }

    return 0;
}