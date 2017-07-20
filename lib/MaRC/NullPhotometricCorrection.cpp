#include "NullPhotometricCorrection.h"


MaRC::NullPhotometricCorrection::~NullPhotometricCorrection (void)
{
}

int
MaRC::NullPhotometricCorrection::correct (
  const BodyData & /* body */,
  const double & /* sub_observ_lat */,
  const double & /* sub_observ_lon */,
  const double & /* sub_solar_lat */,
  const double & /* sub_solar_lon */,
  const double & /* lat */,
  const double & /* lon */,
  const double & /* range */,
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
