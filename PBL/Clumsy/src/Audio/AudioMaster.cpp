
#include "../pch.h"

#include "AudioMaster.h"

namespace Clumsy
{
    AudioMaster* AudioMaster::GetInstance()
    {
        if (!m_Instance)
            m_Instance = new AudioMaster;
        return m_Instance;
    }

    void AudioMaster::PlayAmbientMusic()
    {
        m_SoundEngine->play2D("../Clumsy/src/Audio/forest.mp3", true);
    }

    void AudioMaster::PlayBell()
    {
        m_SoundEngine->play2D("../Clumsy/src/Audio/bell.wav", false);
    }

    void AudioMaster::PlayWalk()
    {
        m_SoundEngine->play2D("../Clumsy/src/Audio/walkShort.wav", false);
    }

	void AudioMaster::PlayChainsaw() 
	{
		m_SoundEngine->play2D("../Clumsy/src/Audio/chwdp.wav", false);
	}

	void AudioMaster::PlayShout()
	{
		m_SoundEngine->play2D("../Clumsy/src/Audio/shout.wav", false);
	}

	void AudioMaster::PlayFire()
	{
		m_SoundEngine->play2D("../Clumsy/src/Audio/fire.wav", false);
	}

    void AudioMaster::Drop()
    {
        m_SoundEngine->drop();
    }

    AudioMaster::AudioMaster()
    {
        m_SoundEngine = irrklang::createIrrKlangDevice();
    }

	void AudioMaster::HandleEvent(Event* event)
	{
        if (event->GetEventId() == "move")
        {
            PlayWalk();
        }
		
	}
}
