#include <iostream>
#include <stdexcept>
using namespace std;

// 栈节点结构体
template <typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    StackNode(T e = T(), StackNode<T>* n = nullptr) : data(e), next(n) {}
};

// 栈模板类
template <typename T>
class Stack {
private:
    int _size;
    StackNode<T>* topNode;
public:
    Stack() : _size(0), topNode(nullptr) {}
    ~Stack() { clear(); }

    // 入栈
    void push(const T& e) {
        topNode = new StackNode<T>(e, topNode);
        ++_size;
    }

    // 出栈
    T pop() {
        if (empty()) throw std::out_of_range("Stack is empty!");
        StackNode<T>* node = topNode;
        T e = node->data;
        topNode = topNode->next;
        delete node;
        --_size;
        return e;
    }

    // 访问栈顶元素
    T& top() {
        if (empty()) throw std::out_of_range("Stack is empty!");
        return topNode->data;
    }

    // 判空
    bool empty() const { return _size == 0; }
    // 获取栈大小
    int size() const { return _size; }
    // 清空栈
    void clear() { while (!empty()) pop(); }

    // 遍历栈（从栈顶到栈底）
    void traverse(void (*visit)(T&)) {
        for (StackNode<T>* p = topNode; p; p = p->next)
            visit(p->data);
    }

    template <typename VST>
    void traverse(VST& visit) {
        for (StackNode<T>* p = topNode; p; p = p->next)
            visit(p->data);
    }
};
