#ifndef MRUDEQUE_H
#define MRUDEQUE_H

#include <QList>
#include <QtDebug>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE
QT_MODULE(Core)

template <class T>
class MRUDeque : public QList<T>
{
public:

    // Constructor, size is the number of items that is going to be in deque.
    // When size is reached, the item at the end of the deque will be popped.
    // It is forced to be a particular size so that the entire deque can be
    // iterated over to display a number (size) of the most recently inserted items.
    inline MRUDeque(int size) : maxSize(size) {this->size = 0;}
    inline ~MRUDeque() {}
    // push item type T on deque
    inline void push(const T &item) {
        if(this->size < maxSize)
        {
            QList<T>::push_front(item);
            size++;
        }
        else
        {
            QList<T>::pop_back();
            QList<T>::push_front(item);
        }
    }

private:
    // Stores the size of the deque.
    int maxSize;
    // index, gets up to maxSize and stays there for duration of Deque object
    int size;

};

QT_END_NAMESPACE
QT_END_HEADER

#endif // MRUDEQUE_H
