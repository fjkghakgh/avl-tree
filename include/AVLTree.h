#pragma once

#include <vector>

class AVLTree
{
public:
    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

    ~AVLTree();

private:
    struct Node
    {
        Node(int value);

        int value;
        int height = 0;
        int size = 1;
        Node * left_node = nullptr;
        Node * right_node = nullptr;

        void update();
        int height_difference() const;
        void unhook();

        ~Node();
    };

    using NodePtr = Node *;

    Node * root = nullptr;

    static void balance(NodePtr & node);

    static NodePtr the_most_left(NodePtr node);
    static NodePtr the_most_right(NodePtr node);

    static NodePtr with_out_the_most_left(NodePtr node);
    static NodePtr with_out_the_most_right(NodePtr node);

    static NodePtr erase_from_tree(NodePtr node);

    static bool contains(NodePtr node, int value);
    static bool insert(NodePtr & node, int value);
    static bool remove(NodePtr & node, int value);

    static void values(NodePtr node, std::vector<int> & result);

    static void ll_rotation(NodePtr & node);
    static void rr_rotation(NodePtr & node);
    static void l_rotation(NodePtr & node);
    static void r_rotation(NodePtr & node);
};
