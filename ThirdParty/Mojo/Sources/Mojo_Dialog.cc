#include <Mojo/Dialog.h>
#include "Mojo_Window.h"

#include <assert.h>
#include <shlobj.h>
#include <commdlg.h>

inline namespace Mojo
{
    bool MessageBox::Show(const char* title, const char* message)
    {
        return MessageBox::Show(title, message, MessageBoxType::Ok);
    }

    bool MessageBox::Show(const char* title, const char* message, MessageBoxType type)
    {
        UINT uType = 0;
        switch (type)
        {
        case MessageBoxType::Ok:
            uType = MB_OK;
            break;

        case MessageBoxType::OkCancel:
            uType = MB_OKCANCEL;
            break;

        case MessageBoxType::YesNo:
            uType = MB_YESNO;
            break;

        case MessageBoxType::YesNoCancel:
            uType = MB_YESNOCANCEL;
            break;

        default:
            assert(false && "Invalid message box type.");
            break;
        }

        return ::MessageBoxA(Window::_mainWindow, message, title, uType) == 1;
    }

    const char* OpenFileDialog::Show(const char* title, const char* filter)
    {
        return OpenFileDialog::Show(title, filter, 1, "");
    }

    const char* OpenFileDialog::Show(const char* title, const char* filter, int filterIndex)
    {
        return OpenFileDialog::Show(title, filter, filterIndex, "");
    }

    const char* OpenFileDialog::Show(const char* title, const char* filter, const char* path)
    {
        return OpenFileDialog::Show(title, filter, 1, path);
    }

    const char* OpenFileDialog::Show(const char* title, const char* filter, int filterIndex, const char* path)
    {
        OPENFILENAMEA ofn = {};         
        thread_local char result[1024] = "";

        HWND hwnd = Window::_mainWindow;

        // Initialize OPENFILENAME
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = hwnd;
        ofn.lpstrFile       = result;
        ofn.nMaxFile        = sizeof(result);
        ofn.lpstrFilter     = filter;
        ofn.nFilterIndex    = filterIndex;
        ofn.lpstrTitle      = title;
        ofn.lpstrInitialDir = path;
        ofn.lpstrFileTitle  = NULL;
        ofn.nMaxFileTitle   = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Display the Open dialog box. 
        if (::GetOpenFileNameA(&ofn))
        {
            return result;
        }
        else
        {
            return "";
        }
    }

    const char* SaveFileDialog::Show(const char* title, const char* filter)
    {
        return SaveFileDialog::Show(title, filter, 1, "");
    }

    const char* SaveFileDialog::Show(const char* title, const char* filter, int filterIndex)
    {
        return SaveFileDialog::Show(title, filter, filterIndex, "");
    }

    const char* SaveFileDialog::Show(const char* title, const char* filter, const char* path)
    {
        return SaveFileDialog::Show(title, filter, 1, path);
    }

    const char* SaveFileDialog::Show(const char* title, const char* filter, int filterIndex, const char* path)
    {
        OPENFILENAMEA ofn = {};         
        thread_local char result[1024] = "";

        HWND hwnd = Window::_mainWindow;

        // Initialize OPENFILENAME
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = hwnd;
        ofn.lpstrFile       = result;
        ofn.nMaxFile        = sizeof(result);
        ofn.lpstrFilter     = filter;
        ofn.nFilterIndex    = filterIndex;
        ofn.lpstrTitle      = title;
        ofn.lpstrInitialDir = path;
        ofn.lpstrFileTitle  = NULL;
        ofn.nMaxFileTitle   = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags           = OFN_OVERWRITEPROMPT;

        // Display the Open dialog box. 
        if (::GetSaveFileNameA(&ofn))
        {
            return result;
        }
        else
        {
            return "";
        }
    }

    const char* BrowseFolderDialog::Show(const char* title)
    {
        thread_local char path[MAX_PATH] = "";

        BROWSEINFOA bi = { 0 };
        bi.lpszTitle = title;
        bi.hwndOwner = Window::_mainWindow;

        LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
        if (pidl)
        {
            // get the name of the folder and put it in path
            SHGetPathFromIDListA(pidl, path);
        }
        else
        {
            path[0] = 0;
        }

        return path;
    }
}