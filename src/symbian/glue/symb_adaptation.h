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

#ifndef SYMB_ADAPTATION_H_
#define SYMB_ADAPTATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/*This interface should be build in the application side
 * the gpsp is just statically linking into these functions
 * for integration
 * */

/*
 * This stuff is used from the C++ side to C side
 * */
int symb_main( char* rom, char* bios );

//audio adaptation

#ifdef __cplusplus
};
#endif

#endif /* SYMB_ADAPTATION_H_ */
