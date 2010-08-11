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

#include <QFileDialog>
#include <QString>
#include <QSettings>
#include <QDir>

#include "emusettings.h"
#include "cuteDebug.h"

#define KSettingsVersion 4

EmuSettings::EmuSettings(QWidget *parent)
    : QMainWindow(parent)
{
	loadSettings();
	
	ui.setupUi(this);
	showFullScreen();
	
	saveSlotIndexChanged(psxsettings.iLastSlot);
	//int audioOn, int samplerte, int stereoOn, int volume, bool enablespeedhack, QWidget *parent 
	//TODO: read settings
	audiosettings = new AudioSettings(psxsettings.iAudioOn, psxsettings.iSampleRate, 
										psxsettings.iStereo, psxsettings.iVolume, this );
	audiosettings->setGeometry(QRect(0, 0, 640, 150));
	audiosettings->hide();
	
	antvideosettings =new videosettings( psxsettings.iFrameSkip, this );
	antvideosettings->setGeometry(QRect(0, 0, 640, 150));
	antvideosettings->hide();
	
	fileview = new filewidget( psxsettings.iLastROM, psxsettings.iBios,
			this );
	fileview->setGeometry(QRect(0, 0, 640, 150));
	fileview->hide();
		
	keysettings =new controlsettings( this );
	keysettings->setGeometry(QRect(0, 0, 640, 150));
	keysettings->hide();
	
	currentWidget = EMainWidget;
	
	ui.saveSlotBox->setCurrentIndex(psxsettings.iLastSlot);
	
	//connect buttons
	connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(showFileMenu()));
	connect(ui.audioButton, SIGNAL(clicked()), this, SLOT(showAudioSettings()));
	connect(ui.videoButton, SIGNAL(clicked()), this, SLOT(showVideoSettings()));
	connect(ui.controlsButton, SIGNAL(clicked()), this, SLOT(showControlSettings()));
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui.loadStateButton, SIGNAL(clicked()), this, SLOT(loadStateButtonClicked()));
	connect(ui.saveStateButton, SIGNAL(clicked()), this, SLOT(saveStateButtonClicked()));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
	connect(ui.ContinueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
	connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(aboutClicked()));
	connect(ui.saveSlotBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveSlotIndexChanged(int)));
	
	//connect audio settins
	connect( audiosettings, SIGNAL(AudioOn(int)), this, SLOT( setAudioOn(int)));
	connect( audiosettings, SIGNAL(SampleRate(int)), this, SLOT( setSampleRate(int)));
	connect( audiosettings, SIGNAL(enableStereo(int)), this, SLOT( setStereoOn(int)));
	connect( audiosettings, SIGNAL(Volume(int)), this, SLOT( setVolume(int)));
	
	//connect video settings
	connect( antvideosettings, SIGNAL(frameskip(int)), this, SLOT( frameskip(int) ));
	
	//connect filewidget
	connect( fileview, SIGNAL(loadROM()), this, SLOT( loadROM()));
	connect( fileview, SIGNAL(setBios()), this, SLOT( setBios()));
	
	//connect control settings
	connect( keysettings, SIGNAL(runkeyconfig()), this, SLOT( keyConfig() ));
	
	romloaded = false;
	settingsChanged = false;
}

EmuSettings::~EmuSettings()
{
	savecurrentSettings();
	
	//delete all views
	delete keysettings;
	delete antvideosettings;
	delete audiosettings;
	delete fileview;
}

void EmuSettings::setRemoteControl( QRemoteControlKeys* remote )
	{
	remotecontrol = remote;
	}
void EmuSettings::loadROM()
    {    
    __DEBUG_IN
    QDir dir(psxsettings.iLastROM);
    //TODO: any other imageformats?
    QString rom = QFileDialog::getOpenFileName(this,
                    tr("Open File"),
                    dir.absolutePath(),
                    tr("ROMs (*.BIN *.bin *.cue *.CUE *.IMG *.img *.ccd *.CCD *.sub *.SUB *.PBP *.pbp);;"));
    if( rom.isEmpty() )
        {
		return;
        }
    
    psxsettings.iLastROM = rom;
    savecurrentSettings();
    emit( LoadROM( rom,psxsettings ));
    romloaded = true;
    __DEBUG_OUT
    }

