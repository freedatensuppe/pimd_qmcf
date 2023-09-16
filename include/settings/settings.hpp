#ifndef _SETTINGS_HPP_

#define _SETTINGS_HPP_

#include <string>        // for string, allocator
#include <string_view>   // for basic_string_view, string_view

namespace settings
{
    /**
     * @class Settings
     *
     * @brief Stores the general settings of the simulation
     *
     */
    class Settings
    {
      private:
        static inline std::string _jobtype;   // no default value

        static inline bool _isMMActivated            = false;
        static inline bool _isQMActivated            = false;
        static inline bool _isRingPolymerMDActivated = false;

      public:
        Settings()  = default;
        ~Settings() = default;

        static void activateMM() { _isMMActivated = true; }
        static void activateQM() { _isQMActivated = true; }
        static void activateRingPolymerMD() { _isRingPolymerMDActivated = true; }

        static void deactivateMM() { _isMMActivated = false; }
        static void deactivateQM() { _isQMActivated = false; }
        static void deactivateRingPolymerMD() { _isRingPolymerMDActivated = false; }

        /***************************
         * standard getter methods *
         ***************************/

        [[nodiscard]] static std::string getJobtype() { return _jobtype; }

        [[nodiscard]] static bool isMMActivated() { return _isMMActivated; }
        [[nodiscard]] static bool isQMActivated() { return _isQMActivated; }
        [[nodiscard]] static bool isRingPolymerMDActivated() { return _isRingPolymerMDActivated; }

        /***************************
         * standard setter methods *
         ***************************/

        static void setJobtype(const std::string_view jobtype) { _jobtype = jobtype; }

        static void setIsMMActivated(const bool isMM) { _isMMActivated = isMM; }
        static void setIsQMActivated(const bool isQM) { _isQMActivated = isQM; }
        static void setIsRingPolymerMDActivated(const bool isRingPolymerMD) { _isRingPolymerMDActivated = isRingPolymerMD; }
    };

}   // namespace settings

#endif   // _SETTINGS_HPP_