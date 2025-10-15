#include <iostream>
#include <cstdlib>  // ���� rand() ����������ʱ���ѡ���㷨��
using namespace std;

// �����ȣ�Rank�������ڱ�ʾ�����нڵ��λ�����������������±꣩
typedef int Rank;

// ����ڵ�ṹģ��
template <typename T> 
struct ListNode {
    T data;               // �ڵ�洢������
    ListNode<T>* pred;    // ָ��ǰ���ڵ��ָ��
    ListNode<T>* succ;    // ָ���̽ڵ��ָ��

    // �ڵ㹹�캯��
    // ������e�����ݣ���p��ǰ��ָ�룩��s�����ָ�룩������Ĭ��ֵ
    ListNode(T e = T(), ListNode<T>* p = nullptr, ListNode<T>* s = nullptr)
        : data(e), pred(p), succ(s) {}
};

// ����ڵ�λ�õı������� ListNode<T>* ����д
// ģ������� C++11 �����ϱ�׼֧�֣�����ʱ��� -std=c++11��
template <typename T> 
using ListNodePosi = ListNode<T>*;

// ˫��������ģ�壨���ڱ��ڵ㣬�򻯱߽紦��
template <typename T> 
class List {
private:
    int _size;               // ����ʵ�ʽڵ��������������ڱ���
    ListNodePosi<T> header;  // ͷ�ڱ��ڵ㣨���洢���ݣ���ͷ��������
    ListNodePosi<T> trailer; // β�ڱ��ڵ㣨���洢���ݣ���β��������

protected:
    // ��ʼ�����������ڱ��ڵ㲢������ʼ����
    void init() {
        header = new ListNode<T>();  // ����ͷ�ڱ���Ĭ�Ϲ��죩
        trailer = new ListNode<T>(); // ����β�ڱ���Ĭ�Ϲ��죩
        header->succ = trailer;      // ͷ�ڱ��ĺ��ָ��β�ڱ�
        trailer->pred = header;      // β�ڱ���ǰ��ָ��ͷ�ڱ�
        _size = 0;                   // ��ʼ����Ϊ 0
    }

    // �������ɾ������ʵ�ʽڵ㣨�����ڱ���
    int clear() {
        int oldSize = _size;         // ��¼ԭʼ����
        while (_size > 0) {
            remove(header->succ);    // �ӵ�һ��ʵ�ʽڵ㿪ʼɾ��
        }
        return oldSize;              // ����ɾ���Ľڵ���
    }

    // ���ƽڵ㣺�� p ��ʼ���� n ���ڵ㵽��ǰ����
    void copyNodes(ListNodePosi<T> p, int n) {
        init();                      // �ȳ�ʼ����ǰ����
        while (n-- > 0) {
            insertAsLast(p->data);   // �������ݵ�β��
            p = p->succ;             // �ƶ�����һ���ڵ�
        }
    }

    // �鲢�㷨������ǰ������ p ��ʼ�� n ���ڵ��� L �� q ��ʼ�� m ���ڵ�鲢
    // ǰ�᣺�����ֶ�������ģ��鲢����������
    void merge(ListNodePosi<T>& p, int n, List<T>& L, ListNodePosi<T> q, int m) {
        ListNodePosi<T> pp = p->pred;  // ��¼ p ��ǰ�������ڹ鲢�����ӣ�
        while (m > 0 && n > 0) {       // ���߶��нڵ�ʱѭ��
            // ȡ��С�Ľڵ���뵽��ǰ����
            if (q->data < p->data) {
                insertB(p, L.remove(q));  // �� L �� q �ڵ��Ƶ� p ֮ǰ
                q = L.first();            // ���� q Ϊ L �����׽ڵ�
                m--;
            } else {
                p = p->succ;  // ��ǰ�ڵ��С��ֱ�Ӻ���
                n--;
            }
        }
        // �� L �л���ʣ��ڵ㣬ȫ�����뵱ǰ����
        if (m > 0) {
            while (m-- > 0) {
                ListNodePosi<T> next = q->succ;  // ��ǰ��¼��һ���ڵ�
                insertB(p, L.remove(q));
                q = next;
            }
        }
        _size += m;    // ���µ�ǰ�����ȣ���ʱ m ��Ϊ 0����Ӱ�죩
        L._size -= m;  // ���� L �ĳ���
    }

