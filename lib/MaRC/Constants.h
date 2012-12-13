// -*- C++ -*-

//==========================================================================
/**
 *  @file Constants.h
 *
 *  $Id: Constants.h,v 1.3 2004/06/12 04:02:24 othman Exp $
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
 * <ossama@dre.vanderbilt.edu> in November 2003.
 *
 *  @author Associated Universities, Inc. Washington DC, USA
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef MARC_AIPS_CONSTANTS_H
#define MARC_AIPS_CONSTANTS_H

/**
 * @namespace C
 *
 * @brief Various mathematical and physical constants.
 *
 * The conversion factors convert to an internal representation.  For
 * example, C::inch when used as a factor converts from inchs to internal
 * units (in this case metres).  When used as a divisor it converts from
 * internal units to inchs.
 * @verbatim
 *
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
 * For derived quantities such as volume, velocity, force, and power, the
 * units are specified in terms of those of the fundamental quantities.
*/
namespace C
{
  // Begin Mathematical constants


  /// Foo
  void foo (void);

  /**
   * @name Irrationals
   */
  //@{

  /// sqrt(2)
  static const double sqrt2 = 1.4142135623730950488;

  /// sqrt(3)
  static const double sqrt3 = 1.7320508075688772935;

  /// 1/sqrt(2)
  static const double _1_sqrt2 = 0.70710678118654752440;

  /// 1/sqrt(3)
  static const double _1_sqrt3 = 0.57735026918962576451;

  //@}

  /**
   *@name Pi and functions thereof
   */
  //@{
  /// pi
  static const double pi = 3.141592653589793238462643;

  /// 2*pi
  static const double _2pi = 6.283185307179586476925286;

  /// pi/2
  static const double pi_2 = 1.570796326794896619231322;

  /// pi/4
  static const double pi_4 = 0.7853981633974483096156608;

  /// 1/pi
  static const double _1_pi = 0.3183098861837906715377675;

  /// 2/pi
  static const double _2_pi = 0.6366197723675813430755350;

  /// 1/sqrt(pi)
  static const double _1_sqrtpi = 0.5641895835477562869480795;

  /// 2/sqrt(pi)
  static const double _2_sqrtpi = 1.1283791670955125738961590;
  //@}

  /**
   * @name e and functions thereof
   */
  //@{
  /// e
  static const double e = 2.718281828459045235360287;

  /// ln(2)
  static const double ln2 = 0.6931471805599453094172321;

  /// ln(10)
  static const double ln10 = 2.3025850929940456840179915;

  /// log2(e)
  static const double log2e = 1.4426950408889634074;

  /// log10(e)
  static const double log10e = 0.4342944819032518276511289;
  //@}

  /**
   * @name gamma and functions thereof
   */
  //@{
  /// gamma
  static const double gamma = 0.577215664901532860606512;

  /// ln(gamma)
  static const double lngamma = -0.549539312981644822337662;

  /// e**gamma
  static const double etogamma = 1.7810724179901979852;
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
  static const double c = 2.99792458e+08;

  /// Gravitational constant
  static const double Gravity = 6.67259e-11;

  /// Planck's constant
  static const double Planck = 6.6260755e-34;

  /// gas constant
  static const double GasConst = 8.314510;
  /// Avogardo's constant
  static const double Avogadro = 6.0221367e+23;

  /// electron charge
  static const double qe = 1.60217733e-19;

  /// proton mass
  static const double mp = 1.6726231e-27;

  /// proton mass / electron mass
  static const double mp_me = 1836.152701;
  //@}

  /**
   * @name Derived physical constants (SI units)
   */
  //@{
  /// magnetic permeability of vacuum
  static const double mu0 = 4.0e-7 * C::pi;

  /// electric permittivity of vacuum
  static const double epsilon0 = 1.0 / ( C::mu0 * C::c * C::c );

  /// Planck's constant divided by 2*pi
  static const double Planck_2pi = C::Planck / C::_2pi;

  /// atomic mass unit
  static const double u = 1.0e-3 / C::Avogadro;

  /// Boltzmann's constant
  static const double Boltzmann = C::GasConst / C::Avogadro;

  /// Faraday's constant
  static const double Faraday = C::Avogadro * C::qe;

  /// electron mass
  static const double me = C::mp / C::mp_me;

  /// classical electron radius
  static const double re = 2.8179e-15;

  /// Bohr radius
  static const double a0 = 5.2918e-11;
  //@}

