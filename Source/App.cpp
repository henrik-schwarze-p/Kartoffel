#ifdef MEGA
#include "UIRC.h"
#include "Instance.h"
#endif

#include "Target.h"
#include "Namespaces.h"
#include "MainLoop.h"
#include "UI.h"
#include "BoardLib.h"
#include "Launcher.h"
#include "Heap.h"
#include "Rules.h"
#include "Graph.h"

void loop() {
    mainLoop();
}

void setup() {
    initScheduler();
    initBoard();
    tftReset();
    initHeap();
    launchAll();
    initTimer();
    message(PSTR("Kartoffel - K. Schwarze Jan-3-2025"), desktop::startScreen);
}
