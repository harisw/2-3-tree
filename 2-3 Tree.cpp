// DynamicBSTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct node {
    int minKey;
    int val;
    int maxKey;
    node* lChild;
    node* mChild;
    node* rChild;
    node* p;
    node() {
        val = NULL;
        minKey = NULL;
        maxKey = NULL;
        lChild = NULL;
        mChild = NULL;
        rChild = NULL;
        p = NULL;
    }
};

struct twothreeTree {
    node* root;
    twothreeTree() 
    {
        root = NULL;
    }
    void  insert(int inp)
    {
        if (root == NULL) {
            node* newNode = new node;
            newNode->val = inp;
            root = newNode;
            return;
        }
        node* x = root;
        node* y = x;
        while (1) {
            if (x->lChild == NULL && x->rChild == NULL)
                break;

            y = x;
            
            if (x->val != NULL) {
                if (inp < x->val)
                    x = x->lChild;
                else
                    x = x->rChild;
            }
            else if (x->minKey != NULL || x->maxKey != NULL) {
                if (inp < x->minKey)
                    x = x->lChild;
                else if (inp > x->maxKey)
                    x = x->rChild;
                else
                    x = x->mChild;
            }
        }
        
        if (x->val != NULL) {       //IF THE LEAF IS 2-NODE
            if (inp < x->val) {
                x->minKey = inp;
                x->maxKey = x->val;
            }
            else {
                x->maxKey = inp;
                x->minKey = x->val;
            }
            x->val = NULL;
        }
        else {                      //IF THE LEAF IS 3 NODE
            if (inp < x->minKey) {
                x->val = x->minKey;
                x->minKey = inp;
            }
            else if (inp > x->minKey && inp < x->maxKey) {
                x->val = inp;
            }
            else {
                x->val = x->maxKey;
                x->maxKey = inp;
            }
            split(x);
        }
    }
    void split(node* x, int pos = 0)
    {
        node* parent = x->p;
        if (parent == NULL) {        //IF THIS IS ROOT
            node* newNode = new node;
            newNode->val = x->val;
            node* lnNode = new node;
            node* rnNode = new node;

            if (x->mChild != NULL && x->mChild->val == NULL) {
                node* leftGrandChild = new node;
                leftGrandChild->val = x->mChild->minKey;
                lnNode->rChild = leftGrandChild;
                lnNode->lChild = x->lChild;
                lnNode->lChild->p = lnNode;

                node* rightGrandChild = new node;
                rightGrandChild->val = x->mChild->maxKey;
                rnNode->lChild = rightGrandChild;
                rnNode->rChild = x->rChild;
                rnNode->rChild->p = rnNode;
            }
            //MOVE SMALL&LARGE KEY TO 2 NODES

            lnNode->val = x->minKey;
            lnNode->p = newNode;
            newNode->lChild = lnNode;


            rnNode->val = x->maxKey;
            rnNode->p = newNode;
            newNode->rChild = rnNode;
            root = newNode;
        }
        else {
            //MOVE MID KEY UP TO PARENT
            moveKeyUp(x);
            node* lnNode = new node;
            node* rnNode = new node;

            if (x->mChild != NULL && x->mChild->val == NULL) {
                node* leftGrandChild = new node;
                leftGrandChild->val = x->mChild->minKey;
                lnNode->rChild = leftGrandChild;
                lnNode->lChild = x->lChild;
                lnNode->lChild->p = lnNode;

                node* rightGrandChild = new node;
                rightGrandChild->val = x->mChild->maxKey;
                rnNode->lChild = rightGrandChild;
                rnNode->rChild = x->rChild;
                rnNode->rChild->p = rnNode;
            }
            ////MOVE SMALL&LARGE KEY TO 2 NODES
            //lnNode->val = x->minKey;
            //lnNode->p = parent;

            //rnNode->val = x->maxKey;
            //rnNode->p = parent;

            x->val = NULL;
            if (parent->lChild == x) {
                //MOVE SMALL&LARGE KEY TO 2 NODES
                lnNode->val = x->minKey;
                lnNode->p = parent;

                rnNode->val = x->maxKey;
                rnNode->p = parent;

                parent->lChild = lnNode;
                if(parent->mChild == NULL)
                    parent->mChild = rnNode;
                else {
                    parent->mChild->minKey = rnNode->val;
                    parent->mChild->maxKey = parent->mChild->val;
                    parent->mChild->val = NULL;
                    split(parent);
                }
            }
            else if(parent->rChild == x){
                //MOVE SMALL&LARGE KEY TO 2 NODES
                lnNode->val = x->minKey;
                lnNode->p = parent;

                rnNode->val = x->maxKey;
                rnNode->p = parent;
                parent->rChild = rnNode;
                if(parent->mChild == NULL)
                    parent->mChild = lnNode;
                else {
                    parent->mChild->maxKey = lnNode->val;
                    parent->mChild->minKey = parent->mChild->val;
                    parent->mChild->val = NULL;
                    split(parent);
                }
            }
            else {
                split(parent);
            }
        }
    }
    void moveKeyUp(node* x)
    {
        int midKey = x->val;

        node* parent = x->p;
        if (parent->val != NULL) {
            if (midKey < parent->val) {
                parent->minKey = midKey;
                parent->maxKey = parent->val;
            }
            else {
                parent->maxKey = midKey;
                parent->minKey = parent->val;
            }
            parent->val = NULL;
        }
        else {
            if (midKey < parent->minKey) {
                parent->val = parent->minKey;
                parent->minKey = midKey;
            }
            else if (midKey > parent->maxKey) {
                parent->val = parent->maxKey;
                parent->maxKey = midKey;
            }
            else {
                parent->val = midKey;
            }
        }

    }

