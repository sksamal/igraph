
#include <igraph.h>
#include <string.h>

typedef igraph_vector_t Path;
typedef igraph_vector_ptr_t Paths;


void igraph_paths_init(Paths *ps, int size);
void igraph_paths_destroy(Paths *ps);
void igraph_paths_add(Paths *ps, Path *p);
int igraph_paths_size(Paths *ps);
void igraph_paths_sort(Paths *ps);
int igraph_paths_break(Paths *ps, int more);
void igraph_paths_merge(Paths *ps, Paths *ps1);
void igraph_paths_str(Paths *ps, char *pathsstr);
void igraph_paths_print(Paths *ps);
void igraph_paths_clear(Paths *ps);
void igraph_paths_remove(Paths *ps, int index);

void igraph_path_init(Path *p, int size);
void igraph_path_destroy(Path *p);
void igraph_path_add(Path *p, int *vs, int len);
void igraph_path_addv(Path *p, int v);
int igraph_path_size(Path *p);
int igraph_path_merge(Path *p, Path *p1);
void igraph_path_str(Path *p, char *pathstr);
void igraph_path_print(Path *p);
void igraph_path_clear(Path *p);

void igraph_paths_init(Paths *ps, int size)
{ 
  igraph_vector_ptr_init(ps,size);
} 

void igraph_paths_destroy(Paths *ps)
{
  igraph_vector_ptr_destroy_all(ps);
}

void igraph_paths_add(Paths *ps, Path *p)
{
  igraph_vector_ptr_push_back(ps,p);
}

void igraph_paths_remove(Paths *ps, int index)
{
  igraph_vector_ptr_remove(ps,index);
}

int igraph_paths_size(Paths *ps)
{
  return igraph_vector_ptr_size(ps);
}

void igraph_paths_sort(Paths *ps)
{
  for(int i=0; i<igraph_vector_ptr_size(ps); i++)
   {
     Path *temp = VECTOR(*ps)[i];
     int j=i-1;
     for(; j>=0; j--)
	{
	  Path *vj = VECTOR(*ps)[j];
	  if(igraph_vector_size(vj) < igraph_vector_size(temp)) {
		VECTOR(*ps)[j+1] = VECTOR(*ps)[j]; 
		VECTOR(*ps)[j] = temp;
          }
          else break;
	}
   }
}

int igraph_paths_break(Paths *ps, int more)
{
  /* No paths to break */
  if(igraph_vector_ptr_size(ps)<1) return -1;
  
  int reqd = more;
  for(int i=0; i<igraph_vector_ptr_size(ps); i++) {
	Path *vi = VECTOR(*ps)[i]; 
	reqd = reqd - (igraph_vector_size(vi) -1);
        if (reqd <=0) break;
    }
  
  /* Not possible to break */
  if (reqd > 0)  return -1;

  reqd = more;
  int size = igraph_vector_ptr_size(ps);

  char pathstr[1000];
  for(int i=0; i<size; i++) {
	Path *vi = VECTOR(*ps)[i]; 
	int lpos = igraph_vector_size(vi) -1; 
  	while(reqd > 0 && lpos > 0) {
	  Path *p = (Path*) malloc(sizeof(Path*));
	  igraph_path_init(p,0);
	  igraph_path_addv(p,(int) VECTOR(*vi)[lpos]);
	  igraph_vector_remove(vi,lpos); 
	  lpos--; reqd--;
	  igraph_paths_add(ps,p);
	} 
	if(reqd == 0) break;
      }
   igraph_paths_sort(ps);
   return 1;
 }