  /**
   * @name Physical quantities (SI units)
   */
  //@{
  /// solar radius
  static const double R0 = 6.9599e+08;
  //@}

  // End Physical constants, and quantities


  // Begin Physical units

  /**
   * @name Numerical conversion factors
   */
  //@{
  /// e+24 (Y)
  static const double yotta = 1.0e+24;

  /// e+21 (Z)
  static const double zetta = 1.0e+21;

  /// e+18 (E)
  static const double exa = 1.0e+18;

  /// e+15 (P)
  static const double peta = 1.0e+15;

  /// e+12 (T)
  static const double tera = 1.0e+12;

  /// e+09 (G)
  static const double giga = 1.0e+09;

  /// e+06 (M)
  static const double mega = 1.0e+06;

  /// e+03 (k)
  static const double kilo = 1.0e+03;

  /// e+02 (h)
  static const double hecto = 1.0e+02;

  /// e+01 (D)
  static const double deka = 1.0e+01;

  /// e-01 (d)
  static const double deci = 1.0e-01;

  /// e-02 (c)
  static const double centi = 1.0e-02;

  /// e-03 (m)
  static const double milli = 1.0e-03;

  /// e-06 (u)
  static const double micro = 1.0e-06;

  /// e-09 (n)
  static const double nano = 1.0e-09;

  /// e-12 (p)
  static const double pico = 1.0e-12;

  /// e-15 (f)
  static const double femto = 1.0e-15;

  /// e-18 (a)
  static const double atto = 1.0e-18;

  /// e-21 (z)
  static const double zepto = 1.0e-21;

  /// e-24 (y)
  static const double yocto = 1.0e-24;
  //@}

  /**
   * @name Angular measure
   */
  //@{
  /// radian
  static const double radian = 1.0;

  /// circle
  static const double circle = (2.0 * C::pi) * C::radian;

  /// circuit
  static const double circuit = C::circle;

  /// cycle
  static const double cycle = C::circle;

  /// revolution
  static const double rev = C::circle;

  /// revolution
  static const double revolution = C::circle;

  /// rotation
  static const double rotation = C::circle;

  /// degree
  static const double degree = (1.0 / 360.0) * C::circle;

  /// arcminute
  static const double arcmin = (1.0 /  60.0) * C::degree;

  /// arcsecond
  static const double arcsec = (1.0 /  60.0) * C::arcmin;

  /// grad
  static const double grad = (1.0 / 400.0) * C::circle;
  //@}

  /**
   * Solid angular measure
   */
  //@{
  /// steradian
  static const double steradian = 1.0;

  /// sphere
  static const double sphere = (4.0 * C::pi) * C::steradian;

  /// square degree
  static const double square_degree = C::degree * C::degree;

  /// square arcminute
  static const double square_arcmin = C::arcmin * C::arcmin;

  /// square arcsecond
  static const double square_arcsec = C::arcsec * C::arcsec;
  //@}

  // End Numerical conversion factors


  // Begin Physical conversion factors

  /**
   * @name Time interval [T]
   */
  //@{
  /// second
  static const double second = 1.0;

  /// minute
  static const double minute = 60.0 * C::second;

  /// hour
  static const double hour = 60.0 * C::minute;

  /// day
  static const double day = 24.0 * C::hour;

  /// week
  static const double week = 7.0 * C::day;

  /// fortnight
  static const double fortnight = 2.0 * C::week;
  //@}

  /**
   * @nameFrequency [1/T]
   */
  //@{
  /// Hertz
  static const double Hertz = 1.0 / C::second;
  //@}

  /**
   * @name Length [L]
   */
  //@{
  /// metre
  static const double metre = 1.0;

  /// metre (American spelling)
  static const double meter = C::metre;

  /// Fermi
  static const double Fermi = 1.0e-15 * C::metre;

  /// Angstrom
  static const double Angstrom = 1.0e-10 * C::metre;

  /// inch
  static const double inch = 2.54e-2 * C::metre;

  /// thou (inch/1000)
  static const double thou = 1.0e-3 * C::inch;

  /// hand
  static const double hand = 4.0 * C::inch;

  /// span
  static const double span = 9.0 * C::inch;

  /// foot
  static const double foot = 12.0 * C::inch;

  /// yard
  static const double yard = 3.0 * C::foot;

  /// fathom
  static const double fathom = 6.0 * C::foot;

  /// rod
  static const double rod = 16.5 * C::foot;

  /// perch
  static const double perch = C::rod;

  /// rope
  static const double rope = 20.0 * C::foot;

