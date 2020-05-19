#include <dsplib.h>

#define NUM_SAMPLES 5000 //liczba probek

int samples[NUM_SAMPLES]; //tworzenie tablicy z probkami

void saw(int* samples_table, unsigned int lprobek, const int krok);
void rect(int* samples_table, unsigned int lprobek, const int krok, const int wartoscProgowa);
void tri(int* samples_table, unsigned int lprobek, const int krok);
void sint(int* samples_table, unsigned int lprobek, const int krok);

void main(void) {
  rand16init(); //inicjowanie funkcji rand

  //obliczenie kroku i przypisanie go do stalej
  const int krok = 137;
  int* samples_table = &samples[0];

  //Funkcje realizujace dane zadania

  //saw(samples_table, NUM_SAMPLES, krok);          //sygnal piloksztaltny
  //rect(samples_table, NUM_SAMPLES, krok, 16384);  //sygnal prostokatny
  //tri(samples_table, NUM_SAMPLES, krok);          //sygnal trojkatny
  //sint(samples_table, NUM_SAMPLES, krok);         //sygnal sinusoidalny za pomoca szeregu Taylora

  //sine((DATA*)samples_table, (DATA*)samples_table, NUM_SAMPLES);  //sygnal sinusoidalny przy wykorzystaniu DSPLIB -- wymagane odkomentowanie funkcji saw!!
  rand16((DATA*)samples_table, 5000);	//bialy szum przy wykorzystaniu DSPLIB
  while (1); // do not exit
}

void saw(int* samples_table, unsigned int lprobek, const int krok){
  static int aku = 0;  //akumulator
  int i = 0;
  while (i < lprobek){
    samples_table[i] = aku;
    aku += krok;
    i++;
  }

}

void rect(int* samples_table, unsigned int lprobek, const int krok, const int wartoscProgowa){
  saw(samples_table, lprobek, krok); //wywolanie funkcji saw() do uzyskania probek sygnalu pilokszaltnego
  int i = 0;
  while (i < lprobek){
	 if (samples_table[i] > wartoscProgowa){
		samples_table[i] = 32767; //gorna wartosc sygnalu
	 }
	 else{
		 samples_table[i] = -32768; //dolna wartosc sygnalu
	 }
	 i++;
  }

}


void tri(int* samples_table, unsigned int lprobek, const int krok){
  saw(samples_table, lprobek, krok); //wywolanie funkcji saw() do uzyskania probek sygnalu pilokszaltnego
  int i = 0;
  while (i < lprobek){
	 samples_table[i] = (samples_table[i] < 0 ? -samples_table[i] : samples_table[i]); //obliczanie wartosci bezwzglednej
	 samples_table[i] = 2* samples_table[i] - 32767; //obliczanie sygnalu trojkatnego z sygnalu piloksztaltnego
	 i++;
  }

}

void sint(int* samples_table, unsigned int lprobek, const int krok){
	const long int a1 = 12868;             //pierwszy wspolczynnik szeregu
	const long int a3 = -21167;            //trzeci wspolczynnik szeregu
	const long int a5 = 10445;	           //piaty wspolczynnik szeregu
	const long int a7 = -2455;	           //siodmy wspolczynnik szeregu
	saw(samples_table, NUM_SAMPLES, krok); //wywolanie funkcji generujacej sygnal piloksztaltny

	int i = 0;
	while (i < lprobek){
		int x = samples_table[i]; //tworzenie pomocniczej zmiennej x z dana probka

		x = (x < 0 ? -x : x); //obliczanie wartosci bezwzglednej
		if (x > 16384){
			x = 32767 - x;	//wartosc bezwzgledna kata fazowego
		}

		int x2 = _smpy(x, x);	//obliczenie drugiej potegi
		int x3 = _smpy(x2, x);	//obliczenie trzeciej potegi
		int x5 = _smpy(x3, x2);	//obliczenie piatej potegi
		int x7 = _smpy(x5, x2);	//obliczenie siodmej potegi

		if (samples_table[i] <0){
			samples_table[i] = -(int)(((a1*x + a3*x3 + a5*x5 + a7*x7) + (1<<11)) >> 12);  //dla ujemnej wartosci fazy oraz przejscie do zapisu Q15
		}
		else{
			samples_table[i] = (int)(((a1*x + a3*x3 + a5*x5 + a7*x7) + (1<<11)) >> 12);  //dla dodatniej wartosci fazy oraz przejscie do zapisu Q15
		}

		i++;
	}
}
