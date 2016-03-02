#include<stdio.h>
#include<stdlib.h>

typedef struct city{    // The graph is recorded by the method of adjacency lists, this is the structure I use for every vertices.
    int id;
    long long cost;
    struct city *next;
}City;

typedef struct map{
    int CityNum;
    int RoadNum;
    long long *OneToAll;    // The array used to store the costs from city 1 to all the other cities.
    long long *AllToOne;    // The array used to store the costs from all cities to city 1.
    City **AdjCityOut;  // original version -> out-degree, for OneToAll
    City **AdjCityIn;   // reverse version -> in-degree, for AllToOne
}Map;

Map *CreateMap(){	// This is the function to create the specified graph
    Map *M=(Map *)malloc(sizeof(Map));
    scanf("%d%d",&M->CityNum,&M->RoadNum);
    M->AdjCityOut=(City **)malloc(M->CityNum*sizeof(City *));
    M->AdjCityIn=(City **)malloc(M->CityNum*sizeof(City *));
    int i;
    for(i=0;i<M->CityNum;i++){
        M->AdjCityOut[i]=(City *)malloc(sizeof(City));
        M->AdjCityOut[i]->id=i+1; // id of the city
        M->AdjCityOut[i]->cost=0; // cost of one city to itself is 0
        M->AdjCityOut[i]->next=M->AdjCityOut[i];
        M->AdjCityIn[i]=(City *)malloc(sizeof(City));
        M->AdjCityIn[i]->id=i+1; // id of the city
        M->AdjCityIn[i]->cost=0; // cost of one city to itself is 0
        M->AdjCityIn[i]->next=M->AdjCityIn[i];
    }
    int u,v;
    long long c;
    City *new_node;
    for(i=0;i<M->RoadNum;i++){
        scanf("%d%d%lld",&u,&v,&c);
        // deal with original part
        new_node=(City *)malloc(sizeof(City));
        new_node->id=v;
        new_node->cost=c;
        new_node->next=M->AdjCityOut[u-1]->next;
        M->AdjCityOut[u-1]->next=new_node;
        // deal with reverse part
        new_node=(City *)malloc(sizeof(City));
        new_node->id=u;
        new_node->cost=c;
        new_node->next=M->AdjCityIn[v-1]->next;
        M->AdjCityIn[v-1]->next=new_node;
    }
    M->OneToAll=(long long *)malloc(M->CityNum*sizeof(long long));
    M->AllToOne=(long long *)malloc(M->CityNum*sizeof(long long));
    return M;
}	// After executing this function, all the informations of the graph except OneToAll and AllToOne are created

void DeleteMap(Map *M){  // Just free all the memory I use in my program, to prevent memory leak
    City *ptr,*ptr2;
    int i;
    for(i=0;i<M->CityNum;i++){
        for(ptr=M->AdjCityOut[i]->next;ptr!=M->AdjCityOut[i];){
            ptr2=ptr;
            ptr=ptr->next;
            free(ptr2);
        }
        free(ptr);
    }
    for(i=0;i<M->CityNum;i++){
        for(ptr=M->AdjCityIn[i]->next;ptr!=M->AdjCityIn[i];){
            ptr2=ptr;
            ptr=ptr->next;
            free(ptr2);
        }
        free(ptr);
    }
    free(M->AdjCityOut);
    free(M->AdjCityIn);
    free(M->OneToAll);
    free(M->AllToOne);
    free(M);
}

typedef struct heap_node{	// This is the data structure I use in my heap
    int city_id;
    long long cost;
}HeapNode;

typedef struct heap{	// My heap contains the following datatype
    int heap_size;
    int node_num;	// record the amount of nodes in the heap now
    HeapNode *store;
}Heap;

void Swap(HeapNode *a,HeapNode *b){
    HeapNode temp=*a;
    *a=*b;
    *b=temp;
}

void AdjustHeap1(Heap *H){          // for function "AddToHeap()", time complexity = O(logn) as mentioned in previous lecture
    int i=H->node_num-1,j=(i-1)/2;
    do{
        if(H->store[i].cost<H->store[j].cost)
            Swap(&H->store[i],&H->store[j]);
        else
            return;
        i=j;
        j=(i-1)/2;
    }while(j>=0);
}

void AdjustHeap2(Heap *H,int now){          // for function "GetMin()", time complexity =O(logn)
    int l=2*now+1,r=2*now+2,min=now;
    if(l<H->node_num&&H->store[l].cost<H->store[min].cost)
        min=l;
    if(r<H->node_num&&H->store[r].cost<H->store[min].cost)
        min=r;
    if(min!=now){
        Swap(&H->store[now],&H->store[min]);
        AdjustHeap2(H,min);
    }
}

void AddToHeap(Heap *H,int city_id,long long cost){
    H->store[H->node_num].city_id=city_id;
    H->store[H->node_num].cost=cost;
    H->node_num++;
    AdjustHeap1(H);
}

