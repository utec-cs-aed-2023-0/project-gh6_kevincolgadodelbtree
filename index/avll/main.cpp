#include <iostream>
#include "avl.h"
#include "tester.h"
using namespace std;

int main(int argc, char const *argv[])
{
    AVLTree<int> *avl = new AVLTree<int>();

    avl->insert(35);
    avl->insert(30);
    avl->insert(27);
    avl->insert(11);
    avl->insert(16);
    avl->insert(100);
    avl->insert(50);
    avl->insert(91);
    avl->insert(73);
    avl->insert(5);
    avl->printBT();
    ASSERT(avl->isBalanced() == true, "The avl is not working");

    ASSERT(avl->height() == 3, "The avl is not working");
    ASSERT(avl->getPreOrder() == "30 16 11 5 27 50 35 91 73 100 ", "The avl is not working");

    avl->remove(30);

    avl->remove(35);

    avl->remove(100);


    ASSERT(avl->height() == 2, "The avl is not working");

    ASSERT(avl->getPreOrder() == "27 11 5 16 73 50 91 ", "The avl is not working");

    return 0;

}
