#include<stdio.h>
#define MAXSIZE 10
#define MODULO 1000000007

int Matrix[MAXSIZE][MAXSIZE];
long long Det=0LL;                               // 宣告算出的結果:det(A)

long long ModuloTransform(long long x)           // 根據 module 的數學定義所寫出的函式
{
    if(x>=0)
        return x%MODULO;
    while(x<0)
        x+=MODULO;
    return x;
}

void Computation(int permute[],int n,int switches)
{
    int i;
    long long multi=1LL;                         // 儘管有destributive law, 萬一被除數小於除數，乘上另一個大數之後仍有可能overflow, 因此直接宣告長整數
    for(i=0;i<n;i++)
    {
        multi*=(long long)(Matrix[i][permute[i]]);
        multi=ModuloTransform(multi);
    }
    if(switches%2==0)
        Det+=multi;
    else
        Det-=multi;
    Det=ModuloTransform(Det);
}

void Swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

void Permutation(int permute[],int n,int now,int switches)           // now紀錄現在何者為交換頭，switches紀錄交換次數
{
    if(now==n)  // 進入Computation 函式
    {
        Computation(permute,n,switches);
        return;
    }
    int i;
    for(i=now;i<n;i++)
    {
        Swap(&permute[now],&permute[i]);
        if(i==now)
            Permutation(permute,n,now+1,switches);
        else
            Permutation(permute,n,now+1,switches+1);
        Swap(&permute[now],&permute[i]);
    }
}

int main()
{
    int n,i,j;
    scanf("%d",&n);
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            scanf("%d",&Matrix[i][j]);
    int permute[MAXSIZE];             // 根據計算determinant的演算法則，需要一個陣列去做排列組合作為索引值
    for(i=0;i<n;i++)
        permute[i]=i;                 // 初始索引值
    Permutation(permute,n,0,0);       // 進入排列組合
    printf("%lld\n",Det);             // 輸出結果
    return 0;
}
