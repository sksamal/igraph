/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include <string.h>
#include "igraph_paths.h"
#define MLEVELS 7
#define LSIZE 7 

void isATFree(igraph_t *g, char *gname, int depth, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1);
void isDP(igraph_t *g, char *gname, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1);
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text, igraph_vector_t *map, igraph_bool_t inverse);
void LBFS(igraph_t *g, igraph_vector_t *Y, igraph_vector_t *X, igraph_vector_t *label, igraph_vector_t *map);
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map);
int minPaths(igraph_t* g, char *gname, int depth, igraph_vector_t *Y); 
igraph_bool_t processForAT(igraph_t *g, char *gname, int depth, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *g1);
igraph_bool_t processForDP(igraph_t *g, char *gname, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *g1);
void cutNeighbors(igraph_t* g,int j,int *loc,igraph_vector_t *bni, igraph_vector_t *cni, igraph_vector_t *nni);
igraph_bool_t isDominSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *fnn, igraph_t *g1);
igraph_bool_t isLevelSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1);
igraph_bool_t isAdjSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1);
void forwardNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *fnn);
void currentNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *cnn);
void getNeighbors(igraph_t* g,int j, int *loc,igraph_vector_t *ne, int dir);
igraph_bool_t isHamiltonian(igraph_t* g, int *loc, int l);
void isHamUsingOurAlgo(igraph_t *g, igraph_bool_t *iso, char *path);
void isHamUsingLAD(igraph_t *g, igraph_bool_t *iso, char *path);
void isHamUsingVF2(igraph_t *g, igraph_bool_t *iso,char *path);
int read_graph_edgelist(igraph_t *graph, char *description, FILE *instream, igraph_integer_t n, igraph_bool_t directed); 

// dir=-1 means left, dir=0 current, dir=1 means right 
/* Get the forward or backward or current neighbor based on the value of dir */
void getNeighbors(igraph_t* g, int j, int *loc, igraph_vector_t *ne, int dir) {
      igraph_vector_t left,current,right;
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);

      /* Based on dir, update vector ne and return */
      if(dir>0) cutNeighbors(g,j,loc,&left,&current,ne);
      if(dir<0) cutNeighbors(g,j,loc,ne,&current,&right);
      else cutNeighbors(g,j,loc,&left,ne,&right); 
	
      igraph_vector_destroy(&left);
      igraph_vector_destroy(&current);
      igraph_vector_destroy(&right);
}

/* Same as igraph_read_graph_edgelist with ability to read description */
int read_graph_edgelist(igraph_t *graph, char *description, FILE *instream, igraph_integer_t n, igraph_bool_t directed) {

  /* read comment from first line then pass it to the original function */
     size_t len;
     int c = getc (instream);
     if(c == '-')
     {	getline(&description,&len,instream);
/*	printf("Description: %s",description);*/	}
     else
     { 	ungetc (c, instream); description[0]='\0'; 	}

     return igraph_read_graph_edgelist(graph,instream,n,directed); 
}

/* This method cuts the neighbors of a vertex v into left, right and current neighbors
 * based on the grid-labelling scheme */
void cutNeighbors(igraph_t* g, int j, int *loc, igraph_vector_t *bni, igraph_vector_t *cni, igraph_vector_t *nni) {

   /* Get all neighbours */
   igraph_vector_t ni;
   igraph_vector_init(&ni, 8);
   igraph_neighbors(g,&ni,j,IGRAPH_ALL);

   /* resize various vectors  for backward, current and next*/
//   igraph_vector_init(bni, igraph_vector_size(&ni));
//   igraph_vector_init(cni, igraph_vector_size(&ni));
//   igraph_vector_init(nni, igraph_vector_size(&ni));

   /* Find appropriate level to which the vertex belongs to */
   int i=0;
   while(j>=loc[i]) i++;

   /* Find the first vertex in backward (b), current(c) and next(n) level */
   int b=(i==0)?-999:((i==1)?0:loc[i-2]);
   int c=(i==0)?0:loc[i-1];
   int n=loc[i];

  /* For every neighbor, based on its position, put it in appropriate neighbor vector */
  for (int k=0; k<igraph_vector_size(&ni); k++) {
    int w = (int) VECTOR(ni)[k];
//    printf("\nj=%d :: i=%d, k=%d, w=%d, b=%d, c=%d, n=%d", j, i, k, w, b, c, n);
    if(w>=b && w<c)
	igraph_vector_insert(bni,0,w);	
    if(w>=c && w<n)
	igraph_vector_insert(cni,0,w);	
    if(w>=n)
	igraph_vector_insert(nni,0,w);
//    fprintf(f, " %li", (long int) VECTOR(*v)[i]);
  }
   /* Finally resize the vectors and return */
   igraph_vector_resize_min(bni); 
   igraph_vector_resize_min(cni); 
   igraph_vector_resize_min(nni); 

}

/* This method finds all non-neighbors of a vertex j (in level i) in it's next level (i+1)
 * returns them in a vector*/
void forwardNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *fnn) {

   /* First find j's level */
   int i=0;
   while(j>=loc[i]) i++;
   if(i==locsize-1) return ;

   /* For every vertex in next level, if they are not connected add it in the vector */
   igraph_bool_t connected;
   for(int k=loc[i]; k<loc[i+1];k++) {
      igraph_are_connected(g,j,k,&connected);
      if(!connected)
	igraph_vector_insert(fnn,0,k);
   }
   igraph_vector_resize_min(fnn);
}

/* This method finds all non-neighbors of a vertex j (in level i) in the same level (i)
 * returns them in a vector*/
void currentNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *cnn) {

   /* First find j's level */
   int i=0;
   while(j>=loc[i]) i++;
   if(i==locsize) return ;

   /* For every vertex in next level, if they are not connected add it in the vector */
   igraph_bool_t connected;
   for(int k=loc[i-1]; k<loc[i];k++) {
      igraph_are_connected(g,j,k,&connected);
      if(!connected)
	igraph_vector_insert(cnn,0,k);
   }
   igraph_vector_resize_min(cnn);
}
/* This method checks the local AT-Free condition in both the directions. Returns a 
 * true if the localcheck worked, else adds edges in g1 (which is essentially a copy of g)
 * to satisfy the local check . It takes j and it's forward non-neighbors as
 * input 
 * It basically checks if it there is a dominating pair and if there are vertices 
 * avoiding that - part of AT-Free condition*/
