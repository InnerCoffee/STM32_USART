#ifndef _HMC830_H_
#define _HMC830_H_

#include "sys.h"


#define HMC830_CEN PBout(12)
#define HMC830_SEN PAout(8)
#define HMC830_SDI PBout(15)
#define HMC830_SCK PBout(14)
#define HMC830_LD  PBin(13)

void Hmc830Config(void);
void spi_wr(unsigned char dat,unsigned char *p);
void WriteAFre(unsigned long int fre);
void hmc830_ini(unsigned char *p);
void Hmc830FreWrite(u32 wfre);


#endif

