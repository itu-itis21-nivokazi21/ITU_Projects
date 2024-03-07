// Erblina Nivokazi 150200917 Algo HW3
// EB
// Implementing Red-Black Tree in C++

#include <iostream>

namespace RBT {
  struct Node {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
    int color;
  };
}

class RedBlackTree {
private:
  RBT::Node *root;
  RBT::Node *sentinel;  // Here, different from bst, I declare a sentinel since nullptr is not sufficient to check for leaves, since nil in rbt is black it has a color its not just a nullptr

  void insertHelper(RBT::Node* &node, RBT::Node *newNode) { // Helper function for insert
    //  Two pointers are declared to help with the function
    RBT::Node *parentCity = sentinel; // Pointer to track the parent of the current node
    RBT::Node *currentCity = node; // Pointer to traverse the tree starting from the node given in the parameter 

    // Traverse the tree to find the appropriate position for the new node
    while (currentCity != sentinel) { // As long as the current City is not sentinel then it continues to go left and right accordingly
        parentCity = currentCity; // Update the parent pointer before moving down the tree
        // Compare the data of the new node with the current node
        if (newNode->data < currentCity->data) {
            currentCity = currentCity->left;
        } else {
            currentCity = currentCity->right;
        }
    }

    // Set the parent of the new node to the last non-null node encountered
    newNode->parent = parentCity;
    
    // Update the appropriate child pointer of the parent to point to the new node
    if (parentCity == sentinel) {
      // If the tree was initially empty then the new node becomes the root
      root = newNode;
    } else if (newNode->data < parentCity->data) {
      // If the new node's data is smaller, it becomes the left child of the parent
      parentCity->left = newNode;
    } else {
      // If the new node's data is greater or equal, it becomes the right child of the parent
      parentCity->right = newNode;
    }
  }

  void insertFix(RBT::Node *node) { // Functio to Fix the Red-Black Tree properties after insertion

    // Check if the current node, its parent, and its grandparent are not sentinel nodes
    if (node == sentinel || node->parent == sentinel || node->parent->parent == sentinel) {
      // If any of them is a sentinel, no fix is needed
      return;
    }
    RBT::Node *uncle;  // Pointer to uncle node
    // Continue fixing until the current node's parent is not red (color == 1) or until reaching the root
    while (node->parent != sentinel && node->parent->color == 1) {
      
      if (node->parent == node->parent->parent->right) {
        // If the parent is the right child of the grandparent

        uncle = node->parent->parent->left;  // Set uncle as the left child of the grandparent
        if (uncle->color == 1) {
          // Case 1: When uncle is red
          uncle->color = 0; // Uncle is set to black
          node->parent->color = 0; // node parent is set to black
          node->parent->parent->color = 1; // Grandparent is set to red
          node = node->parent->parent;   // Move to the grandparent
          
        } else {
          // Case 2: Uncle is black
          if (node == node->parent->left) {
            node = node->parent;
            rightRotate(node);
          }
          // Case 3: Uncle is black, and the current node is a right child
          node->parent->color = 0;
          node->parent->parent->color = 1;
          leftRotate(node->parent->parent);
        }
      } else {
        // If the parent is the left child of the grandparent

        uncle = node->parent->parent->right;  // Set uncle as the right child of the grandparent
        if (uncle->color == 1) {
          // Case 1: Uncle is red

          uncle->color = 0; // Color of uncle is changed to black
          node->parent->color = 0;  // node parent color is also changed to black
          node->parent->parent->color = 1;  // grandparent is changed to red
          node = node->parent->parent;  // Move to the grandparent
        } else {
          // Case 2: Uncle is black
                
          if (node == node->parent->right) {
            node = node->parent;
            leftRotate(node);
          }
          // Case 3: Uncle is black, and the current node is a left child
          node->parent->color = 0;
          node->parent->parent->color = 1;
          rightRotate(node->parent->parent);
        }
      }
      
      // The function breaks if the current node is the root
      if (node == root) {
        break;
      }
    }
    root->color = 0;   // Set the root to black to maintain the Red-Black Tree properties
  }

