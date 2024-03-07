// Erblina Nivokazi 150200917 Algo HW3
// EB
// Implementing Binary Search Tree in C++

#include <iostream>

namespace BST
{
  struct Node
  {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
  };
}

class BinarySearchTree
{
private:
  BST::Node *root;  // I'm setting a root pointer that I can use throughout the functions

  // Helper Functions for the functions in public
  void insertHelper(BST::Node *node, BST::Node *newCityNode)  // This function I wrote as a helper function for insert
  { 
    if (node->data > newCityNode->data) // At first I check whether the data in the new City NOde that I'm inserting
    {                                   // If the new City Node has data smaller than the other node then I go to the left
      if (node->left == nullptr)        // If the node in the left is null then I insert the new node immediately
      {
        node->left = newCityNode;       // Insertion of the newCityNode
        newCityNode->parent = node;     // Setting the newCityNode's parent to node
      }
      else  // If the node in the left is not null then I recall the function 
      {     // When recalling the function the function will continue checking until it hits a nullptr which it can replace with the newCityNode pointer
        insertHelper(node->left, newCityNode); 
      }
    }
    else  // If the newCityNOde data is bigger than the node's data
    {     // Then it starts checking in the right side of the node for a nullptr position to insert the node 
      if (node->right == nullptr) // If the node's right pointer is nullptr then the nodeCityNode is inserted 
      {
        node->right = newCityNode;  // Insertion of the newCityNode to the right
        newCityNode->parent = node;
      }
      else // But if the right pointer of the node is not nullptr then the function is called again
      {    // The function is called again until it finds a correct free position
        insertHelper(node->right, newCityNode); // Insertion of the newCityNode
      }
    }
  }

  int getHeightHelper(BST::Node *node)  // This is a helper function for the getHeight function in the public part of this class
  {
    if (node == nullptr)  // I check for nullptr immediately as I enter the function
    {
      return -1;  // When I hit a nullptr I return -1 because the root is not supposed to be counted as part of the height
    }
    int n = getHeightHelper(node->left);   // I call the function for the left side to check the hieght in the left side
    int m = getHeightHelper(node->right);  // I call the function for the right side to check the height in the right side

    int i;      // I declare an i integer value
    if (n > m)  // Here I check whether the lenght in n (left side) is bigger than in the right side. I choose the longer side
    {
      i = n;
    }
    else
    {
      i = m;
    }
    return i + 1; // Return the height + 1 as e recursive 
  }

  int totalNodesHelper(BST::Node *node) // This is a helper function to get the total nodes of the tree
  {
    if (node == nullptr)  // First checking whether the node is a nullptr or not
    {
      return 0; // If yes then return 0
    }
    int n = totalNodesHelper(node->left); // Recursively call the function for the left side and ckeep adding the result to itself
    int m = totalNodesHelper(node->right); // Recursively call the function for the right side and keep adding the result to itself
    return n + m + 1; // Add both sides of the tree and return them (Adding both results of left and right)
  }

  BST::Node *searchTreeHelper(BST::Node *node, int num) // This is a helper function for the searchTree function
  {
    if (node == nullptr)  // Checking for nullptr so that the function doesnt continue further
    {
      return nullptr; 
    }
    else if (num < node->data)  // We check whether the node we're searching for is smaller or bigger
    {                           // If the number is smaller then we take this route
      return searchTreeHelper(node->left, num); // I recursively call the function to search the tree from the left side of the tree 
    }
    else if (num > node->data)  // Checking whether the node is bigger
    {                           // If the data im searching for is bigger then recursively call the right side of the tree
      return searchTreeHelper(node->right, num);
    }
    else  // If neither then just return the node
    {
      return node;
    }
  }

  BST::Node *successorHelper(BST::Node * node){ // This is a helper function for the successor function
    while (node->left != nullptr) {             // This is basically a minimum function that find the minimum value in the left of the right node which i pass as a parameter
        node = node->left;
    }
    return node;
  }

  BST::Node *predecessorHelper(BST::Node * node){ // This works the same way as the successorHelper function
    while (node->right != nullptr) {              // instead of the minimum it finds the maximum data containing node in the left side
      node = node->right;                         // Left node is passed as parameter from the predecessor function
    }
    return node;
  }

