/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 *
 * */

#include <igraph.h>
#include <string.h>
#include "igraph_atfree.h"

void isATFree(igraph_t *g, char *gname, int depth, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1);
void exportToDot(igraph_t *g, int *loc, int l, char *filename, char *text, igraph_vector_t *map, igraph_bool_t inverse);
void LBFS(igraph_t *g, igraph_vector_t *Y, igraph_vector_t *X, igraph_vector_t *label, igraph_vector_t *map);
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map);
igraph_bool_t processForAT(igraph_t *g, char *gname, int depth, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *g1);

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
	exit(1);
   }

  char fileName[40];
  sprintf(fileName,"%s.dot.edg",argv[1]);
  FILE *fp = fopen(fileName,"r");
  if(fp == NULL) {
     printf("\nFile %s not found",argv[1]); 
     exit(1);
   }
  igraph_read_graph_edgelist(&g,fp,0,0);

/* 
 int n = igraph_vcount(&g);
 int m = igraph_ecount(&g);
// printf("\nNumber of vertices=%d",n);
// printf("\nNumber of Edges=%d",m);
*/
 igraph_vector_t Y;
 igraph_vector_init(&Y,0);
/* VECTOR(Y)[0] = rand()%n;
*/
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
/**/
  /* Randomly assign values in loc, just for printing */
//  int loc[dia+1], s = n/(dia+1);
//  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
//  loc[dia]=n;

  /* Calculate dominating pair set */
//  igraph_vector_t X,Y,label1,label2,map1,map2;
    igraph_vector_t X;
/*  igraph_vector_t X,label1,label2,map1,map2;
  igraph_vector_t invmap2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
  igraph_vector_init(&map2,n);
  igraph_vector_init(&invmap2,n);
  igraph_vector_init(&X,0); */
//  igraph_vector_init(&Y,1);
//  VECTOR(Y)[0] = rand()%n;
  
  char sspos[680];
  char sname[30];
/*
  sprintf(sname,"%s%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s",sname);
  exportToDot(&g,loc,dia+1,sname,sspos,NULL,0);
  LBFS(&g,&Y,&X,&label1,&map1);
  printf("\n Y="); igraph_vector_print(&Y);
  printf(" X="); igraph_vector_print(&X);

  char x[100], y[100];
  for(int i=0;i<igraph_vector_size(&X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(X)[i]);
  }

  for(int i=0;i<igraph_vector_size(&Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(Y)[i]);
  }

  igraph_vector_null(&map1);
  OrderGrid(&g,loc,dia+1,&X,&map1);
  printf("\n Loc=");
  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  sprintf(sname,"lbfs1-%s%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s,Y=[%s] , X=[%s]  ",sname,y,x);
  exportToDot(&g,loc,dia+1,sname,sspos,&map1,0);

  igraph_vector_clear(&Y);
  LBFS(&g,&X,&Y,&label2,&map2);
  printf("\n X="); igraph_vector_print(&X);
  printf(" Y="); igraph_vector_print(&Y);

  for(int i=0;i<igraph_vector_size(&X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(X)[i]);
  }

  for(int i=0;i<igraph_vector_size(&Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(Y)[i]);
  }

  igraph_vector_null(&map2);
  OrderGrid(&g,loc,dia+1,&Y,&map2);
  printf("\n Loc=");
  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  //printf("\nmap1="); igraph_vector_print(&map1);
  //printf("\nmap2="); igraph_vector_print(&map2);
*/
  /* We check AT-Free congition before printing */
  // Map the graph to gmap 
/*  igraph_t gmap; 
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
//   printf("\nmap2="); igraph_vector_print(&map2);
//   printf("\ninvmap2="); igraph_vector_print(&invmap2);
*/
 /* Copy the graph to gmap1, check ATFree condition
   *  and make it AT-Free if it is not already */
/*  igraph_t gmap1;
  igraph_copy(&gmap1,&gmap);
  igraph_bool_t isatfree;
  isATFree(&gmap,loc,dia+1,&isatfree,&gmap1);
*/
  /* Print the graph g with ATFree result */
/*  sprintf(sname,"lbfs2-%s%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(&g,loc,dia+1,sname,sspos,&map2,0);
*/
  /* Export mapped versions */
