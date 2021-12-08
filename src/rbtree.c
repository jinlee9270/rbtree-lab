#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *node_nil = (node_t *)calloc(1, sizeof(node_t));
  node_nil->color = RBTREE_BLACK;
  node_nil->left = NULL;
  node_nil->right = NULL;
  p->root = node_nil;
  p->nil = node_nil;
  return p;
}

void delete_node(rbtree *t, node_t *n){
  if(n != t->nil){
    delete_node(t, n->left);
    delete_node(t, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t) {
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *new_node){
  node_t *new_r = new_node->right;
  new_node->right = new_r->left;
  if(new_r->left != t->nil)
  {
    new_r->left->parent = new_node;
  }
  new_r->parent = new_node->parent;
  if(new_node->parent == t->nil)
  {
    t->root = new_r;
  }
  else if(new_node == new_node->parent->left)
  {
    new_node->parent->left = new_r;
  }
  else
  {
    new_node->parent->right = new_r;
  }
  new_r->left = new_node;
  new_node->parent = new_r;
}

void right_rotate(rbtree *t, node_t *new_node){
  node_t *new_l = new_node->left;
  new_node->left = new_l->right;
  if(new_l->right != t->nil)
  {
    new_l->right->parent = new_node;
  }
  new_l->parent = new_node->parent;
  if(new_node->parent == t->nil)
  {
    t->root = new_l;
  }
  else
  {
    if(new_node->parent->right == new_node)
    {
      new_node->parent->right = new_l;
    }
    else
    {
      new_node->parent->left = new_l;
    }
  }
  new_l->right = new_node;
  new_node->parent = new_l;
}

void rb_insert_fixup(rbtree *t, node_t *target){
  node_t *y;

  while(target->parent->color == RBTREE_RED)
  {
    if(target->parent == target->parent->parent->left)
    {
      y = target->parent->parent->right;
      if(y->color == RBTREE_RED)
      {
        target->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }
      else
      {
        if(target == target->parent->right)
        {
          target = target->parent;
          left_rotate(t, target);
        }
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        right_rotate(t,target->parent->parent);
      }
    }
    else
    {
      y = target->parent->parent->left;
      if(y->color == RBTREE_RED)
      {
        target->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }
      else
      {
        if(target == target->parent->left)
        {
          target = target->parent;
          right_rotate(t, target);
        }
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        left_rotate(t,target->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    node_t *parent = t->nil; // parent
    node_t *ptr = t->root; // temp
    while(ptr != t->nil)
    {
        parent = ptr;
        if(ptr->key > key)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }
    new_node -> parent = parent;
    if (parent == t->nil)
    {
      t->root = new_node;
    }else if(key < parent->key){
      parent->left = new_node;
    }else{
      parent->right = new_node;
    }
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->key = key;
    new_node->color = RBTREE_RED;
    rb_insert_fixup(t, new_node);
  return new_node;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *ptr;
  ptr = t->root;
  while(ptr != t->nil)
  {
    if(ptr->key > key)
    {
      ptr = ptr->left;
    }
    else if(ptr->key < key)
    {
      ptr = ptr->right;
    }
    else
    {
      return ptr;
    }
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {

  node_t *ptr;
  ptr = t->root;
  if(ptr != t->nil){
    while(ptr ->left != t->nil){
      ptr = ptr->left;
    }
  }
  return ptr;
}
node_t *node_min(const rbtree *t, node_t *n) {

  node_t *ptr;
  ptr = n;
  while(ptr ->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
}


node_t *rbtree_max(const rbtree *t) {

  node_t *ptr;
  ptr = t->root;
  if(ptr != t->nil){
    while(ptr ->right != t->nil){
      ptr = ptr->right;
    }
  }
  return ptr;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil)
  {
    t->root = v;
  }
  else if(u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rb_delete_fixup(rbtree *t, node_t *target){
  node_t *u;
  while(t->root != target && target->color == RBTREE_BLACK)
  {
    if(target->parent->left ==target)
    {
      u = target->parent->right;
      if(u->color == RBTREE_RED)
      {
        target->parent->color = RBTREE_RED;
        u->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        u = target->parent->right;
      }
      if(u->left->color == RBTREE_BLACK && u->right->color == RBTREE_BLACK)
      {
        u->color = RBTREE_RED;
        target = target->parent;
      }
      else
      {
        if(u->right->color == RBTREE_BLACK)
        {
          u->color = RBTREE_RED;
          u->left->color = RBTREE_BLACK;
          right_rotate(t, u);
          u = target->parent->right;
        }
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }
    else
    {
      u = target->parent->left;
      if(u->color == RBTREE_RED)
      {
        target->parent->color = RBTREE_RED;
        u->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        u = target->parent->left;
      }
      if(u->right->color == RBTREE_BLACK && u->left->color == RBTREE_BLACK)
      {
        u->color = RBTREE_RED;
        target = target->parent;
      }
      else
      {
        if(u->left->color == RBTREE_BLACK)
        {
          u->color = RBTREE_RED;
          u->right->color = RBTREE_BLACK;
          left_rotate(t, u);
          u = target->parent->left;
        }
        u->color = u->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *ptr;
  color_t y_color = p->color;
  if(p->left == t->nil)
  {
    ptr = p->right;
    rb_transplant(t, p, p->right);
  }
  else if(p->right == t->nil)
  {
    ptr = p->left;
    rb_transplant(t, p, p->left);
  }
  else
  {
    y = node_min(t, p->right);
    y_color = y->color;
    ptr = y->right;
    if(y->parent == p)
    {
      ptr->parent = y;
    }
    else
    {
      rb_transplant(t, y, y->right);
      p->right->parent = y;
      y->right = p->right;
    }
    rb_transplant(t, p, y);
    y->left = p->left;
    p->left->parent = y;
    y->color = p->color;
  }
  if(y_color == RBTREE_BLACK)
  {
    rb_delete_fixup(t, ptr);
  }
  free(p);
  return 0;
}

int node_to_array(const rbtree *t, const node_t *n, key_t *arr, int i){
  if(n->left != t->nil)
  {
    i = node_to_array(t, n->left, arr, i);
  }

  arr[i] = n->key;
  i++;
  
  if(n->right != t->nil)
  {
    i = node_to_array(t, n->right, arr, i);
  }
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_to_array(t, t->root, arr, 0);
  return 0;
}

