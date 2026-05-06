template<typename T>
void merge(std::vector<T>& vec, int p, int q, int r) {
    int n1 = q-p+1;
    int n2 = r-q;
    std::vector<T> L(n1); 
    std::vector<T> R(n2);
    for (int i = 0; i < n1 ; i++) {
        L[i] = vec[p+i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = vec[q+1+j];
    }
    int i = 0, j = 0, k = p;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) { 
        vec[k] = R[j];
        j++;
        k++;
    }
}

template<typename T>
void merge_sort(std::vector<T>& vec, int p, int r) { 
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(vec, p, q);
        merge_sort(vec, q + 1, r);
        merge(vec, p, q, r);
    }
}
