/***************************************************************************
 *   Copyright (C) 2005 by David Saxton                                    *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "libraryitem.h"
#include "logic.h"
#include "matrixdisplaydriver.h"

#include <klocale.h>
#include <qpainter.h>
#include <qstring.h>

#include <cassert>

// Thank you Scott Dattalo!
// http://www.dattalo.com/gnupic/lcdfont.inc

//  code; decimal; hex; codepage 437
// not all codes implemented
// After further investigation, the higher codes correspond to japanese katakana
// letters. this is getting intesting.

// TODO: identify the code page that is actually implemented then make sure it is
// correct or fill it in with page 437 codes.
// Otherwise, implement multiple codepages and let user select at runtime. 

static char characterMap[256][5] = {
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //0	0x00	NULL
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //1	0x01  [white happy face]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //2	0x02  [black happy face]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //3	0x03  [heart]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //4	0x04  [daimond] 
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //5	0x05  [club]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //6	0x06  [spade] 
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //7	0x07  [dot]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //8	0x08  [inverted dot]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //9	0x09  [circle] 
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //10	0x0A  [inverted circle]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //11	0x0B  [mars symbol]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //12	0x0C  [venus symbol]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //13	0x0D  [note]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //14	0x0E  [double note]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //15	0x0F  [sun]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //16	0x10  [play/ right pointing triangle]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //17	0x11  [left pointing triangle]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //18	0x12  [up-down arrow]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //19	0x13	!!
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //20	0x14  [paragraph symbol]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //21	0x15  [chapter symbol]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //22	0x16  [bar]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //23	0x17  [up-down arrow with underline]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //24	0x18  [up arrow]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //25	0x19  [down arrow]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //26	0x1A  [right arrow]	
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //27	0x1B  [left arrow]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //28	0x1C
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //29	0x1D  [left-right arrow]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //30	0x1E  [up triangle]	
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //31	0x1F  [down triangle]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //32	0x20  [space]
{ 0x00, 0x00, 0x7d, 0x00, 0x00 }, //33	0x21	!
{ 0x00, 0x70, 0x00, 0x70, 0x00 }, //34	0x22	"
{ 0x14, 0x7f, 0x14, 0x7f, 0x14 }, //35	0x23	#
{ 0x12, 0x2a, 0x7f, 0x2a, 0x24 }, //36	0x24	$
{ 0x62, 0x64, 0x08, 0x13, 0x23 }, //37	0x25	%
{ 0x36, 0x49, 0x55, 0x22, 0x05 }, //38	0x26	&
{ 0x00, 0x50, 0x60, 0x00, 0x00 }, //39	0x27	'
{ 0x00, 0x1c, 0x22, 0x41, 0x00 }, //40	0x28	(
{ 0x00, 0x41, 0x22, 0x1c, 0x00 }, //41	0x29	)
{ 0x14, 0x08, 0x3e, 0x08, 0x14 }, //42	0x2A	*
{ 0x08, 0x08, 0x3e, 0x08, 0x08 }, //43	0x2B	+
{ 0x00, 0x05, 0x06, 0x00, 0x00 }, //44	0x2C	,
{ 0x08, 0x08, 0x08, 0x08, 0x08 }, //45	0x2D	-
{ 0x00, 0x03, 0x03, 0x00, 0x00 }, //46	0x2E	.
{ 0x02, 0x04, 0x08, 0x10, 0x20 }, //47	0x2F	/
{ 0x3e, 0x45, 0x49, 0x51, 0x3e }, //48	0x30	0
{ 0x00, 0x21, 0x7f, 0x01, 0x00 }, //49	0x31	1
{ 0x21, 0x43, 0x45, 0x49, 0x31 }, //50	0x32	2
{ 0x42, 0x41, 0x51, 0x69, 0x46 }, //51	0x33	3
{ 0x0c, 0x14, 0x24, 0x7f, 0x04 }, //52	0x34	4
{ 0x72, 0x51, 0x51, 0x51, 0x4e }, //53	0x35	5
{ 0x1e, 0x29, 0x49, 0x49, 0x06 }, //54	0x36	6
{ 0x40, 0x47, 0x48, 0x50, 0x60 }, //55	0x37	7
{ 0x36, 0x49, 0x49, 0x49, 0x36 }, //56	0x38	8
{ 0x30, 0x49, 0x49, 0x4a, 0x3c }, //57	0x39	9
{ 0x00, 0x36, 0x36, 0x00, 0x00 }, //58	0x3A	:
{ 0x00, 0x35, 0x36, 0x00, 0x00 }, //59	0x3B	;
{ 0x08, 0x14, 0x22, 0x41, 0x00 }, //60	0x3C	<
{ 0x14, 0x14, 0x14, 0x14, 0x14 }, //61	0x3D	=
{ 0x41, 0x22, 0x14, 0x08, 0x00 }, //62	0x3E	>
{ 0x20, 0x40, 0x45, 0x48, 0x30 }, //63	0x3F	?
{ 0x26, 0x49, 0x4f, 0x41, 0x3e }, //64	0x40	@
{ 0x3f, 0x44, 0x44, 0x44, 0x3f }, //65	0x41	A
{ 0x7f, 0x49, 0x49, 0x49, 0x36 }, //66	0x42	B
{ 0x3e, 0x41, 0x41, 0x41, 0x22 }, //67	0x43	C
{ 0x7f, 0x41, 0x41, 0x41, 0x3e }, //68	0x44	D
{ 0x7f, 0x49, 0x49, 0x49, 0x41 }, //69	0x45	E
{ 0x7f, 0x48, 0x48, 0x48, 0x40 }, //70	0x46	F
{ 0x3e, 0x41, 0x49, 0x49, 0x2f }, //71	0x47	G
{ 0x7f, 0x08, 0x08, 0x08, 0x7f }, //72	0x48	H
{ 0x00, 0x41, 0x7f, 0x41, 0x00 }, //73	0x49	I
{ 0x02, 0x01, 0x41, 0x7e, 0x40 }, //74	0x4A	J
{ 0x7f, 0x08, 0x14, 0x22, 0x41 }, //75	0x4B	K
{ 0x7f, 0x01, 0x01, 0x01, 0x01 }, //76	0x4C	L
{ 0x7f, 0x40, 0x20, 0x40, 0x7f }, //77	0x4D	M
{ 0x7f, 0x10, 0x08, 0x04, 0x7f }, //78	0x4E	N
{ 0x3e, 0x41, 0x41, 0x41, 0x3e }, //79	0x4F	O
{ 0x7f, 0x48, 0x48, 0x48, 0x30 }, //80	0x50	P
{ 0x3e, 0x41, 0x45, 0x42, 0x3d }, //81	0x51	Q
{ 0x7f, 0x48, 0x4c, 0x4a, 0x31 }, //82	0x52	R
{ 0x31, 0x49, 0x49, 0x49, 0x46 }, //83	0x53	S
{ 0x40, 0x40, 0x7f, 0x40, 0x40 }, //84	0x54	T
{ 0x7e, 0x01, 0x01, 0x01, 0x7e }, //85	0x55	U
{ 0x7c, 0x02, 0x01, 0x02, 0x7c }, //86	0x56	V
{ 0x7e, 0x01, 0x0e, 0x01, 0x7e }, //87	0x57	W
{ 0x63, 0x14, 0x08, 0x14, 0x63 }, //88	0x58	X
{ 0x70, 0x08, 0x07, 0x08, 0x70 }, //89	0x59	Y
{ 0x43, 0x45, 0x49, 0x51, 0x61 }, //90	0x5A	Z
{ 0x00, 0x7f, 0x41, 0x41, 0x00 }, //91	0x5B	[
{ 0x54, 0x34, 0x1f, 0x34, 0x54 }, //92	0x5C	\
{ 0x00, 0x41, 0x41, 0x7f, 0x00 }, //93	0x5D	]
{ 0x10, 0x20, 0x40, 0x20, 0x10 }, //94	0x5E	^
{ 0x01, 0x01, 0x01, 0x01, 0x01 }, //95	0x5F	`
{ 0x00, 0x40, 0x20, 0x10, 0x00 }, //96	0x60	a
{ 0x02, 0x15, 0x15, 0x15, 0x0f }, //97	0x61	b
{ 0x7f, 0x09, 0x11, 0x11, 0x0e }, //98	0x62	c
{ 0x0e, 0x11, 0x11, 0x11, 0x02 }, //99	0x63	d
{ 0x0e, 0x11, 0x11, 0x09, 0x7f }, //100	0x64	e
{ 0x0e, 0x15, 0x15, 0x15, 0x0c }, //101	0x65	f
{ 0x08, 0x3f, 0x48, 0x40, 0x20 }, //102	0x66	g
{ 0x30, 0x49, 0x49, 0x49, 0x7e }, //103	0x67	h
{ 0x7f, 0x08, 0x10, 0x10, 0x0f }, //104	0x68	i
{ 0x00, 0x11, 0x5f, 0x01, 0x00 }, //105	0x69	j
{ 0x02, 0x01, 0x21, 0x7e, 0x00 }, //106	0x6A	k
{ 0x7f, 0x04, 0x0a, 0x11, 0x00 }, //107	0x6B	l
{ 0x00, 0x41, 0x7f, 0x01, 0x00 }, //108	0x6C	m
{ 0x1f, 0x10, 0x0c, 0x10, 0x0f }, //109	0x6D	n
{ 0x1f, 0x08, 0x10, 0x10, 0x0f }, //110	0x6E	o
{ 0x0e, 0x11, 0x11, 0x11, 0x0e }, //111	0x6F	p
{ 0x1f, 0x14, 0x14, 0x14, 0x08 }, //112	0x70	q
{ 0x08, 0x14, 0x14, 0x0c, 0x1f }, //113	0x71	r
{ 0x1f, 0x08, 0x10, 0x10, 0x08 }, //114	0x72	s
{ 0x09, 0x15, 0x15, 0x15, 0x12 }, //115	0x73	t
{ 0x20, 0x7e, 0x21, 0x01, 0x02 }, //116	0x74	u
{ 0x1e, 0x01, 0x01, 0x02, 0x1f }, //117	0x75	v
{ 0x1c, 0x02, 0x01, 0x02, 0x1c }, //118	0x76	w
{ 0x1e, 0x01, 0x06, 0x01, 0x1e }, //119	0x77	x
{ 0x11, 0x0a, 0x04, 0x0a, 0x11 }, //120	0x78	y
{ 0x18, 0x05, 0x05, 0x05, 0x1e }, //121	0x79	z
{ 0x11, 0x13, 0x15, 0x19, 0x11 }, //122	0x7A	{
{ 0x00, 0x08, 0x36, 0x41, 0x00 }, //123	0x7B	|
{ 0x00, 0x00, 0x7f, 0x00, 0x00 }, //124	0x7C	}
{ 0x00, 0x41, 0x36, 0x08, 0x00 }, //125	0x7D	~
{ 0x08, 0x08, 0x2a, 0x1c, 0x08 }, //126	0x7E
{ 0x08, 0x1c, 0x2a, 0x08, 0x08 }, //127	0x7F
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //128	0x80
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //129	0x81
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //130	0x82
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //131	0x83
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //132	0x84
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //133	0x85
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //134	0x86
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //135	0x87
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //136	0x88
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //137	0x89
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //138	0x8A
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //139	0x8B
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //140	0x8C
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //141	0x8D
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //142	0x8E
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //143	0x8F
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //144	0x90
{ 0x07, 0x05, 0x07, 0x00, 0x00 }, //145	0x91	ae
{ 0x00, 0x00, 0x78, 0x40, 0x40 }, //146	0x92	AE
{ 0x01, 0x01, 0x0f, 0x00, 0x00 }, //147	0x93
{ 0x04, 0x02, 0x01, 0x00, 0x00 }, //148	0x94
{ 0x00, 0x0c, 0x0c, 0x00, 0x00 }, //149	0x95
{ 0x28, 0x28, 0x29, 0x2a, 0x3c }, //150	0x96
{ 0x10, 0x11, 0x16, 0x14, 0x18 }, //151	0x97
{ 0x02, 0x04, 0x0f, 0x10, 0x00 }, //152	0x98
{ 0x0c, 0x08, 0x19, 0x09, 0x0e }, //153	0x99
{ 0x09, 0x09, 0x0f, 0x09, 0x09 }, //154	0x9A
{ 0x09, 0x0a, 0x0c, 0x1f, 0x08 }, //155	0x9B	[cents]
{ 0x08, 0x1f, 0x08, 0x0a, 0x0c }, //156	0x9C	[pounds]
{ 0x01, 0x09, 0x09, 0x0f, 0x01 }, //157	0x9D	[yen]
{ 0x15, 0x15, 0x15, 0x1f, 0x00 }, //158	0x9E	
{ 0x0c, 0x00, 0x0d, 0x01, 0x0e }, //159	0x9F	[function]
{ 0x04, 0x04, 0x04, 0x04, 0x04 }, //160	0xA0	
{ 0x40, 0x41, 0x5e, 0x48, 0x70 }, //161	0xA1
{ 0x04, 0x08, 0x1f, 0x20, 0x40 }, //162	0xA2
{ 0x38, 0x20, 0x61, 0x22, 0x3c }, //163	0xA3
{ 0x11, 0x11, 0x1f, 0x11, 0x11 }, //164	0xA4
{ 0x22, 0x24, 0x28, 0x7f, 0x20 }, //165	0xA5
{ 0x21, 0x7e, 0x20, 0x21, 0x3e }, //166	0xA6
{ 0x28, 0x28, 0x7f, 0x28, 0x28 }, //167	0xA7
{ 0x08, 0x31, 0x21, 0x22, 0x3c }, //168	0xA8	upside down ?
{ 0x10, 0x60, 0x21, 0x3e, 0x20 }, //169	0xA9
{ 0x21, 0x21, 0x21, 0x21, 0x3f }, //170	0xAA
{ 0x20, 0x79, 0x22, 0x7c, 0x20 }, //171	0xAB	1/2
{ 0x29, 0x29, 0x01, 0x02, 0x1c }, //172	0xAC	1/4
{ 0x21, 0x22, 0x24, 0x2a, 0x31 }, //173	0xAD	
{ 0x20, 0x7e, 0x21, 0x29, 0x31 }, //174	0xAE	<<
{ 0x30, 0x09, 0x01, 0x02, 0x3c }, //175	0xAF	>>
{ 0x08, 0x31, 0x29, 0x26, 0x3c }, //176	0xB0
{ 0x28, 0x29, 0x3e, 0x48, 0x08 }, //177	0xB1
{ 0x30, 0x00, 0x31, 0x02, 0x3c }, //178	0xB2
{ 0x10, 0x51, 0x5e, 0x50, 0x10 }, //179	0xB3
{ 0x00, 0x7f, 0x08, 0x04, 0x00 }, //180	0xB4
{ 0x11, 0x12, 0x7c, 0x10, 0x10 }, //181	0xB5
{ 0x01, 0x21, 0x21, 0x21, 0x01 }, //182	0xB6
{ 0x21, 0x2a, 0x24, 0x2a, 0x30 }, //183	0xB7
{ 0x22, 0x24, 0x6f, 0x34, 0x22 }, //184	0xB8
{ 0x00, 0x01, 0x02, 0x7c, 0x00 }, //185	0xB9
{ 0x0f, 0x00, 0x20, 0x10, 0x0f }, //186	0xBA
{ 0x7e, 0x11, 0x11, 0x11, 0x11 }, //187	0xBB
{ 0x20, 0x21, 0x21, 0x22, 0x3c }, //188	0xBC
{ 0x10, 0x20, 0x10, 0x08, 0x06 }, //189	0xBD
{ 0x26, 0x20, 0x7f, 0x20, 0x26 }, //190	0xBE
{ 0x20, 0x24, 0x22, 0x25, 0x38 }, //191	0xBF
{ 0x00, 0x2a, 0x2a, 0x2a, 0x01 }, //192	0xC0
{ 0x0e, 0x12, 0x22, 0x02, 0x07 }, //193	0xC1
{ 0x01, 0x0a, 0x04, 0x0a, 0x30 }, //194	0xC2
{ 0x28, 0x3e, 0x29, 0x29, 0x29 }, //195	0xC3
{ 0x10, 0x7f, 0x10, 0x14, 0x18 }, //196	0xC4
{ 0x01, 0x21, 0x21, 0x3f, 0x01 }, //197	0xC5
{ 0x29, 0x29, 0x29, 0x29, 0x3f }, //198	0xC6
{ 0x10, 0x50, 0x51, 0x52, 0x1c }, //199	0xC7
{ 0x78, 0x01, 0x02, 0x7c, 0x00 }, //200	0xC8
{ 0x1f, 0x00, 0x3f, 0x01, 0x06 }, //201	0xC9
{ 0x3f, 0x01, 0x02, 0x04, 0x08 }, //202	0xCA
{ 0x3f, 0x21, 0x21, 0x21, 0x3f }, //203	0xCB
{ 0x38, 0x20, 0x21, 0x22, 0x3c }, //204	0xCC
{ 0x21, 0x21, 0x01, 0x02, 0x0c }, //205	0xCD
{ 0x20, 0x10, 0x40, 0x20, 0x00 }, //206	0xCE
{ 0x70, 0x50, 0x70, 0x00, 0x00 }, //207	0xCF
{ 0x0e, 0x11, 0x09, 0x06, 0x19 }, //208	0xD0
{ 0x02, 0x55, 0x15, 0x55, 0x0f }, //209	0xD1
{ 0x1f, 0x2a, 0x2a, 0x2a, 0x14 }, //210	0xD2
{ 0x0a, 0x15, 0x15, 0x11, 0x02 }, //211	0xD3
{ 0x3f, 0x02, 0x02, 0x04, 0x3e }, //212	0xD4
{ 0x0e, 0x11, 0x19, 0x15, 0x12 }, //213	0xD5
{ 0x0f, 0x12, 0x22, 0x22, 0x1c }, //214	0xD6
{ 0x1c, 0x22, 0x22, 0x22, 0x3f }, //215	0xD7
{ 0x02, 0x01, 0x1e, 0x10, 0x10 }, //216	0xD8
{ 0x20, 0x20, 0x00, 0x70, 0x00 }, //217	0xD9
{ 0x00, 0x00, 0x10, 0x5f, 0x00 }, //218	0xDA
{ 0x28, 0x10, 0x28, 0x00, 0x00 }, //219	0xDB
{ 0x18, 0x24, 0x7e, 0x24, 0x08 }, //220	0xDC
{ 0x14, 0x7f, 0x15, 0x01, 0x01 }, //221	0xDD
{ 0x1f, 0x48, 0x50, 0x50, 0x0f }, //222	0xDE
{ 0x0e, 0x51, 0x11, 0x51, 0x0e }, //223	0xDF
{ 0x3f, 0x12, 0x22, 0x22, 0x1c }, //224	0xE0
{ 0x1c, 0x22, 0x22, 0x12, 0x3f }, //225	0xE1	[calculus beta]
{ 0x3c, 0x52, 0x52, 0x52, 0x3c }, //226	0xE2	[upper case gamma]
{ 0x03, 0x05, 0x02, 0x05, 0x06 }, //227	0xE3	[pi]
{ 0x1a, 0x26, 0x20, 0x26, 0x1a }, //228	0xE4	[upper case sigma]
{ 0x1e, 0x41, 0x01, 0x42, 0x1f }, //229	0xE5	[lower case sigma]
{ 0x63, 0x55, 0x49, 0x41, 0x41 }, //230	0xE6	[mu]
{ 0x22, 0x3c, 0x20, 0x3e, 0x22 }, //231	0xE7	[lower case gamma]
{ 0x51, 0x4a, 0x44, 0x4a, 0x51 }, //232	0xE8	[phi]
{ 0x3c, 0x02, 0x02, 0x02, 0x3f }, //233	0xE9	[theta]
{ 0x28, 0x28, 0x3e, 0x28, 0x48 }, //234	0xEA	[omega]
{ 0x22, 0x3c, 0x28, 0x28, 0x2e }, //235	0xEB	[delta]
{ 0x3e, 0x28, 0x38, 0x28, 0x3e }, //236	0xEC	[infinity]
{ 0x04, 0x04, 0x15, 0x04, 0x04 }, //237	0xED	[null]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //238	0xEE	[set member of]
{ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f }, //239	0xEF	[set intersection]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //240	0xF0	[equivalent]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //241	0xF1	[pluss minus]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //242	0xF2	>_
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //243	0xF3	<_
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //244	0xF4	upper part of integral sign.
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //245	0xF5	lower part of integral sign.
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //246	0xF6	[divide by sign] 
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //247	0xF7	[similar to]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //248	0xF8	[degrees]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //249	0xF9	[sqrt]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //250	0xFA	[superscript n]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //251	0xFB	[squared]
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //252	0xFC	
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //253	0xFD
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, //254	0xFE
{ 0x00, 0x00, 0x00, 0x00, 0x00 } //255	0xFF
};

inline static bool displayBit(unsigned value, unsigned row, unsigned column ) {
	assert( value < 256 );
	assert( row < 7 );
	assert( column < 5 );
	return characterMap[value][column] & (1 << row);
}

Item* MatrixDisplayDriver::construct(ItemDocument *itemDocument, bool newItem, const char *id )
{
	return new MatrixDisplayDriver( (ICNDocument*)itemDocument, newItem, id );
}

LibraryItem *MatrixDisplayDriver::libraryItem()
{
	return new LibraryItem(
		"ec/matrix_display_driver",
		i18n("Matrix Display Driver"),
		i18n("Integrated Circuits"),
		"ic2.png",
		LibraryItem::lit_component,
		MatrixDisplayDriver::construct );
}

MatrixDisplayDriver::MatrixDisplayDriver( ICNDocument *icnDocument, bool newItem, const char *id )
	: Component( icnDocument, newItem, id ? id : "Matrix Display Driver" )
{
	m_name = i18n("Matrix Display Driver");

	m_prevCol = 0;
	m_nextCol = 0;
	m_scanCount = 2;

	createProperty( "diode-configuration", Variant::Type::Select );
	property("diode-configuration")->setCaption( i18n("Configuration") );
	property("diode-configuration")->setAllowed( QStringList::split(',',"Row Cathode,Column Cathode") );
	property("diode-configuration")->setValue("Row Cathode");
	property("diode-configuration")->setAdvanced(true);

	QStringList pins = QStringList::split( ',', "D0,D1,D2,D3,D4,D5,D6,D7,,,,,,C4,C3,C2,C1,C0,,R0,R1,R2,R3,R4,R5,R6", true );
	initDIPSymbol( pins, 64 );
	initDIP(pins);

	m_pValueLogic.resize(8, 0);
	for ( unsigned i = 0; i < 8; ++i )
		m_pValueLogic[i] = createLogicIn( ecNodeWithID("D"+QString::number(i)) );

	m_pRowLogic.resize(7, 0);
	for ( unsigned i = 0; i < 7; ++i ) {
		m_pRowLogic[i] = createLogicOut( ecNodeWithID("R"+QString::number(i)), false );
		m_pRowLogic[i]->setOutputLowConductance( 1.0 );
		m_pRowLogic[i]->setOutputHighVoltage(5.0);
	}

	m_pColLogic.resize(5, 0);
	for ( unsigned i = 0; i < 5; ++i ) {
		m_pColLogic[i] = createLogicOut( ecNodeWithID("C"+QString::number(i)), false );
		m_pColLogic[i]->setOutputHighVoltage(5.0);
	}
}

MatrixDisplayDriver::~MatrixDisplayDriver()
{
}

void MatrixDisplayDriver::stepNonLogic()
{
	if(++m_scanCount < 5 ) return;

	m_scanCount = 0;

	m_pColLogic[m_prevCol]->setHigh(false);
	m_pColLogic[m_nextCol]->setHigh(true);

	unsigned value = 0;
	for(unsigned i = 0; i < 8; ++i )
		value |= ( m_pValueLogic[i]->isHigh() ) ? (1 << i) : 0;

	for(unsigned row = 0; row < 7; row++ ) {
		m_pRowLogic[row]->setHigh(!displayBit(value, row, m_nextCol));
	}

	m_prevCol = m_nextCol;

	m_nextCol++;
	if(m_nextCol >= 5 ) m_nextCol = 0;
}