  /// chain
  static const double chain = 22.0 * C::yard;

  /// furlong
  static const double furlong = 220.0 * C::yard;

  /// English statute mile
  static const double mile = 5280.0 * C::foot;

  /// nautical mile
  static const double nautical_mile = 6080.0 * C::foot;

  /// point
  static const double point = (1.0 / 72.0) * C::inch;

  /// pica
  static const double pica = 12.0 * C::point;

  /// astronomical unit
  static const double astronomical_unit = 1.4959787066e+11 * C::metre;

  /// light second
  static const double light_second = C::c * C::metre;

  /// light year
  static const double light_year = 9.46073047e+15 * C::metre;

  /// parsec
  static const double parsec = 3.26156378 * C::light_year;
  //@}

  /**
   * @name Area [L*L]
   */
  //@{
  /// square metre
  static const double square_metre = C::metre * C::metre;

  /// square meter (American spelling)
  static const double square_meter = C::square_metre;

  /// are
  static const double are = 100.0 * C::square_metre;

  /// barn
  static const double barn = 1.0e-28 * C::square_metre;

  /// square inch
  static const double square_inch = C::inch * C::inch;

  /// square foot
  static const double square_foot = C::foot * C::foot;

  /// square yard
  static const double square_yard = C::yard * C::yard;

  /// square mile
  static const double square_mile = C::mile * C::mile;

  /// square perch
  static const double square_perch = C::perch * C::perch;

  /// rood
  static const double rood = 40.0 * C::square_perch;

  /// acre
  static const double acre = 4.0 * C::rood;

  /// square
  static const double square = 100.0 * C::square_foot;
  //@}

  /**
   * @name Volume [L*L*L]
   */
  //@{
  /// cubic metre
  static const double cubic_metre = C::metre * C::square_metre;

  /// cubic meter (American spelling)
  static const double cubic_meter = C::cubic_metre;

  /// stere
  static const double stere = C::cubic_metre;

  /// litre
  static const double litre = 1.0e-3 * C::cubic_metre;

  /// liter (American spelling)
  static const double liter = C::litre;

  /// cubic inch
  static const double cubic_inch = C::inch * C::square_inch;

  /// cubic foot
  static const double cubic_foot = C::foot * C::square_foot;

  /// cubic yard
  static const double cubic_yard = C::yard * C::square_yard;

  /// cubic mile
  static const double cubic_mile = C::mile * C::square_mile;

  /// (Brit) gallon
  static const double gallon = 277.4193 * C::cubic_inch;

  /// (Brit) quart
  static const double quart = (1.0 / 4.0) * C::gallon;

  /// (Brit) pint
  static const double pint = (1.0 / 2.0) * C::quart;

  /// (Brit) gill
  static const double gill = (1.0 / 4.0) * C::pint;

  /// (Brit) fluid ounce
  static const double fluid_ounce = (1.0 / 5.0) * C::gill;

  /// (Brit) drachm
  static const double drachm = (1.0 / 8.0) * C::fluid_ounce;

  /// (Brit) scruple
  static const double scruple = (1.0 / 3.0) * C::drachm;

  /// (Brit) minim
  static const double minim = (1.0 / 20.0) * C::scruple;

  /// (US liq) gallon
  static const double USgallon = 231.0 * C::cubic_inch;

  /// (US liq) quart
  static const double USquart = (1.0 / 4.0) * C::USgallon;

  /// (US liq) pint
  static const double USpint = (1.0 / 2.0) * C::USquart;

  /// (US liq) gill
  static const double USgill = (1.0 / 4.0) * C::USpint;

  /// (US liq) fluid ounce
  static const double USfluid_ounce = (1.0 / 4.0) * C::USgill;

  /// (US liq) dram
  static const double USdram = (1.0 / 8.0) * C::USfluid_ounce;

  /// (US liq) minim
  static const double USminim = (1.0 / 60.0) * C::USdram;
  //@}

  /**
   * @name Speed [L/T]
   */
  //@{
  /// nautical miles per hour
  static const double knot = C::nautical_mile / C::hour;
  //@}

  /**
   * @name Acceleration (speed / time) [L/(T*T)]
   */
  //@{
  /// gravitational acceleration
  static const double g = 9.80665 * C::metre / C::second / C::second;
  //@}

  /**
   * @name Mass [M]
   */
  //@{
  /// gram
  static const double gram = 0.001;

  /// metric ton
  static const double tonne = 1000.0 * C::kilo * C::gram;

  /// metric carat
  static const double carat = (1.0 / 5.0) * C::gram;

