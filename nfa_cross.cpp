#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> build(const std::vector<int>& sequence, const std::vector<int>& permutation){
    std::vector<int> result;
    std::vector<int> array;
    int i = 0;
    array.insert(array.end(), sequence.begin(), sequence.end());
    array.push_back(0);
    int j = array.size();
    int j_start = j;
    array.insert(array.end(), permutation.begin(), permutation.end());
    array.push_back(0);

    while(array[i] != 0 || array[j] != 0){
        if (array[i] == array[j]){
            result.push_back(array[i]);
            i++;
            j++;
        }
        else if (array[j] == 0){
            result.push_back(array[i]);
            i++;
        }
        else if (i == 0 || array[i] == 0){
            result.push_back(array[j]);
            j++;
        }
        else if (j != j_start){
            i = 0;
        }
    }

    return result;
}

std::vector<int> superpermutation(int n){
    std::vector<int> permutation(n);
    for (int i = 0; i < n; i++){
        permutation[i] = i + 1; //Permutations start with 1
    }

    std::vector<std::vector<int>> permutations;
    std::vector<int> result = permutation;

    while (std::next_permutation(permutation.begin(), permutation.end())){
        result = build(result, permutation);
    }

    return result;
}

int main(){
    int n = 3;
    auto super = superpermutation(n);
    std::cout << "Superpermutation for n = " << n << " with size " << super.size() << ":\n\n";
    for (int i = 0; i < super.size(); i++){
        std::cout << super[i];
    }
    std::cout << "\n";
    /*
    auto arr = build(std::vector<int>({1, 2, 3}), std::vector<int>({3, 1, 2}));
    for (int i = 0; i < arr.size(); i++){
        std::cout << arr[i];
    }
    */
}
