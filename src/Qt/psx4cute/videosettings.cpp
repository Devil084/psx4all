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

#include "videosettings.h"

videosettings::videosettings(int frameskip, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.frameSkipBox->setCurrentIndex( frameskip );
	connect(ui.frameSkipBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setFrameskip(int)));
	
}

videosettings::~videosettings()
{

}

void videosettings::setFrameskip( int framestoskip )
	{
	emit (frameskip( framestoskip ));
	}
