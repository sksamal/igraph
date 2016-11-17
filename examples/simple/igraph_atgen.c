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

  /* Write g to the file specified in stdin */ 
  FILE *fp = fopen(argv[1],"w");
  char spos[60];
  SETGAS(&g, "label", argv[1]);
  SETGAS(&g, "labelloc", "bottom");
  igraph_write_graph_dot(&g, fp);
  fclose(fp);

  sprintf(spos,"%s.edg",argv[1]);  
  fp = fopen(spos,"w");
  igraph_write_graph_edgelist(&g, fp);
  fclose(fp);
 
  igraph_destroy(&g);
  return 0;
}

