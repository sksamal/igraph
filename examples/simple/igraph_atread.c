/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include <string.h>
#include "igraph_atfree.h"

void isATFree(igraph_t *g, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1);
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text, igraph_vector_t *map, igraph_bool_t inverse);
void LBFS(igraph_t *g, igraph_vector_t *Y, igraph_vector_t *X, igraph_vector_t *label, igraph_vector_t *map);
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map);

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

  /* Randomly assign values in loc, just for printing */
  int loc[dia+1], s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  /* Calculate dominating pair set */
  igraph_vector_t X,Y,label1,label2,map1,map2;
  igraph_vector_t invmap2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
  igraph_vector_init(&map2,n);
  igraph_vector_init(&invmap2,n);
  igraph_vector_init(&X,0);
  igraph_vector_init(&Y,1);
  VECTOR(Y)[0] = rand()%n;
  
  char sspos[680];
  char sname[30];
  sprintf(sname,"%s",argv[1]);  
  sprintf(sspos,"Graph=%s",sname);
  exportToDot(&g,loc,dia+1,sname,sspos,NULL,0);
  LBFS(&g,&Y,&X,&label1,&map1);
  printf("\n Y="); igraph_vector_print(&Y);
  printf(" X="); igraph_vector_print(&X);
  igraph_vector_null(&map1);
  OrderGrid(&g,loc,dia+1,&X,&map1);
  printf("\n Loc=");
  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  sprintf(sname,"lbfs1-%s",argv[1]);  
  sprintf(sspos,"Graph=%s,Y=[] , X=[]  ",sname,0);
  exportToDot(&g,loc,dia+1,sname,sspos,&map1,0);
  igraph_vector_clear(&Y);
  LBFS(&g,&X,&Y,&label2,&map2);
  printf("\n X="); igraph_vector_print(&X);
  printf(" Y="); igraph_vector_print(&Y);
  igraph_vector_null(&map2);
  OrderGrid(&g,loc,dia+1,&Y,&map2);
  printf("\n Loc=");
  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  //printf("\nmap1="); igraph_vector_print(&map1);
  //printf("\nmap2="); igraph_vector_print(&map2);

  /* We check AT-Free congition before printing */
  // Map the graph to gmap 
  igraph_t gmap; 
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(&g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }
   printf("\nmap2="); igraph_vector_print(&map2);
   printf("\ninvmap2="); igraph_vector_print(&invmap2);

 /* Copy the graph to gmap1, check ATFree condition
   *  and make it AT-Free if it is not already */
  igraph_t gmap1;
  igraph_copy(&gmap1,&gmap);

  /* Copy the graph to g1, check ATFree condition
   *  and make it AT-Free if it is not already */
  igraph_bool_t isatfree;
  isATFree(&gmap,loc,dia+1,&isatfree,&gmap1);

  /* Print the graph g with ATFree result */
  sprintf(sname,"lbfs2-%s",argv[1]);  
  sprintf(sspos,"Graph=%s,X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&g,loc,dia+1,sname,sspos,&map2,0);

  sprintf(sname,"maplbfs2-%s",argv[1]);  
  sprintf(sspos,"Graph=%s (mapped),X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,NULL,0);

  sprintf(sname,"remaplbfs2-%s",argv[1]);  
  sprintf(sspos,"Graph=%s,X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,&map2,1);

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  /* Finally run the same test on vertices of gmap1
   * to make sure it is AT-Free */
  igraph_t gmap2;
  igraph_copy(&gmap2,&gmap1);
  int isatfree1=1;
  isATFree(&gmap1,loc,dia+1,&isatfree1,&gmap2);

  if(isatfree1) printf("\nG' is AT-Free");
  else printf("\nG' is not AT-Free");
 
  /* Write gmap1 to <at><file specified in stdin> */
  sprintf(sname,"at-%s",argv[1]);  
  igraph_bool_t oiso=isHamiltonian(&gmap1,loc,dia+1);
  if(oiso) sprintf(sname,"hat-%s",argv[1]);  
  sprintf(sspos,"Graph=%s, ATFree=%d,OurAlgo=%d",sname,isatfree1,oiso);
  exportToDot(&gmap1,loc,dia+1,sname,sspos,&map2,1);

  /* Run the LAD and VFS isomorphism algorithms 
   *   present in igraph and if successful rewrite gmap1*/
  char path1[300], path2[300];
  path1[0]='\0';
  path2[0]='\0';
  igraph_bool_t iso1=0, iso2=0;
  isHamUsingLAD(&gmap1,&iso1, path1);
  isHamUsingVF2(&gmap1,&iso2, path2);
  sprintf(sspos,"Graph=%s, AT-Free=%d,OurAlgo=%d,\nLAD=%d [%s],\nVF2=%d [%s] ",sname,isatfree1,oiso,iso1, path1,iso2, path2);
  exportToDot(&gmap1,loc,dia+1,sname,sspos,&map2,1);
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
  igraph_vector_destroy(&map2);
  igraph_vector_destroy(&X);
  igraph_vector_destroy(&Y);
  igraph_destroy(&g);
  igraph_destroy(&gmap1);
  igraph_destroy(&gmap2);
  return 0;
}

/* Size or l = number of levels */
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map) {

	int mapIndex = 0;
	igraph_vector_t vlevel;
	igraph_vector_copy(&vlevel,X);

	int n = igraph_vcount(g);
 	igraph_bool_t visited[n];	
	for(int i=0;i<n;i++) visited[i]=0;

	printf("\n");
	for(int i=0;i<l;i++)
	 {
	   igraph_vs_t vs;
	   igraph_vs_vector(&vs, &vlevel);

	   igraph_vector_ptr_t res;
	   igraph_vector_ptr_init(&res,0);
	   printf("i=%d, Vlevel=",i);igraph_vector_print(&vlevel);
	   for(int j=0;j<igraph_vector_size(&vlevel);j++)
	      {
		int v = VECTOR(vlevel)[j];
		VECTOR(*map)[mapIndex++] = v;
		visited[v] = 1;
	      }
	   igraph_integer_t vssize;
	   igraph_vs_size(g,&vs,&vssize);
	   loc[i] = vssize + ((i==0)?0:loc[i-1]);
	   igraph_neighborhood(g,&res,vs,1,IGRAPH_ALL,0);

	   /* Reset vlevel to the vertices in current level 
 	    */
	   igraph_vector_clear(&vlevel);
	   for(int j=0;j<igraph_vector_ptr_size(&res);j++) {
		igraph_vector_t *v = VECTOR(res)[j];
		for(int k=0;k<igraph_vector_size(v);k++) {
		  if(!igraph_vector_contains(&vlevel,VECTOR(*v)[k]) && !visited[((int)VECTOR(*v)[k])])
			igraph_vector_insert(&vlevel,0,VECTOR(*v)[k]);
		  }
//		printf("\ni=%d,j=%d: ",i,j);
//		igraph_vector_print(v);
		igraph_vector_destroy(v);
		igraph_free(v);
	   }

//	   printf(" N(Vlevel)=");igraph_vector_print(&vlevel);
	   igraph_vs_destroy(&vs);
	   igraph_vector_ptr_destroy(&res);
	 }
	igraph_vector_destroy(&vlevel);
}


void LBFS(igraph_t *g, igraph_vector_t *vstart, igraph_vector_t *X, igraph_vector_t *label, 
	igraph_vector_t *map) {

   /* Starting label */ 
   int n = igraph_vcount(g);
   igraph_strvector_t llabel;
   igraph_strvector_init(&llabel, n);
   for(int i=0;i<igraph_strvector_size(&llabel);i++) {
      char *tmpstr="000";
      igraph_strvector_set(&llabel,i,tmpstr);
   }

   int m=0; 
   for(int i=0;i<igraph_vector_size(vstart);i++) {
   	char strlabel[100];
	if(n+1<10)	  sprintf(strlabel,"00%d",n+1);
	else if(n+1<100)  sprintf(strlabel,"0%d",n+1);
	else if(n+1<1000) sprintf(strlabel,"%d",n+1);
   	int v = VECTOR(*vstart)[i];
   	/* Set lexlabel of v as some positive number */
   	igraph_strvector_set(&llabel,v,strlabel);
//	printf("v=%d",v);
     } 

  /*   printf("\n LLabels are: ");
     for(int i=0;i<igraph_strvector_size(&llabel);i++) { 
	char *tmplabel;
 	igraph_strvector_get(&llabel,i,&tmplabel);
	printf("(%d):%s",i,tmplabel); 
     } */
   while(n>0) {

	/* Find the maximum and its index */
	int maxes = 1, maxValue=0, maxIndex=0;
	char maxlabel[100];
	maxlabel[0] = '\0';
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) maxes++;
	    if(strcmp(maxlabel,tmplabel)<0)
	      { maxes=1; strcpy(maxlabel,tmplabel); maxIndex=i; }
	}
     
	/* Assign label of the maxes and set their llabels to -1 */ 
	for(int i=0;i<igraph_strvector_size(&llabel);i++) {
//	    if(maxValue==(int)VECTOR(llabel)[i]) {
	    char *tmplabel;
 	    igraph_strvector_get(&llabel,i,&tmplabel);
	    if(strcmp(maxlabel,tmplabel)==0) {
 		VECTOR(*label)[i] = n-maxes+1;
		VECTOR(*map)[m++] = i;
   		igraph_strvector_set(&llabel,i,"\0");
   //             printf("\nn=%d,maxes=%d, v=%d, label(v)=%d, maxIndex=%d, maxlabel=%s",n-maxes+1,maxes,i,(int)VECTOR(*label)[i],i,maxlabel);
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
//	  	   /* Label only if not labelled */
	    	   char *tmplabel, newlabel[100];
 	    	   igraph_strvector_get(&llabel,u,&tmplabel);
		   if(strcmp(tmplabel,"\0")>0) {
			int v = (int)VECTOR(*label)[i];
			if(v<10)	sprintf(newlabel,"%s,00%d",tmplabel,v);
			else if(v<100)  sprintf(newlabel,"%s,0%d",tmplabel,v);
			else if(v<1000) sprintf(newlabel,"%s,%d",tmplabel,v);
 	    	      igraph_strvector_set(&llabel,u,newlabel);
		   }
       		}
       		igraph_vector_destroy(&ni);
	     }
	}