igraph_bool_t isDominSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *fnn, igraph_t *g1) {

   igraph_bool_t connected, atfree, isatfree=1;

  /* For each forward non-neighbors of j */ 
  for (int i=0; i<igraph_vector_size(fnn)-1; i++) {
    	int w = (int) VECTOR(*fnn)[i];
   	igraph_vector_t ibni,icni,inni;
   	igraph_vector_init(&ibni, 1);
   	igraph_vector_init(&icni, 1);
   	igraph_vector_init(&inni, 1);
	/* Get backward neighbors of w and see if it is connected to j */
	cutNeighbors(g,w,loc,&ibni,&icni,&inni);
	for (int k=0; k<igraph_vector_size(&ibni)-1; k++) {
	  int v = (int) VECTOR(ibni)[k];	
	  igraph_are_connected(g,v,j,&connected);
	  atfree=1;
	  /* If not connected to j, find its backward neighbors and 
 	   * see if it is connected to j */
	  if(!connected)
	   {
   		igraph_vector_t kbni,kcni,knni;
   		igraph_vector_init(&kbni, 1);
   		igraph_vector_init(&kcni, 1);
   		igraph_vector_init(&knni, 1);
		cutNeighbors(g,v,loc,&kbni,&kcni,&knni);
		/*Randomly decide if we need to add edges (u,j) 
                 *for all u with backward neighbors  */
		igraph_bool_t shallAdd = rand()%2;
		for (int l=0; l<igraph_vector_size(&kbni)-1; l++) {
		  int u = (int) VECTOR(kbni)[l];
	          igraph_are_connected(g,u,j,&connected);
		  if(!connected) {
//		     if(atfree) printf("\nB(%d,%d): (",w,j);
		     atfree=0;
		     /* Did I connect it already(just now?) */
		     igraph_are_connected(g1,u,j,&connected);
//		     if(!connected) printf("Connect %d---%d and ",u,j);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,j);
		  }
	        }
//		if(!atfree) { if(shallAdd) printf(")* "); else printf(") "); }
                /* If not atfree, and edges are not added to backward neighbors earlier,
                 * add edge (v,j) in g1 */ 
		if(!atfree) {
		  /* Did I connect it already(just now?) */
		  igraph_are_connected(g1,v,j,&connected);
//		  if(!connected) printf("OR (Connect %d---%d)",v,j);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,j);
//		  if(!shallAdd) printf("*");
		}
		isatfree=isatfree & atfree;
		igraph_vector_destroy(&kbni);
		igraph_vector_destroy(&kcni);
		igraph_vector_destroy(&knni);
	   }
	}
	igraph_vector_destroy(&ibni);
	igraph_vector_destroy(&icni);
	igraph_vector_destroy(&inni);


	/* Do the exact same thing with j in forward direction */
   	/* Find forward non-neighbors of j */
   	igraph_vector_t jbni,jcni,jnni;
   	igraph_vector_init(&jbni, 1);
   	igraph_vector_init(&jcni, 1);
   	igraph_vector_init(&jnni, 1);
   	cutNeighbors(g,j,loc,&jbni,&jcni,&jnni);

	/* See if any of them is connected to w */
	for (int k=0; k<igraph_vector_size(&jnni)-1; k++) {
	  int v = (int) VECTOR(jnni)[k];	
	  igraph_are_connected(g,v,w,&connected);
	  atfree=1;

	  /* If not connected to w, find its forward neighbors and 
 	   * see if it is connected to w */
	  if(!connected)
	   {
   		igraph_vector_t kbni,kcni,knni;
   		igraph_vector_init(&kbni, 1);
   		igraph_vector_init(&kcni, 1);
   		igraph_vector_init(&knni, 1);
		cutNeighbors(g,v,loc,&kbni,&kcni,&knni);
		/* Randomly decide where to add edge at (v,j) or 
                 * for all u at (u,j) */ 	
		igraph_bool_t shallAdd = rand()%2;
		for (int l=0; l<igraph_vector_size(&knni)-1; l++) {
		  int u = (int) VECTOR(knni)[l];
	          igraph_are_connected(g,u,w,&connected);
		  if(!connected) {
//		     if(atfree) printf("\nF(%d,%d): ( ",j,w);
		     atfree=0;
		     igraph_are_connected(g1,u,w,&connected);
//		     if(!connected) printf("Connect %d---%d and ",u,w);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,w); 
		  }
	        } 
//		if(!atfree) {if(shallAdd) printf(")* "); else printf(") "); }
		if(!atfree) {
		  igraph_are_connected(g1,v,w,&connected);
//		  if(!connected) printf("OR (Connect %d---%d)",v,w);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,w);
//		  if(!connected && !shallAdd) printf("*");
	        }
		isatfree=isatfree & atfree;
		igraph_vector_destroy(&kbni);
		igraph_vector_destroy(&kcni);
		igraph_vector_destroy(&knni);
	   }
	}
	igraph_vector_destroy(&jbni);
	igraph_vector_destroy(&jcni);
	igraph_vector_destroy(&jnni);

     }
  return isatfree;		
}

void isHamUsingOurAlgo(igraph_t *g, igraph_bool_t *iso, char *path) 
{
  igraph_integer_t dia;
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  int loc[dia+1];
  igraph_vector_t X,Y,map2;
  igraph_t gmap1;
  igraph_vector_init(&X,0);
  igraph_vector_init(&Y,0);
  igraph_bool_t isatfree = processForAT(g,"hamcheck",0,loc,&X,&Y,&map2,&gmap1);

  int start = 0, totextra=0;;
  int contr[dia+1], needc[dia+1], extra[dia+1], maxc[dia+1], minc[dia+1];
  int isHamiltonian = 1;
  printf("\nLevel [minc,maxc] needc extra contr");

   /* Calculate max-compoments (maxc), min-compoments(minc), extra/deficit (extra) of number
    * of vertices and amount it can contribute to the adjacent levels(contr) 
    * J0: In this loop, we check if condition is satisfied, without having to contribute
    * to its neighbors (simple hamiltonian)*/ 
   for(int l=0;l<dia+1;l++) {
	maxc[l] = loc[l]-start;  /* Maximum contribution or number of vertices in the elvel */
        minc[l] =0;
/* 	int visited[maxc[l]];		
	for(int i=0;i<maxc[l];i++)
	   visited[i]=0; */

      /* For each level, extract the subgraph and find minPaths */
      igraph_t subg;
      igraph_vs_t vs;
      igraph_vs_seq(&vs,start,loc[l]-1);
//      igraph_vector_print(&tlevel);
//      printf("\n[%s]|%s L%d from %d to %d",gname,depthstring,i,j,loc[i]-1);
      igraph_induced_subgraph(g,&subg,vs,IGRAPH_SUBGRAPH_AUTO);
      igraph_vs_destroy(&vs);
      char ggname[100];
      char gname[100]="testing";
      sprintf(ggname,"%s_subg%d",gname,l);
      igraph_vector_t subY;
      igraph_vector_init(&subY,0);
      minc[l] = minPaths(&subg, ggname,1,&subY);
      igraph_destroy(&subg);
      igraph_vector_destroy(&subY);
	/* Computes the no of components in the level */
/*	for(int i=0;i<maxc[l];i++) {
	   if(visited[i]==0) visited[i]=++minc[l];	
	   if(visited[i]<0) continue;
   	   igraph_vector_t snei;
    	   igraph_vector_init(&snei,1);
	   getNeighbors(g,start+i,loc,&snei,0); 
  //     	   printf("\n\t start=%d, start+i=%d,visited[i]=%d size(snei)=%d cc=%d",start,start+i,visited[i],igraph_vector_size(&snei)-1,minc[l]);
	   for (int k=0; k<igraph_vector_size(&snei)-1; k++) {
	  	int u = (int) VECTOR(snei)[k];
	  	if(visited[u-start]==0) visited[u-start]=abs(visited[i]);
//		printf("\n\t\tu=%du-start=%d, visited[u-start]=%d",u,u-start,visited[u-start]);
	   }
	   visited[i]*=-1; */  /*Over for this vertex */
/*	   igraph_vector_destroy(&snei);
	} */

       /* Simple hamiltonian condition, 122..221 */
       if(l==0 || l== dia) needc[l]=1;
       else needc[l]=2;

       if(needc[l]>maxc[l]) isHamiltonian = 0;  /*Flag if need is more than max it can contribute */
       extra[l]=(needc[l]-minc[l]);
       contr[l]=(maxc[l]-needc[l]);
       //printf("\nLevel [minc,maxc] needc extra contr");
       printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
       start = loc[l];
       totextra+=extra[l];
       isHamiltonian = isHamiltonian && (extra[l]>=0);
   }
	/* If all needs are met, a simple HamCycle exists and return  else continue*/
	if( !isHamiltonian ) printf("\nJ0[Simple]:Graph is non-hamiltonian");
	else { printf("\nJ0[Simple]:Graph is hamiltonian"); *iso = 1; }

	/* Search for complex hamcycle conditions, i.e when needs can be satisfied using
           its neighbors */ 
	totextra=0; isHamiltonian=1;
   	printf("\n\nLevel [minc,maxc] needc extra contr");
   	for(int l=0;l<dia+1;l++) {
		if(l!=0)  
		   while(extra[l]<0 && contr[l-1]>0)
			{ extra[l]++; contr[l-1]--; }
		if(l!=dia)
		   while(extra[l]<0 && contr[l+1]>0)
			{ extra[l]++; contr[l+1]--; }
//       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        totextra+=extra[l];
		isHamiltonian = isHamiltonian && (extra[l]>=0);
	 }

	for(int l=0;l<dia;l++)
       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	
	/* If a complex hamcycle exists, return true */	
	if(!isHamiltonian) 
	  { *iso = 0; printf("\nJ1[Complex]:Graph is non-hamiltonian"); }
	else 
	  { *iso = 1; printf("\nJ1[Complex]:Graph is hamiltonian"); }
  
	printf("\nNumber of vertices(l0 to l%d):",dia);	
	for(int l=0;l<dia;l++) 		   
	 printf("%d ",extra[l]); 

}

/* Uses igraph LAD algorithm to return a hampath 
 * if it exists  timesout after 180 seconds*/
void isHamUsingLAD(igraph_t *g, igraph_bool_t *iso, char *path) {
  igraph_t ring;
  igraph_vector_t map;
  igraph_vector_init(&map,0);
  /* Create a cycle with same number of vertices as g */
  igraph_ring(&ring, igraph_vcount(g), /*directed=*/ 0, /*mutual=*/ 0, /*circular=*/1);
  printf("\nInLAD");
  igraph_subisomorphic_lad(&ring, g, NULL, iso, &map,NULL, /* induced = */ 0, 180);
  if (!(*iso)) printf("\nLAD Isomorphism:G is non-hamiltonian");
  else { 
	printf("\nLAD Isomorphism: G is hamiltonian");
  	sprintf(path,"HC:");
  	for(int i=0;i<igraph_vector_size(&map);i++)
	   sprintf(path,"%s-%d",path,(int) VECTOR(map)[i]);
       }  
  igraph_vector_destroy(&map);
  igraph_destroy(&ring);
}

