/* psx4cute
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

#include "symb_adaptation.h"
#include "psxadaptation.h"

#include <e32std.h>
#include <e32base.h>

#include "MEmulatorAdaptation.h"
#include "cuteDebug.h"
#include "debug.h"

#include "common.h"
#include "minimal.h"

#include "symbian_memoryhandler.h"
extern MEmulatorAdaptation* g_adaption;
psxadaptation* g_Adapt;

u16* g_screenptr;
quint8 paused;
unsigned long key = 0;

unsigned long	gp2x_ticks_per_second = 1000;

#include <QFileInfo>
/*
 * 240 x 160 resolution is used in gpsp emulator, the image is scaled in the apdatation layer
 * */
#define KSCREENPITCH 240
#define KSCREENHEIGHT 160
#define KSCREENWIDTH 240

extern int symb_main( char* rom, char* bios, char* gamedir );
void* gp2x_sdlwrapper_screen_pixels = 0;

psxadaptation::psxadaptation( QBlitterWidget* widget  )
    {
    blitter = widget;
    g_Adapt = this;
    }

void psxadaptation::run()
    {
    __DEBUG_IN
    
    __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());
 
    
	connect(this, SIGNAL(frameblit()), blitter, SLOT(render()), 
			Qt::BlockingQueuedConnection );
	
	__DEBUG3("Starting the psx, ROM and bios:", rom, gsettings.iBios );
	// TODO: Start the main in the emulator
	QFileInfo info( gsettings.iBios );
	QString path = info.canonicalPath();
	symb_main( (char*) rom.toStdString().c_str(), (char*) gsettings.iBios.toStdString().c_str(), 
			(char*) path.toStdString().c_str() );
	
	__DEBUG1("Main loop returned!");
    disconnect(this, SIGNAL(frameblit()), blitter, SLOT(render()) );
    }


psxadaptation::~psxadaptation()
{
   //call the exit stuff for the main
}

void psxadaptation::Start()
	{
    __DEBUG_IN
    paused = 0;
    __DEBUG_OUT
    
	}


void psxadaptation::Stop()
	{
    __DEBUG_IN
    paused = 1;
    __DEBUG_OUT
	}

void psxadaptation::ResetGame()
	{
    __DEBUG_IN
    
    __DEBUG_OUT	
	}

void psxadaptation::LoadRom( QString aFileName,  TPsxSettings settings )
	{
    gsettings = settings; 
    rom = aFileName;
    paused = 0;
    start( QThread::LowPriority );
	}

void psxadaptation::SaveState( int aState )
	{
    __DEBUG_IN
    
    __DEBUG_OUT
	}

void psxadaptation::LoadState( int aState )
	{
    __DEBUG_IN
    
    __DEBUG_OUT
	}

void psxadaptation::exitpsx()
	{
    __DEBUG_IN
    
    __DEBUG_OUT
	}

void psxadaptation::blit( const quint16* screen )
	{
    __DEBUG_IN
    emit(frameblit());
    __DEBUG_OUT
	}

unsigned long gp2x_joystick_read(void)
	{
	__DEBUG_IN
	while( paused == 1)
		{
		 symb_usleep(15000);
		} 
	antKeyEvent event;

	while(g_adaption->getKeyEvent(event))
		{
		if(event.keyDown )
			{
			key |= event.key;
			}
		else
			{
			key &= ~ event.key;
			}
		}
	__DEBUG_OUT
	return key;
	}


void gp2x_video_flip()
	{    
	__DEBUG_IN
	g_Adapt->blit( (quint16*) gp2x_sdlwrapper_screen_pixels );
	__DEBUG_OUT
	}

void gp2x_video_RGB_clearscreen16(void)
{
 // memset(sdlscreen->pixels, 0, sdlscreen->pitch*sdlscreen->h);
}

void gp2x_timer_delay_raw(unsigned long raws)
{
  //
}

unsigned long gp2x_timer_read(void)
{
  //
}

unsigned long gp2x_timer_raw(void)
{
  //
}

void gp2x_timer_delay(unsigned long ticks)
	{
	//symbian
	User::AfterHighRes(1000 * ticks );
	}

void gp2x_change_res(int w, int h)
{
	
}

void gp2x_init(int ticks_per_second, int bpp, int rate, int bits, int stereo, int hz, int solid_font)
{
	gp2x_sdlwrapper_screen_pixels = malloc( 320*240*2 );
}

void gp2x_deinit(void)
{
	
}

void gp2x_printf(gp2x_font *f, int x, int y, const char *format, ...)
{
	
}
//Debug helpers for the C-side
void WriteLOG( char* x )
	{
	__DEBUG1(x);
	}
void WriteLOG1( char* x, char* v )
	{
	__DEBUG2(x,v);
	}
void WriteLOG2( char* x, char* v, char* v1 )
	{
	__DEBUG3(x,v,v1);
	}
void WriteLOG3( char* x, char* v, char* v1, char* v2 )
	{
	__DEBUG4(x,v,v1,v2);
	}
void WriteLOG4( char* x, char* v, char* v1, char* v2, char* v3 )
	{
	
	}

void InitDEBUG()
	{
	
	}
