#include <stdio.h>
#include <stdlib.h>

typedef struct struct_node {
    struct struct_node *left;
    struct struct_node  *right;
    int data;
    int h;
} node;

node* create_node(int value){
    node *res = malloc(sizeof(node));
    res->left = NULL;
    res->right = NULL;
    res->h = 1;
    res->data = value;
    return res;
}

int calcH(node *n){
    int right = n->right ? n->right->h + 1: 1;
    int left = n->left ? n->left->h + 1: 1;
    return left > right ? left : right;
}

node* rightRotation(node* n){
    node *root = n->left;
    node *C = root->right;
    root->right = n;
    n->left = C;
    n->h = calcH(n);
    root->h = calcH(root);
    return root;
}

node* leftRotation(node *n){
    node* root = n->right;
    node* C = root->left;
    root->left = n;
    n->right = C;
    n->h = calcH(n);
    root->h = calcH(root);
    return root;
}

node *balanceRight(node *n){
    int hL = n->left->left ? n->left->left->h : 0;
    int hC = n->left->right ? n->left->right->h : 0;
    if(hC > hL) {
        n->left = leftRotation(n->left);
    }
    return rightRotation(n);
}

node* balanceLeft(node *n){
    int hR = n->right->right ? n->right->right->h : 0;
    int hC = n->right->left ? n->right->left->h : 0;
    if(hC > hR) {
        n->right = rightRotation(n->right);
    }
    return leftRotation(n);
}

node* balanceTree(node* n){
    int right = n->right ? n->right->h : 0;
    int left = n->left ? n->left->h : 0;
    int diff = right - left;

    switch (diff){
        case -2:
            return balanceRight(n);
        case 2:
            return balanceLeft(n);
        default:
            return n;
    }
}

node* insert_in_tree( node* root, int value){
    if(root == NULL){
        return create_node(value);
    }

    if(value < root->data){
        root->left = insert_in_tree(root->left, value);
    } else if (value > root->data){
        root->right = insert_in_tree(root->right, value);
    } else return root;

    root->h = calcH(root);

    return balanceTree(root);
}

node* clear_tree(node* root){
    if(root != NULL) {
        clear_tree(root->left);
        clear_tree(root->right);
        free(root);
    }
    return NULL;
}

node* remove_from_tree(node* root, int value){
	if(root == NULL) return NULL;

	if(root->data == value){
		if(root->left == NULL){
			if(root->right == NULL){
				free(root);
				return NULL;
			} else {
				node *newRoot = root->right;
				free(root);
				return newRoot;
			}
		}else{
			if(root->right == NULL){
				node *newRoot = root->left;
				free(root);
				return newRoot;
			} else {
				node *minRightValue = root->right;
				while(minRightValue->left != NULL) minRightValue = minRightValue->left;
				root->data = minRightValue->data;
				root->right = remove_from_tree(root->right, root->data);
			}
		}
	} else if(root->data > value){
		root->left = remove_from_tree(root->left, value);
	} else {
		root->right = remove_from_tree(root->right, value);
	}

	root->h = calcH(root);
	return balanceTree(root);
}

typedef struct struct_item{
	void *payload;
	struct struct_item* next;
} item;

typedef struct struct_queue{
	item* begin;
	item* end;
}queue;

queue* create_queue(){
	queue* res = malloc(sizeof(queue));
	res->begin = NULL;
	res->end = NULL;
	return res;
}

void queue_push(queue* q, void *payload){
	item *i = malloc(sizeof(item));
	i->next = NULL;
	i->payload = payload;

	if(q->end == NULL){
		q->begin = i;
		q->end = i;
	}else{
		q->end->next = i;
		q->end = i;
	}
}

void* queue_pop(queue *q){
	if(q->begin == NULL) return NULL;
	item *current = q->begin;
	q->begin = q->begin->next;
	if(q->begin == NULL) q->end = NULL;
	void* payload = current->payload;
	free(current);
	return payload;
}

void print_tree( node* root){
	queue* q_current = NULL;
	queue* q_next = create_queue();
	queue_push(q_next, (void *) root);


	puts("------BEGIN TREE------");
	int isNotLastLevel;
	do{
		free(q_current);
		q_current = q_next;
		q_next = create_queue();
		void* payload;

		isNotLastLevel = 0;
		while(q_current->begin != NULL){
			payload = queue_pop(q_current);
			if(payload != NULL){
				node* n = (node *)payload;
				printf("%d ", n->data);
				queue_push(q_next, n->left);
				queue_push(q_next, n->right);
				isNotLastLevel = isNotLastLevel || n->left || n->right;
			}else{
				printf("_ ");
				queue_push(q_next, NULL);
				queue_push(q_next, NULL);
			}
		}
		puts("");
	} while(isNotLastLevel);
	puts("-------END TREE-------");

	free(q_current);
	while(q_next->begin != NULL) queue_pop(q_next);
	free(q_next);
}

int main() {
    node* t = NULL;
    for(int i = 1; i < 16; ++i){
    	printf("insert value %d\n", i);
	    t = insert_in_tree(t, i);
		print_tree(t);
    }

    for(int i = 8; i < 16; ++i){
	    printf("remove value %d\n", i);
	    t = remove_from_tree(t, i);
	    print_tree(t);
    }

    t = clear_tree(t);
	return 0;
}
