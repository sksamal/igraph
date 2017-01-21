
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

int igraph_paths_size(Paths *ps)
{
  return igraph_vector_ptr_size(ps);
}

void igraph_paths_sort(Paths *ps)
{
  for(int i=0; i<igraph_vector_ptr_size(ps); i++)
   {
     Path *temp = VECTOR(*ps)[i];
     for(int j=i-1; j>0; j--)
	{
	  Path *vj = VECTOR(*ps)[j];
	  if(igraph_vector_size(vj) > igraph_vector_size(temp)) {
		VECTOR(*ps)[j+1] = VECTOR(*ps)[j]; 
          }
	  else
	  	{ VECTOR(*ps)[j+1] = temp; break; }
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
  for(int i=0; i<size; i++) {
	Path *vi = VECTOR(*ps)[i]; 
	int lpos = igraph_vector_size(vi) -1; 
  	while(reqd > 0 && lpos > 0) {
 	  igraph_vector_t p1;
	  igraph_vector_init(&p1,1); 
	  VECTOR(p1)[0] = (int) VECTOR(*vi)[lpos]; 
	  igraph_vector_remove(vi,lpos); 
	  lpos--; reqd--;
	  igraph_vector_ptr_push_back(ps,&p1);
	} 
	if(reqd == 0) break;
      }
   igraph_paths_sort(ps);
   return 0;
 }

void igraph_paths_merge(Paths *ps, Paths *ps1) {
   
  for(int i=0; i<igraph_vector_ptr_size(ps1); i++) {
	Path *vi = VECTOR(*ps1)[i]; 
	int lvalue = (int) VECTOR(*vi)[0];
	int j=0;
  	for(; j<igraph_vector_ptr_size(ps); j++) {
	    Path *vj = VECTOR(*ps)[j];   /* Find if the first vertex in ps1 matches with last vertex in ps */
	    int rpos = igraph_vector_size(vj) -1;
	    if(lvalue == (int)VECTOR(*vj)[rpos]) {
		igraph_vector_append(vj,vi);
		igraph_vector_destroy(vi);
	 	break;
	    }	
	 }
	if (j== igraph_vector_ptr_size(ps))  /* Not found, need to push back the complete vector */
	    igraph_vector_ptr_push_back(ps,vi);
    }
   igraph_paths_sort(ps);
}
