
template<typename T>

void insertion_sort_var(std::vector<T>& vec, int p, int r ){
 for (int j=p+1;j<=r; j++){
    T key = vec[j];
    int i= j-1;
    while (i>=p && vec[i]>key){
        vec[i+1]=vec[i];
        i=i-1;
    }
    vec[i+1]=key;
 }

}