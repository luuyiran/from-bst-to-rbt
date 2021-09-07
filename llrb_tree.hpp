/**
 * @file llrb_tree.hpp
 * @author luuyiran (luuyiran@gmail.com)
 * @brief left lening red black tree map
 * @version 0.1
 * @date 2021-09-07
 * 
 * https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
 * https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
 */
#ifndef __LEFT_LEANING_RED_BLACK_TREE_H__
#define __LEFT_LEANING_RED_BLACK_TREE_H__
#include "bst.hpp"

#define    LLRB_RED               (1u)
#define    LLRB_BLACK             (0u)


#define llrb_is_red(node) (nullptr == (node) ? 0 : ((node)->color == LLRB_RED))
#define llrb_set_red(node)  do {if (node) node->color = LLRB_RED;} while(0)
#define llrb_set_black(node)  do {if (node) node->color = LLRB_BLACK;} while(0)


template <typename keyType, typename valueType>
class llrb_map : public bst_map<keyType, valueType> {
public:
    typedef bst_map<keyType, valueType>             base;
    typedef typename base::NODE                     NODE;
    typedef typename base::link_type                link_type;
    typedef typename base::iterator                 iterator;


    virtual iterator insert(const keyType &key, const valueType &value) {
        link_type n = new NODE(key, value);
        assert(n);
        base::insert(n);
        llrb_fix_up_to_root(n);
        return iterator(n);
    }

    virtual iterator remove(const keyType &key) {
        link_type &root = base::getRoot();
        root = delete_recursive(root, key);
        return iterator(root);
    }


    virtual const char *name() const {
        return LLRB_TREE;
    }


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
private:
    /**
     *       parent                       parent
     *        |                            |
     *        y    Right Rotate (y)        x
     *       / \   ----------------->     / \
     *      x   T3                      T1   y
     *     / \      <--------------         / \
     *   T1   T2     Left Rotation(x)     T2   T3
     */
    link_type llrbtree_right_rotate(link_type y) {
        if (!y) return nullptr;
        link_type x = y->left;
        link_type T2 = x->right;
        link_type parent = y->parent;

        x->right = y;
        y->parent = x;

        y->left = T2;
        if (T2) T2->parent = y;

        x->parent = parent;
        if (parent) {
            if (y == parent->right)
                parent->right = x;
            else
                parent->left = x;
        } else
            base::setRoot(x);

        x->color = y->color;
        llrb_set_red(x->right);

        return x;
    }

    link_type llrbtree_left_rotate(link_type x) {
        if (!x) return nullptr;
        link_type y = x->right;
        link_type T2 = y->left;
        link_type parent = x->parent;

        y->left = x;
        x->parent = y;

        x->right = T2;
        if (T2) T2->parent = x;

        y->parent = parent;
        if (parent) {
            if (x == parent->left)
                parent->left = y;
            else
                parent->right = y;
        } else
            base::setRoot(y);

        y->color = x->color;
        llrb_set_red(y->left);

        return y;
    }

    // 相当于拆分父节点或合并兄弟节点，调整黑高
    void color_flip(link_type node) {
        if (node) {
            node->color = !(node->color);
            if (node->left) node->left->color = !(node->left->color);
            if (node->right) node->right->color = !(node->right->color);
        }
    }

    void llrb_fix_up_to_root(link_type node) {
        if (nullptr == node) return;
        node = llrb_fix_up(node);

        // bottom-up
        llrb_fix_up_to_root(node->parent);
    }

    link_type llrb_fix_up(link_type node) {
        if (nullptr == node) return nullptr;
        if (llrb_is_red(node->right)) 
            node = llrbtree_left_rotate(node);
        if (node->left && llrb_is_red(node->left) &&
            llrb_is_red(node->left->left))
            node = llrbtree_right_rotate(node);

        if (llrb_is_red(node->left) && llrb_is_red(node->right))
            color_flip(node);

        return node;
    }



    void dlete_max() {
        link_type &root = base::getRoot();
        root = delete_max(root);
        llrb_set_black(root);
    }

    /**
     *            node
     *             \
     *              o  (black)
     *             /
     *            p (black)
     */
    link_type move_red_right(link_type node) {
        color_flip(node);   // node 从父亲借出来，并合并它的两个孩子
        if (llrb_is_red(node->left->left)) {    // 变为 5 叉树
            node = llrbtree_right_rotate(node);
            color_flip(node);   // node 被收回父节点，左右子树分离，5叉树被拆开
        }
        return node;
    }

    link_type delete_max(link_type node) {
        if (llrb_is_red(node->left))
            node = llrbtree_right_rotate(node);
        
        // 已到达最右
        if (node->right == nullptr) {
            delete node;
            base::dec();
            return nullptr;
        }

        if (!llrb_is_red(node->right) && !llrb_is_red(node->right->left))
            node = move_red_right(node);
        
        node->right = delete_max(node->right);
        if (node->right) 
            node->right->parent = node;

        return llrb_fix_up(node);
    }



    void dlete_min() {
        link_type &root = base::getRoot();
        root = delete_min(root);
        llrb_set_black(root);
    }

    /**
     *             node
     *              /
     *             o  (black)
     *            /
     *           p (black)
     */
    link_type move_red_left(link_type node) {
        color_flip(node);   // node 从父亲借出来，并合并它的两个孩子
        if (llrb_is_red(node->right->left)) {    // 变为 5 叉树
            node->right = llrbtree_right_rotate(node->right);
            node = llrbtree_left_rotate(node);
            color_flip(node);   // node 被收回父节点，左右子树分离，5叉树被拆开
        }
        return node;
    }

    link_type delete_min(link_type node) {
        // 已到达最左
        if (node->left == nullptr) {
            delete node;
            base::dec();
            return nullptr;
        }

        if (!llrb_is_red(node->left) && !llrb_is_red(node->left->left))
            node = move_red_left(node);
        
        node->left = delete_min(node->left);
        if (node->left) 
            node->left->parent = node;

        return llrb_fix_up(node);
    }

    link_type delete_recursive(link_type node, const keyType &key) {
        if (key < node->key) {
            if (!llrb_is_red(node->left) && !llrb_is_red(node->left->left))
                node = move_red_left(node);
            node->left = delete_recursive(node->left, key);
            if (node->left) 
                node->left->parent = node;
        } else {
            if (llrb_is_red(node->left))
                node = llrbtree_right_rotate(node);
            if (key == node->key && node->right == nullptr) {
                delete node;
                base::dec();
                return nullptr;
            }
            if (!llrb_is_red(node->right) && !llrb_is_red(node->right->left))
                node = move_red_right(node);

            
            if (key == node->key) {
                link_type right_min = __node_base_first(node->right);
                node->key = right_min->key;             // 替换后继的键值
                node->value = right_min->value;
                node->right = delete_min(node->right);  // 删除后继
            } else {
                node->right = delete_recursive(node->right, key);
            }
        }
        return llrb_fix_up(node);
    }
};


#endif 
