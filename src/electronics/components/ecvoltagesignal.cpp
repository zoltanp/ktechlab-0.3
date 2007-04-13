/***************************************************************************
 *   Copyright (C) 2003-2004 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ecnode.h"
#include "ecvoltagesignal.h"
#include "libraryitem.h"
#include "pin.h"
#include "simulator.h"
#include "voltagesignal.h"

#include <klocale.h>
#include <qpainter.h>

Item* ECVoltageSignal::construct( ItemDocument *itemDocument, bool newItem, const char *id )
{
	return new ECVoltageSignal( (ICNDocument*)itemDocument, newItem, id );
}

LibraryItem* ECVoltageSignal::libraryItem()
{
	return new LibraryItem(
		QString("ec/voltage_signal"),
		i18n("Voltage Signal"),
		i18n("Sources"),
		"voltagesignal.png",
		LibraryItem::lit_component,
		ECVoltageSignal::construct );
}

ECVoltageSignal::ECVoltageSignal( ICNDocument *icnDocument, bool newItem, const char *id )
	: Component( icnDocument, newItem, (id) ? id : "voltage_signal" )
{
	m_name = i18n("Voltage Signal");
	m_desc = i18n("Provides a variety of voltage signals.");
	setSize( -8, -8, 16, 16 );
	
	init1PinLeft();
	init1PinRight();
	
	m_pNNode[0]->pin()->setGroundType( Pin::gt_medium );
	m_voltageSignal = createVoltageSignal( m_pNNode[0], m_pPNode[0], 0. );
	m_voltageSignal->setStep( 1./LINEAR_UPDATE_RATE, ElementSignal::st_sinusoidal, 50. );

// TODO: NEEDS PHASE ANGLE PARAMETER =))))
// Ideally phase angle is entered in units of PI so 180 degrees is simply "1.0"

	createProperty( "frequency", Variant::Type::Double );
	property("frequency")->setCaption( i18n("Frequency") );
	property("frequency")->setUnit("Hz");
	property("frequency")->setMinValue(1e-9);
	property("frequency")->setMaxValue(1e3);
	property("frequency")->setValue(50.0);
	
	createProperty( "voltage", Variant::Type::Double );
	property("voltage")->setCaption( i18n("Voltage Range") );
	property("voltage")->setUnit("V");	
	property("voltage")->setMinValue(-1e12);
	property("voltage")->setMaxValue(1e12);
	property("voltage")->setValue(5.0);
	
	addDisplayText( "~", QRect( -8, -8, 16, 16 ), "~" );
	addDisplayText( "voltage", QRect( -16, -24, 32, 16 ), "" );
}


ECVoltageSignal::~ECVoltageSignal()
{
}

void ECVoltageSignal::dataChanged()
{
	const double voltage = dataDouble("voltage");
	const double frequency = dataDouble("frequency");
	
	QString display = QString::number( voltage / getMultiplier(voltage), 'g', 3 ) + getNumberMag(voltage) + "V";
	setDisplayText( "voltage", display );
	
	m_voltageSignal->setStep( 1./LINEAR_UPDATE_RATE, ElementSignal::st_sinusoidal, frequency );
	m_voltageSignal->setVoltage(voltage);
}


void ECVoltageSignal::drawShape( QPainter &p )
{
	initPainter(p);
	p.drawEllipse( (int)x()-8, (int)y()-8, width(), height() );
	deinitPainter(p);
}
