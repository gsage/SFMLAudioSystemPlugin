/*
-----------------------------------------------------------------------------
This file is a part of Gsage engine

Copyright (c) 2014-2016 Artem Chernyshev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "AudioSystem.h"
#include "Logger.h"
#include <SFML/Audio.hpp>
#include <condition_variable>

namespace Gsage {

  AsyncPlayer::AsyncPlayer()
    : mThread(0)
    , mPlaying(false)
  {
  }

  AsyncPlayer::~AsyncPlayer()
  {
    stop();
  }

  void AsyncPlayer::play(const std::string& fileName, AsyncPlayer::PlayType type) 
  {
    stop();
    mStop = false;
    switch(type)
    {
      case MUSIC:
        // Load a music file
        mThread = new std::thread(&AsyncPlayer::playMusic, this, fileName);
        break;
      case SOUND:
        // Load a sound buffer from a wav file
        mThread = new std::thread(&AsyncPlayer::playSound, this, fileName);
        break;
    }
  }

  void AsyncPlayer::playMusic(const std::string& fileName)
  {
    sf::Music music;
    if (!music.openFromFile(fileName.c_str()))
      return;
    music.play();
    mPlaying = true;
    while (music.getStatus() == sf::Music::Playing && !mStop)
    {
      // Leave some CPU time for other processes
      sf::sleep(sf::milliseconds(100));
    }
    mPlaying = false;
  }

  void AsyncPlayer::playSound(const std::string& fileName)
  {
    sf::SoundBuffer buffer;
    sf::Sound sound(buffer);
    if (!buffer.loadFromFile(fileName.c_str()))
      return;

    // Create a sound instance and play it
    sound.play();
    mPlaying = true;
    while (sound.getStatus() == sf::Sound::Playing && !mStop)
    {
      // Leave some CPU time for other processes
      sf::sleep(sf::milliseconds(100));
    }
    mPlaying = false;
  }

  void AsyncPlayer::stop()
  {
    mStop = true;
    if(mThread) {
      mThread->join();
      delete mThread;
      mThread = 0;
    }
  }

  bool AsyncPlayer::isPlaying() const
  {
    return mPlaying;
  }

  AudioSystem::AudioSystem()
    : mMaxPlayersCount(256)
  {
    mPlayers.reserve(mMaxPlayersCount);
  }

  AudioSystem::~AudioSystem()
  {
    for(AsyncPlayers::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it){
      (*it).mStop = true;
    }
  }

  bool AudioSystem::initialize(const DataNode& settings)
  {
    EngineSystem::initialize(settings);
    mMaxPlayersCount = settings.get("playersCount", mMaxPlayersCount);
  }

  void AudioSystem::updateComponent(AudioComponent* component, Entity* entity, const double& time)
  {
    // nothing
  }

  int AudioSystem::playSound(const std::string& fileName)
  {
    return play(fileName, AsyncPlayer::SOUND);
  }

  int AudioSystem::playMusic(const std::string& fileName)
  {
    return play(fileName, AsyncPlayer::MUSIC);
  }

  bool AudioSystem::stopPlayer(int handle)
  {
    if(handle < 0 || handle >= mPlayers.size()){
      return false;
    }

    mPlayers[handle].stop();
    return true;
  }

  int AudioSystem::play(const std::string& fileName, AsyncPlayer::PlayType playType)
  {
    int index = -1;

    for(int i = 0; i < mPlayers.size(); i++) {
      if(!mPlayers[i].isPlaying()) {
        index = i;
      }
    }

    if(index == -1)
    {
      if(mPlayers.size() >= mMaxPlayersCount)
      {
        LOG(ERROR) << "Can't play sound " << fileName << ": max number of async sound players is reached";
        return -1;
      }

      mPlayers.push_back(AsyncPlayer());
      index = mPlayers.size() - 1;
    }

    mPlayers[index].play(fileName, playType);
    return index;
  }
}
