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

struct block *CreateBlocks(int n)  // �@�M�w M,N�A�N���t�m�X�̤j�i�઺ block��
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

void InsertNode(struct block *root,int k,int x,int N)  // ���Jnode �즹 unrolled linked list
{
    int which_block=(k-1)/N;  // �M�w�O��ĴX��block
    struct block *now_block=root;
    while(which_block)  // ���ʦܸ�block
    {
        now_block=now_block->next;
        which_block--;
    }
    int insert_place=(k-1)%N;  // �M�w���J��m(���X�B�쨺��)
    struct node *new_node=(struct node *)malloc(sizeof(struct node));
    new_node->value=x;
    if(insert_place<0)  // ���b�쥻�Y����m���e���A�������n���Ჾ
    {
        if(now_block->head==NULL)  // �o�O�@���٥��ϥιL��block
            new_node->next=new_node;
        else  // �w��node�s�b�b��block
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
    // �ѤU�����ʧ@:
    if(now_block->nodeCount<=N)  // ����block�ү�e�Ǫ��̤j�q�A��������
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
    if(next_block->head==NULL)  // ���ϥιL��block
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
    scanf("%d",&M);  // M:�̦h���X��node
    N=(int)sqrt((float)M);  // N:�C��block��e�Ǫ�node��
    int block_num=M/N+1;  // block���ƶq
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
