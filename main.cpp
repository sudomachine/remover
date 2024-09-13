#include <iostream>
#include <map>
#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>

/*
 * requirements
 *
 * boost 1.82 (https://archives.boost.io/release/1.82.0/source/boost_1_82_0.7z)
 * */

/* Configuration file struct (json)

{
    "paths": [
        {
            "path": "path_1",
            "interval": "interval_1"
        },
        {
            "path": "path_2",
            "interval": "interval_2"
        },
        {
            "path": "path_3",
            "interval": "interval_3"
        }
    ],
    "remove_interval": "interval",
    "scan_interval": "interval"
}

 */

/**
 * @brief The file system Superviser class for removing files and directories
 */
class Superviser {

public:
    Superviser(): 
    remove_interval_(0), scan_interval_(0), paths_({}) {
    }

    Superviser(const std::string& config): Superviser() {
        readConfig(config);
        
    }

    bool readConfig(const std::string& config_file)
    {
        // Read json config file if there is exist and valid
        boost::property_tree::ptree config_js;
        try {
            boost::property_tree::read_json(config_file, config_js);
        }
        catch (const std::exception& exc) {
            std::cerr << "Such json file does not exist or it is invalid! (" << config_file << ")" << std::endl;
            std::cerr << exc.what() << std::endl;
            return false;
        }

        // Check fields of the config file (config file must contain all this tags)
        bool validJson = true;
        validJson = tagInJson("remove_interval", config_js) * validJson;
        validJson = tagInJson("scan_interval", config_js) * validJson;
        validJson = tagInJson("paths", config_js) * validJson;
        for (boost::property_tree::ptree::value_type& v : config_js.get_child("paths"))
        {
            auto pt = v.second;
            validJson = tagInJson("path", pt) * validJson;
            validJson = tagInJson("interval", pt) * validJson;
        }
        if (!validJson)
            return false;

        // Read interval time values if there are valid
        try {
            // Save value to the Remover object
            remove_interval_ = config_js.get<uint32_t>("remove_interval");
        }
        catch (const std::exception& exc) {
            std::cerr << "The 'remove_interval' field has an invalid value (must be positive integer)!" << std::endl;
            return false;
        }

        try {
            // Save value to the Remover object
            scan_interval_ = config_js.get<uint32_t>("scan_interval");
        }
        catch (const std::exception& exc) {
            std::cerr << "The 'scan_interval' field has an invalid value (must be positive integer)!" << std::endl;
            return false;
        }

        // Read paths and intervals time values if there are valid
        for (boost::property_tree::ptree::value_type & v : config_js.get_child("paths"))
        {
            std::string path{""};
            uint32_t interval{ 0 };

            // read and validate each path
            path = v.second.get<std::string>("path");
            if (!boost::filesystem::exists(boost::filesystem::path(path)))
            {
                std::cerr << "The 'path' field has an invalid value (such path does not exist)!" << std::endl;
                return false;
            }

            // read and validate each interval life time for each path
            try {
                interval = v.second.get<uint32_t>("interval");
            }
            catch (const std::exception& exc) {
                std::cerr << "The 'interval' field has an invalid value (must be positive integer)!" << std::endl;
                return false;
            }

            // Save values to the Remover object
            paths_.emplace(std::make_pair(path, interval));
        }

        return true;
    }

    inline bool tagInJson(const std::string& tag, boost::property_tree::ptree& json, bool printCerr = true) 
    {
        if (json.count(tag) == 0)
        {
            if (printCerr)
                std::cerr << "Json file does not contain the '" << tag << "' tag!" << std::endl;
            return false;
        }

        return true;
    };

    void remove() {}

    void scan() {}

private:

    /**
     * @brief List of paths to removing of file or directory and there ages.
     * The first value is path to file/directory.
     * The second value is elapsed time since modifying of file (in days).
     */
    std::map<std::string, uint32_t> paths_;

    /**
     * @brief The period of the removing function call (in milliseconds).
     * The removing starts after the program starts and then after the time interval.
     */
    uint32_t remove_interval_;

    /**
     * @brief The period of the scanning function call (in seconds).
     * The scanning starts after the program starts and then after the time interval.
     */
    uint32_t scan_interval_;

    // config_read() function
    // Считывание и обновление всех параметров из файла конфигурации должно происходить при старте программы и перед каждым сканированием

private:

};

int main(int argc, char** argv)
{
    std::cout << "run program" << std::endl;

    std::cout << "Compiler version: " << _MSC_VER << std::endl;
    
    Superviser remover("C:\\users\\s-u-d\\Work\\remover\\config.json");

    return 0;
}
