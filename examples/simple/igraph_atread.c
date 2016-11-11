/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include <string.h>
#include "igraph_atfree.h"

void isATFree(igraph_t *g, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1);
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text);
void LBFS(igraph_t *g, igraph_vector_t *Y, igraph_vector_t *X, igraph_vector_t *label);

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
	printf("\nUsage: %s <dotfile> \n\t file to read",argv[0]);
	exit(1);
   }

  char fileName[40];
  sprintf(fileName,"%s.edg",argv[1]);
  FILE *fp = fopen(fileName,"r");
//  igraph_read_graph_dot(&g, fp);
  igraph_read_graph_edgelist(&g,fp,0,0);
 
  int n = igraph_vcount(&g);
  int m = igraph_ecount(&g);
  printf("\nNumber of vertices=%d",n);
  printf("\nNumber of Edges=%d",m);

  igraph_integer_t result,dia;
  igraph_vertex_connectivity(&g,&result,1);
  printf("\nVertex connectivity=%d",result);
  igraph_diameter(&g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf("\nDiameter=%d",dia);
  int loc[dia+1];

  /* Calculate dominating pair set */
  igraph_vector_t X,Y,label1,label2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&X,1);
  igraph_vector_init(&Y,1);
  VECTOR(Y)[0] = rand()%n;
  
  LBFS(&g,&Y,&X,&label1);
  LBFS(&g,&X,&Y,&label2);
  exit(1);
  igraph_t g1;
  igraph_empty(&g1, n, IGRAPH_UNDIRECTED);
  igraph_copy(&g1,&g);
//  OrderGrid(&g, &g1, &loc, X, Y);

  /* Copy the graph to g2, check ATFree condition
   *  and make it AT-Free if it is not already */
  igraph_t g2;
  igraph_copy(&g2,&g1);
  igraph_bool_t isatfree;
  isATFree(&g1,loc,dia+1,&isatfree,&g2);

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  /* Finally run the same test on vertices of g3 
   * to make sure it is AT-Free */
  igraph_t g3;
  igraph_copy(&g3,&g2);
  int isatfree1=1;
  isATFree(&g2,loc,dia+1,&isatfree1,&g3);

  if(isatfree1) printf("\nG' is AT-Free");
  else printf("\nG' is not AT-Free");
 
  /* Write g1 to the file specified in stdin */ 
  char spos[680];
  char name[30];
  sprintf(name,"gd-%s",argv[1]);  
  sprintf(spos,"Graph=%s, AT-Free=%d",name,isatfree);
  exportToDot(&g1,loc,dia+1,name,spos);

  /* Write g2 to <at><file specified in stdin> */
  sprintf(name,"at-%s",argv[1]);  
  igraph_bool_t oiso=isHamiltonian(&g2,loc,dia+1);
  if(oiso) sprintf(name,"h-%s",name);  
  sprintf(spos,"Graph=%s, ATFree=%d,OurAlgo=%d",argv[1],isatfree1,oiso);
  exportToDot(&g2,loc,dia+1,name,spos);

  /* Run the LAD and VFS isomorphism algorithms 
   *   present in igraph and if successful rewrite g1*/
  char path1[300], path2[300];
  path1[0]='\0';
  path2[0]='\0';
  igraph_bool_t iso1=0, iso2=0;
  isHamUsingLAD(&g1,&iso1, path1);
  isHamUsingVF2(&g1,&iso2, path2);
  sprintf(spos,"Graph=%s, AT-Free=%d,OurAlgo=%d,\nLAD=%d [%s],\nVF2=%d [%s] ",argv[1],isatfree1,oiso,iso1, path1,iso2, path2);
  exportToDot(&g2,loc,dia+1,name,spos);
  igraph_destroy(&g);
  igraph_destroy(&g1);
  igraph_destroy(&g2);
  return 0;
}

