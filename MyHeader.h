#ifndef MYHEADER_H_INCLUDED
#define MYHEADER_H_INCLUDED


struct benzinarie
{
    struct combustibil
    {
        float pra, pras, prv, prvs, profit, profits, s_inc, s_incs, coefpr;
        char tip[20];
        int cv10z[10], cap_rez;
    };
    int cap_rezt;
    struct combustibil cm[5];
    char nume[20];
    float aux, profitbzs, profitbzf;
};

float procentBaril(float a, float b);
float maxVct(int n, struct benzinarie bz[]);
int indice(int n, struct benzinarie bz[]);
void swap(int *x, int *y);
void bubbleSort(int v[], int n);
void display();

#endif // MYHEADER_H_INCLUDED
