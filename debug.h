#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __ROSCO_M68K
#define debugf(...)
#else
#define debugf(...)     printf(__VA_ARGS__)
#endif

#endif