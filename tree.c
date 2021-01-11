#include "tree.h"

struct Performance *newPerformance(){ //create perf
  struct Performance *theNewP;
  theNewP = malloc(sizeof(struct Performance));
  if (theNewP == NULL){
    fprintf(stderr, "failed to allocate memory in malloc.\n");
    exit(1);
  }

  theNewP->reads=0;
  theNewP->writes=0;
  theNewP->mallocs=0;
  theNewP->frees=0;

return theNewP;
}

void attachNode (struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width){
  struct Node *pushNode;
  pushNode = malloc(sizeof(struct Node));

  if (pushNode == NULL){
    fprintf(stderr, "failed to allocate memory in malloc.\n");  //test for failed malloc
    exit(1);
  }

  if ((pushNode->data = (void*)malloc(width)) == NULL){ //test for failed malloc
    fprintf(stderr, "failed to malloc data.\n");
    exit(1);
  }

    pushNode->lt = NULL;
    pushNode->gte = NULL;

    memcpy(pushNode->data, src, width); //copying bytes

    (*node_ptr) = pushNode;

    performance->mallocs = performance->mallocs + 1;
    performance->writes = performance->writes + 1;
}

int comparNode (struct Performance *performance, struct Node **node_ptr,int (*compar)(const void *, const void *) ,void *target){
  performance->reads = performance->reads + 1;
  return (compar(target, (*node_ptr)->data));
}

struct Node **next (struct Performance *performance, struct Node **node_ptr, int direction){
  if (*node_ptr == NULL){
    fprintf(stderr, "node is null (next).\n");
    exit(1);
  }

  performance->reads = performance->reads + 1;

  if (direction >= 0){
      return &((*node_ptr)->gte);
  } else {
      return &((*node_ptr)->lt);
  }
}

void readNode (struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width){
  if (*node_ptr == NULL){
    fprintf(stderr, "node is null (read).\n");
    exit(1);
  }

  memcpy(dest, (*node_ptr)->data, width);  //cpy to dest

  performance->reads = performance->reads + 1;
}

void detachNode (struct Performance *performance, struct Node **node_ptr){
  if (*node_ptr == NULL){
    fprintf(stderr, "node is null (detach).\n");
    exit(1);
  }

  free((*node_ptr)->data);
  free (*node_ptr);

  *node_ptr = NULL;

  performance->frees = performance->frees + 1;
}

int isEmpty (struct Performance *performance, struct Node **node_ptr){
  if (*node_ptr == NULL){
    return 1;
  }
  return 0;
}

void addItem (struct Performance *performance, struct Node **node_ptr,int (*compar)(const void *, const void *) ,void *src, unsigned int width){
  struct Node** temp = node_ptr;

  while (isEmpty(performance, temp) == 0){
    temp = (next(performance, temp, comparNode(performance, temp, compar, src)));
  }

  attachNode(performance, temp, src, width);
}

void freeTree(struct Performance *performance, struct Node **node_ptr){

  if (isEmpty(performance, node_ptr) == 0){
    freeTree(performance, next(performance, node_ptr, 1));
    freeTree(performance, next(performance, node_ptr, -1));
    detachNode(performance, node_ptr);
  }

}

int searchItem (struct Performance *performance, struct Node **node_ptr,int (*compar)(const void *, const void *) ,void *target, unsigned int width){
  struct Node** temp = node_ptr;


  while (isEmpty(performance, temp) == 0){
    int compVal = comparNode(performance, temp, compar, target);

    if (compVal == 0){ //compar = 0
      readNode(performance, temp, target, width);
      return 1;
    } else {
      temp = next(performance,temp,compVal);
    }

  }
  return 0;
}
