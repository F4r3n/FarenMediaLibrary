#pragma once

#include <map>
#include "NonCopyable.h"
#include "Speaker.h"
#include "Listener.h"

namespace fm
{
class MusicManager : public fm_system::NonCopyable
{
public:
    MusicManager();
    ~MusicManager();
    Listener& getListener();
    static MusicManager& getInstance();

private:
    Speaker speaker;
    Listener listener;
    static MusicManager _instance;
};
}