#ifndef CORE_SYSTEM_COMPAT_H_
#define CORE_SYSTEM_COMPAT_H_
    #ifdef WIN32
    // Code to specifically avoid having to write reams of code each time we want to check the win32
    // last error message.

    #include <windows.h>
    #include <string>

    inline ::std::string GetLastErrorAsString()
    {
        //Get the error message, if any.
        DWORD errorMessageID = ::GetLastError();
        if(errorMessageID == 0)
            return "No error message has been recorded";

        LPSTR messageBuffer = NULL;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);

        return message;
    }
    #endif // WIN32
#endif
