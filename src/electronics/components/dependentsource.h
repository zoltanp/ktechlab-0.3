/***************************************************************************
 *   Copyright (C) 2005 by David Saxton                                    *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DEPENDENTSOURCE_H
#define DEPENDENTSOURCE_H

#include "simplecomponent.h"

#include "cccs.h"
#include "ccvs.h"
#include "vccs.h"
#include "vcvs.h"

/**
@author David Saxton
*/
class DependentSource : public SimpleComponent {

public:
	DependentSource(ICNDocument *icnDocument, bool newItem, const char *id);
	~DependentSource();

protected:
	void drawOutline(QPainter &p);
	void drawTopArrow(QPainter &p);
	void drawBottomArrow(QPainter &p);
};

/**
@short Current Controlled Current Source
@author David Saxton
*/
class ECCCCS : public DependentSource {

public:
	ECCCCS(ICNDocument *icnDocument, bool newItem, const char *id = 0L);
	~ECCCCS();

	static Item* construct(ItemDocument *itemDocument, bool newItem, const char *id);
	static LibraryItem *libraryItem();

protected:
	virtual void dataChanged();
	virtual void drawShape(QPainter &p);

	CCCS m_cccs;
};

/**
@short Current Controlled Voltage Source
@author David Saxton
*/
class ECCCVS : public DependentSource {

public:
	ECCCVS(ICNDocument *icnDocument, bool newItem, const char *id = 0);
	~ECCCVS();

	static Item *construct(ItemDocument *itemDocument, bool newItem, const char *id);
	static LibraryItem *libraryItem();

protected:
	virtual void dataChanged();
	virtual void drawShape(QPainter &p);

	CCVS m_ccvs;
};

/**
@short Voltage Controlled Current Source
@author David Saxton
*/
class ECVCCS : public DependentSource {

public:
	ECVCCS(ICNDocument *icnDocument, bool newItem, const char *id = 0);
	~ECVCCS();

	static Item* construct(ItemDocument *itemDocument, bool newItem, const char *id);
	static LibraryItem *libraryItem();

protected:
	virtual void dataChanged();
	virtual void drawShape(QPainter &p);

	VCCS m_vccs;
};

/**
@short Voltage Controlled Voltage Source
@author David Saxton
*/
class ECVCVS : public DependentSource {

public:
	ECVCVS(ICNDocument *icnDocument, bool newItem, const char *id = 0);
	~ECVCVS();

	static Item* construct(ItemDocument *itemDocument, bool newItem, const char *id);
	static LibraryItem *libraryItem();

protected:
	virtual void dataChanged();
	virtual void drawShape(QPainter &p);

	VCVS m_vcvs;
};

#endif
