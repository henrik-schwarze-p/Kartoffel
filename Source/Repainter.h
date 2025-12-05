//
//  Repainter.h
//  Kartoffel
//
//  Created by Kurt Schwarze on 05.09.23.
//  Copyright © 2023 Synium. All rights reserved.
//

#ifndef Repainter_h
#define Repainter_h

#define REPAINT 8
#define REPAINT_CLEAN_MIDDLE 4
#define REPAINT_MENUBAR 2
#define REPAINT_TOOLBAR 1

int  repaintState();
void setRepaintState(int state);

void repaint();
void repaintCleanMiddle();
void repaintToolbar();

#endif /* Repainter_h */