int GetMin(Heap *H){
    int temp=H->store[0].city_id;
    H->store[0].city_id=H->store[H->node_num-1].city_id;
    H->store[0].cost=H->store[H->node_num-1].cost;
    H->node_num--;
    AdjustHeap2(H,0);
    return temp;
}

int IsEmpty(Heap *H){
    if(H->node_num)
        return 0;
    return 1;
}

void SubFunction1(Map *M){
    // create a heap
    Heap *H=(Heap *)malloc(sizeof(Heap));
    H->heap_size=M->RoadNum;
    H->store=(HeapNode *)malloc(H->heap_size*sizeof(HeapNode));
    H->node_num=0;
    // create finish
    int *InSet=(int *)malloc(M->CityNum*sizeof(int));	// To record if the city is in the set(which means its smallest cost has been determined already)r not
    int i;
    for(i=0;i<M->CityNum;i++){
        InSet[i]=0;             // at the beginning, no city is in the set
        M->OneToAll[i]=-1;      // at the beginning, no city has been visited yet
    }
    City *ptr=M->AdjCityOut[0]->next;
    for(;ptr!=M->AdjCityOut[0];ptr=ptr->next){
        M->OneToAll[ptr->id-1]=ptr->cost;
        AddToHeap(H,ptr->id,ptr->cost);
    }
    M->OneToAll[0]=0;
    InSet[0]=1;
    int now;
    while(1){
        do{
	    if(IsEmpty(H))
		return;
            now=GetMin(H);
        }while(InSet[now-1]);
        InSet[now-1]=1;
        for(ptr=M->AdjCityOut[now-1]->next;ptr!=M->AdjCityOut[now-1];ptr=ptr->next){
            if(!InSet[ptr->id-1]){
                if(((M->OneToAll[now-1]+ptr->cost)<M->OneToAll[ptr->id-1])||M->OneToAll[ptr->id-1]==-1){
                    M->OneToAll[ptr->id-1]=M->OneToAll[now-1]+ptr->cost;
                    AddToHeap(H,ptr->id,M->OneToAll[ptr->id-1]);
                }
            }
        }
    }
    free(InSet);
    free(H->store);
    free(H);
}

void SubFunction2(Map *M){	// Basically, the fuction does the same thing with SubFunction1, just change "OneToAll" to "AllToOne"
    // create a heap
    Heap *H=(Heap *)malloc(sizeof(Heap));
    H->heap_size=M->RoadNum;
    H->store=(HeapNode *)malloc(H->heap_size*sizeof(HeapNode));
    H->node_num=0;
    // create finish
    int *InSet=(int *)malloc(M->CityNum*sizeof(int));
    int i;
    for(i=0;i<M->CityNum;i++){
        InSet[i]=0;             // at the beginning, no city is in the set
        M->AllToOne[i]=-1;      // at the beginning, no city has been visited yet
    }
    City *ptr=M->AdjCityIn[0]->next;
    for(;ptr!=M->AdjCityIn[0];ptr=ptr->next){
        M->AllToOne[ptr->id-1]=ptr->cost;
        AddToHeap(H,ptr->id,ptr->cost);
    }
    M->AllToOne[0]=0;
    InSet[0]=1;
    int now;
    while(1){
        do{
	    if(IsEmpty(H))
		return;
            now=GetMin(H);
        }while(InSet[now-1]);
        InSet[now-1]=1;
        for(ptr=M->AdjCityIn[now-1]->next;ptr!=M->AdjCityIn[now-1];ptr=ptr->next){
            if(!InSet[ptr->id-1]){
                if(((M->AllToOne[now-1]+ptr->cost)<M->AllToOne[ptr->id-1])||M->AllToOne[ptr->id-1]==-1){
                    M->AllToOne[ptr->id-1]=M->AllToOne[now-1]+ptr->cost;
                    AddToHeap(H,ptr->id,M->AllToOne[ptr->id-1]);
                }
            }
        }
    }
    free(InSet);
    free(H->store);
    free(H);
}

void CreateTable(Map *M){
    SubFunction1(M); // deal with OneToAll
    SubFunction2(M); // deal with AllToOne
}

void QueryFunction(Map *M){
    int q;
    scanf("%d",&q);  // number of queries
    int u,v;
    for(;q>0;q--){
        scanf("%d%d",&u,&v); // from city u, with passing city 1, to city v
        if(M->AllToOne[u-1]==-1||M->OneToAll[v-1]==-1)
            puts("-1");
        else
            printf("%lld\n",M->AllToOne[u-1]+M->OneToAll[v-1]);
    }
}

int main(){
    Map *M=CreateMap();
    CreateTable(M);
    QueryFunction(M);
    DeleteMap(M);
    return 0;
}