void igraph_paths_merge(Paths *ps, Paths *ps1) {
   
  for(int i=0; i<igraph_vector_ptr_size(ps1); i++) {
	Path *vi = VECTOR(*ps1)[i]; 
	int lvalue = (int) VECTOR(*vi)[0];
	int rvalue = (int) VECTOR(*vi)[igraph_vector_size(vi)-1];
	int j=0;
  	for(; j<igraph_vector_ptr_size(ps); j++) {
	    Path *vj = VECTOR(*ps)[j];   /* Find if the first vertex in ps1 matches with last vertex in ps */
	    int rpos = igraph_vector_size(vj) -1;
	    if(lvalue == (int)VECTOR(*vj)[rpos] || rvalue == (int)VECTOR(*vj)[rpos]) {
		igraph_path_merge(vj,vi);
	 	break;
	    }	
	 }
	if (j== igraph_vector_ptr_size(ps))  /* Not found, need to push back the complete vector */
	    igraph_paths_add(ps,vi);
    }
   igraph_paths_sort(ps);
}

void igraph_paths_simplify(Paths *ps) {

   for(int i=0; i<igraph_paths_size(ps);i++)
    {
       Path *pi = VECTOR(*ps)[i];  
       for(int j=i+1; j<igraph_paths_size(ps);j++)
        {
          Path *pj = VECTOR(*ps)[j]; 
	  if(igraph_path_merge(pi,pj))
		igraph_path_clear(pj);
	}
    }
   igraph_paths_sort(ps);
   for(int i=0; i<igraph_paths_size(ps);i++)
    {
       Path *pi = VECTOR(*ps)[i]; 
       if(igraph_vector_size(pi)==0)
	 { igraph_paths_remove(ps,i); i--; }
    }
 }

void igraph_paths_str(Paths *ps, char *pathsstr) {
   *pathsstr='\0';
   char pathstr[100];
   for(int i=0; i<igraph_paths_size(ps);i++)
    {
       igraph_path_str((Path*)VECTOR(*ps)[i],pathstr);  
       if(i==igraph_paths_size(ps)-1)
       	  sprintf(pathsstr,"%s%s",pathsstr,pathstr);
       else
          sprintf(pathsstr,"%s%s,",pathsstr,pathstr);
    }
}

void igraph_paths_print(Paths *ps) {
    char pathsstr[400];
    igraph_paths_str(ps,pathsstr);
    printf("%s\n",pathsstr);
 }

void igraph_paths_clear(Paths *ps) {

  igraph_vector_ptr_clear(ps);

}

void igraph_path_init(Path *p, int size) {

   igraph_vector_init(p,size);

}

void igraph_path_add(Path *p, int *vs, int len) {

  for(int i=0; i<len; i++)
    igraph_vector_push_back(p,vs[i]); 

 }

void igraph_path_addv(Path *p, int v) {

   igraph_vector_push_back(p,v);

}

void igraph_path_destroy(Path *p) {

  igraph_vector_destroy(p);
}

int igraph_path_merge(Path *p, Path *p1) {

 /* Only appends p1 to p , in either direction */
 /* Check if last vertex of p is same as first vertex of p1 */ 
 int lvalue = (int)VECTOR(*p)[igraph_vector_size(p)-1];
 if(lvalue == (int)VECTOR(*p1)[0]) { 
 	igraph_vector_remove(p1,0);
 	igraph_vector_append(p,p1);
	return 1;
 }

 /* Check if last vertex of p is same as last vertex of p1 */ 
 if(lvalue == (int)VECTOR(*p1)[igraph_vector_size(p1)-1]) { 
	igraph_vector_reverse(p1);
	igraph_vector_remove(p1,0);
	igraph_vector_append(p,p1);
	return 1;
  }
// igraph_vector_destroy(p1);
 
 return -1;
}
 
int igraph_path_size(Path *p) {

  return igraph_vector_size(p);

}

void igraph_path_str(Path *p, char *pathstr) {

   *pathstr = '\0';
   for(int i=0; i<igraph_path_size(p); i++) {
     if(i==0)
      sprintf(pathstr,"%d",(int)VECTOR(*p)[i]);
     else
      sprintf(pathstr,"%s-%d",pathstr,(int)VECTOR(*p)[i]);
   }
}

void igraph_path_print(Path *p) {
    char pathstr[200];
    igraph_path_str(p,pathstr);
    printf("%s\n",pathstr);
 }

void igraph_path_clear(Path *p) {

   int size = igraph_path_size(p);
   for(int i=0; i<size;i++)
       igraph_vector_remove(p,0);
 }
