#include <exception>
#include <string>
#include <format>

namespace dicomcodecs
{
    /// <summary>
    /// dicomcodec exception clas
    /// </summary>
    class exception : public std::exception
    {
        public:
            exception(const char* codec, const char* reason) : codec_(codec), reason_(reason) 
            {
                what_.resize(strlen(codec) + strlen(reason) + 4);
                sprintf((char*)what_.c_str(), "%s %s", codec, reason);
            }

            virtual ~exception() throw() {}

            virtual const char * what () const throw ()
            {
                return what_.c_str();
            }

            const char* codec() const throw () {
                return codec_;
            }

            const char* reason() const throw () {
                return reason_;
            }

        private:
            std::string what_;
            const char* codec_;
            const char* reason_;
        };
}
