template<typename T>

void selection_sort(std::vector<T>& vec){
    for (size_t i=0;i<vec.size();i++){
        size_t min=i;
        for (size_t j=i+1; j<vec.size();j++){
            if(vec[j]<vec[min]){
                min=j;
            }
        }
        T keep = vec[i];
        vec[i]=vec[min];
        vec[min]=keep;
    }


}