/* Uses igraph VF2 algorithm to return a hampath if 
 * possible */
void isHamUsingVF2(igraph_t *g, igraph_bool_t *iso,char *path) {
  igraph_t ring;
  igraph_vector_t map;
  igraph_vector_init(&map,0);
  /* Create a cycle with same number of vertices as g */
  igraph_ring(&ring, igraph_vcount(g), /*directed=*/ 0, /*mutual=*/ 0, /*circular=*/1);
  printf("In VF2");
  igraph_subisomorphic_vf2(g, &ring, NULL, NULL, NULL, NULL, iso, &map,NULL,NULL,NULL,NULL);
  if (!(*iso)) printf("\nVF2 Isomorphism:G is non-hamiltonian");
  else  {
	 printf("\nVF2 Isomorphism: G is hamiltonian");
	 sprintf(path,"HC:");
  	 for(int i=0;i<igraph_vector_size(&map);i++)
	 sprintf(path,"%s-%d",path,(int) VECTOR(map)[i]);
	}
  igraph_vector_destroy(&map);
  igraph_destroy(&ring);
}

/* Our algorithm for necessary condition of Hamiltonian cycle in ATFree graph
 * */
igraph_bool_t isHamiltonian(igraph_t *g,int *loc,int locsize) {

   int start = 0, totextra=0;;
   int contr[locsize], needc[locsize], extra[locsize], maxc[locsize], minc[locsize];
   int isHamiltonian = 1;
   printf("\nLevel [minc,maxc] needc extra contr");

   /* Calculate max-compoments (maxc), min-compoments(minc), extra/deficit (extra) of number
    * of vertices and amount it can contribute to the adjacent levels(contr) 
    * J0: In this loop, we check if condition is satisfied, without having to contribute
    * to its neighbors (simple hamiltonian)*/ 
   for(int l=0;l<locsize;l++) {
	maxc[l] = loc[l]-start;  /* Maximum contribution or number of vertices in the elvel */
        minc[l] =0;
/* 	int visited[maxc[l]];		
	for(int i=0;i<maxc[l];i++)
	   visited[i]=0; */

      /* For each level, extract the subgraph and find minPaths */
      igraph_t subg;
      igraph_vs_t vs;
      igraph_vs_seq(&vs,start,loc[l]-1);
//      igraph_vector_print(&tlevel);
//      printf("\n[%s]|%s L%d from %d to %d",gname,depthstring,i,j,loc[i]-1);
      igraph_induced_subgraph(g,&subg,vs,IGRAPH_SUBGRAPH_AUTO);
      igraph_vs_destroy(&vs);
      char ggname[100];
      char gname[100]="testing";
      sprintf(ggname,"%s_subg%d",gname,l);
      igraph_vector_t subY;
      igraph_vector_init(&subY,0);
      minc[l] = minPaths(&subg, ggname,1,&subY);
      igraph_destroy(&subg);
      igraph_vector_destroy(&subY);
	/* Computes the no of components in the level */
/*	for(int i=0;i<maxc[l];i++) {
	   if(visited[i]==0) visited[i]=++minc[l];	
	   if(visited[i]<0) continue;
   	   igraph_vector_t snei;
    	   igraph_vector_init(&snei,1);
	   getNeighbors(g,start+i,loc,&snei,0); 
  //     	   printf("\n\t start=%d, start+i=%d,visited[i]=%d size(snei)=%d cc=%d",start,start+i,visited[i],igraph_vector_size(&snei)-1,minc[l]);
	   for (int k=0; k<igraph_vector_size(&snei)-1; k++) {
	  	int u = (int) VECTOR(snei)[k];
	  	if(visited[u-start]==0) visited[u-start]=abs(visited[i]);
//		printf("\n\t\tu=%du-start=%d, visited[u-start]=%d",u,u-start,visited[u-start]);
	   }
	   visited[i]*=-1; */  /*Over for this vertex */
/*	   igraph_vector_destroy(&snei);
	} */

       /* Simple hamiltonian condition, 122..221 */
       if(l==0 || l==locsize-1) needc[l]=1;
       else needc[l]=2;

       if(needc[l]>maxc[l]) isHamiltonian = 0;  /*Flag if need is more than max it can contribute */
       extra[l]=(needc[l]-minc[l]);
       contr[l]=(maxc[l]-needc[l]);
       //printf("\nLevel [minc,maxc] needc extra contr");
       printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
       start = loc[l];
       totextra+=extra[l];
       isHamiltonian = isHamiltonian && (extra[l]>=0);
   }
	/* If all needs are met, a simple HamCycle exists and return  else continue*/
	if( !isHamiltonian ) printf("\nJ0[Simple]:Graph is non-hamiltonian");
	else { printf("\nJ0[Simple]:Graph is hamiltonian"); return 1; }

	/* Search for complex hamcycle conditions, i.e when needs can be satisfied using
           its neighbors */ 
	totextra=0; isHamiltonian=1;
   	printf("\n\nLevel [minc,maxc] needc extra contr");
   	for(int l=0;l<locsize;l++) {
		if(l!=0)  
		   while(extra[l]<0 && contr[l-1]>0)
			{ extra[l]++; contr[l-1]--; }
		if(l!=locsize-1)
		   while(extra[l]<0 && contr[l+1]>0)
			{ extra[l]++; contr[l+1]--; }
//       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        totextra+=extra[l];
		isHamiltonian = isHamiltonian && (extra[l]>=0);
	 }

	for(int l=0;l<locsize;l++)
       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	
	/* If a complex hamcycle exists, return true */	
	if(!isHamiltonian) printf("\nJ1[Complex]:Graph is non-hamiltonian");
	else printf("\nJ1[Complex]:Graph is hamiltonian");
  
	printf("\nNumber of vertices(l0 to l%d):",locsize-1);	
	for(int l=0;l<locsize;l++) 		   
	 printf("%d ",extra[l]); 
	return isHamiltonian;

}

int pathCover(igraph_t* g, char *gname, int depth, igraph_vector_t *Y, Paths *paths) {
   
  igraph_vector_t map1, map2, X, label1, label2;
  int n = igraph_vcount(g);
  int m = igraph_ecount(g);
  
  printf("\nGname: %s , n: %d, m: %d",gname,n,m);

  char depthstring[20] = "-\0";
  
  if(m==0) {
   for(int i=0;i<n;i++) {
     Path path;
     igraph_path_init(&path,1);
     VECTOR(path)[0] = i;
     igraph_paths_add(paths,&path); 
//     printf("\ndepth=%d,Paths=",depth);igraph_paths_print(paths);
   }
     return n;
  }

  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
  igraph_vector_init(&map2,n);
  igraph_vector_init(&X,0);
  
  
  igraph_integer_t dia;
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);

  /* Randomly assign values in loc, just for printing */
  int loc[dia+1];
  int s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  if(igraph_vector_size(Y)==0) {
  igraph_vector_init(Y,1);
  VECTOR(*Y)[0] = rand()%n;
  }

  char sname[200];
 // sprintf(sname,"%s_original",gname);
//  exportToDot(g,loc,dia+1,sname,"maxPaths",NULL,0);
  /*Run LBFS */
  LBFS(g,Y,&X,&label1,&map1);
 
  /* Align to Grid */
  igraph_vector_null(&map1);
  OrderGrid(g,loc,dia+1,&X,&map1);
//  sprintf(sname,"%s_lbfs1",gname);
//  exportToDot(g,loc,dia+1,sname,"lbfs1",&map1,0);

  /* Run LBFS again from other-side */
  igraph_vector_clear(Y);
  LBFS(g,&X,Y,&label2,&map2);

  /* Map to grid again */
  igraph_vector_null(&map2);
  OrderGrid(g,loc,dia+1,Y,&map2);
