#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>

#include "Vector.h"
#include "LinkedList.h"

namespace {
    void perfomTest(size_t amount) {
        using namespace std::chrono;
        if (amount > 100000000) amount = 100000000;
        else if (amount < 10) amount = 10;

        aisdi::LinkedList<int> list;
        aisdi::Vector<int> vector;

        int element = 12345;

        //append
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for (size_t count = 0; count <= amount; count++) {
            vector.append(element);
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto vector_time = duration_cast<milliseconds>(t2 - t1).count();

        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        for (size_t count = 0; count <= amount; count++) {
            list.append(element);
        }
        high_resolution_clock::time_point t4 = high_resolution_clock::now();

        auto list_time = duration_cast<milliseconds>(t4 - t3).count();

        std::cout << "Append " << amount << " elementow:" << std::endl << vector_time << " - wektor" << std::endl
                  << list_time << " - lista" << std::endl;

        //insert poczatek
        t1 = high_resolution_clock::now();
        vector.insert(vector.begin(), element);
        t2 = high_resolution_clock::now();

        vector_time = duration_cast<milliseconds>(t2 - t1).count();

        t3 = high_resolution_clock::now();
        list.insert(list.begin(), element);
        t4 = high_resolution_clock::now();

        list_time = duration_cast<milliseconds>(t4 - t3).count();

        std::cout << "Insert 2 elementow na poczatku:" << std::endl << vector_time << " - wektor" << std::endl
                  << list_time << " - lista" << std::endl;

        //insert koniec
        t1 = high_resolution_clock::now();
        vector.insert(vector.end(), element);
        t2 = high_resolution_clock::now();

        vector_time = duration_cast<milliseconds>(t2 - t1).count();

        t3 = high_resolution_clock::now();
        list.insert(list.end(), element);
        t4 = high_resolution_clock::now();

        list_time = duration_cast<milliseconds>(t4 - t3).count();

        std::cout << "Insert dwoch elementow na koncu:" << std::endl << vector_time << " - wektor" << std::endl
                  << list_time << " - lista" << std::endl;

        //erase
        t1 = high_resolution_clock::now();
        vector.erase(vector.begin(), vector.end());
        t2 = high_resolution_clock::now();

        vector_time = duration_cast<milliseconds>(t2 - t1).count();

        t3 = high_resolution_clock::now();
        list.erase(list.begin(), list.end());
        t4 = high_resolution_clock::now();

        list_time = duration_cast<milliseconds>(t4 - t3).count();

        std::cout << "Erase od poczatku do konca:" << std::endl << vector_time << " - wektor" << std::endl << list_time
                  << " - lista" << std::endl;

        //prepend
        amount /= 100;
        t1 = high_resolution_clock::now();
        for (size_t count = 0; count <= amount; count++) {
            vector.prepend(element);
        }
        t2 = high_resolution_clock::now();

        vector_time = duration_cast<milliseconds>(t2 - t1).count();

        t3 = high_resolution_clock::now();
        for (size_t count = 0; count <= amount; count++) {
            list.prepend(element);
        }
        t4 = high_resolution_clock::now();

        list_time = duration_cast<milliseconds>(t4 - t3).count();

        std::cout << "Prepending " << amount << " elements took" << std::endl << vector_time << " in vector"
                  << std::endl << list_time << " in list" << std::endl << std::endl << std::endl << std::endl;
    }

}

int main() {
    for (size_t amount = 1000000; amount <= 10000000; amount *= 10)
        perfomTest(amount);
    return 0;
}