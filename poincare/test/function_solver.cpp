#include <apps/shared/global_context.h>
#include <poincare/expression.h>
#include "helper.h"

using namespace Poincare;

enum class ExtremumType : uint8_t {
  Maximum,
  Minimum,
  Root
};

bool doubles_are_approximately_equal(double d1, double d2) {
  bool d2IsNaN = std::isnan(d2);
  if (std::isnan(d1)) {
    return d2IsNaN;
  }
  if (d2IsNaN) {
    return false;
  }
  return std::abs(d1-d2) < 0.00001;
}

void assert_next_extrema_are(
    ExtremumType extremumType,
    int numberOfExtrema,
    Coordinate2D * extrema,
    Expression e,
    const char * symbol,
    Context * context,
    double start = -1.0,
    double step = 0.1,
    double max = 100.0,
    Preferences::ComplexFormat complexFormat = Preferences::ComplexFormat::Real,
    Preferences::AngleUnit angleUnit = Preferences::AngleUnit::Degree)
{
  double currentStart = start;
  for (int i = 0; i < numberOfExtrema; i++) {
    quiz_assert_log_if_failure(!std::isnan(currentStart), e);
    Coordinate2D nextExtrema;
    if (extremumType == ExtremumType::Maximum) {
      nextExtrema = e.nextMaximum(symbol, currentStart, step, max, context, complexFormat, angleUnit);
    } else if (extremumType == ExtremumType::Minimum) {
      nextExtrema = e.nextMinimum(symbol, currentStart, step, max, context, complexFormat, angleUnit);
    } else if (extremumType == ExtremumType::Root) {
      nextExtrema = Coordinate2D(e.nextRoot(symbol, currentStart, step, max, context, complexFormat, angleUnit), 0.0 );
    }
    currentStart = nextExtrema.abscissa() + step;
    quiz_assert_log_if_failure(
        (doubles_are_approximately_equal(extrema[i].abscissa(), nextExtrema.abscissa()))
        && (doubles_are_approximately_equal(extrema[i].value(), nextExtrema.value())),
        e);
  }
}

QUIZ_CASE(poincare_function_extremum) {
  const char * symbol = "a";
  int symbolLength = strlen(symbol);
  Shared::GlobalContext globalContext;
  {
    // cos
    Expression e = Cosine::Builder(Symbol::Builder(symbol, symbolLength));
    {
      constexpr int numberOfMaxima = 3;
      Coordinate2D maxima[numberOfMaxima] = {
        Coordinate2D(0.0, 1.0),
        Coordinate2D(360.0, 1.0),
        Coordinate2D(NAN, NAN)};
      assert_next_extrema_are(ExtremumType::Maximum, numberOfMaxima, maxima, e, symbol, &globalContext, -1.0, 0.1, 500.0);
    }
    {
      constexpr int numberOfMinima = 1;
      Coordinate2D minima[numberOfMinima] = {
        Coordinate2D(180.0, -1.0)};
      assert_next_extrema_are(ExtremumType::Minimum, numberOfMinima, minima, e, symbol, &globalContext, 0.0, 0.1, 300.0);
    }
  }
  {
    // x^2
    Expression e = Power::Builder(Symbol::Builder(symbol, symbolLength), Rational::Builder(2));
    {
      constexpr int numberOfMaxima = 1;
      Coordinate2D maxima[numberOfMaxima] = {
        Coordinate2D(NAN, NAN)};
      assert_next_extrema_are(ExtremumType::Maximum, numberOfMaxima, maxima, e, symbol, &globalContext);
    }
    {
      constexpr int numberOfMinima = 1;
      Coordinate2D minima[numberOfMinima] = {
        Coordinate2D(0.0, 0.0)};
      assert_next_extrema_are(ExtremumType::Minimum, numberOfMinima, minima, e, symbol, &globalContext);
    }
  }

  {
    // 3
    Expression e = Rational::Builder(3);
    {
      constexpr int numberOfMaxima = 1;
      Coordinate2D maxima[numberOfMaxima] = {
        Coordinate2D(NAN, 3.0)};
      assert_next_extrema_are(ExtremumType::Maximum, numberOfMaxima, maxima, e, symbol, &globalContext);
    }
    {
      constexpr int numberOfMinima = 1;
      Coordinate2D minima[numberOfMinima] = {
        Coordinate2D(NAN, 3.0)};
      assert_next_extrema_are(ExtremumType::Minimum, numberOfMinima, minima, e, symbol, &globalContext);
    }
  }

  {
    // 0
    Expression e = Rational::Builder(0);
    {
      constexpr int numberOfMaxima = 1;
      Coordinate2D maxima[numberOfMaxima] = {
        Coordinate2D(NAN, 0.0)};
      assert_next_extrema_are(ExtremumType::Maximum, numberOfMaxima, maxima, e, symbol, &globalContext);
    }
    {
      constexpr int numberOfMinima = 1;
      Coordinate2D minima[numberOfMinima] = {
        Coordinate2D(NAN, 0.0)};
      assert_next_extrema_are(ExtremumType::Minimum, numberOfMinima, minima, e, symbol, &globalContext);
    }
  }
}

