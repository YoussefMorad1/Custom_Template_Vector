#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

template<class T>
class OYVector {
private:
    size_t size_, capacity_;
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

    /// \brief initialize vector with sepcific capacity and size = 0
    OYVector(int = 0);

    /// \brief initialize vector with n elements from an array
    OYVector(T *, int);

    /// \brief copy constructor to copy from vector to another
    OYVector(const OYVector &);

    /// \brief destructor to de-allocate allocated memory
    ~OYVector();

    /// \brief copy assignment operator
    OYVector<T> &operator=(const OYVector<T> &);

    /// \brief move assignment operator
    OYVector<T> &operator=(OYVector<T> &&);

    /// \brief subscript of the vector using indexes
    T &operator[](int);

    /// \brief adding element to the end of the vector
    int push_back(T);

    /// \brief removing last element of the vector
    T pop_back();

    /// \brief remove the element at the parameter iterator
    void erase(iterator);

    /// \brief remove elements between the parameter iterators
    void erase(iterator, iterator);

    /// \brief clears all element in vector
    void clear();

    /// \brief insert element where the iterator points
    void insert(iterator, T);

    /// \brief return iterator at begin
    iterator begin();

    /// \brief return iterator at end
    iterator end();

    /// \brief comparison operators between vectors equal or not
    bool operator==(const OYVector<T> &);

    /// \brief comparison operators between vectors (lexographically)
    bool operator<(const OYVector<T> &);

    /// \brief comparison operators between vectors (lexographically)
    bool operator>(const OYVector<T> &);

    /// \brief return vector size
    int size() const {
        return size_;
    }

    /// \brief return vector current memory capacity (allocated)
    int capacity() {
        return capacity_;
    }

    /// \brief change size of the vector
    void resize(int);

    /// \brief check if vector is empty
    bool empty(){
        return (size_ == 0);
    }

    /// \brief print the vector
    friend std::ostream& operator<< <>(std::ostream&, OYVector<T>&);
};


/// \tparam T
/// \param n: new size to vector
template<class T>
void OYVector<T>::resize(int n) {
    if(n <= capacity_){
        size_ = n;
    }
    else {
        capacity_ = n;
        T *tmp = new T[capacity_];
        for (int i = 0; i < size_; ++i) {
            tmp[i] = data[i];
        }
        delete[] data;
        data = tmp;
        size_ = capacity_;
        tmp = nullptr;
    }
}

///
/// \tparam T
/// \param out
/// \param vec: vector to be printer
template<class T>
std::ostream& operator<<(std::ostream& out, OYVector<T>& vec){
    for (int i = 0; i < vec.size(); ++i) {
        out << vec[i] << ' ';
    }
    return out;
}

/// \tparam T
/// \param other
template<class T>
bool OYVector<T>::operator==(const OYVector<T> &other) {
    if (size_ != other.size_)
        return false;
    for (int i = 0; i < size_; ++i) {
        if (data[i] != other.data[i])
            return false;
    }
    return true;
}

/// \tparam T
/// \param other
template<class T>
bool OYVector<T>::operator<(const OYVector<T> &other) {
    for (int i = 0; i < std::min(size_, other.size_); ++i) {
        if (data[i] > other.data[i])
            return false;
        else if (data[i] < other.data[i])
            return true;
    }
    if (size_ < other.size_)
        return true;
    else
        return false;
}

/// \tparam T
/// \param other
template<class T>
bool OYVector<T>::operator>(const OYVector<T> &other) {
    return other < (*this);
}

/// \tparam T
/// \param it: iterator to the element to erase
template<class T>
void OYVector<T>::erase(iterator it) {
    OYVector<T> newVec(this->size_);
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

/// \tparam T
/// \param it1: iterator to begin of elements to delete
/// \param it2: iterator to last element to delete
template<class T>
void OYVector<T>::erase(iterator it1, iterator it2) {
    if(it2 < it1){
        return;
    }
    OYVector<T> newVec(this->size_);
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
void OYVector<T>::clear() {
    capacity_ = size_ = 0;
    delete[] data;
    data = nullptr;
}

/// \tparam T
/// \param it: iterator of place to insert
/// \param x: element value to be inserted
template<class T>
void OYVector<T>::insert(iterator it, T x) {
    OYVector<T> newVec(this->size_ + 1);
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
typename OYVector<T>::iterator OYVector<T>::begin() {
    return iterator(data);
}

template<class T>
typename OYVector<T>::iterator OYVector<T>::end() {
    return iterator(data + size_);
}

template<class T>
OYVector<T>::OYVector(int x) {
    capacity_ = x;
    data = new T[capacity_];
    size_ = 0;
}

/// \tparam T*: array_pointer
/// \param int: size of array
template<class T>
OYVector<T>::OYVector(T *arr, int n) {
    capacity_ = size_ = n;
    data = new T[capacity_];
    for (int i = 0; i < n; ++i) {
        data[i] = arr[i];
    }
}

template<class T>
OYVector<T>::OYVector(const OYVector<T> &other) {
    capacity_ = size_ = other.size_;
    data = new T[capacity_];
    for (int i = 0; i < other.size_; ++i) {
        data[i] = other.data[i];
    }
}

template<class T>
OYVector<T>::~OYVector() {
    delete[] data;
    data = nullptr;
}

template<class T>
OYVector<T> &OYVector<T>::operator=(const OYVector<T> &other) {
    if ((*this) == other)
        return (*this);
    delete[] data;

    capacity_ = size_ = other.size_;
    data = new T[capacity_];
    for (int i = 0; i < size_; ++i) {
        data[i] = other.data[i];
    }
}

template<class T>
OYVector<T> &OYVector<T>::operator=(OYVector<T> &&other) {
    if ((*this) == other)
        return (*this);
    delete[] data;

    capacity_ = size_ = other.size_;
    data = new T[capacity_];
    for (int i = 0; i < size_; ++i) {
        data[i] = other.data[i];
    }

    delete[] other.data;
}

template<class T>
T &OYVector<T>::operator[](int i) {
    return data[i];
}

template<class T>
int OYVector<T>::push_back(T x) {
    if (size_ == capacity_) {
        capacity_ = 2 * capacity_;
        if(!capacity_)
            capacity_ = 1;
        T *tmp = new T[capacity_];
        for (int i = 0; i < size_; ++i) {
            tmp[i] = data[i];
        }
        delete[] data;
        data = tmp;
        tmp = nullptr;
    }
    data[size_] = x;
    size_++;
    return size_;
}

template<class T>
T OYVector<T>::pop_back() {
    size_--;
    return data[size_];
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

    OYVector<int> v;
    for (int i = 0; i < 15; ++i) {
        v.push_back(i);
    }

    v = std::move(v);

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
