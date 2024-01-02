//-----------------------------------------------------------------------------
// Name: Lia Cui
// CRUZID: ljcui
// Assignment Name: PA7
//
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>

#ifndef DICTIONARY_H_INCLUDE_
#define DICTIONARY_H_INCLUDE_


// Exported types -------------------------------------------------------------
typedef std::string keyType;  // must be comparable using <, <=, >, >=, ==, !=
typedef int         valType;

class Dictionary{

private:

   // private Node struct
   struct Node{
      // Node fields
      keyType key;
      valType val;
      Node* parent;
      Node* left;
      Node* right;
      // Node constructor
      Node(keyType k, valType v);
   };

   // Dictionary fields
   Node* nil;
   Node* root;
   Node* current;
   int   num_pairs;

   // Helper Functions (Optional) ---------------------------------------------

   Node* findKeyhelper(std::string k, Node* ptr) const {
      if (ptr != NULL){
         if (ptr->key == k)
               return ptr;
         else{
               if (ptr->key > k)
                  return findKeyhelper(k, ptr->left);
               else
                  return findKeyhelper(k, ptr->right);
         }
      }
      else{
         return NULL;
      }
   }

   // inOrderString()
   // Appends a string representation of the tree rooted at R to string s. The
   // string appended consists of: "key : value \n" for each key-value pair in
   // tree R, arranged in order by keys.
   std::string inOrderString(Node* R) const{
      std::string s = "";
      if (R == nullptr) {
         return "";
      }

      if (R->left != nullptr) {
         s += inOrderString(R->left);
      }
      s = s + R->key + " : " + std::to_string(R->val) + "\n";
      if (R->right != nullptr) {
         s += inOrderString(R->right);
      }
      return s;
   }

   // preOrderString()
   // Appends a string representation of the tree rooted at R to s. The appended
   // string consists of keys only, separated by "\n", with the order determined
   // by a pre-order tree walk.
   std::string preOrderString(Node* R) const{
      std::string s = "";
      if (R == nullptr) {
         return "";
      }

      s += R->key + "\n";

      if (R->left != nullptr) {
         s += preOrderString(R->left);
      }
      if (R->right != nullptr) {
         s += preOrderString(R->right);
      }
      return s;
   }

   // preOrderCopy()
   // Recursively inserts a deep copy of the subtree rooted at R into this 
   // Dictionary. Recursion terminates at N.
   void preOrderCopy(Node* R, Node* N){
      if(R != N || R != nil){
         setValue(R->key, R->val);
         preOrderCopy(R->left, N);
         preOrderCopy(R->right, N);
      }
      return;
   }

   // postOrderDelete()
   // Deletes all Nodes in the subtree rooted at R.
   void postOrderDelete(Node* R){
//      if(R != nil || R != nullptr){
      if(R != nullptr){
         postOrderDelete(R->left);
         postOrderDelete(R->right);
         remove(R->key);
      }
   }

   // search()
   // Searches the subtree rooted at R for a Node with key==k. Returns
   // the address of the Node if it exists, returns nil otherwise.
   Node* search(Node* R, keyType k) const{
      while(R != nullptr && k != R->key){
         R = (k < R->key) ? R->left : R->right;
      }
      return R;
   }

   // findMin()
   // If the subtree rooted at R is not empty, returns a pointer to the 
   // leftmost Node in that subtree, otherwise returns nil.
   Node* findMin(Node* R){
      if(R != nil){
         Node* N = R;
         while(N->left != nil){
               N = N->left;
         }
         return N;
      }else{
         return nil;
      }
   }

   // findMax()
   // If the subtree rooted at R is not empty, returns a pointer to the 
   // rightmost Node in that subtree, otherwise returns nil.
   Node* findMax(Node* R){
      if(R != nil){
         Node* N = R;
         while(N->right != nil){
               N = N->right;
         }
         return N;
      }else{
         return nil;
      }
   }

   // findNext()
   // If N does not point to the rightmost Node, returns a pointer to the
   // Node after N in an in-order tree walk.  If N points to the rightmost 
   // Node, or is nil, returns nil. 
   Node* findNext(Node* N){
      if(N == nil){
         return nil;
      }
      if(N->right != nil){
         return findMin(N->right);
      }
      Node* M = N->parent;
      while(M != nil && N == M->right){
         N = M;
         M = M->parent;
      }
      return M;
   }

