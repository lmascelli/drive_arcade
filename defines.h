#pragma once

/******************************************************************************
 *                                 DEFINES
 ******************************************************************************/

#define MACHINE_ZERO 1e-9

// clang-format off
#define COLOR(X)  (X >> 24) & 0x000000ff ,\
                  (X >> 16) & 0x000000ff ,\
                  (X >>  8) & 0x000000ff ,\
                  (X      ) & 0x000000ff

#define TRANSPARENT COLOR(0x00000000)
#define BLACK       COLOR(0x000000ff)
#define WHITE       COLOR(0xffffffff)
#define RED         COLOR(0xff0000ff)
#define GREEN       COLOR(0x00ff00ff) 
#define DARK_GREEN  COLOR(0x00aa00ff)
#define BLUE        COLOR(0x0000ffff)
#define AZURE       COLOR(0x5050ffff)
#define GRAY        COLOR(0xaaaaaaff)
// clang-format on
