#include <vector>


//bubblesort
template<typename T>
void bubble_sort(std::vector<T>& vec) {
    if (vec.size() < 2) return;  // niente da ordinare
    for (std::size_t i = 0; i + 1 < vec.size(); i++) { //uso std::size_t perchè è il tpo che mi viene restituito dal metodo size() e non int, poi confrontandoli mi darebbe un flag
        for (std::size_t j = vec.size() - 1; j > i; j--) { //attenzione a sottrarre a vec.size() che non ha i negativi essendo std::size_t
            if (vec[j] < vec[j-1]) {
                T keep= vec[j];
                vec[j]=vec[j-1];
                vec[j-1]=keep;
            }
        }
    }
}