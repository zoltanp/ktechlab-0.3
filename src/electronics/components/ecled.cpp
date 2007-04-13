/***************************************************************************
 *   Copyright (C) 2003 by David Saxton                                    *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "colorcombo.h"
#include "diode.h"
#include "ecled.h"
#include "ecnode.h"
#include "libraryitem.h"
#include "simulator.h"

#include <klocale.h>
#include <qpainter.h>

Item* ECLed::construct( ItemDocument *itemDocument, bool newItem, const char *id )
{
	return new ECLed( (ICNDocument*)itemDocument, newItem, id );
}

LibraryItem* ECLed::libraryItem()
{
	return new LibraryItem(
		QString::QString("ec/led"),
		i18n("LED"),
		i18n("Outputs"),
		"led.png",
		LibraryItem::lit_component,
		ECLed::construct
			);
}

ECLed::ECLed( ICNDocument *icnDocument, bool newItem, const char *id )
	: ECDiode( icnDocument, newItem, (id) ? id : "led" )
{
	m_bDynamicContent = true;
	m_name = i18n("LED");
	m_desc = i18n("Light Emitting Diode");
	setSize( -8, -16, 24, 24, true );
	avg_brightness = 255;
	lastUpdatePeriod = 0.;
	r=g=b=0;
	last_brightness = 255;
	
	createProperty( "0-color", Variant::Type::Color );
	property("0-color")->setCaption( i18n("Color") );
	property("0-color")->setColorScheme( ColorCombo::LED );
}

ECLed::~ECLed()
{
}

void ECLed::dataChanged()
{
	QColor color = dataColor("0-color");
	r = color.red();
	g = color.green();
	b = color.blue();
	r /= 0x100;
	g /= 0x100;
	b /= 0x100;
}

void ECLed::stepNonLogic()
{
	double interval = 1./LINEAR_UPDATE_RATE;
	avg_brightness += brightness(m_diode->current())*interval;
	lastUpdatePeriod += interval;
}

void ECLed::drawShape( QPainter &p )
{
	int _x = int(x());
	int _y = int(y());
	
	initPainter(p);
	
	//BEGIN draw "Diode" part
	uint _b;
	if ( lastUpdatePeriod == 0. )
		_b = last_brightness;
	else
	{
		_b = (uint)(avg_brightness/lastUpdatePeriod);
		last_brightness = _b;
	}
	avg_brightness = 0.;
	lastUpdatePeriod = 0.;
	
	p.setBrush( QColor( uint(255-(255-_b)*(1-r)), uint(255-(255-_b)*(1-g)), uint(255-(255-_b)*(1-b)) ) );
	
	QPointArray pa(3);
	pa[0] = QPoint( 8, 0 );
	pa[1] = QPoint( -8, -8 );
	pa[2] = QPoint( -8, 8 );
	pa.translate( _x, _y );
	p.drawPolygon(pa);
	p.drawPolyline(pa);
	
	p.drawLine( _x+8, _y-8, _x+8, _y+8 );
	//END draw "Diode" part
	
	
	
	//BEGIN draw "Arrows" part
	p.drawLine( _x+7, _y-10, _x+10, _y-13 ); // Tail of left arrow
	p.drawLine( _x+10, _y-13, _x+8, _y-13 ); // Left edge of left arrow tip
	p.drawLine( _x+10, _y-13, _x+10, _y-11 ); // Right edge of left arrow tip
	
	p.drawLine( _x+10, _y-7, _x+13, _y-10 ); // Tail of right arrow
	p.drawLine( _x+13, _y-10, _x+11, _y-10 ); // Left edge of right arrow tip
	p.drawLine( _x+13, _y-10, _x+13, _y-8 ); // Right edge of right arrow tip
	
	p.drawLine( _x+8, _y-13, _x+13, _y-8 ); // Diagonal line that forms base of both arrow tips
	//END draw "Arrows" part1
	
	
	deinitPainter(p);
}


uint ECLed::brightness( double i )
{
	if ( i > 0.018 ) return 0;
	if ( i < 0.002 ) return 255;
	return (uint)(255*(1-((i-0.002)/0.016)));
}