//  sprintf(sname,"%s_lbfs2",gname);
//  exportToDot(g,loc,dia+1,sname,"lbfs2",&map2,0);

  // Map the graph to gmap on which we will work
  igraph_vector_t invmap2;
  igraph_t gmap; 
  igraph_vector_init(&invmap2,n);
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }

   int npaths=1;
   int contr[dia+1], extra[dia+1], maxc[dia+1], minc[dia+1];
   int need=1;
   igraph_bool_t recalc = 0;
   Paths *lpaths[dia+1];
  /* Get the subgraphs for each level and recursively call the method if 
   * it has any edges */ 
   for(int i=0; i<dia+1; i++)
    {
      int j= (i==0)?0:loc[i-1];
	maxc[i] = loc[i]- j;  /* Maximum contribution or number of vertices in the elvel */

      igraph_t subg;
      igraph_vs_t vs;
      igraph_vs_seq(&vs,j,loc[i]-1);
      igraph_vector_t map,invmap;
      igraph_vector_init(&map,0);
      igraph_vector_init(&invmap,0);
//      printf("\n[%s]|%s L%d from %d to %d",gname,depthstring,i,j,loc[i]-1);
      igraph_induced_subgraph_map(&gmap,&subg,vs,IGRAPH_SUBGRAPH_AUTO,&map,&invmap);
      igraph_vs_destroy(&vs);
      char ggname[100];
      sprintf(ggname,"%s_subg%d",gname,i);
      igraph_vector_t subY;
      igraph_vector_init(&subY,0);
      lpaths[i] = (Paths*)malloc(sizeof(Paths*));
      igraph_paths_init(lpaths[i],0);
      int spaths = minc[i] = pathCover(&subg, ggname,depth+1,&subY, lpaths[i]);
      for(int k=0;k<igraph_paths_size(lpaths[i]);k++) {
	 Path *p = VECTOR(*lpaths[i])[k];
//	 printf("\nDepth=%d L%d: k=%d",depth,i,k);igraph_path_print(p);
	 for( int m=0;m<igraph_path_size(p);m++) 
		VECTOR(*p)[m]=	(int)VECTOR(invmap)[(int)VECTOR(*p)[m]];
//	 printf("\nDepth=%d L%d: k=%d Path=",depth,i,k);igraph_path_print(p);
      }	
       printf("\nDepth=%d L%d: Paths=",depth,i);igraph_paths_print(lpaths[i]);
 //     printf("\n[%s]|%s L%d paths=%d",gname,depthstring,i,subpaths);
      npaths= npaths + spaths - 1;

      igraph_vector_destroy(&subY);
      igraph_destroy(&subg);

      extra[i]=(need-minc[i]);
      contr[i]=(maxc[i]-need);
      if(extra[i]<0) recalc = 1; 
  //    printf("\n[%s]|%s Level [minc,maxc] needc extra contr",gname,depthstring);
  //    printf("\n[%s]|%s%5d| [%3d,%3d]  %5d %5d %5d",gname,depthstring,i,minc[i],maxc[i],1,extra[i],contr[i]);
  }

   for(int i=0;i<dia+1;i++)
    {	printf("\nL%d: ",i);igraph_paths_print(lpaths[i]); }
   recalc=1;
   int help[2][dia+1];
   for(int i=0;i<dia+1;i++)
    { help[0][i] = help[1][i] = 0; }

   if(recalc) 
    {
	npaths=1;
   	for(int l=0;l<dia+1;l++) {
		if(l!=0)  
		   while(extra[l]<0 && contr[l-1]>0)
			{ extra[l]++; contr[l-1]--; help[1][l-1]++; }
		if(l!=dia)
		   while(extra[l]<0 && contr[l+1]>0)
			{ extra[l]++; contr[l+1]--; help[0][l+1]++; }
//       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        npaths= npaths - extra[l];
	 }

	    
    /* Merge subpaths based on help */	
   	igraph_paths_init(paths,0);
   	for(int l=0;l<dia+1;l++) {
	    printf("\nL%d: ",l);igraph_paths_print(lpaths[l]);
	    printf("\nhelp[0][%d]=%d,help[1][%d]=%d",l,help[0][l],l,help[1][l]);
	    igraph_paths_break(lpaths[l],help[0][l]+help[1][l]);
//	    printf("L%d: ",l);igraph_paths_print((Paths*)&lpaths[l]);
	}
	igraph_paths_merge(paths,lpaths[0]);
   	for(int l=0;l<dia;l++) {
	    Paths edges;
	    igraph_paths_init(&edges,0);
	    for(int j=0; j<igraph_paths_size(lpaths[l]); j++) {
		Path *p = VECTOR(*lpaths[l])[j];
		int v = (int)VECTOR(*p)[igraph_vector_size(p)-1];
		igraph_vector_t snei;
		igraph_vector_init(&snei,0);
	   	getNeighbors(&gmap,v,loc,&snei,1);
		for(int i=0;i<igraph_vector_size(&snei);i++)
		{
		  Path *p = (Path*) malloc(sizeof(Path*)); 
		  igraph_path_init(p,0);
		  igraph_path_addv(p,j);
		  igraph_path_addv(p,(int)VECTOR(snei)[i]);
		  igraph_paths_add(&edges,p);
  		}
	      }
	    igraph_paths_merge(paths,&edges);
	    igraph_paths_simplify(paths);
	    igraph_paths_merge(paths,lpaths[l+1]);
	    igraph_paths_simplify(paths);
  	    igraph_paths_destroy(&edges);
 	}			
	    
  //    printf("\n[%s]|%s Level [minc,maxc] needc extra contr",gname,depthstring);
  //    for(int i=0;i<dia+1;i++)
  //    	printf("\n[%s]|%s%5d| [%3d,%3d]  %5d %5d %5d",gname,depthstring,i,minc[i],maxc[i],1,extra[i],contr[i]);
    }

  for(int i=0;i<dia+1;i++)
	if(lpaths[i]) igraph_paths_destroy(lpaths[i]);
  igraph_vector_destroy(&X);
//  igraph_vector_destroy(Y);
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
  igraph_vector_destroy(&map2);
  igraph_destroy(&gmap);

  /* Finally return the minimum number of paths possible */
     return npaths;
}

/* Experimental approach */
int minPaths(igraph_t* g, char *gname, int depth, igraph_vector_t *Y) {

  igraph_vector_t map1, map2, X, label1, label2;
  int n = igraph_vcount(g);
  int m = igraph_ecount(g);

  char depthstring[20] = "-\0";
//  for(int i=0;i<depth;i++) sprintf(depthstring,"%s--",depthstring);

//  printf("\n[%s]|%s Number of vertices=%d",gname,depthstring,n);
//  printf(", Edges=%d",m);
  igraph_integer_t dia;
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
//  printf(", Diameter=%d",dia);

  /* No point in moving ahead if there are no vertices or no edges */
  if(n==0 || n==1 || m==0) { 
//  printf("\n[%s]|%s L%d paths=%d",gname,depthstring,0,n);
  return n;
  }

  igraph_vector_init(&map1,n);
  igraph_vector_init(&map2,n);
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&X,0);
  if(igraph_vector_size(Y)==0) {
  	igraph_vector_init(Y,1);
  	VECTOR(*Y)[0] = rand()%n;
    }

  /* Randomly assign values in loc, just for printing */
  int loc[dia+1];
  int s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  char sname[200];
 // sprintf(sname,"%s_original",gname);
//  exportToDot(g,loc,dia+1,sname,"maxPaths",NULL,0);
  /*Run LBFS */
  LBFS(g,Y,&X,&label1,&map1);
 
  /* Align to Grid */
  igraph_vector_null(&map1);
  OrderGrid(g,loc,dia+1,&X,&map1);
//  sprintf(sname,"%s_lbfs1",gname);
//  exportToDot(g,loc,dia+1,sname,"lbfs1",&map1,0);

  /* Run LBFS again from other-side */
  igraph_vector_clear(Y);
  LBFS(g,&X,Y,&label2,&map2);

  /* Map to grid again */
  igraph_vector_null(&map2);
  OrderGrid(g,loc,dia+1,Y,&map2);
