#include<stdio.h>
#include<stdlib.h>

typedef struct city{
    int ID;
    int x;  // x - coordinate of the city
    int y;  // y - coordinate of the city
}City;

typedef struct island{
    int CityNum;  // number of cities on this island
    City *CityOnIsland;  // all cities
}Island;

void CreateIsland(Island *AllIsland,int T){
    int i,j;
    for(i=0;i<T;i++){
        scanf("%d",&AllIsland[i].CityNum);
        AllIsland[i].CityOnIsland=(City *)malloc(AllIsland[i].CityNum*sizeof(City));
        for(j=0;j<AllIsland[i].CityNum;j++){
            scanf("%d%d",&AllIsland[i].CityOnIsland[j].x,&AllIsland[i].CityOnIsland[j].y);
            AllIsland[i].CityOnIsland[j].ID=j;
        }
    }
}

typedef struct edge{
    int ID1,ID2;
    long long cost;
}Edge;

typedef struct set{
    int ID;
    int NumInSet;
    struct set *root;
}Set;

Set FindSet(Set *s){
    Set *temp;
    for(temp=s;temp->root!=temp;temp=temp->root);
    return *temp;
}

void Union(Set *S,int id1,int id2){
    if(S[id1].NumInSet>=S[id2].NumInSet){
        S[id2].root=&S[id1];
        S[id1].NumInSet+=S[id2].NumInSet;
    }
    else{
        S[id1].root=&S[id2];
        S[id2].NumInSet+=S[id1].NumInSet;
    }
}

int CompareFun1(const void *a,const void *b){
    City A=*((City *)a);
    City B=*((City *)b);
    if(A.x>B.x)
        return 1;
    if(A.x==B.x&&A.y>B.y)
        return 1;
    return -1;
}

int CompareFun2(const void *a,const void *b){
    City A=*((City *)a);
    City B=*((City *)b);
    if(A.y>B.y)
        return 1;
    if(A.y==B.y&&A.x>B.x)
        return 1;
    return -1;
}

int CompareFun3(const void *a,const void *b){
    Edge A=*((Edge *)a);
    Edge B=*((Edge *)b);
    return A.cost-B.cost;
}

long long Kruskal(Island *I){
    Edge *E=(Edge *)malloc((4*I->CityNum)*sizeof(Edge));  // maximum amount of edges

    qsort(I->CityOnIsland,I->CityNum,sizeof(City),CompareFun1);  //  將所有 city 按照相同的x座標加以排序 (不知道能否使用 qsort )
    int EdgeNum=0;
    int i;
    for(i=1;i<I->CityNum;i++){  //  得到縱向所有相鄰的城市之edge, 存在 E 裡
        if(I->CityOnIsland[i].x==I->CityOnIsland[i-1].x){
            E[EdgeNum].ID1=I->CityOnIsland[i-1].ID;
            E[EdgeNum].ID2=I->CityOnIsland[i].ID;
            E[EdgeNum].cost=I->CityOnIsland[i].y-I->CityOnIsland[i-1].y;
            EdgeNum++;
        }
    }
    qsort(I->CityOnIsland,I->CityNum,sizeof(City),CompareFun2);  // 將所有 city 按照相同的y座標加以排序
    for(i=1;i<I->CityNum;i++){  // 得到橫向所有相鄰的城市之edge, 存在 E 裡
        if(I->CityOnIsland[i].y==I->CityOnIsland[i-1].y){
            E[EdgeNum].ID1=I->CityOnIsland[i-1].ID;
            E[EdgeNum].ID2=I->CityOnIsland[i].ID;
            E[EdgeNum].cost=I->CityOnIsland[i].x-I->CityOnIsland[i-1].x;
            EdgeNum++;
        }
    }
    // 根據 Kruskal's Algorithm, 將所有 E 裡面的 edge 由小到大加以排序, 逐步完成此一演算法
    qsort(E,EdgeNum,sizeof(Edge),CompareFun3);   // sort the edges
    Set *S=(Set *)malloc(I->CityNum*sizeof(Set));   // construct the MST
    for(i=0;i<I->CityNum;i++){
        S[i].ID=i;
        S[i].root=&S[i];
        S[i].NumInSet=1;
    }
    long long C=0;
    Set s1,s2;
    int EdgeInUse=0;
    for(i=0;i<EdgeNum;i++){
        s1=FindSet(&S[E[i].ID1]);
        s2=FindSet(&S[E[i].ID2]);
        if(s1.ID!=s2.ID){
            Union(S,s1.ID,s2.ID);
            C+=E[i].cost;
            EdgeInUse++;
        }
    }
    free(S);
    free(E);
    if(EdgeInUse!=I->CityNum-1)
        C=-1;
    return C;
}

void OutPut(Island *AllIsland,int T){
    int i;
    long long cost;
    for(i=0;i<T;i++){
        cost=Kruskal(&AllIsland[i]);
        printf("%lld\n",cost);
    }
}

int main(){
    int T;  // number of islands;
    scanf("%d",&T);
    Island *AllIsland=(Island *)malloc(T*sizeof(Island));
    CreateIsland(AllIsland,T);
    OutPut(AllIsland,T);        // to do all outputs
    return 0;
}
