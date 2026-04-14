
#include <vector>
#include "bubble_sort.h"
#include "is_sorted.h"
#include "randfiller.h"
#include <random>


std::vector<int> genera_vettore(){
    randfiller rf;
        std::random_device rd; 

        static std::mt19937 gen(std::random_device{}()); 
    
        std::uniform_int_distribution<int> dist(4, 8192); 
    
        int lunghezza = dist(gen);

        std::vector<int> vec(lunghezza);
        rf.fill(vec,-22322124,5343412);
        return(vec);
}

int main(){

    for (int i=0;i<100;i++){

        std::vector<int> vec = genera_vettore();
       

         bubble_sort(vec);
         if (!is_sorted(vec)){
            return EXIT_FAILURE;
         }
    std::vector<std::string> vec_s;
    vec_s ={"ciao", "come stai", "nonno", "forte","veleggiare"};
     
      
      bubble_sort(vec_s);
      if (!is_sorted(vec_s)){
        return EXIT_FAILURE;
      }




}
return EXIT_SUCCESS;
}

