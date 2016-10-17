/* -*- mode: C -*-  */
/* 

*/

#include <igraph.h>

void cutNeighbors(igraph_t* g,int j,int *loc,igraph_vector_t *bni, igraph_vector_t *cni, igraph_vector_t *nni);
int main() {
  
  igraph_t g;
  igraph_vector_t v;
  int ret;
  time_t t;
  srand((unsigned) time(&t));

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  // l = number of levels, s = max size of each level (>=2)
  int conns=3;
  int l=rand()%7+conns,s=rand()%7+conns;  
  int loc[l];    // store start
  double scale=0.4;
  int count=0;


  for(int i=0;i<l;i++) {
	count+= (s>conns)?(rand()%(s-conns)+conns):conns;
	loc[i] = count;
//	printf("%d ",loc[i]);
  }	
 
  /* empty directed graph, zero vertices */
  igraph_empty(&g, count, IGRAPH_UNDIRECTED);

  igraph_vector_t neighs;
  igraph_vector_init(&neighs,8);
  igraph_bool_t connected;
  for(int i=0;i<l;i++) {
    int p = (i<=1)?0:loc[i-2];
    int c = (i>0)?loc[i-1]:0;
    int n = loc[i];
    for (int j=c;j<n;j++)  { /* Vertices in level i */ 

     igraph_neighbors(&g,&neighs,j,IGRAPH_ALL);
     int cs = igraph_vector_size(&neighs); 
     while(cs<conns) {

	int dir=0;
    	if((i!=0) && (i==l-1 || dir==0)) { 
		int rb = rand()%(loc[i-1]-p); 
		igraph_are_connected(&g,j, p+rb,&connected);
//		printf("Back: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rb,loc[i-1]-p);
		if(!connected)
		     { igraph_add_edge(&g,j,p+rb); dir=1; cs++; }
		}
	if((i!=l-1) && (i==0 || dir==1)) 	{
		int rf = rand()%(loc[i+1]-n); 
		igraph_are_connected(&g,j, n+rf,&connected);
//		printf("Forward: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rf,loc[i+1]-n);
		if(!connected)
		   { igraph_add_edge(&g,j,n+rf); dir=0; cs++; } 
		}

	int t=rand()%(3*(n-c));
	if((cs>1) && (t < (n-c))) {
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

  igraph_vector_t left,current, right;
  for(int j=0; j<loc[l-1];j++)
   {
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      cutNeighbors(&g,j,loc,&left,&current,&right);
      printf("\n j=%d: L=( ",j);
      for(int i=0;i<igraph_vector_size(&left)-1;i++)
 	printf("%d ",(int) VECTOR(left)[i]);	
      printf(") C=( ");
      for(int i=0;i<igraph_vector_size(&current)-1;i++)
 	printf("%d ",(int) VECTOR(current)[i]);	
      printf(") R=( ");
      for(int i=0;i<igraph_vector_size(&right)-1;i++)
 	printf("%d ",(int) VECTOR(right)[i]);	
      printf(")");
//      printf("\n\t\t j=%d : left=%d curr=%d right=%d",j, igraph_vector_size(&left),igraph_vector_size(&current),igraph_vector_size(&right));
  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
   }

  SETGAS(&g, "name", "AT-Free graph");
  SETGAN(&g, "vertices", igraph_vcount(&g));
  SETGAN(&g, "edges", igraph_ecount(&g));
//  SETGAB(&g, "famous", 1);
  igraph_simplify(&g,1,1,0);
  for(int i=0;i<l;i++) {
    int o = (i==0)?0:loc[i-1];
    for(int j=o;j<loc[i];j++) {
  char spos[8];
  int low = (s-(loc[i]-o))/2;
 // SETVAS(&g, "fixedsize",j,"true");
  sprintf(spos,"%f,%f!",i*scale,(j-o+low)*scale);
  SETVAS(&g, "pos",j,spos);
  SETVAS(&g, "shape",j,"point");
  SETVAN(&g, "fontsize",j,6);
  SETVAN(&g, "xlabel",j,j);
}
}
//  igraph_write_graph_graphml(&g, stdout, /*prefixattr=*/ 1);
  FILE *fp = fopen("aa.dot","w");
  igraph_write_graph_dot(&g, fp);
  fclose(fp);
  igraph_destroy(&g);
  return 0;
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

  for (i=0; i<igraph_vector_size(&ni); i++) {
    int w = (int) VECTOR(ni)[i];
//    printf("\nj=%d :: i=%d, w=%d, b=%d, c=%d, n=%d", j, i, w, b, c, n);
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
