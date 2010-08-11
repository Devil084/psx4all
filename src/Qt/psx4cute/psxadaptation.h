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

#ifndef PSXADAPTATION_H_
#define PSXADAPTATION_H_

#include <QThread>

#include "QBlitterWidget.h"
#include "psxSettings.h"

/* the gpspadapation is basically the emulation thread
 * it's also passing commands safely from the UI thread
 * into the gpsp via Qts signal/slot system */
class psxadaptation : public QThread 
{
    Q_OBJECT

public:
    psxadaptation( QBlitterWidget* widget );
    ~psxadaptation();
    
    virtual void run();
public:
    void blit( const quint16* screen );
public slots:
    
    void Start();
    void Stop();
    void ResetGame();
    void LoadRom( QString aFileName,  TPsxSettings settings );
    
    void SaveState( int aState );
    void LoadState( int aState );    
    void exitpsx();

public:
    void showpsxFPS( bool fps );
    QString gameconfigpath();
    QString getMainpath();
    void showErrorNote( QString message );
    
signals:
    void frameblit();
    void dispatchErrorNote( QString message );
    
private:
    QBlitterWidget* blitter; //not owned
    TPsxSettings gsettings;
    QString rom;
    
};


#endif /* PSXADAPTATION_H_ */
