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

#ifdef __SYMBIAN32__

#include <e32std.h>

#include "symbian_memoryhandler.h"

#include "defines.h"

static RChunk* g_code_chunk = NULL;
static RHeap* g_code_heap = NULL;

extern u8* recMemBase;

struct app_Preferences preferences;

#define KDistanceFromCodeSection 0x500000

//in Symbian we have to tell to the OS that these memoryblocks contains codes,
//so we can invalidate and execute the codeblock in the future.
typedef unsigned char byte; 

TInt CreateChunkAt(TUint32 addr,TInt minsize, TInt maxsize )
{
  TInt err = g_code_chunk->CreateLocalCode( minsize, maxsize );
  if( err )
	  return err;
  if ((TUint32)g_code_chunk->Base() != addr)
      {
      TUint offset = (TInt)g_code_chunk->Base();
      offset = addr-offset;
      g_code_chunk->Close();
      RChunk temp;
      if( offset > 0x7FFFFFFF )
    	  {
    	  //shit, offset too big :(
    	  return KErrNoMemory;
    	  }
      TInt chunkoffset = (TInt) offset;
      err = temp.CreateLocal(0,chunkoffset);
      if( err )
    	  {
    	  temp.Close();
    	  return err;
    	  }
      err = g_code_chunk->CreateLocalCode(minsize,maxsize);        
      temp.Close();
      }
  return err;
}

int create_all_translation_caches()
	{
	TInt minsize = RECMEM_SIZE;
	TInt maxsize = RECMEM_SIZE + 4096;
	
	RProcess process;
	TModuleMemoryInfo  info;
	
	TInt error = process.GetMemoryInfo( info );
	if( error )
		return error;
	
	TUint32 programAddr = 0x10000000;//hardcoded, see main.cpp in cute side
	programAddr += info.iCodeSize;
	
	TUint32 destAddr = programAddr - KDistanceFromCodeSection;
	
	g_code_chunk = new RChunk();
	
	TInt err = CreateChunkAt(destAddr,minsize, maxsize );
	
	TUint32 dynamiaddr = (TUint32) g_code_chunk->Base();
	
	if( err )
		return err;
	
	g_code_heap = UserHeap::ChunkHeap(*g_code_chunk, minsize, 1, maxsize );
	if( g_code_heap != NULL )
	    {
	    recMemBase = (u8*) g_code_heap->Alloc( RECMEM_SIZE );
	    if( recMemBase == NULL)
	            DEBUG("ROM ALLOC FAIL!");
	    }
    return 0;
	}
/*
int create_all_translation_caches()
	{
	TInt minsize = RECMEM_SIZE;
	TInt maxsize = RECMEM_SIZE + 4096;
	
	g_code_chunk = new RChunk();
	
	int err = g_code_chunk->CreateLocalCode(minsize, maxsize );
	if( err )
		return err;
	
	g_code_heap = UserHeap::ChunkHeap(*g_code_chunk, minsize, 1, maxsize );
	if( g_code_heap != NULL )
	    {
	    recMemBase = (u8*) g_code_heap->Alloc( RECMEM_SIZE );
	    if( recMemBase == NULL)
	            DEBUG("ROM ALLOC FAIL!");
	    }
    return 0;
	}
*/
void clear_insn_cache(u32 start, u32 end, u32 type)
{
    TAny* a = reinterpret_cast<TAny*>( start );
    TAny* b = reinterpret_cast<TAny*>( end );
	User::IMB_Range( a, b );   
}

void SymbianPackHeap()
	{
	User::CompressAllHeaps();
	User::Heap().Compress();
	}

void close_all_caches()
    {
    
    g_code_heap->Free( recMemBase );

    g_code_heap->Close();
 

    g_code_chunk->Close();;
    }

void setDefaultPreferences() {
    preferences.frameSkip = 1;
    preferences.debug = 0;
    preferences.canDeleteROMs = 0;
    preferences.landscape = 1;
    preferences.muted = 1;
    preferences.scaled = 1;
    preferences.selectedSkin = 0;
    preferences.interlace = 0;
    preferences.bios = 1;
}

u32 SymbianRelativeOffset( u32 source, u32 offset )
{
u32 new_source = source + 8;
u32 final_offset = ((offset - new_source) / 4) & 0xFFFFFF;
// It seems it needs this printf with new_source as an argument for some sort of cache issue	
printf("", new_source);
return final_offset;
}

void Symbianfunctioncheck( u32 func )
	{
	u32 newfunc = func;
	
	}


void symb_usleep(int aValue)
{
	User::AfterHighRes(aValue);
}

#endif
