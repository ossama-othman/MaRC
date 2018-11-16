// -*- C++ -*-
/**
 *  @file Constants.h
 *
 *  Mathematical and physical constants and conversion factors
 *
 *  Copyright (C) 1993, 1994, 1995
 *  Associated Universities, Inc. Washington DC, USA.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
 *
 * Correspondence concerning AIPS++ should be addressed as follows:
 *        Internet email: aips2-request@nrao.edu.
 *        Postal address: AIPS++ Project Office
 *                        National Radio Astronomy Observatory
 *                        520 Edgemont Road
 *                        Charlottesville, VA 22903-2475 USA
 *
 *
 * Conversion from C++ structure to namespace by Ossama Othman
 * in November 2003.
 *
 * Use of C++11 constexpr keyword introduced by Ossama Othman
 * in July 2017.
 *
 *  @author Associated Universities, Inc. Washington DC, USA
 *  @author Ossama Othman
 */

#ifndef MARC_AIPS_CONSTANTS_H
#define MARC_AIPS_CONSTANTS_H

/**
 * @namespace C
 *
 * @brief Various mathematical and physical constants.
 *
 * The conversion factors convert to an internal representation.  For
 * example, @c C::inch when used as a factor converts from inchs to
 * internal units (in this case metres).  When used as a divisor it
 * converts from internal units to inchs.
 *
 * @verbatim
 * Compound usage accords with conventional dimensional analysis, e.g.:
 *
 *    * Factor to convert seconds to days:
 *         C::second / C::day
 *
 *    * Factor to convert kilometers to feet:
 *         (C::kilo * C::metre) / C::foot
 *
 *    * Factor to convert acres to hectares:
 *         C::acre / (C::hecto * C::are)
 *
 *    * Factor to convert millibars to kilopascals:
 *         (C::milli * C::bar) / (C::kilo * C::Pascal)
 *
 * The units used internally are as follows:
 *
 *     angle                        radian
 *     solid angle                  steradian
 *     time                  [T]    second
 *     length                [L]    metre
 *     mass                  [M]    kilogram
 *     charge                [Q]    Coulomb
 *     luminous intensity    [Iv]   candela
 *     amount of substance   [N]    mole
 *
 * @endverbatim
 *
 * For derived quantities such as volume, velocity, force, and power,
 * the units are specified in terms of those of the fundamental
 * quantities.
 *
 * @todo Some of these constants are outdated, and should be updated
 *       with the latest values, such as those found on the NIST Web
 *       site (https://physics.nist.gov/cuu/Constants/index.html).
 *       We may also want to remove those that aren't used by MaRC,
 *       which is the majority of them.
 */
namespace C
{
  // Begin Mathematical constants

  /**
   * @name Irrationals
   */
  //@{

  /// sqrt(2)
  static constexpr double sqrt2 = 1.4142135623730950488;

  /// sqrt(3)
  static constexpr double sqrt3 = 1.7320508075688772935;

  /// 1/sqrt(2)
  static constexpr double _1_sqrt2 = 0.70710678118654752440;

  /// 1/sqrt(3)
  static constexpr double _1_sqrt3 = 0.57735026918962576451;

  //@}

  /**
   *@name Pi and functions thereof
   */
  //@{
  /// pi
  static constexpr double pi = 3.141592653589793238462643;

  /// 2*pi
  static constexpr double _2pi = 6.283185307179586476925286;

  /// pi/2
  static constexpr double pi_2 = 1.570796326794896619231322;

  /// pi/4
  static constexpr double pi_4 = 0.7853981633974483096156608;

  /// 1/pi
  static constexpr double _1_pi = 0.3183098861837906715377675;

  /// 2/pi
  static constexpr double _2_pi = 0.6366197723675813430755350;

  /// 1/sqrt(pi)
  static constexpr double _1_sqrtpi = 0.5641895835477562869480795;

  /// 2/sqrt(pi)
  static constexpr double _2_sqrtpi = 1.1283791670955125738961590;
  //@}

  /**
   * @name e and functions thereof
   */
  //@{
  /// e
  static constexpr double e = 2.718281828459045235360287;

  /// ln(2)
  static constexpr double ln2 = 0.6931471805599453094172321;

  /// ln(10)
  static constexpr double ln10 = 2.3025850929940456840179915;

  /// log2(e)
  static constexpr double log2e = 1.4426950408889634074;

