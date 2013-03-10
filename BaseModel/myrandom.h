#ifndef __MY_RANDOM_H_FILE__
#define __MY_RANDOM_H_FILE__

//function to generate random number between 0 and 1
//parameter method determine which type of generator be used
// 1: fastest one
// 2: best one
// 3: take tradeoff
float BASEMODEL_API MyRand(int method=2);

double BASEMODEL_API rand_gaussian (void);	/* Gaussian with mean zero and unit variance */
double BASEMODEL_API rand_exp (void);		/* Exponential with mean one */
double BASEMODEL_API rand_cauchy (void);	/* Cauchy centred at zero with unit width */
double BASEMODEL_API rand_gamma (double);	/* Gamma with given shape parameter */
double BASEMODEL_API rand_beta (double, double); /* Beta with given parameters */



#endif
