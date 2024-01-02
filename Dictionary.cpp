//-----------------------------------------------------------------------------
// Name: Lia Cui
// CRUZID: ljcui
// Assignment Name: PA7
//
//-----------------------------------------------------------------------------

    #include<iostream>
    #include<string>
    #include<stdlib.h>

    #include "Dictionary.h"


    Dictionary::Node::Node(keyType k, valType v){
        key = k;
        val = v;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }


    // Class Constructors & Destructors ----------------------------------------
    
    // Creates new Dictionary in the empty state. 
    Dictionary::Dictionary(){
        //nil = new Node("NIL", -1);
        nil = nullptr;
        root = nullptr;
        current = nullptr;
        num_pairs = 0;
    }

    // Copy constructor.
    Dictionary::Dictionary(const Dictionary& D){
//        nil = new Node("NIL", -1);
        nil = nullptr;
        root = nullptr;
        num_pairs = 0;
        current = nullptr;

        preOrderCopy(D.root, nullptr);
    }

    // Destructor
    Dictionary::~Dictionary(){
        clear();
        delete nil;
        nil = nullptr;
        current = nullptr;
        root = nullptr;
    }


    // Access functions --------------------------------------------------------

    // size()
    // Returns the size of this Dictionary.
    int Dictionary::size() const{
        return num_pairs;
    }


    // contains()
    // Returns true if there exists a pair such that key==k, and returns false
    // otherwise.
    bool Dictionary::contains(keyType k) const{
        Node* N = findKeyhelper(k, root);
        if (N != NULL){
            return true;
        }else{
            return false;
        }
    }

    // getValue()
    // Returns a reference to the value corresponding to key k.
    // Pre: contains(k)
    valType& Dictionary::getValue(keyType k) const{
        if(contains(k) == false){
        throw std::logic_error("Dictionary: getValue(): key " + k + " does not exist");
        }
        Node* A = search(this->root, k);
        return A->val;
    }

    // hasCurrent()
    // Returns true if the current iterator is defined, and returns false 
    // otherwise.
    bool Dictionary::hasCurrent() const{
        if(current == nil || current == nullptr){
            return false;
        }else{
            return true;
        }
    }

    // currentKey()
    // Returns the current key.
    // Pre: hasCurrent() 
    keyType Dictionary::currentKey() const{
        if(hasCurrent() == false ){
        throw std::logic_error("Dictionary: currentKey(): current undefined");
        }
        return current->key;
    }

    // currentVal()
    // Returns a reference to the current value.
    // Pre: hasCurrent()
    valType& Dictionary::currentVal() const{
        if(hasCurrent() == false ){
        throw std::logic_error("Dictionary: currentVal(): current undefined");
        }
        return current->val;
    }


    // Manipulation procedures -------------------------------------------------

    // clear()
    // Resets this Dictionary to the empty state, containing no pairs.
    void Dictionary::clear(){
        postOrderDelete(root);
        current = nil;
    }

    // setValue()
    // If a pair with key==k exists, overwrites the corresponding value with v, 
    // otherwise inserts the new pair (k, v).
    void Dictionary::setValue(keyType k, valType v){
        if(root == nullptr){
            Node* N = new Node(k, v);
            root = N;
        }else{
            Node* pos = root;
            Node* par = nullptr;
            while(pos != nullptr){
                par = pos;
                if(k.compare(pos->key) < 0){
                    pos = pos->left;
                }else if(k.compare(pos->key) > 0){
                    pos = pos->right;
                }else{
                    pos->val = v;
                    return;
                }
            }

            if(k.compare(par->key) < 0){
                Node* N = new Node(k, v);
                par->left = N;
                N->parent = par;
            }else if(k.compare(par->key) > 0){
                Node* N = new Node(k, v);
                par->right = N;
                N->parent = par;
            }else{
                par->val = v;
                return;
            }
        }
        num_pairs++;
        
    }

    // remove()
    // Deletes the pair for which key==k. If that pair is current, then current
    // becomes undefined.
    // Pre: contains(k).
    void Dictionary::remove(keyType k){
        Node* Z = search(root, k);
        if(Z == nullptr){
        throw std::logic_error("Dictionary: remove(): key " + k + " does not exist");
        }
        if(Z->left == nullptr || Z->left == nil){
            transplant(Z, Z->right);
        }else if(Z->right == nullptr || Z->right == nil){
            transplant(Z, Z->left);
        }else{
            Node* Y = findMin(Z->right);
            if(Y->parent != Z){
                transplant(Y, Y->right);
                Y->right = Z->right;
                Y->right->parent = Y;
            }
            transplant(Z, Y);
            Y->left = Z->left;
            Y->left->parent = Y;
        }
        if(Z == current){
            current = nil;
        }
        if(Z->parent != nullptr && Z->parent->right == Z){
            Z->parent->right = nullptr;
        }else if(Z->parent != nullptr && Z->parent->left == Z){
            Z->parent->left = nullptr;
        }
        delete Z;
        num_pairs--;
    }

    // begin()
    // If non-empty, places current iterator at the first (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::begin(){
        if(num_pairs > 0){
            current = root;
            while(current != nullptr && current->left != nullptr){
                current = current->left;
            }
        }
    }

    // end()
    // If non-empty, places current iterator at the last (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::end(){
        if(num_pairs > 0){
            current = root;
            while(current != nullptr && current->right != nullptr){
                current = current->right;
            }
        }
    }

    // next()
    // If the current iterator is not at the last pair, advances current 
    // to the next pair (as defined by the order operator < on keys). If 
    // the current iterator is at the last pair, makes current undefined.
    // Pre: hasCurrent()
    void Dictionary::next(){
        if(hasCurrent() == false ){
        throw std::logic_error("Dictionary: next(): current undefined");
        }
        current = findNext(current);
    }

    // prev()
    // If the current iterator is not at the first pair, moves current to  
    // the previous pair (as defined by the order operator < on keys). If 
    // the current iterator is at the first pair, makes current undefined.
    // Pre: hasCurrent()
    void Dictionary::prev(){
        if(hasCurrent() == false ){
        throw std::logic_error("Dictionary: prev(): current undefined");
        }
        current = findPrev(current);
    }


    // Other Functions ---------------------------------------------------------

    // to_string()
    // Returns a string representation of this Dictionary. Consecutive (key, value)
    // pairs are separated by a newline "\n" character, and the items key and value 
    // are separated by the sequence space-colon-space " : ". The pairs are arranged 
    // in order, as defined by the order operator <.
    std::string Dictionary::to_string() const{
        //Dictionary D = *this;
        return inOrderString(root);
    }

    // pre_string()
    // Returns a string consisting of all keys in this Dictionary. Consecutive
    // keys are separated by newline "\n" characters. The key order is given
    // by a pre-order tree walk.
    std::string Dictionary::pre_string() const{
        //Dictionary D = *this;
        return preOrderString(root);
    }

    // equals()
    // Returns true if and only if this Dictionary contains the same (key, value)
    // pairs as Dictionary D.
    bool Dictionary::equals(const Dictionary& D) const{
        return to_string() == D.to_string();
    }


    // Overloaded Operators ----------------------------------------------------
    
    // operator<<()
    // Inserts string representation of Dictionary D into stream, as defined by
    // member function to_string().
    std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
        return stream << D.to_string();
    }

    // operator==()
    // Returns true if and only if Dictionary A equals Dictionary B, as defined
    // by member function equals(). 
    bool operator==( const Dictionary& A, const Dictionary& B ){
        return A.equals(B);
    }

    // operator=()
    // Overwrites the state of this Dictionary with state of D, and returns a
    // reference to this Dictionary.
    Dictionary& Dictionary::operator=( const Dictionary& D ){
        if(this != &D){
            Dictionary temp = D;
            std::swap(nil, temp.nil);
            std::swap(root, temp.root);
            std::swap(current, temp.current);
            std::swap(num_pairs, temp.num_pairs);
        }

        return *this;
    }


