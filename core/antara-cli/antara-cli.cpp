#include <iostream>
#include "cxxopts.hpp"
#include "libusb.h"

int main(int argc, char *argv[])
{

    cxxopts::Options options("BUMANTARA-SDK:ANTARA", "ANTARA Command-Line Tools");

    options.add_options()("h,help", "Help page")("i,ID", " Set device ID (4 values)", cxxopts::value<std::vector<int>>())("s,scan", "Scan for device connected")("r,read", "Receive messages", cxxopts::value<std::vector<int>>())("w,write", "Send message", cxxopts::value<std::vector<int>>());

    std::vector<std::uint8_t> ID;

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
            std::vector<int> tempID = result["ID"].as<std::vector<int>>();

            if (tempID.size() > 4)
            {
                std::cerr << "Invalid ID (4 values)\n";
                return 1;
            }

            for (auto &&val : tempID)
            {
                if ((val < 0) || (val > 255))
                {
                    std::cerr << "Invalid ID value must be 0 - 255 per ID\n";
                    return 1;
                }
            }

            for (auto &&val : tempID)
            {
                ID.push_back(static_cast<std::uint8_t>(val));
            }

            std::cout << "Group     ID : " << std::to_string(ID[0]) << "\nSystem    ID : " << std::to_string(ID[1]) << "\nNode      ID : " << std::to_string(ID[2]) << "\nComponent ID : " << std::to_string(ID[3]) << "\n";
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