/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include "igraph_atfree.h"
#define MLEVELS 7
#define LSIZE 7 

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
  // l = number of levels, s = max size of each level (>=2) , conns = minimum connectivity
  int conns=3;    
  int l=rand()%MLEVELS+conns,s=rand()%LSIZE+conns;  
  int loc[l];    // store start
  double scale=1.0;
  int count=0;

  /* Randomly determine the size for every level 
 *   loc[0] = second level starting vertex and so on  */
  for(int i=0;i<l;i++) {
	count+= (s>conns)?(rand()%(s-conns)+conns):conns;
	loc[i] = count;
//	printf("%d ",loc[i]);
  }	
 
  /* empty directed graph, zero vertices */
  igraph_empty(&g, count, IGRAPH_UNDIRECTED);
  igraph_bool_t connected;

  /* For every level, find the previous, current and next starting vertex */
  for(int i=0;i<l;i++) {
    int p = (i<=1)?0:loc[i-2];
    int c = (i>0)?loc[i-1]:0;
    int n = loc[i];

    for (int j=c;j<n;j++)  { /* Vertices in level i */ 

     /* Get the left, right and current neighbors using our method */
     igraph_vector_t left,current,right;
     igraph_vector_init(&left,1);
     igraph_vector_init(&current,1);
     igraph_vector_init(&right,1);
     cutNeighbors(&g,j,loc,&left,&current,&right);
//     igraph_bool_t backward = (i==0)?1:igraph_vector_size(&left)-1;
//     igraph_bool_t forward = (i==l-1)?1:igraph_vector_size(&right)-1;
//
     /* other = does it have connections in other levels ? 
      * conns = Get the existing connections for the vertex j */
     igraph_bool_t other = ((i==0)?0:igraph_vector_size(&left) -1) + ((i==l-1)?0:igraph_vector_size(&right) -1);
     int cs = igraph_vector_size(&left)-1 + igraph_vector_size(&right) -1 + igraph_vector_size(&current)-1; 

     /* We start adding edges in forward and backward directions alternately making sure, there is 
      * atleast one left or right neighbor*/
     int dir=1;
     while(cs<conns || !other) {

    	if((i!=0) && (i==l-1 || dir==0)) 	{ 
		int rb = rand()%(loc[i-1]-p); 
		igraph_are_connected(&g,j, p+rb,&connected);
//		printf("Back: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rb,loc[i-1]-p);
		if(!connected)
		     { igraph_add_edge(&g,j,p+rb); dir=1; cs++; }
		other = 1;
		}
	if((i!=l-1) && (i==0 || dir==1)) 	{
		int rf = rand()%(loc[i+1]-n); 
		igraph_are_connected(&g,j, n+rf,&connected);
//		printf("Forward: i=%d,cs=%d,isconn=%d,rb=%d,size=%d\n",i, cs, connected,rf,loc[i+1]-n);
		if(!connected)
		   { igraph_add_edge(&g,j,n+rf); dir=0; cs++; } 
		other = 1;
		}

        /* Only after minimum requirement is satisfied, add edges in the same level,
         * use a randomness of 1/3 */
	int t=rand()%(3*(n-c));
	if((cs>0) && (t < (n-c)) && (c+t!=j)) {
            igraph_are_connected(&g,j, c+t,&connected);
	    if(!connected)
	      { igraph_add_edge(&g,j,c+t); cs++; } 
	}
    }	
  }
  }

/*  igraph_vector_t *neighs;               // neighbors example
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
//  igraph_simplify(&g,1,1,0);   // removes cycles and edges
  igraph_integer_t result;
  igraph_vertex_connectivity(&g,&result,1);
  printf("\nVertex connectivity=%d",result);

  /* Set the positions in .dot file */
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

  /* Copy the graph to g1, check localATFree condition
   *  and make it AT-Free if it is not already */
  igraph_t g1;
  igraph_vector_t left,current, right,fnn;
  igraph_bool_t isatfree=1;
  igraph_copy(&g1,&g);

  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check */
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

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy og g */   
      isatfree = isatfree & isDominSatisfied(&g,j,loc,&fnn,&g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
   }

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  /* Finally run the same test on vertices of g1 
   * to make sure it is AT-Free */
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
 
  /* Write g to the file specified in stdin */ 
  FILE *fp = fopen(argv[1],"w");
  char spos[680];
  sprintf(spos,"Graph=%s, AT-Free=%d",argv[1],isatfree);
  SETGAS(&g, "label", spos);
  SETGAS(&g, "labelloc", "bottom");
  igraph_write_graph_dot(&g, fp);
  fclose(fp);
  sprintf(spos,"%s.edg",argv[1]);  

  fp = fopen(spos,"w");
  igraph_write_graph_edgelist(&g, fp);
  fclose(fp);
 
  /* Write g1 to <m><file specified in stdin> */
  sprintf(spos,"m%s",argv[1]);  
  fp = fopen(spos,"w");
  igraph_bool_t oiso=isHamiltonian(&g1,loc,l);
  sprintf(spos,"Graph=%s, ATFree=%d,OurAlgo=%d",argv[1],isatfree1,oiso);
  SETGAS(&g1, "label", spos);
  SETGAS(&g, "labelloc", "bottom");
  igraph_write_graph_dot(&g1, fp);
  fclose(fp);

  sprintf(spos,"m%s.edg",argv[1]);  
  fp = fopen(spos,"w");
  igraph_write_graph_edgelist(&g1, fp);
  fclose(fp);

  /* Run the LAD and VFS isomorphism algorithms 
   *   present in igraph and if successful rewrite g1*/
  char path1[300], path2[300];
  path1[0]='\0';
  path2[0]='\0';
  igraph_bool_t iso1=0, iso2=0;
  isHamUsingLAD(&g1,&iso1, path1);
  isHamUsingVF2(&g1,&iso2, path2);
  sprintf(spos,"m%s",argv[1]);
  fp = fopen(spos,"w");
  sprintf(spos,"Graph=%s, AT-Free=%d,OurAlgo=%d,\nLAD=%d [%s],\nVF2=%d [%s] ",argv[1],isatfree1,oiso,iso1, path1,iso2, path2);
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