  void inorderHelper(BST::Node* node, std::pair<std::string, int> A[], int& index) {  // This a helper function for the inorder function that is in the public part of the object
    if (node != nullptr) {    // The parameters are same as the function but im taking the index as a reference so that its value changes throughout the function calls
        inorderHelper(node->left, A, index); // For inorder first the left side of the tree is printed, in this case I'm not printing I'm writing into the array 
        A[index++] = std::make_pair(node->name, node->data);
        inorderHelper(node->right, A, index); // Then the right side of the tree is called recursively
    }
  }

  void preorderHelper(BST::Node* node, std::pair<std::string, int> A[], int& index) {  // This a helper function for the preorder function that is in the public part of the object
    if (node != nullptr) {    // The parameters are same as the function but im taking the index as a reference so that its value changes throughout the function calls
        A[index++] = std::make_pair(node->name, node->data);
        preorderHelper(node->left, A, index); // Left side is recursively called 
        preorderHelper(node->right, A, index); // Then the right side of the tree is called recursively
    }
  }

    void postorderHelper(BST::Node* node, std::pair<std::string, int> A[], int& index) {  // This a helper function for the postorder function that is in the public part of the object
    if (node != nullptr) {    // The parameters are same as the function but im taking the index as a reference so that its value changes throughout the function calls
        postorderHelper(node->left, A, index); // Left side is recursively called 
        postorderHelper(node->right, A, index); // Then the right side of the tree is called recursively
        A[index++] = std::make_pair(node->name, node->data); // Data is added
    }
  }

  void deleteNodeHelper(BST::Node* node, int testpop){ // This is a helper function for the delete function in public
    if (node == nullptr)  // First check for null so that the function doesn't start running in an empty node
    {
        return;
    }

    if (node->data > testpop) // If the node containing the data I want to delete is smaller than the data in my current node
    {                         // I call the function recursively from the left side of the tree
        deleteNodeHelper(node->left, testpop);
    }
    else if (node->data < testpop)  // But if the node containing the data that I want to delete is bigger then
    {                               // I call the function recursively from the right side of the tree
        deleteNodeHelper(node->right, testpop);
    }
    else  // If its neither of the above cases then the function checks the structure of the node containing the data
    {     // The data is found in this node
        if (node->left == nullptr)   // This is the first case where the node containing the data has a nullptr as a left child 
        {
          // Case 1: Node has no left child
          // Replace current node with its right child
          BST::Node *temp = node->right;  // I put the right child in a temporary node, because I'm going to replace it with the node that will be deleted
          if (node->parent != nullptr)
          {
              if (node == node->parent->left)  
              {
                // If the current node is the left child of its parent then replace it with temp
                node->parent->left = temp;
              }
              else
              {
                // If the current node is the right child of its parent
                node->parent->right = temp;
              }
          }
          if (temp != nullptr)
          {
              temp->parent = node->parent;
          }
          // Delete the current node
          delete node;
        }
        else if (node->right == nullptr)
        {
          // Case 2: Node has no right child
          // Replace current node with its left child
          BST::Node *temp = node->left;
          if (node->parent != nullptr)
          {
              if (node == node->parent->left)
              {
                  // If the current node is the left child of its parent
                  node->parent->left = temp;
              }
              else
              {
                  // If the current node is the right child of its parent
                  node->parent->right = temp;
              }
          }
          if (temp != nullptr)
          {
              temp->parent = node->parent;
          }
          delete node;
        }
        else
        {
          // Case 3: Node has both left and right children
          // Find the successor, replace current node's data with successor's data,
          // and recursively delete the successor
          BST::Node *successor = successorHelper(node);
          node->data = successor->data;
          deleteNodeHelper(successor, successor->data);
        }
    }  
  
}
  
  // I wasn't sure how to implement these since in the main they're not used so I just implemented them as print functions
  // Since they're already written in the bst.cpp file
  void preOrderHelper(BST::Node *node) {  // This is a helper function for the preorder function 
    if (node != nullptr) {  
    std::cout << node->name << " " << node->data << std::endl;  // In the preOrder it first prints the node then the left and right children
    preOrderHelper(node->left); // Recursively calling the preOrderHelper function for the left side of the tree
    preOrderHelper(node->right);  // Recursively calling the preOrderHelper function for the right side of the tree
    }
  }

