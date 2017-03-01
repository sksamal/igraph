/* -*- mode: C -*-  
 * Nov 1, 2016: Fix for there can be some vertices, that are connected to 
 *  		one side at a minimum (min requirement is no longer 2, just 1)
 * Generates a grid labelled graph, which can later by processed for AT Check
 * */

#include <igraph.h>
#include "igraph_atfree.h"
#define MLEVELS 7
#define LSIZE 7 

//	      0	   1      2        3       4      5      6        7      8       9     10
enum label { none, box, cross, pzeep , pyesp , pzeel , pyesl , lzeep , lyesp , lzeel , lyesl };
enum strt { n, l , z , s , ps, pz}; 

struct port {
   unsigned int bit[8]; /* 8 bits */
   unsigned int active;
};
typedef struct port at_port;


struct unit {
    at_port p1, p2;  // three to store connections, last bit for activation
  };
typedef struct unit at_unit;

struct node {
   at_unit *atu;
   struct node *next;
   enum label type;
  };
typedef struct node at_row;

void init_unit(at_unit *atu, enum strt lbl);
void append_unit(at_row *head, at_unit *atu);
void create_structure(at_row *atr, enum label lbl) ;
void append_dunit(at_row *head, enum label lbl);

void init_unit(at_unit *atu, enum strt lbl) {

    atu->p1.bit[0] = 1; 
    atu->p2.bit[0] = 1; 

   switch (lbl) {

	case l:
	    atu->p2.bit[3] = atu->p1.bit[2] = 1; 
	    break; 

	case s:
	    atu->p1.bit[4] =  1; 
	    break; 

        case z:
	    atu->p2.bit[7] = 1; 
	    break; 

	case ps:
	    atu->p1.bit[4] =  1; 
	    atu->p2.bit[5] =  1; 
	    break; 
	
	case pz:
	    atu->p1.bit[6] =  1; 
	    atu->p2.bit[7] =  1; 
	    break; 
	
      }
 }

void append_unit(at_row *head, at_unit *atu) {
 
    at_row *node = (at_row*)malloc(sizeof(at_row*));
    node->atu = atu;
    node->next = NULL;
    if(head == NULL) 
       { head = node; return; }
    at_row *tail = head;

    while(tail->next!=NULL) tail=tail->next; 
    tail->next = node;

    tail->atu->p1.bit[0] = atu->p1.bit[1] = (tail->atu->p1.bit[0] | atu->p1.bit[1]); 
    tail->atu->p2.bit[0] = atu->p2.bit[1] = (tail->atu->p2.bit[0] | atu->p2.bit[1]); 
     
    tail->atu->p1.bit[4] = atu->p2.bit[5] = (tail->atu->p1.bit[4] | atu->p2.bit[5]); 
    tail->atu->p2.bit[7] = atu->p1.bit[6] = (tail->atu->p1.bit[7] | atu->p2.bit[6]); 
 }
     
void append_dunit(at_row *head, enum label lbl) {

    at_unit *atu1 = (at_unit*)malloc(sizeof(at_unit*));
    at_unit *atu2 = (at_unit*)malloc(sizeof(at_unit*));

   switch (lbl) {

	case box:
	    init_unit(atu1,l); init_unit(atu2,l);  break; 
	case cross:
	    init_unit(atu1,s); init_unit(atu1,z);  break; 
        case pzeep:
	    init_unit(atu1,pz); init_unit(atu2,z); break; 
        case pyesp:
	    init_unit(atu1,ps); init_unit(atu2,s); break; 
        case pzeel:
	    init_unit(atu1,pz); init_unit(atu2,l); break; 
        case pyesl:
	    init_unit(atu1,ps); init_unit(atu2,l); break; 
        case lzeep:
	    init_unit(atu1,l);  init_unit(atu1,z); init_unit(atu2,z);
	    break; 
        case lyesp:
	    init_unit(atu1,l); init_unit(atu1,s); init_unit(atu2,s);
	    break; 
        case lzeel:
	    init_unit(atu1,l); init_unit(atu1,z); init_unit(atu2,l);
	    break; 
        case lyesl:
	    init_unit(atu1,l); init_unit(atu1,s); init_unit(atu2,l);
	    break; 
      }
	    append_unit(head,atu1);
	    append_unit(head,atu2);
 }

