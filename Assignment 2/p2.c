/*
 * Problem 2 in Assignment 2
 * COMP20007 Design of Algorithms
 * Semester 1 2019
 *
 * Written by: [Patrick Randell (836026)]
 * CODE SAMPLED FROM WORKSHOP SOLUTIONS written by Tobias Edawrds
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stack.h"	

//------------------------------ Constants --------------------------------//
//Initial distance values when updating arrays
#define INF INT_MAX
//dimension of distances values in solution arrays
#define VALUE 0 
//dimension of visited markers (node preceeding current node)
#define VISITEDBY 1
//dimension of k solution to k-edges problem
#define USING_K_EDGES 2
//Dimension of shortest path array
#define TWODEE 2
//Value assigned to unvisited nodes
#define UNVISITED -1

//------------------------------------------------------------------------//
/* --- DO NOT CHANGE THE CODE BELOW THIS LINE --- */

void problem_2_a();
void problem_2_b();
void print_usage_and_exit(char **argv);

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage_and_exit(argv);
  }

  /* If the argument provided is "a" then run problem_2_a(),
   * run problem_2_b() for "b", and fail otherwise. */
  if (strcmp(argv[1], "a") == 0) {
    problem_2_a();
  } else if (strcmp(argv[1], "b") == 0) {
    problem_2_b();
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


/* Problem 2a implementation using 2D arrays that are updated for every newly
	read in edge. */
void problem_2_a() {
	
	//Variable declarations
	int shortest,edges=0,outdegree,from ,to, weight,n,first;
	int i, j, l;
	
	//Get number of nodes
	scanf("%d", &n);
	
	//Create 2D array for all nodes. 1st for value, 2nd for visited marker, 
	// that gets assigned the value of the node preceeding it in the shortest
	// path.
	int arr[n][TWODEE];
	
	//Set initial distances to INF, and mark all as unvisited (-1).
	for (i=0;i<n;i++) {
		arr[i][VALUE]=INF;
		arr[i][VISITEDBY]=UNVISITED;
	}
	
	//Set first node as visited with distance of 0.
	arr[0][VALUE]=0;
	arr[0][VISITEDBY]=0;
	
	//Taking advantage of topological order (every next node read in, will
	// not visit any previous nodes), update shortest distance for every new
	// edge.
	for (i=0; i<n;i++) {
		from=i;
		
		//get outdegree
		scanf("%d",&outdegree);
		
		j=0;
		while (j<outdegree) {
			//get edge
			scanf("%d %d",&to,&weight);
			
			//If from node hasnt already been visited, no path back to source,
			// so can skip over this iteration. (Property of topo ordering).
			if (arr[from][VISITEDBY]==UNVISITED) { 
				j++; 
				continue;
			}
			
			//only update values if theyre reduced, dynamic! Update 2nd dim
			// to contain value of its preceeding node.
			if ((arr[from][VALUE]+weight)<arr[to][VALUE]) {
				arr[to][VALUE]=(arr[from][VALUE]+weight);
				arr[to][VISITEDBY]=from;
			}
			j++;
		}
	}
	
	//retrieve the shortest distance to last node, which is the last value 
	//in the shortest distance array.
	l=n-1;
	shortest=arr[l][VALUE];
	
	//Using a stack, we can easily rebuild the path back to the source using 
	// the preceeding nodes.
	Stack *stack= new_stack();
	stack_push(stack,l);
	
	//loop that traces back to the source node using the 2nd Array Dim.
	while(l>0) {
		stack_push(stack,arr[l][VISITEDBY]);
		l=arr[l][VISITEDBY];
		edges++;
	}
	
	//If we couldnt reach the source, we found a node with -1 in the 2nd
	// array dimension, so there's no path!
	first=stack_pop(stack);
	if (first!=0) {
		printf("No Path\n");
		return;
	}
	
	//Printing preliminary results, before printing the path from the stack.
	printf("%d\n",shortest);
	printf("%d\n", edges);
	printf("%d\n",first);
	while (stack_size(stack)) {
		printf("%d\n",stack_pop(stack));
	}
	free_stack(stack);

}

//------------------------------------------------------------------------//

/* Problem 2b implementation that uses 3D array, similarly to 2a, to keep
	track of preceeding nodes, but also preceeding solutions to the
	dynamic problem of k-1 edges. This is needed to rebuild the path 
	back to the source. */
void problem_2_b() {
	
	//Variable declarations
	int shortest,edges=0,outdegree,from ,to, weight,n,first,atmostk;
	int i, j, k,l,y;
	
	//Retrieve node numbers and at most K edge value
	scanf("%d %d", &n,&atmostk);
	
	//Generate adjacency matrix to store the graph, and 3D array to store 
	//dynamic updates of the 0-> k+1 iterations.
		// 1st dim: distance value
		// 2nd dim: preceeding node/visited marker
		// 3rd dim: solution to shortest path using k-1 edges.
	int graph[n][n];
	int arr[n][TWODEE][atmostk+1];
	
	//loop to initialise distances to INF, visited to -1, for all solutions.
	// Also sets adjacency matrix values to INF.
	for (i=0;i<n;i++) {
		
		//Solution array
		for (j=0;j<=atmostk;j++) {
			arr[i][VALUE][j]=INF;
			arr[i][VISITEDBY][j]=UNVISITED;
		}

		//Adj matrix
		for (k=0;k<n;k++) {
			graph[i][k]=INF;
		}
	}
	
	//Initialise first node to 0 distance, visited, for both the 0th and 1st
	// solution.
	arr[0][VALUE][0]=0;
	arr[0][VISITEDBY][0]=0;
	arr[0][VALUE][1]=0;
	arr[0][VISITEDBY][1]=0;

	//Generate Adjacency matrix from input data.
	for (i=0; i<n;i++) {
		from=i;
		scanf("%d",&outdegree);
		j=0;
		while (j<outdegree) {
				scanf("%d %d",&to,&weight);
				graph[from][to]=weight;
				j++;
		}
	}
	
	//Update the solution array to contain the solution to the problem using
	// at most 0 edges, then 1, then 2... etc. Storing each consecutive
	// iteration in the next index of the 3rd array dimension.
	for (i=0;i<atmostk;i++) {
		for (j=0;j<n;j++) {
			
			//If from node not visited, skip iteration (topological sort).
			if (arr[j][VISITEDBY][i]==UNVISITED) {
				continue;
				
			} else {
				
				//for all edges from j to k, update distance if its shorter.
				for(k=0;k<n;k++) {
					if ((graph[j][k]!=INF) && 
						(arr[k][VALUE][i+1] > arr[j][VALUE][i]+graph[j][k])) {
					
						arr[k][VALUE][i+1]=arr[j][VALUE][i]+graph[j][k];
						arr[k][VISITEDBY][i+1]=j;
					}
				}
			}
		}
	}

	//Shortest is the last element of the last update.
	shortest=arr[n-1][VALUE][atmostk];
	Stack *stack= new_stack();
	
	//Rebuild path by going back through the solutions to the k iterations,
	// finding the preceeding nodes, and add to stack. Manually adding first
	// two values.
	
	// push last node and its predecessor
	y=n-1;
	stack_push(stack,y);
	stack_push(stack,arr[y][VISITEDBY][atmostk]);
	
	//increase number of edges and reassign loop variables to go back through
	// predecessors, and k solutions.
	edges++;
	y=arr[y][VISITEDBY][atmostk];
	l=atmostk-1;
	
	//Loop to create stack.
	while(y>0) {
		stack_push(stack,arr[y][VISITEDBY][l]);
		y=arr[y][VISITEDBY][l];
		edges++;
		l--;
	}
	
	//If we didnt reach the source node, theres no path.
	first =stack_pop(stack);
	if (first!=0) {
		printf("No Path\n");
		return;
	}
	
	//Print required values before printing path
	printf("%d\n",shortest);
	printf("%d\n", edges);
	printf("%d\n",first);
	while (stack_size(stack)) {
		printf("%d\n",stack_pop(stack));
	}
	free_stack(stack);
}

//------------------------------------------------------------------------//