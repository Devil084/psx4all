/* psx4symbian
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef SYMBIAN_MEMORY_HANDLER_H_
#define SYMBIAN_MEMORY_HANDLER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

int create_all_translation_caches();
void clear_insn_cache(u32 start, u32 end, u32 type);
void SymbianPackHeap();
void close_all_caches();
void setDefaultPreferences();
u32 SymbianRelativeOffset( u32 source, u32 offset );
void Symbianfunctioncheck( u32 func );
typedef unsigned char byte; 
void symb_usleep(int aValue);

struct app_Preferences {
    byte frameSkip;
    byte debug;
    byte canDeleteROMs;
    byte landscape;
    byte allowSuspend;
    byte scaled;
    byte muted;
    byte selectedSkin;
    byte interlace;
    byte bios;
};

#ifdef __cplusplus
};
#endif

#endif /* SYMBIAN_MEMORY_HANDLER_H_ */
