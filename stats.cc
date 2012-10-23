/* A module to track desired metrics in client server communication */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "stats.h"


#define DEFAULT_STATS_INTERVAL			2 //in seconds


// initialize counters and start stats refresh thread
connxnstats_t* stats_initialize(int updateInterval) {
	connxnstats_t* stats = (connxnstats_t*)malloc(sizeof(connxnstats_t));
	stats->bytesSent = 0;
	stats->bytesRecd = 0;
	stats->bytesSent_total = 0;
	stats->bytesRecd_total = 0;
	stats->updateInterval = updateInterval;
	pthread_create(&stats->updateThread, NULL, (void *(*)(void *))&stats_refresh, stats);

	return stats;
}

// initialize counters and start stats refresh thread
connxnstats_t* stats_initialize() {
	int updateInterval = DEFAULT_STATS_INTERVAL;
	return stats_initialize(updateInterval);
}

// called by client/server apps to report bytes sent; increments relevent counters
void stats_reportBytesSent(connxnstats_t* stats, unsigned int bytesSent) {
	stats->bytesSent += bytesSent;
	stats->bytesSent_total += bytesSent;
}

// called by client/server apps to report bytes received; increments relevent counters
void stats_reportBytesRecd(connxnstats_t* stats, unsigned int bytesRecd) {
	stats->bytesRecd += bytesRecd;
	stats->bytesRecd_total += bytesRecd;
}

// calculate and output cumulative and average stats on this connection
void stats_finalize(connxnstats_t* stats) {
	// stop stats update thread
	pthread_cancel(stats->updateThread);
	pthread_join(stats->updateThread, NULL);

	printf("\n------FINAL CONNECTION STATS------\n");
	char readableNumStr[2][100];
	stats_readifyNum(stats->bytesSent_total, readableNumStr[0]);
	stats_readifyNum(stats->bytesRecd_total, readableNumStr[1]);
	printf("TOTAL SENT = %sB, ", readableNumStr[0]);
	printf("TOTAL RECEIVED = %sB\n", readableNumStr[1]);
	printf("----------------------------------\n");

	free(stats);
}

// stats update runs as a thread, triggers every n sec and displays stats data
void stats_refresh(connxnstats_t* stats) {
	while (1) {
		//Periodically print stats
		int bw_up = stats->bytesSent * 8 / stats->updateInterval;
		int bw_dwn = stats->bytesRecd * 8 / stats->updateInterval;
		char readableNumStr[4][100];
		stats_readifyNum(stats->bytesSent_total, readableNumStr[0]);
		stats_readifyNum(bw_up, readableNumStr[1]);
		stats_readifyNum(stats->bytesRecd_total, readableNumStr[2]);
		stats_readifyNum(bw_dwn, readableNumStr[3]);
		printf("%sB sent (%sb/s), ", readableNumStr[0], readableNumStr[1]);
		printf("%sB received (%sb/s)\n", readableNumStr[2], readableNumStr[3]);
		stats->bytesSent = 0;
		stats->bytesRecd = 0;

		// refresh every n seconds. check for external stats termination
		sleep(stats->updateInterval);
		pthread_testcancel();
	}
}

// make numbers more readable by adding magnitude prefixes
void stats_readifyNum(int num, char* readableNumStr) {
	if (num < 1000) {
		sprintf(readableNumStr, "%d ", num);
	} else if (num >= 1000 && num < 1000000) {
		sprintf(readableNumStr, "%.2f K", (float)num/1000);
	} else if (num >= 1000000 && num < 1000000000) {
		sprintf(readableNumStr, "%.2f M", (float)num/1000000);
	} else {
		sprintf(readableNumStr, "%.2f G", (float)num/1000000000);
	}
}
