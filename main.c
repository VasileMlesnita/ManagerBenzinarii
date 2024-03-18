#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "MyHeader.h"

int *pn;
struct benzinarie pv[20];

void display()
{
    int i, j, max;
    char text[5];
    float dx, ky;
    dx=1.9/ *pn;
    max=pv[0].cap_rezt;
    for(i=1; i< *pn; i++)
        if(max<pv[i].cap_rezt)
            max=pv[i].cap_rezt;
    ky=1.3/max;
    glClearColor(0.0f, 0.0f, 0.0f, 0.05f);
    glClear(GL_COLOR_BUFFER_BIT);
    for(i=0; i< *pn; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f, i/10.4, 0.0f);
        glVertex2f(-0.95+i*dx, -0.5);
        glVertex2f(-0.95+i*dx+0.14, -0.5);
        glVertex2f(-0.95+i*dx+0.14, -0.5+pv[i].cap_rezt*ky);
        glVertex2f(-0.95+i*dx, -0.5+pv[i].cap_rezt*ky);
        glEnd();

        glRasterPos2f(-1.0+i*dx+dx/2, -0.5+pv[i].cap_rezt*ky+0.05);
        sprintf(text, "%d", pv[i].cap_rezt);
        for(j=0; j<strlen(text); j++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text[j]);
        glRasterPos2f(-1.0+i*dx+dx/2, -0.6);
        for(j=0; j<strlen(pv[i].nume); j++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, pv[i].nume[j]);
    }
    glFlush();
}

