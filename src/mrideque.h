#ifndef MRIDEQUE_H
#define MRIDEQUE_H

#include <QList>
#include <QtDebug>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE
QT_MODULE(Core)

template <class T>
class MRIDeque : public QList<T>
{
public:

    // Constructor, maxSize is the number of items that is going to be in deque.
    // When maxSize is reached, the item at the end of the deque will be popped.
    // It is forced to be a particular size so that the entire deque can be
    // iterated over to display the most recently inserted items.
    inline MRIDeque(int size) : maxSize(size){}
    inline ~MRIDeque() {}
    // push item type T on deque
    inline void push(const T &item) {
        if(this->count() < maxSize)
        {
            QList<T>::push_front(item);
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
};

QT_END_NAMESPACE
QT_END_HEADER

#endif // MRIDEQUE_H
