#ifndef __STATS_H
#define __STATS_H

typedef struct {
	unsigned int bytesSent;
	unsigned int bytesRecd;
	unsigned long bytesSent_total;
	unsigned long bytesRecd_total;
	int updateInterval;
	pthread_t updateThread;
} connxnstats_t;


connxnstats_t* stats_initialize(int updateInterval);
connxnstats_t* stats_initialize();
void stats_reportBytesSent(connxnstats_t* stats, unsigned int bytesSent);
void stats_reportBytesRecd(connxnstats_t* stats, unsigned int bytesRecd);
void stats_finalize(connxnstats_t* stats);
void stats_refresh(connxnstats_t* stats);
void stats_readifyNum(int num, char* readableNumStr);


#endif
