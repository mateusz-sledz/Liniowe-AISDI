#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <boost/concept_check.hpp>

namespace aisdi {

    template<typename Type>
    class LinkedList {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;


    private:
        class element;
        element* first;
        element* last;
        size_type size;

    public:
        LinkedList() {
            size = 0;
            first = new element();
            last = new element(first,nullptr);
            first->next = last;
        }

        LinkedList(std::initializer_list<Type> l)
        {
            size = 0;
            first = new element();
            last = new element(first,nullptr);
            first->next = last;

            auto itt = l.begin();
            while(itt != l.end())
            {
                append(*itt);
                itt++;
            }
        }

        LinkedList(const LinkedList &other)
        {
            size = 0;
            first = new element();
            last = new element(first,nullptr);
            first->next = last;

            if(other.size == 0)
                return;
            const_iterator it = other.begin();
            while(it != other.end())
            {
                append(*it);
                it++;
            }
        }

        LinkedList(LinkedList &&other)
        {
              first = other.first;
            last = other.last;
            size = other.size;
            other.first = nullptr;
            other.last = nullptr;
            other.size = 0;
        }

        ~LinkedList()
        {
            while(first != nullptr)
            {
                element* temp = first;
                first = first->next;
                delete temp;
            }
            first = last = nullptr;
            size = 0;
        }

        LinkedList &operator=(const LinkedList &other) {
            if(this != &other) {


                element *next = first;
                while (next != nullptr) {
                    element *deleteMe = next;
                    next = next->next;
                    delete deleteMe;
                }
                first = last = nullptr;
                size = 0;
                first = new element();
                last = new element(first, nullptr);
                first->next = last;

                if (other.size == 0)
                    return *this;
                const_iterator it = other.cbegin();
                while (it != other.cend()) {
                    append(*it);
                    it++;
                }
            }
            return *this;
        }

		
        LinkedList &operator=(LinkedList &&other) {
          if(this != &other)
          {
              auto curr = first;
              while(curr != nullptr)
              {
                  element* temp = curr;
                  curr = curr->next;
                  delete temp;
              }
              first = other.first;
              last = other.last;
              size = other.size;
              other.first = nullptr;
              other.last = nullptr;
              other.size = 0;
          }
          return *this;
        }

        bool isEmpty() const {
            return (size == 0);
        }

        size_type getSize() const {
            return size;
        }

        void append(const Type &item) {
            if(size == 0)
            {
                element* new_el = new element(item,first,last);
                first->next = new_el;
                last->prev = new_el;
                size++;
                return;
            }
            element* new_el = new element(item,last->prev,last);
            new_el->prev->next = new_el;
            last->prev= new_el;
            size++;
        }

        void prepend(const Type &item) {
            if(size == 0)
            {
                append(item);
                return;
            }

            element* new_el = new element(item,first, first->next);
            first->next = new_el;
            new_el->next->prev = new_el;
            size++;
        }

        void insert(const const_iterator &insertPosition, const Type &item)
        {
            if(insertPosition == end())
            {
                append(item);
                return;
            }
            element* to_insert = new element(item, insertPosition.current->prev, insertPosition.current);
            to_insert->prev->next = to_insert;
            to_insert->next->prev = to_insert;
            size++;
        }

        Type popFirst() {
            if (size == 0)
                throw std::out_of_range("No element to pop");

            Type ret = first->next->value;
            element* deleteMe = first->next;
            first->next = first->next->next;
            first->next->prev = first;
            delete deleteMe;
            size--;
            return ret;
        }

        Type popLast() {
            if (size == 0)
                throw std::out_of_range("No element to pop");
            Type ret = last->prev->value;
            element* deleteMe = last->prev;
            last->prev = last->prev->prev;
            last->prev->next = last;
            delete deleteMe;
            size--;
            return ret;
        }

        void erase(const const_iterator &possition)
        {
            if(isEmpty())
                throw std::out_of_range("no element to erase");
            if(possition.current->next == nullptr)
                throw std::out_of_range("cant delete end");

            possition.current->prev->next = possition.current->next;
            possition.current->next->prev = possition.current->prev;
            delete possition.current;
            size--;
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
        {
            firstIncluded.current->prev->next = lastExcluded.current;
            lastExcluded.current->prev = firstIncluded.current->prev;

            size_t counter = 0;
            auto it = firstIncluded;
            element* temp;


            while(it != lastExcluded)
            {
                temp = it.current->next;
                delete it.current;
                it.current = temp;
                counter++;
            }
            size -= counter;

        }

        iterator begin() {
            return iterator(first->next , this);
        }

        iterator end() {
            return iterator(last, this);
        }

        const_iterator cbegin() const {
            return const_iterator(first->next, this);
        }

        const_iterator cend() const {
            return const_iterator(last, this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }
    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        element* current;
        const LinkedList<Type>* this_list;

        ConstIterator()= default;

        explicit ConstIterator(element* init, const LinkedList<Type>* init2) {
            current = init;
            this_list = init2;
        }

        reference operator*() const
        {
            if(current->next == nullptr)
                throw std::out_of_range("out of range");
            return (current->value);
        }

        ConstIterator &operator++()
        {
            auto p = this_list->end();
            if( p.current == current)
                throw std::out_of_range("Last item cannot be iterated ++");

            current = current->next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            auto p = this_list->end();
            if( p.current == current)
                throw std::out_of_range("Last item cannot be iterated ++");

            auto result = *this;
            current = current->next;
            return result;
        }

        ConstIterator &operator--()
        {
            if(current->prev->prev == nullptr)
                throw std::out_of_range("Last item cannot be iterated --");
            current = current->prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            if(current->prev->prev == nullptr)
                throw std::out_of_range("Last item cannot be iterated ++");

            auto result = *this;
            current = current->prev;
            return result;
        }

        ConstIterator operator+(difference_type d) const
        {
            auto p = this_list->end();
            if( p.current == current->next)
                throw std::out_of_range("out of range");

            auto result = *this;

            for(long i = 1; i <= d ; i++)
            {
                if(result.current->next == nullptr)
                    throw std::out_of_range("out of range");
                result.current = result.current->next;
            }
            return result;
        }

        ConstIterator operator-(difference_type d) const
        {
            if(current->prev->prev == nullptr)
                throw std::out_of_range("Last item cannot be iterated --");
            auto result = *this;

            for(long i = d; i >=1 ; i--)
            {
                if(current->prev->prev == nullptr)
                    throw std::out_of_range("out of range");
                result.current = result.current->prev;
            }

            return result;

        }

        bool operator==(const ConstIterator &other) const
        {
            return other.current == current;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return other.current != current;
        }
    };

    template <typename Type>
    class LinkedList<Type>::element{
    public:
        Type value;

        element* prev;
        element* next;

        element(element * previous = nullptr, element * next = nullptr):
                prev(previous), next(next)
        {}

        element(const Type& item, element * previous = nullptr, element * next = nullptr):
                value(item), prev(previous), next(next)
        {}

        ~element()= default;
    };


    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator() {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}


        Iterator(element* init, LinkedList<Type>* init2 ) :ConstIterator(init, init2)
        {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