/*  sprintf(sname,"maplbfs2-%s%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s (mapped),X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,NULL,0);

  sprintf(sname,"remaplbfs2-%s%d",argv[1],iter);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,&map2,1);

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");
*/
  char gname[100];
  int loc[dia+1];
  igraph_t gmap1;
  igraph_vector_t map2;
  sprintf(gname,"%s%d",argv[1],iter);
  igraph_bool_t isatfree = processForAT(&g,gname,0,loc,&X,&Y,&map2,&gmap1);

  if(isatfree) printf("\nG is AT-Free");
  else printf("\nG is not AT-Free");

  /* Finally run the same test on vertices of gmap1
   * to make sure it is AT-Free */
  igraph_t gmap2;
  igraph_copy(&gmap2,&gmap1);
  int isatfree1=1;
  isATFree(&gmap1,gname,0,loc,dia+1,&isatfree1,&gmap2);

  if(isatfree1) printf("\nG' is AT-Free");
  else printf("\nG' is not AT-Free");
 
  /* Write gmap1 to <at><file specified in stdin> */
//  sprintf(sname,"at-%s",argv[1]);  
  igraph_bool_t oiso=1;isHamiltonian(&gmap1,loc,dia+1);
//  if(oiso) 
    sprintf(sname,"hat-%s%d",argv[1],iter);  
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

  igraph_vs_t vs;
  igraph_vs_vector(&vs, &X);
  printf("\nIter%d :Deleting vertices.. ",iter);igraph_vector_print(&X);
  igraph_delete_vertices(&g,vs);
  igraph_vector_t Ytemp;
  igraph_vector_copy(&Ytemp,&Y);
  for(int i=0;i<igraph_vector_size(&X);i++)
      for(int j=0;j<igraph_vector_size(&Ytemp);j++)
         if((int)VECTOR(X)[i] < (int)VECTOR(Ytemp)[j])  VECTOR(Y)[j] = VECTOR(Y)[j] -1;
  igraph_vector_destroy(&Ytemp);

  sprintf(sspos,"%s%d.dot.edg",argv[1],iter);  
  fp = fopen(sspos,"w");
  igraph_write_graph_edgelist(&g, fp);
  fclose(fp);
//  igraph_vector_destroy(&label1);
//  igraph_vector_destroy(&label2);
//  igraph_vector_destroy(&map1);
  igraph_vector_destroy(&map2);
  igraph_vector_destroy(&X);
  igraph_destroy(&gmap1);
  igraph_destroy(&gmap2);
  igraph_vs_destroy(&vs);
 }

//  igraph_vector_destroy(&label1);
//  igraph_vector_destroy(&label2);
//  igraph_vector_destroy(&map1);
//  igraph_vector_destroy(&map2);
//  igraph_vector_destroy(&X);
  igraph_vector_destroy(&Y);
  igraph_destroy(&g);
  return 0;
}