   // findPrev()
   // If N does not point to the leftmost Node, returns a pointer to the
   // Node before N in an in-order tree walk.  If N points to the leftmost 
   // Node, or is nil, returns nil.
   Node* findPrev(Node* N){
      if(N == nil){
         return nil;
      }
      if(N->left != nil){
         return findMax(N->left);
      }
      Node* M = N->parent;
      while(M != nil && N == M->left){
         N = M;
         M = M->parent;
      }
      return M;
   }

   // transplant()
   //helper function for remove()
   void transplant(Node* A, Node* B){
        if(A->parent == nil){
            this->root = B;
        }else if(A == A->parent->left){
            A->parent->left = B;
        }else{
            A->parent->right = B;
        }
        if(B != nil){
            B->parent = A->parent;
        }
    }

public:

   // Class Constructors & Destructors ----------------------------------------
   
   // Creates new Dictionary in the empty state. 
   Dictionary();

   // Copy constructor.
   Dictionary(const Dictionary& D);

   // Destructor
   ~Dictionary();


   // Access functions --------------------------------------------------------

   // size()
   // Returns the size of this Dictionary.
   int size() const;

   // contains()
   // Returns true if there exists a pair such that key==k, and returns false
   // otherwise.
   bool contains(keyType k) const;

   // getValue()
   // Returns a reference to the value corresponding to key k.
   // Pre: contains(k)
   valType& getValue(keyType k) const;

   // hasCurrent()
   // Returns true if the current iterator is defined, and returns false 
   // otherwise.
   bool hasCurrent() const;

   // currentKey()
   // Returns the current key.
   // Pre: hasCurrent() 
   keyType currentKey() const;

   // currentVal()
   // Returns a reference to the current value.
   // Pre: hasCurrent()
   valType& currentVal() const;


   // Manipulation procedures -------------------------------------------------

   // clear()
   // Resets this Dictionary to the empty state, containing no pairs.
   void clear();

   // setValue()
   // If a pair with key==k exists, overwrites the corresponding value with v, 
   // otherwise inserts the new pair (k, v).
   void setValue(keyType k, valType v);

   // remove()
   // Deletes the pair for which key==k. If that pair is current, then current
   // becomes undefined.
   // Pre: contains(k).
   void remove(keyType k);

   // begin()
   // If non-empty, places current iterator at the first (key, value) pair
   // (as defined by the order operator < on keys), otherwise does nothing. 
   void begin();

   // end()
   // If non-empty, places current iterator at the last (key, value) pair
   // (as defined by the order operator < on keys), otherwise does nothing. 
   void end();

   // next()
   // If the current iterator is not at the last pair, advances current 
   // to the next pair (as defined by the order operator < on keys). If 
   // the current iterator is at the last pair, makes current undefined.
   // Pre: hasCurrent()
   void next();

   // prev()
   // If the current iterator is not at the first pair, moves current to  
   // the previous pair (as defined by the order operator < on keys). If 
   // the current iterator is at the first pair, makes current undefined.
   // Pre: hasCurrent()
   void prev();


   // Other Functions ---------------------------------------------------------

   // to_string()
   // Returns a string representation of this Dictionary. Consecutive (key, value)
   // pairs are separated by a newline "\n" character, and the items key and value 
   // are separated by the sequence space-colon-space " : ". The pairs are arranged 
   // in order, as defined by the order operator <.
   std::string to_string() const;

   // pre_string()
   // Returns a string consisting of all keys in this Dictionary. Consecutive
   // keys are separated by newline "\n" characters. The key order is given
   // by a pre-order tree walk.
   std::string pre_string() const;

   // equals()
   // Returns true if and only if this Dictionary contains the same (key, value)
   // pairs as Dictionary D.
   bool equals(const Dictionary& D) const;


   // Overloaded Operators ----------------------------------------------------
   
   // operator<<()
   // Inserts string representation of Dictionary D into stream, as defined by
   // member function to_string().
   friend std::ostream& operator<<( std::ostream& stream, Dictionary& D );

   // operator==()
   // Returns true if and only if Dictionary A equals Dictionary B, as defined
   // by member function equals(). 
   friend bool operator==( const Dictionary& A, const Dictionary& B );

   // operator=()
   // Overwrites the state of this Dictionary with state of D, and returns a
   // reference to this Dictionary.
   Dictionary& operator=( const Dictionary& D );

};


#endif
