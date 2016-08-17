/*
-----------------------------------------------------------------------------
This file is a part of Gsage engine SFML audio system plugin

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
    if (mLuaInterface && mLuaInterface->getState())
    {
      luabind::module(mLuaInterface->getState())
      [
        luabind::class_<AudioSystem>("AudioSystem")
          .def("playSound", &AudioSystem::playSound)
          .def("playMusic", &AudioSystem::playMusic)
          .def("stopPlayer", &AudioSystem::stopPlayer)
      ];
      luabind::globals(mLuaInterface->getState())["audio"] = audio;
    }
    else
    {
      LOG(WARNING) << "Lua bindings for audio plugin were not registered due to nil lua state";
    }

    return true;
  }

  void AudioPlugin::uninstall()
  {
    mEngine->removeSystem("audio");
  }

  AudioPlugin* audioPlugin = NULL;

  extern "C" bool PluginExport dllStartPlugin(GsageFacade* facade)
  {
    if(audioPlugin != NULL)
    {
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
    if(!res)
      return false;
    delete audioPlugin;
    audioPlugin = NULL;
    return true;
  }
}