void create_structure(at_row *head, enum label lbl) {

    at_unit *atu1 = (at_unit*)malloc(sizeof(at_unit*));
    at_unit *atu2 = (at_unit*)malloc(sizeof(at_unit*));
    at_row *node2 = (at_row*)malloc(sizeof(at_row*));
    atu1->p1.bit[0] = atu2->p1.bit[1] = (atu1->p1.bit[0] | 1); 
    atu1->p2.bit[0] = atu2->p2.bit[1] = (atu1->p2.bit[0] | 1); 
    head->atu = atu1;
    head->next = node2;
    node2->atu = atu2;
    node2->next = NULL;

   switch (lbl) {

	case box:
	    atu1->p2.bit[3] = atu1->p1.bit[2] = (atu1->p1.bit[2] | 1); 
	    atu2->p2.bit[3] = atu2->p1.bit[2] = (atu2->p1.bit[2] | 1); 
	    break; 

	case cross:
	    atu2->p2.bit[5] = atu1->p1.bit[4] = (atu1->p1.bit[4] | 1); 
	    atu2->p1.bit[6] = atu1->p2.bit[7] = (atu1->p2.bit[7] | 1); 
	    break; 

        case pzeep:
	    atu2->p2.bit[7] = atu1->p1.bit[6] = (atu1->p1.bit[6] | 1); 
	    atu2->p1.bit[6] = atu1->p2.bit[7] = (atu1->p2.bit[7] | 1); 
	    break; 
			
        case pyesp:
	    atu2->p1.bit[4] = atu1->p2.bit[5] = (atu1->p1.bit[5] | 1); 
	    atu2->p2.bit[5] = atu1->p1.bit[4] = (atu1->p2.bit[4] | 1); 
	    break; 
			
        case pzeel:
	    atu1->p1.bit[6] = (atu1->p1.bit[6] | 1); 
	    atu2->p1.bit[6] = atu1->p2.bit[7] = (atu1->p2.bit[7] | 1); 
	    atu2->p2.bit[3] = atu2->p1.bit[2] = (atu2->p1.bit[2] | 1); 
	    break; 
   
        case pyesl:
	    atu1->p2.bit[5] = (atu1->p1.bit[5] | 1); 
	    atu2->p2.bit[5] = atu1->p1.bit[4] = (atu1->p2.bit[4] | 1); 
	    atu2->p2.bit[3] = atu2->p1.bit[2] = (atu2->p1.bit[2] | 1); 
	    break; 

        case lzeep:
	    atu1->p2.bit[3] = atu1->p1.bit[2] = (atu1->p1.bit[2] | 1); 
	    atu2->p1.bit[6] = atu1->p2.bit[7] = (atu1->p2.bit[7] | 1); 
	    atu2->p2.bit[7] = (atu2->p2.bit[7] | 1); 
	    break; 
   
        case lyesp:
	    atu1->p2.bit[3] = atu1->p1.bit[2] = (atu1->p1.bit[2] | 1); 
	    atu2->p2.bit[5] = atu1->p1.bit[4] = (atu1->p2.bit[4] | 1); 
	    atu2->p2.bit[7] = (atu2->p2.bit[7] | 1); 
	    break; 

        case lzeel:
	    atu1->p2.bit[3] = atu1->p1.bit[2] = (atu1->p1.bit[2] | 1); 
	    atu2->p1.bit[6] = atu1->p2.bit[7] = (atu1->p2.bit[7] | 1); 
	    atu2->p2.bit[3] = atu2->p1.bit[2] = (atu2->p1.bit[2] | 1); 
	    break; 
   
        case lyesl:
	    atu1->p2.bit[3] = atu1->p1.bit[2] = (atu1->p1.bit[2] | 1); 
	    atu2->p2.bit[5] = atu1->p1.bit[4] = (atu1->p2.bit[4] | 1); 
	    atu2->p2.bit[3] = atu2->p1.bit[2] = (atu2->p1.bit[2] | 1); 
	    break; 
      }
 }

int main(int argc, char** argv) {
  
  igraph_t g;
  int ret;
  time_t t;
  srand((unsigned) time(&t));
  int count = 0;

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  at_row *head = NULL, *dhead = NULL; 
  at_unit atu1, atu2, atu3;
  init_unit(&atu1,l);
  init_unit(&atu2,z);
  append_unit(head,&atu1);
  append_unit(head,&atu2);
  append_dunit(dhead,box);
  append_unit(dhead,&atu2);


  /* empty directed graph, zero vertices */
  igraph_empty(&g, 2, IGRAPH_UNDIRECTED);

  at_row *atrptr = dhead;
  int v = 2;
  while(atrptr!=NULL) 
  {
  	at_unit *ptr = atrptr->atu;
	igraph_add_vertices(&g,2,0);
	/*Draw backwards*/
	if(ptr->p1.bit[1]==1) igraph_add_edge(&g,v,v-2);
	if(ptr->p1.bit[2]==1) igraph_add_edge(&g,v,v+1);
	if(ptr->p1.bit[7]==1) igraph_add_edge(&g,v,v-1);
	if(ptr->p2.bit[5]==1) igraph_add_edge(&g,v+1,v-2);
	if(ptr->p2.bit[1]==1) igraph_add_edge(&g,v,v-2);
  
	if(atrptr->next==NULL) {
	   igraph_add_vertices(&g,2,0);
	   if(ptr->p1.bit[0]==1) igraph_add_edge(&g,v,v+2);
	   if(ptr->p1.bit[4]==1) igraph_add_edge(&g,v,v+3);
	   if(ptr->p2.bit[6]==1) igraph_add_edge(&g,v+1,v+2);
	   if(ptr->p2.bit[0]==1) igraph_add_edge(&g,v,v+2);
	}

	v=v+2;

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
