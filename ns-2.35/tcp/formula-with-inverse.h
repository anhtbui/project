#include "formula.h"

// mulTFRC removed double b_to_p(double b, double rtt, double tzero, int psize, int bval)
double b_to_p(double b, double rtt, double tzero, int psize, int bval, double n_mulTFRC, double p_real) // mulTFRC added
{
	double p, pi, bres;
	int ctr=0;
	p=0.5;pi=0.25;
	while(1) {
// mulTFRC removed                bres=p_to_b(p,rtt,tzero,psize, bval);
		bres=p_to_b(p,rtt,tzero,psize, bval, n_mulTFRC, p_real,0); //mulTFRC added
		/*
		 * if we're within 5% of the correct value from below, this is OK
		 * for this purpose.
		 */
		if ((bres>0.95*b)&&(bres<1.05*b)) 
			return p;
		if (bres>b) {
			p+=pi;
		} else {
			p-=pi;
		}
		pi/=2.0;
		ctr++;
		if (ctr>25) {
			return p;
		}
	}
}
