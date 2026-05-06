
#include<utility>
#include<vector>
template <typename T>


int partition(std::vector<T>& vec,int p, int r){
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
void quick_sort(std::vector<T>& vec, int p, int r){
    if (p<r){
        int q=partition(vec,p,r);
        quick_sort(vec,p,q-1);
        quick_sort(vec,q+1,r);        

        
        

    }
    

}