  /// log10(e)
  static constexpr double log10e = 0.4342944819032518276511289;
  //@}

  /**
   * @name gamma and functions thereof
   */
  //@{
  /// gamma
  static constexpr double gamma = 0.577215664901532860606512;

  /// ln(gamma)
  static constexpr double lngamma = -0.549539312981644822337662;

  /// e**gamma
  static constexpr double etogamma = 1.7810724179901979852;
  //@}

  // End Mathematical constants


  /*
   *  Begin Physical constants, and quantities
   *
   * References:
   *     "Explanatory Supplement to the Astronomical Almanac",
   *
   *     "Handbook of Chemistry and Physics", 55th ed.
   *      Robert C. Weast (Ed), 1974.
   *      CRC Press Inc.
   *      ISBN 087819-54-1
   */

  /**
   * @name Fundamental physical constants (SI units)
   */
  //@{
  /// velocity of light
  static constexpr double c = 299792458;

  /// Gravitational constant
  static constexpr double Gravity = 6.67259e-11;

  /// Planck's constant
  static constexpr double Planck = 6.62607015e-34;

  /// gas constant
  static constexpr double GasConst = 8.3144598;

  /// Avogardo's constant
  static constexpr double Avogadro = 6.02214076e+23;

  /// electron charge
  static constexpr double qe = 1.602176634e-19;

  /// proton mass
  static constexpr double mp = 1.6726231e-27;

  /// proton mass / electron mass
  static constexpr double mp_me = 1836.152701;
  //@}

  /**
   * @name Derived physical constants (SI units)
   */
  //@{
  /// magnetic permeability of vacuum
  static constexpr double mu0 = 4.0e-7 * C::pi;

  /// electric permittivity of vacuum
  static constexpr double epsilon0 = 1.0 / ( C::mu0 * C::c * C::c );

  /// Planck's constant divided by 2*pi
  static constexpr double Planck_2pi = C::Planck / C::_2pi;

  /// atomic mass unit
  static constexpr double u = 1.0e-3 / C::Avogadro;

  /// Boltzmann's constant
  static constexpr double Boltzmann = C::GasConst / C::Avogadro;

  /// Faraday's constant
  static constexpr double Faraday = C::Avogadro * C::qe;

  /// electron mass
  static constexpr double me = C::mp / C::mp_me;

  /// classical electron radius
  static constexpr double re = 2.8179e-15;

  /// Bohr radius
  static constexpr double a0 = 5.2918e-11;
  //@}

  /**
   * @name Physical quantities (SI units)
   */
  //@{
  /// solar radius
  static constexpr double R0 = 6.9599e+08;
  //@}

  // End Physical constants, and quantities


  // Begin Physical units

  /**
   * @name Numerical conversion factors
   */
  //@{
  /// e+24 (Y)
  static constexpr double yotta = 1.0e+24;

  /// e+21 (Z)
  static constexpr double zetta = 1.0e+21;

  /// e+18 (E)
  static constexpr double exa = 1.0e+18;

  /// e+15 (P)
  static constexpr double peta = 1.0e+15;

  /// e+12 (T)
  static constexpr double tera = 1.0e+12;

  /// e+09 (G)
  static constexpr double giga = 1.0e+09;

  /// e+06 (M)
  static constexpr double mega = 1.0e+06;

  /// e+03 (k)
  static constexpr double kilo = 1.0e+03;

  /// e+02 (h)
  static constexpr double hecto = 1.0e+02;

  /// e+01 (D)
  static constexpr double deka = 1.0e+01;

  /// e-01 (d)
  static constexpr double deci = 1.0e-01;

  /// e-02 (c)
  static constexpr double centi = 1.0e-02;

  /// e-03 (m)
  static constexpr double milli = 1.0e-03;

  /// e-06 (u)
  static constexpr double micro = 1.0e-06;

  /// e-09 (n)
  static constexpr double nano = 1.0e-09;

  /// e-12 (p)
  static constexpr double pico = 1.0e-12;

  /// e-15 (f)
  static constexpr double femto = 1.0e-15;

  /// e-18 (a)
  static constexpr double atto = 1.0e-18;

  /// e-21 (z)
  static constexpr double zepto = 1.0e-21;

  /// e-24 (y)
  static constexpr double yocto = 1.0e-24;
  //@}

  /**
   * @name Angular measure
   */
  //@{
  /// radian
  static constexpr double radian = 1.0;

