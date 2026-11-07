#include <cstdint>

#include "Catch2/single_include/catch.hpp"
#include "potential/electrostatics/helpme.h"

#include <complex>
#include <memory>
#include <type_traits>
#include <utility>

namespace {

using Wrapper = helpme::FFTWWrapper<double>;

static_assert(!std::is_copy_constructible<Wrapper>::value, "FFTW plans must not be copied");
static_assert(!std::is_copy_assignable<Wrapper>::value, "FFTW plans must not be copied");

// Exercise all six plans after ownership changes. FFTW's inverse transforms
// return the input multiplied by the transform dimension.
void checkTransforms(Wrapper& wrapper, int dimension) {
    helpme::vector<double> realInput(dimension), realOutput(dimension);
    helpme::vector<std::complex<double>> spectrum(dimension / 2 + 1);
    helpme::vector<std::complex<double>> complexInput(dimension), complexOutput(dimension), intermediate(dimension);
    for (int i = 0; i < dimension; ++i) {
        realInput[i] = i + 1;
        complexInput[i] = std::complex<double>(i + 1, 1 - i);
    }

    wrapper.transform(realInput.data(), spectrum.data());
    wrapper.transform(spectrum.data(), realOutput.data());
    for (int i = 0; i < dimension; ++i) {
        REQUIRE(realOutput[i] / dimension == Approx(realInput[i]).margin(1e-10));
    }

    wrapper.transform(complexInput.data(), intermediate.data(), FFTW_FORWARD);
    wrapper.transform(intermediate.data(), complexOutput.data(), FFTW_BACKWARD);
    for (int i = 0; i < dimension; ++i) {
        REQUIRE(complexOutput[i].real() / dimension == Approx(complexInput[i].real()).margin(1e-10));
        REQUIRE(complexOutput[i].imag() / dimension == Approx(complexInput[i].imag()).margin(1e-10));
    }

    intermediate = complexInput;
    wrapper.transform(intermediate.data(), FFTW_FORWARD);
    wrapper.transform(intermediate.data(), FFTW_BACKWARD);
    for (int i = 0; i < dimension; ++i) {
        REQUIRE(intermediate[i].real() / dimension == Approx(complexInput[i].real()).margin(1e-10));
        REQUIRE(intermediate[i].imag() / dimension == Approx(complexInput[i].imag()).margin(1e-10));
    }
}

template <typename Real>
void checkDisabledPrecision() {
    // Empty wrappers must also compile and allow repeated cleanup when this
    // precision has no linked FFTW implementation.
    helpme::FFTWWrapper<Real> empty;
    empty.destroy();
    empty.destroy();
    REQUIRE_THROWS_WITH(helpme::FFTWWrapper<Real>(16),
                        Catch::Matchers::Contains("precision mode that has not been linked"));
}

}  // namespace

TEST_CASE("FFTW wrappers reject disabled precision", "[fftw][precision]") {
#if HAVE_FFTWF != 1
    SECTION("single precision") { checkDisabledPrecision<float>(); }
#endif
#if HAVE_FFTWL != 1
    SECTION("long double precision") { checkDisabledPrecision<long double>(); }
#endif
#if HAVE_FFTWF == 1 && HAVE_FFTWL == 1
    SUCCEED("All supported precisions are enabled");
#endif
}

TEST_CASE("FFTW move assignment survives source destruction", "[fftw][lifetime]") {
    Wrapper destination(32);
    {
        Wrapper source(16);
        destination = std::move(source);
    }
    checkTransforms(destination, 16);
}

TEST_CASE("FFTW move construction survives source destruction", "[fftw][lifetime]") {
    std::unique_ptr<Wrapper> destination;
    {
        Wrapper source(16);
        destination.reset(new Wrapper(std::move(source)));
    }
    checkTransforms(*destination, 16);
}

TEST_CASE("FFTW wrappers allow self-move and repeated cleanup", "[fftw][lifetime]") {
    Wrapper empty;
    empty.destroy();
    empty.destroy();

    Wrapper wrapper(16);
    Wrapper& alias = wrapper;
    wrapper = std::move(alias);
    checkTransforms(wrapper, 16);

    wrapper.destroy();
    wrapper.destroy();
    wrapper = Wrapper(32);
    checkTransforms(wrapper, 32);

    wrapper = Wrapper();
}

TEST_CASE("PME destruction preserves independent FFTW plans", "[fftw][lifetime]") {
    Wrapper survivor(16);
    {
        helpme::PMEInstance<double> unrelated;
        unrelated.setup(1, 0.3, 6, 16, 16, 16, 1, 1);
        unrelated.setup(1, 0.3, 6, 24, 24, 24, 1, 1);
    }
    checkTransforms(survivor, 16);
}