void LBFS(igraph_t *g, igraph_vector_t *vstart, igraph_vector_t *X, igraph_vector_t *label) {

   /* Starting label */ 
   int n = igraph_vcount(g);
   igraph_strvector_t llabel;
   igraph_strvector_init(&llabel, n);
   for(int i=0;i<igraph_vector_size(vstart);i++)
      igraph_strvector_set(&llabel,i,"000");
 
   char strlabel[100];
   sprintf(strlabel,"%3d",n+1);

   for(int i=0;i<igraph_vector_size(vstart);i++) {
   	int v = VECTOR(*vstart)[i];
   	/* Set lexlabel of v as some positive number */
   	igraph_strvector_set(&llabel,v,strlabel);
     } 

   while(n>0) {

	/* Find the maximum and its index */
	int maxes = 1, maxValue=0, maxIndex=0;
	char maxlabel[100];
	maxlabel[0] = '\0';
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
	    //if(maxValue==(int)VECTOR(llabel)[i]) maxes++;
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) maxes++;
	  //  if(maxValue<(int)VECTOR(llabel)[i]) 
	    if(strcmp(maxlabel,tmplabel)<0)
//	      { maxes=1;maxValue=(int)VECTOR(llabel)[i]; maxIndex=i; }
	      { maxes=1; strcpy(maxlabel,tmplabel); maxIndex=i; }
	}
     
	/* Assign label of the maxes and set their llabels to -1 */ 
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
//	    if(maxValue==(int)VECTOR(llabel)[i]) {
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) {
 		VECTOR(*label)[i] = n-maxes+1;
	//	VECTOR(llabel)[i] = -1;
   		igraph_strvector_set(&llabel,i,"\0");
                printf("\nn=%d,maxes=%d, v=%d, label(v)=%d, maxIndex=%d, maxlabel=%s",n-maxes+1,maxes,i,(int)VECTOR(*label)[i],i,maxlabel);
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
//	  	   if((int)VECTOR(llabel)[u] >= 0)  /* If not labelled */
	    	   char *tmplabel, newlabel[100];
 	    	   igraph_strvector_get(&llabel,u,&tmplabel);
		   if(strcmp(tmplabel,"\0")>0) {
		      sprintf(newlabel,"%s,%3d",tmplabel,(int)VECTOR(*label)[i]);	
 	    	      igraph_strvector_set(&llabel,u,newlabel);
		    //  VECTOR(llabel)[u]+= (int)VECTOR(*label)[i] ;
		   }
       		}
       		igraph_vector_destroy(&ni);
	     }
	}
/*     printf("\n  Labels are: ");
     for(int i=0;i<igraph_vector_size(label);i++)  
	printf("(%d):%d",i,(int)VECTOR(*label)[i]);
     printf("\n  LLabels are: ");
     for(int i=0;i<igraph_vector_size(&llabel);i++)  
	printf("(%d):%d",i,(int)VECTOR(llabel)[i]); */
//       v = maxIndex; 
   }
   
     for(int i=0;i<igraph_vector_size(label);i++) { 
	printf("\nLabel of (%d): %d",i,(int)VECTOR(*label)[i]);
	if((int)VECTOR(*label)[i]==1)
	  igraph_vector_insert(X,0,i);
	}
//     igraph_vector_destroy(&llabel);
       igraph_strvector_destroy(&llabel);
}

void isATFree(igraph_t *g, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1) {

  igraph_vector_t left,current, right,fnn;
  *isatfree=1;

  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check */
  for(int j=0; j<loc[l-1];j++)
   {
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      cutNeighbors(g,j,loc,&left,&current,&right);
      forwardNonNeighbors(g,j,loc,l,&fnn);

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy og g */   
      *isatfree = *isatfree & isDominSatisfied(g,j,loc,&fnn,g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
   }
}

void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text) {

  int scale=1.0;
  /* Find max size of a level */
  int s=loc[0];
  for(int i=1;i<l;i++)
    if(s>loc[i]-loc[i-1]) s = loc[i]-loc[i-1];

  /* Set the positions in .dot file */
  double off=0.1;
  for(int i=0;i<l;i++) {
    int o = (i==0)?0:loc[i-1];
    for(int j=o;j<loc[i];j++) {
  char spos[8];
  int low = (s-(loc[i]-o))/2;
 // SETVAS(&g, "fixedsize",j,"true");
  sprintf(spos,"%f,%f!",i*scale-((j-o)%2)*off,(j-o+low)*scale);
  SETVAS(g, "pos",j,spos);
  SETVAS(g, "shape",j,"point");
  SETVAN(g, "fontsize",j,6);
  SETVAN(g, "xlabel",j,j);
}
}

  /* Write g to the file specified in stdin */ 
  FILE *fp = fopen(filename,"w");
  SETGAS(g, "label", text);
  SETGAS(g, "labelloc", "bottom");
  igraph_write_graph_dot(g, fp);
  fclose(fp);
}
