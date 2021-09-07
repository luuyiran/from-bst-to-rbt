#ifndef __BALANCED_BINARY_SEARCH_TREE_HPP__
#define __BALANCED_BINARY_SEARCH_TREE_HPP__
#include "helper.hpp"

template <typename keyType, typename valueType>
struct __node_base {
    typedef keyType key_type;
    typedef valueType value_type;

    typedef __node_base<keyType, valueType>  *link_type;

    __node_base(const keyType& k, const valueType& v)
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          height(1),
          key(k),
          value(v) {}

    link_type parent;
    link_type left;
    link_type right;
    union {
        size_t color;
        size_t height;
    };

    key_type   key;
    value_type value;
};




/*-----------------------------------------------------------------------------*/

template <typename keyType, typename valueType>
__node_base<keyType, valueType>*
__node_base_next(__node_base<keyType, valueType> *root) {
    __node_base<keyType, valueType> *parent;
    if (root == nullptr) return nullptr;
    if (root->right) {
        root = root->right;
        while (root->left) root = root->left;
        return root;
    }
    while ((parent = root->parent) && root == parent->right) root = parent;
    return parent;
}

template <typename keyType, typename valueType>
__node_base<keyType, valueType>*
__node_base_prev(__node_base<keyType, valueType> *root) {
    __node_base<keyType, valueType> *parent;
    if (root == nullptr) return nullptr;
    if (root->left) {
        root = root->left;
        while (root->right) root = root->right;
        return root;
    }
    while ((parent = root->parent) && root == parent->left) root = parent;
    return parent;
}

template <typename keyType, typename valueType>
__node_base<keyType, valueType>*
__node_base_first(__node_base<keyType, valueType> *root) {
    if (nullptr == root) return nullptr;
    while (root->left) root = root->left;
    return root;
}

template <typename keyType, typename valueType>
__node_base<keyType, valueType>*
__node_base_last(__node_base<keyType, valueType> *root) {
    if (nullptr == root) return nullptr;
    while (root->right) root = root->right;
    return root;
}

/* 把 node 放在 parent 之后，放置位置在 pos，pos保存父节点 parent 的左子树或右子树的指针 */
template <typename keyType, typename valueType>
inline void link_node_base(__node_base<keyType, valueType> *parent, 
                           __node_base<keyType, valueType> *node, 
                           __node_base<keyType, valueType> **pos) {
    node->parent = parent;
    node->left = node->right = nullptr;
    node->height = 1;  /* 新节点高度默认为1，且1代表红色 */

    /* pos保存的是 &parent->left;或者 &parent->right; */
    /* 因此下面一行代码相当于 parent->left = node;或者 parent->right = node; */
    *pos = node;        
}

/* 删除节点node, 用以右子树的最大值来替换 */
template <typename keyType, typename valueType>
void _remove_replace(__node_base<keyType, valueType> **root, 
                     __node_base<keyType, valueType> *node, 
                     __node_base<keyType, valueType> **prt = nullptr, 
                     __node_base<keyType, valueType> **chld = nullptr, size_t *clr = nullptr
                     ) {
    size_t color;
    __node_base<keyType, valueType> *child, *parent;
    if (nullptr == *root || nullptr == node) return;

    /* two children */
    if (node->left && node->right) {
        __node_base<keyType, valueType> *left;
        __node_base<keyType, valueType> *old = node;

        node = node->right;
        while ((left = node->left) != nullptr)
            node = node->left;
        child = node->right;
        parent = node->parent;
        color = node->color;

        if (child) child->parent = parent;

        if (parent) {
            if (parent == old) {
                parent->right = child;
                parent = node;
            } else
                parent->left = child;
        } else 
            *root = child;

        /* 右子树的最小节点替换删除位置的节点 */
        node->parent = old->parent;
        node->color = old->color;
        node->right = old->right;
        node->left = old->left;

        if (old->parent) {
            if (old->parent->left == old)
                old->parent->left = node;
            else
                old->parent->right = node;
        } else
            *root = node; /* old 是根节点 */

        old->left->parent = node;
        if (old->right)
            old->right->parent = node;
    } else { 
        /* no child, or only one */
        child = node->left ? node->left : node->right;
        parent = node->parent;
        color = node->color;
        if (child)
            child->parent = parent;
        if (parent) {
            if (parent->left == node)
                parent->left = child;
            else
                parent->right = child;
        }
        else
            *root = child;
    }

    if (chld) *chld = child;
    if (prt) *prt = parent;
    if (clr) *clr = color;
}

