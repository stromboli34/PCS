
#include <iostream>
#include <vector>
#include "timecounter.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "insert_sort.h"
#include "bubble_sort.h"
#include "select_sort.h"
#include "quick_sort_v2.h"
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
    for (int dim=4;dim<=100;dim++){
        std::vector<std::vector<int>> vec_1;
        for (int k=0;k<=99;k++){
            std::vector<int>  vec_2= genera_vettore(dim);
            vec_1.push_back(vec_2);
        }       
     std::vector<std::vector<int>> vec_a = vec_1;
     std::vector<std::vector<int>> vec_b = vec_1;
     std::vector<std::vector<int>> vec_c = vec_1;
     std::vector<std::vector<int>> vec_d =vec_1;
     std::vector<std::vector<int>> vec_e =vec_1;
     std::vector<std::vector<int>> vec_f =vec_1;


     t.tic();
     for (int k=0;k<=99;k++){
        bubble_sort(vec_a[k]);
     }
     double elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Bubble: " << elapsed/97 << "\n";
     
      t.tic();
     for (int k=0;k<=99;k++){
        insertion_sort(vec_b[k]);
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Insertion: "<< elapsed/97 << "\n";

      t.tic();
     for (int k=0;k<=99;k++){
        selection_sort(vec_c[k]);
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Selection: "<< elapsed/97 << "\n";    

    

    t.tic();
     for (int k=0;k<=99;k++){
        quick_sort(vec_d[k],0,dim-1);
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Quick: "<< elapsed/97 << "\n";

      t.tic();
     for (int k=0;k<=99;k++){
        merge_sort(vec_e[k],0,dim-1);
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Merge: "<< elapsed/97 << "\n";
    

     t.tic();
     for (int k=0;k<=99;k++){
        std::sort(vec_1[k].begin(),vec_1[k].end());
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Sort Nativo: "<< elapsed/97 << "\n";
     t.tic();
     for (int k=0;k<=99;k++){
        quick_sort_mod(vec_f[k],0,dim-1);
     }
      elapsed=t.toc();
      std::cout << "Dimensione: " << dim << " " << "Media Quick Modificato: "<< elapsed/97 << "\n";

    }
    return 0;
    }



    

        
    
   