#include "Modules/AudioManager.hpp"
#include "Components/Transform.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {

    if (instance != nullptr) { throw std::runtime_error("FFS"); }

    instance = this;
}

AudioManager* AudioManager::GetInstance() {
    return instance;
}

void AudioManager::Init() {

    if (ma_engine_init(nullptr, &engine) != ma_result::MA_SUCCESS) {
        std::cerr << "Failed to initialize the audio engine. " << std::endl;
        abort();  // Erreur critique
    }

    AddAudio("mario64", "Assets/Sounds/Dire_Dire_Docks.mp3", AudioManager::LoadMethod::Asynchrone, AudioManager::ReadMethod::Preloading);

}

void AudioManager::Update() {
    UpdatePositionAudio();

    // Code temporaire
    //std::cout << "Entrez : ";
    //std::cin >> test;

    //if (test == "pause") {
    //    AudioPause("mario64");
    //}
    //else if (test == "start") {
    //    AudioStart("mario64");
    //    //test3D();
    //    AudioSetFadeIn("mario64", 10000);
    //}
    //else if (test == "z") {
    //    AudioStruct* currentAudio = nullptr;
    //    if (AudioBrowser("mario64", currentAudio)) {
    //        ma_sound_set_position(currentAudio->sound, 0, 1, 0);
    //    }
    //}
    //else if (test == "q") {
    //    AudioStruct* currentAudio = nullptr;
    //    if (AudioBrowser("mario64", currentAudio)) {
    //        ma_sound_set_position(currentAudio->sound, -1, 0, 0);
    //    }
    //}
    //else if (test == "s") {
    //    AudioStruct* currentAudio = nullptr;
    //    if (AudioBrowser("mario64", currentAudio)) {
    //        ma_sound_set_position(currentAudio->sound, 0, -1, 0);
    //    }
    //}
    //else if (test == "d") {
    //    AudioStruct* currentAudio = nullptr;
    //    if (AudioBrowser("mario64", currentAudio)) {
    //        ma_sound_set_position(currentAudio->sound, 1, 0, 0);
    //    }
    //}
}


void AudioManager::AddAudio(const std::string& name, const char* link) {

    AudioStruct* nameCheck = nullptr;
    if (instance->AudioBrowser(name, nameCheck)) {
        std::cerr << "Audioalready exist" << std::endl;
        return;
    }

    delete nameCheck; // plus besoin

    ma_sound* sound = new ma_sound();
    ma_result result = ma_sound_init_from_file(&instance->engine, link, 0, nullptr, nullptr, sound);
    if (result != ma_result::MA_SUCCESS) {
        std::cerr << "Incorrect path : " << std::endl;
        return;
    }

    ma_sound_set_volume(sound, 0.5f);

    AudioStruct currentAudio{
        .name = name,
        .sound = sound,
    };

    instance->audioList.push_back(currentAudio);
}
void AudioManager::AddAudio(const std::string& name, const char* link, AudioManager::LoadMethod loadMethod, AudioManager::ReadMethod readMethod) {

    AudioStruct* audio = nullptr;
    if (instance->AudioBrowser(name, audio)) {
        std::cerr << "Audio already exist" << std::endl;
        return;
    }

    delete audio; // plus besoin



    ma_sound* sound = new ma_sound;
    ma_result result = ma_sound_init_from_file(
        &instance->engine, link,
        (loadMethod == AudioManager::LoadMethod::Asynchrone ? ma_sound_flags::MA_SOUND_FLAG_ASYNC : ma_sound_flags::MA_SOUND_FLAG_WAIT_INIT) |
        (readMethod == AudioManager::ReadMethod::Preloading ? ma_sound_flags::MA_SOUND_FLAG_DECODE : ma_sound_flags::MA_SOUND_FLAG_STREAM),
        nullptr, nullptr, sound
    );

    // JIC i fucked up ^^
    //if (loadMethod == AudioManager::LoadMethod::Asynchrone) {
    //    if (readMethod == AudioManager::ReadMethod::Preloading) {
    //        result = ma_sound_init_from_file(&engine, link, ma_sound_flags::MA_SOUND_FLAG_ASYNC | ma_sound_flags::MA_SOUND_FLAG_DECODE, nullptr, nullptr, sound);
    //    }
    //    else {
    //        result = ma_sound_init_from_file(&engine, link, ma_sound_flags::MA_SOUND_FLAG_ASYNC, nullptr, nullptr, sound);
    //    }
    //}
    //else if (readMethod == AudioManager::ReadMethod::Preloading) {
    //    result = ma_sound_init_from_file(&engine, link, ma_sound_flags::MA_SOUND_FLAG_DECODE, nullptr, nullptr,  sound);
    //}
    //else
    //    result = ma_sound_init_from_file(&engine, link, 0, nullptr, nullptr, sound);  // Basic method

    if (result != ma_result::MA_SUCCESS) {
        std::cerr << "Incorrect path : " << link << std::endl;
        return;
    }
    

    ma_sound_set_volume(sound, 0.5f);

    AudioStruct currentAudio{
        .name = name,
        .sound = sound,
    };

    instance->audioList.push_back(currentAudio);
}