  int getHeightHelper(RBT::Node *node) {  // Helper function for getHeight, implementation is the same as in bst with minor changes
    if (node == sentinel) // Instead of nullptr here I check with sentinel instead
    {
      return -1;
    }
    int n = getHeightHelper(node->left);
    int m = getHeightHelper(node->right);

    int i;
    if (n > m)
    {
      i = n;
    }
    else
    {
      i = m;
    }
    return i + 1;
  }

  int totalNodesHelper(RBT::Node *node) { // Same implementation as for bst
    if (node == sentinel) // Minor change; instead of checking for nullptr check for sentinel
    {
      return 0;
    }
    int n = totalNodesHelper(node->left);
    int m = totalNodesHelper(node->right);
    return n + m + 1;
  }

  RBT::Node *searchTreeHelper(RBT::Node *node, int num){  // Same implementation as for bst.cpp
    if (node == sentinel) // Minor change: instead of nullptr here function checks for sentinel
    {
      return sentinel;
    }
    else if (num < node->data)
    {
      return searchTreeHelper(node->left, num);
    }
    else if (num > node->data)
    {
      return searchTreeHelper(node->right, num);
    }
    else
    {
      return node;
    }
  }

  RBT::Node *successorHelper(RBT::Node * node){ // Successor Helper function for successor function which just basically returns the most left node
    while (node->left != sentinel) {            // Again same implementation as in bst, but instead of nullptr sentinel is used
        node = node->left;
    }
    return node;
  }

  RBT::Node *predecessorHelper(RBT::Node * node){ // Same functionality as successorHelper but it returns the right most node, so the maximum value holding node
    while (node->right != sentinel) {
      node = node->right;
    }
    return node;
  }

  void inorderHelper(RBT::Node* node, std::pair<std::string, int> A[], int& index) {  // inorderHelper for inorder function, same implementation as in bst
    if (node != sentinel) {   // Instead of nullptr, I now check the node with sentinel
        inorderHelper(node->left, A, index);  // Function is recursively calling itself for the left side of the tree since its inorder
        A[index++] = std::make_pair(node->name, node->data);
        inorderHelper(node->right, A, index);
    }
  }

  void preorderHelper(RBT::Node* node, std::pair<std::string, int> A[], int& index) {  // preorderHelper for preorder function, same implementation as in bst
    if (node != sentinel) {   // Instead of nullptr, I now check the node with sentinel
        A[index++] = std::make_pair(node->name, node->data);  // First the node is written to array since its preorder
        preorderHelper(node->left, A, index);  // Function is recursively calling itself for the left side of the tree since its preorder
        preorderHelper(node->right, A, index);  // Then it recursively calls the right side
    }
  }

  void postorderHelper(RBT::Node* node, std::pair<std::string, int> A[], int& index) {  // inorderHelper for inorder function, same implementation as in bst
    if (node != sentinel) {   // Instead of nullptr, I now check the node with sentinel
        postorderHelper(node->left, A, index);  // The function is first recursively calling itself from the left side of the tree since its postorder
        postorderHelper(node->right, A, index); // Then it recursively calls itself from the right side
        A[index++] = std::make_pair(node->name, node->data);  // Writing to array
    }
  }


  // These functions I've only implemented because at first I wasnt sure how to fill these functions since they weren't 
  // mentioned in the main so I implemented them in this way too just in case
  void preOrderHelper(RBT::Node *node) {
    if (node != sentinel) {
    std::cout << node->name << "," << node->data << std::endl;
    preOrderHelper(node->left);
    preOrderHelper(node->right);
    }
  }

  void postOrderHelper(RBT::Node *node) {
    if (node != sentinel) {
    postOrderHelper(node->left);
    postOrderHelper(node->right);
    std::cout << node->name << "," << node->data << std::endl;
    }
  }

  void inOrderHelper(RBT::Node *node) {
    if (node != sentinel) {
    inOrderHelper(node->left);
    std::cout << node->name << "," << node->data << std::endl;
    inOrderHelper(node->right);
    }
  }

