#include <stdio.h>
#define max 20
int numPkts,lkRate,bkCapacity,pkts[max];
void leakyBucket()
{
	int time=0;
	int bucket=0;
	int dropped;
	int leaked;
	int incoming;
	printf("Time\tIncoming\tBucket\tDropped\tLeaked\n");
	for(int i=0;i<numPkts;i++)
	{
		incoming=pkts[i];
		dropped=0;
		leaked=0;
		bucket+=pkts[i];
		if(bucket>bkCapacity)
		{
			dropped=bucket-bkCapacity;
		}
		bucket=bucket-dropped;
		printf("%d\t%d\t%d\t",time,incoming,bucket);
		if(bucket>lkRate)
		{
			bucket=bucket-lkRate;
			leaked=lkRate;
		}
		else
		{
			leaked=bucket;
			bucket=0;
		}
		printf("%d\t%d\n",dropped,leaked);
		time++;
	}
	while(bucket>0)
	{
		incoming=0;
		printf("%d\t%d\t%d\t",time,incoming,bucket);
		if(bucket>lkRate)
		{
			bucket=bucket-lkRate;
			leaked=lkRate;
		}
		else
		{
			leaked=bucket;
			bucket=0;
		}
		dropped=0;
		printf("%d\t%d\n",dropped,leaked);
		time++;
	}
}
int main()
{
	printf("Enter the number of packets\n");
	scanf("%d",&numPkts);
	printf("Enter the leak rate");
	scanf("%d",&lkRate);
	printf("Enter bucket Capacity\n");
	scanf("%d",&bkCapacity);
	printf("Enter the incoming packet sizes\n");
	for(int i=0;i<numPkts;i++)
	{
		scanf("%d",&pkts[i]);
	}
	leakyBucket();
	return 0;
}