igraph_bool_t processForAT(igraph_t *g, char *gname, int depth, int *loc, igraph_vector_t *X, igraph_vector_t *Y, igraph_vector_t *map2, igraph_t *gmap1)
{
  int n = igraph_vcount(g);
  int m = igraph_ecount(g);

  igraph_empty(gmap1, n, IGRAPH_UNDIRECTED);
  igraph_vector_init(map2,n);
  igraph_vector_init(X,0);

  /* No point in moving ahead if there are no vertices or no edges */
  if(n==0) return 1;

  if(m==0) {
    for(int i=0;i<n;i++)
 	igraph_vector_insert(X,0,i);
    return 1;
   }

  if(igraph_vector_size(Y)==0) {
//  igraph_vector_t Y;
  igraph_vector_init(Y,1);
  VECTOR(*Y)[0] = rand()%n;
  }

  char depthstring[20] = "\0";
  for(int i=0;i<depth;i++) sprintf(depthstring,"%s--",depthstring);
  printf("\n|%s[%s]Number of vertices=%d",depthstring,gname,n);
  printf(", Edges=%d",m);
  igraph_integer_t result,dia;
  igraph_vertex_connectivity(g,&result,1);
  printf(" Vertex connectivity=%d",result);
  igraph_diameter(g,&dia,0,0,0,IGRAPH_UNDIRECTED,1);
  printf(", Diameter=%d",dia);

  /* Randomly assign values in loc, just for printing */
//  int loc[dia+1], 
  int s = n/(dia+1);
  for(int i=0;i<dia;i++) loc[i]=(i+1)*s;
  loc[dia]=n;

  /* Calculate dominating pair set */
//  igraph_vector_t X,Y,label1,label2,map1,map2;
  igraph_vector_t label1,label2,map1;//,map2;
  igraph_vector_t invmap2;
  igraph_vector_init(&label1,n);
  igraph_vector_init(&label2,n);
  igraph_vector_init(&map1,n);
//  igraph_vector_init(map2,n);
  igraph_vector_init(&invmap2,n);
//  igraph_vector_init(X,0);
//  igraph_vector_init(&Y,1);
//  VECTOR(Y)[0] = rand()%n;
  
  char sspos[680];
  char sname[200];

  sprintf(sname,"%s",gname);  
  sprintf(sspos,"Graph=%s",sname);
  exportToDot(g,loc,dia+1,sname,sspos,NULL,0);
  LBFS(g,Y,X,&label1,&map1);
//  printf("\n Y="); igraph_vector_print(Y);
//  printf(" X="); igraph_vector_print(X);

  char x[100], y[100];
  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(&map1);
  OrderGrid(g,loc,dia+1,X,&map1);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  sprintf(sname,"lbfs1-%s",gname);  
  sprintf(sspos,"Graph=%s,Y=[%s] , X=[%s]  ",sname,y,x);
//  exportToDot(g,loc,dia+1,sname,sspos,&map1,0);

  igraph_vector_clear(Y);
  LBFS(g,X,Y,&label2,map2);
  printf("\n|%sX=",depthstring); igraph_vector_print(X);
  printf("|%sY=",depthstring); igraph_vector_print(Y);

  for(int i=0;i<igraph_vector_size(X);i++) {
	if(i==0) sprintf(x,"%d",(int)VECTOR(*X)[0]);
	else  	 sprintf(x,"%s %d",x,(int)VECTOR(*X)[i]);
  }

  for(int i=0;i<igraph_vector_size(Y);i++) {
	if(i==0) sprintf(y,"%d",(int)VECTOR(*Y)[0]);
	else  	 sprintf(y,"%s %d",y,(int)VECTOR(*Y)[i]);
  }

  igraph_vector_null(map2);
  OrderGrid(g,loc,dia+1,Y,map2);
//  printf("\n Loc=");
//  for(int i=0;i<dia+1;i++) printf(" %d ",loc[i]);
  //printf("\nmap1="); igraph_vector_print(&map1);
  //printf("\nmap2="); igraph_vector_print(&map2);

  /* We check AT-Free congition before printing */
  // Map the graph to gmap 
  igraph_t gmap; 
  igraph_empty(&gmap, n, IGRAPH_UNDIRECTED);
  for(int ii=0;ii<n;ii++) {
    igraph_integer_t i = (int)VECTOR(*map2)[ii];
    VECTOR(invmap2)[i] = ii;
   }
  for(int i=0;i<m;i++) {
     igraph_integer_t from, to;
     igraph_edge(g,i,&from,&to);
     from=(int)VECTOR(invmap2)[from];
     to=(int)VECTOR(invmap2)[to]; 
     igraph_add_edge(&gmap,from,to);
    }
//   printf("\nmap2="); igraph_vector_print(&map2);
//   printf("\ninvmap2="); igraph_vector_print(&invmap2);

 /* Copy the graph to gmap1, check ATFree condition
   *  and make it AT-Free if it is not already */
//  igraph_t gmap1;
  igraph_copy(gmap1,&gmap);
  igraph_bool_t isatfree;
  isATFree(&gmap,gname,depth,loc,dia+1,&isatfree,gmap1);

  /* Print the graph g with ATFree result */
  sprintf(sname,"lbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(g,loc,dia+1,sname,sspos,map2,0);

  /* Export mapped versions */
  sprintf(sname,"maplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s (mapped),X=[],Y=[],ATFree=%d ",sname,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,NULL,0);

/*
  sprintf(sname,"remaplbfs2-%s",gname);  
  sprintf(sspos,"Graph=%s,X=[%s],Y=[%s],ATFree=%d ",sname,x,y,isatfree);
  exportToDot(&gmap,loc,dia+1,sname,sspos,map2,1); */

//  if(isatfree) printf("\nG is AT-Free");
//  else printf("\nG is not AT-Free");
 
  igraph_vector_destroy(&label1);
  igraph_vector_destroy(&label2);
  igraph_vector_destroy(&map1);
//  igraph_destroy(&gmap1);
  igraph_destroy(&gmap);
  return isatfree;
}

/* Size or l = number of levels */
void OrderGrid(igraph_t *g, int *loc, int l, igraph_vector_t *X, igraph_vector_t *map) 
{
	int mapIndex = 0;
	igraph_vector_t vlevel;
	igraph_vector_copy(&vlevel,X);

	int n = igraph_vcount(g);
 	igraph_bool_t visited[n];	
	for(int i=0;i<n;i++) visited[i]=0;

//	printf("\n");
	for(int i=0;i<l;i++)
	 {
	   igraph_vs_t vs;
	   igraph_vs_vector(&vs, &vlevel);

	   igraph_vector_ptr_t res;
	   igraph_vector_ptr_init(&res,0);
//	   printf("i=%d, Vlevel=",i);igraph_vector_print(&vlevel);
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

void isATFree(igraph_t *g, char *gname, int depth, int *loc, int l, igraph_bool_t *isatfree, igraph_t *g1) {

  igraph_vector_t left,current, right,fnn, cnn;
  *isatfree=1;
  
  /* Every graph with 5 vertices or less is AT-Free */
  if(igraph_vcount(g)<=5) return;
 
  /* For every vertex j, find the non-neighbors in adjacent levels 
   * and apply the localATFree check 
   *
   * Also get the neighbors in the current levels and 
   * apply the level check */
  for(int i=0; i<l;i++) {
    int j= (i==0)?0:loc[i-1];
    igraph_vector_t tlevel;
    igraph_vector_init(&tlevel,0);
    for(; j<loc[i];j++)
    {
      igraph_vector_insert(&tlevel,0,j);
      igraph_vector_init(&left,1);
      igraph_vector_init(&current,1);
      igraph_vector_init(&right,1);
      igraph_vector_init(&fnn,1);
      igraph_vector_init(&cnn,1);
      cutNeighbors(g,j,loc,&left,&current,&right);
      forwardNonNeighbors(g,j,loc,l,&fnn);
      currentNonNeighbors(g,j,loc,l,&cnn);

    /*  For vertex j, is the local condition true ? , If not add edges
     *  in graph g1, which was originally a copy of g 
     *  This is for AT (x,y,z) in three different levels */   
      *isatfree = *isatfree & isDominSatisfied(g,j,loc,&fnn,g1);
 
     /* 2L-AT */ 
      *isatfree = *isatfree & isAdjSatisfied(g,j,loc,&cnn,g1);

  igraph_vector_destroy(&left);
  igraph_vector_destroy(&current);
  igraph_vector_destroy(&right);
  igraph_vector_destroy(&fnn);
  igraph_vector_destroy(&cnn);
 }
 
    /* 1L-AT For every level, extract the subgraph of the level and run the 
     * complete process again 
     * This is for AT (x,y,z) with all in the same level without using
     * any vertices or paths from any other levels */
      igraph_t subg, gmap1;
      igraph_vs_t vs;
      igraph_vector_t map2,X,Y;
      int subdia;
      char ggname[100];
      igraph_vector_init(&X,0);
      igraph_vector_init(&map2,0);
      igraph_vector_init(&Y,0);
      igraph_empty(&gmap1, 0, IGRAPH_UNDIRECTED);
      sprintf(ggname,"%s_subg%d",gname,i);
      igraph_vs_vector(&vs, &tlevel);
      igraph_vector_init(&map2,igraph_vector_size(&tlevel));
      igraph_induced_subgraph(g,&subg,vs,IGRAPH_SUBGRAPH_AUTO);
      igraph_diameter(&subg,&subdia,0,0,0,IGRAPH_UNDIRECTED,1);
//      printf("\n[%s]L%d: Subgraph Diameter=%d, Vertices=%d, Edges=%d",ggname,i,subdia,igraph_vcount(&subg),igraph_ecount(&subg));
      int loc[subdia+1];
      igraph_bool_t atfree = processForAT(&subg,ggname,depth+1,loc,&X,&Y,&map2,&gmap1);
      char depthstring[20]="\0";
      for(int k=0;k<depth;k++) sprintf(depthstring,"%s--",depthstring); 
      if(!atfree) printf("\n|%s[%s]L%d has an AT.",depthstring,ggname,i);
      *isatfree = *isatfree & atfree;
      igraph_vector_destroy(&tlevel);
      igraph_vs_destroy(&vs);
      igraph_vector_destroy(&X);
      igraph_vector_destroy(&Y);
      igraph_vector_destroy(&map2);
      igraph_destroy(&gmap1);
      igraph_destroy(&subg);
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
  char fName[100];
  sprintf(fName,"%s.dot",filename);
  FILE *fp = fopen(fName,"w");
  SETGAS(g, "label", text);
  SETGAS(g, "labelloc", "bottom");
  igraph_write_graph_dot(g, fp);
  fclose(fp);
}
