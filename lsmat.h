#ifndef __LSMAT_H__
#define __LSMAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

double** load_dmatrix( const char *filename, int rows, int cols )
{
    int i, j;
    FILE *pf = fopen(filename, "rt");
    if(pf==NULL)
    {
        printf("Error open file!\n");
        return 0;
    }

    double ** data = (double**) malloc( rows * sizeof(double*) );
    
    for( i=0;i<rows;i++ )
    {
        data[i] = (double*) malloc( cols * sizeof(double) );
        for( j=0;j<cols;j++ )
        {
            fscanf( pf, "%lf", &data[i][j]);
        }
        fscanf( pf, "\n" );
    }
    fclose(pf);
    return data;
}

void save_dmatrix(const char *filename, double **data, int rows, int cols)
{
    int i, j;
    FILE *pf = fopen(filename, "w+");
    if(pf == NULL)
    {
        printf("Opne file failed!\n");
        return;
    }
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            fprintf(pf, "%f ", data[i][j]);
        }
        fprintf(pf, "\n");
        fflush(pf);
    }
    fclose(pf);
}

void release_dmatrix(double **data, int rows)
{
    int i;
    for(i=0;i<rows;i++)
    {
        if(data[i])
            free(data[i]);
    }
    free(data);
}

#endif
