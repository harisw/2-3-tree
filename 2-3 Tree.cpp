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
        if (p->lChild = x) {
            p->lChild = NULL;
        } else if (p->mChild = x) {
            p->mChild = NULL;
        }
        else {
            p->rChild = NULL;
        }
    }
    void deletion(int key)
    {
        node* x = search(root, key);
        if (x == NULL) {
            cout << "Key NOT FOUND" << endl;
            return;
        }

        if (x->lChild == NULL && x->rChild == NULL) {
            if (x->val == NULL) {
                deleteKeyFrom3Node(x, key);
                return;
            }
            else {
                deleteKeyFrom2Node(x);
                node* p = x->p;
                if (p->val == NULL) {
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
            }
        }
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
}