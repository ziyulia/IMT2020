
#include "constantblackscholesprocess.hpp"
#include "mceuropeanengine.hpp"
#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>
#include <ql/quantlib.hpp>
#include <iostream>
#include <chrono>

// TO COMPILE : g++ main.cpp -o output -lQuantLib
// Then ./output

using namespace QuantLib;

int main() {

    try {

	auto start = std::chrono::steady_clock::now();

        std::cout << std::endl;

        // set up dates
        Calendar calendar = TARGET();
        Date todaysDate(15, May, 1998);
        Date settlementDate(17, May, 1998);
        Settings::instance().evaluationDate() = todaysDate;

        // our options
        Option::Type type(Option::Put);
        Real underlying = 36;
        Real strike = 40;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.06;
        Volatility volatility = 0.20;
        Date maturity(17, May, 1999);
        DayCounter dayCounter = Actual365Fixed();
        Time t = Actual360().yearFraction(settlementDate,maturity);
        
        std::cout << "Option type = "  << type << std::endl;
        std::cout << "Maturity = "        << maturity << std::endl;
        std::cout << "Underlying price = "        << underlying << std::endl;
        std::cout << "Strike = "                  << strike << std::endl;
        std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate)
                  << std::endl;
        std::cout << "Dividend yield = " << io::rate(dividendYield)
                  << std::endl;
        std::cout << "Volatility = " << io::volatility(volatility)
                  << std::endl;
        std::cout << "dayCount = " << t
                << std::endl;
        std::cout << std::endl ;
        
        
        
        std::vector<Date> exerciseDates;
        for (Integer i=1; i<=4; i++)
            exerciseDates.push_back(settlementDate + 3*i*Months);

        boost::shared_ptr<Exercise> europeanExercise(
                                         new EuropeanExercise(maturity));

        Handle<Quote> underlyingH(
            boost::shared_ptr<Quote>(new SimpleQuote(underlying)));

        Handle<YieldTermStructure> flatTermStructure(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(
            boost::shared_ptr<BlackVolTermStructure>(
                new BlackConstantVol(settlementDate, calendar, volatility,
                                     dayCounter)));
        boost::shared_ptr<StrikedTypePayoff> payoff(
                                        new PlainVanillaPayoff(type, strike));
        boost::shared_ptr<GeneralizedBlackScholesProcess> gbsProcess(
                 new GeneralizedBlackScholesProcess(underlyingH, flatDividendTS,
                                               flatTermStructure, flatVolTS));

        // options
        VanillaOption europeanOption(payoff, europeanExercise);

        // Analytic formulas:

        // Monte Carlo Method:
        int timeSteps = 20;
        Size mcSeed = 42;

	// Using the MCEuropeanEngine_2 from mceuropeanengine.hpp
        boost::shared_ptr<PricingEngine> mcengine;
        mcengine = MakeMCEuropeanEngine_2<PseudoRandom>(gbsProcess)
            .withSteps(timeSteps)
            .withAbsoluteTolerance(0.02)
            .withSeed(mcSeed);
        europeanOption.setPricingEngine(mcengine);

        Real errorEstimate = europeanOption.errorEstimate();
	std::cout << "Error estimate = " << errorEstimate << std::endl; // Accuracy
        std::cout << "NPV = " << europeanOption.NPV() << std::endl; // Value
	
	// Total elapsed time (including couts and variables definition)
	auto end = std::chrono::steady_clock::now();
	std::cout << "Total elapsed time in milliseconds : " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << std::endl;

        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
}

