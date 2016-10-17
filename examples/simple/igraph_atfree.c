/* -*- mode: C -*-  */
/* 

*/

#include <igraph.h>

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
}
}
  /* Add edges 
  igraph_vector_init(&v, 4);
  VECTOR(v)[0]=2; VECTOR(v)[1]=1;
  VECTOR(v)[2]=3; VECTOR(v)[3]=3;
  igraph_add_edges(&g, &v, 0);

  if (igraph_vcount(&g) != 0) {
    return 1;
  }
  if (igraph_ecount(&g) != 0) {
    return 2;
  } */
//  igraph_write_graph_graphml(&g, stdout, /*prefixattr=*/ 1);
  igraph_write_graph_dot(&g, stdout);
  igraph_destroy(&g);
  return 0;
}
