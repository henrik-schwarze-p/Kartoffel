//
//  Repainter.c
//  Kartoffel
//
//  Created by Kurt Schwarze on 05.09.23.
//  Copyright © 2023 Synium. All rights reserved.
//

#include "Repainter.h"

int _repaint = 0;

int repaintState() {
    return _repaint;
}

void setRepaintState(int state) {
    _repaint = state;
}

// The middle part of the screen has to be repainted. It won't be cleaned before.
void repaint() {
    _repaint |= REPAINT;
}

// When repainted, clean the middle part of the screen
void repaintCleanMiddle() {
    _repaint |= REPAINT_CLEAN_MIDDLE;
}

// Toolbar has to be repainted
void repaintToolbar() {
    _repaint |= REPAINT_TOOLBAR;
}