  void transplant(RBT::Node *sourceNode, RBT::Node *targetNode) {
    // Replace one subtree with another in the Red-Black Tree

    // Check if the source node is the root of the tree
    if (sourceNode->parent == sentinel) {
      // Case 1: Source node is the root
      root = targetNode; // Set the target node as the new root
    } else if (sourceNode == sourceNode->parent->left) {
      // Case 2: Source node is a left child of its parent
      sourceNode->parent->left = targetNode;  // Set the target node as the left child of source node's parent
    } else {
      // Case 3: Source node is a right child of its parent
      sourceNode->parent->right = targetNode;  // Set the target node as the right child of source node's parent
    }
    
    // Update the parent pointer of the target node to point to the source node's parent
    targetNode->parent = sourceNode->parent;
  }

  RBT::Node *findMin(RBT::Node *node) { // I created this function to find the minimum value, I needed this in the deleteFixUp
    while (node->left != sentinel) {
        node = node->left;
    }
    return node;
  }

  void deleteFixUp(RBT::Node *node){
    // Fix the Red-Black Tree properties after a deletion

    RBT::Node *sibling;
    
    // Continue fixing until the current node is the root or its color is red (color == 1)
    while(node != root && (sibling = (node == node->parent->left ? node->parent->right : node->parent->left))->color == 0){
      
      if (node == node->parent->left){
        // If the current node is a left child
        sibling = node->parent->right;  // Set sibling as the right child of the parent

        if(sibling->color == 1){
          // Case 1: Sibling is red
          sibling->color = 0; // Set sibling color to black
          node->parent->color = 1;  // Set the node's parent's color to red
          leftRotate(node->parent); // leftRotating the subtree with the root as node->parent
          sibling = node->parent->right; // sibling is set to be node's parents' right child
        }

      if(sibling->left->color == 0 && sibling->right->color == 0){
        // Case 2: Both of sibling's children are black
                
        sibling->color = 1;
        node = node->parent;  // Move up the tree
      }else{
        if(sibling->right->color==0){
          // Case 3: Sibling's right child is black
          // Similar to case 1 but instead of leftRotate-ing the node's parent here it is rightRotated
          sibling->left->color = 0;
          sibling->color = 1;
          rightRotate(sibling);
          sibling = node->parent->right;
        }
        // Case 4: Sibling's right child is red
        sibling->color = node->parent->color;
        node->parent->color = 0;
        sibling->parent->color = 1;
        rightRotate(node->parent);
        node= root;   // Break out of the loop
      }
    } else {
      // If the current node is a right child
    
      sibling = node->parent->left;    // Set sibling as the left child of the parent

      if (sibling->color == 1) {
          // Case 1: Sibling is red
          sibling->color = 0; // sibling's color is changed to black
          node->parent->color = 1;  // node's parent's color is set to red
          rightRotate(node->parent);  // rightRotating the subtree with the root as node->parent
          sibling = node->parent->left; // sibling now is the parent's left child
      }

    if (sibling->right->color == 0 && sibling->right->color == 0) {
          // Case 2: Both of sibling's children are black
          sibling->color = 1;
          node = node->parent;  // Move up the tree
        } else {
          if (sibling->left->color == 0) {
            // Case 3: Sibling's left child is black
            // Similar to case 1 but instead of rightRotation, leftRotation is being called on the subtree with the node's parent as the root
            sibling->right->color = 0;
            sibling->color = 1;
            leftRotate(sibling);
            sibling = node->parent->left;
          }
          // Case 4: Sibling's left child is red
          sibling->color = node->parent->color;
          node->parent->color = 0;
          sibling->left->color = 0;
          rightRotate(node->parent);
          node = root;  // Break out of the loop
        }
      }
    }
    node->color = 0;  // Set the current node to black to maintain the Red-Black Tree properties

  }

void deleteNodeHelper(RBT::Node *node, int testpop) {
    // THis is a Helper function for deleting a node with a specific key in the Red-Black Tree

    // Find the node with the specified key to delete
    RBT::Node *nodeToDelete = searchTreeHelper(this->root, testpop);  // (z) Find the node with the key to delete

    // Check if the node with the key was not found in the tree
    if (nodeToDelete == sentinel) {
        std::cout << "Key was not found in the tree" << std::endl;  // Print a message and return if not found
        return;
    }

    RBT::Node *successorNode;  // (x) Node that will replace the deleted node
    RBT::Node *tempNode;       // (y) Temporary node to store information during deletion

    tempNode = nodeToDelete;
    int tempNode_color = tempNode->color;

    // Determine the successor node based on the structure of the node to delete
    if (nodeToDelete->left == sentinel) {
        // If the left child is a sentinel, use the right child as the successor
        successorNode = nodeToDelete->right;
        transplant(nodeToDelete, nodeToDelete->right);  // Replace the node with its right child
    } else if (nodeToDelete->right == sentinel) {
        // If the right child is a sentinel, use the left child as the successor
        successorNode = nodeToDelete->left;
        transplant(nodeToDelete, nodeToDelete->left);   // Replace the node with its left child
    } else {
        // If the node has both left and right children, I find the successor from the right subtree
        tempNode = findMin(nodeToDelete->right);  // Here the function finds the minimum node in the right subtree
        tempNode_color = tempNode->color;          // Saves the color of the minimum node
        successorNode = tempNode->right;

        if (tempNode->parent == nodeToDelete) {
            // If the minimum node is directly connected to the node to delete then:
            successorNode->parent = tempNode;
        } else {
            // else the minimum node is not directly connected to the node to delete
            transplant(tempNode, tempNode->right);      // Replacing the minimum node with its right child
            tempNode->right = nodeToDelete->right;      // Connecting the right subtree of the node to delete
            tempNode->right->parent = tempNode;          // ANd Updating the parent of the right subtree
        }

        transplant(nodeToDelete, tempNode);           // Calling transplant to replace the node to delete with the minimum node
        tempNode->left = nodeToDelete->left;           // Connecting the left subtree of the node to delete
        tempNode->left->parent = tempNode;             // Update the parent of the left subtree
        tempNode->color = nodeToDelete->color;         // And copy the color of the node to delete
    }

    delete nodeToDelete;  // The memory occupied by the node to delete is freed

    if (tempNode_color == 0) {
        // If the color of the replacement node is black, then the deleteFixUp function is called to fix the Red-Black Tree properties
        deleteFixUp(successorNode);
    }
}


public:
  RedBlackTree() {  // Different from bst here the constructor for RedBlackTree creates the sentinel which a NIL Red-Black Tree node
                    // And root is set to be a sentinel at first
    sentinel = new RBT::Node;
    sentinel->color = 0;
    sentinel->left = nullptr;
    sentinel->right = nullptr;
    sentinel->parent = nullptr;
    root = sentinel;
  }

