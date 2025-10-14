#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <algorithm> // ����std::swap
#include <stdexcept> // �����쳣����

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
class Vector {
protected:
    Rank _size;       // ��ǰԪ�ظ���
    int _capacity;    // ����
    T* _elem;         // Ԫ������

    // ������A��[lo, hi)���临��Ԫ��
    void copyFrom(const T* A, Rank lo, Rank hi) {
        _capacity = 2 * (hi - lo);  // ��ʼ������ΪԪ�ظ�����2��
        _elem = new T[_capacity];
        _size = 0;
        while (lo < hi) {
            _elem[_size++] = A[lo++];  // ����Ԫ��
        }
    }

    // ���ݲ�������������ʱ��
    void expand() {
        if (_size < _capacity) return;  // ��������
        _capacity = std::max(_capacity, DEFAULT_CAPACITY);  // ȷ����С����
        T* oldElem = _elem;
        _elem = new T[_capacity <<= 1];  // ��������
        for (Rank i = 0; i < _size; ++i) {
            _elem[i] = oldElem[i];  // ���ƾ�Ԫ��
        }
        delete[] oldElem;  // �ͷžɿռ�
    }

    // ���ݲ�������������ʱ��
    void shrink() {
        if (_capacity <= DEFAULT_CAPACITY || _size * 4 > _capacity) return;  // ��������
        T* oldElem = _elem;
        _elem = new T[_capacity >>= 1];  // ��������
        for (Rank i = 0; i < _size; ++i) {
            _elem[i] = oldElem[i];  // ���ƾ�Ԫ��
        }
        delete[] oldElem;  // �ͷžɿռ�
    }

    // ð�������һ��ɨ�裨[lo, hi)��
    bool bubble(Rank lo, Rank hi) {
        bool sorted = true;
        while (++lo < hi) {
            if (_elem[lo - 1] > _elem[lo]) {
                std::swap(_elem[lo - 1], _elem[lo]);  // ���������
                sorted = false;
            }
        }
        return sorted;
    }

    // ð������[lo, hi)��
    void bubbleSort(Rank lo, Rank hi) {
        while (!bubble(lo, hi--));  // ����ɨ�裬ֱ������
    }

    // ����[lo, hi)��������Ԫ������
    Rank max(Rank lo, Rank hi) {
        Rank maxIdx = lo;
        for (Rank i = lo + 1; i < hi; ++i) {
            if (_elem[i] > _elem[maxIdx]) {
                maxIdx = i;
            }
        }
        return maxIdx;
    }

    // ѡ������[lo, hi)��
    void selectionSort(Rank lo, Rank hi) {
        while (lo < --hi) {
            std::swap(_elem[max(lo, hi)], _elem[hi]);  // �����Ԫ�طŵ�ĩβ
        }
    }

    // �鲢�������ϲ�[lo, mi)��[mi, hi)��
    void merge(Rank lo, Rank mi, Rank hi) {
        T* temp = new T[hi - lo];  // ��ʱ����
        Rank i = lo, j = mi, k = 0;
        while (i < mi && j < hi) {  // �ϲ�������������
            temp[k++] = (_elem[i] <= _elem[j]) ? _elem[i++] : _elem[j++];
        }
        while (i < mi) temp[k++] = _elem[i++];  // ����ʣ��Ԫ��
        while (j < hi) temp[k++] = _elem[j++];
        for (i = lo, k = 0; i < hi; ++i) {  // ���ƻ�ԭ����
            _elem[i] = temp[k++];
        }
        delete[] temp;  // �ͷ���ʱ�ռ�
    }

    // �鲢����[lo, hi)��
    void mergeSort(Rank lo, Rank hi) {
        if (hi - lo < 2) return;  // ����Ԫ����Ȼ����
        Rank mi = (lo + hi) / 2;
        mergeSort(lo, mi);        // ��벿������
        mergeSort(mi, hi);        // �Ұ벿������
        merge(lo, mi, hi);        // �鲢
    }