  void postOrderHelper(BST::Node *node) { // Same functionanlities and implementation as in the preOrder expect the left child is printed first then right then the node
    if (node != nullptr) {
    postOrderHelper(node->left);
    postOrderHelper(node->right);
    std::cout << node->name << " " << node->data << std::endl;
    }
  }

  void inOrderHelper(BST::Node *node) { // Same functionanlities and implementation as in the preOrder expect the left child is printed first then the node then the right child
    if (node != nullptr) {
    inOrderHelper(node->left);
    std::cout << node->name << "," << node->data << "\n";
    inOrderHelper(node->right);
    }
  }



public:
  BinarySearchTree() : root(nullptr)  // Setting the root to a nullptr
  {
  }

  void preorder() // The preorder function works by calling the helper function that I wrote in the private
  {
    preOrderHelper(this->root); // I send the root as a parameter so that it can traverse the actual tree
  }

  void postorder()  // Same as the preorder 
  {
    postOrderHelper(this->root);
  }

  void inorder() // This is the other inorder which just prints the tree in an inordered way
  {
    inOrderHelper(this->root);
  }

  void inorder(std::pair<std::string, int> A[], int index) // The inorder function works by calling the helper function that I wrote in the private
  {                                                        // This is the inorder that is used in the main.cpp file
    int currentIndex = index;
    inorderHelper(this->root, A, currentIndex);                  // I needed the inOrderHelper function so that I can actually send the pointer of the root and use it
  }

  // The preorder function, implementation similar to inorder
  void preorder(std::pair<std::string, int> A[], int index)
  {                                                       
    int currentIndex = index;
    preorderHelper(this->root, A, currentIndex);
  }

  // postorder function implementation similar to inorder
  void postorder(std::pair<std::string, int> A[], int index)
  {                                                      
    int currentIndex = index;
    postorderHelper(this->root, A, currentIndex);
  }

  BST::Node *searchTree(int testPop)  // It just calls the helper function, which takes as parameter root pointer and the value of the data that is being searching
  {
    return searchTreeHelper(this->root, testPop);
  }

  BST::Node *successor(BST::Node *node) // Successor function for bst
  {
    if(node->right != nullptr){ // Checking whether the right child of the node is nullptr, if its not then we can check it for successor
      return successorHelper(node->right);  // Call successorHelper function which just basically returns the minimum of the left side of the right node 
    }
    BST::Node *p = node->parent;  // If the right child of the node is not null then we check the parent
    while (p != nullptr && node == p->right) { // We check the parent node for as long as the node is it's right child
        node = p;
        p = p->parent;
    }
    return p; // return the parent node
  }

  BST::Node *predecessor(BST::Node *node) // Predecessor function for bst
  { // The implementation of this function is the sam as predecessor except for the direction in which it checks for the predecessor
    if(node->left != nullptr){  // It checks the left side for it's right most node
      return predecessorHelper(node->left);
    }
    BST::Node *p = node->parent;
    while (p != nullptr && node == p->left) { // Same as successor except for the direction
        node = p;
        p = p->parent;
    }
    return p;
  }

  void insert(std::string city, int population) // Insert function for bst
  {
    BST::Node *newCity = new BST::Node; // Creating a new node with the inputs
    newCity->data = population;
    newCity->name = city;
    newCity->left = newCity->right = newCity->parent = nullptr;

    if (root == nullptr)  // If the root is null then it just inserts to the root
    {
      this->root = newCity;
    }
    else
    {
      insertHelper(this->root, newCity);  // If the root is not null then it calls the helper function 
    }
  }

  void deleteNode(int testPop)  // For delete I call the helper function directly
  {
    deleteNodeHelper(this->root, testPop);
  }

  int getHeight() // Call helper function
  {
    return getHeightHelper(this->root);
  }

  BST::Node *getMaximum() // getMaximum recursively calls itself getting the right child for the node, this way it retrieves the maximum value of the tree
  {
    BST::Node *maximum = this->root;
    while (maximum->right != nullptr)
    {
      maximum = maximum->right;
    }
    return maximum;
  }

  BST::Node *getMinimum() // Same implementation as getMaximum but here the function recursively calls itself with the left child as parameter
  {                       // So that it reaches the most left child and return it as minimum
    BST::Node *minimum = this->root;
    while (minimum->left != nullptr)
    {
      minimum = minimum->left;
    }
    return minimum;
  }

  int getTotalNodes() // Call helper function
  {
    return totalNodesHelper(this->root);
  }
};