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

#include "AudioPlugin.h"
#include "AudioSystem.h"
#include "GsageFacade.h"
#include "Logger.h"
#include "lua/LuaInterface.h"
#include <luabind/luabind.hpp>

namespace Gsage {

  const std::string PLUGIN_NAME = "AudioPlugin";

  AudioPlugin::AudioPlugin()
  {
  }

  AudioPlugin::~AudioPlugin()
  {
  }

  const std::string& AudioPlugin::getName() const
  {
    return PLUGIN_NAME;
  }

  bool AudioPlugin::install()
  {
    AudioSystem* audio = mEngine->addSystem<AudioSystem>();
    if(mLuaInterface && mLuaInterface->getState())
    {
      luabind::module(mLuaInterface->getState())
      [
        luabind::class_<AudioSystem>("AudioSystem")
          .def("playSound", &AudioSystem::playSound)
          .def("playMusic", &AudioSystem::playMusic)
      ];

      luabind::globals(mLuaInterface->getState())["audio"] = audio;
    }
    return true;
  }

  void AudioPlugin::uninstall()
  {
    // not supported yet
  }

  AudioPlugin* audioPlugin = NULL;

  extern "C" bool PluginExport dllStartPlugin(GsageFacade* facade)
  {
    if(audioPlugin != NULL)
    {
      LOG(WARNING) << "Portaudio plugin is already installed";
      return false;
    }
    audioPlugin = new AudioPlugin();
    return facade->installPlugin(audioPlugin);
  }

  extern "C" bool PluginExport dllStopPlugin(GsageFacade* facade)
  {
    if(audioPlugin == NULL)
      return true;

    bool res = facade->uninstallPlugin(audioPlugin);
    delete audioPlugin;
    audioPlugin = NULL;
    return res;
  }
}