  /// circle
  static constexpr double circle = (2.0 * C::pi) * C::radian;

  /// circuit
  static constexpr double circuit = C::circle;

  /// cycle
  static constexpr double cycle = C::circle;

  /// revolution
  static constexpr double rev = C::circle;

  /// revolution
  static constexpr double revolution = C::circle;

  /// rotation
  static constexpr double rotation = C::circle;

  /// degree
  static constexpr double degree = (1.0 / 360.0) * C::circle;

  /// arcminute
  static constexpr double arcmin = (1.0 /  60.0) * C::degree;

  /// arcsecond
  static constexpr double arcsec = (1.0 /  60.0) * C::arcmin;

  /// grad
  static constexpr double grad = (1.0 / 400.0) * C::circle;
  //@}

  /**
   * Solid angular measure
   */
  //@{
  /// steradian
  static constexpr double steradian = 1.0;

  /// sphere
  static constexpr double sphere = (4.0 * C::pi) * C::steradian;

  /// square degree
  static constexpr double square_degree = C::degree * C::degree;

  /// square arcminute
  static constexpr double square_arcmin = C::arcmin * C::arcmin;

  /// square arcsecond
  static constexpr double square_arcsec = C::arcsec * C::arcsec;
  //@}

  // End Numerical conversion factors


  // Begin Physical conversion factors

  /**
   * @name Time interval [T]
   */
  //@{
  /// second
  static constexpr double second = 1.0;

  /// minute
  static constexpr double minute = 60.0 * C::second;

  /// hour
  static constexpr double hour = 60.0 * C::minute;

  /// day
  static constexpr double day = 24.0 * C::hour;

  /// week
  static constexpr double week = 7.0 * C::day;

  /// fortnight
  static constexpr double fortnight = 2.0 * C::week;
  //@}

  /**
   * @nameFrequency [1/T]
   */
  //@{
  /// Hertz
  static constexpr double Hertz = 1.0 / C::second;
  //@}

  /**
   * @name Length [L]
   */
  //@{
  /// metre
  static constexpr double metre = 1.0;

  /// metre (American spelling)
  static constexpr double meter = C::metre;

  /// Fermi
  static constexpr double Fermi = 1.0e-15 * C::metre;

  /// Angstrom
  static constexpr double Angstrom = 1.0e-10 * C::metre;

  /// inch
  static constexpr double inch = 2.54e-2 * C::metre;

  /// thou (inch/1000)
  static constexpr double thou = 1.0e-3 * C::inch;

  /// hand
  static constexpr double hand = 4.0 * C::inch;

  /// span
  static constexpr double span = 9.0 * C::inch;

  /// foot
  static constexpr double foot = 12.0 * C::inch;

  /// yard
  static constexpr double yard = 3.0 * C::foot;

  /// fathom
  static constexpr double fathom = 6.0 * C::foot;

  /// rod
  static constexpr double rod = 16.5 * C::foot;

  /// perch
  static constexpr double perch = C::rod;

  /// rope
  static constexpr double rope = 20.0 * C::foot;

  /// chain
  static constexpr double chain = 22.0 * C::yard;

  /// furlong
  static constexpr double furlong = 220.0 * C::yard;

  /// English statute mile
  static constexpr double mile = 5280.0 * C::foot;

  /// nautical mile
  static constexpr double nautical_mile = 6080.0 * C::foot;

  /// point
  static constexpr double point = (1.0 / 72.0) * C::inch;

  /// pica
  static constexpr double pica = 12.0 * C::point;

  /// astronomical unit
  static constexpr double astronomical_unit = 1.495978707e+11 * C::metre;

  /// light second
  static constexpr double light_second = C::c * C::metre;

  /// light year
  static constexpr double light_year = 9.46073047e+15 * C::metre;

  /// parsec
  static constexpr double parsec = 3.26156378 * C::light_year;
  //@}

  /**
   * @name Area [L*L]
   */
  //@{
  /// square metre
  static constexpr double square_metre = C::metre * C::metre;

  /// square meter (American spelling)
  static constexpr double square_meter = C::square_metre;

  /// are
  static constexpr double are = 100.0 * C::square_metre;

  /// barn
  static constexpr double barn = 1.0e-28 * C::square_metre;

  /// square inch
  static constexpr double square_inch = C::inch * C::inch;

  /// square foot
  static constexpr double square_foot = C::foot * C::foot;

