#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int factorial(int n){
  int result = 0;
  if(n == 0){
    result = 1;
  }
  else if(n > 0){
    result = n * factorial(n - 1);
  }
  return result;
}

std::string algorithm(const int n){
  std::set<std::vector<int> > P;
  std::vector<int> alpha(n);
  for(int i = 0; i < n; i++){
    alpha[i] = i + 1;
  }
  std::vector<int> S = alpha;
  while(std::next_permutation(alpha.begin(), alpha.end())){
    P.insert(alpha);
  }
  while(!P.empty()){
    for(int i = 1; i <= n; i++){
      std::vector<int> A(S.end() - n + i, S.end());
      std::vector<int> B(S.end() - n, S.end() - n + i);
      bool complete = false;
      for(int j = 0; j < factorial(i); j++){
        std::vector<int> AB = A;
        AB.insert(AB.end(), B.begin(), B.end());
        if(P.count(AB) == 1){
          S.insert(S.end(), B.begin(), B.end());
          P.erase(AB);
          complete = true;
          break;
        }
        std::next_permutation(B.begin(), B.end());
      }
      if(complete){
        break;
      }
    }
  }
  std::string result;
  for(int digit : S){
    result += std::to_string(digit);
  }
  return result;
}

int main(){
  std::string result = algorithm(6);
  std::string optimal =
    "1234561234516234512634512364513264513624513642513645213645123465123415"
    "6234152634152364152346152341652341256341253641253461253416253412653412"
    "3564123546123541623541263541236541326543126453162435162431562431652431"
    "6254316245316425314625314265314256314253614253164523146523145623145263"
    "1452361452316453216453126435126431526431256432156423154623154263154236"
    "1542316542315642135642153624153621453621543621534621354621345621346521"
    "3462513462153642156342165342163542163452163425163421564325164325614325"
    "6413256431265432165432615342613542613452613425613426513426153246513246"
    "5312463512463152463125463215463251463254163254613254631245632145632415"
    "6324516324561324563124653214653241653246153264153261453261543265143625"
    "1436521435621435261435216435214635214365124361524361254361245361243561"
    "2436514235614235164235146235142635142365143265413625413652413562413526"
    "41352461352416352413654213654123 ";

  std::cout << "Length for n = 6 is " << result.size() << std::endl;
  std::cout << std::endl;
  std::cout << "Superpermutation for n = 6:" << std::endl;
  std::cout << result << std::endl << std::endl;
  std::cout << "Computed | Known Optimal:" << std::endl;
  for(int i = 0; i < result.size(); i++){
    std::cout << result[i] << " | " << optimal[i] << ", digit " << i + 1
      << " " << ((result[i] == optimal[i]) ? "same" : "differ") << std::endl;
  }
}