int main(int argc, char** argv)
{
    FILE *f1, *f2, *f3;
    int i, j, k, n, r, p=0, mov=0, bzv=0, aux3, aux4, h[15] = {0}, l;
    struct benzinarie bz[20];
    char optn[6], exit[]="exit";
    float pbari, pbara, a1, max=0, cmv = 0, bzm=0;

    f1=fopen("Benzinarii.txt", "r");
    f2=fopen("Combustibili.txt", "r");
    f3=fopen("Output.txt", "w");

    if(!f1 || !f2 || !f3)
    {
        printf("Unul din fisiere nu poate fi deschis!");
        return 1;
    }

    n=0;
    while(!feof(f1))
    {
        fscanf(f1, "%s", bz[n].nume);
        n++;
    }

    fclose(f1);

    fscanf(f2, "%d%f", &k, &pbari);

    pbara=pbari;

    for(i=0; i<n; i++)
        for(j=0; j<k; j++)
        {
            fscanf(f2,"%s%f%f%d", bz[i].cm[j].tip, &bz[i].cm[j].pra, &bz[i].cm[j].prv, &bz[i].cm[j].cap_rez);
            for(r=0; r<10; r++)
                fscanf(f2,"%d", &bz[i].cm[j].cv10z[r]);
        }

    fclose(f2);

    printf("Au fost introduse %d benzinarii si %d categorii de combustibil pentru fiecare.\n\n", n, k);
    for(i=0; i<n; i++)
    {
        printf("Informatii benzinarie %d:\nDenumire benzinarie: %s\n", i+1, bz[i].nume);
        for(j=0; j<k; j++)
        {
            printf("Denumire combustibil %d: %s\nPret achizitie: %.2f, Pret de baza: %.2f, Capacitatea rezervorului: %d\n", j+1, bz[i].cm[j].tip, bz[i].cm[j].pra, bz[i].cm[j].prv, bz[i].cm[j].cap_rez);
            printf("Cantitatea vanduta intr-un interval de 10 zile: ");
            for(r=0; r<10; r++)
                printf("%d ", bz[i].cm[j].cv10z[r]);
            printf("\n");
        }
        printf("\n");
    }
    fflush(stdout);
    printf("Alegeti una dintre optiunile de mai jos:\n");
    fflush(stdout);
    while(strcmp(optn, exit) != 0)
    {
        printf("1. Introduceti o noua valoare pentru pretul barilului.\n");
        printf("2. Calculati profitul pentru fiecare benzinarie.\n");
        printf("3. Benzinaria cu profitul cel mai mare.\n");
        printf("4. Cea mai consumata categorie de combustibil.\n");
        printf("5. Media cantitatii de combustibil vandute.\n");
        printf("6. Sortare dupa capacitatea rezervorului (in fisier).\n");
        printf("7. Intervalul necesar reumplerii rezervorului.\n");
        printf("8. Afisare grafic pentru capacitatea rezervorului benzinariilor.\n");
        printf("Tastati \"exit\" sau combinatia de taste ALT+F4 pentru a opri programul.\n");
        fflush(stdout);
        scanf("%s", optn);

        if(optn[0]=='1')
        {
            printf("\nIntroduceti pretul barilului in $: ");
            printf("\n");
            fflush(stdout);
            scanf("%f", &pbara);
        }

        for(i=0; i<n; i++)
            for(j=0; j<k; j++)
            {
                bz[i].cm[j].pras = bz[i].cm[j].pra;
                bz[i].cm[j].prvs = bz[i].cm[j].prv;
            }

        if(pbari != pbara)
        {
            a1=procentBaril(pbari, pbara);
            for(i=0; i<n; i++)
            {
                for(j=0; j<k; j++)
                {
                    bz[i].cm[j].pras = bz[i].cm[j].pra+(bz[i].cm[j].pra*a1)/100;
                    if(a1>0)
                        bz[i].cm[j].prvs = bz[i].cm[j].prv+(bz[i].cm[j].prv*1.10*(a1))/100;
                    else if(a1<0)
                        bz[i].cm[j].prvs = bz[i].cm[j].prv+(bz[i].cm[j].prv*1.05*(a1))/100;
                }
            }
        }

        if(optn[0]=='1')
        {
            if(a1>0)
                printf("Pretul barilului a crescut cu %.2f procente.\n\n", a1);
            else if(a1<0)
                printf("Pretul barilului a scazut cu %.2f procente.\n\n", -a1);
            fflush(stdout);
        }

        for(i=0; i<n; i++)
            for(j=0; j<k; j++)
            {
                bz[i].cm[j].profits = 0;
                bz[i].cm[j].s_incs = 0;
                bz[i].cm[j].coefpr = 1;
                bz[i].cm[j].s_incs = bz[i].cm[j].cv10z[0]*bz[i].cm[j].prvs;
                bz[i].cm[j].profits = (bz[i].cm[j].prvs-bz[i].cm[j].pras)*bz[i].cm[j].cv10z[0];

                for(r=0; r<9; r++)
                {
                    if(bz[i].cm[j].cv10z[r]>bz[i].cm[j].cv10z[r+1])
                    {
                        bz[i].cm[j].coefpr +=0.02;
                        bz[i].cm[j].s_incs += bz[i].cm[j].cv10z[r+1]*bz[i].cm[j].coefpr*bz[i].cm[j].prv;
                        bz[i].cm[j].profits += (bz[i].cm[j].coefpr*bz[i].cm[j].prvs-bz[i].cm[j].pras)*bz[i].cm[j].cv10z[r+1];
                    }
                    else
                    {
                        bz[i].cm[j].coefpr -=0.02;
                        bz[i].cm[j].s_incs += bz[i].cm[j].cv10z[r+1]*bz[i].cm[j].coefpr*bz[i].cm[j].prv;
                        bz[i].cm[j].profits += (bz[i].cm[j].coefpr*bz[i].cm[j].prvs-bz[i].cm[j].pras)*bz[i].cm[j].cv10z[r+1];

                    }
                }
            }

        if(optn[0]=='2')
        {
            printf("\n");
            fflush(stdout);
            for(i=0; i<n; i++)
            {
                printf("Profitul benzinariei [%d]%s este: ", i+1, bz[i].nume);
                fflush(stdout);
                bz[i].profitbzs = 0;
                for(j=0; j<k; j++)
                    bz[i].profitbzs += bz[i].cm[j].profits;

                printf("%.2f lei ", bz[i].profitbzs);
                for(j=0; j<k; j++)
                {
                    if(j%2==0)
                        printf("%s[%.2f]+", bz[i].cm[j].tip, bz[i].cm[j].profits);
                    else
                        printf("%s[%.2f]", bz[i].cm[j].tip, bz[i].cm[j].profits);
                    fflush(stdout);
                }
                printf("\n");
                fflush(stdout);
            }
            printf("\n");
            fflush(stdout);
        }

        if(optn[0]=='3')
        {
            max = maxVct(n, bz);
            p = indice(n, bz);
            printf("\nBenzinaria %s a avut cel mai mare profit: %.2f lei\n\n", bz[p].nume, max);

        }

        if(optn[0]=='4')
        {
            aux3=0, aux4=0;
            for(i=0; i<n; i++)
                for(j=0; j<k; j++)
                {
                    if(strstr(bz[i].cm[j].tip, "Motorina"))
                    {
                        for(r=0; r<10; r++)
                            aux3 += bz[i].cm[j].cv10z[r];
                    }

                    if(strstr(bz[i].cm[j].tip, "Benzina"))
                    {
                        for(r=0; r<10; r++)
                            aux4 += bz[i].cm[j].cv10z[r];
                    }
                }
            mov=aux3;
            bzv=aux4;

            if(mov>bzv)
                printf("\nCombustibilul preferat este Motorina: %d litri vanduti.\n\n", mov);
            if(mov<bzv)
                printf("\nCombustibilul preferat este Benzina: %d litri vanduti.\n\n", bzv);
            if(mov==bzv)
                printf("\nAtat benzina cat si motorina s-au vandut in cantitati egale: %d litri vanduti.\n\n", mov);
            aux3=aux4=0;

        }

        if(optn[0]=='5')
        {
            aux3 = 0;
            aux4 = 0;
            for(i=0; i<n; i++)
            {
                for(j=0; j<k; j++)
                {
                    for(r=0; r<10; r++)
                        aux3 += bz[i].cm[j].cv10z[r];
                }
                cmv = (float)aux3/(k*10);
                printf("Benzinaria %s vinde o cantitate medie de %.0f L/zi\n", bz[i].nume, cmv);
                aux4 +=cmv;
                aux3=0;
            }
            bzm = (float)aux4/n;
            printf("Media cantitatii vandute de toate benzinariile este: %.0f L/zi\n", bzm);
            printf("\n");
            aux4 = 0;
        }

        if(optn[0]=='6')
        {
            l=0;
            for(i=0; i<n; i++)
            {
                bz[i].cap_rezt=0;
                for(j=0; j<k; j++)
                    bz[i].cap_rezt+=bz[i].cm[j].cap_rez;
                h[l++]= bz[i].cap_rezt;
            }
            bubbleSort(h, l);
            fprintf(f3, "NumeBenzinarie CapacitateRezervor[L] Profit[lei]\n");
            for(j=0; j<l; j++)
                for(i=0; i<n; i++)
                {
                    if(h[j] == bz[i].cap_rezt)
                        fprintf(f3, "%-14s %-21d %.2f\n", bz[i].nume,bz[i].cap_rezt, bz[i].profitbzf);
                }
            fclose(f3);
        }

        if(optn[0]=='7')
        {
            aux3=0;
            aux4=0;
            printf("\nBenzinaria    TipCombustibil    IntervalReumplere[zile]\n");
            for(i=0; i<n; i++)
            {
                for(j=0; j<k; j++)
                {
                    for(r=0; r<10; r++)
                        aux3 += bz[i].cm[j].cv10z[r];
                    cmv = (float)aux3/10;
                    aux4=bz[i].cm[j].cap_rez/cmv;
                    aux3=0;
                    if(j%2==0)
                        printf("%-13s %-29s %d\n", bz[i].nume, bz[i].cm[j].tip, aux4);
                    else if(j%2!=0)
                        printf("%-13s %-29s %d\n", "", bz[i].cm[j].tip, aux4);
                }
                aux4=0;
            }
            printf("\n");
        }

        if(optn[0]=='8')
        {
            pn=&n;
            for(i=0; i<n; i++)
            {
                strcpy(pv[i].nume, bz[i].nume);
                pv[i].cap_rezt = bz[i].cap_rezt;
            }

            glutInit(&argc, argv);
            glutInitWindowSize(1200, 600);
            glutInitWindowPosition(150, 100);
            glutCreateWindow("Grafic capacitate rezervoare ");
            glutDisplayFunc(display);
            glutMainLoop();
        }

        for(i=0; i<n; i++)
        {
            for(j=0; j<k; j++)
                for(r=0; r<10; r++)
                {
                    bz[i].cm[j].profits = bz[i].cm[j].profit;
                    bz[i].cm[j].s_incs = bz[i].cm[j].s_inc;
                }
            bz[i].aux = bz[i].profitbzs;
            bz[i].profitbzf = bz[i].aux;
        }
    }
    return 0;
}


