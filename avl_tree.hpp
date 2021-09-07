/**
 * @file avl_tree.hpp
 * @author luuyiran (luuyiran@gmail.com)
 * @brief avl tree map
 * @version 0.1
 * @date 2021-09-07
 * 
 * https://www.geeksforgeeks.org/avl-tree-set-2-deletion/
 * https://github.com/skywind3000/avlmini
 */
#ifndef __AVL_TREE_HPP__
#define __AVL_TREE_HPP__
#include "bst.hpp"

#ifndef MIN
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#endif 

#ifndef MAX
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#endif 


template <typename keyType, typename valueType>
class avl_map : public bst_map<keyType, valueType> {
public:
    typedef bst_map<keyType, valueType>            base;
    typedef typename base::link_type link_type;
    typedef typename base::iterator iterator;


    virtual iterator insert(link_type node) {
        link_type &root = base::getRoot();
        link_type *pos = &root;
        link_type parent = nullptr;
        while (*pos) {
            parent = *pos;
            if (node->key < (*pos)->key)
                pos = &((*pos)->left);
            else if (node->key > (*pos)->key)
                pos = &((*pos)->right);
            else {
                (*pos)->value = node->value;
                delete node;
                return iterator(*pos);
            }
        }
        link_node_base(parent, node, pos);
        base::inc();
        avl_rebalance(parent);
        return iterator(node);
    }

    virtual iterator remove(link_type node) {
        iterator itor(node);
        link_type parent = nullptr;
        link_type &root = base::getRoot();
        if (nullptr == node) return itor;
        ++itor;
        _remove_replace(&root, node, &parent);
        delete(node);
        base::dec();
        avl_rebalance(parent);
        return itor;
    }

    virtual const char *name() const {
        return AVL_TREE;
    }

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
private:
    size_t avl_height(link_type node) {
        return nullptr == node ? 0 : node->height;
    }

    size_t avl_update_height(link_type node) {
        return nullptr == node ? 
            0 : (1 + MAX(avl_height(node->left), avl_height(node->right)));
    }

    int avl_balance_factor(link_type node) {
        if (nullptr == node) return 0;
        return avl_height(node->left) - avl_height(node->right);
    }


    /*
             y    Right Rotate (y)        x
            / \   ----------------->     / \
           x   T3                      T1   y
          / \      <--------------         / \
        T1   T2     Left Rotation(x)     T2   T3
    */
    link_type avl_right_rotate(link_type y) {
        link_type x = y->left;
        link_type T2 = x->right;
        link_type parent = y->parent;

        x->right = y;
        y->parent = x;

        y->left = T2;
        if (T2) T2->parent = y;

        x->parent = parent;
        if (parent) {
            if (parent->left == y)
                parent->left = x;
            else
                parent->right = x;
        } else {
            base::setRoot(x);
        }

        y->height = avl_update_height(y);
        x->height = avl_update_height(x);

        return x;
    }

    link_type avl_left_rotate(link_type x) {
        link_type y = x->right;
        link_type T2 = y->left;
        link_type parent = x->parent;

        y->left = x;
        x->parent = y;

        x->right = T2;
        if (T2) T2->parent = x;

        y->parent = parent;
        if (parent) {
            if (parent->left == x)
                parent->left = y;
            else
                parent->right = y;
        } else {
            base::setRoot(y);
        }

        /* 注意顺序，先计算较矮的 */
        x->height = avl_update_height(x);
        y->height = avl_update_height(y);

        return y;
    }



    void avl_rebalance(link_type node) {
        if (nullptr == node) return;
        size_t height = avl_update_height(node);
        int factor = avl_balance_factor(node);

        // 剪枝，如果高度无变化且无需调整，则结束回溯
        if (node->height != height)
            node->height = height;
        else if (factor >= -1 && factor <= 1)
            return;

        link_type parent = node->parent;

        /* If this node becomes unbalanced, then there are 4 cases */

        /************************************************************
        a) Left Left Case
            T1, T2, T3 and T4 are subtrees.
                    z                                      y
                   / \                                   /  \
                  y   T4      Right Rotate (z)          x    z
                / \          - - - - - - - - ->       / \   / \
              x   T3                                T1  T2 T3  T4
             / \
           T1   T2
        ************************************************************/
        if (factor > 1 && avl_balance_factor(node->left) >= 0)
            avl_right_rotate(node);

        /************************************************************
        b) Right Right Case
              z                                y
             /  \                            /   \
            T1   y     Left Rotate(z)       z      x
                /  \   - - - - - - - ->    / \    / \
               T2   x                     T1  T2 T3  T4
                   / \
                 T3  T4
        ************************************************************/
        else if (factor < -1 && avl_balance_factor(node->right) <= 0)
            avl_left_rotate(node);

        /************************************************************
        c) Left Right Case
                z                               z                           x
              / \                            /   \                        /  \
             y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
            / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    /
        \ T1   x                          y    T3                    T1  T2 T3
        T4
              / \                        / \
            T2   T3                    T1   T2
        ************************************************************/
        else if (factor > 1 && avl_balance_factor(node->left) < 0) {
            avl_left_rotate(node->left);
            avl_right_rotate(node);
        }

        /************************************************************
        d) Right Left Case
               z                            z                            x
              / \                          / \                          /  \
            T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
                / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
               x   T4                      T2   y                  T1  T2  T3 T4
              / \                              /  \
            T2   T3                           T3   T4
        ************************************************************/
        else if (factor < -1 && avl_balance_factor(node->right) > 0) {
            avl_right_rotate(node->right);
            avl_left_rotate(node);
        }

        avl_rebalance(parent);
    }
};






#endif 
