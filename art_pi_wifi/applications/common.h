#ifndef _COMMON_H
#define _COMMON_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "common.h"

#define LED0_PIN  GET_PIN(A, 8)

extern void Led0_ON(void);
extern void Led0_OFF(void);



#endif