    // ��������Ļ��ֲ�����[lo, hi)��
    Rank partition(Rank lo, Rank hi) {
        std::swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);  // ���ѡ���׼Ԫ��
        T pivot = _elem[lo];
        --hi;  // ��hi-1��ʼ
        while (lo < hi) {
            while (lo < hi && pivot <= _elem[hi]) --hi;  // ����������С�ڻ�׼��Ԫ��
            _elem[lo] = _elem[hi];
            while (lo < hi && _elem[lo] <= pivot) ++lo;  // ���������Ҵ��ڻ�׼��Ԫ��
            _elem[hi] = _elem[lo];
        }
        _elem[lo] = pivot;  // ��׼Ԫ�ع�λ
        return lo;          // ���ػ�׼λ��
    }

    // ��������[lo, hi)��
    void quickSort(Rank lo, Rank hi) {
        if (hi - lo < 2) return;  // ����Ԫ����Ȼ����
        Rank pivot = partition(lo, hi);
        quickSort(lo, pivot);     // ��벿������
        quickSort(pivot + 1, hi); // �Ұ벿������
    }

    // �������������˲���
    void heapify(Rank i, Rank n) {
        while (true) {
            Rank left = 2 * i + 1;
            Rank right = 2 * i + 2;
            Rank largest = i;
            if (left < n && _elem[left] > _elem[largest]) largest = left;
            if (right < n && _elem[right] > _elem[largest]) largest = right;
            if (largest != i) {
                std::swap(_elem[i], _elem[largest]);
                i = largest;
            } else {
                break;
            }
        }
    }

    // ������[lo, hi)��
    void heapSort(Rank lo, Rank hi) {
        int n = hi - lo;
        // ��������
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(lo + i, lo + n);
        }
        // ��ȡ���Ԫ�ز�������
        for (int i = n - 1; i > 0; --i) {
            std::swap(_elem[lo], _elem[lo + i]);
            heapify(lo, lo + i);
        }
    }

