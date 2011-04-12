#ifndef PCDEBUG_H
#define PCDEBUG_H

#include "javaEssentials.h"
#include <jni.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdarg>

//function that implements the debug concept
void PCDebug(const char* msg, ...);

#include "PCDebug.c"

#endif
