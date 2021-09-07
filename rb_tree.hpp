/**
 * @file rb_tree.hpp
 * @author luuyiran (luuyiran@gmail.com)
 * @brief red black tree map
 * @version 0.1
 * @date 2021-09-07
 * 
 * https://github.com/torvalds/linux/blob/master/lib/rbtree.c
 * https://github.com/torvalds/linux/blob/master/include/linux/rbtree.h
 * https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-introduction-to-algorithms-sma-5503-fall-2005/video-lectures/lecture-10-red-black-trees-rotations-insertions-deletions/lec10.pdf
 */
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__
#include "bst.hpp"

#define RB_RED               (1u)
#define RB_BLACK             (0u)
#define rbt_is_red(node)     (RB_RED == (node)->color)
#define rbt_is_black(node)   (RB_BLACK == (node)->color)
#define rbt_set_red(node)    ((node)->color = RB_RED)
#define rbt_set_black(node)  ((node)->color = RB_BLACK)

template <typename keyType, typename valueType>
class rbt_map : public bst_map<keyType, valueType> {
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
        rbt_rebalance(node);
        return iterator(node);
    }

    virtual iterator remove(link_type node) {
        iterator itor(node);
        link_type parent = nullptr;
        link_type child = nullptr;
        size_t color = RB_RED;
        link_type &root = base::getRoot();
        if (nullptr == node) return itor;
        ++itor;
        _remove_replace(&root, node, &parent, &child, &color);
        delete(node);
        base::dec();
        if (color == RB_BLACK) 
            rbt_erase_fixup(&root, child, parent);
        return itor;
    }

    virtual const char *name() const {
        return RB_TREE;
    }


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
private:
    /*
             y    Right Rotate (y)        x
            / \   ----------------->     / \
           x   T3                      T1   y
          / \      <--------------         / \
        T1   T2     Left Rotation(x)     T2   T3
    */
    link_type rbtree_right_rotate(link_type y) {
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

        return x;
    }

    link_type rbtree_left_rotate(link_type x) {
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
        return y;
    }

    void rbt_rebalance(link_type node) {
        link_type parent, gparent;
        // node->color = RB_RED;
        while ((parent = node->parent) && rbt_is_red(parent)) {
            gparent = parent->parent;

            if (parent == gparent->left) {
                link_type uncle = gparent->right;
                /* case 1 */
                if (uncle && rbt_is_red(uncle)) {
                    rbt_set_black(uncle);
                    rbt_set_black(parent);
                    rbt_set_red(gparent);
                    node = gparent;
                    continue;
                }
                /* case 2 */
                if (parent->right == node) {
                    link_type tmp;
                    rbtree_left_rotate(parent);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }
                /* case 3 */
                rbt_set_black(parent);
                rbt_set_red(gparent);
                rbtree_right_rotate(gparent);
            } else {
                link_type uncle = gparent->left;
                if (uncle && rbt_is_red(uncle)) {
                    rbt_set_black(uncle);
                    rbt_set_black(parent);
                    rbt_set_red(gparent);
                    node = gparent;
                    continue;
                }

                if (parent->left == node) {
                    link_type tmp;
                    rbtree_right_rotate(parent);
                    tmp = parent;
                    parent = node;
                    node = tmp;
                }

                rbt_set_black(parent);
                rbt_set_red(gparent);
                rbtree_left_rotate(gparent);
            }
        }

        link_type &root = base::getRoot();
        rbt_set_black(root);
    }

    void rbt_erase_fixup(link_type *root, link_type node,
                                link_type parent) {
        link_type other;
        while ((!node || rbt_is_black(node)) && node != *root) {
            if (parent->left == node) {
                other = parent->right;
                if (rbt_is_red(other)) {
                    rbt_set_black(other);
                    rbt_set_red(parent);
                    rbtree_left_rotate(parent);
                    other = parent->right;
                }
                if ((!other->left || rbt_is_black(other->left)) &&
                    (!other->right || rbt_is_black(other->right))) {
                    rbt_set_red(other);
                    node = parent;
                    parent = node->parent;
                } else {
                    if (!other->right || rbt_is_black(other->right)) {
                        link_type o_left;
                        if ((o_left = other->left)) rbt_set_black(o_left);
                        rbt_set_red(other);
                        rbtree_right_rotate(other);
                        other = parent->right;
                    }
                    other->color = parent->color;
                    rbt_set_black(parent);
                    if (other->right) rbt_set_black(other->right);
                    rbtree_left_rotate(parent);
                    node = *root;
                    break;
                }
            } else {
                other = parent->left;
                if (rbt_is_red(other)) {
                    rbt_set_black(other);
                    rbt_set_red(parent);
                    rbtree_right_rotate(parent);
                    other = parent->left;
                }
                if ((!other->left || rbt_is_black(other->left)) &&
                    (!other->right || rbt_is_black(other->right))) {
                    rbt_set_red(other);
                    node = parent;
                    parent = node->parent;
                } else {
                    if (!other->left || rbt_is_black(other->left)) {
                        link_type o_right;
                        if ((o_right = other->right)) rbt_set_black(o_right);
                        rbt_set_red(other);
                        rbtree_left_rotate(other);
                        other = parent->left;
                    }
                    other->color = parent->color;
                    rbt_set_black(parent);
                    if (other->left) rbt_set_black(other->left);
                    rbtree_right_rotate(parent);
                    node = *root;
                    break;
                }
            }
        }
        if (node) rbt_set_black(node);
    }
};

#endif 
