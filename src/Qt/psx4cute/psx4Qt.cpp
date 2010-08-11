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

#include "psx4Qt.h"

#include "cuteDebug.h"
  
#include "minimal.h"

#define BIT_U		0x1
#define BIT_D		0x10
#define BIT_L 		0x4
#define BIT_R	 	0x40
#define BIT_SEL		1<<9
#define BIT_ST		1<<8
#define BIT_LPAD	1<<10
#define BIT_RPAD	1<<11
#define BIT_A		1<<12
#define BIT_B		1<<13
#define BIT_X		1<<14
#define BIT_Y		1<<15

uint KAntKeyTable[12]={BIT_U,BIT_D,BIT_L,BIT_R,BIT_A,
		BIT_B,BIT_X,BIT_Y,BIT_LPAD,BIT_RPAD, BIT_SEL, BIT_ST };

MEmulatorAdaptation* g_adaption;

psx4Qt::psx4Qt(QWidget *parent)
    : QMainWindow(parent)
{
	QThread::currentThread()->setPriority( QThread::NormalPriority );
	showFullScreen();
	
    //create button widgets
    dpad = new DPadWidget( this );
    dpad->setGeometry(QRect(0, 0, 128, 360));
    dpad->show();
    connect(dpad, SIGNAL(showMenu()), this, SLOT( showAntSnesMenu()) );
    connect(dpad, SIGNAL(virtualKeyEvent(quint32, bool)), this, SLOT( virtualKeyEvent(quint32, bool)) );
    
    widget = new QBlitterWidget();
    widget->setObjectName(QString::fromUtf8("QBlitterWidget"));
    widget->setGeometry(QRect(160, 0, 640, 360));
    
    adaptation = new psxadaptation( widget );
   // ui.menuButton->setEditFocus(false);
    
    //connect buttons from the ui
   // connect(ui.menuButton, SIGNAL(clicked()), this, SLOT( showAntSnesMenu()) );
    
    //the widget cam be repainted with repaint or update commands
    //widget->repaint();    
    
    //TODO: use Qt::QueuedConnection in here?
    connect(this, SIGNAL(Start()), widget, SLOT(startDSA()) );
    connect(this, SIGNAL(Stop()), widget, SLOT(stopDSA()) );
    
    connect(this, SIGNAL(Start()), adaptation, SLOT(Start()) );
    connect(this, SIGNAL(Stop()), adaptation, SLOT(Stop()) );
     
    
    //connect all adaptation stuff
    connect(this, SIGNAL(doLoadROM(QString,TPsxSettings)), adaptation, SLOT(LoadRom(QString,TPsxSettings)) );
    
    connect(this, SIGNAL(Start()), this, SLOT(listencontrols()) );
    
    g_adaption = this;
}

psx4Qt::~psx4Qt()
{
	delete widget;
	delete dpad;
}

void psx4Qt::setRemoteControl( QRemoteControlKeys* remote )
	{
	remotecontrol = remote;
	}

void psx4Qt::keyPressEvent( QKeyEvent * event)
    {
	__DEBUG_IN
    __DEBUG2("Key Pressed: ", event->text()); 
    
    quint32 c = event->nativeScanCode();
    antKeyEvent keyevent;
    
	__DEBUG2("key pressed, scancode is ", c );
	for(TInt i=0;i<10;i++)
		{
		if(c==iAntSettings.iScanKeyTable[i])
			{
		    __DEBUG4("pressed gpsp mapped key in round:", i, "which is:", KAntKeyTable[i]);
			keyevent.key = KAntKeyTable[i];
			keyevent.keyDown = true;
			iPressedKeys.append( keyevent );
			break;
			}
		}

    }

void psx4Qt::keyReleaseEvent(QKeyEvent* event)
	{
	__DEBUG_IN
    quint32 c = event->nativeScanCode();
    antKeyEvent keyevent;
    
	__DEBUG2("key released, scancode is ", c );
	for(TInt i=0;i<10;i++)
		{
		if(c==iAntSettings.iScanKeyTable[i])
			{
			__DEBUG4("released gpsp mapped key in round:", i, "which is:", KAntKeyTable[i]);
			keyevent.key = KAntKeyTable[i];
			keyevent.keyDown = false;
			iPressedKeys.append( keyevent );
			break;
			}
		}
	__DEBUG_OUT
	}

void psx4Qt::LoadROM(QString rom, TPsxSettings antSettings )
    {
	__DEBUG_IN
	iAntSettings = antSettings;
   
	emit( doLoadROM( rom, antSettings ));
	
	emit(Start());
	__DEBUG_OUT
    }

void psx4Qt::showAntSnesMenu()
	{
	__DEBUG_IN
	emit( Stop() );
	emit( showmenu() );
	__DEBUG_OUT
	}

void psx4Qt::LoadState( int state )
	{
	__DEBUG_IN
	emit(Start());
	__DEBUG_OUT
	}

void psx4Qt::SaveState( int state )
	{
	__DEBUG_IN
	widget->saveStateImage( iAntSettings.iLastROM, state );
	emit(Start());
	__DEBUG_OUT
	}

void psx4Qt::reset()
	{
	emit( Start() );
	}

void psx4Qt::continueGame()
	{
	emit( Start() );
	}

void psx4Qt::updateSettings( TPsxSettings settings )
	{
	__DEBUG_IN
	iAntSettings = settings;
	__DEBUG_OUT
	}

void psx4Qt::virtualKeyEvent( quint32 aKey, bool isDown )
	{
	__DEBUG_IN
    antKeyEvent keyevent;
    keyevent.key = aKey;
    keyevent.keyDown = isDown;
    iPressedKeys.append( keyevent );
    __DEBUG_OUT
	}

int psx4Qt::getKeyEvent( antKeyEvent& keyEvent )
	{
	if( iPressedKeys.isEmpty() )
		{
		return 0;
		}
	keyEvent = iPressedKeys.takeFirst();
	 __DEBUG3("returning keyevent, pressed, scancode", keyEvent.keyDown, keyEvent.key );
	 return 1;
	}

void psx4Qt::listencontrols()
	{
	remotecontrol->subscribeKeyEvent(this);
	}

