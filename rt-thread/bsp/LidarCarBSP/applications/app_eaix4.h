#ifndef __EAIX4_H__
#define __EAIX4_H__

#include <rtthread.h>
#include <rthw.h>

void Eaix4Scaning(void);
void Eaix4stop(void);
void Eaix4Version(void);
void Eaix4Health(void);

rt_err_t eaix4_open(const char *name);
rt_uint8_t eaix4_getchar(void);
void eaix4_putchar(const rt_uint8_t c);
void eaix4_putstring(const rt_uint8_t *s);

void eaix4_init(void);

#endif
