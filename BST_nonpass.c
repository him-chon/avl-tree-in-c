// Binary Search Tree ADT implementation ... COMP9024 22T3

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "BST.h"
#include "pointerQueue.h"

#define RANDOM_ROOT_INSERT (rand() % 10 < 4)   // 40% chance

#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

Tree avlRotation(Tree);
Tree localAVLRotation(Tree, Item);

typedef struct Node {
   Item data;
   Tree left, right;
} Node;

// make a new node containing data
Tree newNode(Item it) {
   Tree new = malloc(sizeof(Node));
   assert(new != NULL);
   data(new) = it;
   left(new) = right(new) = NULL;
   return new;
}

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      free(t);
   }
}

// compute height of Tree
int TreeHeight(Tree t) {
   if (t == NULL) {
      return -1;
   } else {
      int lheight = 1 + TreeHeight(left(t));
      int rheight = 1 + TreeHeight(right(t));
      if (lheight > rheight)
	 return lheight;
      else
	 return rheight;
   }
}

// count #nodes in Tree
int TreeNumNodes(Tree t) {
   if (t == NULL)
      return 0;
   else
      return 1 + TreeNumNodes(left(t)) + TreeNumNodes(right(t));
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, Item it) {
   if (t == NULL)
      return false;
   else if (it < data(t))
      return TreeSearch(left(t), it);
   else if (it > data(t))
      return TreeSearch(right(t), it);
   else                                 // it == data(t)
      return true;
}

// insert a new item into a Tree
Tree TreeInsert(Tree t, Item it) {
   if (t == NULL)
      t = newNode(it);
   else if (it < data(t))
      left(t) = TreeInsert(left(t), it);
   else if (it > data(t))
      right(t) = TreeInsert(right(t), it);
   return t;
}

Tree joinTrees(Tree t1, Tree t2) {
   if (t1 == NULL)
      return t2;
   else if (t2 == NULL)
      return t1;
   else {
      Tree curr = t2;
      Tree parent = NULL;
      while (left(curr) != NULL) {    // find min element in t2
	 parent = curr;
	 curr = left(curr);
      }
      if (parent != NULL) {
	 left(parent) = right(curr);  // unlink min element from parent
	 right(curr) = t2;
      }
      left(curr) = t1;
      return curr;                    // min element is new root
   }
}

// delete an item from a Tree
Tree TreeDelete(Tree t, Item it) {
   if (t != NULL) {
      if (it < data(t))
	 left(t) = TreeDelete(left(t), it);
      else if (it > data(t))
	 right(t) = TreeDelete(right(t), it);
      else {
	 Tree new;
	 if (left(t) == NULL && right(t) == NULL) 
	    new = NULL;
	 else if (left(t) == NULL)    // if only right subtree, make it the new root
	    new = right(t);
	 else if (right(t) == NULL)   // if only left subtree, make it the new root
	    new = left(t);
	 else                         // left(t) != NULL and right(t) != NULL
	    new = joinTrees(left(t), right(t));
	 free(t);
	 t = new;
      }
   }
   return t;
}

Tree rotateRight(Tree n1) {
   if (n1 == NULL || left(n1) == NULL)
      return n1;
   Tree n2 = left(n1);
   left(n1) = right(n2);
   right(n2) = n1;
   return n2;
}

Tree rotateLeft(Tree n2) {
   if (n2 == NULL || right(n2) == NULL)
      return n2;
   Tree n1 = right(n2);
   right(n2) = left(n1);
   left(n1) = n2;
   return n1;
}

Tree insertAtRoot(Tree t, Item it) { 
   if (t == NULL) {
      t = newNode(it);
   } else if (it < data(t)) {
      left(t) = insertAtRoot(left(t), it);
      t = rotateRight(t);
   } else if (it > data(t)) {
      right(t) = insertAtRoot(right(t), it);
      t = rotateLeft(t);
   }
   return t;
}

Tree insertRandom(Tree t, Item it) { 
   if (t == NULL)
      t = newNode(it);
   if (RANDOM_ROOT_INSERT)
      return insertAtRoot(t, it);
   else
      return TreeInsert(t, it);
}

Tree insertAVL(Tree t, Item it) {
   if (t == NULL)
      return newNode(it);
   if (it == data(t))
      return t;

   if (it < data(t))
      left(t) = insertAVL(left(t), it);
   else
      right(t) = insertAVL(right(t), it);

   int hL = TreeHeight(left(t));
   int hR = TreeHeight(right(t));
   if ((hL - hR) > 1) {
      if (it > data(left(t))) {
         left(t) = rotateLeft(left(t));
      }
      t = rotateRight(t);
   } else if ((hR - hL) > 1) {
      if (it < data(right(t))) {
	 right(t) = rotateRight(right(t));
      }
      t = rotateLeft(t);
   }

   return t;
}