//  sprintf(sname,"%s_lbfs2",gname);
//  exportToDot(g,loc,dia+1,sname,"lbfs2",&map2,0);

  // Map the graph to gmap on which we will work
  igraph_vector_t invmap2;
  igraph_t gmap; 
  igraph_vector_init(&invmap2,n);
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }

   int paths=1;
   int contr[dia+1], extra[dia+1], maxc[dia+1], minc[dia+1];
   igraph_bool_t recalc = 0;
  /* Get the subgraphs for each level and recursively call the method if 
   * it has any edges */ 
   for(int i=0; i<dia+1; i++)
    {
      int j= (i==0)?0:loc[i-1];
	maxc[i] = loc[i]- j;  /* Maximum contribution or number of vertices in the elvel */

//      igraph_vector_t tlevel;
//      igraph_vector_init(&tlevel,0);
//      for(int k=j;k<loc[i];k++)
//      	igraph_vector_insert(&tlevel,0,k);
      igraph_t subg;
      igraph_vs_t vs;
      igraph_vs_seq(&vs,j,loc[i]-1);
//      printf("\n[%s]|%s L%d from %d to %d",gname,depthstring,i,j,loc[i]-1);
      igraph_induced_subgraph(&gmap,&subg,vs,IGRAPH_SUBGRAPH_AUTO);
      igraph_vs_destroy(&vs);
      char ggname[100];
      sprintf(ggname,"%s_subg%d",gname,i);
      igraph_vector_t subY;
      igraph_vector_init(&subY,0);
      int subpaths = minc[i] = minPaths(&subg, ggname,depth+1,&subY);
 //     printf("\n[%s]|%s L%d paths=%d",gname,depthstring,i,subpaths);
      paths= paths + subpaths - 1;
      igraph_vector_destroy(&subY);
      igraph_destroy(&subg);

      extra[i]=(1-minc[i]);
      contr[i]=(maxc[i]-1);
      if(extra[i]<0) recalc = 1; 
  //    printf("\n[%s]|%s Level [minc,maxc] needc extra contr",gname,depthstring);
  //    printf("\n[%s]|%s%5d| [%3d,%3d]  %5d %5d %5d",gname,depthstring,i,minc[i],maxc[i],1,extra[i],contr[i]);
    }

   if(recalc) 
    {
	paths=1;
   	for(int l=0;l<dia+1;l++) {
		if(l!=0)  
		   while(extra[l]<0 && contr[l-1]>0)
			{ extra[l]++; contr[l-1]--; }
		if(l!=dia)
		   while(extra[l]<0 && contr[l+1]>0)
			{ extra[l]++; contr[l+1]--; }
//       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        paths= paths - extra[l];
	 }
	
  //    printf("\n[%s]|%s Level [minc,maxc] needc extra contr",gname,depthstring);
  //    for(int i=0;i<dia+1;i++)
  //    	printf("\n[%s]|%s%5d| [%3d,%3d]  %5d %5d %5d",gname,depthstring,i,minc[i],maxc[i],1,extra[i],contr[i]);
    }

  igraph_vector_destroy(&X);
//  igraph_vector_destroy(Y);
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
  igraph_vector_destroy(&map2);
  igraph_destroy(&gmap);

  /* Finally return the minimum number of paths possible */
     return paths;
}


/* This method checks the local AT-Free condition for vertices in same level. In case where
 * (x,y,z) belong to the same level and the AT doesnot involve a path using any of the other levels.
 * We call such case as 1L-AT
 * Returns true if the condition worked
 * to satisfy the local check . It takes j and it's current level non-neighbors as
 * input  - This desnot work and is no longer needed superceded by isAdjSatisfied
 * Note: g1 functionality is not yet implemented */
igraph_bool_t isLevelSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1) {

   igraph_bool_t connected, atfree, isatfree=1;

  /* For each current non-neighbors of j */ 
  for (int i=0; i<igraph_vector_size(cnn)-1; i++) {
    	int w = (int) VECTOR(*cnn)[i];
   	igraph_vector_t ibni,icni,inni;
   	igraph_vector_init(&ibni, 1);
   	igraph_vector_init(&icni, 1);
   	igraph_vector_init(&inni, 1);
	/* Get current neighbors of w and see if it is connected to j */
	cutNeighbors(g,w,loc,&ibni,&icni,&inni);
	for (int k=0; k<igraph_vector_size(&icni)-1; k++) {
	  int v = (int) VECTOR(icni)[k];	
	  igraph_are_connected(g,v,j,&connected);
	  atfree=1;
	  /* If not connected to j, find its current neighbors and 
 	   * see if it is connected to j */
	  if(!connected)
	   {
   		igraph_vector_t kbni,kcni,knni;
   		igraph_vector_init(&kbni, 1);
   		igraph_vector_init(&kcni, 1);
   		igraph_vector_init(&knni, 1);
		cutNeighbors(g,v,loc,&kbni,&kcni,&knni);
		/*Randomly decide if we need to add edges (u,j) 
                 *for all u with current neighbors  */
		igraph_bool_t shallAdd = rand()%2;
		for (int l=0; l<igraph_vector_size(&kcni)-1; l++) {
		  int u = (int) VECTOR(kcni)[l];
	          igraph_are_connected(g,u,j,&connected);
		  if(!connected) {
//		     if(atfree) printf("\nC:(%d,%d) (",w,j);
		     atfree=0;
		     //printf("Connect %d---%d and ",u,j);
		     /* Did I connect it already(just now?) */
		     igraph_are_connected(g1,u,j,&connected);
//		     if(!connected) printf("Connect %d---%d and ",u,j);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,j);
		  }
	        }
//		if(shallAdd) printf("*");
                /* If not atfree, and edges are not added to current neighbors earlier,
                 * add edge (v,j) in g1 */ 
		if(!atfree) {
		  /* Did I connect it already(just now?) */
		 // printf(") OR (Connect %d---%d)",v,j);
		  igraph_are_connected(g1,v,j,&connected);
//		  if(!connected) printf(") OR (Connect %d---%d)",v,j);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,j);
		  if(!connected && !shallAdd) printf("*");
		}
		isatfree=isatfree & atfree;
		igraph_vector_destroy(&kbni);
		igraph_vector_destroy(&kcni);
		igraph_vector_destroy(&knni);
	   }
	}
	igraph_vector_destroy(&ibni);
	igraph_vector_destroy(&icni);
	igraph_vector_destroy(&inni);

	/* No need to do the exact same thing with j 
         * as it will be covered when other vertices are explored*/
     }
  return isatfree;		
}

/* This method checks a local AT-Free condition for certain vertices in same levels . 
 * It detects an AT where atleast (x,y) belong to the same level, and z may belong to any level, but th * e AT is i* foemed using the adjacent levels. We call this as 2L-AT (AT-formed within two adjacent 
 * levels)
 * Returns true if the localcheck worked, else adds edges in g1 (which is essentially a copy of g)
 * to satisfy the local checkadds edges in g1 (which is essentially a copy of g)
 * to satisfy the local check . It takes j and it's current level non-neighbors as
 * input 
 * Note: Right now, it doesnot add edges and return an AT-Free graph in g1*/