void EmuSettings::setBios()
	{
    savecurrentSettings();
    QDir dir(psxsettings.iBios);
    QString bios = QFileDialog::getOpenFileName(this,
                    tr("Bios"),
                    dir.absolutePath(),
                    tr("ROMs (*.bin *.BIN);;"));
    if( bios.isEmpty() )
        {
		return;
        }
    
    psxsettings.iBios = bios;
	}

void EmuSettings::showFileMenu()
	{
	if( currentWidget == EFileWidget )
		{
		//hide file Widget, and continue
		fileview->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		fileview->show();
		currentWidget = EFileWidget;
		}
	}

void EmuSettings::keyConfig()
    {
    keydialog = new keyconfigdialog( this );
    connect(keydialog, SIGNAL(configDone()), this, SLOT(keyconfigDone()));
    remotecontrol->subscribeKeyEvent( keydialog );
    keydialog->show();
    keydialog->setFocus();
    }

void EmuSettings::frameskip( int skip )
    {
	__DEBUG_IN
	__DEBUG2("Frameskip is", skip );
	settingsChanged = true;
    psxsettings.iFrameSkip = skip;
	__DEBUG_OUT
    }

void EmuSettings::showAudioSettings()
	{
	if( currentWidget == EAudioWidget )
		{
		//hide audio Widget, and continue
		audiosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		audiosettings->show();
		currentWidget = EAudioWidget;
		}
	}

void EmuSettings::showVideoSettings()
	{
	if( currentWidget == EVideoWidget )
		{
		//hide video Widget, and continue
		antvideosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		antvideosettings->show();
		currentWidget = EVideoWidget;
		}
	
	}

void EmuSettings::showControlSettings()
	{
	if( currentWidget == EKeyWidget )
		{
		//hide keysettings Widget, and continue
		keysettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		keysettings->show();
		currentWidget = EKeyWidget;
		}
	}

void EmuSettings::setAudioOn( int audioOn )
    {
	settingsChanged = true;
    psxsettings.iAudioOn = (bool) audioOn;
    }

void EmuSettings::setSampleRate( int sampleRate )
    {
	__DEBUG_IN
	settingsChanged = true;
	__DEBUG2("SampleRate is", sampleRate );
    psxsettings.iSampleRate = sampleRate;
	__DEBUG_OUT
    }

void EmuSettings::setStereoOn( int stereoOn )
    {
	__DEBUG_IN
	settingsChanged = true;
	psxsettings.iStereo = (bool) stereoOn;
	__DEBUG_OUT
    }

void EmuSettings::setVolume( int volume )
    {
	settingsChanged = true;
	psxsettings.iVolume = volume;
    }

void EmuSettings::keyconfigDone()
    {
	settingsChanged = true;
	//get the key config
	for( int i=0; i<12; i++)
		{
		__DEBUG2("keyconfigDone: keyID is ", keydialog->getKeyBind(i) );
		psxsettings.iScanKeyTable[i] = keydialog->getKeyBind(i);
		}
    //Delete the dialog
	keydialog->hide();
    delete keydialog;
    keydialog = NULL;
    }

void EmuSettings::exit()
	{
	savecurrentSettings();
	QApplication::exit(0);
	}

void EmuSettings::resetButtonClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(psxsettings) );
		settingsChanged = false;
    	}
    
	if( romloaded )
		emit( resetGame() );
	}

void EmuSettings::continueClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(psxsettings) );
		settingsChanged = false;
    	}
    
	//if there was no ROM loaded, load previous ROM
	if( !romloaded )
		{
		emit( LoadROM( psxsettings.iLastROM, psxsettings ));
	    romloaded = true;
		}
	//just continue the game
	else
		{
		emit( continueGame() );
		}
	}

void EmuSettings::aboutClicked()
	{
	__DEBUG_IN
	//show about dialog
	about = new aboutdialog( this ); 
	connect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	about->show();
	about->setFocus();
	__DEBUG_OUT
	}

void EmuSettings::closeAboutDialog()
	{
	__DEBUG_IN
	disconnect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	delete about;
	about = NULL;
	__DEBUG_OUT
	}

void EmuSettings::saveSlotIndexChanged( int index )
	{
	__DEBUG_IN
	//change the pixmap
	/*
	QPixmap *pixmap = new QPixmap();
	QString filename = antsettings.iLastROM.left( antsettings.iLastROM.size() - 4 );
	filename.append( QString::number(index) );
	filename.append( ".jpg");
	
	bool loaded = pixmap->load(filename);
	if( loaded )
		{
		ui.stateLabel->setPixmap(*pixmap);
		ui.stateLabel->setScaledContents(true);
		}
	*/
	__DEBUG_OUT
	}

