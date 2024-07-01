#include "algebra.h"
#include <stdio.h>
#include <math.h>

int min(int a,int b)
{
    if(a<=b)
    return a;
    else
    return b;
}

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    // ToDo
    if (a.cols != b.cols || a.rows != b.rows)
    {
        printf("Error: Matrix a and b must have the same rows and cols.");
        return create_matrix(0, 0);
    }
    Matrix sum;
    sum.rows = a.rows;
    sum.cols = b.cols;
    for (int i = 0; i < sum.rows; i++)
    {
        for (int j = 0; j < sum.cols; j++)
        {
            sum.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return sum;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    // ToDo
    if (a.cols != b.cols || a.rows != b.rows)
    {
        printf("Error: Matrix a and b must have the same rows and cols.");
        return create_matrix(0, 0);
    }
    Matrix sub;
    sub.rows = a.rows;
    sub.cols = b.cols;
    for (int i = 0; i < sub.rows; i++)
    {
        for (int j = 0; j < sub.cols; j++)
        {
            sub.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return sub;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    // ToDo
    if (a.cols != b.rows)
    {
        printf("Error: Matrix a must be equal to the rows of Matrix b.");
        return create_matrix(0, 0);
    }
    Matrix mul;
    mul.rows = a.rows;
    mul.cols = b.cols;
    for (int i = 0; i < mul.rows; i++)
    {
        for (int j = 0; j < mul.cols; j++)
        {
            mul.data[i][j] = 0;
            for (int k = 0; k < mul.cols; k++)
            {
                mul.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return mul;
}

Matrix scale_matrix(Matrix a, double k)
{
    // ToDo
    Matrix scale;
    scale.cols = a.cols;
    scale.rows = a.rows;
    for (int i = 0; i < scale.rows; i++)
    {
        for (int j = 0; j < scale.cols; j++)
        {
            scale.data[i][j] = k * a.data[i][j];
        }
    }
    return scale;
}

Matrix transpose_matrix(Matrix a)
{
    // ToDo
    Matrix trans;
    trans.rows = a.cols;
    trans.cols = a.rows;
    for (int i = 0; i < trans.rows; i++)
    {
        for (int j = 0; j < trans.cols; j++)
        {
            trans.data[i][j] = a.data[j][i];
        }
    }
    return trans;
}

double det_matrix(Matrix a)
{
    double det = 0;
    if (a.cols != a.rows)
        return 0;
    if (a.cols == 1)
    {
        return a.data[0][0];
    }
    if (a.cols == 2)
    {
        return a.data[0][0] * a.data[1][1] - a.data[0][1] * a.data[1][0];
    }
    for (int i = 0; i < a.cols; i++)
    {
        // copy a matrix
        Matrix a_i;
        a_i.rows = a.rows - 1;
        a_i.cols = a.cols - 1;
        for (int m = 0; m < a_i.rows; m++)
        {
            for (int n = 0; n < i ; n++)
            {
                a_i.data[m][n] = a.data[m + 1][n];
            }
            for (int n = i ; n < a_i.cols; n++)
            {
                a_i.data[m][n] = a.data[m + 1][n + 1];
            }
        }
        det += power(-1,i+2) * a.data[0][i] * det_matrix(a_i);
    }
    return det;
}

Matrix inv_matrix(Matrix a)
{
    // ToDo
    if(a.rows!=a.cols)
    {
        printf("This matrix do not have an inverse matrix.\n");
        return create_matrix(0,0);
    }
    if (det_matrix(a) == 0)
    {
        printf("This matrix do not have an inverse matrix.\n");
        return create_matrix(0, 0);
    }
        
    // 伴随矩阵的求解
    Matrix a_adjoint;
    a_adjoint.rows = a.rows;
    a_adjoint.cols = a.cols;
    for (int i = 0; i < a_adjoint.rows; i++)
    {
        for (int j = 0; j < a_adjoint.cols; j++)
        {
            /* code */
            // 创建矩阵a_ji;
            Matrix a_ji;
            a_ji.rows = a.rows - 1;
            a_ji.cols = a.cols - 1;
            int m,n;
            /*第0~j-1行保持不变*/
            for (m = 0; m < j; m++)
            {
                for (n = 0; n < i ; n++)
                {
                    a_ji.data[m][n] = a.data[m][n];
                }
                for (n = i ; n < a_ji.cols; n++)
                {
                    a_ji.data[m][n] = a.data[m][n + 1];
                }
            }
            /*第j行到结束往下取一行*/
            for(m=j;m<a_ji.rows;m++)
            {
                for (n = 0; n < i; n++)
                {
                    a_ji.data[m][n] = a.data[m+1][n];
                }
                for (n = i ; n < a_ji.cols; n++)
                {
                    a_ji.data[m][n] = a.data[m+1][n + 1];
                }
            }
            //求a_ij
            a_adjoint.data[i][j]=power(-1,i+j)*det_matrix(a_ji);
        }
    }
    return scale_matrix(a,1/det_matrix(a_adjoint));
}

int rank_matrix(Matrix a)
{
    // ToDo
    int rank=min(a.rows,a.cols);
    int i;
    Matrix b;
    b.cols=a.cols;
    b.rows=a.rows;
    int c=0;
    /*查找全0列并将其放在矩阵的末尾*/
    for(int h=0;h<a.cols;h++)
    {
        if(judge_zero_col(a,h)==1)
        {
            
            for(int l=0;l<a.rows;l++)
            {
                b.data[l][c]=a.data[l][h];
            }
            c++;
        }
    }
    //printf("b is :\n");
    //print_matrix(b);
    //printf("\n");
    /*第i行能否使用高斯消元法*/
    for(i=0;i<b.cols;i++)
    {
        if(b.data[i][i]!=0)
        {
            my_label:
            /*进行高斯消元操作,将对角线以下的数全部化0*/
            for(int k=i+1;k<b.rows;k++)
            {
                int gauss_factor=b.data[k][i]/b.data[i][i];
                if(b.data[k][i]!=0)
                {
                    for(int s=i;s<b.cols;s++)
                    {
                        if(s==i)
                        b.data[k][s]=0;
                        else
                        {
                            b.data[k][s]=b.data[k][s]-gauss_factor*b.data[i][s];
                        }
                    }
                }
                
            }
            /*打印矩阵，便于检查*/
           // print_matrix(b);

        }
        else
        {
            /*在下方找非0元素*/
            int j;
            for(j=i+1;j<b.rows;j++)
            {
                /*下方有非0元素*/
                if(b.data[j][i]!=0)
                {
                    /*互换行与行*/
                    switch_rows(&b,j,i);
                    /*打印换行后的矩阵，方便调试*/
                    //print_matrix(b);
                    //printf("\n");
                    goto my_label;
                }
            } 
            /*下方无非0元素*/
            if(j==b.rows)
            {
                /*将秩减一，将当前列换到最后一列*/
                rank--;
            }
        }
    }
    return rank;
}

double trace_matrix(Matrix a)
{
    // ToDo
    if(a.cols!=a.rows)
    return 0;
    else
    {
        int number=a.cols;
        double trace=0;
        for(int i=0;i<number;i++)
        {
            trace+=a.data[i][i];
        }

        return trace;
    }
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}

void switch_rows(Matrix *pmatrix, int row1, int row2)
{
    double temp[MAX_MATRIX_SIZE];
    for(int i=0;i<pmatrix->cols;i++)
    {
        temp[i]=pmatrix->data[row1][i];
        pmatrix->data[row1][i]=pmatrix->data[row2][i];
        pmatrix->data[row2][i]=temp[i];
    }
}

void switch_cols(Matrix *qmatrix, int col1, int col2)
{
    double temp[MAX_MATRIX_SIZE];
    for(int n=0;n<qmatrix->rows;n++)
    {
        temp[n]=qmatrix->data[n][col1];
        qmatrix->data[n][col1]=qmatrix->data[n][col2];
        qmatrix->data[n][col2]=temp[n];
    }
}

int judge_zero_col(Matrix a, int col)
{
    int flag = 0;
    for(int t=0;t<a.rows;t++)
    {
        if(a.data[t][col]!=0)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int power(int a,int b)
{
    int result=1; 
    for(int k=1;k<=b;k++)
    {
        result*=a;
    }
    return result;
}