  /// square yard
  static constexpr double square_yard = C::yard * C::yard;

  /// square mile
  static constexpr double square_mile = C::mile * C::mile;

  /// square perch
  static constexpr double square_perch = C::perch * C::perch;

  /// rood
  static constexpr double rood = 40.0 * C::square_perch;

  /// acre
  static constexpr double acre = 4.0 * C::rood;

  /// square
  static constexpr double square = 100.0 * C::square_foot;
  //@}

  /**
   * @name Volume [L*L*L]
   */
  //@{
  /// cubic metre
  static constexpr double cubic_metre = C::metre * C::square_metre;

  /// cubic meter (American spelling)
  static constexpr double cubic_meter = C::cubic_metre;

  /// stere
  static constexpr double stere = C::cubic_metre;

  /// litre
  static constexpr double litre = 1.0e-3 * C::cubic_metre;

  /// liter (American spelling)
  static constexpr double liter = C::litre;

  /// cubic inch
  static constexpr double cubic_inch = C::inch * C::square_inch;

  /// cubic foot
  static constexpr double cubic_foot = C::foot * C::square_foot;

  /// cubic yard
  static constexpr double cubic_yard = C::yard * C::square_yard;

  /// cubic mile
  static constexpr double cubic_mile = C::mile * C::square_mile;

  /// (Brit) gallon
  static constexpr double gallon = 277.4193 * C::cubic_inch;

  /// (Brit) quart
  static constexpr double quart = (1.0 / 4.0) * C::gallon;

  /// (Brit) pint
  static constexpr double pint = (1.0 / 2.0) * C::quart;

  /// (Brit) gill
  static constexpr double gill = (1.0 / 4.0) * C::pint;

  /// (Brit) fluid ounce
  static constexpr double fluid_ounce = (1.0 / 5.0) * C::gill;

  /// (Brit) drachm
  static constexpr double drachm = (1.0 / 8.0) * C::fluid_ounce;

  /// (Brit) scruple
  static constexpr double scruple = (1.0 / 3.0) * C::drachm;

  /// (Brit) minim
  static constexpr double minim = (1.0 / 20.0) * C::scruple;

  /// (US liq) gallon
  static constexpr double USgallon = 231.0 * C::cubic_inch;

  /// (US liq) quart
  static constexpr double USquart = (1.0 / 4.0) * C::USgallon;

  /// (US liq) pint
  static constexpr double USpint = (1.0 / 2.0) * C::USquart;

  /// (US liq) gill
  static constexpr double USgill = (1.0 / 4.0) * C::USpint;

  /// (US liq) fluid ounce
  static constexpr double USfluid_ounce = (1.0 / 4.0) * C::USgill;

  /// (US liq) dram
  static constexpr double USdram = (1.0 / 8.0) * C::USfluid_ounce;

  /// (US liq) minim
  static constexpr double USminim = (1.0 / 60.0) * C::USdram;
  //@}

  /**
   * @name Speed [L/T]
   */
  //@{
  /// nautical miles per hour
  static constexpr double knot = C::nautical_mile / C::hour;
  //@}

  /**
   * @name Acceleration (speed / time) [L/(T*T)]
   */
  //@{
  /// gravitational acceleration
  static constexpr double g = 9.80665 * C::metre / C::second / C::second;
  //@}

  /**
   * @name Mass [M]
   */
  //@{
  /// gram
  static constexpr double gram = 0.001;

  /// metric ton
  static constexpr double tonne = 1000.0 * C::kilo * C::gram;

  /// metric carat
  static constexpr double carat = (1.0 / 5.0) * C::gram;

  /// pound (avoirdupois)
  static constexpr double pound = 0.45359237 * C::kilo * C::gram;

  /// ounce (avoirdupois)
  static constexpr double ounce = (1.0 / 16.0) * C::pound;

  /// stone
  static constexpr double stone = 14.0 * C::pound;

  /// stone
  static constexpr double quarter = 2.0 * C::stone;

  /// (long) hundredweight
  static constexpr double hundredweight = 4.0 * C::quarter;

  /// (long) ton
  static constexpr double ton = 20.0 * C::hundredweight;

  /// cental
  static constexpr double cental = 100.0 * C::pound;

  /// short quarter (Brit)
  static constexpr double shortquarter = 25.0 * C::pound;

