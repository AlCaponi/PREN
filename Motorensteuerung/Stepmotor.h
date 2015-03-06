/* 
 * File:   Stepmotor.h
 * Author: Sascha
 *
 * Eigenschaften des Motors
 * Black = A Motor coil A pin 1
 * Green = A- Motor coil A pin 2
 * Red = B Motor coil B pin 1
 * Blue = B- Motor coil B pin 2
 * 200 Steps 1 Umdrehung
 * 
 * Created on 6. März 2015, 08:56
 */

#ifndef STEPMOTOR_H
#define	STEPMOTOR_H

void walkLeft(int steps, bool fast);
void walkRight(int steps, bool fast);
double getDistance();
void fire();

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* STEPMOTOR_H */