/*     printf("\n  Labels are: ");
     for(int i=0;i<igraph_vector_size(label);i++)  
	printf("(%d):%d",i,(int)VECTOR(*label)[i]);
     printf("\n  Map is: ");
     for(int i=0;i<igraph_vector_size(map);i++)  
	printf("(%d):%d",i,(int)VECTOR(*map)[i]);
     printf("\n  LLabels are: ");
     for(int i=0;i<igraph_strvector_size(&llabel);i++) { 
	char *tmplabel;
 	igraph_strvector_get(&llabel,i,&tmplabel);
	printf("(%d):%s",i,tmplabel); 
     } */
//       v = maxIndex; 
   }
   
     for(int i=0;i<igraph_vector_size(label);i++) { 
//	printf("\nLabel of (%d): %d, Map : %d",i,(int)VECTOR(*label)[i],(int)VECTOR(*map)[i]);
	if((int)VECTOR(*label)[i]==1)
	  igraph_vector_insert(X,0,i);
	} 
       igraph_strvector_destroy(&llabel);
}

void isATFree(igraph_t *g, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1) {

  igraph_vector_t left,current, right,fnn, cnn;
  *isatfree=1;
 
  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check 
   *
   * Also get the neighbors in the current levels and 
   * apply the level check */
  for(int j=0; j<loc[l-1];j++)
   {
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      igraph_vector_init(&cnn,1);
      cutNeighbors(g,j,loc,&left,&current,&right);
      forwardNonNeighbors(g,j,loc,l,&fnn);
      currentNonNeighbors(g,j,loc,l,&cnn);

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy of g */   
      *isatfree = *isatfree & isDominSatisfied(g,j,loc,&fnn,g1);

    /*  For vertex j, is the level condition true ? , If not add edges
     *  in graph g1, which was originally a copy of g */   
      *isatfree = *isatfree & isLevelSatisfied(g,j,loc,&cnn,g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
  igraph_vector_destroy(&cnn);
   }
}

/* Note: l= number of levels */
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text, igraph_vector_t *map, igraph_bool_t inverse ) {

  double scale=1.0;
  /* Find max size of a level */
  int s=loc[0];
  for(int i=1;i<l;i++)
    if(s<loc[i]-loc[i-1]) s = loc[i]-loc[i-1];

  /* Set the positions in .dot file */
  double off=0.1;
  int v;
  for(int i=0;i<l;i++) {
    int o = (i==0)?0:loc[i-1];
    for(int j=o;j<loc[i];j++) {
  	if(!inverse && map !=NULL)  v = (int)VECTOR(*map)[j];
	else 		v = j;
  	char spos[8];
  	int low = (s-(loc[i]-o))/2;
//	printf("\n\tj=%d, low=%d, loc[i]=%d, o=%d, scale=%f",j,low,loc[i],o,scale);
 	// SETVAS(&g, "fixedsize",j,"true");
  	sprintf(spos,"%f,%f!",i*scale-((j-o)%2)*off,(j-o+low)*scale);
 	SETVAS(g, "pos",v,spos);
  	SETVAS(g, "shape",v,"point");
  	SETVAN(g, "fontsize",v,6);
	if(inverse && map !=NULL) SETVAN(g, "xlabel",v,(int)VECTOR(*map)[v]);
  	else 	    SETVAN(g, "xlabel",v,v);
     }
}

  /* Write g to the file specified in stdin */ 
  FILE *fp = fopen(filename,"w");
  SETGAS(g, "label", text);
  SETGAS(g, "labelloc", "bottom");
  igraph_write_graph_dot(g, fp);
  fclose(fp);
}
