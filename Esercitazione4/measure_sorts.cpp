#include <iostream>
#include <vector>
#include "timecounter.h"
#include "bubble_sort.h"
#include "insert_sort.h"
#include "select_sort.h"
#include "randfiller.h"
#include <cmath>
#include <algorithm>
 std::vector<int> genera_vettore(int lunghezza){
    randfiller rf;    
        
    std::vector<int> vec(lunghezza);
    rf.fill(vec,-22322124,5343412);
    return(vec);
        
}


int main(){
    timecounter t;

    for (int k=2;k<=13;k++){
    std::vector<int> vec= genera_vettore(pow(2,k));
    std::vector<int> vec_1=vec;
    std::vector<int> vec_2=vec;
    std::vector<int> vec_3=vec;

    t.tic();
    bubble_sort(vec_1);
    double elapsed = t.toc();
    std::cout << "Lunghezza: " << k << " Bubble: " << elapsed << "\n";
    t.tic();
    insertion_sort(vec_2);
    elapsed = t.toc();
    std::cout << "Lunghezza: " << k << " Insertion: " << elapsed << "\n";
    t.tic();
    selection_sort(vec_3);
    elapsed = t.toc();
    std::cout << "Lunghezza: " << k << " Selection: " << elapsed << "\n";
    t.tic();
    std::sort(vec.begin(),vec.end());
    elapsed = t.toc();
    std::cout << "Lunghezza: " << k << " Sort Nativo: " << elapsed << "\n";
    
    





    }

   
return 0;

}