QUIZ_CASE(poincare_function_root) {
  const char * symbol = "a";
  int symbolLength = strlen(symbol);
  Shared::GlobalContext globalContext;
  {
    // cos
    Expression e = Cosine::Builder(Symbol::Builder(symbol, symbolLength));
    constexpr int numberOfRoots = 3;
    Coordinate2D roots[numberOfRoots] = {
      Coordinate2D(90.0, 0.0),
      Coordinate2D(270.0, 0.0),
      Coordinate2D(450.0, 0.0)};
    assert_next_extrema_are(ExtremumType::Root, numberOfRoots, roots, e, symbol, &globalContext, 0.0, 0.1, 500.0);
  }
  {
    // x^2
    Expression e = Power::Builder(Symbol::Builder(symbol, symbolLength), Rational::Builder(2));
    constexpr int numberOfRoots = 1;
    Coordinate2D roots[numberOfRoots] = {
      Coordinate2D(0.0, 0.0)};
    assert_next_extrema_are(ExtremumType::Root, numberOfRoots, roots, e, symbol, &globalContext);
  }
  {
    // x^2-4
    Expression e = Subtraction::Builder(Power::Builder(Symbol::Builder(symbol, symbolLength), Rational::Builder(2)), Rational::Builder(4));
    constexpr int numberOfRoots = 2;
    Coordinate2D roots[numberOfRoots] = {
      Coordinate2D(-2.0, 0.0),
      Coordinate2D(2.0, 0.0)};
    assert_next_extrema_are(ExtremumType::Root, numberOfRoots, roots, e, symbol, &globalContext, -5.0);
  }
  {
    // 3
    Expression e = Rational::Builder(3);
    constexpr int numberOfRoots = 1;
    Coordinate2D roots[numberOfRoots] = {
      Coordinate2D(NAN, 0.0)};
    assert_next_extrema_are(ExtremumType::Root, numberOfRoots, roots, e, symbol, &globalContext);
  }

  {
    // 0
    Expression e = Rational::Builder(0);
    constexpr int numberOfRoots = 1;
    Coordinate2D roots[numberOfRoots] = {
      Coordinate2D(-0.9, 0.0)};
    assert_next_extrema_are(ExtremumType::Root, numberOfRoots, roots, e, symbol, &globalContext);
  }

}

void assert_next_intersections_are(
    Expression otherExpression,
    int numberOfIntersections,
    Coordinate2D * intersections,
    Expression e,
    const char * symbol,
    Context * context,
    double start = -1.0,
    double step = 0.1,
    double max = 500.0,
    Preferences::ComplexFormat complexFormat = Preferences::ComplexFormat::Real,
    Preferences::AngleUnit angleUnit = Preferences::AngleUnit::Degree)
{
  double currentStart = start;
  for (int i = 0; i < numberOfIntersections; i++) {
    quiz_assert_log_if_failure(!std::isnan(currentStart), e);
    Coordinate2D nextIntersection = e.nextIntersection(symbol, currentStart, step, max, context, complexFormat, angleUnit, otherExpression);
    currentStart = nextIntersection.abscissa() + step;
    quiz_assert_log_if_failure(
        (doubles_are_approximately_equal(intersections[i].abscissa(), nextIntersection.abscissa()))
        && (doubles_are_approximately_equal(intersections[i].value(), nextIntersection.value())),
        e);
  }
}
QUIZ_CASE(poincare_function_intersection) {
  const char * symbol = "a";
  int symbolLength = strlen(symbol);
  Shared::GlobalContext globalContext;
  Expression e = Cosine::Builder(Symbol::Builder(symbol, symbolLength));

  {
    // cos with y=2
    Expression otherExpression = Rational::Builder(2);
    constexpr int numberOfIntersections = 1;
    Coordinate2D intersections[numberOfIntersections] = {
      Coordinate2D(NAN, NAN)};
    assert_next_intersections_are(otherExpression, numberOfIntersections, intersections, e, symbol, &globalContext);
  }

  {
    // cos with y=1
    Expression otherExpression = Rational::Builder(1);
    constexpr int numberOfIntersections = 2;
    Coordinate2D intersections[numberOfIntersections] = {
      Coordinate2D(0.0, 1.0),
      Coordinate2D(360.0, 1.0)};
    assert_next_intersections_are(otherExpression, numberOfIntersections, intersections, e, symbol, &globalContext);
  }

  {
    // cos with y=0
    Expression otherExpression = Rational::Builder(0);
    constexpr int numberOfIntersections = 3;
    Coordinate2D intersections[numberOfIntersections] = {
      Coordinate2D(90.0, 0.0),
      Coordinate2D(270.0, 0.0),
      Coordinate2D(450.0, 0.0)};
    assert_next_intersections_are(otherExpression, numberOfIntersections, intersections, e, symbol, &globalContext);
  }
}
