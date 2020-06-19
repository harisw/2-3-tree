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
    node* tempL;
    node* tempR;
    node* p;
    node() {
        val = NULL;
        minKey = NULL;
        maxKey = NULL;
        lChild = NULL;
        mChild = NULL;
        rChild = NULL;
        p = NULL;
        tempL = NULL;
        tempR = NULL;
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

    void backupParent(node* x)
    {
        node* parent = x->p;
        if (parent->lChild == x) {
            node* rnNode = new node;
            rnNode->val = x->maxKey;
            parent->tempL = rnNode;
            convert3To2(parent->lChild, "min");
        }
        else if (parent->rChild == x) {
            node* lnNode = new node;
            lnNode->val = x->minKey;
            parent->tempR = lnNode;

            convert3To2(parent->rChild, "max");
        }
        else {
            node* tempL = new node;
            tempL->val = x->minKey;
            node* tempR = new node;
            tempR->val = x->maxKey;
            parent->tempL = tempL;
            parent->tempR = tempR;
        }
        return;
    }

    void split(node* x)
    {
        node* parent = x->p;
        if (root == x) {        //IF THIS IS ROOT
            node* newNode = new node;
            newNode->val = x->val;
            node* newLeft = new node;
            node* newRight = new node;

            //MOVE SMALL&LARGE KEY TO 2 NODES
            newLeft->val = x->minKey;
            newLeft->p = newNode;
            newLeft->lChild = x->lChild;
            if (newLeft->lChild != NULL)
                newLeft->lChild->p = newLeft;

            newRight->val = x->maxKey;
            newRight->p = newNode;
            newRight->rChild = x->rChild;
            if (newRight->rChild != NULL)
                newRight->rChild->p = newRight;

            if (x->tempL != NULL && x->tempR != NULL) {
                newLeft->rChild = x->tempL;
                x->tempL->p = newLeft;
                newRight->lChild = x->tempR;
                x->tempR->p = newRight;
            }
            else if (x->tempL != NULL) {
                newLeft->rChild = x->tempL;
                x->tempL->p = newLeft;
                newRight->lChild = x->mChild;
                x->mChild->p = newRight;
            }
            else if (x->tempR != NULL) {
                newRight->lChild = x->tempR;
                x->tempR->p = newRight;
                newLeft->rChild = x->mChild;
                x->mChild->p = newLeft;
            }
            x->tempL = NULL;
            x->tempR = NULL;

            newNode->rChild = newRight;
            newNode->lChild = newLeft;
            newLeft->p = newNode;
            newRight->p = newNode;
            root = newNode;
        }
        else if (x->tempL != NULL || x->tempR != NULL) {        //IF THIS IS INTERNAL NODE AND HAS BACKUP NODE
            moveKeyUp(x);

            node* newLeft = new node;
            newLeft->val = x->minKey;
            newLeft->lChild = x->lChild;
            newLeft->lChild->p = newLeft;

            node* newRight = new node;
            newRight->val = x->maxKey;
            newRight->rChild = x->rChild;
            newRight->rChild->p = newRight;

            if (x->tempL != NULL && x->tempR != NULL) {
                newLeft->rChild = x->tempL;
                x->tempL->p = newLeft;
                newRight->lChild = x->tempR;
                x->tempR->p = newRight;
            }
            else if (x->tempL != NULL) {
                newLeft->rChild = x->tempL;
                x->tempL->p = newLeft;
                newRight->lChild = x->mChild;
                x->mChild->p = newRight;
            }
            else if (x->tempR != NULL) {
                newRight->lChild = x->tempR;
                x->tempR->p = newRight;
                newLeft->rChild = x->mChild;
                x->mChild->p = newLeft;
            }
            x->tempL = NULL;
            x->tempR = NULL;

            if (parent->val != NULL && parent->minKey != NULL) {           //IF PARENT ALREADY HAVE 3      AND SPLIT HAPPEN IN LEAF
                //BACKUP NODE TO BE SPLIT AT PARENT
                if (parent->lChild == x) {
                    parent->tempL = newRight;
                    parent->lChild = newLeft;
                }
                else if (parent->rChild == x) {
                    parent->tempR = newLeft;
                    parent->rChild = newRight;
                }
                else {
                    parent->tempL = newLeft;
                    parent->tempR = newRight;
                }
                split(parent);
            }
            else{
                if (parent->lChild == x) {
                    parent->lChild = newLeft;
                    parent->mChild = newRight;
                }
                else {
                    parent->mChild = newLeft;
                    parent->rChild = newRight;
                }
                newLeft->p = parent;
                newRight->p = parent;
            }
        }
        else {
            //MOVE MID KEY UP TO PARENT
            moveKeyUp(x);
            node* lnNode = new node;
            lnNode->val = x->minKey;
            node* rnNode = new node;
            rnNode->val = x->maxKey;

            if (parent->val != NULL && parent->minKey != NULL) {           //IF PARENT ALREADY HAVE 3      AND SPLIT HAPPEN IN LEAF
                //BACKUP NODE TO BE SPLIT AT PARENT
                if (parent->lChild == x) {
                    parent->tempL = rnNode;
                    parent->lChild = lnNode;
                    lnNode->p = parent;
                    //convert3To2(parent->lChild, "min");
                }
                else if (parent->rChild == x) {
                    parent->tempR = lnNode;
                    parent->rChild = rnNode;
                    rnNode->p = parent;
                    //convert3To2(parent->rChild, "max");
                }
                else {
                    node* tempL = new node;
                    tempL->val = x->minKey;
                    node* tempR = new node;
                    tempR->val = x->maxKey;
                    parent->tempL = tempL;
                    parent->tempR = tempR;
                    tempL->p = parent;
                    tempR->p = parent;
                }
                split(parent);
            }
            else {
                //IF PARENT IS 2NODE
                if (parent->lChild == x) {
                    parent->lChild = lnNode;
                    parent->mChild = rnNode;
                }
                else if (parent->rChild == x) {
                    parent->mChild = lnNode;
                    parent->rChild = rnNode;
                }
                lnNode->p = parent;
                rnNode->p = parent;
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
        //return parent;
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
            if (grandp->lChild->val == NULL) {       //IF left UNCLE IS 3NODE
                p->val = grandp->minKey;
                node* uncle = grandp->lChild;

                grandp->minKey = uncle->maxKey;
                convert3To2(uncle, "min");

                //STEAL UNCLE CHILD
                p->lChild = uncle->rChild;
                uncle->rChild = uncle->mChild;
                uncle->lChild = NULL;
            }
            else if (grandp->rChild->val == NULL) {     //IF RIGHT UNCLE IS 3NODE
                p->val = grandp->maxKey;
                node* uncle = grandp->rChild;
                grandp->maxKey = uncle->minKey;
                convert3To2(uncle, "max");

                p->rChild = uncle->lChild;
                uncle->lChild = uncle->mChild;
                uncle->mChild = NULL;
            }
            else {                              //MERGE LEFT UNCLE AND P
                p->val = grandp->minKey;
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
            res = res->lChild;
            while (res->lChild != NULL && res->rChild != NULL)
                res = res->rChild;
        }
        else {
            res = res->rChild;
            while (res->lChild != NULL && res->rChild != NULL)
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
            }
        }
    }
    void deletion(int key, node* target = NULL)
    {
        node* x = new node;
        if (target == NULL)
            x = search(root, key);
        else
            x = target;
        if (x == NULL) {
            cout << key << " : Key NOT FOUND" << endl;
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
                    deleteKeyFrom2Node(x);
                    getSuccessor(x);
                }
                else {
                    node* p = x->p;
                    node* newChild = new node;
                    newChild->minKey = x->lChild->val;
                    newChild->maxKey = x->rChild->val;
                    if (p->lChild == x) {
                        /*deleteKeyFrom2Node(x);*/
                        x->lChild = newChild;
                        getPredeccessor(x);
                    }
                    else if (p->mChild == x) {
                        //deleteKeyFrom2Node(x);
                        if (p->lChild->val == NULL || p->rChild->val == NULL) {   //IF LEFT SIBLING IS 3NODE OR RIGHT SIBLING IS 2NODE
                            x->rChild = newChild;
                        }
                        else {
                            x->lChild = newChild;
                        }
                        getPredeccessor(x);
                    }
                    else {
                        //deleteKeyFrom2Node(x);
                        x->rChild = newChild;
                        getPredeccessor(x);
                    }
                }
            }
            else {
                deleteKeyFrom3Node(x, key);
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
                deletion(successor->maxKey, successor);     //DELETE SUCCESSOR FROM LEAF
            }
            else {
                node* successor = getInnerEdgeLeaf("right");
                x->maxKey = successor->minKey;
                deletion(successor->minKey, successor);
            }
        }
        if(target == NULL)
            cout << key << " : Key DELETE SUCCESSFUL" << endl;
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
    int size = 0;
    while (getline(in, line))
    {
        tree.insert(stoi(line));
        size++;
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
    in_delete.close();

    cout << endl << endl << "######################################" << endl;
    cout << "search again" << endl;
    ifstream other_search("search-15.txt");
    res = 0;
    while (getline(other_search, line))
    {
        res = tree.search(tree.root, stoi(line));
        if (res)
            cout << line << " FOUND!!" << endl;
        else
            cout << line << "  NOT FOUND" << endl;
    }
    other_search.close();
}