Tree deleteAVL(Tree t, Item it) {
   if (t != NULL) {
      if (it < data(t))
         left(t) = TreeDelete(left(t), it);
      else if (it > data(t))
         right(t) = TreeDelete(right(t), it);
      else {
         Tree new;
         if (left(t) == NULL && right(t) == NULL)
            new = NULL;
         else if (left(t) == NULL)    // if only right subtree, make it the new root
            new = right(t);
         else if (right(t) == NULL)   // if only left subtree, make it the new root
            new = left(t);
         else                         // left(t) != NULL and right(t) != NULL
            new = joinTrees(left(t), right(t));
         free(t);
         t = new;
         return t;
      }
   }
   int hL = TreeHeight(left(t));
   int hR = TreeHeight(right(t));
   printf("left: %d, right: %d, node: %d", hL, hR, data(t));

   if ((hL - hR) > 1) {
      printf("left: %d, right: %d, node: %d", hL, hR, data(t));
      if (TreeHeight(right(left(t))) > TreeHeight(left(left(t)))) {
         left(t) = rotateLeft(left(t));
      }
      t = rotateRight(t);
   } else if ((hR - hL) > 1) {
      printf("left: %d, right: %d, node: %d", hL, hR, data(t));
      if (TreeHeight(left(right(t))) > TreeHeight(right(right(t)))) {
         right(t) = rotateRight(right(t));
      }
      t = rotateLeft(t);
   }
   return t;
}
//   // Only attempt deletion and subsequent rotation if node exist
//   if(TreeSearch(t, it)){
//      t = TreeDelete(t, it);
//      if (abs(TreeHeight(left(t)) - TreeHeight(right(t))) > 1){
//         t = localAVLRotation(t, it);
//         t = avlRotation(t);
//      }
//   }
//   return t;
//}

Tree localAVLRotation(Tree t, Item it) {
   if (t == NULL)
      return NULL;
   if (it < data(t))
      left(t) = localAVLRotation(left(t), it);
   else
      right(t) = localAVLRotation(right(t), it);

   int hL = TreeHeight(left(t));
   int hR = TreeHeight(right(t));

   if ((hL - hR) > 1) {
      if (it < data(left(t))) {
         left(t) = rotateLeft(left(t));
      }
      t = rotateRight(t);
   } else if ((hR - hL) > 1) {
      if (it > data(left(t))) {
         right(t) = rotateRight(right(t));
      }
      t = rotateLeft(t);
   }
   return t;
}


Tree avlRotation(Tree t) {
   if (t == NULL)
      return NULL;

   int hL = TreeHeight(left(t));
   int hR = TreeHeight(right(t));

   if ((hL - hR) > 1) {
      //recursion to left side if left is deeper
      left(t) = avlRotation(left(t));
      if (right(left(t)) != NULL) {
         left(t) = rotateLeft(left(t));
      }
      t = rotateRight(t);
   } else if ((hR - hL) > 1) {
      //recursion to right side if right is deeper
      right(t) = avlRotation(right(t));
      if (left(right(t)) != NULL) {
         right(t) = rotateRight(right(t));
      }
      t = rotateLeft(t);
   }
   return t;
}

Tree insertSplay(Tree t, Item it) {
   if (t == NULL)
      return newNode(it);
   if (it == data(t))
      return t;

   if (it < data(t)) {
      if (left(t) == NULL) {
	 left(t) = newNode(it);
      } else if (it < data(left(t))) {
	 left(left(t)) = insertSplay(left(left(t)), it);
	 t = rotateRight(t);
      } else if (it > data(left(t))) {
	 right(left(t)) = insertSplay(right(left(t)), it);
	 left(t) = rotateLeft(left(t));
      }
      return rotateRight(t);
   } else {
      if (right(t) == NULL) {
	 right(t) = newNode(it);
      } else if (it < data(right(t))) {
	 left(right(t)) = insertSplay(left(right(t)), it);
	 right(t) = rotateRight(right(t));
      } else if (it > data(right(t))) {
	 right(right(t)) = insertSplay(right(right(t)), it);
	 t = rotateLeft(t);
      }
      return rotateLeft(t);
   }
}

Tree partition(Tree t, int i) {
   if (t != NULL) {
      assert(0 <= i && i < TreeNumNodes(t));
      int m = TreeNumNodes(left(t));
      if (i < m) {
	 left(t) = partition(left(t), i);
	 t = rotateRight(t);
      } else if (i > m) {
	 right(t) = partition(right(t), i-m-1);
	 t = rotateLeft(t);
      }
   }
   return t;
}

Tree rebalance(Tree t) {
   int n = TreeNumNodes(t);

   if (n >= 3) {
      t = partition(t, n/2);           // put node with median key at root
      left(t) = rebalance(left(t));    // then rebalance each subtree
      right(t) = rebalance(right(t));
   }
   return t;
}

// display tree, solution by Chin Pok Leung

void showTreeBot(Tree t);