  /// pound (avoirdupois)
  static const double pound = 0.45359237 * C::kilo * C::gram;

  /// ounce (avoirdupois)
  static const double ounce = (1.0 / 16.0) * C::pound;

  /// stone
  static const double stone = 14.0 * C::pound;

  /// stone
  static const double quarter = 2.0 * C::stone;

  /// (long) hundredweight
  static const double hundredweight = 4.0 * C::quarter;

  /// (long) ton
  static const double ton = 20.0 * C::hundredweight;

  /// cental
  static const double cental = 100.0 * C::pound;

  /// short quarter (Brit)
  static const double shortquarter = 25.0 * C::pound;

  /// short hundredweight
  static const double shortcwt = 4.0 * C::shortquarter;

  /// short ton
  static const double shortton = 20.0 * C::shortcwt;
  //@}

  /**
   * @name Force (mass * acceleration) [M*L/(T*T)]
   */
  //@{

  /// Newton
  static const double Newton =
    C::kilo * C::gram * C::metre / C::second / C::second;

  /// dyne
  static const double dyne = 1.0e-5 * C::Newton;
  //@}

  /**
   * @name Pressure (force / area) [M/(L*T*T)]
   */
  //@{
  /// Pascal
  static const double Pascal = C::Newton / C::square_metre;

  /// atmosphere
  static const double atmosphere = 1.01325e+5 * C::Pascal;

  /// bar
  static const double bar = 1.0e+5 * C::Pascal;

  /// torr
  static const double torr = (1.0 / 760.0) * C::atmosphere;

  /// mm of Mercury
  static const double mmHg = (13.5951 * C::g) * C::Pascal;
  //@}

  /**
   * @name Energy (force * length) [M*L*L/(T*T)]
   */
  //@{
  /// Joule
  static const double Joule = C::Newton * C::metre;

  /// kiloWatt * hour
  static const double kWh = 3.6e+6 * C::Joule;

  /// erg
  static const double erg = 1.0e-7 * C::Joule;

  /// calorie (thermochemical)
  static const double calorie = 4.184 * C::Joule;

  /// calorie (International Steam)
  static const double calorie_IT = 4.1868 * C::Joule;

  /// British thermal unit
  static const double Btu = 1.05435e+3 * C::Joule;

  /// electron volt
  static const double eV = C::qe * C::Joule;
  //@}

  /**
   * @name Temperature difference (energy) [M*L*L/(T*T)]
   */
  //@{
  /// Kelvin
  static const double Kelvin = C::Boltzmann * C::Joule;

  /// Celsius
  static const double Celsius = C::Kelvin;

  /// Centigrade
  static const double Centigrade = C::Celsius;

  /// Fahrenheit
  static const double Fahrenheit = (5.0 / 9.0) * C::Kelvin;

  /// Rankine
  static const double Rankine = C::Fahrenheit;
  //@}

  /**
   * @name Temperature at 0 on each temperature scale
   */
  //@{
  /// Absolute Zero (Kelvins)
  static const double Kelvin_0 = 0.0 * C::Kelvin;

  /// Celsius Zero
  static const double Celsius_0 = 273.15 * C::Kelvin;

  /// Centigrade Zero
  static const double Centigrade_0 = C::Celsius_0;

  /// Fahrenheit Zero
  static const double Fahrenheit_0 = 459.67 * C::Fahrenheit;

  /// Absolute Zero (Rankines)
  static const double Rankine_0 = 0.0 * C::Fahrenheit;
  //@}

  /**
   * @name Power (energy / time) [M*L*L/(T*T*T)]
   */
  //@{
  /// Watt
  static const double Watt = C::Joule / C::second;

  /// horsepower
  static const double horsepower = 745.7 * C::Watt;
  //@}

  /**
   * @name Flux density (power / area / frequency) [M/(T*T)]
   */
  //@{
  /// Jansky
  static const double Jansky = 1.0e-26 * C::Watt / C::square_metre / C::Hertz;

  /// flux units
  static const double fu = C::Jansky;
  //@}

  /**
   * @name Electric charge [Q] (Coulomb)
   */
  //@{
  /// Coulomb
  static const double Coulomb = 1.0;

  /// abCoulomb (emu)
  static const double abCoulomb = 10.0 * C::Coulomb;

  /// statCoulomb (esu)
  static const double statCoulomb = (0.1 / C::c) * C::Coulomb;
  //@}