    // �鲢���򣺶� p ��ʼ�� n ���ڵ�������򣨷���˼�룩
    void mergeSort(ListNodePosi<T> p, int n) {
        if (n < 2) return;  // �����ڵ���������
        int mid = n >> 1;   // ȡ�е㣨�ȼ��� n/2��Ч�ʸ��ߣ�
        ListNodePosi<T> q = p;
        for (int i = 0; i < mid; i++) q = q->succ;  // �ҵ��������
        mergeSort(p, mid);         // �ݹ�����ǰ���
        mergeSort(q, n - mid);     // �ݹ��������
        merge(p, mid, *this, q, n - mid);  // �鲢����
    }

    // ѡ�����򣺶� p ��ʼ�� n ���ڵ��������ÿ��ѡ���Ԫ�ط�β����
    void selectionSort(ListNodePosi<T> p, int n) {
        ListNodePosi<T> tail = p;
        // �ҵ����������β��λ�ã���ʼΪ�� n ���ڵ�ĺ�̣�
        for (int i = 0; i < n; i++) tail = tail->succ;
        while (n > 1) {  // ֱ��ֻʣһ���ڵ�
            ListNodePosi<T> max = selectMax(p, n);  // �ҵ����ڵ�
            // �������ڵ��뵱ǰβ�ڵ������
            swap(max->data, (tail->pred)->data);
            tail = tail->pred;  // β�ڵ�ǰ��
            n--;
        }
    }

    // �������򣺶� p ��ʼ�� n ���ڵ��������������뵽ǰ����������䣩
    void insertionSort(ListNodePosi<T> p, int n) {
        for (int i = 0; i < n; i++) {
            // �ҵ�����λ�ò����뵱ǰ�ڵ�����
            insertA(search(p->data, i, p), p->data);
            p = p->succ;       // ������һ���ڵ�
            remove(p->pred);   // ɾ��ԭλ�õĽڵ�
        }
    }

public:
    // �޲ι��캯������ʼ��������
    List() { init(); }

    // ���ƹ��캯���������������� L
    List(List<T> const& L) { copyNodes(L.first(), L._size); }

    // ���ָ��ƹ��죺���� L �д��� r ��ʼ�� n ���ڵ�
    List(List<T> const& L, Rank r, int n) {
        ListNodePosi<T> p = L.first();
        for (int i = 0; i < r; i++) p = p->succ;  // �ƶ����� r ��Ӧ�Ľڵ�
        copyNodes(p, n);
    }

    // �ӽڵ㸴�ƹ��죺���ƴӽڵ� p ��ʼ�� n ���ڵ�
    List(ListNodePosi<T> p, int n) { copyNodes(p, n); }

    // �����������ͷ����нڵ㣨�����ڱ���
    ~List() {
        clear();         // ��ɾ������ʵ�ʽڵ�
        delete header;   // �ͷ�ͷ�ڱ�
        delete trailer;  // �ͷ�β�ڱ�
    }

    // ����������
    Rank size() const { return _size; }

    // �ж������Ƿ�Ϊ��
    bool empty() const { return _size <= 0; }

    // ���� [] �������ͨ���ȷ��ʽڵ����ݣ��������飩
    // ʱ�临�Ӷ� O(n)�������ʺ�������ʣ���Ϊ����ʹ��
    T& operator[](Rank r) const {
        ListNodePosi<T> p = first();
        for (int i = 0; i < r; i++) p = p->succ;  // �ƶ����� r ���ڵ�
        return p->data;
    }

    // ���ص�һ��ʵ�ʽڵ㣨ͷ�ڱ��ĺ�̣�
    ListNodePosi<T> first() const { return header->succ; }

    // �������һ��ʵ�ʽڵ㣨β�ڱ���ǰ����
    ListNodePosi<T> last() const { return trailer->pred; }

