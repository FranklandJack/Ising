#include "IsingInputParameters.hpp"

std::ostream& operator<<(std::ostream& out, const IsingInputParameters& params)
{
	int outputColumnWidth = 30;
    out << "Input-Parameters..." << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Rows: " << std::right << params.rowCount << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Columns: " << std::right << params.columnCount << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Dynamics: " << std::right << ((params.dynamics==IsingInputParameters::Kawasaki) ? "Kawasaki" : "Glauber") << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Temperature: " << std::right << params.temperature << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "J: " << std::right << params.jConstant << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "K_B: " << std::right << params.boltzmannConstant << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Burn-Period: " << std::right << params.burnPeriod<< '\n';
	out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Sweeps: " << std::right << params.sweeps<< '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Measurement-Interval: " << std::right << params.measurementInterval << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Autocorrelation-Range: " << std::right << params.autoCorrelationRange << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Error-Method-Used: " << std::right << ((params.errorType==IsingInputParameters::Bootstrap) ? "Bootstrap" : "Jack-Knife") << '\n';
    return out;
}