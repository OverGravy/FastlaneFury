#ifndef __LIBS_GRAPHICS_INSTRUCTION_H__
#define __LIBS_GRAPHICS_INSTRUCTION_H__

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game_core.h"
#include "Ptask.h"
#include "Label_primitives.h"

// function that render the instruction on screen
void render_instruction(BITMAP *dest_buffer, struct argument_struct *arg);

#endif