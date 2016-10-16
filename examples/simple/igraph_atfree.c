/* -*- mode: C -*-  */
/* 
   IGraph library.
   Copyright (C) 2006-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard st, Cambridge MA, 02139 USA
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA 
   02110-1301 USA

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
  int l=rand()%7+3,s=rand()%7+3;  
  int loc[l];    // store start
  double scale=0.4;
  int count=0;
  int conns=4;


  for(int i=0;i<l;i++) {
	count+= (rand()%(s-2)+2);
	loc[i] = count;
//	printf("%d ",loc[i]);
  }	
 
  /* empty directed graph, zero vertices */
  igraph_empty(&g, count, IGRAPH_UNDIRECTED);

  for(int i=0;i<l;i++) {
    int p = (i<=1)?0:loc[i-2];
    int c = (i>0)?loc[i-1]:0;
    int n = loc[i];
    for (int j=c;j<n;j++)  { /* Vertices in level i */ 
	int rb1=-1,rb2=-1,rf1=-1,rf2=-1 ;

     int cs = conns;
     while(1) {
    	if(i!=0) { 
		do { rb1 = rand()%(loc[i-1]-p); } while(rb2==rb1);
		igraph_add_edge(&g,j,p+rb1);
		}
	else 	{
		do { rf1 = rand()%(loc[i+1]-n); } while(rf2==rf1);
		igraph_add_edge(&g,j,n+rf1); 
		}
	if(!--cs>0) break;
    	if(i!=l-1) {
		do { rf2 = rand()%(loc[i+1]-n); } while(rf2==rf1);
		igraph_add_edge(&g,j,n+rf2);
		}
	else 	{
		do { rb2 = rand()%(loc[i-1]-p); } while(rb2==rb1);
		igraph_add_edge(&g,j,p+rb2);
		}
	if(!--cs>0) break;
//    	if(i!=0) igraph_add_edge(&g,j,p+rand()%(loc[i-1]-p));
//	else igraph_add_edge(&g,j,n+rand()%(loc[i+1]-n)); 
//    	if(i!=l-1) igraph_add_edge(&g,j,n+rand()%(loc[i+1]-n));
//	else igraph_add_edge(&g,j,p+rand()%(loc[i-1]-p));

	int t=rand()%(3*(n-c));
	if(t < (n-c))
	  igraph_add_edge(&g,j,c+t);
	if(!--cs>0) break;
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
