#include "NullPhotometricCorrection.h"


MaRC::NullPhotometricCorrection::~NullPhotometricCorrection (void)
{
}

int
MaRC::NullPhotometricCorrection::correct (
  const BodyData & /* body */,
  double /* sub_observ_lat */,
  double /* sub_observ_lon */,
  double /* sub_solar_lat */,
  double /* sub_solar_lon */,
  double /* lat */,
  double /* lon */,
  double /* range */,
  double & /* data */)
{
  return 0;
}

MaRC::PhotometricCorrection *
MaRC::NullPhotometricCorrection::clone (void) const
{
  // Stateless so no need to copy construct.
  return new NullPhotometricCorrection;
}