igraph_bool_t isAdjSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1) {

   igraph_bool_t connected, atfree, isatfree=1;

   /* Get all neighbours */
   igraph_vector_t jni;
   igraph_vector_init(&jni, 1);
   igraph_neighbors(g,&jni,j,IGRAPH_ALL);
 //  printf("\nj=%d, jni=",j);igraph_vector_print(&jni);

  /* For each current non-neighbors of j */ 
  for (int i=0; i<igraph_vector_size(cnn)-1; i++) {
    	int w = (int) VECTOR(*cnn)[i];

	if(igraph_vector_contains(&jni,w)) continue;	
//	if(w<j) continue;
        /* Get all neighbours of w */
        igraph_vector_t ini;
        igraph_vector_init(&ini, 1);
        igraph_neighbors(g,&ini,w,IGRAPH_ALL);

	/* All three conditions should hold good */
	/* 1. One common neighbor should exist */
	/* 2. One non-neighbor of j should exist N(w) */
	/* 3. One non-neighbor of w should exist in N(j) */
	igraph_vector_t inc,jnc,ijcc;
	igraph_vector_init(&jnc,0);
	igraph_vector_init(&inc,0);
	igraph_vector_init(&ijcc,0);

	igraph_vector_sort(&ini);
	igraph_vector_sort(&jni);
	igraph_vector_intersect_sorted(&ini,&jni,&ijcc);
	igraph_vector_difference_sorted(&ini,&jni,&jnc);
	igraph_vector_difference_sorted(&jni,&ini,&inc);
	
//	printf("\nSize: i=%d,j=%d,common=%d,inc=%d,jnc=%d",w,j,igraph_vector_size(&ijcc),
//		igraph_vector_size(&inc),igraph_vector_size(&jnc));
//	printf("ini=");igraph_vector_print(&ini);
//	printf("jni=");igraph_vector_print(&jni);
//	printf("\n\tijcc=");igraph_vector_print(&ijcc);
//	printf(", inc=");igraph_vector_print(&inc);
//	printf(", jnc=");igraph_vector_print(&jnc);
	if(!igraph_vector_size(&inc) || !igraph_vector_size(&jnc)
	   || !igraph_vector_size(&ijcc))  continue;

	atfree=1;
	igraph_vs_t vsi, vsj;
	igraph_vs_vector(&vsi, &inc);
	igraph_vs_vector(&vsj, &jnc);
	igraph_vector_t ic, jc, ijc, injc;
	igraph_vector_ptr_t ires, jres;
	igraph_vector_init(&ic,0);
	igraph_vector_init(&injc,0);
	igraph_vector_init(&jc,0);
	igraph_vector_ptr_init(&ires,0);
	igraph_vector_ptr_init(&jres,0);
	igraph_vector_init(&ijc,0);

	/* A = Calculate N[N(i) not in N(j)] */
	igraph_neighborhood(g,&ires,vsi,1,IGRAPH_ALL,0);
	for(int k=0;k<igraph_vector_ptr_size(&ires);k++) {
		igraph_vector_t *v = VECTOR(ires)[k];
//		printf("\t\t(i)v=");igraph_vector_print(v);
		for(int t=0;t<igraph_vector_size(v);t++) {
		  int u = (int)VECTOR(*v)[t];
		  if(!igraph_vector_contains(&ic,u) && !igraph_vector_contains(&inc,u))
		     igraph_vector_insert(&ic,0,u);
		  }
	}
	igraph_vector_sort(&ic);
	igraph_vector_difference_sorted(&ic,&ijcc,&injc);

	/* Calculate B = Calculate N[N(j) not in N(i)] */
	igraph_neighborhood(g,&jres,vsj,1,IGRAPH_ALL,0);
	for(int k=0;k<igraph_vector_ptr_size(&jres);k++) {
		igraph_vector_t *v = VECTOR(jres)[k];
//		printf("\t\t(j)v=");igraph_vector_print(v);
		for(int t=0;t<igraph_vector_size(v);t++) {
		  int u = (int)VECTOR(*v)[t];
		  if(!igraph_vector_contains(&jc,u) && !igraph_vector_contains(&jnc,u))
		     igraph_vector_insert(&jc,0,u);
		  }
	}
	igraph_vector_sort(&jc);
//	printf("\n\tic=");igraph_vector_print(&ic);
//	printf(", injc=");igraph_vector_print(&injc);
//	printf(", jc=");igraph_vector_print(&jc);

	/* Calculate C = A intersect B , if none, then set AT-Free condition true*/	
	igraph_vector_intersect_sorted(&injc,&jc,&ijc);
//	printf(", ijc=");igraph_vector_print(&ijc);
	if(igraph_vector_size(&ijc)==0) continue;

	for(int t=0;t<igraph_vector_size(&ijc);t++) {
		int u = (int)VECTOR(ijc)[t];
        	igraph_vector_t uni, ijnuc;
        	igraph_vector_init(&uni, 0);
        	igraph_vector_init(&ijnuc, 0);
        	igraph_neighbors(g,&uni,u,IGRAPH_ALL);
	 	igraph_vector_difference_sorted(&ijcc,&uni,&ijnuc);	
		if(igraph_vector_size(&ijnuc)!=0) {
		   atfree=0;
//		if(u>j && u>w)   // Avoid duplicate messages
		   printf("\n(%d,%d,%d) form an AT",j,w,u);
	     }
	}

	isatfree = isatfree & atfree;
	igraph_vector_destroy(&inc);
	igraph_vector_destroy(&jnc);
	igraph_vector_destroy(&ijcc);
	igraph_vector_destroy(&injc);
	igraph_vector_destroy(&ic);
	igraph_vector_destroy(&jc);
	igraph_vector_destroy(&ijc);
	igraph_vector_ptr_destroy(&ires);
	igraph_vector_ptr_destroy(&jres);
	igraph_vs_destroy(&vsi);
	igraph_vs_destroy(&vsj);
//	printf("\nj=%d:atfree=%d, isATFree=%d",j,atfree, isatfree);
   }
  return isatfree;		
}

/* This method takes a graph G (which is not yet ordered or arranged in grid) and orders it in a grid,
 * calls the isATFree function and returns back pokable sets in X and Y (if possible and unique), the 
 * loc array, the mapped vector that can be used to ordered form from original graph.
 * It also returns back the AT-Freed graph constructed by isATFree (note this is not fully implemented) */
