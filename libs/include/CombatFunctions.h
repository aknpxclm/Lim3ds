#ifndef COMBATFUNCTIONS_H_
#define COMBATFUNCTIONS_H_

void ClashingAtk(int* SinSanity, int* EnSanity, int* SinCoin, int* EnCoin, int SinBase, int EnBase, int SinPow, int EnPow, double* SinHealth, double* EnHealth);
void UnopposedAtk(int Coin, int Base, int Pow, double* OpposingHealth);
int ComparePriority(int Pri1,int Pri2);

#define CurrentIndex 1

#endif
