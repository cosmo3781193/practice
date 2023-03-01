#include <iostream>
#include <deque>
#include <string>
#include <vector>

using Color = bool;
using KEY_TYPE = int;
using VALUE_TYPE = int;

static const Color red = false;
static const Color black = true;

class RBtree_node
{
public:
    Color color;
    RBtree_node* parent;
    RBtree_node* left;
    RBtree_node* right;

    KEY_TYPE key;
    VALUE_TYPE value;
    RBtree_node(Color color_) :color(color_), parent(nullptr), left(nullptr), right(nullptr), key(-99999) {}
    RBtree_node(Color color_, KEY_TYPE key_, RBtree_node* nil) :
        color(color_), parent(nil), left(nil), right(nil), key(key_) {}
};

class RBtree
{
private:
    RBtree_node* root;
    RBtree_node* nil;
public:
    RBtree()
    {
        nil = new RBtree_node(black);
        root = nil;
    }
    void leftRotate(RBtree_node* left_node);
    void rightRotate(RBtree_node* right_node);
    void insertNode(KEY_TYPE key);
    void fixInsert(RBtree_node* node);
    RBtree_node* searchNode(KEY_TYPE key);
    RBtree_node* successor(RBtree_node* node);
    void deleteNode(KEY_TYPE key);
    void fixDelete(RBtree_node* node);
    void print();
    void printMiddle(RBtree_node* node);
};

void RBtree::leftRotate(RBtree_node* left_node)
{
    RBtree_node* right_node = left_node->right;
    left_node->right = right_node->left;
    if (right_node->left != nil)
    {
        left_node->right->parent = left_node;
    }
    right_node->parent = left_node->parent;
    if (left_node == root)
    {
        root = right_node;
    }
    else if (left_node == left_node->parent->left)
    {
        left_node->parent->left = right_node;
    }
    else
    {
        left_node->parent->right = right_node;
    }
    left_node->parent = right_node;
    right_node->left = left_node;
}
void RBtree::rightRotate(RBtree_node* right_node)
{
    RBtree_node* left_node = right_node->left;
    right_node->left = left_node->right;
    if (left_node->right != nil)
    {
        right_node->left->parent = right_node;
    }
    left_node->parent = right_node->parent;
    if (right_node == root)
    {
        root = left_node;
    }
    else if (right_node == right_node->parent->right)
    {
        right_node->parent->right = left_node;
    }
    else
    {
        right_node->parent->left = left_node;
    }
    right_node->parent = left_node;
    left_node->right = right_node;
}

void RBtree::insertNode(KEY_TYPE key)
{
    RBtree_node* prev = nil;
    RBtree_node* cur = root;
    while (cur != nil)
    {
        prev = cur;
        if (key > cur->key)
        {
            cur = cur->right;
        }
        else if (key < cur->key)
        {
            cur = cur->left;
        }
        else
        {
            return;
        }
    }

    RBtree_node* new_node = new RBtree_node(red, key, nil);
    new_node->parent = prev;
    if (prev == nil)
    {
        root = new_node;
    }
    else if (key < prev->key)
    {
        prev->left = new_node;
    }
    else
    {
        prev->right = new_node;
    }
    fixInsert(new_node);
    print();
}

void RBtree::fixInsert(RBtree_node* new_node)
{
    while (new_node->parent->color == red)
    {
        if (new_node->parent == new_node->parent->parent->left)
        {
            RBtree_node* uncle = new_node->parent->parent->right;
            if (uncle->color == red)
            {
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                new_node = new_node->parent->parent;
            }
            else
            {
                if (new_node == new_node->parent->right)
                {
                    new_node = new_node->parent;
                    leftRotate(new_node);
                }

                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                rightRotate(new_node->parent->parent);
            }
        }
        else
        {
            RBtree_node* uncle = new_node->parent->parent->left;
            if (uncle->color == red)
            {
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                new_node = new_node->parent->parent;
            }
            else {
                if (new_node == new_node->parent->left)
                {
                    new_node = new_node->parent;
                    rightRotate(new_node);
                }
                new_node->parent->color = black;
                new_node->parent->parent->color = red;
                leftRotate(new_node->parent->parent);
            }
        }
    }
    root->color = black;
}
RBtree_node* RBtree::searchNode(KEY_TYPE key)
{
    RBtree_node* cur = root;
    while (cur != nil)
    {
        if (key > cur->key)
        {
            cur = cur->right;
        }
        else if (key < cur->key)
        {
            cur = cur->left;
        }
        else
        {
            return cur;
        }
    }
    return cur;
}
RBtree_node* RBtree::successor(RBtree_node* node)
{
    if (node->right != nil)
    {
        RBtree_node* res = node->right;
        while (res->left != nil)
        {
            res = res->left;
        }
        return res;
    }
    else
    {
        while (node != root && node != node->parent->left)
        {
            node = node->parent;
        }
        return node->parent;
    }
}
void RBtree::deleteNode(KEY_TYPE key)
{
    RBtree_node* key_node = searchNode(key);
    RBtree_node* delete_node;
    RBtree_node* delete_son;
    if (key_node->left != nil && key_node->right != nil)
    {
        delete_node = successor(key_node);
        delete_son = delete_node->right;
    }
    else
    {
        delete_node = key_node;
        if (key_node->left != nil)
        {
            delete_son = key_node->left;
        }
        else
        {
            delete_son = key_node->right;
        }
    }
    delete_son->parent = delete_node->parent;
    if (delete_node == root)
    {
        root = delete_son;
    }
    else if (delete_node == delete_node->parent->left)
    {
        delete_node->parent->left = delete_son;
    }
    else
    {
        delete_node->parent->right = delete_son;
    }
    key_node->key = delete_node->key;
    key_node->value = delete_node->value;
    if (delete_node->color == black)
    {
        fixDelete(delete_son);
    }
    delete delete_node;
    print();
}

