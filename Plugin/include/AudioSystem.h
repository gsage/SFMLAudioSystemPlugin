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
  class AudioSystem;
  /**
   * AsyncPlayer plays sound or music in a separate thread
   */
  class AsyncPlayer
  {
    public:
      enum PlayType {
        MUSIC,
        SOUND
      };

      AsyncPlayer();
      virtual ~AsyncPlayer();
      /**
       * Play file
       *
       * @param fileName: path to the sound file
       * @param type: play type AsyncPlayer::PlayType SOUND or MUSIC
       *
       * Sound is loaded into memory, while music does not.
       */
      void play(const std::string& fileName, PlayType type);

      /**
       * Stop AsyncPlayer playback and remove current thread
       */
      void stop();

      /**
       * If thread is in the loop of playback this will return true
       */
      bool isPlaying() const;
    private:

      friend class AudioSystem;

      void playSound(const std::string& fileName);
      void playMusic(const std::string& fileName);

      bool mStop;
      bool mPlaying;
      std::thread* mThread;
  };

  class AudioSystem : public ComponentStorage<AudioComponent>
  {
    public:
      static const std::string SYSTEM;
      AudioSystem();
      virtual ~AudioSystem();

      /**
       * Initialize audio system
       * @param settings: Settings of the system
       */
      virtual bool initialize(const DataNode& settings);
      /**
       * Update a single component
       */
      virtual void updateComponent(AudioComponent* component, Entity* entity, const double& time);

      /**
       * Load the file into memory and play it
       *
       * @param filename: Sound file path
       * @returns: int handle of the player
       */
      int playSound(const std::string& filename);

      /**
       * Buffer file and play it.
       *
       * @param filename: Sound file path
       * @returns: int handle of the player
       */
      int playMusic(const std::string& filename);

      /**
       * Stops the player playback
       *
       * @param handle: int handle, returned from the playSound or playMusic methods
       */
      bool stopPlayer(int handle);

    private:

      int play(const std::string& fileName, AsyncPlayer::PlayType playType);

      typedef std::vector<AsyncPlayer> AsyncPlayers;
      AsyncPlayers mPlayers;

      int mMaxPlayersCount;

  };
}

#endif
