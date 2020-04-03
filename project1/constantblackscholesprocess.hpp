
#include <ql/stochasticprocess.hpp>

namespace QuantLib {

    class ConstantBlackScholesProcess : public StochasticProcess1D {
    //black scholes process
    public:
        
        Real call_option_black_scholes(const Real& S,
                   const Real& K,
                   const Rate& r,
                   const Volatility& sigma,
                   const Time& t)
        {
            Real time = t;//Actual360().yearFraction(settlement,maturity);
             Real d1 = (log(S/K)+r*time)/(sigma*sqrt(time))+0.5*sigma*sqrt(time);
             Real d2 = d1-sigma*sqrt(time);
             return S*N(d1)-K*exp(-r*time)*N(d2);
        };
        
        Real put_option_black_scholes(const Real& S,
                   const Real& K,
                   const Rate& r,
                   const Volatility& sigma,
                   const Time& t)
        {
            Real time = t;//Actual360().yearFraction(settlement,maturity);
             Real d1 = (log(S/K)+r*time)/(sigma*sqrt(time))+0.5*sigma*sqrt(time);
             Real d2 = d1-sigma*sqrt(time);
             return -S*N(-d1) + K*exp(-r*time)*N(-d2);
        };


        Real N(const Real& z) //cdf of the normal distribution
        {
            if (z >  6.0) { return 1.0; };
            if (z < -6.0) { return 0.0; };
            Real b1 =  0.31938153;
            Real b2 = -0.356563782;
            Real b3 =  1.781477937;
            Real b4 = -1.821255978;
            Real b5 =  1.330274429;
            Real p  =  0.2316419;
            Real c2 =  0.3989423;
            Real a=fabs(z);
            Real t = 1.0/(1.0+a*p);
            Real b = c2*exp((-z)*(z/2.0));
            Real n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;
            n = 1.0-b*n;
            if ( z < 0.0 ) n = 1.0 - n;
            return n;
        };
           
    private:
        Real K;
        Real S ;
        Rate r;
        Volatility sigma ;
        Time t;
    };
    
}