  /// short hundredweight
  static constexpr double shortcwt = 4.0 * C::shortquarter;

  /// short ton
  static constexpr double shortton = 20.0 * C::shortcwt;
  //@}

  /**
   * @name Force (mass * acceleration) [M*L/(T*T)]
   */
  //@{

  /// Newton
  static constexpr double Newton =
    C::kilo * C::gram * C::metre / C::second / C::second;

  /// dyne
  static constexpr double dyne = 1.0e-5 * C::Newton;
  //@}

  /**
   * @name Pressure (force / area) [M/(L*T*T)]
   */
  //@{
  /// Pascal
  static constexpr double Pascal = C::Newton / C::square_metre;

  /// atmosphere
  static constexpr double atmosphere = 1.01325e+5 * C::Pascal;

  /// bar
  static constexpr double bar = 1.0e+5 * C::Pascal;

  /// torr
  static constexpr double torr = (1.0 / 760.0) * C::atmosphere;

  /// mm of Mercury
  static constexpr double mmHg = (13.5951 * C::g) * C::Pascal;
  //@}

  /**
   * @name Energy (force * length) [M*L*L/(T*T)]
   */
  //@{
  /// Joule
  static constexpr double Joule = C::Newton * C::metre;

  /// kiloWatt * hour
  static constexpr double kWh = 3.6e+6 * C::Joule;

  /// erg
  static constexpr double erg = 1.0e-7 * C::Joule;

  /// calorie (thermochemical)
  static constexpr double calorie = 4.184 * C::Joule;

  /// calorie (International Steam)
  static constexpr double calorie_IT = 4.1868 * C::Joule;

  /// British thermal unit
  static constexpr double Btu = 1.05435e+3 * C::Joule;

  /// electron volt
  static constexpr double eV = C::qe * C::Joule;
  //@}

  /**
   * @name Temperature difference (energy) [M*L*L/(T*T)]
   */
  //@{
  /// Kelvin
  static constexpr double Kelvin = C::Boltzmann * C::Joule;

  /// Celsius
  static constexpr double Celsius = C::Kelvin;

  /// Centigrade
  static constexpr double Centigrade = C::Celsius;

  /// Fahrenheit
  static constexpr double Fahrenheit = (5.0 / 9.0) * C::Kelvin;

  /// Rankine
  static constexpr double Rankine = C::Fahrenheit;
  //@}

  /**
   * @name Temperature at 0 on each temperature scale
   */
  //@{
  /// Absolute Zero (Kelvins)
  static constexpr double Kelvin_0 = 0.0 * C::Kelvin;

  /// Celsius Zero
  static constexpr double Celsius_0 = 273.15 * C::Kelvin;

  /// Centigrade Zero
  static constexpr double Centigrade_0 = C::Celsius_0;

  /// Fahrenheit Zero
  static constexpr double Fahrenheit_0 = 459.67 * C::Fahrenheit;

  /// Absolute Zero (Rankines)
  static constexpr double Rankine_0 = 0.0 * C::Fahrenheit;
  //@}

  /**
   * @name Power (energy / time) [M*L*L/(T*T*T)]
   */
  //@{
  /// Watt
  static constexpr double Watt = C::Joule / C::second;

  /// horsepower
  static constexpr double horsepower = 745.7 * C::Watt;
  //@}

  /**
   * @name Flux density (power / area / frequency) [M/(T*T)]
   */
  //@{
  /// Jansky
  static constexpr double Jansky = 1.0e-26 * C::Watt / C::square_metre / C::Hertz;

  /// flux units
  static constexpr double fu = C::Jansky;
  //@}

  /**
   * @name Electric charge [Q] (Coulomb)
   */
  //@{
  /// Coulomb
  static constexpr double Coulomb = 1.0;

  /// abCoulomb (emu)
  static constexpr double abCoulomb = 10.0 * C::Coulomb;

  /// statCoulomb (esu)
  static constexpr double statCoulomb = (0.1 / C::c) * C::Coulomb;
  //@}

  /**
   * @name Electric current (charge / time) [Q/T]
   */
  //@{
  /// Ampere
  static constexpr double Ampere = C::Coulomb / C::second;

  /// abAmpere (emu)
  static constexpr double abAmpere = 10.0 * C::Ampere;

  /// statAmpere (esu)
  static constexpr double statAmpere = (0.1 / C::c) * C::Ampere;
  //@}

