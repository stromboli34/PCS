
#include<utility>
#include<vector>
#include "insert_sort_var.h"
template <typename T>


int partition_v2(std::vector<T>& vec,int p, int r){
    T x=vec[r];
    int i=p-1;
    for (int j=p;j<r;j++){
        if (vec[j]<=x){
            i=i+1;
            std::swap(vec[i],vec[j]);

        }
    }
    std::swap(vec[i+1],vec[r]);
    return i+1;


}

template <typename T>
void quick_sort_mod(std::vector<T>& vec, int p, int r){
    if (p<r){
        int size_iter= r-p+1;
        if(size_iter>=35){
        int q=partition_v2(vec,p,r);
        quick_sort_mod(vec,p,q-1);
        quick_sort_mod(vec,q+1,r); 
        }
        else{

        insertion_sort_var(vec,p,r);   
        }    
        

    }
    

}