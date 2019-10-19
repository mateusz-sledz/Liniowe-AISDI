//
//  Created by Mateusz Sledz on 03.11.2018.
//  Copyright © 2018 Mateusz Sledz. All rights reserved.
//

#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <boost/concept_check.hpp>

namespace aisdi
{
    
    template <typename Type>
    class Vector
    {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;
        
        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

    private:
        pointer x;
        size_t size, preallocated;

    public:
        Vector()
        {
            x = new Type[20];
            preallocated = 20;
            size = 0;
        }

        Vector(std::initializer_list<Type> l)
        {
            size = l.size();
            preallocated = size +1;
            x = new Type[ preallocated ];
            auto itt = l.begin();
            for( size_type i = 0; i < size; i++ )
                x[i] = *(itt++);
        }
        
        Vector(const Vector& other)
        {
            preallocated = other.preallocated;
            x = new Type[preallocated];
            size = 0;
            for(size_t i = 0; i < other.size; i++ )
                {
                    x[i] = other.x[i] ;
                    size++;
                }
        }
        
        Vector(Vector&& other)
        {
            x = other.x;
            size = other.size;
            preallocated = other.preallocated;
            other.x = nullptr;
            other.size = 0;
            other.preallocated = 0;
        }
        
        ~Vector()
        {
            delete[] x;
        }
        
        Vector& operator=(const Vector& other)
        {
            size = other.size;
            preallocated = other.preallocated;
            Type* temp = new Type[preallocated];

            for(size_t i = 0; i < size; i++ )
            {
                temp[i] = other.x[i];
            }
            delete[] x;
            x = temp;
            return *this;
        }
        
        Vector& operator=(Vector&& other)
        {
            if(this != &other)
            {
                delete[] x;
                x = other.x;
                size = other.size;
                preallocated = other.preallocated;
                other.x = nullptr;
                other.size = 0;
                preallocated = 0;
            }
            return *this;
        }

        bool isEmpty() const
        {
            return size==0;
        }
        
        size_type getSize() const
        {
            return size;
        }
        
        void append(const Type& item)
        {
            x[size] = item;
            size++;
            if(size == preallocated)
                {
                    preallocated *= 2;
                    Type* temp = new Type[preallocated];
                    for(size_t i = 0; i < size; i++ )
                        temp[i] = x[i];
                    delete[] x;
                    x = temp;
                }
        }
        
        void prepend(const Type& item)
        {
            if(item == 0)
                throw std::out_of_range("element empty") ;

            if(size == 0)
            {
                x[0] = item;
                size++;
                return;
            }
            Type *temp = new Type[preallocated];
            size_t k = 1;
            for(size_t i = 0; i < size; i++ )
            {
                temp[k] = x[i];
                k++;
            }
            temp[0] = item;
            size++;
            delete[] x;
            x = temp;
            if(size == preallocated)
            {
                preallocated *= 2;
                Type *temp2 = new Type[preallocated];
                for(size_t i = 0; i < size; i++ )
                    temp2[i] = x[i];
                delete[] x;
                x = temp2;
            }
        }
        
        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if(insertPosition == cbegin())
            {
                prepend(item);
                return;
            }
            if(insertPosition == cend())
            {
                append(item);
                return;
            }

            auto r = cbegin();
            size_t p = 0;
            Type *temp = new Type[preallocated];
            for(auto i = r; i != insertPosition ; i++)
                {
                    temp[p] = *i;
                    p++;
                }
            temp[p] = item ;
            p++;
            auto t = cend();
            for(auto i = insertPosition; i != t ; i++ )
            {
                temp[p] = *i;
                p++;
            }
            delete[] x;
            x = temp;
            size++;

            if(size == preallocated)
            {
                preallocated *= 2;
                Type *temp2 = new Type[preallocated];
                for(size_t i = 0; i < size; i++)
                    temp2[i] = x[i];
                delete[] x;
                x = temp2;
            }
        }
        
