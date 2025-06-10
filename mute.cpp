#include <windows.h>
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "user32.lib")

void ToggleMicrophoneMute() {
    CoInitialize(nullptr);

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pVolume = nullptr;

    // Create device enumerator
    if (CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pEnumerator)) == S_OK) {
        // Get default audio input device (microphone)
        if (pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice) == S_OK) {
            // Activate the audio endpoint volume interface
            if (pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&pVolume)) == S_OK) {
                BOOL isMuted = FALSE;
                pVolume->GetMute(&isMuted);
                pVolume->SetMute(!isMuted, nullptr);

                // Get new mute status
                BOOL newMute = FALSE;
                pVolume->GetMute(&newMute);
                std::cout << "Microphone " << (newMute ? "muted" : "unmuted") << std::endl;

                pVolume->Release();
            }
            pDevice->Release();
        }
        pEnumerator->Release();
    }

    CoUninitialize();
}

int main() {
    // Register the hotkey Ctrl + Alt + Shift + Q
    if (!RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 0x51)) {
        std::cerr << "Failed to register hotkey." << std::endl;
        return 1;
    }

    std::cout << "Press Ctrl + Alt + Shift + Q to toggle microphone mute/unmute." << std::endl;

    // Message loop to listen for hotkey events
    MSG msg;
    while (true) {
        if (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                ToggleMicrophoneMute();
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
