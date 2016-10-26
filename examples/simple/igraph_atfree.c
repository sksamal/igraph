/* -*- mode: C -*-  */

#include <igraph.h>

void cutNeighbors(igraph_t* g,int j,int *loc,igraph_vector_t *bni, igraph_vector_t *cni, igraph_vector_t *nni);
igraph_bool_t isDominSatisfied(igraph_t* g, int j, int *loc, igraph_vector_t *fnn, igraph_t *g1);
void forwardNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *fnn);
void getNeighbors(igraph_t* g,int j, int *loc,igraph_vector_t *ne, int dir);
igraph_bool_t isHamiltonian(igraph_t* g, int *loc, int l);

int main(int argc, char** argv) {
  
  igraph_t g;
  igraph_vector_t v;
  int ret;
  time_t t;
  srand((unsigned) time(&t));

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  if(argc < 2) { 
	printf("\nUsage: %s <dotfile>",argv[0]);
	exit(1);
   }
  // l = number of levels, s = max size of each level (>=2)
  int conns=3;
  int l=rand()%7+conns,s=rand()%7+conns;  
  int loc[l];    // store start
  double scale=1.0;
  int count=0;


  for(int i=0;i<l;i++) {
	count+= (s>conns)?(rand()%(s-conns)+conns):conns;
	loc[i] = count;
//	printf("%d ",loc[i]);
  }	
 
  /* empty directed graph, zero vertices */
  igraph_empty(&g, count, IGRAPH_UNDIRECTED);
  igraph_bool_t connected;

  for(int i=0;i<l;i++) {
    int p = (i<=1)?0:loc[i-2];
    int c = (i>0)?loc[i-1]:0;
    int n = loc[i];
    for (int j=c;j<n;j++)  { /* Vertices in level i */ 

     igraph_vector_t left,current,right;
     igraph_vector_init(&left,1);
     igraph_vector_init(&current,1);
     igraph_vector_init(&right,1);
     cutNeighbors(&g,j,loc,&left,&current,&right);
     igraph_bool_t backward = (i==0)?1:igraph_vector_size(&left)-1;
     igraph_bool_t forward = (i==l-1)?1:igraph_vector_size(&right)-1;
     int cs = igraph_vector_size(&left)-1 + igraph_vector_size(&right) -1 + igraph_vector_size(&current)-1; 
     int dir=1;
     while(cs<conns || !backward || !forward) {

    	if((i!=0) && (i==l-1 || dir==0)) { 
		int rb = rand()%(loc[i-1]-p); 
		igraph_are_connected(&g,j, p+rb,&connected);
//		printf("Back: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rb,loc[i-1]-p);
		if(!connected)
		     { igraph_add_edge(&g,j,p+rb); dir=1; cs++; }
		backward = 1;
		}
	if((i!=l-1) && (i==0 || dir==1)) 	{
		int rf = rand()%(loc[i+1]-n); 
		igraph_are_connected(&g,j, n+rf,&connected);
//		printf("Forward: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rf,loc[i+1]-n);
		if(!connected)
		   { igraph_add_edge(&g,j,n+rf); dir=0; cs++; } 
		forward = 1;
		}

	int t=rand()%(3*(n-c));
	if((cs>1) && (t < (n-c)) && (c+t!=j)) {
            igraph_are_connected(&g,j, c+t,&connected);
	    if(!connected)
	      { igraph_add_edge(&g,j,c+t); cs++; } 
	}
    }	
  }
  }

/*  igraph_vector_t *neighs;
  for(int i=0;i<igraph_vcount(&g);i++)
   {
     igraph_neighbors(&g,neighs,i,0);
     igraph_vector_size(neighs); 
   } */

  SETGAS(&g, "name", "Graph G");
  SETGAS(&g, "label", "Graph G");
  SETGAN(&g, "vertices", igraph_vcount(&g));
  SETGAN(&g, "edges", igraph_ecount(&g));
//  SETGAB(&g, "famous", 1);
//  igraph_simplify(&g,1,1,0);
  igraph_integer_t result;
  igraph_vertex_connectivity(&g,&result,1);
  printf("\nVertex connectivity=%d",result);
  double off=0.1;
  for(int i=0;i<l;i++) {
    int o = (i==0)?0:loc[i-1];
    for(int j=o;j<loc[i];j++) {
  char spos[8];
  int low = (s-(loc[i]-o))/2;
 // SETVAS(&g, "fixedsize",j,"true");
  sprintf(spos,"%f,%f!",i*scale-((j-o)%2)*off,(j-o+low)*scale);
  SETVAS(&g, "pos",j,spos);
  SETVAS(&g, "shape",j,"point");
  SETVAN(&g, "fontsize",j,6);
  SETVAN(&g, "xlabel",j,j);
}
}

  igraph_t g1;
  /* Check if AT-Free */
  igraph_vector_t left,current, right,fnn;
  igraph_bool_t isatfree=1;
  igraph_copy(&g1,&g);

  for(int j=0; j<loc[l-1];j++)
   {
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      cutNeighbors(&g,j,loc,&left,&current,&right);
      forwardNonNeighbors(&g,j,loc,l,&fnn);
/*      printf("\n j=%d: L=( ",j);
      for(int i=0;i<igraph_vector_size(&left)-1;i++)
 	printf("%d ",(int) VECTOR(left)[i]);	
      printf(") C=( ");
      for(int i=0;i<igraph_vector_size(&current)-1;i++)
 	printf("%d ",(int) VECTOR(current)[i]);	
      printf(") R=( ");
      for(int i=0;i<igraph_vector_size(&right)-1;i++)
 	printf("%d ",(int) VECTOR(right)[i]);	
      printf(") FNN=( ");
      for(int i=0;i<igraph_vector_size(&fnn)-1;i++)
 	printf("%d ",(int) VECTOR(fnn)[i]);	
      printf(")"); */
//      printf("\n\t\t j=%d : left=%d curr=%d right=%d",j, igraph_vector_size(&left),igraph_vector_size(&current),igraph_vector_size(&right));
      isatfree = isatfree & isDominSatisfied(&g,j,loc,&fnn,&g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
   }

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  igraph_t g2;
  igraph_copy(&g2,&g1);
  int isatfree1=1;
  for(int j=0;j<igraph_vcount(&g1);j++) {
  igraph_vector_init(&fnn,1);
  forwardNonNeighbors(&g1,j,loc,l,&fnn);
  isatfree = isatfree & isDominSatisfied(&g1,j,loc,&fnn,&g2);
  }
  if(isatfree1) printf("\nG' is AT-Free");
  else printf("\nG' is not AT-Free");
  
//  igraph_write_graph_graphml(&g, stdout, /*prefixattr=*/ 1);
  FILE *fp = fopen(argv[1],"w");
  char spos[80];
  sprintf(spos,"Graph=%s, AT-Free=%d",argv[1],isatfree);
  SETGAS(&g, "label", spos);
  SETGAS(&g, "labelloc", "bottom");
  igraph_write_graph_dot(&g, fp);
  fclose(fp);
  sprintf(spos,"m%s",argv[1]);
  fp = fopen(spos,"w");
  sprintf(spos,"Graph=%s, AT-Free=%d, Hamiltonian=%d",argv[1],isatfree1,isHamiltonian(&g1,loc,l));
  SETGAS(&g1, "label", spos);
  SETGAS(&g, "labelloc", "bottom");
//  SETGAS(&g, "labeljust", "left");
  igraph_write_graph_dot(&g1, fp);
  fclose(fp);
  igraph_destroy(&g);
  igraph_destroy(&g1);
  igraph_destroy(&g2);
  return 0;
}

// dir=-1 means left, dir=0 current, dir=1 means right 
void getNeighbors(igraph_t* g, int j, int *loc, igraph_vector_t *ne, int dir) {
      igraph_vector_t left,current,right;
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);

      if(dir>0) cutNeighbors(g,j,loc,&left,&current,ne);
      if(dir<0) cutNeighbors(g,j,loc,ne,&current,&right);
      else cutNeighbors(g,j,loc,&left,ne,&right); 
	
      igraph_vector_destroy(&left);
      igraph_vector_destroy(&current);
      igraph_vector_destroy(&right);
}

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

   int b=(i==0)?-999:((i==1)?0:loc[i-2]);
   int c=(i==0)?0:loc[i-1];
   int n=loc[i];

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

