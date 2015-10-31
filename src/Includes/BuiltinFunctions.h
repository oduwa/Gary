
#include <iostream>
#include <vector>

template<typename T> void printVector(std::vector<T> v)
{
    for (int i = 0; i < v.size(); i++){
        if(i == 0){
            /* First element */
            std::cout << "[" << v[i] << ", ";
        }
        else if(i == v.size()-1){
            /* Last element */
            std::cout << v[i] << "]" << std::endl;
        }
        else{
            /* Other elements */
            std::cout << v[i] << ", ";
        }
        
    }
}