igraph_bool_t processForAT(igraph_t *g, char *gname, int depth, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *gmap1)
{
  int n = igraph_vcount(g);
  int m = igraph_ecount(g);

  igraph_empty(gmap1, n, IGRAPH_UNDIRECTED);
  igraph_vector_init(map2,n);
  igraph_vector_init(X,0);

  /* No point in moving ahead if there are no vertices or no edges */
  if(n==0) return 1;

  if(m==0) {
    for(int i=0;i<n;i++)
 	igraph_vector_insert(X,0,i);
    return 1;
   }

  if(igraph_vector_size(Y)==0) {
//  igraph_vector_t Y;
  igraph_vector_init(Y,1);
  VECTOR(*Y)[0] = rand()%n;
  }

  char depthstring[20] = "\0";
  for(int i=0;i<depth;i++) sprintf(depthstring,"%s--",depthstring);
  printf("\n|%s[%s]Number of vertices=%d",depthstring,gname,n);
  printf(", Edges=%d",m);
  igraph_integer_t result,dia;
  igraph_vertex_connectivity(g,&result,1);
  printf(" Vertex connectivity=%d",result);
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf(", Diameter=%d",dia);

  /* Randomly assign values in loc, just for printing */
//  int loc[dia+1], 
  int s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  /* Calculate dominating pair set */
//  igraph_vector_t X,Y,label1,label2,map1,map2;
  igraph_vector_t label1,label2,map1;//,map2;
  igraph_vector_t invmap2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
//  igraph_vector_init(map2,n);
  igraph_vector_init(&invmap2,n);
//  igraph_vector_init(X,0);
//  igraph_vector_init(&Y,1);
//  VECTOR(Y)[0] = rand()%n;
  
  char sspos[680];
  char sname[200];

  sprintf(sname,"%s",gname);  
  sprintf(sspos,"Graph=%s",sname);
  exportToDot(g,loc,dia+1,sname,sspos,NULL,0);
  LBFS(g,Y,X,&label1,&map1);
//  printf("\n Y="); igraph_vector_print(Y);
//  printf(" X="); igraph_vector_print(X);

  char x[100], y[100];
  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(&map1);
  OrderGrid(g,loc,dia+1,X,&map1);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  sprintf(sname,"lbfs1-%s",gname);  
  sprintf(sspos,"Graph=%s,Y=[%s] , X=[%s]  ",sname,y,x);
//  exportToDot(g,loc,dia+1,sname,sspos,&map1,0);

  igraph_vector_clear(Y);
  LBFS(g,X,Y,&label2,map2);
  printf("\n|%sX=",depthstring); igraph_vector_print(X);
  printf("|%sY=",depthstring); igraph_vector_print(Y);

  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(map2);
  OrderGrid(g,loc,dia+1,Y,map2);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  //printf("\nmap1="); igraph_vector_print(&map1);
  //printf("\nmap2="); igraph_vector_print(&map2);

  /* We check AT-Free congition before printing */
  // Map the graph to gmap 
  igraph_t gmap; 
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(*map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }
//   printf("\nmap2="); igraph_vector_print(&map2);
//   printf("\ninvmap2="); igraph_vector_print(&invmap2);

 /* Copy the graph to gmap1, check ATFree condition
   *  and make it AT-Free if it is not already */
//  igraph_t gmap1;
  igraph_copy(gmap1,&gmap);
  igraph_bool_t isatfree;
  isATFree(&gmap,gname,depth,loc,dia+1,&isatfree,gmap1);

  /* Send the original mapped one */
  igraph_copy(gmap1,&gmap);

  /* Print the graph g with ATFree result */
  sprintf(sname,"lbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(g,loc,dia+1,sname,sspos,map2,0);

  /* Export mapped versions */
  sprintf(sname,"maplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s (mapped),X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,NULL,0);

/*
  sprintf(sname,"remaplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,map2,1); */

//  if(isatfree) printf("\nG is AT-Free");
//  else printf("\nG is not AT-Free");
 
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
//  igraph_destroy(&gmap1);
  igraph_destroy(&gmap);
  return isatfree;
}

/* This method takes a graph G (which is not yet ordered or arranged in grid) and orders it in a grid,
 * calls the isDP function and returns back pokable sets in X and Y (if possible and unique), the 
 * loc array, the mapped vector that can be used to ordered form from original graph.
 * It also returns back DP graph constructed by isDP in gmap1 */ 
igraph_bool_t processForDP(igraph_t *g, char *gname, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *gmap1)
{
  int n = igraph_vcount(g);
  int m = igraph_ecount(g);

  igraph_empty(gmap1, n, IGRAPH_UNDIRECTED);
  igraph_vector_init(map2,n);
  igraph_vector_init(X,0);

  /* No point in moving ahead if there are no vertices or no edges */
  if(n==0) return 1;

  if(m==0) {
    for(int i=0;i<n;i++)
 	igraph_vector_insert(X,0,i);
    return 1;
   }

  if(igraph_vector_size(Y)==0) {
//  igraph_vector_t Y;
  igraph_vector_init(Y,1);
  VECTOR(*Y)[0] = rand()%n;
  }

  printf("\n|[%s]Number of vertices=%d",gname,n);
  printf(", Edges=%d",m);
  igraph_integer_t result,dia;
  igraph_vertex_connectivity(g,&result,1);
  printf(" Vertex connectivity=%d",result);
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf(", Diameter=%d",dia);

  /* Randomly assign values in loc, just for printing */
//  int loc[dia+1], 
  int s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  /* Calculate dominating pair set */
//  igraph_vector_t X,Y,label1,label2,map1,map2;
  igraph_vector_t label1,label2,map1;//,map2;
  igraph_vector_t invmap2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
//  igraph_vector_init(map2,n);
  igraph_vector_init(&invmap2,n);
//  igraph_vector_init(X,0);
//  igraph_vector_init(&Y,1);
//  VECTOR(Y)[0] = rand()%n;
  
  char sspos[680];
  char sname[200];

  sprintf(sname,"%s",gname);  
  sprintf(sspos,"Graph=%s",sname);
  exportToDot(g,loc,dia+1,sname,sspos,NULL,0);
  LBFS(g,Y,X,&label1,&map1);
//  printf("\n|Y="); igraph_vector_print(Y);
//  printf("|X="); igraph_vector_print(X);

  char x[100], y[100];
  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(&map1);
  OrderGrid(g,loc,dia+1,X,&map1);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  sprintf(sname,"lbfs1-%s",gname);  
  sprintf(sspos,"Graph=%s,Y=[%s] , X=[%s]  ",sname,y,x);
//  exportToDot(g,loc,dia+1,sname,sspos,&map1,0);

  igraph_vector_clear(Y);
  LBFS(g,X,Y,&label2,map2);
  printf("\n|X="); igraph_vector_print(X);
  printf("|Y="); igraph_vector_print(Y);

  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(map2);
  OrderGrid(g,loc,dia+1,Y,map2);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  //printf("\nmap1="); igraph_vector_print(&map1);
  //printf("\nmap2="); igraph_vector_print(&map2);

  /* We check AT-Free congition before printing */
  // Map the graph to gmap 
  igraph_t gmap; 
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(*map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }
//   printf("\nmap2="); igraph_vector_print(&map2);
//   printf("\ninvmap2="); igraph_vector_print(&invmap2);

 /* Copy the graph to gmap1, check DP condition
   *  and make it DP if it is not already */
//  igraph_t gmap1;
  igraph_copy(gmap1,&gmap);
  igraph_bool_t isdp;
  isDP(&gmap,gname,loc,dia+1,&isdp,gmap1);

  /* Send the original mapped one */
//  igraph_copy(gmap1,&gmap);

  /* Print the graph g with DP result */
  sprintf(sname,"lbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],DP=%d ",sname,x,y,isdp);
  exportToDot(g,loc,dia+1,sname,sspos,map2,0);

  /* Export mapped versions */
  sprintf(sname,"maplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s (mapped),X=[],Y=[],DP=%d ",sname,isdp);
  exportToDot(&gmap,loc,dia+1,sname,sspos,NULL,0);

/*
  sprintf(sname,"remaplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,map2,1); */

//  if(isatfree) printf("\nG is AT-Free");
//  else printf("\nG is not AT-Free");
 
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
//  igraph_destroy(&gmap1);
  igraph_destroy(&gmap);
  return isdp;
}

/* Size or l = number of levels 
 * Orders the graph using BFS when X (vertices in level 0) are given and creates a map
 * and returns it*/
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map) 
{
	int mapIndex = 0;
	igraph_vector_t vlevel;
	igraph_vector_copy(&vlevel,X);

	int n = igraph_vcount(g);
 	igraph_bool_t visited[n];	
	for(int i=0;i<n;i++) visited[i]=0;

//	printf("\n");
	for(int i=0;i<l;i++)
	 {
	   igraph_vs_t vs;
	   igraph_vs_vector(&vs, &vlevel);

	   igraph_vector_ptr_t res;
	   igraph_vector_ptr_init(&res,0);
//	   printf("i=%d, Vlevel=",i);igraph_vector_print(&vlevel);
	   for(int j=0;j<igraph_vector_size(&vlevel);j++)
	      {
		int v = VECTOR(vlevel)[j];
		VECTOR(*map)[mapIndex++] = v;
		visited[v] = 1;
	      }
	   igraph_integer_t vssize;
	   igraph_vs_size(g,&vs,&vssize);
	   loc[i] = vssize + ((i==0)?0:loc[i-1]);
	   igraph_neighborhood(g,&res,vs,1,IGRAPH_ALL,0);

	   /* Reset vlevel to the vertices in current level 
 	    */
	   igraph_vector_clear(&vlevel);
	   for(int j=0;j<igraph_vector_ptr_size(&res);j++) {
		igraph_vector_t *v = VECTOR(res)[j];
		for(int k=0;k<igraph_vector_size(v);k++) {
		  if(!igraph_vector_contains(&vlevel,VECTOR(*v)[k]) && !visited[((int)VECTOR(*v)[k])])
			igraph_vector_insert(&vlevel,0,VECTOR(*v)[k]);
		  }
//		printf("\ni=%d,j=%d: ",i,j);
//		igraph_vector_print(v);
		igraph_vector_destroy(v);
		igraph_free(v);
	   }

//	   printf(" N(Vlevel)=");igraph_vector_print(&vlevel);
	   igraph_vs_destroy(&vs);
	   igraph_vector_ptr_destroy(&res);
	 }
	igraph_vector_destroy(&vlevel);
}


/* Runs Lexicographic BFS from vertex or vertices in vstart (X), returns back the label of each vertex , mapping between original and labels and the pokable set X */
void LBFS(igraph_t *g, igraph_vector_t *vstart, igraph_vector_t *X, igraph_vector_t *label, 
	igraph_vector_t *map) {

   /* Starting label */ 
   int n = igraph_vcount(g);
   igraph_strvector_t llabel;
   igraph_strvector_init(&llabel, n);
   for(int i=0;i<igraph_strvector_size(&llabel);i++) {
      char *tmpstr="000";
      igraph_strvector_set(&llabel,i,tmpstr);
   }

   int m=0; 
   for(int i=0;i<igraph_vector_size(vstart);i++) {
   	char strlabel[100];
	if(n+1<10)	  sprintf(strlabel,"00%d",n+1);
	else if(n+1<100)  sprintf(strlabel,"0%d",n+1);
	else if(n+1<1000) sprintf(strlabel,"%d",n+1);
   	int v = VECTOR(*vstart)[i];
   	/* Set lexlabel of v as some positive number */
   	igraph_strvector_set(&llabel,v,strlabel);
//	printf("v=%d",v);
     } 

 /*    printf("\n LLabels are: ");
     for(int i=0;i<igraph_strvector_size(&llabel);i++) { 
	char *tmplabel;
 	igraph_strvector_get(&llabel,i,&tmplabel);
	printf("(%d):%s",i,tmplabel); 
     } */
   while(n>0) {

	/* Find the maximum and its index */
	int maxes = 1, maxValue=0, maxIndex=0;
	char maxlabel[100];
	maxlabel[0] = '\0';
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) maxes++;
	    if(strcmp(maxlabel,tmplabel)<0)
	      { maxes=1; strcpy(maxlabel,tmplabel); maxIndex=i; }
	}
     
	/* Assign label of the maxes and set their llabels to -1 */ 
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
//	    if(maxValue==(int)VECTOR(llabel)[i]) {
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) {
 		VECTOR(*label)[i] = n-maxes+1;
		VECTOR(*map)[m++] = i;
   		igraph_strvector_set(&llabel,i,"\0");
   //             printf("\nn=%d,maxes=%d, v=%d, label(v)=%d, maxIndex=%d, maxlabel=%s",n-maxes+1,maxes,i,(int)VECTOR(*label)[i],i,maxlabel);
	    }
	}
	n=n-maxes; 

	/* Propagate labels of all maxes*/
	for(int i=0;i<igraph_vector_size(label);i++) {
	    if((int)VECTOR(*label)[i]==n+1) {
        	/* Get all neighbours */
   		igraph_vector_t ni;
   		igraph_vector_init(&ni, 1);
   		igraph_neighbors(g,&ni,i,IGRAPH_ALL);

		/* Propagate lexlabels */
        	for(int j=0;j<igraph_vector_size(&ni);j++) {
          	   int u = (int) VECTOR(ni)[j];
//	  	   /* Label only if not labelled */
	    	   char *tmplabel, newlabel[100];
 	    	   igraph_strvector_get(&llabel,u,&tmplabel);
		   if(strcmp(tmplabel,"\0")>0) {
			int v = (int)VECTOR(*label)[i];
			if(v<10)	sprintf(newlabel,"%s,00%d",tmplabel,v);
			else if(v<100)  sprintf(newlabel,"%s,0%d",tmplabel,v);
			else if(v<1000) sprintf(newlabel,"%s,%d",tmplabel,v);
 	    	      igraph_strvector_set(&llabel,u,newlabel);
		   }
       		}
       		igraph_vector_destroy(&ni);
	     }
	}
