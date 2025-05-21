/*
 * delays.h
*
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */


#ifndef DELAYS_H_
#define DELAYS_H_

#ifdef __cplusplus
extern "C" {
#endif

void Delay_us(uint16_t us);

void Delay_us_new(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* DELAYS_H_ */
