#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray {
private:
   char* array;
   int index;
   int size;
   bool usemutex;
public:
   SharedArray(int n, bool use) : size(n), index(0), usemutex(use) {
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray() {
      delete[] array;
   }
   void addChar(char c) {

      if (usemutex)
      {
         #pragma omp ordered
         {
            array[index] = c;
            spendSomeTime();
            index++;
         }
      }
      else
      {
         array[index] = c;
         spendSomeTime();
         index++;
      } 

   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 10000; i++) {
         for (int j = 0; j < 100; j++) {
            // These loops shouldn't be removed by the compiler
         }
      }
   }
};


class ArrayFiller {
private:
   static const int nThreads = 3;
   static const int nTimes = 20;
   SharedArray* array;
public:
   ArrayFiller(bool usemutex) {
      array = new SharedArray(nThreads * nTimes, usemutex);
   }
   void fillArrayConcurrently() {

      omp_set_num_threads(nThreads);

      #pragma omp parallel for schedule(runtime) ordered
      for (int i = 0; i < nThreads * nTimes; ++i){
         array->addChar('A' + omp_get_thread_num());
      }

   }
   void printStats() {
      std::cout << array->toString() << std::endl;
      for (int i = 0; i < nThreads; ++i)
         std::cout << (char) ('A'+i) << "="
                   << array->countOccurrences('A'+i) << " ";
      std::cout << std::endl;
   }
   ~ArrayFiller() {
      delete array;
   }

};


int main() {

   // RUNTIME
   std::cout << "\nCase 1: using runtime (with Mutex)" << std::endl;
   ArrayFiller m1(true);
   m1.fillArrayConcurrently();
   m1.printStats();

   std::cout << "\nCase 2: using runtime (without Mutex)" << std::endl;
   ArrayFiller m2(false);
   m2.fillArrayConcurrently();
   m2.printStats();

   // AUTO
   std::cout << "\nCase 3: using auto (with Mutex)" << std::endl;
   ArrayFiller m3(true);
   omp_set_schedule(omp_sched_auto, 0);
   m3.fillArrayConcurrently();
   m3.printStats();

   std::cout << "\nCase 4: using auto (without Mutex)" << std::endl;
   ArrayFiller m4(false);
   omp_set_schedule(omp_sched_auto, 0);
   m4.fillArrayConcurrently();
   m4.printStats();

   // STATIC
   std::cout << "\nCase 5: using static without chunk (with Mutex)" << std::endl;
   ArrayFiller m5(true);
   omp_set_schedule(omp_sched_static, 0);
   m5.fillArrayConcurrently();
   m5.printStats();

   std::cout << "\nCase 6: using static without chunk (without Mutex)" << std::endl;
   ArrayFiller m6(false);
   omp_set_schedule(omp_sched_static, 0);
   m6.fillArrayConcurrently();
   m6.printStats();

   std::cout << "\nCase 7: using static with chunk 8 (with Mutex)" << std::endl;
   ArrayFiller m7(true);
   omp_set_schedule(omp_sched_static, 8);
   m7.fillArrayConcurrently();
   m7.printStats();

   std::cout << "\nCase 8: using static with chunk 8 (without Mutex)" << std::endl;
   ArrayFiller m8(false);
   omp_set_schedule(omp_sched_static, 8);
   m8.fillArrayConcurrently();
   m8.printStats();

   // DYNAMIC
   std::cout << "\nCase 9: using dynamic without chunk (with Mutex)" << std::endl;
   ArrayFiller m9(true);
   omp_set_schedule(omp_sched_dynamic, 0);
   m9.fillArrayConcurrently();
   m9.printStats();

   std::cout << "\nCase 10: using dynamic without chunk (without Mutex)" << std::endl;
   ArrayFiller m10(false);
   omp_set_schedule(omp_sched_dynamic, 0);
   m10.fillArrayConcurrently();
   m10.printStats();

   std::cout << "\nCase 11: using dynamic with chunk 8 (with Mutex)" << std::endl;
   ArrayFiller m11(true);
   omp_set_schedule(omp_sched_dynamic, 8);
   m11.fillArrayConcurrently();
   m11.printStats();

   std::cout << "\nCase 12: using dynamic with chunk 8 (without Mutex)" << std::endl;
   ArrayFiller m12(false);
   omp_set_schedule(omp_sched_dynamic, 8);
   m12.fillArrayConcurrently();
   m12.printStats();

   // GUIDED
   std::cout << "\nCase 13: using guided without chunk (with Mutex)" << std::endl;
   ArrayFiller m13(true);
   omp_set_schedule(omp_sched_guided, 0);
   m13.fillArrayConcurrently();
   m13.printStats();

   std::cout << "\nCase 14: using guided without chunk (without Mutex)" << std::endl;
   ArrayFiller m14(false);
   omp_set_schedule(omp_sched_guided, 0);
   m14.fillArrayConcurrently();
   m14.printStats();

   std::cout << "\nCase 15: using guided with chunk 8 (with Mutex)" << std::endl;
   ArrayFiller m15(true);
   omp_set_schedule(omp_sched_guided, 8);
   m15.fillArrayConcurrently();
   m15.printStats();

   std::cout << "\nCase 16: using guided with chunk 8 (without Mutex)" << std::endl;
   ArrayFiller m16(false);
   omp_set_schedule(omp_sched_guided, 8);
   m16.fillArrayConcurrently();
   m16.printStats();
}

