#include<stdio.h>
#include<stdlib.h>
#define BASE 26				// a ~ z, 26 elements
#define MODULE 1000000007	// when module is large enough, the probability of collision is extremely small
#define MAX_LENGTH 200001	// maximum length of string S

char S[MAX_LENGTH],*str1,*str2;
long long HV[MAX_LENGTH],BaseExp[MAX_LENGTH];	// HV : hash value array ; BaseExp[0]=1, BaseExp[1]=26, ...
int N,len,i,j,q,a,b,len_m,start,mid,end,max_len,record;
long long R1,R2;

void MakeTable(){
	for(i=0,BaseExp[0]=1;i<len;i++){
		HV[i]=S[i]-'a';
		if(i>0){
			HV[i]=(HV[i]+(HV[i-1]*BASE%MODULE))%MODULE;		// hash value part
			BaseExp[i]=BaseExp[i-1]*BASE%MODULE;			// exponent part
		}
	}
}

int SameHashValue(){
	R1=HV[a+mid];
	if(a>0)
		R1-=(HV[a-1]*BaseExp[mid+1]%MODULE);
	R1=(R1+MODULE)%MODULE;
	R2=HV[b+mid];
	if(b>0)
		R2-=(HV[b-1]*BaseExp[mid+1]%MODULE);
	R2=(R2+MODULE)%MODULE;
	if(R1==R2)
		return 1;
	return 0;
}

void Swap(int *x,int *y){
	int temp=*x;
	*x=*y;
	*y=temp;
}

void Query(){
	scanf("%d",&q);
	for(;q>0;q--){
		scanf("%d%d",&a,&b);
		a--;
		b--;
		if(a>b)
			Swap(&a,&b);
		for(max_len=0,str1=&S[a],str2=&S[b],len_m=len-b,start=0,end=len_m-1,record=0;;){	// apply binary search
			mid=(start+end)/2;
			if(SameHashValue()){
				start=mid+1;
				record=mid;
				max_len=mid+1;
			}
			else
				end=mid-1;
			if(start>end)
				break;
		}
		printf("%d\n",max_len);
	}
}

int main(){
    scanf("%d",&N);
    for(;N>0;N--){
        scanf("%d",&len);
		scanf("%s",S);
		MakeTable();	// establish two tables, HV and BaseExp
		Query();
    }
    return 0;
}