    node* search(node* n, int target)
    {
        if (n == NULL)
            return 0;

        //if (n->lChild == NULL && n->rChild == NULL) {
        //}
        if (n->val != NULL) {
            if (target == n->val)
                return n;
            else if (target < n->val)
                return search(n->lChild, target);
            else
                return search(n->rChild, target);
        }
        else {
            if (n->minKey == target || n->maxKey == target)
                return n;

            if (target < n->minKey)
                return search(n->lChild, target);
            else if (target > n->maxKey)
                return search(n->rChild, target);
            else
                return search(n->mChild, target);
        }
    }

    void convert3To2(node* x, string side)      //HELPER FUNCTION TO CONVERT 3NODE TO 2NODE
    {
        if (side == "min") {
            x->val = x->minKey;
            x->minKey = NULL;
            x->maxKey = NULL;
        }
        else {
            x->val = x->maxKey;
            x->minKey = NULL;
            x->maxKey = NULL;
        }
    }
    void deleteKeyFrom3Node(node* x, int target)
    {
        if (x->minKey == target) {
            convert3To2(x, "max");
        }
        else {
            convert3To2(x, "min");
        }
    }
    void deleteKeyFrom2Node(node* x)
    {
        node* p = x->p;
        if (p->lChild == x) {
            p->lChild = NULL;
        } else if (p->mChild == x) {
            p->mChild = NULL;
        }
        else {
            p->rChild = NULL;
        }
    }
    void dragChild(node* p, string direction)
    {
        if (direction == "left") {
            p->lChild = p->mChild;
            p->mChild = p->rChild;
        }
        else {
            p->mChild = p->lChild;
            p->rChild = p->mChild;
        }
    }
    void getPredeccessor(node* p)
    {
        //MOVE GRANDP AS P
        node* grandp = p->p;
        if (grandp->lChild == p) {
            p->val = grandp->minKey;

            node* uncle = grandp->mChild;
            if (uncle->val == NULL) {       //IF UNCLE IS 2NODE
                grandp->minKey = uncle->minKey;
                convert3To2(uncle, "max");
            
                //STEAL UNCLE CHILD
                p->rChild = uncle->lChild;
                uncle->lChild = uncle->mChild;
                uncle->mChild = NULL;
            }
            else {
                //MERGE UNCLE AND PARENT
                p->minKey = p->val;
                p->val = NULL;
                p->maxKey = uncle->val;

                p->mChild = uncle->lChild;
                p->rChild = uncle->rChild;
                p->mChild->p = p;
                p->rChild->p = p;
                grandp->mChild = NULL;
            }
        }
        else if (grandp->mChild == p) {
            p->val = grandp->minKey;
            
            if (grandp->lChild->val == NULL) {       //IF left UNCLE IS 3NODE
                node* uncle = grandp->lChild;

                grandp->minKey = uncle->maxKey;
                convert3To2(uncle, "min");

                //STEAL UNCLE CHILD
                p->lChild = uncle->rChild;
                uncle->rChild = uncle->mChild;
                uncle->lChild = NULL;
            }
            else if (grandp->rChild->val == NULL) {     //IF RIGHT UNCLE IS 3NODE
                node* uncle = grandp->rChild;

                grandp->maxKey = uncle->minKey;
                convert3To2(uncle, "max");

                p->rChild = uncle->lChild;
                uncle->lChild = uncle->mChild;
                uncle->mChild = NULL;
            }
            else {                              //MERGE LEFT UNCLE AND P
                node* uncle = grandp->lChild;

                p->maxKey = p->val;
                p->val = NULL;
                p->minKey = uncle->val;

                p->mChild = uncle->rChild;
                p->lChild = uncle->lChild;
                p->mChild->p = p;
                p->lChild->p = p;
                grandp->lChild = p;
                grandp->mChild = NULL;
            }
        }
        else {
            p->val = grandp->maxKey;

            node* uncle = grandp->mChild;
            if (uncle->val == NULL) {       //IF UNCLE IS 2NODE
                grandp->maxKey = uncle->maxKey;
                convert3To2(uncle, "min");

                //STEAL UNCLE CHILD
                p->lChild = uncle->rChild;
                uncle->rChild = uncle->mChild;
                uncle->mChild = NULL;
            }
            else {
                //MERGE UNCLE AND PARENT
                p->maxKey = p->val;
                p->val = NULL;
                p->minKey = uncle->val;

                p->mChild = uncle->rChild;
                p->lChild = uncle->lChild;
                p->mChild->p = p;
                p->lChild->p = p;
                grandp->mChild = NULL;
            }
        }
    }
    void getSuccessor(node* x)
    {
        if (x->val != NULL) {
            if (x->lChild->val == NULL) {
                x->val = x->lChild->maxKey;
                convert3To2(x, "min");
            }
            else {
                x->val = x->rChild->minKey;
                convert3To2(x, "max");
            }
        }
    }
    node* getInnerEdgeLeaf(string direction)
    {
        node* res = root;
        if (direction == "left") {
            while (res->lChild == NULL && res->rChild == NULL)
                res = res->rChild;
        }
        else {
            while (res->lChild == NULL && res->rChild == NULL)
                res = res->lChild;
        }
        return res;
    }
    void getSuccessor3Node(node* x)
    {
        if (x->minKey == NULL) {
            if (x->lChild->val == NULL) {
                x->minKey = x->lChild->maxKey;
                convert3To2(x->lChild, "min");
            }
            else if (x->mChild->val == NULL) {
                x->minKey = x->mChild->minKey;
                convert3To2(x->mChild, "max");
            }
            else {      //MERGE L&M CHILD
                x->lChild->minKey = x->lChild->val;
                x->lChild->maxKey = x->mChild->val;
                x->lChild->val = NULL;
                x->mChild = NULL;
                convert3To2(x, "max");
            }
        }
        else {
            if (x->mChild->val == NULL) {
                x->maxKey = x->mChild->maxKey;
                convert3To2(x->mChild, "min");
            }
            else if (x->rChild->val == NULL) {
                x->maxKey = x->rChild->minKey;
                convert3To2(x->rChild, "max");
            }
            else {
                x->rChild->minKey = x->mChild->val;
                x->rChild->maxKey = x->rChild->val;
                x->rChild->val = NULL;
                x->mChild = NULL;
                convert3To2(x, "min");
            }
        }
    }
    void deletion(int key, node* target = NULL)
    {
        node* x = target == NULL ? search(root, key) : target;
        if (x == NULL) {
            cout << "Key NOT FOUND" << endl;
            return;
        }

        if (x->lChild == NULL && x->rChild == NULL) {
            if (x->val == NULL) {
                deleteKeyFrom3Node(x, key);
            }
            else {
                deleteKeyFrom2Node(x);
                node* p = x->p;
                if (p->val == NULL) {       //IF PARENT IS 3NODE
                    if (p->lChild == NULL) {    //IF DELETE LCHILD
                        if (p->mChild->val == NULL) {
                            node* mChild = p->mChild;
                            node* newX = new node;
                            newX->val = p->minKey;
                            p->lChild = newX;

                            p->minKey = mChild->minKey;
                            convert3To2(mChild, "max");
                        }
                        else {
                            node* newX = new node;
                            newX->minKey = p->minKey;
                            newX->maxKey = p->mChild->val;
                            p->lChild = newX;
                            p->mChild = NULL;
                            convert3To2(p, "max");
                        }
                    }
                    else if (p->mChild == NULL) {       //IF DELETE MCHILD
                        if (p->lChild->val == NULL) {
                            node* lChild = p->lChild;
                            node* newX = new node;
                            newX->val = p->minKey;
                            p->mChild = newX;

                            p->minKey = lChild->maxKey;
                            convert3To2(lChild, "min");
                        }
                        else if (p->rChild->val == NULL) {
                            node* rChild = p->rChild;
                            node* newX = new node;
                            newX->val = p->maxKey;
                            p->mChild = newX;

                            p->maxKey = rChild->minKey;
                            convert3To2(rChild, "max");
                        }
                        else {      //MERGE MID AND RIGHT
                            node* newX = new node;
                            newX->minKey = p->maxKey;
                            newX->maxKey = p->rChild->val;
                            p->rChild = newX;
                            p->mChild = NULL;
                            convert3To2(p, "min");
                        }
                    }
                    else {  //IF DELETE RCHILD
                        if (p->mChild->val == NULL) {
                            node* mChild = p->mChild;
                            node* newX = new node;
                            newX->val = p->maxKey;
                            p->rChild = newX;

                            p->maxKey = mChild->maxKey;
                            convert3To2(mChild, "min");
                        }
                        else {
                            node* newX = new node;
                            newX->maxKey = p->maxKey;
                            newX->minKey = p->mChild->val;
                            p->rChild = newX;
                            p->mChild = NULL;
                            convert3To2(p, "min");
                        }
                    }
                }


                else {          //IF PARENT IS 2NODE  
                    if (p->lChild == NULL) {     //IF X IS LEFT
                        node* sibling = p->rChild;
                        //IF SIBLING IS 3NODE
                        if (sibling->val == NULL) {
                            node* newX = new node;
                            newX->val = p->val;
                            p->lChild = newX;
                            p->val = sibling->minKey;
                            convert3To2(p->rChild, "max");
                        }
                        else {
                            //MERGE P AND RCHILD AS NEW LCHILD
                            node* newLChild = new node;
                            newLChild->minKey = p->val;
                            newLChild->maxKey = p->rChild->val;
                            p->lChild = newLChild;

                            getPredeccessor(p);
                        }
                    }
                    else { //IF X IS RIGHT
                        node* sibling = p->lChild;
                        //IF SIBLING IS 3NODE
                        if (sibling->val == NULL) {
                            node* newX = new node;
                            newX->val = p->val;
                            p->rChild = newX;
                            p->val = sibling->maxKey;
                            convert3To2(p->lChild, "min");
                        }
                        else {
                            node* newChild = new node;
                            newChild->minKey = p->lChild->val;
                            newChild->maxKey = p->val;
                            if (p->p->lChild->val == NULL || p->p->rChild->val != NULL) {   //IF LEFT UNCLE IS 3NODE OR RIGHT UNCLE IS 2NODE
                                p->rChild = newChild;
                            }
                            else {
                                p->lChild = newChild;
                            }
                            getPredeccessor(p);
                        }
                    }
                }
            }
        } else if (root != x) {     //IF X IS INTERNAL NODE
            if (x->val != NULL) {
                if (x->lChild->val == NULL || x->rChild->val == NULL) {     //IF ONE OF CHILD IS 3NODE
                    getSuccessor(x);
                }
                else {
                    node* p = x->p;
                    node* newChild = new node;
                    newChild->minKey = x->lChild->val;
                    newChild->maxKey = x->rChild->val;
                    if (p->lChild == x) {
                        x->lChild = newChild;
                        getPredeccessor(x);
                    }
                    else if (p->mChild == p) {
                        if (p->lChild->val == NULL || p->rChild->val != NULL) {   //IF LEFT SIBLING IS 3NODE OR RIGHT SIBLING IS 2NODE
                            x->rChild = newChild;
                        }
                        else {
                            x->lChild = newChild;
                        }
                        getPredeccessor(x);
                    }
                    else {
                        x->rChild = newChild;
                        getPredeccessor(x);
                    }
                }
            }
            else {
                getSuccessor3Node(x);
            }
        }
        else {      //IF X IS ROOT
            if (x->minKey == key || x->val != NULL) {
                node* successor = getInnerEdgeLeaf("left");
                if (x->val != NULL)
                    x->val = successor->maxKey;
                else
                    x->minKey = successor->maxKey;
                deletion(0, successor);
            }
            else {
                node* successor = getInnerEdgeLeaf("right");
                x->maxKey = successor->minKey;
                deletion(0, successor);
            }
        }
        cout << "DELETE SUCCESSFUL" << endl;
    }
};

int main()
{
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    double totalDur = 0;

    string line;
    int inp;
    ifstream in("input-15.txt");

    twothreeTree tree;

    while (getline(in, line))
    {
        tree.insert(stoi(line));
    }
    in.close();

    ifstream in_search("search-15.txt");
    bool res = 0;
    while (getline(in_search, line))
    {
        res = tree.search(tree.root, stoi(line));
        if (res)
            cout << line << " FOUND!!" << endl;
        else
            cout << line << "  NOT FOUND" << endl;
    }
    in_search.close();

    ifstream in_delete("delete-4.txt");
    while (getline(in_delete, line))
    {
        tree.deletion(stoi(line));
    }
    in_search.close();
}