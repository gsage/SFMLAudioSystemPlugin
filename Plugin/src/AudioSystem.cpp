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
  {
  }

  AsyncPlayer::~AsyncPlayer()
  {
    deleteThread();
  }

  void AsyncPlayer::play(const std::string& fileName, AsyncPlayer::PlayType type) 
  {
    deleteThread();
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
    while (music.getStatus() == sf::Music::Playing && !mStop)
    {
      // Leave some CPU time for other processes
      sf::sleep(sf::milliseconds(100));
    }
  }

  void AsyncPlayer::playSound(const std::string& fileName)
  {
    sf::SoundBuffer buffer;
    sf::Sound sound(buffer);
    if (!buffer.loadFromFile(fileName.c_str()))
      return;

    // Create a sound instance and play it
    sound.play();
    while (sound.getStatus() == sf::Sound::Playing && !mStop)
    {
      // Leave some CPU time for other processes
      sf::sleep(sf::milliseconds(100));
      LOG(INFO) << "Playing";
    }
  }

  void AsyncPlayer::stop()
  {
    mStop = true;
  }

  void AsyncPlayer::deleteThread()
  {
    stop();
    if(mThread) {
      mThread->join();
      delete mThread;
    }
  }

  AudioSystem::AudioSystem()
  {
  }

  AudioSystem::~AudioSystem()
  {
  }

  void AudioSystem::updateComponent(AudioComponent* component, Entity* entity, const double& time)
  {
    // nothing
  }

  void AudioSystem::playSound(const std::string& fileName)
  {
    mPlayers.push_back(AsyncPlayer());
    mPlayers.back().play(fileName, AsyncPlayer::SOUND);
  }

  void AudioSystem::playMusic(const std::string& fileName)
  {
    mPlayers.push_back(AsyncPlayer());
    mPlayers.back().play(fileName, AsyncPlayer::MUSIC);
  }

}