/*     printf("\n  Labels are: ");
     for(int i=0;i<igraph_vector_size(label);i++)  
	printf("(%d):%d",i,(int)VECTOR(*label)[i]);
     printf("\n  Map is: ");
     for(int i=0;i<igraph_vector_size(map);i++)  
	printf("(%d):%d",i,(int)VECTOR(*map)[i]);
     printf("\n  LLabels are: ");
     for(int i=0;i<igraph_strvector_size(&llabel);i++) { 
	char *tmplabel;
 	igraph_strvector_get(&llabel,i,&tmplabel);
	printf("(%d):%s",i,tmplabel); 
     } */
//       v = maxIndex; 
   }
   
     for(int i=0;i<igraph_vector_size(label);i++) { 
//	printf("\nLabel of (%d): %d, Map : %d",i,(int)VECTOR(*label)[i],(int)VECTOR(*map)[i]);
	if((int)VECTOR(*label)[i]==1)
	  igraph_vector_insert(X,0,i);
	} 
       igraph_strvector_destroy(&llabel);
}

/* Checks if the graph is AT-Free. Assumes that the graph is arranged in grid. Returns back 
 * an AT-Free graph in g1 (if possible).
 * NOTE: The process doesnot stop after finding an AT, it continues to process all the vertices
 * The complete ATFreeing process still needs to be implemented.
 * Three conditions are checked:
 * (1) Non-adjacent vertices between consecutive levels - for AT along dominating path
 * (2) 2L-AT where 2 vertices from same level are involved and missed by condition (1)
 * (3) 3L-AT where 3 vertices from same level are involved and missed by condition (1) and (2)
 *   (sometimes 2 and 3 may overlap, so the three conditions are not mutually exclusive)
 */
void isATFree(igraph_t *g, char *gname, int depth, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1) {

  igraph_vector_t left,current, right,fnn, cnn;
  *isatfree=1;
  
  /* Every graph with 5 vertices or less is AT-Free */
  if(igraph_vcount(g)<=5) return;
 
  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check 
   *
   * Also get the neighbors in the current levels and 
   * apply the level check */
  for(int i=0; i<l;i++) {
    int j= (i==0)?0:loc[i-1];
    igraph_vector_t tlevel;
    igraph_vector_init(&tlevel,0);
    for(; j<loc[i];j++)
    {
      igraph_vector_insert(&tlevel,0,j);
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      igraph_vector_init(&cnn,1);
      cutNeighbors(g,j,loc,&left,&current,&right);
      forwardNonNeighbors(g,j,loc,l,&fnn);
      currentNonNeighbors(g,j,loc,l,&cnn);

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy of g 
     *  This is for AT (x,y,z) in three different levels */   
      *isatfree = *isatfree & isDominSatisfied(g,j,loc,&fnn,g1);
 
     /* 2L-AT */ 
      *isatfree = *isatfree & isAdjSatisfied(g,j,loc,&cnn,g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
  igraph_vector_destroy(&cnn);
 }
 
    /* 1L-AT For every level, extract the subgraph of the level and run the 
     * complete process again 
     * This is for AT (x,y,z) with all in the same level without using
     * any vertices or paths from any other levels */
      igraph_t subg, gmap1;
      igraph_vs_t vs;
      igraph_vector_t map2,X,Y;
      int subdia;
      char ggname[100];
      igraph_vector_init(&X,0);
      igraph_vector_init(&map2,0);
      igraph_vector_init(&Y,0);
      igraph_empty(&gmap1, 0, IGRAPH_UNDIRECTED);
      sprintf(ggname,"%s_subg%d",gname,i);
      igraph_vs_vector(&vs, &tlevel);
      igraph_vector_init(&map2,igraph_vector_size(&tlevel));
      igraph_induced_subgraph(g,&subg,vs,IGRAPH_SUBGRAPH_AUTO);
      igraph_diameter(&subg,&subdia,0,0,0,IGRAPH_UNDIRECTED,1);
//      printf("\n[%s]L%d: Subgraph Diameter=%d, Vertices=%d, Edges=%d",ggname,i,subdia,igraph_vcount(&subg),igraph_ecount(&subg));
      int loc[subdia+1];
      igraph_bool_t atfree = processForAT(&subg,ggname,depth+1,loc,&X,&Y,&map2,&gmap1);
      char depthstring[20]="\0";
      for(int k=0;k<depth;k++) sprintf(depthstring,"%s--",depthstring); 
      if(!atfree) printf("\n|%s[%s]L%d has an AT.",depthstring,ggname,i);
      *isatfree = *isatfree & atfree;
      igraph_vector_destroy(&tlevel);
      igraph_vs_destroy(&vs);
      igraph_vector_destroy(&X);
      igraph_vector_destroy(&Y);
      igraph_vector_destroy(&map2);
      igraph_destroy(&gmap1);
      igraph_destroy(&subg);
   }
}

/* Checks if the g is a Dominating pair graph. Assumes that the graph is arranged in grid. Returns back 
 * an DP graph in g1.
 * NOTE: The process doesnot stop after finding an AT, it continues to process all the vertices
 * Following condition is checked:
 * (1) Non-adjacent vertices between consecutive levels - for AT along dominating path
 */
void isDP(igraph_t *g, char *gname, int *loc, int l, igraph_bool_t *isdp, igraph_t *g1) {

  igraph_vector_t left,current, right,fnn, cnn;
  *isdp=1;
 
  igraph_t g2;
  igraph_copy(&g2,g1); 
  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check 
   *
   * Also get the neighbors in the current levels and 
   * apply the level check */
  for(int i=0; i<l;i++) {
    int j= (i==0)?0:loc[i-1];
    for(; j<loc[i];j++)
    {
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      igraph_vector_init(&cnn,1);
      cutNeighbors(g,j,loc,&left,&current,&right);
      forwardNonNeighbors(g,j,loc,l,&fnn);
      currentNonNeighbors(g,j,loc,l,&cnn);

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy of g 
     *  This is for AT (x,y,z) in three different levels */   
      igraph_bool_t islocaldp = isDominSatisfied(&g2,j,loc,&fnn,g1);
      *isdp = *isdp & islocaldp;
      if(!islocaldp) igraph_copy(&g2,g1); 

      igraph_vector_destroy(&left);
      igraph_vector_destroy(&current);
      igraph_vector_destroy(&right);
      igraph_vector_destroy(&fnn);
      igraph_vector_destroy(&cnn);
  }
 }
  igraph_destroy(&g2);
}

/* Note: l= number of levels */
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text, igraph_vector_t *map, igraph_bool_t inverse ) {

  double scale=1.0;
  /* Find max size of a level */
  int s=loc[0];
  for(int i=1;i<l;i++)
    if(s<loc[i]-loc[i-1]) s = loc[i]-loc[i-1];

  /* Set the positions in .dot file */
  double off=0.1;
  int v;
  for(int i=0;i<l;i++) {
    int o = (i==0)?0:loc[i-1];
    for(int j=o;j<loc[i];j++) {
  	if(!inverse && map !=NULL)  v = (int)VECTOR(*map)[j];
	else 		v = j;
  	char spos[80];
  	int low = (s-(loc[i]-o))/2;
//	printf("\n\tj=%d, low=%d, loc[i]=%d, o=%d, scale=%f",j,low,loc[i],o,scale);
 	// SETVAS(&g, "fixedsize",j,"true");
  	sprintf(spos,"%f,%f!",i*scale-((j-o)%2)*off,(j-o+low)*scale);
 	SETVAS(g, "pos",v,spos);
  	SETVAS(g, "shape",v,"point");
  	SETVAN(g, "fontsize",v,6);
	if(inverse && map !=NULL) SETVAN(g, "xlabel",v,(int)VECTOR(*map)[v]);
  	else 	    SETVAN(g, "xlabel",v,v);
     }
}

  /* Write g to the file specified in stdin */ 
  char fName[100];
  sprintf(fName,"%s.dot",filename);
  FILE *fp = fopen(fName,"w");
  SETGAS(g, "label", text);
  SETGAS(g, "labelloc", "bottom");
  igraph_write_graph_dot(g, fp);
  fclose(fp);
}
