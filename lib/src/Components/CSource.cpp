#include "Components/CSource.h"
//#include <sndfile.h> TODO replace by my own
#include <EntityManager.h>
#include <Core/Debug.h>

using namespace fmc;

CSource::CSource() {
     _name = "Source";
}

CSource::CSource(const std::string& path) {
    _name = "Source";
}

CSource::~CSource() {

}