bool AudioManager::AudioBrowser(const std::string& name, AudioStruct*& ptr) {
    for (auto& audio : audioList) {

        if (audio.name == name) {

            ptr = &audio;
            return true;
        }
    }

    return false;
}

void AudioManager::UpdatePositionAudio() { // Va update la position des sons

    for (auto& audio : audioList) {

        if (audio.x != nullptr) {

            ma_sound_set_position(audio.sound, *audio.x, *audio.y, *audio.z);
        }
    }
}

void AudioManager::SetEngineVolume(const float value) {
    _ = ma_engine_set_volume(&instance->engine, value);
}

void AudioManager::Shutdown() {

    std::cout << "Shutdown AudioManager" << std::endl;
    for (auto& audio : audioList) {
        delete audio.sound;
    }

    audioList.clear();
    std::cout << "Shut down AudioManager" << std::endl;
}

#pragma region Basic Method

void AudioManager::AudioStart(const std::string& name) {

    AudioStruct* currentaudio = nullptr;
    if (instance->AudioBrowser(name, currentaudio)) {

        _ = ma_sound_start(currentaudio->sound);
    }
    else {
        std::cerr << "Name not found in AudioStart" << std::endl;
    }
}

void AudioManager::AudioPause(const std::string& name) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {

        if (ma_sound_is_playing(currentAudio->sound)) {
            _ = ma_sound_stop(currentAudio->sound);
        }
        else {
            _ = ma_sound_start(currentAudio->sound);
        }
    }
}

void AudioManager::AudioRestart(const std::string& name) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {

        _ = ma_sound_seek_to_pcm_frame(currentAudio->sound, 0);
        _ = ma_sound_start(currentAudio->sound);
    }
}

void AudioManager::AudioSetVolume(const std::string& name, const float newValue) {
    if (newValue < 0.0f) {
        std::cerr << "Negative value in AudioSetVolume" << std::endl;
        return;
    }

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_volume(currentAudio->sound, newValue);
    }
}

#pragma endregion


#pragma region Advance Methode

void AudioManager::AudioSetLooping(const std::string& name, const bool newValue) {
    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_looping(currentAudio->sound, newValue);
    }
}

void AudioManager::AudioSetPanning(const std::string& name, float newPanningValue) {

    if (newPanningValue > 1.0f) { newPanningValue = 1.0f; }
    else if (newPanningValue < -1.0f ) { newPanningValue = -1.0; }

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_pan(currentAudio->sound, newPanningValue);
    }
}

void AudioManager::AudioSetPitch(const std::string& name, const float newPitchValue) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_pitch(currentAudio->sound, newPitchValue);
    }

}

void AudioManager::SetDopplerEffect(const std::string& name, const float newValue) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_doppler_factor(currentAudio->sound, newValue);
    }
}

void AudioManager::AudioSetPosition(const std::string& name, float* x, float* y, float* z) {
    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        currentAudio->x = x;
        currentAudio->y = y;
        currentAudio->z = z;
    }
}

void AudioManager::AudioSetPosition(const std::string& name, Transform* transform) {
    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        currentAudio->x = &transform->position.x;
        currentAudio->y = &transform->position.y;
        currentAudio->z = &transform->position.z;
    }
}

void AudioManager::AudioSetDirection(const std::string& name, float* forwardX, float* forwardY, float* forwardZ) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        currentAudio->forwardX = forwardX;
        currentAudio->forwardY = forwardY;
        currentAudio->forwardZ = forwardZ;
    }
}

void AudioManager::AudioPlayerSeekTo(const std::string& name, const int timeInMs) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_uint32 sampleRate; // we don t really use it but hey its there ^^
        _ = ma_sound_get_data_format(currentAudio->sound, nullptr, nullptr, &sampleRate, nullptr, 0);

        _ = ma_sound_seek_to_pcm_frame(currentAudio->sound, (unsigned long long)(timeInMs * sampleRate) / 1000);
    }
}

void AudioManager::test3D() {

    ma_engine_listener_set_position(&engine, 0, 0, 0, 0);

    AudioStruct* currentAudio = nullptr;
    if (AudioBrowser("mario64", currentAudio)) {
        ma_sound_set_positioning(currentAudio->sound, ma_positioning::ma_positioning_absolute);
        ma_sound_set_position(currentAudio->sound, 1, 0, 0);
        //ma_sound_set_direction(currentAudio->sound, 1, 0, 0);
    }
}

void AudioManager::AudioSetFadeIn(const std::string& name, float fadeInValue) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_fade_in_milliseconds(currentAudio->sound, 0, -1, (ma_uint64)fadeInValue);
    }
}

void AudioManager::AudioSetMinDistance(const std::string& name, const float minValue) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_min_distance(currentAudio->sound, minValue);
    }
}

void AudioManager::AudioSetMaxDistance(const std::string& name, const float maxValue) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_max_distance(currentAudio->sound, maxValue);
    }
}

void AudioManager::AudioSetVelocity(const std::string& name, float velocityX, float velocityY, float velocityZ) {

    AudioStruct* currentAudio = nullptr;
    if (instance->AudioBrowser(name, currentAudio)) {
        ma_sound_set_velocity(currentAudio->sound, velocityX, velocityY, velocityZ);
    }

}

#pragma endregion