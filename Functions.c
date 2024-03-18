#include <stdio.h>
#include <stdlib.h>
#include "MyHeader.h"

float procentBaril(float a, float b)
{
    float tmp;
    if(a!=b)
        tmp=100-(b*100)/a;
    return -tmp;
}

float maxVct(int n, struct benzinarie bz[])
{
    int i;
    float max = bz[0].profitbzf;
    for(i=1; i<n; i++)
        if(max < bz[i].profitbzf)
            max = bz[i].profitbzf;
    return max;
}

int indice(int n, struct benzinarie bz[])
{
    int i, p=0;
    float max = bz[0].profitbzf;
    for(i=1; i<n; i++)
        if(max < bz[i].profitbzf)
        {
            max = bz[i].profitbzf;
            p=i;
        }
    return p;
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bubbleSort(int v[], int n)
{
    int i, j;
    for(i=0; i<n-1; i++)
        for(j=0; j<n-i-1; j++)
            if(v[j]>v[j+1])
                swap(&v[j], &v[j+1]);
}
