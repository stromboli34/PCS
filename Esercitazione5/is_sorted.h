
template<typename T>

bool is_sorted(const std::vector<T>& vec){
    if (vec.size()<=2){
        return true;
    
    }
    else{
        for (size_t i=0; i<vec.size()-1;i++ ){
            if (vec[i+1]<vec[i]){
                return false;
            }
        }
    }
    return true;
}