public:
    // ���캯����ָ����������ʼԪ�ظ����ͳ�ʼֵ
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        if (s > c) throw std::invalid_argument("��ʼԪ�ظ������ܳ�������");
        _capacity = c;
        _elem = new T[_capacity];
        _size = 0;
        while (_size < s) {
            _elem[_size++] = v;
        }
    }

    // �����鹹�죨n��Ԫ�أ�
    Vector(const T* A, Rank n) {
        copyFrom(A, 0, n);
    }

    // �������[lo, hi)���乹��
    Vector(const T* A, Rank lo, Rank hi) {
        if (lo < 0 || hi > n || lo >= hi) throw std::invalid_argument("��Ч������");
        copyFrom(A, lo, hi);
    }

    // ���ƹ��죨����������
    Vector(const Vector<T>& V) {
        copyFrom(V._elem, 0, V._size);
    }

    // ���ƹ��죨������[lo, hi)���䣩
    Vector(const Vector<T>& V, Rank lo, Rank hi) {
        if (lo < 0 || hi > V._size || lo >= hi) throw std::invalid_argument("��Ч������");
        copyFrom(V._elem, lo, hi);
    }

    // ��������
    ~Vector() {
        delete[] _elem;
    }

    // ��ȡ��ǰԪ�ظ���
    Rank size() const {
        return _size;
    }

    // �ж��Ƿ�Ϊ��
    bool empty() const {
        return _size == 0;
    }

    // ��������Ƿ����򣨷��������������
    int disordered() const {
        int cnt = 0;
        for (Rank i = 1; i < _size; ++i) {
            if (_elem[i - 1] > _elem[i]) cnt++;
        }
        return cnt;
    }

    // ����Ԫ��e������������
    Rank find(const T& e) const {
        return find(e, 0, _size);
    }

    // ����[lo, hi)�����Ԫ��e�������״γ��ֵ�������δ�ҵ�����-1��
    Rank find(const T& e, Rank lo, Rank hi) const {
        if (lo < 0 || hi > _size || lo >= hi) return -1;
        while (lo < hi) {
            if (_elem[lo] == e) return lo;
            lo++;
        }
        return -1;
    }

    // ���������в���Ԫ��e������������
    Rank search(const T& e) const {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }

    // ���������в���[lo, hi)�����Ԫ��e�����ֲ��ң�
    Rank search(const T& e, Rank lo, Rank hi) const {
        if (lo < 0 || hi > _size || lo >= hi) return -1;
        while (lo < hi) {
            Rank mi = (lo + hi) / 2;
            if (e < _elem[mi]) {
                hi = mi;
            } else {
                lo = mi + 1;
            }
        }
        return --lo;  // ���ز�����e�����Ԫ������
    }

    // ����[]�����������Ԫ�أ�
    T& operator[](Rank r) const {
        if (r < 0 || r >= _size) throw std::out_of_range("����Խ��");
        return _elem[r];
    }

    // ����=�������������ֵ��
    Vector<T>& operator=(const Vector<T>& V) {
        if (_elem) delete[] _elem;  // �ͷŵ�ǰ�ռ�
        copyFrom(V._elem, 0, V._size);  // ����������
        return *this;
    }

    // ɾ������r����Ԫ�أ����ر�ɾ��Ԫ�أ�
    T remove(Rank r) {
        if (r < 0 || r >= _size) throw std::out_of_range("����Խ��");
        T e = _elem[r];
        for (Rank i = r; i < _size - 1; ++i) {
            _elem[i] = _elem[i + 1];  // Ԫ��ǰ��
        }
        _size--;
        shrink();  // ����Ƿ���Ҫ����
        return e;
    }

    // ɾ��[lo, hi)�����Ԫ�أ�����ɾ����Ԫ�ظ�����
    int remove(Rank lo, Rank hi) {
        if (lo < 0 || hi > _size || lo >= hi) return 0;
        int cnt = hi - lo;
        while (hi < _size) {
            _elem[lo++] = _elem[hi++];  // Ԫ��ǰ��
        }
        _size -= cnt;
        shrink();  // ����Ƿ���Ҫ����
        return cnt;
    }

    // ������r������Ԫ��e�����ز���λ�ã�
    Rank insert(Rank r, const T& e) {
        if (r < 0 || r > _size) throw std::out_of_range("����λ��Խ��");
        expand();  // ����Ƿ���Ҫ����
        for (Rank i = _size; i > r; --i) {
            _elem[i] = _elem[i - 1];  // Ԫ�غ���
        }
        _elem[r] = e;
        _size++;
        return r;
    }

    // ��ĩβ����Ԫ��e�����ز���λ�ã�
    Rank insert(const T& e) {
        return insert(_size, e);
    }

    // ����[lo, hi)���䣨Ĭ��ʹ�ÿ�������
    void sort(Rank lo, Rank hi) {
        if (lo < 0 || hi > _size || lo >= hi) return;
        // �ɸ�����Ҫ�л������㷨
        quickSort(lo, hi);
        // ������ѡ�㷨��
        // mergeSort(lo, hi);
        // bubbleSort(lo, hi);
        // selectionSort(lo, hi);
        // heapSort(lo, hi);
    }

    // ������������
    void sort() {
        sort(0, _size);
    }

    // ����[lo, hi)�����Ԫ��
    void unsort(Rank lo, Rank hi) {
        if (lo < 0 || hi > _size || lo >= hi) return;
        for (Rank i = hi - 1; i > lo; --i) {
            std::swap(_elem[i], _elem[lo + rand() % (i - lo + 1)]);  // �������
        }
    }

    // ������������
    void unsort() {
        unsort(0, _size);
    }

    // ��������ȥ�أ�����ɾ����Ԫ�ظ�����
    int deduplicate() {
        int oldSize = _size;
        Rank i = 1;
        while (i < _size) {
            if (find(_elem[i], 0, i) == -1) {
                i++;  // δ�ҵ��ظ�Ԫ�أ�����
            } else {
                remove(i);  // �ҵ��ظ�Ԫ�أ�ɾ��
            }
        }
        return oldSize - _size;
    }

    // ��������ȥ�أ�����ɾ����Ԫ�ظ�����
    int uniquify() {
        if (_size < 2) return 0;
        Rank i = 0, j = 1;
        while (j < _size) {
            if (_elem[i] != _elem[j]) {
                _elem[++i] = _elem[j];  // �������ظ�Ԫ��
            }
            j++;
        }
        int cnt = _size - (i + 1);
        _size = i + 1;
        shrink();  // ����Ƿ���Ҫ����
        return cnt;
    }

    // ��������������ָ��汾��
    void traverse(void (*visit)(T&)) {
        for (Rank i = 0; i < _size; ++i) {
            visit(_elem[i]);
        }
    }

    // ������������������汾��
    template <typename VST>
    void traverse(VST& visit) {
        for (Rank i = 0; i < _size; ++i) {
            visit(_elem[i]);
        }
    }
};

#endif // VECTOR_H
