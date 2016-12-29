/* -*- mode: C -*-  */
/* 

*/

#include <igraph.h>
#include <math.h>
#include "igraph_atfree.h"
#define MAXV 50
#define MAXR 7 

int main(int argc, char** argv) {
  
  igraph_t g;
  time_t t;
  int count=500;

  while(count>0) {
  srand((unsigned) time(&t));
  int n = 6 + rand()%MAXV;
  int r = 2 + rand()%MAXR;
  if(n%2!=0 && r%2!=0) r = r+1;

  if(argc>1)
    n = atoi(argv[1]);

  if(argc>2)
    r = atoi(argv[2]);
 
  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  /* Random k-regular graph */
  printf("n=%d,r=%d\n",n,r);
  if(r>=n) continue;
  igraph_k_regular_game(&g, n, r, 0, 0);

  igraph_vector_t Y;
  igraph_vector_init(&Y,0);
  int dia;
  igraph_diameter(&g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf("Vertices=%d, Edges=%d, Diameter=%d, Connectivity=%d",igraph_vcount(&g), igraph_ecount(&g),dia,r);
  if(dia<=1) continue;

  igraph_vector_t X;
  char sspos[680];
  char sname[30];
  char gname[100];
  int loc[dia+1];
  igraph_t gmap1;
  igraph_vector_t map2;
  sprintf(gname,"rand%d_%d_%d",n,r,rand());

  /* Calculate dominating pair sets X and Y after arranging the graph 
   * and see if it is ATFree */
  igraph_bool_t isatfree = processForAT(&g,gname,0,loc,&X,&Y,&map2,&gmap1);
  sprintf(sspos,"n=%d, r=%d, isatfree=%d",n,r,isatfree);

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
