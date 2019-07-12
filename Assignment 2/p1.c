/*
 * Problem 1 in Assignment 2
 * COMP20007 Design of Algorithms
 * Semester 1 2019
 *
 * Written by: [ PATRICK RANDELL ]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------ Function Prototypes ---------------------------//
void intswap(int *first,int *second);
int leftchild(int i);
int rightchild(int i);
int parent(int i);
void rightheapify(int *arr, int n, int i);
void heapify(int *arr, int n, int i);

//------------------------------------------------------------------------//
/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_1_a();
void problem_1_b();
void print_usage_and_exit(char **argv);

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage_and_exit(argv);
  }

  /* If the argument provided is "a" then run problem_1_a(),
   * run problem_1_b() for "b", and fail otherwise. */
  if (strcmp(argv[1], "a") == 0) {
    problem_1_a();
  } else if (strcmp(argv[1], "b") == 0) {
    problem_1_b();
  } else {
    print_usage_and_exit(argv);
  }

  return 0;
}

/* Print the usage information and exit the program. */
void print_usage_and_exit(char **argv) {
  fprintf(stderr, "usage: %s [ab]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* --- DO NOT CHANGE THE CODE ABOVE THIS LINE --- */
//------------------------------------------------------------------------//
//-------------------------- Main Solutions ------------------------------//

/* Problem 1a Solution, using standard heapify concept to create the heap */
void problem_1_a() {
	int n,c,i;
	scanf("%d", &n);
	int arr[n+1];
	arr[0]=0;
	c=n;
	for (i=1;i<=n;i++) {
		scanf("%d", &arr[i]);
	}
	for (i = parent(c); i > 0; i--) {
        heapify(arr, n, i);
    }
    for (i=1;i<=n;i++) {
    	printf("%d\n",arr[i]);
    }
		
}


/* Problem 1b Solution, right heapifies the already heapified solution */
void problem_1_b() {
	int n,c,i ;
	scanf("%d", &n);
	int arr[n+1];
	arr[0]=0;
	c=n;
	for (i=1;i<=n;i++) {
		scanf("%d", &arr[i]);
	}
	for (i = parent(c); i > 0; i--) {
        heapify(arr, n, i);
    }
    for (i=1;i<(n+1)/2;i++) {
    	rightheapify(arr,n,i);
    }
    for (i=1;i<=n;i++) {
    	printf("%d\n",arr[i]);
    }
}


//-------------------------- Helper Functions -----------------------------//

//Finds a parents right child index
int rightchild(int i) {
	return 2*i+1;
}

//Finds a parents left child index
int leftchild(int i) {
	return 2*i;
}

//Finds a right childs parent index
int parent(int i) {
	return (i-1)/2;
}

//Swaps two intergers using their pointers.
void intswap(int *first,int *second) {
	int temp= *first;
	*first = *second;
	*second=temp;
	return;
}

//Recursive heapify function that gets passed subtree parent nodes
void heapify(int *arr, int n, int i)  {
	
	int left, right, largest;
	left=leftchild(i);
	right =rightchild(i);
	largest=i;
	if(left<n && arr[left] > arr[i]) {
		largest = left;
	}
	
	if (right<=n && arr[right] > arr[largest]) {
		largest = right;
	}
	
	if(largest != i) {
		intswap(&arr[i],&arr[largest]);
		heapify(arr,n,largest);
	}
}

//Function to right heapify an existing heap.
void rightheapify(int *arr, int n, int i)  {
	int left, right;
	left=leftchild(i);
	right =rightchild(i);
	if (right<=n && arr[right] < arr[left]) {
		intswap(&arr[right], &arr[left]);
		heapify(arr,n,left);
	}
}

//------------------------------------------------------------------------//