/*-----------------------------------------------------------------------------*/






template <typename keyType, typename valueType>
struct _node_iterator {
    typedef keyType    key_type;
    typedef valueType  value_type;
    typedef valueType& reference;
    typedef valueType* pointer;

    typedef _node_iterator<keyType, valueType>   self;
    typedef __node_base<keyType, valueType>  *link_type;

    link_type node;

    explicit _node_iterator(link_type _x) : node(_x) {}

    self& operator=(link_type _x) {
        node = _x;
        return *this;
    }

    reference operator*() const {
        return node->value;
    }

    pointer operator->() const {
        return &node->value;
    }

    self& operator++() {
        node = __node_base_next(node);
        return *this;
    }
    self operator++(int) {
        self _tmp = *this;
        node = __node_base_next(node);
        return _tmp;
    }

    self& operator--() {
        node = __node_base_prev(node);
        return *this;
    }
    self operator--(int) {
        self _tmp = *this;
        node = __node_base_prev(node);
        return _tmp;
    }

    bool operator==(const self& _x) const {
        return node == _x.node;
    }
    bool operator!=(const self& _x) const {
        return node != _x.node;
    }

    bool operator==(const link_type _x) const {
        return node == _x;
    }
    bool operator!=(const link_type _x) const {
        return node != _x;
    }
};



const char *BST_TREE = "bst ";
const char *AVL_TREE = "avl ";
const char *RB_TREE  = "rbt ";
const char *LLRB_TREE= "llrb";


template <typename keyType, typename valueType>
class bst_map {
public:
    typedef keyType                         key_type;
    typedef valueType                       value_type;
    typedef valueType&                      reference;
    typedef const valueType&                const_reference;
    typedef valueType*                      pointer;
    typedef const valueType*                const_pointer;

    typedef __node_base<keyType, valueType>     NODE;
    typedef __node_base<keyType, valueType>     *link_type;
    typedef _node_iterator<keyType, valueType>  iterator;


    link_type root;
    size_t  size;

    // 辅助测试
    void inc() {size++;}
    void dec() {size--;}
    link_type& getRoot() {return root;}
    void setRoot(link_type node) {root = node;}

    // 绝不在构造和析构中调用虚函数
    bst_map() : root(nullptr), size(0) {}
    virtual ~bst_map() { clear(); }

    reference operator[](const keyType &key) {
        link_type pos = find(key);
        if (pos) 
            return pos->value;
        else {
            return *iterator(insert(key, valueType()));
            //link_type n = new NODE(key, valueType());
            //insert(n);
            //return n->value;
        }
    }

    virtual iterator insert(const keyType &key, const valueType &value) {
        link_type n = new NODE(key, value);
        assert(n);
        return insert(n);
    }

    virtual iterator insert(link_type node) {
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
        size++;
        return iterator(node);
    }


    virtual iterator remove(const keyType &key) {
        link_type node = find(key);
        return remove(node);
    }

    virtual iterator remove(link_type node) {
        iterator itor(node);
        if (nullptr == node) return itor;
        ++itor;
        _remove_replace(&root, node);
        delete(node);
        size--;
        return itor;
    }

    void clear() {
        while (root) {
            remove(root);
        }
    }

    bool empty() const {
        return nullptr == root;
    }

    link_type find(const keyType &key) const {
        link_type pos = root;
        while (pos) {
            if (key < pos->key)
                pos = pos->left;
            else if (key > pos->key) 
                pos = pos->right;
            else
                return pos;
        }
        return nullptr;
    }

    iterator begin() {
        return iterator(__node_base_first(root));
    }

    iterator end() {
        return iterator(nullptr);
    }


    virtual const char *name() const {
        return BST_TREE;
    }


    
};























#endif
