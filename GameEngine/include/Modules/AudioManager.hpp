#pragma once
#include <iostream>
#include <unordered_map>
#include <miniaudio/miniaudio.h>

#include "Module.hpp"
#include "InputManager.hpp"

struct AudioStruct {

	std::string name;
	float* x = nullptr;
	float* y = nullptr;
	float* z = nullptr;
	ma_sound* sound = nullptr;

	float* forwardX = nullptr;
	float* forwardY = nullptr;
	float* forwardZ = nullptr;
};

class AudioManager : public Module {
public:
	enum class LoadMethod {
		Synchrone,
		Asynchrone,
	};

	enum class ReadMethod {
		Streaming,
		Preloading,
	};

	AudioManager();

	static AudioManager* GetInstance();

	void Init() override;
	void Update() override;
	void Shutdown() override;

	/*
	*@brief Creates a sound and adds it to the sound manager
	*
	* @param name : name of the Audio
	* @param link : link to the audio file
	*/
	static void AddAudio(const std::string& name, const char* link);
	/*
	*@brief Creates a sound and adds it to the sound manager
	*
	* @param name : name of the Audio
	*
	* @param link : link to the audio file
	*
	* @param loadMethod : define the load methode (SYNCHRONE, ASYNCHRONE)
	*
	* @param readerMethode : defines how the audio file is played (STREAMING, PRELOADING)
	*
	*/
	static void AddAudio(const std::string& name, const char* link, AudioManager::LoadMethod loadMethod, AudioManager::ReadMethod readMethod);

	// Invidual Audio

	/*
	*@brief Start playing a sound
	*
	* @param name : name of the Audio
	*
	*/
	static void AudioStart(const std::string& name);
	/*
	*@brief Pause or restart a sound after pausing it
	*
	* @param name : name of the Audio
	*
	*/
	static void AudioPause(const std::string& name);
	/*
	*@brief Restart a sound from the beginning
	*
	* @param name : name of the Audio
	*
	*/
	static void AudioRestart(const std::string& name);
	/*
	*@brief Adjust the volume of a sound by multiplicative value, if the value is greater than 1 the sound will be amplified and 0 silences him
	*
	* @param name : name of the Audio
	*
	* @param newValue : the new multiplier
	*
	*/
	static void AudioSetVolume(const std::string& name, float newValue);
	/*
	*@brief With this parameter is activated, the sound will loop indefinitely
	*
	* @param name : name of the Audio
	*
	* @param newValue : pass to true or false
	*
	*/
	static void AudioSetLooping(const std::string& name, bool newValue);
	/*
	*@brief Setting the pan to 0 will result in an unpanned sound. Setting it to -1 will shift everything to the left, whereas +1 will shift it to the right
	*
	* @param name : name of the Audio
	*
	* @param newPanningValue : define the new Panning Value cap between -1 and 1 inclusive
	*
	*/
	static void AudioSetPanning(const std::string& name, float newPanningValue);
	/*
	*@brief A larger value will result in a higher pitch.The pitch must be greater than 0.
	*
	* @param name : name of the Audio
	*
	* @param newPitchValue : define the new pitch Value
	*
	*/
	static void AudioSetPitch(const std::string& name, float newPitchValue);
	static void SetDopplerEffect(const std::string& name, float newValue); // Non tester, neccesite de la velociter sur le son 
	/*
	*@brief Defines the coordinates to which the sound will be attached
	*
	* @param name : name of the Audio
	*
	* @param x : pointer to x
	*
	* @param y : pointe to y
	*
	* @param z : pointer to z
	*
	*/
	static void AudioSetPosition(const std::string& name, float* x, float* y, float* z);
	/*
	*@brief Defines the transform to which the sound will be attached
	*
	* @param name : name of the Audio
	*
	* @param transform : pointer to the object's transform
	*
	*/
	static void AudioSetPosition(const std::string& name, class Transform* transform);
	/*
	*@brief Defines the direction where the soudtrack will be attached
	*
	* @param name : name of the Audio
	*
	* @param forwardX : pointer to x
	*
	* @param forwardY : pointer to y
	*
	* @param forwardZ : pointer to z
	*
	*/
	static void AudioSetDirection(const std::string& name, float* forwardX, float* forwardY, float* forwardZ);
	/*
	*@brief Move playback position to timeInMs milliseconds
	*
	* @param name : name of the Audio
	*
	* @param timeInMs : value where the reader will be moved
	*
	*/
	static void AudioPlayerSeekTo(const std::string& name, int timeInMs);

	static void AudioSetFadeIn(const std::string& name, float fadeInValue);

	static void AudioSetMinDistance(const std::string& name, float minValue);

	static void AudioSetMaxDistance(const std::string& name, float maxValue);

	static void AudioSetVelocity(const std::string& name, float velocityX, float velocityY, float velocityZ);

	static void SetEngineVolume(float value);

	void test3D();


private:
	void UpdatePositionAudio();
	bool AudioBrowser(const std::string& name, AudioStruct*& ptr);



	std::vector<AudioStruct> audioList;
	//std::unordered_map<std::string, ma_sound_group*> groupMap;
	std::string test = "";  // pour l'input dans le terminal, a suprimer plus tard
	ma_engine engine{};
	ma_engine_config engineConfig{};


	static AudioManager* instance;
};

