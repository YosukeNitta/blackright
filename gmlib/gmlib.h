#pragma once

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <memory>
#include <shlwapi.h>

#include "src/FileLoader/IFileLoader.h"
#include "src/PathManager/IPathManager.h"
#include "src/Thread/IThread.h"

#include "src/FileLoader/CFileLoader.h"
#include "src/FileLoader/CArchiveLoader.h"
#include "src/FileLoader/CVirtualFileLoader.h"

#include "src/PathManager/CPathManager.h"

#include "src/Thread/CThread.h"

#include "src/Socket/ISocket.h"
#include "src/Socket/CSocket.h"

#include "src/Server/IServer.h"
#include "src/Server/CServer.h"
