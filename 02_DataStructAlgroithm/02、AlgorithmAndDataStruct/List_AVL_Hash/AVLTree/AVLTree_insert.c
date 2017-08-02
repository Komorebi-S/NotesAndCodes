#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct _node{
	int data;
	int high;//记录当前节点深度
	struct _node* left;
	struct _node* right;
}NODE;
/*创建新节点*/
NODE *construct_node(int data)
{
	NODE * tmp = (NODE *)malloc(sizeof(NODE));
	memset(tmp, 0, sizeof(NODE));
	tmp->high = 0;//新节点高度为0
	tmp->data = data;
	tmp->left = tmp->right = NULL;
	return  tmp;
}
/*中序遍历*/
void inorder_traversal(NODE * root)
{
	if(!root)
		return ;
	NODE * tmp = root;
	if(tmp){
		inorder_traversal(tmp->left);
		printf("%d(%d)  ",tmp->data, tmp->high);
		inorder_traversal(tmp->right);
	}
}
/*先序遍历*/
void preorder_traversal(NODE * root)
{
	if(!root)
		return ;
	NODE * tmp = root;
	if(tmp){
		printf("%d(%d)  ",tmp->data, tmp->high);
		preorder_traversal(tmp->left);
		preorder_traversal(tmp->right);
	}
}
int hight(NODE * root)
{
	if(!root)
		return -1;
	else
		return root->high;
}
int max(int l, int r)
{
	return (l > r ? l : r);
}

/*这四个旋转函数非NULL在调用之前就已经判断*/
NODE *single_rotate_left(NODE * root)//整个树是左旋的单旋转
{
	NODE * tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;

	//旋转后深度随之变化，需要修改深度
	root->high = max(hight(root->left), hight(root->right)) + 1;
	tmp->high = max(hight(tmp->left), hight(tmp->right)) + 1;

	return tmp;//返回新的子树根节点
}
NODE *single_rotate_right(NODE * root)//整个树是右旋的单旋转
{
	NODE * tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;

	root->high = max(hight(root->left), hight(root->right)) + 1;
	tmp->high = max(hight(tmp->left), hight(tmp->right)) + 1;

	return tmp;
}
NODE *double_rotate_left(NODE * root)//整个树是左旋的双旋转
{
	root->left = single_rotate_right(root->left);//先将右子树右旋并接收改变之后的右子树的根节点
	return single_rotate_left(root);//再将整个树左旋
}
NODE *double_rotate_right(NODE * root)//整个树是右旋的双旋转
{
	root->right = single_rotate_left(root->right);//先将左子树左旋并接收改变之后的左子树的根节点
	return single_rotate_right(root);//再将整个树右旋
}
/*先根据二叉排序树规则插入，插完后进行平衡判断*/
NODE * add_node(NODE * root, NODE * newnode)
{
	if(!root)
		return newnode;

	NODE * tmp = root;
	if(tmp->data > newnode->data){
		tmp->left = add_node(tmp->left, newnode);//递归向左子树插入
		//旋转
		if(hight(tmp->left) - hight(tmp->right) == 2){
			if(tmp->left->data > newnode->data)//插到左子树左边，右旋(单旋转)
				tmp = single_rotate_left(tmp);	
			else//插到左子树右边，左子树先左旋整个树再右旋(双旋转)
				tmp = double_rotate_left(tmp);
		}
	}
	else if(tmp->data < newnode->data){
		tmp->right = add_node(tmp->right, newnode);//递归向右子树中插入
		//旋转
		if(hight(tmp->right) - hight(tmp->left) == 2){
			if(tmp->right->data < newnode->data)//插到右子树右边，左旋(单旋转)
				tmp = single_rotate_right(tmp);
			else//插到右子树左边，右子树先右旋整个树再左旋(双旋转)
				tmp = double_rotate_right(tmp);		
		}
	}
	tmp->high = max(hight(tmp->left), hight(tmp->right)) + 1;
	return tmp;
}
int main(int argc, char *argv[])
{	
	if(argc != 2){
		printf("please usage %s data_count\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int num = atoi(argv[1]);
	srand(time(NULL));
	NODE * root = NULL;
	int i;
	for(i = 0; i<num; i++){
		NODE * tmp = construct_node(rand()%1000);
		root = add_node(root, tmp);
	}
	/*打印出先序与中序遍历结果验证是否符合AVL树的规则创建*/
	inorder_traversal(root);
	printf("\n");
	preorder_traversal(root);
	printf("\n");
	return 0;
}
