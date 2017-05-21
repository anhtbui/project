#define MAXRATE 25000000.0
#define SAMLLFLOAT 0.0000001


/*
 * This takes as input the packet drop rate, and outputs the sending
 *   rate in bytes per second.
 */
static double p_to_b(double p, double rtt, double tzero, int psize, int bval, double n_mulTFRC, double p_real, int refinement) 
// mulTFRC edit
{
        double tmp1, tmp2, res;

        if (p < 0 || rtt < 0) {
                return MAXRATE ;
        }
if (n_mulTFRC==1.0){            // mulTFRC



        res=rtt*sqrt(2*bval*p/3);
        tmp1=3*sqrt(3*bval*p/8);
        if (tmp1>1.0) tmp1=1.0;
        tmp2=tzero*p*(1+32*p*p);
        res+=tmp1*tmp2;
//      if (formula_ == 1 && p > 0.25) {
//              // Get closer to RFC 3714, Table 1.
//              // This is for TCP-friendliness with a TCP flow without ECN
//              //   and without timestamps.
//              // Just for experimental purposes.
//              if p > 0.70) {
//                      res=res*18.0;
//              } else if p > 0.60) {
//                      res=res*7.0;
//              } else if p > 0.50) {
//                      res=res*5.0;
//              } else if p > 0.45) {
//                      res=res*4.0;
//              } else if p > 0.40) {
//                      res=res*3.0;
//              } else if p > 0.25) {
//                      res=res*2.0;
//              }
//      }
        // At this point, 1/res gives the sending rate in pps:
        // 1/(rtt*sqrt(2*bval*p/3) + 3*sqrt(3*bval*p/8)*tzero*p*(1+32*p*p))
        if (res < SAMLLFLOAT) {
                res=MAXRATE;
        } else {
                // change from 1/pps to Bps.
                res=psize/res;
        }
// mulTFC begin
	}
	else {
		if (p<0.00000001){ return 1.0*n_mulTFRC*MAXRATE; }

//		else 
			{
			double bt;      // bandwidth
			double x;       // the number of rounds in oa TD-period
			double w;       // the cumulative window
			double tdp=1/p; // packets sent during a TD-period
			double jrL=p_real;//p;    // the average number of packets lost during a loss event
			if(jrL<1.0)jrL=1.0;
if(refinement==1){
if(n_mulTFRC<13.0){
jrL=n_mulTFRC*(1-pow(1.0-1.0/n_mulTFRC,jrL)); 
if (jrL<=1.0)jrL=1.0;
}
}
			//int limit=(((int)n_mulTFRC)!=n_mulTFRC)?((int)n_mulTFRC+1):n_mulTFRC;
			if(n_mulTFRC>1.0)
			{jrL=jrL>ceil(n_mulTFRC)?ceil(n_mulTFRC):jrL;}
			else jrL=1.0;

			double aa1=6.0*n_mulTFRC*n_mulTFRC*p;
			double bb1=-n_mulTFRC*p*bval*jrL+2.0*jrL*jrL*p*bval;
 
			double cc1=n_mulTFRC*n_mulTFRC*p*p*bval*bval*jrL*jrL-4.0*n_mulTFRC*p*p*bval*bval*jrL*jrL*jrL+4.0*jrL*jrL*jrL*jrL*p*p*bval*bval+24.0*n_mulTFRC*n_mulTFRC*p*bval*jrL;

			x=(bb1+sqrt(cc1))/aa1;

			bt=tdp/(x*rtt);

			w=(n_mulTFRC*x)/(2.0*bval)+(3.0*n_mulTFRC*n_mulTFRC*x)/(2.0*bval*jrL);


			double top=1/(1-p);
			double zto=(1.0+p+2.0*pow(p,2)+4.0*pow(p,3)+8.0*pow(p,4)+16.0*pow(p,5)+32.0*pow(p,6))/(1-p);


			//double nTO=jrL/(2.0*n_mulTFRC*x/(bval*jrL));
			double nTO=p_real/w*n_mulTFRC;
			nTO=((nTO*tzero*zto/(x*rtt))>n_mulTFRC)?n_mulTFRC:(nTO*tzero*zto/(x*rtt));
			bt=(tdp-tdp/n_mulTFRC*(nTO))/(x*rtt)+nTO*top/(tzero*zto);

			res=bt*psize;
		}
	}
// mulTFRC end
        if (res > n_mulTFRC*MAXRATE) { // mulTFRC edit
                res = n_mulTFRC*MAXRATE ;  // mulTFRC edit
        }
        return res;
}


