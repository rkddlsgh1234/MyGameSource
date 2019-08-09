#pragma once
#pragma comment (lib, "Msimg32.lib")
#include <Windows.h>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
