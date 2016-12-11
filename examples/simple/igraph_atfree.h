/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#define MLEVELS 7
#define LSIZE 7 
void cutNeighbors(igraph_t* g,int j,int *loc,igraph_vector_t *bni, igraph_vector_t *cni, igraph_vector_t *nni);
igraph_bool_t isDominSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *fnn, igraph_t *g1);
igraph_bool_t isLevelSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1);
void forwardNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *fnn);
void currentNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *cnn);
void getNeighbors(igraph_t* g,int j, int *loc,igraph_vector_t *ne, int dir);
igraph_bool_t isHamiltonian(igraph_t* g, int *loc, int l);
void isHamUsingLAD(igraph_t *g, igraph_bool_t *iso, char *path);
void isHamUsingVF2(igraph_t *g, igraph_bool_t *iso,char *path);

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
 * to satisfy the local checkadds edges in g1 (which is essentially a copy of g)
 * to satisfy the local check . It takes j and it's forward non-neighbors as
 * input */
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
		     if(atfree) printf("\nF(%d,%d): (",w,j);
		     atfree=0;
		     //printf("Connect %d---%d and ",u,j);
		     /* Did I connect it already(just now?) */
		     igraph_are_connected(g1,u,j,&connected);
//		     if(!connected) printf("Connect %d---%d and ",u,j);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,j);
		  }
	        }
//		if(shallAdd) printf("*");
                /* If not atfree, and edges are not added to backward neighbors earlier,
                 * add edge (v,j) in g1 */ 
		if(!atfree) {
		  /* Did I connect it already(just now?) */
		 // printf(") OR (Connect %d---%d)",v,j);
		  igraph_are_connected(g1,v,j,&connected);
//		  if(!connected) printf(") OR (Connect %d---%d)",v,j);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,j);
		  if(!shallAdd) printf("*");
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
//		     printf("Connect %d---%d and ",u,w);
		     igraph_are_connected(g1,u,w,&connected);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,w); 
		  }
	        } 
		if(shallAdd) printf("*");
		if(!atfree) {
//		  printf(") OR (Connect %d---%d)",v,w);
		  igraph_are_connected(g1,v,w,&connected);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,w);
		  if(!connected && !shallAdd) printf("*");
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

/* Our algorithm for necessay condition of Hamiltonian cycle in ATFree graph
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
 	int visited[maxc[l]];		
	for(int i=0;i<maxc[l];i++)
	   visited[i]=0;

	for(int i=0;i<maxc[l];i++) {
	   if(visited[i]==0) visited[i]=++minc[l];	
	   if(visited[i]<0) continue;
   	   igraph_vector_t snei;
    	   igraph_vector_init(&snei,1);
	   getNeighbors(g,start+i,loc,&snei,0); 
  //     	   printf("\n\t start=%d, start+i=%d,visited[i]=%d size(snei)=%d cc=%d",start,start+i,visited[i],igraph_vector_size(&snei)-1,cc);
	   for (int k=0; k<igraph_vector_size(&snei)-1; k++) {
	  	int u = (int) VECTOR(snei)[k];
	  	if(visited[u-start]==0) visited[u-start]=abs(visited[i]);
//		printf("\n\t\tu=%du-start=%d, visited[u-start]=%d",u,u-start,visited[u-start]);
	   }
	   visited[i]*=-1;
	   igraph_vector_destroy(&snei);
	}

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
   }
	/* If all needs are met, a simple HamCycle exists and return  else continue*/
	if(!isHamiltonian || totextra<0) printf("\nJ0[Simple]:Graph is non-hamiltonian");
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
       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        totextra+=extra[l];
		isHamiltonian = isHamiltonian && (totextra>=0);
	 }
	
	/* If a complex hamcycle exists, return true */	
	if(!isHamiltonian) printf("\nJ1[Complex]:Graph is non-hamiltonian");
	else printf("\nJ1[Complex]:Graph is hamiltonian");
  
	printf("\nNumber of vertices(l0 to l%d):",locsize-1);	
	for(int l=0;l<locsize;l++) 		   
	 printf("%d ",extra[l]); 
	return isHamiltonian;

}