void forwardNonNeighbors(igraph_t* g, int j, int *loc, int locsize, igraph_vector_t *fnn) {

   int i=0;
   while(j>=loc[i]) i++;
   if(i==locsize-1) return ;
   igraph_bool_t connected;
   for(int k=loc[i]; k<loc[i+1];k++) {
      igraph_are_connected(g,j,k,&connected);
      if(!connected)
	igraph_vector_insert(fnn,0,k);
   }
   igraph_vector_resize_min(fnn);
}

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
		igraph_bool_t shallAdd = rand()%2;
		for (int l=0; l<igraph_vector_size(&kbni)-1; l++) {
		  int u = (int) VECTOR(kbni)[l];
	          igraph_are_connected(g,u,j,&connected);
		  if(!connected) {
		     if(atfree) printf("\n(");
		     atfree=0;
		     printf("Connect %d---%d and ",u,j);
		     igraph_are_connected(g1,u,j,&connected);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,j);
		  }
	        }
		if(!atfree) {
		  printf(") OR (Connect %d---%d)",v,j);
		  igraph_are_connected(g1,v,j,&connected);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,j);
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
   	/* Find forward neighbors of j */
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
		igraph_bool_t shallAdd = rand()%2;
		for (int l=0; l<igraph_vector_size(&knni)-1; l++) {
		  int u = (int) VECTOR(knni)[l];
	          igraph_are_connected(g,u,w,&connected);
		  if(!connected) {
		     if(atfree) printf("\n(");
		     atfree=0;
		     printf("Connect %d---%d and ",u,w);
		     igraph_are_connected(g1,u,w,&connected);
		     if(shallAdd && !connected) igraph_add_edge(g1,u,w); 
		  }
	        }
		if(!atfree) {
		  printf(") OR (Connect %d---%d)",v,w);
		  igraph_are_connected(g1,v,w,&connected);
		  if(!connected && !shallAdd) igraph_add_edge(g1,v,w);
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

/* Sufficient condition for Hamiltonian */
igraph_bool_t isHamiltonian(igraph_t *g,int *loc,int locsize) {

   igraph_vector_t snei;
   igraph_vector_init(&snei,1);
   int start = 0, totextra=0;;
   int contr[locsize], needc[locsize], extra[locsize], maxc[locsize], minc[locsize];
   int isHamiltonian = 1;
   printf("\nLevel [minc,maxc] needc extra contr");
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

       if(l==0 || l==locsize-1) needc[l]=1;
       else needc[l]=2;

       if(needc[l]>maxc[l]) isHamiltonian = 0;
       extra[l]=(needc[l]-minc[l]);
       contr[l]=(maxc[l]-needc[l]);
       //printf("\nLevel [minc,maxc] needc extra contr");
       printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
       start = loc[l];
       totextra+=extra[l];
//       off+=(size-cc-2);
   }
	if(!isHamiltonian || totextra<0) printf("\nJ0:Graph is non-hamiltonian");
	else { printf("\nJ0:Graph is hamiltonian"); return 1; }

	totextra=0;
   	printf("\n\nLevel [minc,maxc] needc extra contr");
   	for(int l=0;l<locsize;l++) {
		if(l!=0)  
		   while(extra[l]<0 && contr[l-1]>0)
			{ extra[l]++; contr[l-1]--; }

		while(extra[l]<0 && contr[l+1]>0)
			{ extra[l]++; contr[l+1]--; }
       		printf("\n%5d| [%3d,%3d]  %5d %5d %5d",l,minc[l],maxc[l],needc[l],extra[l],contr[l]);
	        totextra+=extra[l];
	 }
			
	if(!isHamiltonian || totextra<0) printf("\nJ1:Graph is non-hamiltonian");
	else printf("\nJ1:Graph is hamiltonian");
	   
	return (isHamiltonian && totextra>=0);
}
