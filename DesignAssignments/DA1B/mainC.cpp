//Serak Gebremedhin
//CPE 301-1001 
//DA1B implementation through C++
//This program will do exactly what the assembly code did, and it will output the results that show the sum of numbers
//divisible by 3 and those not divisible by 3. The 99 numbers that are in the array start at  11(i.e. element 1 holds number 11), 
// and then it adds consecutively until the 99th element is reached.
#include <iostream>

using namespace std;

int main(){

  int a[99];                                   //certain array in memory location with 99 elements
  int x = 11;                                  //data in elements will increment from 11
  for (int i = 0; i < 99; i++){
    a[i] = x;
    x++;
  }

  int notDivBy3Sum = 0;
  int divBy3Sum = 0;

  //for loop checks if the numbers in array are divisible by 3 or not
  for (int i = 0; i < 99; i++){
    if (a[i] % 3 != 0)
      notDivBy3Sum = notDivBy3Sum + a[i];     //adds numbers not divisible by 3
    else
      divBy3Sum = divBy3Sum + a[i];            //adds numbers divisible by 3
  }


  cout << endl << "Sum of numbers not divisible by 3: " << notDivBy3Sum << endl;

  cout << endl << "Sum of number divisible by 3: " << divBy3Sum << endl;

  cout << endl;
  return 0;
}
