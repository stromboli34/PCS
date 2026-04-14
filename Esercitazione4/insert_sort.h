template<typename T>

void insertion_sort(std::vector<T>& vec ){
 for (size_t j=1; j<vec.size(); j++){
    T key = vec[j];
    int i= j-1;
    while (i>=0 && vec[i]>key){
        vec[i+1]=vec[i];
        i=i-1;
    }
    vec[i+1]=key;
 }

}