    // �жϽڵ� p �Ƿ�Ϊ��Ч�ڵ㣨���ڱ��ҷǿգ�
    bool valid(ListNodePosi<T> p) {
        return p != nullptr && p != header && p != trailer;
    }

    // ��������������Ե������������ж������Ƿ�����
    // ������ 0��˵��������ȫ����
    int disordered() const {
        int cnt = 0;
        ListNodePosi<T> p = first();
        for (int i = 1; i < _size; i++) {
            p = p->succ;
            if (p->pred->data > p->data) cnt++;  // ǰ�� > ��̣��γ�����
        }
        return cnt;
    }

    // ���ҽڵ㣺�� p ��ǰ n ���ڵ��в���ֵΪ e �Ľڵ㣨�� p ��ǰ�ң�
    // �����ҵ��Ľڵ�ָ�룬δ�ҵ����� nullptr
    ListNodePosi<T> find(T const& e, int n, ListNodePosi<T> p) const {
        while (n-- > 0) {
            p = p->pred;
            if (e == p->data) return p;
        }
        return nullptr;
    }

    // ��������λ�ã��� p ��ǰ n ���ڵ��в��� <= e �����ڵ㣨���ڲ�������
    ListNodePosi<T> search(T const& e, int n, ListNodePosi<T> p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data <= e) break;  // �ҵ����������Ľڵ�
        }
        return p;  // ���ز���λ�õ�ǰ��
    }

    // �������ֵ�ڵ㣺�� p ��ʼ�� n ���ڵ����ҵ��������Ľڵ�
    ListNodePosi<T> selectMax(ListNodePosi<T> p, int n) {
        ListNodePosi<T> max = p;  // ��ʼ�����һ���ڵ�������
        for (ListNodePosi<T> cur = p; n > 1; n--) {
            cur = cur->succ;
            if (cur->data >= max->data) max = cur;  // �������ֵ�ڵ�
        }
        return max;
    }

    // ������ͷ������ڵ㣬�����½ڵ��ָ��
    ListNodePosi<T> insertAsFirst(T const& e) {
        _size++;  // ���� +1
        // �½ڵ��ǰ����ͷ�ڱ��������ԭ�׽ڵ�
        // ͬʱ����ͷ�ڱ��ĺ�̺�ԭ�׽ڵ��ǰ��ָ���½ڵ�
        return header->succ = header->succ->pred = new ListNode<T>(e, header, header->succ);
    }

    // ������β������ڵ㣬�����½ڵ��ָ��
    ListNodePosi<T> insertAsLast(T const& e) {
        _size++;  // ���� +1
        // �½ڵ��ǰ����ԭβ�ڵ㣬�����β�ڱ�
        // ͬʱ����ԭβ�ڵ�ĺ�̺�β�ڱ���ǰ��ָ���½ڵ�
        return trailer->pred = trailer->pred->succ = new ListNode<T>(e, trailer->pred, trailer);
    }

    // �ڽڵ� p ֮�����ڵ㣬�����½ڵ��ָ��
    ListNodePosi<T> insertA(ListNodePosi<T> p, T const& e) {
        _size++;  // ���� +1
        // �½ڵ��ǰ���� p������� p ��ԭ���
        // ͬʱ���� p �ĺ�̺� p ԭ��̵�ǰ��ָ���½ڵ�
        return p->succ = p->succ->pred = new ListNode<T>(e, p, p->succ);
    }

    // �ڽڵ� p ֮ǰ����ڵ㣬�����½ڵ��ָ��
    ListNodePosi<T> insertB(ListNodePosi<T> p, T const& e) {
        _size++;  // ���� +1
        // �½ڵ��ǰ���� p ��ԭǰ��������� p
        // ͬʱ���� p ԭǰ���ĺ�̺� p ��ǰ��ָ���½ڵ�
        return p->pred = p->pred->succ = new ListNode<T>(e, p->pred, p);
    }

    // ɾ���ڵ� p�����ؽڵ��д洢������
    T remove(ListNodePosi<T> p) {
        T e = p->data;  // ����ڵ�����
        // �Ͽ� p ��ǰ��ڵ�����ӣ�p ��ǰ��ָ�� p �ĺ�̣�p �ĺ��ָ�� p ��ǰ��
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;       // �ͷ� p ���ڴ�
        _size--;        // ���� -1
        return e;       // ����ɾ��������
    }

    // �鲢��ǰ���������� L���鲢�� L �ᱻ��գ�
    void merge(List<T>& L) {
        merge(first(), _size, L, L.first(), L._size);
    }

    // ���򣺶� p ��ʼ�� n ���ڵ��������ѡ�������㷨֮һ��
    void sort(ListNodePosi<T> p, int n) {
        switch (rand() % 3) {
            case 0: insertionSort(p, n); break;  // ���������ʺ�С���ݻ��������
            case 1: selectionSort(p, n); break;  // ѡ�����򣨼򵥵�Ч�ʵͣ�
            default: mergeSort(p, n);            // �鲢����Ч�ʸߣ�O(nlogn)��
        }
    }

    // ��������������
    void sort() { sort(first(), _size); }

    // ȥ�أ�ɾ�������������ظ��Ľڵ㣨������һ�����ֵĽڵ㣩
    // ʱ�临�Ӷ� O(n2)���ʺ���������
    int deduplicate() {
        int oldSize = _size;
        ListNodePosi<T> p = first();
        for (Rank i = 0; i < _size; p = p->succ, i++) {
            // �� p ֮ǰ�� i ���ڵ��в����Ƿ����ظ�
            ListNodePosi<T> q = find(p->data, i, p);
            if (q) remove(q);  // ���ظ���ɾ��
        }
        return oldSize - _size;  // ����ɾ���Ľڵ���
    }

    // ����ȥ�أ�ɾ�����������������ظ��Ľڵ㣨����������������
    // ʱ�临�Ӷ� O(n)��Ч�ʸ���
    int uniquify() {
        if (_size < 2) return 0;  // ������򵥽ڵ�����ȥ��
        int oldSize = _size;
        ListNodePosi<T> p = first(), q;
        while ((q = p->succ) != trailer) {  // ������β�ڱ�Ϊֹ
            if (p->data != q->data) {
                p = q;  // ���ظ������
            } else {
                remove(q);  // �ظ���ɾ�� q
            }
        }
        return oldSize - _size;  // ����ɾ���Ľڵ���
    }

    // ��ת��������ÿ���ڵ��ǰ���ͺ��ָ��
    void reverse() {
        ListNodePosi<T> p = header;
        // ����ÿ��ʵ�ʽڵ��ǰ���ͺ��
        for (int i = 0; i < _size; i++) {
            p = p->succ;
            swap(p->pred, p->succ);  // ����ָ��
        }
        // ����ͷ�ڱ���β�ڱ���ָ��
        swap(header->succ, trailer->pred);
        // �޸����һ���ڵ���β�ڱ�������
        swap(p->pred, p->succ);
    }

    // ������������ָ��汾������ÿ���ڵ�ִ�� visit ����
    void traverse(void (*visit)(T&)) {
        for (ListNodePosi<T> p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }

    // ����������������汾����֧�ָ����Ĳ��������״̬�ĺ�����
    template <typename VST>
    void traverse(VST& visit) {
        for (ListNodePosi<T> p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
};

// ���Ժ�������ӡ�ڵ����ݣ����ڱ������ԣ�
template <typename T>
void print(T& data) {
    cout << data << " ";
}

// ������������������
int main() {
    List<int> list;
    // �����������
    list.insertAsLast(3);
    list.insertAsLast(1);
    list.insertAsLast(2);

    cout << "ԭʼ����";
    list.traverse(print<int>);  // �����3 1 2

    // �������
    list.sort();
    cout << "\n�����";
    list.traverse(print<int>);  // �����1 2 3

    // ȥ�ز��ԣ�����ظ�Ԫ�أ�
    list.insertAsLast(2);
    list.uniquify();  // ��������ȥ��
    cout << "\nȥ�غ�";
    list.traverse(print<int>);  // �����1 2 3

    // ��ת����
    list.reverse();
    cout << "\n��ת��";
    list.traverse(print<int>);  // �����3 2 1

    return 0;
}
