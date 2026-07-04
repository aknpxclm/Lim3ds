#ifndef ASD_H_
#define ASD_H_

void ClashingAtk(int* SinSanity, int* EnSanity, int* SinCoin, int* EnCoin, int SinBase, int EnBase, int SinPow, int EnPow, double* SinHealth, double* EnHealth);
void UnopposedAtk(int Coin, int Base, int Pow, double* OpposingHealth);
int ComparePriority(int Pri1,int Pri2);

#endif
