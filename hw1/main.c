#include<stdio.h>
#include<stdlib.h>
#define SUB_MATRIX_SIZE 3

short int from=-1;  // 紀錄來的方向，一開始時初始化為 -1 (1:從左邊來 2:從上面來 3:從右邊來 4:從下面來)

long long DetComputation(short int matrix[][SUB_MATRIX_SIZE],short int a,short int b)  // 計算該sub_matrix determinant
{
    return (matrix[0][0]*matrix[1][1]*matrix[2][2]+matrix[1][0]*matrix[2][1]*matrix[0][2]+matrix[2][0]*matrix[0][1]*matrix[1][2])
          -(matrix[2][0]*matrix[1][1]*matrix[0][2]+matrix[1][0]*matrix[0][1]*matrix[2][2]+matrix[2][1]*matrix[1][2]*matrix[0][0]);
}

short int ChooseDirection(short int **A,short int **B,short int m,short int n,short int x,short int y)
{
    short int sub_matrix[3][3];
    short int i,j,p,q;
    for(i=0,p=x-1;i<SUB_MATRIX_SIZE;i++,p++)
        for(j=0,q=y-1;j<SUB_MATRIX_SIZE;j++,q++)
            sub_matrix[i][j]=B[p][q];
    long long det=DetComputation(sub_matrix,3,3);
    if(x>0&&x<m-1&&y>0&&y<n-1&&det==A[x][y+1]&&from!=3)  // x>0&&x<m-1&&y>0&&y<n-1 檢測其是否位於邊界，是的話沒有sub_matrix
    {
        from=1;
        return 1;
    }
    if(x>0&&x<m-1&&y>0&&y<n-1&&det==A[x+1][y]&&from!=4)
    {
        from=2;
        return 2;
    }
    if(x>0&&x<m-1&&y>0&&y<n-1&&det==A[x][y-1]&&from!=1)
    {
        from=3;
        return 3;
    }
    if(x>0&&x<m-1&&y>0&&y<n-1&&det==A[x-1][y]&&from!=2)
    {
        from=4;
        return 4;
    }
    return 0;
}

int main()
{
    short int X,Y;  // Maze entry X and Y coordinates
    short int m,n;  // Numbers of rows (m) and columns (n) of the matrices  (since the max value of m or n is 1000, I use "short int" just to save memory)
    short int i,j;
    scanf("%hd %hd",&X,&Y);
    scanf("%hd %hd",&m,&n);
    short int **A=(short int **)malloc(m*sizeof(short int *));
    short int **B=(short int **)malloc(m*sizeof(short int *));
    for(i=0;i<m;i++)
    {
        A[i]=(short int *)malloc(n*sizeof(short int));
        for(j=0;j<n;j++)
            scanf("%hd",&A[i][j]);
    }
    for(i=0;i<m;i++)
    {
        B[i]=(short int *)malloc(n*sizeof(short int));
        for(j=0;j<n;j++)
            scanf("%hd",&B[i][j]);
    }
    X--;  // 真正的 index，方便之後存取
    Y--;
    short int stop=0; // 紀錄是否停止前進
    printf("%d %d\n",X+1,Y+1); // 先列印出一開始的位置
    while(1)
    {
        if(X==0||Y==0||X==m-1||Y==n-1)  // 在邊界，sub-matrix不存在
            break;
        switch(ChooseDirection(A,B,m,n,X,Y))  // 選擇方向
        {
            case 0:
                stop=1;
                break;
            case 1:  // 要向右走
                Y++;
                if(Y==n-1)
                    stop=1;
                break;
            case 2:  // 要向下走
                X++;
                if(X==m-1)
                    stop=1;
                break;
            case 3:  // 要向左走
                Y--;
                if(Y==0)
                    stop=1;
                break;
            case 4:  // 要向上走
                X--;
                if(X==0)
                    stop=1;
                break;
        }
        if(stop)
            break;
        printf("%d %d\n",X+1,Y+1);
    }
    free(A);
    free(B);
    return 0;
}
