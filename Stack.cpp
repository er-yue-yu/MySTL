#include <iostream>
#include <stdexcept>
using namespace std;

// ջ�ڵ�ṹ��
template <typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    StackNode(T e = T(), StackNode<T>* n = nullptr) : data(e), next(n) {}
};

// ջģ����
template <typename T>
class Stack {
private:
    int _size;
    StackNode<T>* topNode;
public:
    Stack() : _size(0), topNode(nullptr) {}
    ~Stack() { clear(); }

    // ��ջ
    void push(const T& e) {
        topNode = new StackNode<T>(e, topNode);
        ++_size;
    }

    // ��ջ
    T pop() {
        if (empty()) throw std::out_of_range("Stack is empty!");
        StackNode<T>* node = topNode;
        T e = node->data;
        topNode = topNode->next;
        delete node;
        --_size;
        return e;
    }

    // ����ջ��Ԫ��
    T& top() {
        if (empty()) throw std::out_of_range("Stack is empty!");
        return topNode->data;
    }

    // �п�
    bool empty() const { return _size == 0; }
    // ��ȡջ��С
    int size() const { return _size; }
    // ���ջ
    void clear() { while (!empty()) pop(); }

    // ����ջ����ջ����ջ�ף�
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
