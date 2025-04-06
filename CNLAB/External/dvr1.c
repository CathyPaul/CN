#include <stdio.h>
#define max 10
#define inf 9999
int dist[max][max],cost[max][max],node[max][max];
int nodes;
void initialise()
{

	for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<nodes;j++)
		{
			dist[i][j]=cost[i][j];
			if(i==j)
			{
				node[i][j]=i;
			}
			else if(cost[i][j]!=inf)
			{
				node[i][j]=j;
			}
			else
			{
				node[i][j]=-1;
			}
		}
	}
}
void updateRoutes()
{
	int updated;
	do
	{
		updated=0;
		for(int i=0;i<nodes;i++)
		{
			for(int j=0;j<nodes;j++)
			{
				for(int k=0;k<nodes;k++)
				{
					if(cost[i][k]!=inf && dist[k][j]!=inf)
					{
						if(dist[i][j]>dist[i][k]+dist[k][j])
						{
							dist[i][j]=dist[i][k]+dist[k][j];
							node[i][j]=node[i][k];
							updated=1;
						}
					}
				}
			}
		}
	}while(updated);
}
void display()
{
	for(int i=0;i<nodes;i++)
	{
		printf("Distance from node %d\n",i);
		for(int j=0;j<nodes;j++)
		{
			printf("\tto nodes %d is %d throught %d\n",j,dist[i][j],node[i][j]);
		}
	}
}
int main()
{
	
	printf("Enter number of nodes\n");
	scanf("%d",&nodes);
	printf("Enter the cost matrix\n");
	for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<nodes;j++)
		{
			scanf("%d",&cost[i][j]);
			if(i==j)
			{
				cost[i][j]=0;
			}
		}
	}
	initialise();
	updateRoutes();
	display();
	return 0;
}