void showTree(Tree t) {
   //showTreeR(t, 0);
   showTreeBot(t);
}

/**
 * gets max value in the tree
 * @param t tree instance
 * @returns max value
 */
int max(Tree t) {
   assert(t != NULL);
   
   return right(t) == NULL ?
      data(t) :
      max(right(t));
}

/**
 * gets min value in the tree
 * @param t tree instance
 * @returns min value
 */
int min(Tree t) {
   assert(t != NULL);

   return left(t) == NULL ?
      data(t) :
      min(left(t));
}

/**
 * f(n):
 * - 1 if n = 0;
 * - floor(log10(n)) + 1 if n > 0;
 * - floor(log10(-n)) + 2 if n < 0;
 * @param n
 * @returns f(n)
 */
int numberLength(int n) {
   if (n == 0) 
      return 1;
   
   int len = 0;

   if (n < 0) {
      len = 1;
      n = -n;
   }

   while (n > 0) {
      len++;
      n /= 10;
   }

   return len;
}

/**
 * compute required node width
 * @param t tree instance
 * @returns max({ f(n) | all n in t })
 */
int nodeWidth(Tree t) {
   // length of max value in tree
   int maxv = numberLength(max(t));
   // length of min value in tree
   int minv = numberLength(min(t));

   return maxv > minv ?
      maxv :
      minv;
}

/**print sub-tree task for `showTreeBot`*/
typedef struct {
   /**sub-tree to be printed*/
   Tree node;
   /**offset*/
   int offset;
} PrintTask;

/**
 * create a new print task instance
 * @param node sub-tree
 * @param offset
 */
PrintTask* newPrintTask(Tree node, int offset) {
   PrintTask* pt = malloc(sizeof(PrintTask));
   assert(pt != NULL);

   pt->node = node;
   pt->offset = offset;
   return pt;
}

/**
 * print the binary tree top-down
 * @param t tree instance
 */
void showTreeBot(Tree t) {
   /*
   for simplicity:
   - width of all nodes are fixed: choose max length of all nodes
   - node values are printed in left-justify.

   example output:
           100
       ....:........
       20          500
   ....:       ....:....
   -1          250     1000
                       :....
                           9999
   */

   if (t == NULL) return;

   // node width
   int node_w = nodeWidth(t);
   // tree width
   int tree_w = TreeNumNodes(t) * node_w;

   // queue for level order traversal (bfs)
   queue q0 = newQueue();
   QueueEnqueue(q0, newPrintTask(t, 0));

   // buffer for node values
   char* str0 = malloc((tree_w + 1) * sizeof(char));
   // buffer for edges
   char* str1 = malloc((tree_w + 1) * sizeof(char));
   assert(str0 != NULL && str1 != NULL);

   // temp buffer for storing node value
   char* nodeValue = malloc((node_w + 1) * sizeof(char));
   assert(nodeValue != NULL);

   str0[tree_w] = '\0';
   str1[tree_w] = '\0';

   // level order traversal
   while (!QueueIsEmpty(q0)) {
      // queue for storing print tasks for next level (if any)
      queue q1 = newQueue();

      // reset strings to spaces
      for (int i = 0; i < tree_w; ++i) {
         str0[i] = ' ';
         str1[i] = ' ';
      }

      while (!QueueIsEmpty(q0)) {
         PrintTask* pt = QueueDequeue(q0);

         int offset_l = TreeNumNodes(left(pt->node)) * node_w;
         pt->offset += offset_l;
         
         // write data(node) to str1
         sprintf(nodeValue, "%d", data(pt->node));

         int currentOffset = pt->offset;

         for (int i = 0; nodeValue[i] != '\0'; ++i)
            str0[currentOffset++] = nodeValue[i];


         if (left(pt->node) != NULL) {
            // space occupied by the left child and its right subtree
            int edge_len = (1 + TreeNumNodes(right(left(pt->node)))) * node_w ;

            // draw edge
            for (int i = pt->offset - edge_len; i <= pt->offset; ++i)
               str1[i] = '.';

            QueueEnqueue(q1, newPrintTask(
               left(pt->node), 
               pt->offset - offset_l
            ));
         }

         if (right(pt->node) != NULL) {
            // space occupied by the root and the left subtree of right child
            int edge_len = (1 + TreeNumNodes(left(right(pt->node)))) * node_w;

            // draw edge
            for (int i = pt->offset; i <= pt->offset + edge_len; ++i)
               str1[i] = '.';

            QueueEnqueue(q1, newPrintTask(
               right(pt->node), 
               pt->offset + node_w
            ));
         }

         if (left(pt->node) != NULL || right(pt->node) != NULL)
            str1[pt->offset] = ':';

         free(pt);
      }

      printf("%s\n%s\n", str0, str1);

      dropQueue(q0);
      q0 = q1;
   }

   dropQueue(q0);

   free(str0);
   free(str1);
   free(nodeValue);
}