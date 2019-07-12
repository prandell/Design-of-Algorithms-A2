/*
 * Problem 3 in Assignment 2
 * COMP20007 Design of Algorithms
 * Semester 1 2019
 *
 * Written by: [Patrick Randell (836026)]
 * CODE SAMPLED FROM WORKSHOP SOLUTIONS written by Tobias Edwards
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include "queue.h"
 #include <math.h>
 
//------------------------ Function Prototypes ---------------------------//
 
 typedef struct BST {
 	 int data;
 	 struct BST *left;
 	 struct BST *right;
 } node;
 
 node *create();
 node *sortedArrayToBST(int *arr, int start, int end);
 void preorder(node *root);
 int insertnode(node *root,node *temp, int depth, int *nodes);
 int inorderToArray(node *root, int* arr, int pos);
 void QueueLevel(node* root, int level, Queue *queue); 
 int height(node* node); 
 void LevelOrderToQueue(node* root,Queue *queue);
 
//------------------------------ Constants -------------------------------//
 
//------------------------------------------------------------------------//
/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_3();

int main(int argc, char **argv) {
  problem_3();
  return 0;
}

/* --- DO NOT CHANGE THE CODE ABOVE THIS LINE --- */
//------------------------------------------------------------------------//
//-------------------------- Main Solutions -----------------------------//

/* Problem 3 solution that uses BST struct data type to create a BST from
	original input, finding the height and the number of unique nodes as
	it's created. BST is then output to a sorted array that is used to create
	a balanced BST */
void problem_3() {
	int n, i, value;
	int tempheight=0, height=0, nodes=0,size;

	//retrieve number of input values
	scanf("%d", &n);
	
	//initialise empty array to contain at most n values (if all unique).
	int c[n];
	
	//initialise nodes to create BST
	node *root=NULL,*temp;
	
	//Loop to retrieve input, creating the BST dynamically and updating the
	// height as we add new values. Also updates the number of unique nodes
	for (i = 0; i < n; i++) {
		scanf("%d", &value);
		temp=create(value);
		
		if(root==NULL) {
			root=temp;
			nodes++;
			
		} else {
			
			tempheight=1+insertnode(root,temp,0,&nodes);
			if (tempheight>height) {
				height=tempheight;
			}
		}
	}
	
	//Outputs the number of unique nodes in the BST, as it stores them in 
	// an array in sorted order. (Since inorder traversals are ordered).
	size=inorderToArray(root, c, 0)-1;
	
	//print some preliminary values
	printf("%d\n", nodes);
	printf("%d\n", height);
	
	//Creates a Balanced binary search tree from sorted array
	node *newroot=sortedArrayToBST(c,0,size);
	
	//Level order traverses the BST into a queue, imputating missing
	// leaf nodes as -1.
	Queue *queue= new_queue();
	LevelOrderToQueue(newroot,queue);
	
	//Out puts the lines needed to print the entire tree in level order
	size=queue_size(queue);
	printf("%d\n", size);
	
	//prints tree
	for (i=0;i<size;i++) {
		printf("%d\n", queue_dequeue(queue));
	}
	free_queue(queue);
	
}

//-------------------------- Helper Functions -----------------------------//

//Creates a Balanced BST by recursively dividing a sorted array.
node *sortedArrayToBST(int *arr, int start, int end) {

    //Base case
    if (start > end) {
        return NULL; 
    }
    
    //Get the middle element and make it root
    int mid = (start + end)/2; 
    node *root;
    root = create(arr[mid]);
  
    //Construct subtrees Inorder
    root->left  = sortedArrayToBST(arr, start, mid-1); 
    root->right = sortedArrayToBST(arr, mid+1, end); 
  
    return root;
}

//Helper function to create a new node
node *create(int data) {
	node *temp;
	temp=(node*)malloc(sizeof(node));
	temp->data=data;
	temp->left=temp->right=NULL;
	return temp;
}

//Special insert node function, that keeps track of height and number of 
// unique nodes. 
int insertnode(node *root,node *temp, int depth, int *nodes) {
	
	//If smaller go left
	if(temp->data < root->data) {
		
		//continue left if not empty, increase depth.
		if(root->left!=NULL) {
			return insertnode(root->left,temp,depth+1,nodes);
		
		//place node when found its position, return depth.
		} else {
			root->left=temp;
			*nodes=*nodes+1;
			return depth+1;
		}
	}
	
	//if bigger go right
	if(temp->data>root->data) {
		
		//continue if not empty, increase depth.
		if(root->right!=NULL) {
			return insertnode(root->right,temp,depth+1,nodes);
			
		//place node when found its position, return depth.
		} else {
			root->right=temp;
			*nodes=*nodes+1;
			return depth+1;
		}
	}
	
	//Note, there is no case for if node is equal to another, thus it just
	//returns 0 in that case.
	return 0;
}


//Traverses unbalanced BST (or any) Inorder, outputting to an array. 
//Essentially sorts nodes and outputs to array.
int inorderToArray(node *root, int* arr, int pos) {
	
	if(root!=NULL) {
		pos=inorderToArray(root->left,arr,pos);
		
		//increase position in array when placed
		arr[pos]=root->data;
		pos++;
		
		pos=inorderToArray(root->right,arr,pos);
		
		//return position
		return pos;
	}
	
	return pos;
}

  
//Outputs level order of BST to a queue.
void LevelOrderToQueue(node* root, Queue *queue) { 
    int h = height(root); 
    int i; 
    for (i=1; i<=h; i++) {
        QueueLevel(root, i,queue);
    }
} 
  
// Queues all nodes at the current level.
void QueueLevel(node* root, int level, Queue *queue) { 
	
	//Base case to imputate leaf nodes
    if (root == NULL) {
    	queue_enqueue(queue, -1);
        return; 
    }
    
    //Base case to enqueue next value in Level order
    if (level == 1) {
    	queue_enqueue(queue,root->data);
    
    //Continue to next level.
    } else if (level > 1) { 
        QueueLevel(root->left, level-1,queue); 
        QueueLevel(root->right, level-1,queue); 
    } 
} 
  
//Compute Tree height. Furthest distance from root to leaf.
int height(node* node) { 
	
    if (node==NULL) {
        return 0; 
    } else { 
    	
        //Get height of both subtrees.
        int heightL = height(node->left); 
        int heightR = height(node->right); 
  
        //take larger
        if (heightL > heightR) {
            return(heightL+1); 
        } else { 
        	return(heightR+1); 
        }
    } 
}
//------------------------------------------------------------------------//