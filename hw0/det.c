#include<stdio.h>
#define MAXSIZE 10
#define MODULO 1000000007

int Matrix[MAXSIZE][MAXSIZE];
long long Det=0LL;                               // �ŧi��X�����G:det(A)

long long ModuloTransform(long long x)           // �ھ� module ���ƾǩw�q�Ҽg�X���禡
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
    long long multi=1LL;                         // ���ަ�destributive law, �U�@�Q���Ƥp�󰣼ơA���W�t�@�Ӥj�Ƥ��ᤴ���i��overflow, �]�������ŧi�����
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

void Permutation(int permute[],int n,int now,int switches)           // now�����{�b��̬��洫�Y�Aswitches�����洫����
{
    if(now==n)  // �i�JComputation �禡
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
    int permute[MAXSIZE];             // �ھڭp��determinant���t��k�h�A�ݭn�@�Ӱ}�C�h���ƦC�զX�@�����ޭ�
    for(i=0;i<n;i++)
        permute[i]=i;                 // ��l���ޭ�
    Permutation(permute,n,0,0);       // �i�J�ƦC�զX
    printf("%lld\n",Det);             // ��X���G
    return 0;
}
