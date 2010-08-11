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

#ifndef GPSP4QT_H
#define GPSP4QT_H

#include <QtGui/QMainWindow>
#include <QKeyEvent>
#include <QList>
#include <QThread>

#include "QBlitterWidget.h"
#include "dpadwidget.h" 
#include "psxSettings.h"
#include "MEmulatorAdaptation.h"
#include "QRemoteControlKeys.h"

#include "dpadwidget.h"
#include "psxadaptation.h"

class psx4Qt : public QMainWindow, MEmulatorAdaptation
{
    Q_OBJECT

public:
    psx4Qt(QWidget *parent = 0);
    ~psx4Qt();

public:
    void setRemoteControl( QRemoteControlKeys* remote );
    void keyPressEvent( QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent* event);
public: //From MEmulatorAdaptation
    int getKeyEvent( antKeyEvent& keyEvent );
public:
    void LoadROM(QString rom, TPsxSettings antSettings);
    void LoadState( int state );
    void SaveState( int state );
    void reset();
    void continueGame();
    
signals:
    void Start();
    void Stop();
    void doLoadROM(QString rom, TPsxSettings antSettings);
    void showmenu();
    
public slots:
    void showAntSnesMenu();
    void updateSettings( TPsxSettings settings );
    void virtualKeyEvent( quint32 aKey, bool isDown );
private slots:
	void listencontrols();
    
private:
    QBlitterWidget* widget;
    
    TPsxSettings iAntSettings;
    QList<antKeyEvent> iPressedKeys;
    QRemoteControlKeys* remotecontrol;
    DPadWidget* dpad;
    psxadaptation* adaptation;
};

#endif // GPSP4QT_H
