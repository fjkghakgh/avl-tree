#include "AVLTree.h"

#include <vector>

AVLTree::Node::Node(int value)
    : value(value)
{
}

AVLTree::~AVLTree()
{
    delete root;
}

AVLTree::Node::~Node()
{
    delete left_node;
    delete right_node;
}

bool AVLTree::contains(NodePtr node, int value)
{
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        return true;
    }
    if (node->value < value) {
        return contains(node->right_node, value);
    }
    else {
        return contains(node->left_node, value);
    }
}

bool AVLTree::contains(int value) const
{
    return contains(root, value);
}

bool AVLTree::insert(NodePtr & node, int value)
{
    if (node == nullptr) {
        node = new Node(value);
        return true;
    }
    if (node->value == value) {
        return false;
    }
    bool result;
    if (node->value > value) {
        result = insert(node->left_node, value);
    }
    else {
        result = insert(node->right_node, value);
    }
    balance(node);
    return result;
}

bool AVLTree::insert(int value)
{
    return insert(root, value);
}

AVLTree::NodePtr AVLTree::erase_from_tree(NodePtr node)
{
    NodePtr r = the_most_right(node->left_node);
    r->left_node = with_out_the_most_right(node->left_node);
    r->right_node = node->right_node;
    return r;
}

bool AVLTree::remove(NodePtr & node, int value)
{
    if (node == nullptr) {
        return false;
    }
    if (node->value == value) {
        NodePtr r;
        if (node->left_node != nullptr) {
            r = erase_from_tree(node);
        }
        else {
            r = node->right_node;
        }
        node->unhook();
        delete node;
        node = r;
        return true;
    }
    bool result;
    if (node->value > value) {
        result = remove(node->left_node, value);
    }
    else {
        result = remove(node->right_node, value);
    }
    balance(node);
    return result;
}

bool AVLTree::remove(int value)
{
    return remove(root, value);
}

std::size_t AVLTree::size() const
{
    return root ? root->size : 0;
}

bool AVLTree::empty() const
{
    return size() == 0;
}

std::vector<int> AVLTree::values() const
{
    std::vector<int> result;
    result.reserve(size());
    values(root, result);
    return result;
}

inline void AVLTree::Node::update()
{
    if (this->left_node == nullptr && this->right_node == nullptr) {
        height = 1;
        size = 1;
    }
    else if (this->left_node == nullptr) {
        height = this->right_node->height + 1;
        size = this->right_node->size + 1;
    }
    else if (this->right_node == nullptr) {
        height = this->left_node->height + 1;
        size = this->left_node->size + 1;
    }
    else {
        height = std::max(this->left_node->height, this->right_node->height) + 1;
        size = this->left_node->size + this->right_node->size + 1;
    }
}

int AVLTree::Node::height_difference() const
{
    int left_height = 0;
    if (this->left_node != nullptr) {
        left_height = left_node->height;
    }
    int right_height = 0;
    if (this->right_node != nullptr) {
        right_height = right_node->height;
    }
    return left_height - right_height;
}

void AVLTree::Node::unhook()
{
    this->left_node = nullptr;
    this->right_node = nullptr;
}

void AVLTree::balance(NodePtr & node)
{
    node->update();
    if (node->height_difference() > 1) {
        if (node->left_node->height_difference() < 0) {
            r_rotation(node);
        }
        else {
            rr_rotation(node);
        }
    }
    if (node->height_difference() < -1) {
        if (node->right_node->height_difference() < 0) {
            ll_rotation(node);
        }
        else {
            l_rotation(node);
        }
    }
    node->update();
}

AVLTree::NodePtr AVLTree::the_most_left(AVLTree::NodePtr node)
{
    if (node->left_node == nullptr) {
        return node;
    }
    else {
        return the_most_left(node->left_node);
    }
}

AVLTree::NodePtr AVLTree::the_most_right(AVLTree::NodePtr node)
{
    if (node->right_node == nullptr) {
        return node;
    }
    else {
        return the_most_right(node->right_node);
    }
}

AVLTree::NodePtr AVLTree::with_out_the_most_left(AVLTree::NodePtr node)
{
    if (node->left_node == nullptr) {
        return node->right_node;
    }
    node->left_node = with_out_the_most_left(node->left_node);
    balance(node);
    return node;
}

AVLTree::NodePtr AVLTree::with_out_the_most_right(AVLTree::NodePtr node)
{
    if (node->right_node == nullptr) {
        return node->left_node;
    }
    node->right_node = with_out_the_most_right(node->right_node);
    balance(node);
    return node;
}

void AVLTree::values(NodePtr node, std::vector<int> & result)
{
    if (node == nullptr) {
        return;
    }
    values(node->left_node, result);
    result.push_back(node->value);
    values(node->right_node, result);
}

void AVLTree::ll_rotation(NodePtr & node)
{
    NodePtr r = node->right_node;
    node->right_node = r->left_node;
    r->left_node = node;
    node->update();
    r->update();
    node = r;
}

void AVLTree::rr_rotation(NodePtr & node)
{
    NodePtr l = node->left_node;
    node->left_node = l->right_node;
    l->right_node = node;
    node->update();
    l->update();
    node = l;
}

void AVLTree::l_rotation(NodePtr & node)
{
    rr_rotation(node->right_node);
    ll_rotation(node);
}

void AVLTree::r_rotation(NodePtr & node)
{
    ll_rotation(node->left_node);
    rr_rotation(node);
}
