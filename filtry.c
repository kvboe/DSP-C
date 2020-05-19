//Jakub Polejowski 171697
//Telekomunikacja 1, semestr 6

#include <dsplib.h>

#define NUM_SAMPLES 5000  //liczba probek do przefiltrowania
#define NUM_FILTER 57  //liczba probek filtru

int samples_in[NUM_SAMPLES];  //tablica z probkami przed filtracja
int samples_out[NUM_SAMPLES];  //tablica z probkami po filtracji
int dbuffer[NUM_FILTER + 2] = {0};  //buffor do funkcji fir() pochodzacej z DSPLIB

//const int FIR[] =   {24, 20, 15, 7, -5, -23, -49, -81, -119, -159, -196, -225, -237, -225,
//					-181, -99, 26, 196, 410, 663, 947, 1250, 1559, 1857, 2129, 2359, 2535, 2644,
//					2681, 2644, 2535, 2359, 2129, 1857, 1559, 1250, 947, 663, 410, 196, 26, -99,    //wartosci probek filtru przed mnozeniem
//					-181, -225, -237, -225, -196, -159, -119, -81, -49, -23, -5, 7, 15, 20, 24};

const int FIR[] = {23, 19, 14, 7, -5, -22, -46, -77, -113, -151, -186, -213, -225, -214,
		 -172, -94, 25, 187, 390, 630, 900, 1188, 1481, 1764, 2022, 2241, 2408, 2512,       //wartosci *0.95
		 2547, 2512, 2408, 2241, 2022, 1764, 1481, 1188, 900, 630, 390, 187, 25, -94,
		 -172, -214, -225, -213, -186, -151, -113, -77, -46, -22, -5, 7, 14, 19, 23};

void blockfir(int* input, const int* filter, int* output, int numSamples, int numFilter);
void saw(int* samples_table, unsigned int lprobek, const int krok);

void main(void) {

	int* samplesIn = &samples_in[0]; //wskaznik do tablicy z probkami wejsciowymi
	int* samplesOut = &samples_out[0]; //wskaznik do tablicy z probkami wyjsciowymi
	const int* FIRsamp = &FIR[0]; //wskaznik do tablicy z wspolczynnikami filtru
	const int krok = 137; //obliczony wczesniej krok do generowania sygnalu piloksztaltnego

	rand16((DATA*)samplesIn, NUM_SAMPLES);                                      //generowanie bialego szumu
	//saw(samplesIn, NUM_SAMPLES, krok);                                        //sygnal piloksztaltny
	blockfir(samplesIn, FIRsamp, samplesOut, NUM_SAMPLES, NUM_FILTER);          //funkcja filtrujaca FIR

	//fir ((DATA*)samplesIn, (DATA*)FIRsamp, (DATA*)samplesOut, (DATA*)dbuffer, NUM_SAMPLES, NUM_FILTER);    //filtracja blokowa za pomoca DSPLIB

	//int k = 0;
	//for (k=0; k<NUM_SAMPLES; k++){
	//	fir ((DATA*)&samplesIn[k], (DATA*)FIRsamp, (DATA*)&samplesOut[k], (DATA*)dbuffer, 1, NUM_FILTER);    //filtracja probka po probce za pomoca DSPLIB
	//}

	while (1); // do not exit
}

void blockfir(int* input, const int* filter, int* output, int numSamples, int numFilter){
	int i, j = 0;
	for (i = 0; i<numSamples; i++){
		long temp = 0;
		for (j = 0; j<numFilter; j++){
			if (i-j >=0)
				temp = _smaci (temp, input[i-j], filter[j]);  //instrukcja wykonujaca obliczenia y = y + a * x
			else
				break;
		}
		output[i] = (int)((temp + (1<<14)) >> 15);  //zaokroaglanie do Q15
	}

}

void saw(int* samples_table, unsigned int lprobek, const int krok){  //generowanie sygnalu piloksztaltnego z poprzedniego zadania
  static int aku = 0;  //akumulator
  int i = 0;
  while (i < lprobek){
    samples_table[i] = aku;
    aku += krok;
    i++;
  }

}
