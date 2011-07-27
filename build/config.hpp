/**
 * File providing compile-time data about runtime-configuration.
 */

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

/* #undef DEBUG */

#define __WDATA_DATA_DIR__ "/home/jacky/.wintermute"
#include <string>

using namespace std;

using std::string;

namespace Wintermute {
    namespace Data {
        class Configuration {
        public:
            static  const string getDirectory(){
                return __WDATA_DATA_DIR__;
            }

            static void Initialize();
            static void Deinitialize();
        };
    }
}

#endif // __CONFIG_HPP__

