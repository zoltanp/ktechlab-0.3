/***************************************************************************
 *   Copyright (C) 2003-2005 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "canvasitemparts.h"
#include "icndocument.h"
#include "cells.h"
#include "component.h"
#include "ecnode.h"
#include "fpnode.h"
#include "itemdocumentdata.h"
#include <kdebug.h>

#include <qbitarray.h>
#include <qpainter.h>

#include <cmath>


CNItem::CNItem( ICNDocument *icnDocument, bool newItem, const QString &id )
	: Item( icnDocument, newItem, id ),
	  CIWidgetMgr( icnDocument->canvas(), this ),
	  p_icnDocument(icnDocument),
	  b_pointsAdded(false)
{
	setZ( ICNDocument::Z::Item );
	setSelected(false);
	
	m_brushCol = QColor( 0xf7, 0xf7, 0xff );
	m_selectedCol = QColor( 101, 134, 192 );
	
	setBrush(m_brushCol);
	setPen( Qt::black );
}

CNItem::~CNItem()
{
	const TextMap::iterator textMapEnd = m_textMap.end();
	for ( TextMap::iterator it = m_textMap.begin(); it != textMapEnd; ++it )
	{
		if (it.data())
			it.data()->setCanvas(0);
		delete (Text*)it.data();
	}
	m_textMap.clear();
	
	updateConnectorPoints(false);
}

int CNItem::rtti() const
{
	return ItemDocument::RTTI::CNItem;
}

bool CNItem::preResize( QRect sizeRect )
{
	if ( (std::abs(sizeRect.width()) < minimumSize().width()) ||
		 (std::abs(sizeRect.height()) < minimumSize().height()) )
		return false;
	
	updateConnectorPoints(false);
	return true;
}

void CNItem::postResize()
{
	updateAttachedPositioning();
}

void CNItem::setVisible( bool yes )
{
	if (b_deleted) {
		Item::setVisible(false);
		return;
	}

	Item::setVisible(yes);

	const TextMap::iterator textMapEnd = m_textMap.end();
	for(TextMap::iterator it = m_textMap.begin(); it != textMapEnd; ++it) {
		it.data()->setVisible(yes);
	}

	const NodeMap::iterator nodeMapEnd = m_nodeMap.end();
	for(NodeMap::iterator it = m_nodeMap.begin(); it != nodeMapEnd; ++it) {
		it.data().node->setVisible(yes);
	}

	CNItem::setDrawWidgets(yes);

	if (!yes)
		updateConnectorPoints(false);
}

void CNItem::setInitialPos( const QPoint &pos )
{
	m_offset = pos - QPoint( (int)x(), (int)y() );
}

void CNItem::reparented( Item *oldParent, Item *newParent )
{
	Item::reparented( oldParent, newParent );
	updateNodeLevels();
}

void CNItem::updateNodeLevels()
{
	int l = level();
	
	// Tell our nodes about our level
	const NodeMap::iterator nodeMapEnd = m_nodeMap.end();
	for ( NodeMap::iterator it = m_nodeMap.begin(); it != nodeMapEnd; ++it )
	{
		it.data().node->setLevel(l);
	}
	
	const ItemList::iterator end = m_children.end();
	for ( ItemList::iterator it = m_children.begin(); it != end; ++it )
	{
		if ( CNItem *cnItem = dynamic_cast<CNItem*>((Item*)*it) )
			cnItem->updateNodeLevels();
	}
}

ConnectorList CNItem::connectorList()
{
	ConnectorList list;

	const NodeMap::iterator nodeMapEnd = m_nodeMap.end();
	for ( NodeMap::iterator it = m_nodeMap.begin(); it != nodeMapEnd; ++it )
	{
		Node *node = p_icnDocument->nodeWithID(it.data().id);
		if (node)
		{
			ConnectorList nodeList = node->inputConnectorList();
			ConnectorList::iterator end = nodeList.end();
			for ( ConnectorList::iterator it = nodeList.begin(); it != end; ++it )
			{
				if ( *it && !list.contains(*it) )
				{
					list.append(*it);
				}
			}
			nodeList = node->outputConnectorList();
			end = nodeList.end();
			for ( ConnectorList::iterator it = nodeList.begin(); it != end; ++it )
			{
				if ( *it && !list.contains(*it) )
				{
					list.append(*it);
				}
			}
		}
	}

	return list;
}

void CNItem::removeItem()
{
	if (b_deleted)
		return;
	
	const TextMap::iterator textMapEnd = m_textMap.end();
	for ( TextMap::iterator it = m_textMap.begin(); it != textMapEnd; ++it )
		it.data()->setCanvas(0);
	
	Item::removeItem();
	updateConnectorPoints(false);
}

void CNItem::restoreFromItemData( const ItemData &itemData )
{
	Item::restoreFromItemData(itemData);
	
	updateConnectorPoints(false);
	
	{
		const BoolMap::const_iterator end = itemData.buttonMap.end();
		for ( BoolMap::const_iterator it = itemData.buttonMap.begin(); it != end; ++it )
		{
			Button *b = button(it.key());
			if (b)
				b->setState(it.data());
		}
	}
	{
		const IntMap::const_iterator end = itemData.sliderMap.end();
		for ( IntMap::const_iterator it = itemData.sliderMap.begin(); it != end; ++it )
		{
			Slider *s = slider(it.key());
			if (s)
				s->setValue(it.data());
		}
	}
}

ItemData CNItem::itemData() const
{
	ItemData itemData = Item::itemData();
	
	const WidgetMap::const_iterator end = m_widgetMap.end();
	for ( WidgetMap::const_iterator it = m_widgetMap.begin(); it != end; ++it )
	{
		if ( Slider *slider = dynamic_cast<Slider*>(*it) )
			itemData.sliderMap[slider->id()] = slider->value();
		
		else if ( Button *button = dynamic_cast<Button*>(*it) )
			itemData.buttonMap[button->id()] = button->state();
		
	}
	
	return itemData;
}

Node* CNItem::createNode( double _x, double _y, int orientation, const QString &name, uint type )
{
	orientation %= 360;
	if(orientation < 0 ) orientation += 360;

	Node::node_dir dir;

	if	(orientation ==   0 ) dir = Node::dir_right;
	else if (orientation ==  90 ) dir = Node::dir_down;
	else if (orientation == 180 ) dir = Node::dir_left;
	else if (orientation == 270 ) dir = Node::dir_up;
	else {
		kdError() << k_funcinfo << "Unknown orientation: " << orientation << endl;
		return 0;
	}
	
	Node *node;
	if ( (type == Node::ec_pin) || (type == Node::ec_junction) )
	{
		node = new ECNode( p_icnDocument, Node::node_type(type), dir, QPoint( 0, 0 ) );
	} else {
		node = new FPNode( p_icnDocument, Node::node_type(type), dir, QPoint( 0, 0 ) );
	}

	node->setLevel( level() );
	
	node->setParentItem(this);
	node->setChildId(name);
	
	NodeInfo info;
	info.id = node->id();
	info.node = node;
	info.x = _x;
	info.y = _y;
	info.orientation = orientation;
	
	m_nodeMap[name] = info;
	
	updateAttachedPositioning();
	
	return node;
}

bool CNItem::removeNode( const QString &name )
{
	NodeMap::iterator it = m_nodeMap.find(name);
	if ( it == m_nodeMap.end() ) {
		return false;
	}
	it.data().node->removeNode();
	p_icnDocument->flushDeleteList();
	m_nodeMap.erase(it);
	return true;
}

Node *CNItem::getClosestNode( const QPoint &pos )
{
	// Work through the nodes, finding the one closest to the (x, y) position
	Node *shortestNode = 0;
	double shortestDistance = 1e10; // Nice large distance :-)
	
	const NodeMap::iterator end = m_nodeMap.end();
	for ( NodeMap::iterator it = m_nodeMap.begin(); it != end; ++it )
	{
		Node *node = p_icnDocument->nodeWithID(it.data().id);
		if (node)
		{
			// Calculate the distance
			// Yeah, it's actually the distance squared; but it's all relative, so doesn't matter
			double distance = std::pow(node->x()-pos.x(),2) + std::pow(node->y()-pos.y(),2);
			
			if ( distance < shortestDistance ) {
				shortestDistance = distance;
				shortestNode = node;
			}
		}
	}
	
	return shortestNode;
}

void CNItem::updateAttachedPositioning()
{
	// Actually, we don't do anything anymore...
}

void CNItem::updateZ( int baseZ )
{
	Item::updateZ(baseZ);
	double _z = z();
	
	const NodeMap::iterator nodeMapEnd = m_nodeMap.end();
	for ( NodeMap::iterator it = m_nodeMap.begin(); it != nodeMapEnd; ++it )
		it.data().node->setZ( _z + 0.5 );
	
	const WidgetMap::iterator widgetMapEnd = m_widgetMap.end();
	for ( WidgetMap::iterator it = m_widgetMap.begin(); it != widgetMapEnd; ++it )
		it.data()->setZ( _z + 0.5 );
	
	const TextMap::iterator textMapEnd = m_textMap.end();
	for ( TextMap::iterator it = m_textMap.begin(); it != textMapEnd; ++it )
		it.data()->setZ( _z + 0.5 );
}

void CNItem::snap( int newx, int newy )
{
	// Ugly looking thing
	// Basically means: Move item to the new position of newx-offsetx and then snap it to the 8-square-side grid
	// This is in one move item call so that any attached connectors are only called once to update their routes.
	moveBy( 4+newx-m_offset.x()-x()-(int)(newx-m_offset.x())%8, 4+newy-m_offset.y()-y()-(int)(newy-m_offset.y())%8 );
}

void CNItem::moveBy( double dx, double dy )
{
	if ( dx == 0 && dy == 0 ) return;
	updateConnectorPoints(false);
	Item::moveBy( dx, dy );
	
	setWidgetsPos( QPoint( int(x()), int(y()) ) );
}

bool CNItem::mousePressEvent( const EventInfo &info )
{
	bool accepted = Item::mousePressEvent(info);
	if (!accepted) accepted = CIWidgetMgr::mousePressEvent(info);
	if (accepted) setChanged();
	return accepted;
}

bool CNItem::mouseReleaseEvent( const EventInfo &info )
{
	bool accepted = Item::mouseReleaseEvent(info);

	if (!accepted) accepted = CIWidgetMgr::mouseReleaseEvent(info);
	if (accepted) setChanged();

	return accepted;
}

bool CNItem::mouseDoubleClickEvent( const EventInfo &info )
{
	bool accepted = Item::mouseDoubleClickEvent(info);
	if (!accepted)
		accepted = CIWidgetMgr::mouseDoubleClickEvent(info);
	if (accepted)
		setChanged();
	return accepted;
}

bool CNItem::mouseMoveEvent( const EventInfo &info )
{
	bool accepted = Item::mouseMoveEvent(info);
	if (!accepted) accepted = CIWidgetMgr::mouseMoveEvent(info);
	if (accepted) setChanged();
	return accepted;
}

bool CNItem::wheelEvent( const EventInfo &info )
{
	bool accepted = Item::wheelEvent(info);
	if (!accepted) accepted = CIWidgetMgr::wheelEvent(info);
	if (accepted) setChanged();
	return accepted;
}

void CNItem::enterEvent()
{
	Item::enterEvent();
	CIWidgetMgr::enterEvent();
	setChanged();
}

void CNItem::leaveEvent()
{
	Item::leaveEvent();
	CIWidgetMgr::leaveEvent();
	setChanged();
}

void CNItem::drawShape( QPainter &p )
{
	if(!isVisible()) return;

// 	initPainter(p);
	if(isSelected()) p.setPen(m_selectedCol);

	p.drawPolygon(areaPoints());
	p.drawPolyline(areaPoints());
// 	deinitPainter(p);
}

void CNItem::initPainter( QPainter &p )
{
	if(isSelected()) p.setPen(m_selectedCol);
}

void CNItem::updateConnectorPoints( bool add )
{
	if(b_deleted || !isVisible()) add = false;

	if(b_pointsAdded == add) return;

	b_pointsAdded = add;

	Cells *cells = p_icnDocument->cells();
	if (!cells) return;

	const int cx = cells->width();
	const int cy = cells->height();

	if ( cx < 1 || cy < 1 ) return;

	// Get translation matrix
	// Hackish...
	QWMatrix m;
	if ( Component *c = dynamic_cast<Component*>(this) )
		m = c->transMatrix( c->angleDegrees(), c->flipped(), int(x()), int(y()), false );
	
	// Convention used here: _UM = unmapped by both matrix and cell reference, _M = mapped

	const QPoint start_UM = QPoint( int(x()+offsetX())-cellSize, int(y()+offsetY())-cellSize );
	const QPoint end_UM = start_UM + QPoint( width()+2*cellSize, height()+2*cellSize );
	
	const QPoint start_M = m.map(start_UM)/cellSize;
	const QPoint end_M = m.map(end_UM)/cellSize;

	int sx_M = start_M.x();
	int ex_M = end_M.x();

	int sy_M = start_M.y();
	int ey_M = end_M.y();

	// Normalise start and end points
	if ( sx_M > ex_M ) {
		const int temp = sx_M;
		sx_M = ex_M;
		ex_M = temp;
	}

	if ( sy_M > ey_M ) {
		const int temp = sy_M;
		sy_M = ey_M;
		ey_M = temp;
	}

	ex_M++;
	ey_M++;

	const int mult = add ? 1 : -1;

	for ( int x = sx_M; x < ex_M; x++ )
	{
		for ( int y = sy_M; y < ey_M; y++ )
		{
			if ( p_icnDocument->isValidCellReference( x, y ) )
			{
				if ( x != sx_M && y != sy_M && x != (ex_M-1) && y != (ey_M-1) )
				{
					(*cells)[x][y].CIpenalty += mult*ICNDocument::hs_item;
				} else {
//				(*cells)[x][y].CIpenalty += mult*ICNDocument::hs_item/2;
					(*cells)[x][y].CIpenalty += mult*ICNDocument::hs_connector*5;
				}
			}
		}
	}
	
#if 0
	// And subtract the positions of the node on the border
	NodeMap::iterator end = m_nodeMap.end();
	for ( NodeMap::iterator it = m_nodeMap.begin(); it != end; ++it )
	{
		const int x = (int)((it->second.node->x()-4)/cellSize);
		const int y = (int)((it->second.node->y()-4)/cellSize);
		if ( p_icnDocument->isValidCellReference(x,y) ) {
			(*cells)[x][y].CIpenalty -= mult*ICNDocument::hs_connector*5;
		}
	}
#endif
	
	const TextMap::iterator textMapEnd = m_textMap.end();
	for ( TextMap::iterator it = m_textMap.begin(); it != textMapEnd; ++it )
	{
		it.data()->updateConnectorPoints(add);
	}

	const WidgetMap::iterator widgetMapEnd = m_widgetMap.end();
	for ( WidgetMap::iterator it = m_widgetMap.begin(); it != widgetMapEnd; ++it )
	{
		it.data()->updateConnectorPoints(add);
	}
}

Text* CNItem::addDisplayText( const QString &id, const QRect & pos, const QString &display, bool internal, int flags )
{
	Text *text = 0;
	TextMap::iterator it = m_textMap.find(id);
	if ( it != m_textMap.end() )
	{
// 		kdWarning() << "CNItem::addDisplayText: removing old text"<<endl;
		delete it.data();
		m_textMap.remove(it);
	}
	
	text = new Text( "", this, pos, canvas(), flags );
	text->setZ( z()+(internal?0.1:-0.1) );
	
	m_textMap[id] = text;
	
	// Calculate the correct size
	setDisplayText( id, display );
	text->show();
	return text;
}

void CNItem::setDisplayText( const QString &id, const QString &display )
{
	TextMap::iterator it = m_textMap.find(id);
	if ( it == m_textMap.end() )
	{
		kdError() << "CNItem::setDisplayText: Could not find text with id \""<<id<<"\""<<endl;
		return;
	}
	it.data()->setText(display);
	updateAttachedPositioning();
}

void CNItem::removeDisplayText( const QString &id )
{
	TextMap::iterator it = m_textMap.find(id);
	if ( it == m_textMap.end() ) {
// 		kdError() << "CNItem::removeDisplayText: Could not find text with id \""<<id<<"\""<<endl;
		return;
	}

	it.data()->updateConnectorPoints(false);
	delete it.data();
	m_textMap.remove(it);
}

QString CNItem::nodeId( const QString &internalNodeId )
{
	NodeMap::iterator it = m_nodeMap.find(internalNodeId);
	if ( it == m_nodeMap.end() ) return "";
	else return it.data().id;
}

Node *CNItem::childNode( const QString &childId )
{
	return p_icnDocument->nodeWithID( nodeId(childId) );
}

NodeInfo::NodeInfo()
{
	node = 0;
	x = 0.;
	y = 0.;
	orientation = 0;
}

#include "cnitem.moc"
