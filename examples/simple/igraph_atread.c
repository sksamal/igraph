/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include "igraph_atfree.h"

int main(int argc, char** argv) {
  
  igraph_t g;
  igraph_vector_t v;
  int ret;
  time_t t;
  srand((unsigned) time(&t));

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  /* empty directed graph, zero vertices */
  igraph_empty(&g, 0, IGRAPH_UNDIRECTED);

  if(argc < 2) { 
	printf("\nUsage: %s <edgelistfile> \n\t file to read",argv[0]);
	printf("\nExample: %s aa for (aa.dot.edg)",argv[0]);
	exit(0);
   }

  char fileName[40];
  sprintf(fileName,"%s.dot.edg",argv[1]);
  FILE *fp = fopen(fileName,"r");
  if(fp == NULL) {
     printf("\nFile %s not found",argv[1]); 
     exit(0);
   }
  //igraph_read_graph_edgelist(&g,fp,0,0);
  char desc[100];
  read_graph_edgelist(&g,desc,fp,0,0);
  printf("\nDesc:%s",desc);
/* 
 int n = igraph_vcount(&g);
 int m = igraph_ecount(&g);
// printf("\nNumber of vertices=%d",n);
// printf("\nNumber of Edges=%d",m);
*/
 igraph_vector_t Y;
 igraph_vector_init(&Y,0);
 igraph_bool_t oiso, iso1, iso2;

 /* Do LBFS and delete X in each step */
// for(int iter=0;igraph_vcount(&g)>0;iter++) {
 for(int iter=0;iter<1;iter++) {

/*  n = igraph_vcount(&g);
  m = igraph_ecount(&g);
  printf("\nNumber of vertices=%d",n);
  printf(", Edges=%d",m);
  igraph_integer_t result,dia;
  igraph_vertex_connectivity(&g,&result,1);
  printf("\nVertex connectivity=%d",result);*/
  int dia;
  igraph_diameter(&g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
//  printf(", Diameter=%d",dia);

  igraph_vector_t X;
  char sspos[680];
  char sname[30];
  char gname[100];
  int loc[dia+1];
  igraph_t gmap1;
  igraph_vector_t map2;
  sprintf(gname,"%s_%d",argv[1],iter);
  /* Calculate dominating pair sets X and Y after arranging the graph 
   * and see if it is ATFree */
  igraph_bool_t isatfree = processForAT(&g,gname,0,loc,&X,&Y,&map2,&gmap1);

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  /* Finally run the same test on vertices of gmap1 (already arranged)
   * to make sure it is AT-Free */
  igraph_t gmap2;
  igraph_copy(&gmap2,&gmap1);
  int isatfree1=1;
  isATFree(&gmap1,gname,0,loc,dia+1,&isatfree1,&gmap2);

  if(isatfree1) printf("\nG' is AT-Free");
  else printf("\nG' is not AT-Free");
 
  printf("\nV=%d,E=%d MV=%d,ME=%d",igraph_vcount(&g),igraph_ecount(&g),igraph_vcount(&gmap1),igraph_ecount(&gmap1));
  igraph_simplify(&gmap1,1,1,NULL);
  igraph_simplify(&g,1,1,NULL);
  printf("\nV=%d,E=%d MV=%d,ME=%d",igraph_vcount(&g),igraph_ecount(&g),igraph_vcount(&gmap1),igraph_ecount(&gmap1));
  /* Write gmap1 to <at><file specified in stdin> */
//  sprintf(sname,"at-%s",argv[1]);  
  oiso=isHamiltonian(&gmap1,loc,dia+1);
//  if(oiso) 
    sprintf(sname,"hat-%s_%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s, ATFree=%d,OurAlgo=%d\nDesc:%s",sname,isatfree1,oiso,desc);
  exportToDot(&gmap1,loc,dia+1,sname,sspos,&map2,1);

  /* Run the LAD and VFS isomorphism algorithms 
   *   present in igraph and if successful rewrite gmap1*/
  char path1[300], path2[300];
  path1[0]='\0';
  path2[0]='\0';
  iso1=-1, iso2=-1;
  isHamUsingLAD(&gmap1,&iso1, path1);
  isHamUsingVF2(&gmap1,&iso2, path2);
  sprintf(sspos,"Graph=%s, AT-Free=%d,OurAlgo=%d\nDesc:%sLAD=%d [%s],\nVF2=%d [%s] ",sname,isatfree1,oiso,desc,iso1, path1,iso2, path2);
  exportToDot(&gmap1,loc,dia+1,sname,sspos,&map2,1);

  igraph_vs_t vs;
  igraph_vs_vector(&vs, &X);
  printf("\nMinPaths=%d\n",minPaths(&g,argv[1],0,&Y));
  printf("\nIter%d :Deleting vertices.. ",iter);igraph_vector_print(&X);
  igraph_delete_vertices(&g,vs);
  igraph_vector_t Ytemp;
  igraph_vector_copy(&Ytemp,&Y);
  for(int i=0;i<igraph_vector_size(&X);i++)
      for(int j=0;j<igraph_vector_size(&Ytemp);j++)
         if((int)VECTOR(X)[i] < (int)VECTOR(Ytemp)[j])  VECTOR(Y)[j] = VECTOR(Y)[j] -1;
  igraph_vector_destroy(&Ytemp);

  sprintf(sspos,"%s_%d.dot.edg",argv[1],iter);  
  fp = fopen(sspos,"w");
  igraph_write_graph_edgelist(&g, fp);
  fclose(fp);
  igraph_vector_destroy(&map2);
  igraph_vector_destroy(&X);
  igraph_destroy(&gmap1);
  igraph_destroy(&gmap2);
  igraph_vs_destroy(&vs);
  if(!isatfree)  /* Not AT-Free */
     return 7;
 }

  igraph_vector_destroy(&Y);
  igraph_destroy(&g);

  if(oiso && (iso1 || iso2)) /* Ham-Ok */
     return 1;
  if(!oiso && (iso1==0 || iso2==0)) /* NonHam-ok */
     return 2;
  if(oiso && (iso1==0 || iso2==0)) /* Issue */
     return 3;	
  if(!oiso && (iso1 || iso2)) /* Issue */
     return 4;
  if(oiso && (iso1<0 && iso2 <0))  /* Ham, unverified */
     return 5;
  if(!oiso && (iso1<0 && iso2 <0)) /* NonHam, unverified */
     return 6;
}