  void leftRotate(RBT::Node *pivotNode) {
    // At first the function performs a left rotation in the Red-Black Tree
    RBT::Node *newParentNode = pivotNode->right;  // The right child of the pivot node becomes the new parent
    pivotNode->right = newParentNode->left;       // Here the left subtree of the new parent moves to the right of the pivot node

    if (newParentNode->left != sentinel) {
        newParentNode->left->parent = pivotNode;  // Update the parent of the moved subtree
    }

    newParentNode->parent = pivotNode->parent;  // Update the parent of the new parent node

    if (pivotNode->parent == sentinel) {
        // If the pivot node is the root
        this->root = newParentNode;  // Set the new parent node as the new root
    } else if (pivotNode == pivotNode->parent->left) {
        // else if the pivot node is a left child
        pivotNode->parent->left = newParentNode;  // Set the new parent node as the left child of the pivot node's parent
    } else {
        // else the pivot node is a right child
        pivotNode->parent->right = newParentNode;  // Here it sets the new parent node as the right child of the pivot node's parent
    }
    newParentNode->left = pivotNode;        // the pivot node os made to be the left child of the new parent node
    pivotNode->parent = newParentNode;      // Update the parent of the pivot node to be the new parent node
  }

  void rightRotate(RBT::Node *pivotNode) {
    // At first the function performs a right rotation in the Red-Black Tree
    RBT::Node *newParentNode = pivotNode->left;  // The new parent is set to the left child of the pivot node
    pivotNode->left = newParentNode->right;      // HEre the right subtree of the new parent is moved to the left of the pivot node

    if (newParentNode->right != sentinel) {
        newParentNode->right->parent = pivotNode;  // The parent of the moved subtree is updated
    }

    newParentNode->parent = pivotNode->parent;  // The parent of the new parent node is updated

    if (pivotNode->parent == sentinel) {
        // If the pivot node is the root
        this->root = newParentNode;  // The new parent node is set  as the new root
    } else if (pivotNode == pivotNode->parent->right) {
        // If the pivot node is a right child
        pivotNode->parent->right = newParentNode;  // The new parent node is set as the right child of the pivot node's parent
    } else {
        // If the pivot node is a left child
        pivotNode->parent->left = newParentNode;  // THe new parent node is set as the left child of the pivot node's parent
    }

    newParentNode->right = pivotNode;   // Make the pivot node the right child of the new parent node
    pivotNode->parent = newParentNode;  // Update the parent of the pivot node to be the new parent node
  }