void EmuSettings::loadStateButtonClicked()
	{
	__DEBUG_IN
	psxsettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
    if( settingsChanged )
    	{
		emit( updateSettings(psxsettings) );
		settingsChanged = false;
    	}
	int state = ui.saveSlotBox->currentIndex();
	emit( LoadState(state) );
	__DEBUG_OUT
	}

void EmuSettings::saveStateButtonClicked()
	{
	__DEBUG_IN
	psxsettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
	if( settingsChanged )
	   	{
		emit( updateSettings(psxsettings) );
		settingsChanged = false;
	    }
	   
	int state = ui.saveSlotBox->currentIndex();
	emit( SaveState(state) );

	__DEBUG_OUT
	}

void EmuSettings::hidecurrentWidget()
	{
	switch( currentWidget )
		{
		case EMainWidget:
			ui.settingsViewWidget->hide();
			break;
		case EAudioWidget:
			audiosettings->hide();
			break;
		case EVideoWidget:
			antvideosettings->hide();
			break;
		case EKeyWidget:
			keysettings->hide();
			break;
		case EFileWidget:
			fileview->hide();
			break;
		}
	}

void EmuSettings::setDefaultSettings()
	{
	__DEBUG_IN
	psxsettings.iVersion = KSettingsVersion;

	for(int i=0;i<12;i++)
		{
		psxsettings.iScanKeyTable[i] = 0;
		}
	
	psxsettings.iLastROM = "";
	psxsettings.iBios = "";
	psxsettings.iLastSlot = 0;
	psxsettings.iShowFPS = false;
	psxsettings.iFrameSkip = 0;
	psxsettings.iAudioOn = false;
	psxsettings.iSampleRate = 22050;
	psxsettings.iStereo = false;
	psxsettings.iVolume = 4;
	psxsettings.iLastSlot = 1;
	__DEBUG_OUT
	}

void EmuSettings::savecurrentSettings()
	{
	__DEBUG_IN

	QSettings settings;
	settings.setValue("psx_version", KSettingsVersion );

	for(int i=0;i<12;i++)
		{
	    QString keyval = "psx_kebind";
	    keyval.append( QString::number(i) );
		settings.setValue(keyval, psxsettings.iScanKeyTable[i]);
		}
	settings.setValue("psx_lastrom",psxsettings.iLastROM);
	settings.setValue("psx_bios",psxsettings.iBios);
	settings.setValue("psx_lastslot",psxsettings.iLastSlot);
	settings.setValue("psx_showfps",psxsettings.iShowFPS);
	settings.setValue("psx_frameskip",psxsettings.iFrameSkip);
	settings.setValue("psx_audioOn",psxsettings.iAudioOn);
	settings.setValue("psx_samplerate",psxsettings.iSampleRate);
	settings.setValue("psx_stereo",psxsettings.iStereo);
	settings.setValue("psx_volume",psxsettings.iVolume);
	settings.setValue("psx_lastslot",psxsettings.iLastSlot);
	settings.sync();
	__DEBUG_OUT
	}

void EmuSettings::loadSettings()
	{
	__DEBUG_IN
	QSettings settings;
	int version = settings.value("psx_version").toInt();
	if( version != KSettingsVersion )
		{
		__DEBUG1("No version was set, creating default settings");
		setDefaultSettings();
		savecurrentSettings();
		return;
		}
	psxsettings.iVersion = version;

	for(int i=0;i<12;i++)
		{
	    QString keyval = "psx_kebind";
	    keyval.append( QString::number(i) );
	    psxsettings.iScanKeyTable[i] = settings.value(keyval).toUInt();
		}
	psxsettings.iLastROM = settings.value("psx_lastrom").toString();
	psxsettings.iBios = settings.value("psx_bios").toString();
	psxsettings.iShowFPS = settings.value("psx_showfps").toBool();
	psxsettings.iFrameSkip = settings.value("psx_frameskip").toInt();
	psxsettings.iAudioOn = settings.value("psx_audioOn").toBool();
	psxsettings.iSampleRate = settings.value("psx_samplerate").toInt();
	psxsettings.iStereo = settings.value("psx_stereo").toBool();
	psxsettings.iVolume = settings.value("psx_volume").toInt();
	psxsettings.iLastSlot = settings.value("psx_lastslot").toInt();
	
	__DEBUG_OUT
	}
