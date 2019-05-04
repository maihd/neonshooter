#pragma once

inline namespace Mojo
{
    enum struct MessageBoxType
    {
        Ok,
        OkCancel,

        YesNo,
        YesNoCancel,

        Default = Ok,
    };

    struct MessageBox
    {
        //static bool Show(const char* message);
        //static bool Show(const char* message, MessageBoxType type);

        static bool Show(const char* title, const char* message);
        static bool Show(const char* title, const char* message, MessageBoxType type);
    };

    struct OpenFileDialog
    {
        //static const char* Show(const char* filter);
        //static const char* Show(const char* filter, int filterIndex);
        //static const char* Show(const char* filter, const char* path);
        //static const char* Show(const char* filter, int filterIndex, const char* path);

        static const char* Show(const char* title, const char* filter);
        static const char* Show(const char* title, const char* filter, int filterIndex);
        static const char* Show(const char* title, const char* filter, const char* path);
        static const char* Show(const char* title, const char* filter, int filterIndex, const char* path);
    };

    struct SaveFileDialog
    {
        //static const char* Show(const char* filter);
        //static const char* Show(const char* filter, int filterIndex);
        //static const char* Show(const char* filter, const char* path);
        //static const char* Show(const char* filter, int filterIndex, const char* path);

        static const char* Show(const char* title, const char* filter);
        static const char* Show(const char* title, const char* filter, int filterIndex);
        static const char* Show(const char* title, const char* filter, const char* path);
        static const char* Show(const char* title, const char* filter, int filterIndex, const char* path);
    };

    struct BrowseFolderDialog
    {
        //static const char* Show(void);
        static const char* Show(const char* title);
    };
}