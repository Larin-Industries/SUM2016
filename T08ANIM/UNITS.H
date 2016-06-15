/* FILE NAME: UNITS.H
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#ifndef __UNITS_H_
#define __UNITS_H_
#include "ANIM.H"

/* Create units */
al5UNIT * AL5_UnitCreateCube( FLT x, FLT y, FLT z );
al5UNIT * AL5_UnitCreateBall( VOID );
al5UNIT * AL5_UnitCreateBallCont( VOID );

#endif /* __UNITS_H_ */