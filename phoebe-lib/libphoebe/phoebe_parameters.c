#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "phoebe_build_config.h"

#include "phoebe_accessories.h"
#include "phoebe_data.h"
#include "phoebe_error_handling.h"
#include "phoebe_global.h"
#include "phoebe_parameters.h"
#include "phoebe_types.h"

int phoebe_init_parameters ()
{
	/*
	 * This (and only this) function declares parameters that are used in
	 * PHOEBE; there is no GUI connection or any other plug-in connection in
	 * this function, only native PHOEBE parameters. PHOEBE drivers, such
	 * as the scripter or the GUI, have to initialize the parameters by
	 * analogous functions.
	 *
	 * Return values:
	 *
	 *   SUCCESS
	 */

	/* **********************   Model parameters   ************************** */

	phoebe_parameter_add ("phoebe_name",                 "Common name of the binary",                  KIND_PARAMETER,  NULL, 0.0, 0.0, 0.0, NO, TYPE_STRING,       "");
	phoebe_parameter_add ("phoebe_indep",                "Independent modeling variable",              KIND_MENU,       NULL, 0.0, 0.0, 0.0, NO, TYPE_STRING,       "Phase");
	phoebe_parameter_add ("phoebe_model",                "Morphological constraints",                  KIND_MENU,       NULL, 0.0, 0.0, 0.0, NO, TYPE_STRING,       "Unconstrained binary system");

	phoebe_parameter_add ("phoebe_lcno",                 "Number of observed light curves",            KIND_MODIFIER,   NULL, 0.0, 0.0, 0.0, NO, TYPE_INT,          0);
	phoebe_parameter_add ("phoebe_rvno",                 "Number of observed RV curves",               KIND_MODIFIER,   NULL, 0.0, 0.0, 0.0, NO, TYPE_INT,          0);
	phoebe_parameter_add ("phoebe_spno",                 "Number of observed spectra",                 KIND_MODIFIER,   NULL, 0.0, 0.0, 0.0, NO, TYPE_INT,          0);

	/* **********************   Model constraints   ************************* */

	phoebe_parameter_add ("phoebe_asini_switch",         "(a sin i) is kept constant",                 KIND_SWITCH,     NULL,   0.0,    0.0,    0.0,  NO, TYPE_BOOL,         NO);
	phoebe_parameter_add ("phoebe_asini",                "(a sin i) constant",                         KIND_PARAMETER,  NULL,   0.0,   1E10,    0.0,  NO, TYPE_DOUBLE,       10.0);

	phoebe_parameter_add ("phoebe_cindex_switch",        "Use the color-index constraint",             KIND_SWITCH,     NULL,   0.0,    0.0,    0.0,  NO, TYPE_BOOL,         NO);
	phoebe_parameter_add ("phoebe_cindex",               "Color-index values",                         KIND_PARAMETER,  "phoebe_lcno",  -100,    100,   1e-2,  NO, TYPE_DOUBLE_ARRAY, 1.0);

	phoebe_parameter_add ("phoebe_msc1_switch",          "Main-sequence constraint for star 1",        KIND_PARAMETER,  NULL,     0,      0,      0,  NO, TYPE_BOOL,         NO);
	phoebe_parameter_add ("phoebe_msc2_switch",          "Main-sequence constraint for star 2",        KIND_PARAMETER,  NULL,     0,      0,      0,  NO, TYPE_BOOL,         NO);

	/* ***********************   Data parameters   ************************** */

	phoebe_parameter_add ("phoebe_lc_filename",          "Observed LC data filename",                  KIND_PARAMETER,  "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Undefined");
	phoebe_parameter_add ("phoebe_lc_sigma",             "Observed LC data standard deviation",        KIND_PARAMETER,  "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_DOUBLE_ARRAY, 0.01);
	phoebe_parameter_add ("phoebe_lc_filter",            "Observed LC data filter",                    KIND_MENU,       "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "440nm (B)");
	phoebe_parameter_add ("phoebe_lc_indep",             "Observed LC data independent variable",      KIND_MENU,       "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Time (HJD)");
	phoebe_parameter_add ("phoebe_lc_dep",               "Observed LC data dependent variable",        KIND_MENU,       "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Magnitude");
	phoebe_parameter_add ("phoebe_lc_indweight",         "Observed LC data individual weighting",      KIND_MENU,       "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Standard deviation");
	phoebe_parameter_add ("phoebe_lc_levweight",         "Observed LC data level weighting",           KIND_MENU,       "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Poissonian scatter");
	phoebe_parameter_add ("phoebe_lc_active",            "Observed LC data is used",                   KIND_SWITCH,     "phoebe_lcno",   0.0,    0.0,    0.0, NO, TYPE_BOOL_ARRAY,    YES);

	phoebe_parameter_add ("phoebe_rv_filename",          "Observed RV data filename",                  KIND_PARAMETER,  "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Undefined");
	phoebe_parameter_add ("phoebe_rv_sigma",             "Observed RV data standard deviation",        KIND_PARAMETER,  "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_DOUBLE_ARRAY, 1.0);
	phoebe_parameter_add ("phoebe_rv_filter",            "Observed RV data filter",                    KIND_MENU,       "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "550nm (V)");
	phoebe_parameter_add ("phoebe_rv_indep",             "Observed RV data independent variable",      KIND_MENU,       "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Time (HJD)");
	phoebe_parameter_add ("phoebe_rv_dep",               "Observed RV data dependent variable",        KIND_MENU,       "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Primary RV");
	phoebe_parameter_add ("phoebe_rv_indweight",         "Observed RV data individual weighting",      KIND_MENU,       "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_STRING_ARRAY, "Standard deviation");
	phoebe_parameter_add ("phoebe_rv_active",            "Observed RV data is used",                   KIND_SWITCH,     "phoebe_rvno",   0.0,    0.0,    0.0, NO, TYPE_BOOL_ARRAY,    YES);

	phoebe_parameter_add ("phoebe_mnorm",                "Flux-normalizing magnitude",                 KIND_PARAMETER,  NULL,   0.0,    0.0,    0.0, NO, TYPE_DOUBLE,       10.0);

	phoebe_parameter_add ("phoebe_bins_switch",          "Data binning",                               KIND_SWITCH,     NULL,   0.0,    0.0,    0.0, NO, TYPE_BOOL,           NO);
	phoebe_parameter_add ("phoebe_bins",                 "Number of bins",                             KIND_PARAMETER,  NULL,   0.0,    0.0,    0.0, NO, TYPE_INT,           100);

	phoebe_parameter_add ("phoebe_ie_switch",            "Interstellar extinction (reddening)",        KIND_SWITCH,     NULL,   0.0,    0.0,    0.0, NO, TYPE_BOOL,           NO);
	phoebe_parameter_add ("phoebe_ie_factor",            "Interstellar extinction coefficient",        KIND_PARAMETER,  NULL,   0.0,    0.0,    0.0, NO, TYPE_DOUBLE,        3.1);
	phoebe_parameter_add ("phoebe_ie_excess",            "Interstellar extinction color excess value", KIND_PARAMETER,  NULL,   0.0,    0.0,    0.0, NO, TYPE_DOUBLE,        0.0);

	phoebe_parameter_add ("phoebe_proximity_rv1_switch", "Proximity effects for primary star RV",      KIND_SWITCH,     NULL,   0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);
	phoebe_parameter_add ("phoebe_proximity_rv2_switch", "Proximity effects for secondary star RV",    KIND_SWITCH,     NULL,   0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);

	/* **********************   System parameters   ************************* */

	phoebe_parameter_add ("phoebe_hjd0",                 "Origin of HJD time",                         KIND_ADJUSTABLE, NULL, -1E10,   1E10, 0.0001, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_period",               "Orbital period in days",                     KIND_ADJUSTABLE, NULL,   0.0,   1E10, 0.0001, NO, TYPE_DOUBLE,        1.0);
	phoebe_parameter_add ("phoebe_dpdt",                 "First time derivative of period (days/day)", KIND_ADJUSTABLE, NULL,  -1.0,    1.0,   1E-6, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_pshift",               "Phase shift",                                KIND_ADJUSTABLE, NULL,  -0.5,    0.5,   0.01, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_sma",                  "Semi-major axis in solar radii",             KIND_ADJUSTABLE, NULL,   0.0,   1E10,   0.01, NO, TYPE_DOUBLE,       10.0);
	phoebe_parameter_add ("phoebe_rm",                   "Mass ratio (secondary over primary)",        KIND_ADJUSTABLE, NULL,   0.0,   1E10,   0.01, NO, TYPE_DOUBLE,        1.0);
	phoebe_parameter_add ("phoebe_incl",                 "Inclination in degrees",                     KIND_ADJUSTABLE, NULL,   0.0,  180.0,   0.01, NO, TYPE_DOUBLE,       80.0);
	phoebe_parameter_add ("phoebe_vga",                  "Center-of-mass velocity in km/s",            KIND_ADJUSTABLE, NULL,  -1E3,    1E3,    1.0, NO, TYPE_DOUBLE,        0.0);

	/* ********************   Component parameters   ************************ */

	phoebe_parameter_add ("phoebe_teff1",                "Primary star effective temperature in K",    KIND_ADJUSTABLE, NULL,  3500,  50000,     10, NO, TYPE_DOUBLE,     6000.0);
	phoebe_parameter_add ("phoebe_teff2",                "Secondary star effective temperature in K",  KIND_ADJUSTABLE, NULL,  3500,  50000,     10, NO, TYPE_DOUBLE,     6000.0);
	phoebe_parameter_add ("phoebe_pot1",                 "Primary star surface potential",             KIND_ADJUSTABLE, NULL,   0.0,   1E10,   0.01, NO, TYPE_DOUBLE,       10.0);
	phoebe_parameter_add ("phoebe_pot2",                 "Secondary star surface potential",           KIND_ADJUSTABLE, NULL,   0.0,   1E10,   0.01, NO, TYPE_DOUBLE,       10.0);
	phoebe_parameter_add ("phoebe_logg1",                "Primary star surface potential",             KIND_ADJUSTABLE, NULL,   0.0,   10.0,   0.01, NO, TYPE_DOUBLE,        4.3);
	phoebe_parameter_add ("phoebe_logg2",                "Primary star surface potential",             KIND_ADJUSTABLE, NULL,   0.0,   10.0,   0.01, NO, TYPE_DOUBLE,        4.3);
	phoebe_parameter_add ("phoebe_met1",                 "Primary star metallicity",                   KIND_ADJUSTABLE, NULL, -10.0,   10.0,   0.01, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_met2",                 "Secondary star metallicity",                 KIND_ADJUSTABLE, NULL, -10.0,   10.0,   0.01, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_f1",                   "Primary star synchronicity parameter",       KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,        1.0);
	phoebe_parameter_add ("phoebe_f2",                   "Secondary star synchronicity parameter",     KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,        1.0);
	phoebe_parameter_add ("phoebe_alb1",                 "Primary star surface albedo",                KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,        0.6);
	phoebe_parameter_add ("phoebe_alb2",                 "Secondary star surface albedo",              KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,        0.6);
	phoebe_parameter_add ("phoebe_grb1",                 "Primary star gravity brightening",           KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.32);
	phoebe_parameter_add ("phoebe_grb2",                 "Primary star gravity brightening",           KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.32);

	/* **********************   Orbit parameters   ************************** */

	phoebe_parameter_add ("phoebe_ecc",                  "Orbital eccentricity",                       KIND_ADJUSTABLE, NULL,   0.0,    1.0,   0.01, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_perr0",                "Argument of periastron",                     KIND_ADJUSTABLE, NULL,   0.0, 2*M_PI,   0.01, NO, TYPE_DOUBLE,        0.0);
	phoebe_parameter_add ("phoebe_dperdt",               "First time derivative of periastron",        KIND_ADJUSTABLE, NULL,  -1.0,    1.0,   1E-6, NO, TYPE_DOUBLE,        0.0);

	/* *********************   Surface parameters   ************************* */

	phoebe_parameter_add ("phoebe_hla",                  "LC primary star flux leveler",               KIND_ADJUSTABLE, "phoebe_lcno",  0.0,   1E10,   0.01, NO, TYPE_DOUBLE_ARRAY, 12.566371);
	phoebe_parameter_add ("phoebe_cla",                  "LC secondary star flux leveler",             KIND_ADJUSTABLE, "phoebe_lcno",  0.0,   1E10,   0.01, NO, TYPE_DOUBLE_ARRAY, 12.566371);
	phoebe_parameter_add ("phoebe_opsf",                 "Third light contribution",                   KIND_ADJUSTABLE, "phoebe_lcno",  0.0,   1E10,   0.01, NO, TYPE_DOUBLE_ARRAY,  0.0);

	phoebe_parameter_add ("phoebe_passband_mode",        "Passband treatment mode",                    KIND_MENU,       NULL,  0.0,    0.0,    0.0, NO, TYPE_STRING,        "Interpolation");
	phoebe_parameter_add ("phoebe_atm1_switch",          "Use Kurucz's models for primary star",       KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);
	phoebe_parameter_add ("phoebe_atm2_switch",          "Use Kurucz's models for secondary star",     KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);
	phoebe_parameter_add ("phoebe_reffect_switch",       "Detailed reflection effect",                 KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,           NO);
	phoebe_parameter_add ("phoebe_reffect_reflections",  "Number of detailed reflections",             KIND_PARAMETER,  NULL,    2,     10,      1, NO, TYPE_INT,             2);

	phoebe_parameter_add ("phoebe_usecla_switch",        "Decouple CLAs from temperature",             KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,           NO);

	/* ********************   Extrinsic parameters   ************************ */

	phoebe_parameter_add ("phoebe_el3_units",            "Units of third light",                       KIND_MENU,       NULL,  0.0,    0.0,    0.0, NO, TYPE_STRING,        "Total light");
	phoebe_parameter_add ("phoebe_el3",                  "Third light contribution",                   KIND_ADJUSTABLE, NULL,  0.0,   1E10,   0.01, NO, TYPE_DOUBLE_ARRAY,  0.0);
	phoebe_parameter_add ("phoebe_extinction",           "Interstellar extinction coefficient",        KIND_ADJUSTABLE, NULL,  0.0,    100,   0.01, NO, TYPE_DOUBLE_ARRAY,  0.0);

	/* *********************   Fitting parameters   ************************* */

	phoebe_parameter_add ("phoebe_grid_finesize1",       "Fine grid size on primary star",             KIND_PARAMETER,  NULL,    5,     60,      1, NO, TYPE_INT,            20);
	phoebe_parameter_add ("phoebe_grid_finesize2",       "Fine grid size on secondary star",           KIND_PARAMETER,  NULL,    5,     60,      1, NO, TYPE_INT,            20);
	phoebe_parameter_add ("phoebe_grid_coarsesize1",     "Coarse grid size on primary star",           KIND_PARAMETER,  NULL,    5,     60,      1, NO, TYPE_INT,             5);
	phoebe_parameter_add ("phoebe_grid_coarsesize2",     "Coarse grid size on secondary star",         KIND_PARAMETER,  NULL,    5,     60,      1, NO, TYPE_INT,             5);

	phoebe_parameter_add ("phoebe_compute_hla_switch",   "Compute passband (HLA) levels",              KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);
	phoebe_parameter_add ("phoebe_compute_vga_switch",   "Compute gamma velocity",                     KIND_SWITCH,     NULL,  0.0,    0.0,    0.0, NO, TYPE_BOOL,          YES);

	/* **********************   DC fit parameters   ************************* */

	phoebe_parameter_add ("phoebe_dc_symder_switch",     "Should symmetrical DC derivatives be used",  KIND_SWITCH,     NULL,    0,      0,      0, NO, TYPE_BOOL,          YES);
	phoebe_parameter_add ("phoebe_dc_lambda",            "Levenberg-Marquardt multiplier for DC",      KIND_PARAMETER,  NULL,  0.0,    1.0,   1e-3, NO, TYPE_DOUBLE,       1e-3);

	phoebe_parameter_add ("phoebe_dc_spot1src",          "Adjusted spot 1 source (at which star is the spot)", KIND_PARAMETER, NULL, 1,   2, 1, NO, TYPE_INT, 1);
	phoebe_parameter_add ("phoebe_dc_spot2src",          "Adjusted spot 2 source (at which star is the spot)", KIND_PARAMETER, NULL, 1,   2, 1, NO, TYPE_INT, 2);
	phoebe_parameter_add ("phoebe_dc_spot1id",           "Adjusted spot 1 id (which spot is to be adjusted)",  KIND_PARAMETER, NULL, 1, 100, 1, NO, TYPE_INT, 1);
	phoebe_parameter_add ("phoebe_dc_spot2id",           "Adjusted spot 2 id (which spot is to be adjusted)",  KIND_PARAMETER, NULL, 1, 100, 1, NO, TYPE_INT, 1);

	/* *******************   Perturbations parameters   ********************* */

	phoebe_parameter_add ("phoebe_ld_model",             "Limb darkening model",                               KIND_MENU,      NULL,      0,      0,      0, NO, TYPE_STRING, "Logarithmic law");
	phoebe_parameter_add ("phoebe_ld_xbol1",             "Primary star bolometric LD coefficient x",           KIND_PARAMETER, NULL,    0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.5);
	phoebe_parameter_add ("phoebe_ld_ybol1",             "Secondary star bolometric LD coefficient x",         KIND_PARAMETER, NULL,    0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.5);
	phoebe_parameter_add ("phoebe_ld_xbol2",             "Primary star bolometric LD coefficient y",           KIND_PARAMETER, NULL,    0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.5);
	phoebe_parameter_add ("phoebe_ld_ybol2",             "Secondary star bolometric LD coefficient y",         KIND_PARAMETER, NULL,    0.0,    1.0,   0.01, NO, TYPE_DOUBLE,       0.5);
	phoebe_parameter_add ("phoebe_ld_lcx1",              "Primary star bandpass LD coefficient x",             KIND_ADJUSTABLE, "phoebe_lcno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_lcx2",              "Secondary star bandpass LD coefficient x",           KIND_ADJUSTABLE, "phoebe_lcno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_lcy1",              "Primary star bandpass LD coefficient y",             KIND_PARAMETER,  "phoebe_lcno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_lcy2",              "Secondary star bandpass LD coefficient y",           KIND_PARAMETER,  "phoebe_lcno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_rvx1",              "Primary RV bandpass LD coefficient x",               KIND_PARAMETER,  "phoebe_rvno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_rvx2",              "Secondary RV bandpass LD coefficient x",             KIND_PARAMETER,  "phoebe_rvno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_rvy1",              "Primary RV bandpass LD coefficient y",               KIND_PARAMETER,  "phoebe_rvno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);
	phoebe_parameter_add ("phoebe_ld_rvy2",              "Secondary RV bandpass LD coefficient y",             KIND_PARAMETER,  "phoebe_rvno",  0.0,    1.0,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.5);

	phoebe_parameter_add ("phoebe_spots_no1",            "Number of spots on primary star",                    KIND_PARAMETER,  NULL,    0,    100,      1, NO, TYPE_INT,            0);
	phoebe_parameter_add ("phoebe_spots_no2",            "Number of spots on secondary star",                  KIND_PARAMETER,  NULL,    0,    100,      1, NO, TYPE_INT,            0);
	phoebe_parameter_add ("phoebe_spots_move1",          "Spots on primary star move in longitude",            KIND_SWITCH,     NULL,    0,      0,      0, NO, TYPE_BOOL,         YES);
	phoebe_parameter_add ("phoebe_spots_move2",          "Spots on secondary star move in longitude",          KIND_SWITCH,     NULL,    0,      0,      0, NO, TYPE_BOOL,         YES);
	phoebe_parameter_add ("phoebe_spots_lat1",           "Latitude of the spot on primary star",               KIND_PARAMETER,  "phoebe_spots_no1",  0.0,   M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.0);
	phoebe_parameter_add ("phoebe_spots_long1",          "Longitude of the spot on primary star",              KIND_PARAMETER,  "phoebe_spots_no1",  0.0, 2*M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.0);
	phoebe_parameter_add ("phoebe_spots_rad1",           "Radius of the spot on primary star",                 KIND_PARAMETER,  "phoebe_spots_no1",  0.0,   M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.2);
	phoebe_parameter_add ("phoebe_spots_temp1",          "Temperature of the spot on primary star",            KIND_PARAMETER,  "phoebe_spots_no1",  0.0,    100,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.9);
	phoebe_parameter_add ("phoebe_spots_lat2",           "Latitude of the spot on secondary star",             KIND_PARAMETER,  "phoebe_spots_no2",  0.0,   M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.0);
	phoebe_parameter_add ("phoebe_spots_long2",          "Longitude of the spot on secondary star",            KIND_PARAMETER,  "phoebe_spots_no2",   0.0, 2*M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.0);
	phoebe_parameter_add ("phoebe_spots_rad2",           "Radius of the spot on secondary star",               KIND_PARAMETER,  "phoebe_spots_no2",  0.0,   M_PI,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.2);
	phoebe_parameter_add ("phoebe_spots_temp2",          "Temperature of the spot on secondary star",          KIND_PARAMETER,  "phoebe_spots_no2",  0.0,    100,   0.01, NO, TYPE_DOUBLE_ARRAY, 0.9);

	/* *********************   Utilities parameters   *********************** */

	phoebe_parameter_add ("phoebe_synscatter_switch",    "Synthetic scatter",                                  KIND_SWITCH,     NULL,    0,      0,      0, NO, TYPE_BOOL,          NO);
	phoebe_parameter_add ("phoebe_synscatter_sigma",     "Synthetic scatter standard deviation",               KIND_PARAMETER,  NULL,  0.0,  100.0,   0.01, NO, TYPE_DOUBLE,      0.01);
	phoebe_parameter_add ("phoebe_synscatter_seed",      "Synthetic scatter seed",                             KIND_PARAMETER,  NULL,  1E8,    1E9,      1, NO, TYPE_DOUBLE,     1.5E8);
	phoebe_parameter_add ("phoebe_synscatter_levweight", "Synthetic scatter weighting",                        KIND_MENU,       NULL,    0,      0,      0, NO, TYPE_STRING, "Poissonian scatter");

	return SUCCESS;
}

int phoebe_init_parameter_options ()
{
	/*
	 * This function adds options to all KIND_MENU parameters. In principle
	 * all calls to phoebe_parameter_add_option () function should be checked
	 * for return value, but since the function issues a warning in case a
	 * qualifier does not contain a menu, it is not really necessary.
	 *
	 * Return values:
	 *
	 *   SUCCESS
	 */

	int i;
	char *passband_str;

	/* Parameter: phoebe_indep                                                */
	phoebe_parameter_add_option ("phoebe_indep", "Time (HJD)");
	phoebe_parameter_add_option ("phoebe_indep", "Phase");

	/* Parameter: phoebe_model                                                */
	phoebe_parameter_add_option ("phoebe_model", "X-ray binary");
	phoebe_parameter_add_option ("phoebe_model", "Unconstrained binary system");
	phoebe_parameter_add_option ("phoebe_model", "Overcontact binary of the W UMa type");
	phoebe_parameter_add_option ("phoebe_model", "Detached binary");
	phoebe_parameter_add_option ("phoebe_model", "Overcontact binary not in thermal contact");
	phoebe_parameter_add_option ("phoebe_model", "Semi-detached binary, primary star fills Roche lobe");
	phoebe_parameter_add_option ("phoebe_model", "Semi-detached binary, secondary star fills Roche lobe");
	phoebe_parameter_add_option ("phoebe_model", "Double contact binary");

	/* Parameter: phoebe_lc_filter                                            */

	for (i = 0; i < PHOEBE_passbands_no; i++) {
		passband_str = concatenate_strings (PHOEBE_passbands[i]->set, ":", PHOEBE_passbands[i]->name, NULL);
		phoebe_parameter_add_option ("phoebe_lc_filter", passband_str);
		free (passband_str);
	}

	/* Parameter: phoebe_lc_indep                                             */
	phoebe_parameter_add_option ("phoebe_lc_indep", "Time (HJD)");
	phoebe_parameter_add_option ("phoebe_lc_indep", "Phase");

	/* Parameter: phoebe_lc_dep                                               */
	phoebe_parameter_add_option ("phoebe_lc_dep", "Magnitude");
	phoebe_parameter_add_option ("phoebe_lc_dep", "Flux");

	/* Parameter: phoebe_lc_indweight                                         */
	phoebe_parameter_add_option ("phoebe_lc_indweight", "Standard weight");
	phoebe_parameter_add_option ("phoebe_lc_indweight", "Standard deviation");
	phoebe_parameter_add_option ("phoebe_lc_indweight", "Unavailable");

	/* Parameter: phoebe_lc_levweight                                         */
	phoebe_parameter_add_option ("phoebe_lc_levweight", "No level-dependent weighting");
	phoebe_parameter_add_option ("phoebe_lc_levweight", "Poissonian scatter");
	phoebe_parameter_add_option ("phoebe_lc_levweight", "Low light scatter");

	/* Parameter: phoebe_rv_filter                                            */
	for (i = 0; i < PHOEBE_passbands_no; i++) {
		passband_str = concatenate_strings (PHOEBE_passbands[i]->set, ":", PHOEBE_passbands[i]->name, NULL);
		phoebe_parameter_add_option ("phoebe_rv_filter", passband_str);
		free (passband_str);
	}

	/* Parameter: phoebe_rv_indep                                             */
	phoebe_parameter_add_option ("phoebe_rv_indep", "Time (HJD)");
	phoebe_parameter_add_option ("phoebe_rv_indep", "Phase");

	/* Parameter: phoebe_rv_dep                                               */
	phoebe_parameter_add_option ("phoebe_rv_dep", "Primary RV");
	phoebe_parameter_add_option ("phoebe_rv_dep", "Secondary RV");

	/* Parameter: phoebe_rv_indweight                                         */
	phoebe_parameter_add_option ("phoebe_rv_indweight", "Standard weight");
	phoebe_parameter_add_option ("phoebe_rv_indweight", "Standard deviation");
	phoebe_parameter_add_option ("phoebe_rv_indweight", "Unavailable");

	/* Parameter: phoebe_ld_model                                             */
	phoebe_parameter_add_option ("phoebe_ld_model",     "Linear cosine law");
	phoebe_parameter_add_option ("phoebe_ld_model",     "Logarithmic law");
	phoebe_parameter_add_option ("phoebe_ld_model",     "Square root law");

	/* Parameter: phoebe_synscatter_levweight                                 */
	phoebe_parameter_add_option ("phoebe_synscatter_levweight", "No level-dependent weighting");
	phoebe_parameter_add_option ("phoebe_synscatter_levweight", "Poissonian scatter");
	phoebe_parameter_add_option ("phoebe_synscatter_levweight", "Low light scatter");

	/* Parameter: phoebe_passband_treatment_mode                              */
	phoebe_parameter_add_option ("phoebe_passband_mode", "None");
	phoebe_parameter_add_option ("phoebe_passband_mode", "Interpolation");
	phoebe_parameter_add_option ("phoebe_passband_mode", "Rigorous");

	/* Parameter: phoebe_el3_units                                            */
	phoebe_parameter_add_option ("phoebe_el3_units", "Total light");
	phoebe_parameter_add_option ("phoebe_el3_units", "Flux");

	return SUCCESS;
}

PHOEBE_parameter *phoebe_parameter_new ()
{
	/*
	 * This function allocates memory for the new parameter and NULLifies
	 * all field pointers for subsequent allocation.
	 */

	PHOEBE_parameter *par = phoebe_malloc (sizeof (*par));

	par->qualifier   = NULL;
	par->description = NULL;
	par->menu        = NULL;
	par->widget      = NULL;
	par->deps        = NULL;

	return par;
}

int phoebe_parameter_add (char *qualifier, char *description, PHOEBE_parameter_kind kind, char *dependency, double min, double max, double step, bool tba, ...)
{
	va_list args;
	PHOEBE_parameter *par = phoebe_parameter_new ();

	par->qualifier   = strdup (qualifier);
	par->description = strdup (description);
	par->kind        = kind;
	par->min         = min;
	par->max         = max;
	par->step        = step;
	par->tba         = tba;

	/* If this parameter has a dependency, add it to the list of children: */
	if (dependency) {
		PHOEBE_parameter *dep = phoebe_parameter_lookup (dependency);
		if (!dep)
			phoebe_lib_error ("dependency %s for %s not found, ignoring.\n", dependency, qualifier);
		else {
			PHOEBE_parameter_list *list = phoebe_malloc (sizeof (*list));
			list->elem = par;
			list->next = dep->deps;
			dep->deps = list;
		}
	}

	va_start (args, tba);
	par->type        = va_arg (args, PHOEBE_type);

	switch (par->type) {
		case TYPE_INT:
			par->value.i = va_arg (args, int);
		break;
		case TYPE_DOUBLE:
			par->value.d = va_arg (args, double);
		break;
		case TYPE_BOOL:
			par->value.b = va_arg (args, bool);
		break;
		case TYPE_STRING: {
			char *str = va_arg (args, char *);
			par->value.str = phoebe_malloc (strlen (str) + 1);
			strcpy (par->value.str, str);
		}
		break;
		case TYPE_INT_ARRAY:
			par->value.array = phoebe_array_new (TYPE_INT_ARRAY);
			par->defaultvalue.i = va_arg (args, int);
		break;
		case TYPE_DOUBLE_ARRAY:
			par->value.vec = phoebe_vector_new ();
			par->defaultvalue.d = va_arg (args, double);
		break;
		case TYPE_BOOL_ARRAY:
			par->value.array = phoebe_array_new (TYPE_BOOL_ARRAY);
			par->defaultvalue.b = va_arg (args, bool);
		break;
		case TYPE_STRING_ARRAY: {
			char *str = va_arg (args, char *);
			par->value.array = phoebe_array_new (TYPE_STRING_ARRAY);
			par->defaultvalue.str = strdup (str);
		}
		break;
	}
	va_end (args);

	if (kind != KIND_MENU) par->menu = NULL;
	else {
		par->menu = phoebe_malloc (sizeof (*(par->menu)));
		par->menu->optno  = 0;
		par->menu->option = NULL;
	}

	phoebe_parameter_commit (par);

	return SUCCESS;
}

int phoebe_parameter_add_option (char *qualifier, char *option)
{
	/*
	 * This function adds an option 'option' to the parameter menu of the
	 * passed qualifier. The qualifier's kind must be KIND_MENU, otherwise
	 * the function will abort.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   ERROR_PARAMETER_KIND_NOT_MENU
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);

	if (!par) {
		phoebe_lib_error ("qualifier %s not found, ignoring.\n", qualifier);
		return ERROR_QUALIFIER_NOT_FOUND;
	}

	if (par->kind != KIND_MENU) {
		phoebe_lib_error ("qualifier %s kind is not a menu, aborting.\n", qualifier);
		return ERROR_PARAMETER_KIND_NOT_MENU;
	}

	par->menu->optno++;
	par->menu->option = phoebe_realloc (par->menu->option, par->menu->optno * sizeof (*(par->menu->option)));
	par->menu->option[par->menu->optno-1] = strdup (option);

	phoebe_debug ("option \"%s\" added to parameter %s.\n", option, qualifier);
	return SUCCESS;
}

unsigned int phoebe_parameter_hash (char *qualifier)
{
	/*
	 * This is the hashing function for storing parameters into the parameter
	 * table.
	 */

	unsigned int h = 0;
	unsigned char *p;

	for (p = (unsigned char *) qualifier; *p != '\0'; p++)
		h = PHOEBE_PT_HASH_MULTIPLIER * h + *p;

	return h % PHOEBE_PT_HASH_BUCKETS;
}

int phoebe_parameter_commit (PHOEBE_parameter *par)
{
	int hash = phoebe_parameter_hash (par->qualifier);
	PHOEBE_pt_bucket *bucket_element = PHOEBE_pt[hash];

	while (bucket_element) {
		if (strcmp (bucket_element->par->qualifier, par->qualifier) == 0) break;
		bucket_element = bucket_element->next;
	}

	if (bucket_element) {
		phoebe_lib_error ("parameter %s already declared, ignoring.\n", par->qualifier);
		return SUCCESS;
	}
	else {
		bucket_element = phoebe_malloc (sizeof (*bucket_element));

		bucket_element->par  = par;
		bucket_element->next = PHOEBE_pt[hash];
		PHOEBE_pt[hash] = bucket_element;
	}

	phoebe_debug ("parameter %s added to bucket %d.\n", par->qualifier, hash);
	return SUCCESS;
}

PHOEBE_parameter *phoebe_parameter_lookup (char *qualifier)
{
	unsigned int hash = phoebe_parameter_hash (qualifier);
	PHOEBE_pt_bucket *bucket_element = PHOEBE_pt[hash];

	while (bucket_element) {
		if (strcmp (bucket_element->par->qualifier, qualifier) == 0) break;
		bucket_element = bucket_element->next;
	}

	return bucket_element->par;
}

int phoebe_parameter_free (PHOEBE_parameter *par)
{
	/*
	 * This function frees all memory allocated for the passed parameter 'par'.
	 */

	if (par->qualifier)   free (par->qualifier);
	if (par->description) free (par->description);
	if (par->menu)        free (par->menu);
	if (par->widget)      free (par->widget);

	while (par->deps) {
		PHOEBE_parameter_list *list = par->deps->next;
		free (par->deps);
		par->deps = list;
	}

	free (par);
}

int phoebe_parameter_update_deps (PHOEBE_parameter *par, int oldval)
{
	/*
	 * This function is called whenever the dimension of parameter arrays must
	 * be changed. Typically this happens when the number of observed data cur-
	 * ves is changed, the number of spots is changed etc.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter_list *list = par->deps;

	int dim = par->value.i;
	int status, j;

	/* If the dimension is the same, there's nothing to be done. */
	if (oldval == dim) return SUCCESS;

	while (list) {
		phoebe_debug ("resizing array %s to dim %d\n", list->elem->qualifier, dim);

		switch (list->elem->type) {
				case TYPE_INT_ARRAY:
					phoebe_array_realloc (list->elem->value.array, dim);
					for (j = oldval; j < dim; j++)
						list->elem->value.array->val.iarray[j] = list->elem->defaultvalue.i;
				break;
				case TYPE_BOOL_ARRAY:
					status = phoebe_array_realloc (list->elem->value.array, dim);
					if (status != SUCCESS) phoebe_lib_error ("%s", phoebe_error (status));
					for (j = oldval; j < dim; j++)
						list->elem->value.array->val.barray[j] = list->elem->defaultvalue.b;
				break;
				case TYPE_DOUBLE_ARRAY:
					phoebe_vector_realloc (list->elem->value.vec, dim);
					for (j = oldval; j < dim; j++)
						list->elem->value.vec->val[j] = list->elem->defaultvalue.d;
				break;
				case TYPE_STRING_ARRAY:
					phoebe_array_realloc (list->elem->value.array, dim);
					for (j = oldval; j < dim; j++)
						list->elem->value.array->val.strarray[j] = strdup (list->elem->defaultvalue.str);
				break;
		}

		list = list->next;
	}

	return SUCCESS;
}

bool phoebe_parameter_menu_option_is_valid (char *qualifier, char *option)
{
	/*
	 * This function is a boolean test for parameter menu options. It returns
	 * TRUE if the option is valid and FALSE if it is invalid.
	 */

	int i;

	/* Is the qualifier valid: */
	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return FALSE;

	/* Is the qualified parameter a menu: */
	if (par->kind != KIND_MENU) return FALSE;

	/* Is the option one of the menu options: */
	for (i = 0; i < par->menu->optno; i++)
		if (strcmp (par->menu->option[i], option) == 0)
			return TRUE;

	return FALSE;
}

int phoebe_get_parameter_value (char *qualifier, ...)
{
	/*
	 * This is the public function for providing the values of parameters
	 * identified by the qualifiers. It is the only function that should be
	 * used for this purpose, all other functions are internal and should
	 * not be used.
	 *
	 * Synopsis:
	 *
	 *   phoebe_get_parameter_value (qualifier, [index, ], &value)
	 *
	 * Return values:
	 *
	 *   ERROR_INDEX_OUT_OF_RANGE
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	int index = 0;
	va_list args;

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	va_start (args, qualifier);

	switch (par->type) {
		case TYPE_INT: {
			int *value = va_arg (args, int *);
			*value = par->value.i;
		}
		break;
		case TYPE_BOOL: {
			bool *value = va_arg (args, bool *);
			*value = par->value.b;
		}
		break;
		case TYPE_DOUBLE: {
			double *value = va_arg (args, double *);
			*value = par->value.d;
		}
		break;
		case TYPE_STRING: {
			const char **value = va_arg (args, const char **);
			*value = par->value.str;
		}
		break;
		case TYPE_INT_ARRAY:
			index = va_arg (args, int);
			{
			int *value = va_arg (args, int *);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			*value = par->value.array->val.iarray[index];
			}
		break;
		case TYPE_BOOL_ARRAY:
			index = va_arg (args, int);
			{
			bool *value = va_arg (args, bool *);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			*value = par->value.array->val.barray[index];
			}
		break;
		case TYPE_DOUBLE_ARRAY:
			index = va_arg (args, int);
			{
			double *value = va_arg (args, double *);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			*value = par->value.vec->val[index];
			}
		break;
		case TYPE_STRING_ARRAY:
			index = va_arg (args, int);
			{
			const char **value = va_arg (args, const char **);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			*value = par->value.array->val.strarray[index];
			}
		break;
		default:
			phoebe_lib_error ("exception handler invoked in phoebe_get_parameter_value (), please report this!\n");
			return ERROR_EXCEPTION_HANDLER_INVOKED;
	}
	va_end (args);

	return SUCCESS;
}

int phoebe_set_parameter_value (char *qualifier, ...)
{
	/*
	 * This is the public function for changing qualifier values. It is the
	 * only function that should be used for this purpose, all other functions
	 * should be regarded as internal and should not be used.
	 *
	 * Synopsis:
	 *
	 *   phoebe_set_parameter_value (qualifier, [curve, ] value)
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   ERROR_INDEX_OUT_OF_RANGE
	 *   SUCCESS
	 */

	int index = 0;
	va_list args;

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	va_start (args, qualifier);

	switch (par->type) {
		case TYPE_INT: {
			int value = va_arg (args, int);
			if (par->kind == KIND_MODIFIER) {
				int oldval = par->value.i;
				par->value.i = value;
				phoebe_parameter_update_deps (par, oldval);
			}
			else
				par->value.i = value;
		}
		break;
		case TYPE_BOOL: {
			bool value = va_arg (args, bool);
			par->value.b = value;
		}
		break;
		case TYPE_DOUBLE: {
			double value = va_arg (args, double);
			par->value.d = value;
		}
		break;
		case TYPE_STRING: {
			char *value = va_arg (args, char *);
			free (par->value.str);
			par->value.str = phoebe_malloc (strlen (value) + 1);
			strcpy (par->value.str, value);

			/*
			 * If the qualified parameter is a menu, let's check if the option
			 * is valid. If it is not, just warn about it, but set its value
			 * anyway.
			 */

			if (par->kind == KIND_MENU && !phoebe_parameter_menu_option_is_valid (qualifier, (char *) value))
				phoebe_lib_warning ("option \"%s\" is not a valid menu option.\n", value);
		}
		break;
		case TYPE_INT_ARRAY:
			index = va_arg (args, int);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			{
			int value = va_arg (args, int);
			par->value.array->val.iarray[index] = value;
			}
		break;
		case TYPE_BOOL_ARRAY:
			index = va_arg (args, int);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			{
			bool value = va_arg (args, bool);
			par->value.array->val.barray[index] = value;
			}
		break;
		case TYPE_DOUBLE_ARRAY:
			index = va_arg (args, int);
			if (index < 0 || index > par->value.vec->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			{
			double value = va_arg (args, double);
			par->value.vec->val[index] = value;
			}
		break;
		case TYPE_STRING_ARRAY:
			index = va_arg (args, int);
			if (index < 0 || index > par->value.array->dim-1)
				return ERROR_INDEX_OUT_OF_RANGE;
			{
			char *value = va_arg (args, char *);
			free (par->value.array->val.strarray[index]);
			par->value.array->val.strarray[index] = phoebe_malloc (strlen (value) + 1);
			strcpy (par->value.array->val.strarray[index], value);

			/*
			 * If the qualified parameter is a menu, let's check if the option is
			 * valid. If it is not, just warn about it, but set its value anyway.
			 */

			if (par->kind == KIND_MENU && !phoebe_parameter_menu_option_is_valid (qualifier, (char *) value))
				phoebe_lib_warning ("option \"%s\" is not a valid menu option.\n", value);
			}
		break;
	}
	va_end (args);

	return SUCCESS;
}

int phoebe_get_parameter_tba (char *qualifier, bool *tba)
{
	/*
	 * This is a public function for reading out qualifier's TBA (To Be Adjusted
	 * bit.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	*tba = par->tba;
	return SUCCESS;
}

int phoebe_set_parameter_tba (char *qualifier, bool tba)
{
	/*
	 * This is the public function for changing qualifier's TBA (To Be Adjusted
	 * bit.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	par->tba = tba;

	return SUCCESS;
}

int phoebe_get_parameter_step (char *qualifier, double *step)
{
	/*
	 * This is a public function for reading out qualifier's step size used
	 * for minimization.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	*step = par->step;
	return SUCCESS;
}

int phoebe_set_parameter_step (char *qualifier, double step)
{
	/*
	 * This is the public function for changing qualifier step.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	par->step = step;

	return SUCCESS;
}

int phoebe_get_parameter_lower_limit (char *qualifier, double *valmin)
{
	/*
	 * This is the public function for reading out the lower parameter limit.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	*valmin = par->min;
	return SUCCESS;
}

int phoebe_set_parameter_lower_limit (char *qualifier, double valmin)
{
	/*
	 * This is the public function for changing the lower parameter limit.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	par->min = valmin;
	return SUCCESS;
}

int phoebe_get_parameter_upper_limit (char *qualifier, double *valmax)
{
	/*
	 * This is the public function for reading out the upper parameter limit.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	*valmax = par->max;
	return SUCCESS;
}

int phoebe_set_parameter_upper_limit (char *qualifier, double valmax)
{
	/*
	 * This is the public function for changing the upper parameter limit.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	par->max = valmax;
	return SUCCESS;
}

int phoebe_get_parameter_limits (char *qualifier, double *valmin, double *valmax)
{
	/*
	 * This is the public function for reading out qualifier limits.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	*valmin = par->min;
	*valmax = par->max;

	return SUCCESS;
}

int phoebe_set_parameter_limits (char *qualifier, double valmin, double valmax)
{
	/*
	 * This is the public function for changing qualifier limits.
	 *
	 * Error codes:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	par->min = valmin;
	par->max = valmax;

	return SUCCESS;
}

int phoebe_el3_units_id (PHOEBE_el3_units *el3_units)
{
	/*
	 * This function assigns the third light units id to el3_units variable.
	 * If an error occurs, -1 is assigned and ERROR_INVALID_EL3_UNITS code is
	 * returned.
	 */

	const char *el3str;
	phoebe_get_parameter_value ("phoebe_el3_units", &el3str);

	*el3_units = PHOEBE_EL3_UNITS_INVALID_ENTRY;
	if (strcmp (el3str, "Total light") == 0) *el3_units = PHOEBE_EL3_UNITS_TOTAL_LIGHT;
	if (strcmp (el3str,        "Flux") == 0) *el3_units = PHOEBE_EL3_UNITS_FLUX;
	if (*el3_units == PHOEBE_EL3_UNITS_INVALID_ENTRY)
		return ERROR_INVALID_EL3_UNITS;

	return SUCCESS;
}

int intern_get_from_keyword_file (char *qualifier, char *value_str)
{
	/*
	 * This is an internal function that looks up a qualifier in the global
	 * parameter table and sets its value to the passed string representation
	 * of that value, value_str.
	 *
	 * Return values:
	 *
	 *   ERROR_QUALIFIER_NOT_FOUND
	 *   SUCCESS
	 */

	int status;

	PHOEBE_parameter *par = phoebe_parameter_lookup (qualifier);
	if (!par) return ERROR_QUALIFIER_NOT_FOUND;

	switch (par->type) {
		case TYPE_INT:
			status = phoebe_set_parameter_value (qualifier, atoi (value_str));
		break;
		case TYPE_BOOL:
			status = phoebe_set_parameter_value (qualifier, atoi (value_str));
		break;
		case TYPE_DOUBLE:
			status = phoebe_set_parameter_value (qualifier, atof (value_str));
		break;
		case TYPE_STRING:
			/* Strip the string of quotes if necessary:                             */
			while (value_str[0] == '"') value_str++;
			while (value_str[strlen(value_str)-1] == '"') value_str[strlen(value_str)-1] = '\0';
			status = phoebe_set_parameter_value (qualifier, value_str);
		break;
		default:
			phoebe_lib_error ("exception handler invoked in intern_get_from_parameter_file (), please report this!\n");
			return ERROR_EXCEPTION_HANDLER_INVOKED;
	}

	if (status != SUCCESS)
		phoebe_lib_error ("%s", phoebe_error (status));

	return status;
}

int phoebe_open_parameter_file (const char *filename)
{
	/*
	 * This function opens PHOEBE 0.3x parameter files. The return value is:
	 *
	 *   ERROR_FILE_IS_EMPTY
	 *   ERROR_INVALID_HEADER
	 *   ERROR_FILE_NOT_REGULAR
	 *   ERROR_FILE_NO_PERMISSIONS
	 *   ERROR_FILE_NOT_FOUND
	 *   SUCCESS
	 */

	char readout_string[255];
	char *readout_str = readout_string;

	char keyword_string[255];
	char *keyword_str = keyword_string;

	char *value_str;

	int i, status;
	int lineno = 0;

	FILE *keyword_file;

	phoebe_debug ("entering function 'open_parameter_file ()'\n");

	/* First a checkup if everything is OK with the filename:                   */
	if (!filename_exists ((char *) filename))               return ERROR_FILE_NOT_FOUND;
	if (!filename_is_regular_file ((char *) filename))      return ERROR_FILE_NOT_REGULAR;
	if (!filename_has_read_permissions ((char *) filename)) return ERROR_FILE_NO_PERMISSIONS;

	keyword_file = fopen (filename, "r");

	/* Parameter files start with a commented header line; see if it's there: */

	fgets (readout_str, 255, keyword_file); lineno++;
	if (feof (keyword_file)) return ERROR_FILE_IS_EMPTY;
	readout_str[strlen(readout_str)-1] = '\0';

	if (strstr (readout_str, "PHOEBE") != 0) {
		/* Yep, it's there! Read out version number and if it's a legacy file,    */
		/* call a suitable function for opening legacy keyword files.             */

		char *version_str = strstr (readout_str, "PHOEBE");
		double version;
			
		if (sscanf (version_str, "PHOEBE %lf", &version) != 1) {
			/* Just in case, if the header line is invalid.                       */
			phoebe_lib_error ("Invalid header line in %s, aborting.\n", filename);
			return ERROR_INVALID_HEADER;
		}
		if (version < 0.30) {
			fclose (keyword_file);
			phoebe_debug ("opening legacy parameter file.\n");
			status = phoebe_open_legacy_parameter_file (filename);
			return status;
		}
		phoebe_debug ("PHOEBE parameter file version: %2.2lf\n", version);
	}

	while (!feof (keyword_file)) {
		fgets (readout_str, 255, keyword_file); lineno++;
		if (feof (keyword_file)) break;

		/*
		 * Clear the read string of leading and trailing spaces, tabs, newlines,
		 * comments and empty lines:
		 */

		readout_str[strlen(readout_str)-1] = '\0';
		if (strchr (readout_str, '#') != 0) (strchr (readout_str, '#'))[0] = '\0';
		while (readout_str[0] == ' ' || readout_str[0] == '\t') readout_str++;
		while (readout_str[strlen(readout_str)-1] == ' ' || readout_str[strlen(readout_str)-1] == '\t') readout_str[strlen(readout_str)-1] = '\0';
		if (strlen (readout_str) == 0) continue;

		/*
		 * Read out the qualifier only. We can't read out the value here
		 * because it may be a string containing spaces and that would result
		 * in an incomplete readout.
		 */

		if (sscanf (readout_str, "%s = %*s", keyword_str) != 1) {
			phoebe_lib_error ("line %d of the parameter file is invalid, skipping.\n", lineno);
			continue;
		}

		value_str = strchr (readout_str, '=');
		if (value_str == NULL) {
			/* If the keyword doesn't have '=', it will be skipped.                 */
			phoebe_lib_error ("qualifier %s initialization (line %d) is invalid.\n", keyword_str, lineno);
			continue;
		}

		/* value_str now points to '=', we need the next character:               */
		value_str++;

		/* Eat all empty spaces and quotes at the beginning and at the end: */
		while (value_str[0] == ' ' || value_str[0] == '\t' || value_str[0] == '"') value_str++;
		while (value_str[strlen (value_str)-1] == ' ' || value_str[strlen (value_str)-1] == '\t' || value_str[strlen (value_str)-1] == '"') value_str[strlen(value_str)-1] = '\0';

		/*
		 * What remains is a qualifier, the element index [i] in case of
		 * wavelength-dependent parameters, and fields in case of an adjustable
		 * parameter. Let's parse it. The following looks really really scary,
		 * but it's actually quite straight-forward and, more importantly,
		 * tested thoroughly. :)
		 */

		{
		PHOEBE_parameter *par;
		char *qualifier = NULL;
		char *field     = NULL;
		char *field_sep = NULL;
		char *elem_sep  = NULL;
		int   elem;
		int   olddim;

		phoebe_debug ("keyword_str: %30s %2d ", keyword_str, strlen(keyword_str));

		if ( (elem_sep = strchr (keyword_str, '[')) && (field_sep = strchr (keyword_str, '.')) ) {
			phoebe_debug ("%2d %2d ", strlen (elem_sep), strlen (field_sep));
			qualifier = phoebe_malloc ( (strlen(keyword_str)-strlen(elem_sep)+1)*sizeof(*qualifier) );
			strncpy (qualifier, keyword_str, strlen(keyword_str)-strlen(elem_sep));
			qualifier[strlen(keyword_str)-strlen(elem_sep)] = '\0';
			sscanf (elem_sep, "[%d]", &elem);
			field = phoebe_malloc (strlen(field_sep)*sizeof(*field));
			strcpy (field, field_sep+1);
			field[strlen(field_sep)-1] = '\0';
			phoebe_debug ("%30s %2d %6s", qualifier, elem, field);

			par = phoebe_parameter_lookup (qualifier);

			/*
			 * Here's a trick: qualifiers in the parameter file may be shuffled
			 * and array elements may appear before their dependencies are
			 * declared. Since we want fool-proof capability, we test the
			 * dependencies here and allocate space if necessary.
			 */

			switch (par->type) {
				case TYPE_INT_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.iarray[i] = par->defaultvalue.i;
					}
					if (strcmp (field,  "VAL") == 0) par->value.array->val.iarray[elem-1] = atoi (value_str);
					if (strcmp (field,  "MIN") == 0) par->min     = atof (value_str);
					if (strcmp (field,  "MAX") == 0) par->max     = atof (value_str);
					if (strcmp (field, "STEP") == 0) par->step    = atof (value_str);
					if (strcmp (field,  "ADJ") == 0) par->tba     = atob (value_str);
				break;
				case TYPE_BOOL_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.barray[i] = par->defaultvalue.b;
					}
					if (strcmp (field,  "VAL") == 0) par->value.array->val.barray[elem-1] = atob (value_str);
					if (strcmp (field,  "MIN") == 0) par->min     = atof (value_str);
					if (strcmp (field,  "MAX") == 0) par->max     = atof (value_str);
					if (strcmp (field, "STEP") == 0) par->step    = atof (value_str);
					if (strcmp (field,  "ADJ") == 0) par->tba     = atob (value_str);
				break;
				case TYPE_DOUBLE_ARRAY:
					if (par->value.vec->dim < elem) {
						olddim = par->value.vec->dim;
						phoebe_vector_realloc (par->value.vec, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.vec->val[i] = par->defaultvalue.d;
					}
					if (strcmp (field,  "VAL") == 0) par->value.vec->val[elem-1] = atof (value_str);
					if (strcmp (field,  "MIN") == 0) par->min     = atof (value_str);
					if (strcmp (field,  "MAX") == 0) par->max     = atof (value_str);
					if (strcmp (field, "STEP") == 0) par->step    = atof (value_str);
					if (strcmp (field,  "ADJ") == 0) par->tba     = atob (value_str);
				break;
				case TYPE_STRING_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.strarray[i] = strdup (par->defaultvalue.str);
					}
					if (strcmp (field,  "VAL") == 0) {
						if (par->value.array->val.strarray[elem-1]) free (par->value.array->val.strarray[elem-1]);
						par->value.array->val.strarray[elem-1] = strdup (value_str);
					}
					if (strcmp (field,  "MIN") == 0) par->min     = atof (value_str);
					if (strcmp (field,  "MAX") == 0) par->max     = atof (value_str);
					if (strcmp (field, "STEP") == 0) par->step    = atof (value_str);
					if (strcmp (field,  "ADJ") == 0) par->tba     = atob (value_str);
				break;
			}
		}
		else if (elem_sep = strchr (keyword_str, '[')) {
			phoebe_debug ("%2d    ", strlen (elem_sep));
			qualifier = phoebe_malloc ( (strlen(keyword_str)-strlen(elem_sep)+1)*sizeof(*qualifier) );
			strncpy (qualifier, keyword_str, strlen(keyword_str)-strlen(elem_sep));
			qualifier[strlen(keyword_str)-strlen(elem_sep)] = '\0';
			sscanf (elem_sep, "[%d]", &elem);
			phoebe_debug ("%30s %2d", qualifier, elem);

			par = phoebe_parameter_lookup (qualifier);

			/*
			 * Here's a trick: qualifiers in the parameter file may be shuffled
			 * and array elements may appear before their dependencies are
			 * declared. Since we want fool-proof capability, we test the
			 * dependencies here and allocate space if necessary.
			 */

			switch (par->type) {
				case TYPE_INT_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.iarray[i] = par->defaultvalue.i;
					}
					par->value.array->val.iarray[elem-1] = atoi (value_str);
				break;
				case TYPE_BOOL_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.barray[i] = par->defaultvalue.b;
					}
					par->value.array->val.barray[elem-1] = atob (value_str);
				break;
				case TYPE_DOUBLE_ARRAY:
					if (par->value.vec->dim < elem) {
						olddim = par->value.vec->dim;
						phoebe_vector_realloc (par->value.vec, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.vec->val[i] = par->defaultvalue.d;
					}
					par->value.vec->val[elem-1] = atof (value_str);
				break;
				case TYPE_STRING_ARRAY:
					if (par->value.array->dim < elem) {
						olddim = par->value.array->dim;
						phoebe_array_realloc (par->value.array, elem);
						for (i = olddim; i < elem-1; i++)
							par->value.array->val.strarray[i] = strdup (par->defaultvalue.str);
					}
					if (par->value.array->val.strarray[elem-1]) free (par->value.array->val.strarray[elem-1]);
					par->value.array->val.strarray[elem-1] = strdup (value_str);
				break;
			}
		}
		else if (field_sep = strchr (keyword_str, '.')) {
			phoebe_debug ("   %2d ", strlen (field_sep));
			qualifier = phoebe_malloc ( (strlen(keyword_str)-strlen(field_sep)+1)*sizeof(*qualifier) );
			strncpy (qualifier, keyword_str, strlen(keyword_str)-strlen(field_sep));
			qualifier[strlen(keyword_str)-strlen(field_sep)] = '\0';
			field = phoebe_malloc (strlen(field_sep)*sizeof(*field));
			strcpy (field, field_sep+1);
			field[strlen(field_sep)-1] = '\0';
			phoebe_debug ("%30s    %6s", qualifier, field);

			par = phoebe_parameter_lookup (qualifier);

			if (strcmp (field,  "VAL") == 0) par->value.d = atof (value_str);
			if (strcmp (field,  "MIN") == 0) par->min     = atof (value_str);
			if (strcmp (field,  "MAX") == 0) par->max     = atof (value_str);
			if (strcmp (field, "STEP") == 0) par->step    = atof (value_str);
			if (strcmp (field,  "ADJ") == 0) par->tba     = atob (value_str);
		}
		else {
			phoebe_debug ("      ");
			qualifier = phoebe_malloc ((strlen(keyword_str)+1)*sizeof(*qualifier));
			strcpy (qualifier, keyword_str);
			phoebe_debug ("%30s   ", qualifier);
			intern_get_from_keyword_file (qualifier, value_str);
		}
		phoebe_debug ("\n");

		if (qualifier) { free (qualifier); qualifier = NULL; }
		if (field)     { free (field);     field     = NULL; }
		}
	}

	fclose (keyword_file);

	phoebe_debug ("leaving function 'open_parameter_file ()'\n");

	return SUCCESS;
}

int intern_save_to_parameter_file (PHOEBE_parameter *par, FILE *file)
{
	/*
	 * This function saves the contents of parameter 'par' to file 'file'.
	 */

	int j;

	if (par->kind == KIND_ADJUSTABLE) {
		switch (par->type) {
			case TYPE_DOUBLE:
				fprintf (file, "%s.VAL  = %lf\n", par->qualifier, par->value.d);
			break;
			case TYPE_DOUBLE_ARRAY:
				if (par->value.vec)
					for (j = 0; j < par->value.vec->dim; j++)
						fprintf (file, "%s[%d].VAL  = %lf\n", par->qualifier, j+1, par->value.vec->val[j]);
			break;
			default:
				phoebe_lib_error ("exception handler invoked in intern_save_to_parameter_file (), please report this!\n");
		}

		fprintf (file, "%s.ADJ  = %d\n",  par->qualifier, par->tba);
		fprintf (file, "%s.STEP = %lf\n", par->qualifier, par->step);
		fprintf (file, "%s.MIN  = %lf\n", par->qualifier, par->min);
		fprintf (file, "%s.MAX  = %lf\n", par->qualifier, par->max);
	}
	else {
		switch (par->type) {
			case TYPE_INT:
				fprintf (file, "%s = %d\n", par->qualifier, par->value.i);
			break;
			case TYPE_BOOL:
				fprintf (file, "%s = %d\n", par->qualifier, par->value.b);
			break;
			case TYPE_DOUBLE:
				fprintf (file, "%s = %lf\n", par->qualifier, par->value.d);
			break;
			case TYPE_STRING:
				fprintf (file, "%s = \"%s\"\n", par->qualifier, par->value.str);
			break;
			case TYPE_INT_ARRAY:
				if (par->value.array)
					for (j = 0; j < par->value.array->dim; j++)
						fprintf (file, "%s[%d] = %d\n", par->qualifier, j+1, par->value.array->val.iarray[j]);
			break;
			case TYPE_BOOL_ARRAY:
				if (par->value.array)
					for (j = 0; j < par->value.array->dim; j++)
						fprintf (file, "%s[%d] = %d\n", par->qualifier, j+1, par->value.array->val.barray[j]);
			break;
			case TYPE_DOUBLE_ARRAY:
				if (par->value.vec)
					for (j = 0; j < par->value.vec->dim; j++)
						fprintf (file, "%s[%d] = %lf\n", par->qualifier, j+1, par->value.vec->val[j]);
			break;
			case TYPE_STRING_ARRAY:
				if (par->value.array)
					for (j = 0; j < par->value.array->dim; j++)
						fprintf (file, "%s[%d] = \"%s\"\n", par->qualifier, j+1, par->value.array->val.strarray[j]);
			break;
			default:
				phoebe_lib_error ("exception handler invoked in intern_save_to_parameter_file (), please report this!\n");
			break;
			}
		}

	return SUCCESS;
}

int phoebe_save_parameter_file (const char *filename)
{
	/*
	 * This function saves PHOEBE 0.3x keyword files.
	 *
	 * Return values:
	 *
	 *   ERROR_FILE_IS_INVALID
	 *   SUCCESS
	 */

	int i;
	FILE *parameter_file;
	PHOEBE_pt_bucket *elem;

	/* First a checkup if everything is OK with the filename: */
	parameter_file = fopen (filename, "w");
	if (!parameter_file) return ERROR_FILE_IS_INVALID;

	/* Write a version header: */
	fprintf (parameter_file, "# Parameter file conforming to %s\n", PHOEBE_VERSION_NUMBER);

	/* Traverse the parameter table and save parameters one by one: */
	for (i = 0; i < PHOEBE_PT_HASH_BUCKETS; i++) {
		elem = PHOEBE_pt[i];
		while (elem) {
			intern_save_to_parameter_file (elem->par, parameter_file);
			elem = elem->next;
		}
	}

	fclose (parameter_file);
	return SUCCESS;
}

int phoebe_open_legacy_parameter_file (const char *filename)
{
	phoebe_lib_error ("Not yet implemented!\n");
	return SUCCESS;
}
