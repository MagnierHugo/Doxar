#include "Modules/ScriptManager.hpp"

#include <filesystem>

#include "GameObject.hpp"
#include "Components/Script.hpp"
#include "Utilities/Logger.hpp"


static std::string wstringToString(const std::wstring& wstr) {
    // Get the size of the converted string
    size_t size_needed = std::wcstombs(nullptr, wstr.c_str(), 0) + 1;

    if (size_needed == static_cast<size_t>(-1)) {
        // Conversion failed (invalid wide character sequence)
        return "";
    }

    // Allocate buffer to hold the converted string
    std::string str(size_needed, '\0');

    // Perform conversion
    std::wcstombs(&str[0], wstr.c_str(), size_needed);
    return str;
}

//static bool CompileScript(const std::string& scriptPath, const std::string& outputPath) {
//    std::string compileCommand = "cl /LD " + scriptPath + " /link /out:" + outputPath;
//
//    // Call the system compiler
//    int result = std::system(compileCommand.c_str());
//
//    return result == 0;
//}


ScriptManager* ScriptManager::instance = nullptr;


void ScriptManager::Init() {
    //MonitorScriptChanges();
    LoadLibraries();
}

void ScriptManager::Update() {
    
}

void ScriptManager::Shutdown() {
    UnloadLibraries();
}

//// Global atomic variable to indicate the need for directory monitoring
//std::atomic<bool> stopMonitoring(false);
//
//void ScriptManager::MonitorScriptChanges() {
//    static HANDLE hDir = INVALID_HANDLE_VALUE;
//    static BYTE buffer[1024];
//    static DWORD bytesReturned;
//
//    // Open directory handle for monitoring
//    if (hDir == INVALID_HANDLE_VALUE) {
//        hDir = CreateFile(
//            L"Assets\\Scripts",                // directory path
//            FILE_LIST_DIRECTORY,               // desired access
//            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // share mode
//            NULL,                              // security attributes
//            OPEN_EXISTING,                     // creation disposition
//            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // flags
//            NULL                               // template file
//        );
//
//        if (hDir == INVALID_HANDLE_VALUE) {
//            LOG_ERROR("Error opening directory: Assets\\Scripts / Error Code: {}", GetLastError());
//            return;
//        }
//    }
//
//    // Start monitoring in a separate thread to make it non-blocking
//    std::thread monitorThread([this]() {
//        char buffer[1024];  // Buffer to store directory change information
//        DWORD bytesReturned = 0;
//
//        while (!stopMonitoring.load()) {
//            if (ReadDirectoryChangesW(
//                hDir,                // directory handle
//                &buffer,             // buffer to store data
//                sizeof(buffer),      // buffer size
//                TRUE,                // watch subdirectories
//                FILE_NOTIFY_CHANGE_FILE_NAME |
//                FILE_NOTIFY_CHANGE_DIR_NAME |
//                FILE_NOTIFY_CHANGE_ATTRIBUTES |
//                FILE_NOTIFY_CHANGE_SIZE |
//                FILE_NOTIFY_CHANGE_LAST_WRITE, // filter
//                &bytesReturned,      // number of bytes returned
//                NULL,                // overlapped structure (not using here)
//                NULL                 // completion routine (not using here)
//            )) {
//                FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)buffer;
//                std::wstring modifiedFile = L"";
//
//                do {
//                    // Check for modified files
//                    if (pNotify->Action == FILE_ACTION_MODIFIED ||
//                        pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME ||
//                        pNotify->Action == FILE_ACTION_ADDED) {
//                        modifiedFile = std::wstring(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));
//                    }
//
//                    // Move to the next notification in the buffer (if any)
//                    if (pNotify->NextEntryOffset == 0)
//                        break;
//                    pNotify = (FILE_NOTIFY_INFORMATION*)((LPBYTE)pNotify + pNotify->NextEntryOffset);
//                } while (true);
//
//                if (!modifiedFile.empty()) {
//                    const std::string file = wstringToString(modifiedFile);
//                    const std::string out = "DLLs\\" + file.substr(0, file.find_last_of('.')) + ".dll";
//                    if (!CompileScript(file, out)) {
//                        LOG_ERROR("Failed to Compile Library: {}", file);
//                        return;
//                    }
//                    LOG_DEBUG("Successfully Compiled Script: {}", file);
//                    LoadScriptLibrary(file);
//                }
//            }
//        }
//
//        });
//
//    monitorThread.detach();  // Detach the thread so it runs independently
//}
//
//// To stop the monitoring (e.g., when shutting down)
//void ScriptManager::StopMonitoringDirectory() {
//    stopMonitoring.store(true);  // Set the flag to stop monitoring
//}

void ScriptManager::LoadScript(GameObject* gameObject, const std::string& scriptName) {
    const std::string libraryPath = "DLLs\\" + scriptName + ".dll";

    if (!instance->libraryHandles.contains(libraryPath)) {
        LOG_ERROR("Script Loading Failed! Library Not Found: {}", libraryPath);
        return; 
    }

    HMODULE& currentLibraryHandle = instance->libraryHandles[libraryPath];

    // Find the "CreateScript" function from the DLL
    typedef Script* (*CreateScriptFn)();
    CreateScriptFn createScript = (CreateScriptFn)GetProcAddress(currentLibraryHandle, "CreateScript");

    if (createScript == nullptr) {
        LOG_ERROR("Error finding CreateScript function");
        FreeLibrary(currentLibraryHandle);
        return;
    }

    // Create the script instance

    gameObject->AddComponent<Script>(createScript());
    // basically does (Script)Script
}

//void ScriptManager::UnloadScriptLibrary(const std::string& libraryPath) {
//    HMODULE& currentLibraryHandle = instance->libraryHandles[libraryPath];
//    if (currentLibraryHandle != nullptr) {
//        // Unload the existing library (DLL)
//        FreeLibrary(currentLibraryHandle);
//        currentLibraryHandle = nullptr;
//    }
//}

//void ScriptManager::LoadScriptLibrary(const std::string& libraryPath) {
//
//    // Unload the previous library (if any)
//    if (instance->libraryHandles.contains(libraryPath)) { UnloadScriptLibrary(libraryPath); }
//
//    std::wstring ws(libraryPath.size(), L' ');
//    ws.resize(std::mbstowcs(&ws[0], libraryPath.c_str(), libraryPath.size()));
//
//    // Load the new DLL
//    HMODULE currentLibraryHandle = LoadLibrary(ws.c_str());
//    if (!currentLibraryHandle) {
//        LOG_ERROR("Error loading DLL: {}", GetLastError());
//        return;
//    }
//}

void ScriptManager::LoadLibraries() {
    if (!std::filesystem::exists("DLLs")) {
        LOG_INFO("The DLLs directory doesn't exist");
        return;
    }

    for (const auto& dir_entry : std::filesystem::directory_iterator{ "DLLs" }) {

        std::string path = dir_entry.path().generic_string();
        if (!path.ends_with(".dll")) { continue; }

        HMODULE handle = LoadLibrary(dir_entry.path().c_str());
        if (handle == nullptr) {
            LOG_ERROR("Error loading DLL: {}", path);
        }

        instance->libraryHandles.insert({ path, handle });
        LOG_INFO("Successfuly loaded DLL: {}", path);
    }
}

void ScriptManager::UnloadLibraries() {

    for (auto& [path, handle] : instance->libraryHandles) {
        if (handle != nullptr) {
            FreeLibrary(handle);
            handle = nullptr;
        }
    }
}