  void preorder() { // preorder for printing the tree
    preOrderHelper(this->root);
  }

  void inorder() {  // inorder for printing the tree
    inOrderHelper(this->root);
  }
  
  void postorder() { // postorder for printing the tree
    postOrderHelper(this->root);
  }

  void inorder(std::pair<std::string, int> A[], int index)
  { // inorder function to write into the array in an inorder way
    inorderHelper(root, A, index);
  }

  void preorder(std::pair<std::string, int> A[], int index)
  { // preorder function to write into the array in an preorder way
    preorderHelper(root, A, index);
  }

  void postorder(std::pair<std::string, int> A[], int index)
  { // postorder function to write into the array in an postorder way
    postorderHelper(root, A, index);
  }

  RBT::Node *searchTree(int testPop) {  // searchTree function, here I return the helper function of searchTree
    return searchTreeHelper(this->root, testPop);
  }

  RBT::Node *successor(RBT::Node *node) { // Successor function is same as in bst.cpp except for some minor changes
    if(node->right != sentinel){  // Minor change; instead of nullptr here the func checks whether the right child is equal to sentinel
      return successorHelper(node->right); // Gets the left most node of the right side fo the tree
    }
    // if the node is a leaf
    RBT::Node *p = node->parent;
    while (p != sentinel && node == p->right) { // Minor change; instead of nullptr here too it is changed to sentinel
        node = p;
        p = p->parent;
    }
    return p;
  }

  RBT::Node *predecessor(RBT::Node *node) { // Predecessor function is also the same as in bst.cpp
    if(node->left != sentinel){ // nullptr changed to sentinel
      return predecessorHelper(node->left);  // Gets the right most node of the right side fo the tree
    }
    // if the node is a leaf
    RBT::Node *p = node->parent;
    while (p != sentinel && node == p->left) {  // nullptr changed to sentinel
        node = p;
        p = p->parent;
    }
    return p;
  }

  void insert(std::string city, int population) { // Insert function for Red-Black Tree
    RBT::Node *newCity = new RBT::Node; // New node is create with the given data that is taken as parameter
    newCity->parent = sentinel;
    newCity->data = population;
    newCity->name = city;
    newCity->left = sentinel;
    newCity->right = sentinel;
    newCity->color = 1;

    insertHelper(this->root, newCity); // insertHelper function is called with root and new node as parameters
    // Check and fix Red-Black Tree properties after insertion
    if (newCity->parent == sentinel) {
      // If the parent of the new node is sentinel, it means the new node is the root
      newCity->color = 0; // THe color of newCity is set to black
      return;
    }

    if (newCity->parent->parent == sentinel) {
      // If the grandparent of the new node is sentinel, it means that there is no violation
      return;
    }

    // Otherwise the insertFix function is called to fix the properties of Red-Black Tree
    insertFix(newCity);
  }

  void deleteNode(int testpop) {  // deleteNode function calls the helper function for deleteNode
    deleteNodeHelper(this->root, testpop);
  }

  int getHeight() { // getHeight function calls the helper function for getHeight
    return getHeightHelper(this->root);
  }

  RBT::Node *getMaximum() { // Same as the function in bst.cpp with adjustments
    RBT::Node *maximum = this->root;
    while (maximum->right != sentinel)
    {
      maximum = maximum->right;
    }
    return maximum;
  }

  RBT::Node *getMinimum() { // Same as the function in bst.cpp with adjustments
    RBT::Node *minimum = this->root;
    while (minimum->left != sentinel)
    {
      minimum = minimum->left;
    }
    return minimum;
  }

  int getTotalNodes() { // This function calls the helper function for getTotalNodes
    return totalNodesHelper(this->root);
  }
};