  /**
   * @name Electric current (charge / time) [Q/T]
   */
  //@{
  /// Ampere
  static const double Ampere = C::Coulomb / C::second;

  /// abAmpere (emu)
  static const double abAmpere = 10.0 * C::Ampere;

  /// statAmpere (esu)
  static const double statAmpere = (0.1 / C::c) * C::Ampere;
  //@}

  // Electric field strength (force / charge) [M*L/(T*T*Q)]

  /**
   * @name Electric potential (energy / charge) [M*L*L/(T*T*Q)]
   */
  //@{
  /// Volt
  static const double Volt = C::Joule / C::Coulomb;

  /// abVolt (emu)
  static const double abVolt = 1.0e-8 * C::Volt;

  /// statVolt (esu)
  static const double statVolt = (C::c * 1.0e-6) * C::Volt;
  //@}

  /**
   * @name Electric resistance (potential / current) [M*L*L/(T*Q*Q)]]
   */
  //@{
  /// Ohm
  static const double Ohm = C::Volt / C::Ampere;

  /// abOhm (emu)
  static const double abOhm = 1.0e-9 * C::Ohm;

  /// starOhm (esu)
  static const double statOhm = (3.0e+3 * C::c) * C::Ohm;
  //@}

  /**
   * @name Electric conductance (current / potential) [T*Q*Q/(M*L*L)]
   */
  //@{
  /// Siemens
  static const double Siemens = C::Ampere / C::Volt;

  /// mho
  static const double mho = C::Siemens;
  //@}

  /**
   * @name Electric capacitance (charge / potential) [T*T*Q*Q/(M*L*L)]
   */
  //@{
  /// Farad
  static const double Farad = C::Coulomb / C::Volt;

  /// abFarad (emu)
  static const double abFarad = 1.0e+9 * C::Farad;

  /// statFarad (esu)
  static const double statFarad = 1.0 / (3.0e+3 * C::c) * C::Farad;
  //@}

  /**
   * @name Electric inductance (potential * time / current) [M*L*L/(Q*Q)]
   */
  //@{
  /// Henry
  static const double Henry = C::Volt * C::second / C::Ampere;

  /// abHenry (emu)
  static const double abHenry = 1.0e-9 * C::Henry;

  /// statHenry (esu)
  static const double statHenry = (3.0e+3 * C::c) * C::Henry;
  //@}

  /**
   * @name Magnetic induction (force / charge / velocity) [M/(T*Q)]
   */
  //@{
  /// Tesla
  static const double Tesla = C::Newton / C::Coulomb / (C::metre / C::second);

  /// Gauss (emu)
  static const double Gauss = 1.0e-4 * C::Tesla;
  //@}

  /**
   * @name Magnetic flux (magnetic induction * area) [M*L*L/(T*Q)]
   */
  //@{
  /// Weber
  static const double Weber = C::Tesla / C::square_metre;

  /// Maxwell (emu)
  static const double Maxwell = 1.0e-8 * C::Weber;

  /// line (emu)
  static const double line = C::Maxwell;
  //@}

  /**
   * @name Magnetomotance = magnetomotive force (current) [Q/T]
   */
  //@{
  /// Ampere-turn
  static const double Ampere_turn = C::Ampere;

  /// abAmpere-turn
  static const double abAmpere_turn = 10.0 * C::Ampere_turn;

  /// Gilbert (emu)
  static const double Gilbert = 10.0 / (4.0 * C::pi) * C::Ampere_turn;

  /// praGilbert (emu)
  static const double praGilbert = (4.0 * C::pi) * C::Ampere_turn;
  //@}

  // Magnetic field intensity (current / length) [Q/(T*L)]:
  //@{
  /// Oersted (emu)
  static const double Oersted = 1000.0 / (4.0 * C::pi) * C::Ampere / C::metre;

  /// praOersted (emu)
  static const double praOersted = (4.0 * C::pi) * C::Ampere / C::metre;
  //@}

  /**
   * @name Radioactivity (Bequerel)
   */
  //@{
  /// Bequerel
  static const double Bequerel = 1.0;
  //@}

  /**
   * @name Luminous intensity [Iv]
   */
  //@{
  /// candela
  static const double candela = 1.0;
  //@}

  /**
   * @name Amount of substance [N]
   */
  //@{
  /// mole
  static const double mole = 1.0;

  /// number of molecules
  static const double molecule = (1.0 / C::Avogadro) * C::mole;
  //@}

  // End Physical conversion factors

  // End Physical units

}

#endif  /* MARC_AIPS_CONSTANTS_H */