void RBtree::fixDelete(RBtree_node* delete_son)
{
    while (delete_son != root && delete_son->color == black)
    {
        if (delete_son == delete_son->parent->left)
        {
            RBtree_node* bro = delete_son->parent->right;
            if (bro->color == red)
            {
                bro->color = black;
                delete_son->parent->color = red;
                leftRotate(delete_son->parent);
                bro = delete_son->parent->right;
            }
            if (bro->left->color == black && bro->right->color == black)
            {
                bro->color = red;
                delete_son = delete_son->parent;
            }
            else
            {
                if (bro->right->color == black)
                {
                    bro->color = red;
                    bro->left->color = black;
                    rightRotate(bro);
                }
                bro->color = bro->parent->color;
                bro->parent->color = black;
                bro->right->color = black;
                leftRotate(delete_son->parent);
                delete_son = root;
            }
        }
        else
        {
            RBtree_node* bro = delete_son->parent->left;
            if (bro->color == red)
            {
                bro->color = black;
                delete_son->parent->color = red;
                rightRotate(delete_son->parent);
                bro = delete_son->parent->left;
            }
            if (bro->right->color == black && bro->left->color == black)
            {
                bro->color = red;
                delete_son = delete_son->parent;
            }
            else {
                if (bro->left->color == black)
                {
                    bro->color = red;
                    bro->right->color = black;
                    leftRotate(bro);
                }
                bro->color = bro->parent->color;
                bro->parent->color = black;
                bro->left->color = black;
                rightRotate(delete_son->parent);
                delete_son = root;
            }
        }
    }
    delete_son->color = black;
}
void RBtree::print()
{
    std::deque<RBtree_node*> dqueue;
    dqueue.push_back(root);
    while (!dqueue.empty())
    {
        int size = (int)dqueue.size();
        for (int i = 0; i < size; ++i)
        {
            RBtree_node* temp = dqueue.front();
            dqueue.pop_front();
            if (temp->left != nullptr)
            {
                dqueue.push_back(temp->left);
            }
            if (temp->right != nullptr)
            {
                dqueue.push_back(temp->right);
            }
            std::string color = temp->color ? "b: " : "r: ";
            std::string keystr = temp == nil ? "nil" : std::to_string(temp->key);
            std::cout << color << keystr << "  ";
        }
        std::cout << std::endl;
    }
}
void RBtree::printMiddle(RBtree_node* node)
{
    if (node == nil)
    {
        return;
    }
    printMiddle(node->left);
    std::string color = node->color ? "b:" : "r:";
    std::cout << color << std::to_string(node->key) << "  ";
    printMiddle(node->right);
}

int main() {
    RBtree rb;
    std::string select;
    KEY_TYPE key;
    while (true) {
        std::cout << "\n输入操作：i：插入key，d：删除key q：退出" << std::endl;
        std::cin >> select;
        if (select == "i")
        {
            std::cout << "输入key" << std::endl;
            std::cin >> key;
            rb.insertNode(key);
        }
        else if (select == "d")
        {
            std::cout << "输入key" << std::endl;
            std::cin >> key;
            rb.deleteNode(key);
        }
        else if (select == "q")
        {
            break;
        }
        else
        {
            std::cout << "输入不合法，重新输入" << std::endl;
        }
    }
    return 0;
}