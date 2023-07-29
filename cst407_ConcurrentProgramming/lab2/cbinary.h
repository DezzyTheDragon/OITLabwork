#pragma once
//******************************************************
// Interface for a binary tree for storing integers.
// NOTE: No attempt is made to balance the tree
// 
//
// Author: Phil Howard

typedef void *tree_t;
typedef void *element_t;

#define LOCK_MODE_NONE          1
#define LOCK_MODE_COARSE        2
#define LOCK_MODE_READ_WRITE    3
#define LOCK_MODE_FINE          4

// Create and initialize a binary tree
tree_t Tree_Init(int lock_type);

// Insert an integer into a binary tree
// If the integer is already in the tree, a duplicate node will be created
void Tree_Insert(tree_t tree, int value);

// Delete an integer from a binary tree
// If the integer is not found, no operation is performed on the tree
// If multiple nodes contain the specified value, only one of them will be 
//    deleted
void Tree_Delete(tree_t tree, int value);

// Find a value in the tree.
// If the value is found, the element is returned.
// If the value is not found, the function returns NULL
//
// Any locks that are required to access the lement will still be held when
// the element is returned. The calling function must call Element_Release in
// order to release the locks before calling any other tree function otherwise 
// a deadlock might result.
element_t Tree_Lookup(tree_t tree, int value);

// Traverse the tree calling the specified function on each node
// Any locks that are required for read access to the element will be held when 
// the function is called. 
//
// The function should NOT release the locks. The Tree_Traverse function will
// handle the release of the locks.
void Tree_Traverse(tree_t tree, void (*func)(element_t element));

// Destroy a tree and free all memory
void Tree_Destroy(tree_t tree);

// Retrieve the value from an element
int Element_Value(element_t element);

// release any locks held by an element
void Element_Release(element_t element);