/* This method checks the local AT-Free condition for vertices in same levels in both the directions. 
 * Returns true if the localcheck worked, else adds edges in g1 (which is essentially a copy of g)
 * to satisfy the local checkadds edges in g1 (which is essentially a copy of g)
 * to satisfy the local check . It takes j and it's current level non-neighbors as
 * input */
igraph_bool_t isLevelSatisfied_o(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1) {

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

/* This method checks the local AT-Free condition for vertices in same levels in both the directions. 
 * Returns true if the localcheck worked, else adds edges in g1 (which is essentially a copy of g)
 * to satisfy the local checkadds edges in g1 (which is essentially a copy of g)
 * to satisfy the local check . It takes j and it's current level non-neighbors as
 * input */
igraph_bool_t isLevelSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *cnn, igraph_t *g1) {

   igraph_bool_t connected, atfree, isatfree=1;

   /* Get all neighbours */
   igraph_vector_t jni;
   igraph_vector_init(&jni, 1);
   igraph_neighbors(g,&jni,j,IGRAPH_ALL);

  /* For each current non-neighbors of j */ 
  for (int i=0; i<igraph_vector_size(cnn)-1; i++) {
    	int w = (int) VECTOR(*cnn)[i];

        /* Get all neighbours of w */
        igraph_vector_t ini;
        igraph_vector_init(&ini, 1);
        igraph_neighbors(g,&ini,w,IGRAPH_ALL);

	/* All three conditions should hold good */
	/* 1. One common neighbor should exist */
	/* 2. One non-neighbor of j should exist N(w) */
	/* 3 One non-neighbor of w should exist in N(j) */
	igraph_vector_t inc,jnc,ijcc;
	igraph_vector_init(&jnc,0);
	igraph_vector_init(&inc,0);
	igraph_vector_init(&ijcc,0);

	igraph_vector_intersect_sorted(&ini,&jni,&ijcc);
	igraph_vector_difference_sorted(&ini,&jni,&jnc);
	igraph_vector_difference_sorted(&jni,&ini,&inc);
	
	printf("\nSize: i=%d,j=%d,common=%d,inc=%d,jnc=%d",w,j,igraph_vector_size(&ijcc),
		igraph_vector_size(&inc),igraph_vector_size(&jnc));
	printf("\n\tijcc=");igraph_vector_print(&ijcc);
	printf(", inc=");igraph_vector_print(&inc);
	printf(", jnc=");igraph_vector_print(&jnc);
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
	igraph_neighborhood(g,&ires,vsi,1,IGRAPH_ALL,0);
	for(int k=0;k<igraph_vector_ptr_size(&ires);k++) {
		igraph_vector_t *v = VECTOR(ires)[k];
		for(int t=0;t<igraph_vector_size(v);t++) {
		  int u = (int)VECTOR(*v)[t];
		  if(!igraph_vector_contains(&ic,u))
		     igraph_vector_insert(&ic,0,u);
		  }
	}
	igraph_vector_difference_sorted(&ic,&ijcc,&injc);
	igraph_neighborhood(g,&jres,vsj,1,IGRAPH_ALL,0);
	for(int k=0;k<igraph_vector_ptr_size(&jres);k++) {
		igraph_vector_t *v = VECTOR(jres)[k];
		for(int t=0;t<igraph_vector_size(v);t++) {
		  int u = (int)VECTOR(*v)[t];
		  if(!igraph_vector_contains(&ic,u))
		     igraph_vector_insert(&jc,0,u);
		  }
	}
	printf("\n\tic=");igraph_vector_print(&ic);
	printf(", injc=");igraph_vector_print(&injc);
	printf(", jc=");igraph_vector_print(&jc);
	igraph_vector_intersect_sorted(&injc,&jc,&ijc);
	printf(", ijc=");igraph_vector_print(&ijc);
	if(igraph_vector_size(&ijc)!=0) {
		atfree=0;
		printf("\n(%d,%d,%d) form an AT",j,w,(int)VECTOR(ijc)[0]);
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
   }
  return isatfree;		
}
