#ifndef __LBG_H__
#define __LBG_H__

#include <stdio.h>
#include "lsmat.h"
#include <math.h>

void lbg( const char * cbname, const char *labelname, double **data, int sample_count, int dim, int bpv )
{
    int i, j, k;
    int cb_size, curr_cb_size;
    double dist_sum, dist_ave, dist_ave_old, delta;

    double **codebook;
    double **cb_tmp;
    int *bin;
    int *label;

    cb_size = 1;
    for(i=0;i<bpv;i++)
        cb_size *= 2;

    if( sample_count < cb_size )
            return;

    // allocate
    codebook = (double**) malloc(cb_size*sizeof(double*));
    cb_tmp  = (double**) malloc(cb_size*sizeof(double*));
    for(i=0;i<cb_size;i++)
    {
        codebook[i] = (double*) malloc(dim * sizeof(double));
        cb_tmp[i] = (double*) malloc(dim * sizeof(double));
    }
    bin = (int*) malloc(cb_size*sizeof(int));
    label = (int*) malloc(sample_count*sizeof(int));


    // initialize one vector
    curr_cb_size = 1;
    for(k=0;k<dim;k++)
    {
        codebook[0][k] = 0;
        for(i=0;i<sample_count;i++)
        {
            codebook[0][k] += data[i][k];
        }
        codebook[0][k] /= sample_count;
        printf("%.5f ", codebook[0][k]);
    }
    printf("\n");

    // loop untill cb_size
    while( curr_cb_size<cb_size )
    {
        // split
        curr_cb_size *= 2;
        for(i=0;i<curr_cb_size;i++)
        {
            double perturb = 0.001;
            if(i%2==0) perturb = -0.001;
            for(k=0;k<dim;k++)
            {
                cb_tmp[i][k] = codebook[i/2][k] * (1.0 + perturb);
            }
        }

        // kmeans --- loop untill not change
        int flag = 0;
        int iter = 0;
        while(flag==0)
        {
            iter++;
            // clear storage
            dist_sum = 0.0;
            for(i=0;i<curr_cb_size;i++)
            {
                bin[i] = 0;
                for(k=0;k<dim;k++)
                {
                    codebook[i][k] = 0.0;
                    printf("%.5f ", cb_tmp[i][k]);
                }
                printf("\n");
            }
            printf("\n");

            // traverse
            for(i=0;i<sample_count;i++)
            {
                double min = DBL_MAX;
                double dist = 0.0;
                int index = -1;
                //printf("data is %f, %f\n",data[i][0],data[i][1]);
                for(j=0;j<curr_cb_size;j++) 
                {
                    dist = 0.0;
                    for(k=0;k<dim;k++)
                    {
                        dist += (data[i][k] - cb_tmp[j][k])*(data[i][k]-cb_tmp[j][k]);
                    }
                    //printf("dist i is %f\n",sqrt(dist));
                    if(dist<min)
                    {
                        
                        //printf("better %f, %f\n",cb_tmp[j][0],cb_tmp[j][1]);
                        min = dist;
                        index = j;
                    }
                }
                label[i] = index;
                bin[index]++;

                dist_sum += sqrt(min);
                //printf("%f %f\n",dist_sum,sqrt(dist));
                for(k=0;k<dim;k++)
                {
                    codebook[index][k] += data[i][k];
                }
            }

            // update
            for(i=0;i<curr_cb_size;i++)
            {
                if(bin[i]>0)
                {
                    for(k=0;k<dim;k++)
                    {
                        codebook[i][k] /= bin[i];
                        cb_tmp[i][k] = codebook[i][k];
                    }
                }
            }
            dist_ave = dist_sum / (sample_count);
            printf("dist_ave is %.8f\n", dist_ave);

            if(iter>1)
            {
                delta = dist_ave_old - dist_ave;
                printf("dist_ave is %.8f, delta is %.8f\n", dist_ave, delta);
                if(delta<0.000001)
                    flag = 1;
            }
            dist_ave_old = dist_ave;
        }
    }

    // save codebook
    save_dmatrix(cbname, codebook, cb_size, dim);
    if(1)
    {
            FILE *pf = fopen(labelname, "w+");
            for(i=0;i<sample_count;i++)
            {
                fprintf(pf, "# %d %d\n", i+1,label[i]+1);
            }
            fclose(pf);
    }

    // free memory
    for(i=0;i<cb_size;i++)
    {
        free(codebook[i]);
        free(cb_tmp[i]);
    }
    free(codebook);
    free(cb_tmp);
    free(bin);
}

#endif
