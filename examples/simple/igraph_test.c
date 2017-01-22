
#include <igraph.h>
#include "igraph_atfree.h"
#include <time.h>

int main() {

  Path p1, p2;
  int vs1[] = {2, 4, 2, 1, 12, 32}; 
  int vs2[] = {11,7};

  igraph_path_init(&p1,0);
  igraph_path_add(&p1,vs1,6);
  char pathstr[100];
  igraph_path_str(&p1,pathstr);
  printf("\nP1= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p1));

  igraph_path_add(&p1,vs2,2);
  igraph_path_str(&p1,pathstr);
  printf("\nP1= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p1));

  igraph_path_addv(&p1,10);
  igraph_path_str(&p1,pathstr);
  printf("\nP1= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p1));

  int vs3[] = { 8,9 };
  igraph_path_init(&p2,0);
  igraph_path_add(&p2,vs3,2);
  igraph_path_str(&p2,pathstr);
  printf("\nP2= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p2));

  printf(" Merge result= %d", igraph_path_merge(&p1,&p2));
  igraph_path_str(&p1,pathstr);
  printf("\nP1= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p1));
  igraph_path_clear(&p2);

  int vs4[] = {10, 7,8,9};
  igraph_path_clear(&p2);
  igraph_path_str(&p2,pathstr);
  printf("\nP2= %s",pathstr);
  igraph_path_add(&p2,vs4,3);
  igraph_path_str(&p2,pathstr);
  printf("\nP2= %s",pathstr);
  printf(" Merge2 = %d", igraph_path_merge(&p1,&p2));
  igraph_path_str(&p1,pathstr);
  printf("\nP1= %s",pathstr);
  printf(" Size= %d",igraph_path_size(&p1));


  /* Testing for Paths */
  Paths ps1,ps2; 
  igraph_paths_init(&ps1,0);
  igraph_paths_add(&ps1,&p2);
  igraph_paths_add(&ps1,&p1);
  igraph_paths_str(&ps1,pathstr);
  printf("\nPS1= %s",pathstr);
  printf(" Size= %d",igraph_paths_size(&ps1));

  Path p3, p4, p5;
  int vs5[] = {1,2,3,4,5,6,7,8,9,10};
  int vs6[] = {0,19,7,4,3,2,1,1,1,1,1,1};
  int vs7[] = {12,14,0,19,7,4,3,2,12,91,18,13,15,10};
  igraph_path_init(&p3,0);
  igraph_path_init(&p4,0);
  igraph_path_init(&p5,0);
  igraph_path_add(&p3,vs5,10);
  igraph_path_add(&p4,vs6,12);
  igraph_path_add(&p5,vs7,14);
  igraph_paths_add(&ps1,&p3);
  igraph_paths_add(&ps1,&p4);
  igraph_paths_add(&ps1,&p5);
  igraph_paths_str(&ps1,pathstr);
  printf("\nPS1= %s",pathstr);
  printf(" Size= %d",igraph_paths_size(&ps1));
  igraph_path_addv(&p2,2);
  igraph_paths_str(&ps1,pathstr);
  printf("\nPS1= %s",pathstr);
  printf(" Size= %d",igraph_paths_size(&ps1));

  igraph_paths_sort(&ps1);
  igraph_paths_str(&ps1,pathstr);
  printf("\nPS1S= %s",pathstr);
  printf(" Size= %d",igraph_paths_size(&ps1));
  
  igraph_paths_break(&ps1,4);
  igraph_paths_str(&ps1,pathstr);
  printf("\nPS1S= %s",pathstr);
  printf(" Size= %d",igraph_paths_size(&ps1));

  igraph_path_destroy(&p2);
  igraph_path_destroy(&p1);
//  igraph_paths_destroy(&ps1);
  return 1;

}
