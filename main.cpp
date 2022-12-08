#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

template<class T>
class YVector {
private:
    size_t size, capacity;
    T *data;
public:
    class iterator : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        T *ptr;
    public:
        iterator(T *ptr) : ptr(ptr) {}

        iterator operator+(int n) {
            return iterator(ptr + n);
        }

        T &operator*() {
            return (*ptr);
        }

        iterator &operator++() { // prefix
            ptr++;
            return *this;
        }

        iterator operator++(int) { //postfix
            iterator temp = *this;
            ptr++;
            return temp;
        }

        bool operator==(iterator other) {
            return (ptr == other.ptr);
        }

        bool operator<(iterator other){
            return ptr < other.ptr;
        }
    };

    YVector(int = 0);

    YVector(T *, int);

    YVector(const YVector &);

    ~YVector();

    YVector<T> &operator=(const YVector<T> &);

    YVector<T> &operator=(YVector<T> &&);

    T &operator[](int);

    int push_back(T);

    T pop_back();

    void erase(iterator);

    void erase(iterator, iterator);

    void clear();

    void insert(iterator, T);

    iterator begin();

    iterator end();

    bool operator==(const YVector<T> &);

    bool operator<(const YVector<T> &);

    bool operator>(const YVector<T> &);

    int size_() const {
        return size;
    }

    int capacity_() {
        return capacity;
    }

    void resize(int);

    bool empty(){
        return (size == 0);
    }

    friend std::ostream& operator<< <>(std::ostream&, YVector<T>&);
};

template<class T>
void YVector<T>::resize(int n) {
    if(n <= capacity){
        size = n;
    }
    else {
        capacity = n;
        T *tmp = new T[capacity];
        for (int i = 0; i < size; ++i) {
            tmp[i] = data[i];
        }
        delete[] data;
        data = tmp;
        size = capacity;
        tmp = nullptr;
    }
}


template<class T>
std::ostream& operator<<(std::ostream& out, YVector<T>& vec){
    for (int i = 0; i < vec.size_(); ++i) {
        out << vec[i] << ' ';
    }
    return out;
}

template<class T>
bool YVector<T>::operator==(const YVector<T> &other) {
    if (size != other.size)
        return false;
    for (int i = 0; i < size; ++i) {
        if (data[i] != other.data[i])
            return false;
    }
    return true;
}

template<class T>
bool YVector<T>::operator<(const YVector<T> &other) {
    for (int i = 0; i < std::min(size, other.size); ++i) {
        if (data[i] > other.data[i])
            return false;
        else if (data[i] < other.data[i])
            return true;
    }
    if (size < other.size)
        return true;
    else
        return false;
}

template<class T>
bool YVector<T>::operator>(const YVector<T> &other) {
    return other < (*this);
}

template<class T>
void YVector<T>::erase(iterator it) {
    YVector<T> newVec(this->size);
    bool found = false;
    for (iterator _it = this->begin(); _it != this->end(); _it++) {
        if (_it == it) {
            found = true;
            continue;
        }
        newVec.push_back(*_it);
    }
    if (!found) {
        throw std::invalid_argument("iterator is outside range\n");
    }
    (*this) = std::move(newVec);
}

template<class T>
void YVector<T>::erase(iterator it1, iterator it2) {
    if(it2 < it1){
        return;
    }
    YVector<T> newVec(this->size);
    bool found1 = false, found2 = false;
    for (iterator _it = this->begin(); _it != this->end(); _it++) {
        if (_it == it1) {
            found1 = true;
        }
        if (_it == it2) {
            found2 = true;
        }
        if ((found1 && found2) || (!found1 && !found2))
            newVec.push_back(*_it);
    }
    if(it2 == this->end())
        found2 = true;
    if (!found1 || !found2) {
        std::ostringstream s;
        s << "iterator " << (found1 ? "end" : "start") << " is outside range\n";
        throw std::invalid_argument(s.str());
    }
    (*this) = std::move(newVec);
}


template<class T>
void YVector<T>::clear() {
    capacity = size = 0;
    delete[] data;
    data = nullptr;
}

template<class T>
void YVector<T>::insert(iterator it, T x) {
    YVector<T> newVec(this->size + 1);
    bool found = false;
    for (iterator _it = this->begin(); _it != this->end(); _it++) {
        if (_it == it) {
            found = true;
            newVec.push_back(x);
        }
        newVec.push_back(*_it);
    }
    if(it == this->end()){
        found = true;
        newVec.push_back(x);
    }
    if (!found) {
        throw std::invalid_argument("iterator is outside range\n");
    }
    (*this) = std::move(newVec);
}

template<class T>
typename YVector<T>::iterator YVector<T>::begin() {
    return iterator(data);
}

template<class T>
typename YVector<T>::iterator YVector<T>::end() {
    return iterator(data + size);
}

template<class T>
YVector<T>::YVector(int x) {
    capacity = x;
    data = new T[capacity];
    size = 0;
}

template<class T>
YVector<T>::YVector(T *arr, int n) {
    capacity = size = n;
    data = new T[capacity];
    for (int i = 0; i < n; ++i) {
        data[i] = arr[i];
    }
}

template<class T>
YVector<T>::YVector(const YVector<T> &other) {
    capacity = size = other.size;
    data = new T[capacity];
    for (int i = 0; i < other.size; ++i) {
        data[i] = other.data[i];
    }
}

template<class T>
YVector<T>::~YVector() {
    delete[] data;
    data = nullptr;
}

template<class T>
YVector<T> &YVector<T>::operator=(const YVector<T> &other) {
    if ((*this) == other)
        return (*this);
    delete[] data;

    capacity = size = other.size;
    data = new T[capacity];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template<class T>
YVector<T> &YVector<T>::operator=(YVector<T> &&other) {
    if ((*this) == other)
        return (*this);
    delete[] data;

    capacity = size = other.size;
    data = new T[capacity];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }

    delete[] other.data;
}

template<class T>
T &YVector<T>::operator[](int i) {
    return data[i];
}

template<class T>
int YVector<T>::push_back(T x) {
    if (size == capacity) {
        capacity = 2 * capacity;
        if(!capacity)
            capacity = 1;
        T *tmp = new T[capacity];
        for (int i = 0; i < size; ++i) {
            tmp[i] = data[i];
        }
        delete[] data;
        data = tmp;
        tmp = nullptr;
    }
    data[size] = x;
    size++;
    return size;
}

template<class T>
T YVector<T>::pop_back() {
    size--;
    return data[size];
}

int main() {
//    {
//        using namespace std;
//        std::vector<int> vec = {1, 2, 3, 4, 5};
//        vec.erase(vec.begin() + 2, vec.end());
//        for (int i = 0; i < vec.size(); ++i) {
//            cout << vec[i] << ' ';
//        }
//        cout << endl;
//    }

    YVector<int> v;
    for (int i = 0; i < 15; ++i) {
        v.push_back(i);
    }
    std::cout << v << std::endl;

    v.erase(v.begin()+3);
    std::cout << v << std::endl;

    v.erase(v.begin(), v.begin()+3);
    std::cout << v << std::endl;

    v.erase(v.begin() + 3, v.end());
    std::cout << v << std::endl;

    v.clear();
    std::cout << v << std::endl;

    for (int i = 0; i < 3; ++i) {
        v.push_back(i);
    }
    std::cout << v << std::endl;
    v.insert(v.begin(), 100);
    std::cout << v << std::endl;
    v.insert(v.begin()+3, 100);
    std::cout << v << std::endl;
    v.insert(v.end(), 100);
    std::cout << v << std::endl;
}
