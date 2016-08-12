#ifndef _AudioSystem_H_
#define _AudioSystem_H_
#
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

#include "ComponentStorage.h"
#include "AudioComponent.h"
#include <string>
#include <thread>

namespace Gsage {
  class AsyncPlayer
  {
    public:
      enum PlayType {
        MUSIC,
        SOUND
      };

      AsyncPlayer();
      virtual ~AsyncPlayer();
      void play(const std::string& fileName, PlayType type);

      void stop();
    private:

      void playSound(const std::string& fileName);
      void playMusic(const std::string& fileName);

      void deleteThread();
      bool mStop;
      std::thread* mThread;
  };

  class AudioSystem : public ComponentStorage<AudioComponent>
  {
    public:
      static const std::string SYSTEM;
      AudioSystem();
      virtual ~AudioSystem();
      /**
       * Update a single component
       */
      virtual void updateComponent(AudioComponent* component, Entity* entity, const double& time);

      void playSound(const std::string& filename);

      void playMusic(const std::string& filename);

    private:

      typedef std::vector<AsyncPlayer> AsyncPlayers;
      AsyncPlayers mPlayers;

  };
}

#endif