        Type popFirst()
        {
            if(isEmpty())
            {
                throw std::out_of_range("No element to pop") ;
            }
            Type y ;
            y = x[0];
            size_t k = 1;
            size_t i = 0;
            do{
                x[i] = x[k];
                k++;
                i++;
            }while( k<size);
            size--;

            return y;
        }
        
        Type popLast()
        {
            if(isEmpty())
            {
                throw std::out_of_range("No element to pop") ;
            }
            size--;
            Type y;
            y = x[size];
            return y;
        }
        
        void erase(const const_iterator& possition)
        {
            if(isEmpty())
                throw std::out_of_range("No element to erase") ;

            if(possition == cend())
                throw std::out_of_range("Erase out of range") ;

            auto t = cbegin();
            size_t u = 0;
            while(t != possition)
            {
                u++;
                t++;
            }

            do{
                x[u] = x[u+1];
                u++;
            }while(u < size);
            size--;
        }
        
        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {
            if(isEmpty())
                throw std::out_of_range("No element to erase") ;
            if(firstIncluded == lastExcluded)
                return;
            auto t = cbegin();
            size_t u = 0;
            size_t p = 0;
            while(t != firstIncluded)
            {
                u++;
                t++;
            }
            p = u;
            while(t != lastExcluded)
            {
                t++;
                p++;
            }

            auto sz = size;
            size -= (p-u);
            do{
                x[u] = x[p];
                x[p].~Type();
                u++;
                p++;
            }while(p < sz);

        }
        
        iterator begin()
        {
            return iterator(x,this);
        }
        
        iterator end()
        {
            return iterator(x+ size, this);
        }
        
        const_iterator cbegin() const
        {
            return const_iterator(x,this);
        }
        
        const_iterator cend() const
        {
            return const_iterator(x + size,this);
        }
        
        const_iterator begin() const
        {
            return cbegin();
        }
        
        const_iterator end() const
        {
            return cend();
        }
    };
    
    template <typename Type>
    class Vector<Type>::ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer2 = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;
        using this_ptr = const Vector<Type>*;

        ConstIterator()
        {}
        
        explicit ConstIterator(pointer2 init, this_ptr init2)
        {
            current = init;
            curr_vector = init2;
        }
        
        reference operator*() const
        {
            if(*this == curr_vector->end())
                throw std::out_of_range("incorrect");
            return *current;
        }
        
        ConstIterator& operator++()
        {
            if((curr_vector->x + curr_vector->size) == current )
                throw std::out_of_range("No element to erase") ;
            current++;
            return *this;
        }
        
        ConstIterator operator++(int )
        {
            if((curr_vector->x + curr_vector->size) == current )
                throw std::out_of_range("No element to erase") ;

            auto old = *this;
            current++;
            return old;
        }
        
        ConstIterator& operator--()
        {
            if((curr_vector->x ) == current )
                throw std::out_of_range("No element to erase") ;

            current--;
            return *this;
        }
        
        ConstIterator operator--(int )
        {
            if((curr_vector->x ) == current )
                throw std::out_of_range("No element to decrease") ;

            current--;
            auto result = *this;
            result.current++;
            return result;
        }
        
        ConstIterator operator+(difference_type d) const
        {
             auto result = *this ;
             result.current += d;
             return result;
        }
        
        ConstIterator operator-(difference_type d) const
        {
            auto result = *this ;
            result.current -= d;
            return result;
        }
        
        bool operator==(const ConstIterator& other) const
        {
            return current == other.current;
        }
        
        bool operator!=(const ConstIterator& other) const
        {
            return current != other.current;
        }

    private:
        pointer2 current;
        this_ptr curr_vector;
    };

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
    {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;
        using this_ptr = Vector<Type>*;


        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator(pointer other, this_ptr init) :ConstIterator(other,init)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            return const_cast<reference>(ConstIterator::operator*());
        }
    };
}

#endif // AISDI_LINEAR_VECTOR_H