  // Electric field strength (force / charge) [M*L/(T*T*Q)]

  /**
   * @name Electric potential (energy / charge) [M*L*L/(T*T*Q)]
   */
  //@{
  /// Volt
  static constexpr double Volt = C::Joule / C::Coulomb;

  /// abVolt (emu)
  static constexpr double abVolt = 1.0e-8 * C::Volt;

  /// statVolt (esu)
  static constexpr double statVolt = (C::c * 1.0e-6) * C::Volt;
  //@}

  /**
   * @name Electric resistance (potential / current) [M*L*L/(T*Q*Q)]]
   */
  //@{
  /// Ohm
  static constexpr double Ohm = C::Volt / C::Ampere;

  /// abOhm (emu)
  static constexpr double abOhm = 1.0e-9 * C::Ohm;

  /// starOhm (esu)
  static constexpr double statOhm = (3.0e+3 * C::c) * C::Ohm;
  //@}

  /**
   * @name Electric conductance (current / potential) [T*Q*Q/(M*L*L)]
   */
  //@{
  /// Siemens
  static constexpr double Siemens = C::Ampere / C::Volt;

  /// mho
  static constexpr double mho = C::Siemens;
  //@}

  /**
   * @name Electric capacitance (charge / potential) [T*T*Q*Q/(M*L*L)]
   */
  //@{
  /// Farad
  static constexpr double Farad = C::Coulomb / C::Volt;

  /// abFarad (emu)
  static constexpr double abFarad = 1.0e+9 * C::Farad;

  /// statFarad (esu)
  static constexpr double statFarad = 1.0 / (3.0e+3 * C::c) * C::Farad;
  //@}

  /**
   * @name Electric inductance (potential * time / current) [M*L*L/(Q*Q)]
   */
  //@{
  /// Henry
  static constexpr double Henry = C::Volt * C::second / C::Ampere;

  /// abHenry (emu)
  static constexpr double abHenry = 1.0e-9 * C::Henry;

  /// statHenry (esu)
  static constexpr double statHenry = (3.0e+3 * C::c) * C::Henry;
  //@}

  /**
   * @name Magnetic induction (force / charge / velocity) [M/(T*Q)]
   */
  //@{
  /// Tesla
  static constexpr double Tesla = C::Newton / C::Coulomb / (C::metre / C::second);

  /// Gauss (emu)
  static constexpr double Gauss = 1.0e-4 * C::Tesla;
  //@}

  /**
   * @name Magnetic flux (magnetic induction * area) [M*L*L/(T*Q)]
   */
  //@{
  /// Weber
  static constexpr double Weber = C::Tesla / C::square_metre;

  /// Maxwell (emu)
  static constexpr double Maxwell = 1.0e-8 * C::Weber;

  /// line (emu)
  static constexpr double line = C::Maxwell;
  //@}

  /**
   * @name Magnetomotance = magnetomotive force (current) [Q/T]
   */
  //@{
  /// Ampere-turn
  static constexpr double Ampere_turn = C::Ampere;

  /// abAmpere-turn
  static constexpr double abAmpere_turn = 10.0 * C::Ampere_turn;

  /// Gilbert (emu)
  static constexpr double Gilbert = 10.0 / (4.0 * C::pi) * C::Ampere_turn;

  /// praGilbert (emu)
  static constexpr double praGilbert = (4.0 * C::pi) * C::Ampere_turn;
  //@}

  // Magnetic field intensity (current / length) [Q/(T*L)]:
  //@{
  /// Oersted (emu)
  static constexpr double Oersted = 1000.0 / (4.0 * C::pi) * C::Ampere / C::metre;

  /// praOersted (emu)
  static constexpr double praOersted = (4.0 * C::pi) * C::Ampere / C::metre;
  //@}

  /**
   * @name Radioactivity (Bequerel)
   */
  //@{
  /// Bequerel
  static constexpr double Bequerel = 1.0;
  //@}

  /**
   * @name Luminous intensity [Iv]
   */
  //@{
  /// candela
  static constexpr double candela = 1.0;
  //@}

  /**
   * @name Amount of substance [N]
   */
  //@{
  /// mole
  static constexpr double mole = 1.0;

  /// number of molecules
  static constexpr double molecule = (1.0 / C::Avogadro) * C::mole;
  //@}

  // End Physical conversion factors

  // End Physical units

}

#endif  /* MARC_AIPS_CONSTANTS_H */
