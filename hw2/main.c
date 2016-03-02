#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct node
{
    int value;
    struct node *next;
};

struct block
{
    int nodeCount;
    struct node *head;
    struct block *next;
};

struct block *CreateBlocks(int n)  // 一決定 M,N，就先配置出最大可能的 block數
{
    struct block *new_block,*prev=NULL;
    while(n>0)
    {
        new_block=(struct block *)malloc(sizeof(struct block));
        new_block->nodeCount=0;
        new_block->head=NULL;
        new_block->next=prev;
        prev=new_block;
        n--;
    }
    return new_block;
}

void InsertNode(struct block *root,int k,int x,int N)  // 插入node 到此 unrolled linked list
{
    int which_block=(k-1)/N;  // 決定是到第幾個block
    struct block *now_block=root;
    while(which_block)  // 移動至該block
    {
        now_block=now_block->next;
        which_block--;
    }
    int insert_place=(k-1)%N;  // 決定插入位置(走幾步到那裏)
    struct node *new_node=(struct node *)malloc(sizeof(struct node));
    new_node->value=x;
    if(insert_place<0)  // 插在原本頭的位置的前面，全部都要往後移
    {
        if(now_block->head==NULL)  // 這是一個還未使用過的block
            new_node->next=new_node;
        else  // 已有node存在在此block
        {
            new_node->next=now_block->head->next;
            now_block->head->next=new_node;
        }
        now_block->head=new_node;
    }
    else
    {
        struct node *ptr=now_block->head,*ptr2=ptr;
        while(insert_place>-1)
        {
            ptr2=ptr2->next;
            insert_place--;
        }
        for(;ptr2!=now_block->head;ptr=ptr->next,ptr2=ptr2->next);
        new_node->next=ptr->next;
        ptr->next=new_node;
    }
    now_block->nodeCount++;
    // insertion complete!!!
    // 剩下平移動作:
    if(now_block->nodeCount<=N)  // 未滿block所能容納的最大量，不必平移
        return;
    now_block->nodeCount--;
    struct node *to_move;
    struct block *next_block=now_block->next;
    while(next_block->nodeCount==N)
    {
        to_move=now_block->head->next;
        now_block->head->next=to_move->next;
        to_move->next=next_block->head->next;
        next_block->head->next=to_move;
        next_block->head=to_move;
        now_block=next_block;
        next_block=next_block->next;
    }
    to_move=now_block->head->next;
    now_block->head->next=to_move->next;
    if(next_block->head==NULL)  // 未使用過的block
    {
        next_block->head=to_move;
        next_block->nodeCount=1;
        to_move->next=to_move;
        return;
    }
    to_move->next=next_block->head->next;
    next_block->head->next=to_move;
    next_block->head=to_move;
    next_block->nodeCount++;
}

void QueryValue(struct block *root,int k,int N)
{
    int which_block=(k-1)/N;
    struct block *now_block=root;
    while(which_block)
    {
        now_block=now_block->next;
        which_block--;
    }
    int step=(k-1)%N;
    struct node *ptr=now_block->head,*ptr2=ptr;
    while(step)
    {
        ptr2=ptr2->next;
        step--;
    }
    for(;ptr2!=now_block->head;ptr=ptr->next,ptr2=ptr2->next);
    printf("%d\n",ptr->value);
}

int main()
{
    int M,N;
    scanf("%d",&M);  // M:最多有幾個node
    N=(int)sqrt((float)M);  // N:每個block能容納的node數
    int block_num=M/N+1;  // block的數量
    struct block *root=CreateBlocks(block_num);
    char command[10];
    int k,x;
    while(M>0)
    {
        scanf("%s",command);
        scanf("%d",&k);
        switch(*command)
        {
            case 'i':
                scanf("%d",&x);
                InsertNode(root,k,x,N);
                break;
            case 'q':
                QueryValue(root,k,N);
                break;
        }
        M--;
    }
    return 0;
}
