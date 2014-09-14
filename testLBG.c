#include <stdio.h>
#include "lsmat.h"
#include "lbg.h"

int main()
{
    //double **data = load_dmatrix("hog.data", 5330, 36);
    //lbg("codebook.data", "label.txt", data, 5330, 36, 3);
    double **data = load_dmatrix("test.data", 20, 2);
    lbg("codebook.data", "label.txt", data, 20, 2, 1);
    return 0;
}
