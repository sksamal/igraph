/* -*- mode: C -*-  */
/* 

*/

#include <igraph.h>
#include <math.h>
#include "igraph_atfree.h"
#define MINV 6 
#define MIND 0 
#define MAXV 70
#define MAXD 20

int main(int argc, char** argv) {
  
  igraph_t g;
  time_t t;
  int count=3000;

  while(count>0) {
  srand((unsigned) time(&t));
  int n = 6 + MINV + rand()%MAXV;

  if(argc>1)
    n = atoi(argv[1]);

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  /* Random graph using random degree sequence*/
  printf("Vertices(n)=%d\n",n);
/*  igraph_vector_t degs;
  igraph_vector_init(&degs,n);

  int sum=0;
  for(int i=0;i<n;i++) {
    VECTOR(degs)[i] = 2 + rand()%MAXD;
    sum+=(int)VECTOR(degs)[i];
  }
  if(sum%2!=0) VECTOR(degs)[n-1]= (int)VECTOR(degs)[n-1]-1;
 
  igraph_degree_sequence_game(&g,&degs,NULL,IGRAPH_DEGSEQ_VL); 
*/
  igraph_empty(&g,n,0);
 
  /* Step1 - Create a simple two-connected graph */
    for(int i=0;i<n;i++)
     {
      igraph_vector_t nei;
      igraph_vector_init(&nei,0);
      while(igraph_vector_size(&nei) < 2) {
      	igraph_neighbors(&g,&nei,i,IGRAPH_ALL);
      	int d = rand()%n;
      	igraph_bool_t connected;
      	igraph_are_connected(&g,i,d,&connected);
      	if(!connected && i!=d)
      	   igraph_add_edge(&g,i,d);
      }
     }
  
   int mdeg = n*(MIND + rand()%MAXD);
   while(mdeg>0) {
     int s = rand()%n;
     int d = rand()%n;
     igraph_bool_t connected;
     igraph_are_connected(&g,s,d,&connected);
     if(!connected && s!=d)
     	igraph_add_edge(&g,s,d);
     mdeg--;
   }

  igraph_vector_t Y;
  igraph_vector_init(&Y,0);
  int dia;
  igraph_diameter(&g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf("Vertices=%d, Edges=%d, Diameter=%d",igraph_vcount(&g), igraph_ecount(&g),dia);
  if(dia<=2) continue;

  igraph_vector_t X;
  char sspos[680];
  char sname[30];
  char gname[100];
  int loc[dia+1];
  igraph_t gmap1;
  igraph_vector_t map2;
  sprintf(gname,"rand%d_%d_%d",n,dia,rand());

  /* Calculate dominating pair sets X and Y after arranging the graph 
   * and see if it is ATFree */
  igraph_bool_t isatfree = processForAT(&g,gname,0,loc,&X,&Y,&map2,&gmap1);
  sprintf(sspos,"n=%d, isatfree=%d",n,isatfree);

  if(isatfree)  {
  	printf("\nG is AT-Free");
  	sprintf(sspos,"atfs/%s.dot.edg",gname);  
  	FILE *fp = fopen(sspos,"w");
  	igraph_write_graph_edgelist(&g, fp);
  	fclose(fp);
	count--;
  }
  else 
	printf("\nG is not AT-Free");
  /* Move all files to temp */
  sprintf(sspos,"mv *%s* temp/; rm -rf temp/*%s*",gname,gname);
  system(sspos);
//  exportToDot(&g,loc,dia+1,gname,sspos,&map2,1);

  igraph_vector_destroy(&map2);
  igraph_vector_destroy(&X);
  igraph_vector_destroy(&Y);
  igraph_destroy(&gmap1);
  igraph_destroy(&g);
  }
  return 0;
}
