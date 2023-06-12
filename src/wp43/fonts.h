// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 Authors

/**
 * \file fonts.h
 ***********************************************/
#if !defined(FONTS_H)
#define FONTS_H

#include "typeDefinitions.h"
#include <stdint.h>

  /**
   * Finds a glyph in a font
   *
   * \param[in] font const Font
   * \param[in] charCode Unicode code point
   * \return
   *   * Glyph index
   *   * -1 when not found in the standard font
   *   * -2 when not found in the numeric font
   */
  int16_t findGlyph            (const font_t *font, uint16_t charCode);

  /**
   * Generates a glyph for a non found glyph containing the hexadecimal Unicoide code point
   *
   * \param[in] font Font for which generate the glyph
   * \param[in] charCode Unicode code point
   */
  void    generateNotFoundGlyph(const int16_t font, uint16_t charCode);

  //<------------ Numeric font glyphs -----------> Not used for now
  #define NUM_0_b                       "\xa4\x73"
  #define NUM_1_b                       "\xa4\x74"
  #define NUM_2_b                       "\xa4\x75"
  #define NUM_3_b                       "\xa4\x76"
  #define NUM_4_b                       "\xa4\x77"
  #define NUM_5_b                       "\xa4\x78"
  #define NUM_6_b                       "\xa4\x79"
  #define NUM_7_b                       "\xa4\x7a"
  #define NUM_8_b                       "\xa4\x7b"
  #define NUM_9_b                       "\xa4\x7c"

//<----------- Standard font glyphs ----------->
#define STD_SPACE                     "\x20"
#define STD_EXCLAMATION_MARK          "\x21"
#define STD_DOUBLE_QUOTE              "\x22"
#define STD_NUMBER_SIGN               "\x23"
#define STD_DOLLAR                    "\x24"
#define STD_PERCENT                   "\x25"
#define STD_AMPERSAND                 "\x26"
#define STD_QUOTE                     "\x27"
#define STD_LEFT_PARENTHESIS          "\x28"
#define STD_RIGHT_PARENTHESIS         "\x29"
#define STD_ASTERISK                  "\x2a"
#define STD_PLUS                      "\x2b"
#define STD_COMMA                     "\x2c"
#define STD_MINUS                     "\x2d"
#define STD_PERIOD                    "\x2e"
#define STD_SLASH                     "\x2f"
#define STD_0                         "\x30"
#define STD_1                         "\x31"
#define STD_2                         "\x32"
#define STD_3                         "\x33"
#define STD_4                         "\x34"
#define STD_5                         "\x35"
#define STD_6                         "\x36"
#define STD_7                         "\x37"
#define STD_8                         "\x38"
#define STD_9                         "\x39"
#define STD_COLON                     "\x3a"
#define STD_SEMICOLON                 "\x3b"
#define STD_LESS_THAN                 "\x3c"
#define STD_EQUAL                     "\x3d"
#define STD_GREATER_THAN              "\x3e"
#define STD_QUESTION_MARK             "\x3f"
#define STD_AT                        "\x40"
#define STD_A                         "\x41"
#define STD_B                         "\x42"
#define STD_C                         "\x43"
#define STD_D                         "\x44"
#define STD_E                         "\x45"
#define STD_F                         "\x46"
#define STD_G                         "\x47"
#define STD_H                         "\x48"
#define STD_I                         "\x49"
#define STD_J                         "\x4a"
#define STD_K                         "\x4b"
#define STD_L                         "\x4c"
#define STD_M                         "\x4d"
#define STD_N                         "\x4e"
#define STD_O                         "\x4f"
#define STD_P                         "\x50"
#define STD_Q                         "\x51"
#define STD_R                         "\x52"
#define STD_S                         "\x53"
#define STD_T                         "\x54"
#define STD_U                         "\x55"
#define STD_V                         "\x56"
#define STD_W                         "\x57"
#define STD_X                         "\x58"
#define STD_Y                         "\x59"
#define STD_Z                         "\x5a"
#define STD_LEFT_SQUARE_BRACKET       "\x5b"
#define STD_BACK_SLASH                "\x5c"
#define STD_RIGHT_SQUARE_BRACKET      "\x5d"
#define STD_CIRCUMFLEX                "\x5e"
#define STD_UNDERSCORE                "\x5f"
#define STD_a                         "\x61"
#define STD_b                         "\x62"
#define STD_c                         "\x63"
#define STD_d                         "\x64"
#define STD_e                         "\x65"
#define STD_f                         "\x66"
#define STD_g                         "\x67"
#define STD_h                         "\x68"
#define STD_i                         "\x69"
#define STD_j                         "\x6a"
#define STD_k                         "\x6b"
#define STD_l                         "\x6c"
#define STD_m                         "\x6d"
#define STD_n                         "\x6e"
#define STD_o                         "\x6f"
#define STD_p                         "\x70"
#define STD_q                         "\x71"
#define STD_r                         "\x72"
#define STD_s                         "\x73"
#define STD_t                         "\x74"
#define STD_u                         "\x75"
#define STD_v                         "\x76"
#define STD_w                         "\x77"
#define STD_x                         "\x78"
#define STD_y                         "\x79"
#define STD_z                         "\x7a"
#define STD_LEFT_CURLY_BRACKET        "\x7b"
#define STD_PIPE                      "\x7c"
#define STD_RIGHT_CURLY_BRACKET       "\x7d"
#define STD_TILDE                     "\x7e"
#define STD_INVERTED_EXCLAMATION_MARK "\x80\xa1"
#define STD_CENT                      "\x80\xa2"
#define STD_POUND                     "\x80\xa3"
#define STD_YEN                       "\x80\xa5"
#define STD_SECTION                   "\x80\xa7"
#define STD_OVERFLOW_CARRY            "\x80\xa9"
#define STD_LEFT_DOUBLE_ANGLE         "\x80\xab"
#define STD_NOT                       "\x80\xac"
#define STD_DEGREE                    "\x80\xb0"
#define STD_PLUS_MINUS                "\x80\xb1"
#define STD_mu_b                      "\x80\xb5"
#define STD_DOT                       "\x80\xb7"
#define STD_RIGHT_DOUBLE_ANGLE        "\x80\xbb"
#define STD_ONE_QUARTER               "\x80\xbc"
#define STD_ONE_HALF                  "\x80\xbd"
#define STD_INVERTED_QUESTION_MARK    "\x80\xbf"
#define STD_A_GRAVE                   "\x80\xc0"
#define STD_A_ACUTE                   "\x80\xc1"
#define STD_A_CIRC                    "\x80\xc2"
#define STD_A_TILDE                   "\x80\xc3"
#define STD_A_DIARESIS                "\x80\xc4"
#define STD_A_RING                    "\x80\xc5"
#define STD_AE                        "\x80\xc6"
#define STD_C_CEDILLA                 "\x80\xc7"
#define STD_E_GRAVE                   "\x80\xc8"
#define STD_E_ACUTE                   "\x80\xc9"
#define STD_E_CIRC                    "\x80\xca"
#define STD_E_DIARESIS                "\x80\xcb"
#define STD_I_GRAVE                   "\x80\xcc"
#define STD_I_ACUTE                   "\x80\xcd"
#define STD_I_CIRC                    "\x80\xce"
#define STD_I_DIARESIS                "\x80\xcf"
#define STD_ETH                       "\x80\xd0"
#define STD_N_TILDE                   "\x80\xd1"
#define STD_O_GRAVE                   "\x80\xd2"
#define STD_O_ACUTE                   "\x80\xd3"
#define STD_O_CIRC                    "\x80\xd4"
#define STD_O_TILDE                   "\x80\xd5"
#define STD_O_DIARESIS                "\x80\xd6"
#define STD_CROSS                     "\x80\xd7"
#define STD_O_STROKE                  "\x80\xd8"
#define STD_U_GRAVE                   "\x80\xd9"
#define STD_U_ACUTE                   "\x80\xda"
#define STD_U_CIRC                    "\x80\xdb"
#define STD_U_DIARESIS                "\x80\xdc"
#define STD_Y_ACUTE                   "\x80\xdd"
#define STD_s_SHARP                   "\x80\xdf"
#define STD_a_GRAVE                   "\x80\xe0"
#define STD_a_ACUTE                   "\x80\xe1"
#define STD_a_CIRC                    "\x80\xe2"
#define STD_a_TILDE                   "\x80\xe3"
#define STD_a_DIARESIS                "\x80\xe4"
#define STD_a_RING                    "\x80\xe5"
#define STD_ae                        "\x80\xe6"
#define STD_c_CEDILLA                 "\x80\xe7"
#define STD_e_GRAVE                   "\x80\xe8"
#define STD_e_ACUTE                   "\x80\xe9"
#define STD_e_CIRC                    "\x80\xea"
#define STD_e_DIARESIS                "\x80\xeb"
#define STD_i_GRAVE                   "\x80\xec"
#define STD_i_ACUTE                   "\x80\xed"
#define STD_i_CIRC                    "\x80\xee"
#define STD_i_DIARESIS                "\x80\xef"
#define STD_eth                       "\x80\xf0"
#define STD_n_TILDE                   "\x80\xf1"
#define STD_o_GRAVE                   "\x80\xf2"
#define STD_o_ACUTE                   "\x80\xf3"
#define STD_o_CIRC                    "\x80\xf4"
#define STD_o_TILDE                   "\x80\xf5"
#define STD_o_DIARESIS                "\x80\xf6"
#define STD_DIVIDE                    "\x80\xf7"
#define STD_o_STROKE                  "\x80\xf8"
#define STD_u_GRAVE                   "\x80\xf9"
#define STD_u_ACUTE                   "\x80\xfa"
#define STD_u_CIRC                    "\x80\xfb"
#define STD_u_DIARESIS                "\x80\xfc"
#define STD_y_ACUTE                   "\x80\xfd"
#define STD_y_DIARESIS                "\x80\xff"
#define STD_A_MACRON                  "\x81\x00"
#define STD_a_MACRON                  "\x81\x01"
#define STD_A_BREVE                   "\x81\x02"
#define STD_a_BREVE                   "\x81\x03"
#define STD_A_OGONEK                  "\x81\x04"
#define STD_a_OGONEK                  "\x81\x05"
#define STD_C_ACUTE                   "\x81\x06"
#define STD_c_ACUTE                   "\x81\x07"
#define STD_C_CARON                   "\x81\x0c"
#define STD_c_CARON                   "\x81\x0d"
#define STD_D_CARON                   "\x81\x0e"
#define STD_d_APOSTROPHE              "\x81\x0f"
#define STD_D_STROKE                  "\x81\x10"
#define STD_d_STROKE                  "\x81\x11"
#define STD_E_MACRON                  "\x81\x12"
#define STD_e_MACRON                  "\x81\x13"
#define STD_E_BREVE                   "\x81\x14"
#define STD_e_BREVE                   "\x81\x15"
#define STD_E_DOT                     "\x81\x16"
#define STD_e_DOT                     "\x81\x17"
#define STD_E_OGONEK                  "\x81\x18"
#define STD_e_OGONEK                  "\x81\x19"
#define STD_E_CARON                   "\x81\x1a"
#define STD_e_CARON                   "\x81\x1b"
#define STD_G_BREVE                   "\x81\x1e"
#define STD_g_BREVE                   "\x81\x1f"
#define STD_h_STROKE                  "\x81\x27"
#define STD_I_MACRON                  "\x81\x2a"
#define STD_i_MACRON                  "\x81\x2b"
#define STD_I_BREVE                   "\x81\x2c"
#define STD_i_BREVE                   "\x81\x2d"
#define STD_I_OGONEK                  "\x81\x2e"
#define STD_i_OGONEK                  "\x81\x2f"
#define STD_I_DOT                     "\x81\x30"
#define STD_i_DOTLESS                 "\x81\x31"
#define STD_L_ACUTE                   "\x81\x39"
#define STD_l_ACUTE                   "\x81\x3a"
#define STD_L_APOSTROPHE              "\x81\x3d"
#define STD_l_APOSTROPHE              "\x81\x3e"
#define STD_L_STROKE                  "\x81\x41"
#define STD_l_STROKE                  "\x81\x42"
#define STD_N_ACUTE                   "\x81\x43"
#define STD_n_ACUTE                   "\x81\x44"
#define STD_N_CARON                   "\x81\x47"
#define STD_n_CARON                   "\x81\x48"
#define STD_O_MACRON                  "\x81\x4c"
#define STD_o_MACRON                  "\x81\x4d"
#define STD_O_BREVE                   "\x81\x4e"
#define STD_o_BREVE                   "\x81\x4f"
#define STD_OE                        "\x81\x52"
#define STD_oe                        "\x81\x53"
#define STD_R_ACUTE                   "\x81\x54"
#define STD_r_ACUTE                   "\x81\x55"
#define STD_R_CARON                   "\x81\x58"
#define STD_r_CARON                   "\x81\x59"
#define STD_S_ACUTE                   "\x81\x5a"
#define STD_s_ACUTE                   "\x81\x5b"
#define STD_S_CEDILLA                 "\x81\x5e"
#define STD_s_CEDILLA                 "\x81\x5f"
#define STD_S_CARON                   "\x81\x60"
#define STD_s_CARON                   "\x81\x61"
#define STD_T_CEDILLA                 "\x81\x62"
#define STD_t_CEDILLA                 "\x81\x63"
#define STD_T_CARON                   "\x81\x64"
#define STD_t_APOSTROPHE              "\x81\x65"
#define STD_U_TILDE                   "\x81\x68"
#define STD_u_TILDE                   "\x81\x69"
#define STD_U_MACRON                  "\x81\x6a"
#define STD_u_MACRON                  "\x81\x6b"
#define STD_U_BREVE                   "\x81\x6c"
#define STD_u_BREVE                   "\x81\x6d"
#define STD_U_RING                    "\x81\x6e"
#define STD_u_RING                    "\x81\x6f"
#define STD_U_OGONEK                  "\x81\x72"
#define STD_u_OGONEK                  "\x81\x73"
#define STD_W_CIRC                    "\x81\x74"
#define STD_w_CIRC                    "\x81\x75"
#define STD_Y_CIRC                    "\x81\x76"
#define STD_y_CIRC                    "\x81\x77"
#define STD_Y_DIARESIS                "\x81\x78"
#define STD_Z_ACUTE                   "\x81\x79"
#define STD_z_ACUTE                   "\x81\x7a"
#define STD_Z_DOT                     "\x81\x7b"
#define STD_z_DOT                     "\x81\x7c"
#define STD_Z_CARON                   "\x81\x7d"
#define STD_z_CARON                   "\x81\x7e"
#define STD_y_UNDER_ROOT              "\x82\x32"
#define STD_y_BAR                     "\x82\x33"
#define STD_x_BAR                     "\x83\x78"
#define STD_x_CIRC                    "\x83\x79"
#define STD_x_UNDER_ROOT              "\x83\x7f"
#define STD_iota_DIALYTIKA_TONOS      "\x83\x90"
#define STD_ALPHA                     "\x83\x91"
#define STD_BETA                      "\x83\x92"
#define STD_GAMMA                     "\x83\x93"
#define STD_DELTA                     "\x83\x94"
#define STD_EPSILON                   "\x83\x95"
#define STD_ZETA                      "\x83\x96"
#define STD_ETA                       "\x83\x97"
#define STD_THETA                     "\x83\x98"
#define STD_IOTA                      "\x83\x99"
#define STD_KAPPA                     "\x83\x9a"
#define STD_LAMBDA                    "\x83\x9b"
#define STD_MU                        "\x83\x9c"
#define STD_NU                        "\x83\x9d"
#define STD_XI                        "\x83\x9e"
#define STD_OMICRON                   "\x83\x9f"
#define STD_PI                        "\x83\xa0"
#define STD_RHO                       "\x83\xa1"
#define STD_SIGMA                     "\x83\xa3"
#define STD_TAU                       "\x83\xa4"
#define STD_UPSILON                   "\x83\xa5"
#define STD_PHI                       "\x83\xa6"
#define STD_CHI                       "\x83\xa7"
#define STD_PSI                       "\x83\xa8"
#define STD_OMEGA                     "\x83\xa9"
#define STD_IOTA_DIALYTIKA            "\x83\xaa"
#define STD_UPSILON_DIALYTIKA         "\x83\xab"
#define STD_alpha_TONOS               "\x83\xac"
#define STD_epsilon_TONOS             "\x83\xad"
#define STD_eta_TONOS                 "\x83\xae"
#define STD_iota_TONOS                "\x83\xaf"
#define STD_upsilon_DIALYTIKA_TONOS   "\x83\xb0"
#define STD_alpha                     "\x83\xb1"
#define STD_beta                      "\x83\xb2"
#define STD_gamma                     "\x83\xb3"
#define STD_delta                     "\x83\xb4"
#define STD_epsilon                   "\x83\xb5"
#define STD_zeta                      "\x83\xb6"
#define STD_eta                       "\x83\xb7"
#define STD_theta                     "\x83\xb8"
#define STD_iota                      "\x83\xb9"
#define STD_kappa                     "\x83\xba"
#define STD_lambda                    "\x83\xbb"
#define STD_mu                        "\x83\xbc"
#define STD_nu                        "\x83\xbd"
#define STD_xi                        "\x83\xbe"
#define STD_omicron                   "\x83\xbf"
#define STD_pi                        "\x83\xc0"
#define STD_rho                       "\x83\xc1"
#define STD_sigma_end                 "\x83\xc2"
#define STD_sigma                     "\x83\xc3"
#define STD_tau                       "\x83\xc4"
#define STD_upsilon                   "\x83\xc5"
#define STD_phi                       "\x83\xc6"
#define STD_chi                       "\x83\xc7"
#define STD_psi                       "\x83\xc8"
#define STD_omega                     "\x83\xc9"
#define STD_iota_DIALYTIKA            "\x83\xca"
#define STD_upsilon_DIALYTIKA         "\x83\xcb"
#define STD_omicron_TONOS             "\x83\xcc"
#define STD_upsilon_TONOS             "\x83\xcd"
#define STD_omega_TONOS               "\x83\xce"

#define STD_QOPPA                     "\x83\xd8"  //JM GREEK
#define STD_qoppa                     "\x83\xd9"  //JM GREEK
#define STD_DIGAMMA                   "\x83\xdc"  //JM GREEK
#define STD_digamma                   "\x83\xdd"  //JM GREEK
#define STD_SAMPI                     "\x83\xe0"  //JM GREEK
#define STD_sampi                     "\x83\xe1"  //JM GREEK

#define STD_WDOT                      "\xa2\xc5"  //JM  22c5
#define STD_WPERIOD                   "\xa7\x89"  //JM  ff0e  Changed from FF0E to 2789
#define STD_WCOMMA                    "\xa7\x88"  //JM  ff0c  Changed from FF0C to 2788
#define STD_NQUOTE                    "\x82\xbc"  //JM  02bc
#define STD_INV_BRIDGE                "\x83\x3a"  //JM  033a   
#define STD_op_i                      "\xa1\x48"  //JM
#define STD_op_j                      "\xa1\x49"  //JM
#define STD_case                      "\xa6\x7b"  //JM
#define STD_num                       "\xa6\x7e"  //JM
#define STD_RIGHT_TACK                "\xa2\xa2"  //JM

#define STD_LEFT_DASHARROW            "\xa1\xe0"
#define STD_UP_DASHARROW              "\xa1\xe1" 
#define STD_RIGHT_DASHARROW           "\xa1\xe2" 
#define STD_DOWN_DASHARROW            "\xa1\xe3" 

#define STD_SPACE_EM                  "\xa0\x03"
#define STD_SPACE_3_PER_EM            "\xa0\x04"
#define STD_SPACE_4_PER_EM            "\xa0\x05"
#define STD_SPACE_6_PER_EM            "\xa0\x06"
#define STD_SPACE_FIGURE              "\xa0\x07"
#define STD_SPACE_PUNCTUATION         "\xa0\x08"
#define STD_SPACE_HAIR                "\xa0\x0a"
#define STD_LEFT_SINGLE_QUOTE         "\xa0\x18"
#define STD_RIGHT_SINGLE_QUOTE        "\xa0\x19"
#define STD_SINGLE_LOW_QUOTE          "\xa0\x1a"
#define STD_SINGLE_HIGH_QUOTE         "\xa0\x1b"
#define STD_LEFT_DOUBLE_QUOTE         "\xa0\x1c"
#define STD_RIGHT_DOUBLE_QUOTE        "\xa0\x1d"
#define STD_DOUBLE_LOW_QUOTE          "\xa0\x1e"
#define STD_DOUBLE_HIGH_QUOTE         "\xa0\x1f"
#define STD_ELLIPSIS                  "\xa0\x26"
#define STD_BINARY_ONE                "\xa0\x27"
#define STD_SUP_MINUS_1               "\xa0\x72"
#define STD_SUB_E_OUTLINE             "\xa0\x73"
#define STD_SUB_0                     "\xa0\x80"
#define STD_SUB_1                     "\xa0\x81"
#define STD_SUB_2                     "\xa0\x82"
#define STD_SUB_3                     "\xa0\x83"
#define STD_SUB_4                     "\xa0\x84"
#define STD_SUB_5                     "\xa0\x85"
#define STD_SUB_6                     "\xa0\x86"
#define STD_SUB_7                     "\xa0\x87"
#define STD_SUB_8                     "\xa0\x88"
#define STD_SUB_9                     "\xa0\x89"
#define STD_SUB_PLUS                  "\xa0\x8a"
#define STD_SUB_MINUS                 "\xa0\x8b"
#define STD_SUP_ASTERISK              "\xa0\x8f"
#define STD_SUP_INFINITY              "\xa0\x9e"
#define STD_SUB_INFINITY              "\xa0\x9f"
#define STD_EURO                      "\xa0\xac"
#define STD_COMPLEX_C                 "\xa1\x02"
#define STD_PLANCK                    "\xa1\x0e"
#define STD_PLANCK_2PI                "\xa1\x0f"
#define STD_NATURAL_N                 "\xa1\x15"
#define STD_RATIONAL_Q                "\xa1\x1a"
#define STD_REAL_R                    "\xa1\x1d"
#define STD_INTEGER_Z                 "\xa1\x24"
#define STD_SUP_0                     "\xa1\x60"
#define STD_SUP_1                     "\xa1\x61"
#define STD_SUP_2                     "\xa1\x62"
#define STD_SUP_3                     "\xa1\x63"
#define STD_SUP_4                     "\xa1\x64"
#define STD_SUP_5                     "\xa1\x65"
#define STD_SUP_6                     "\xa1\x66"
#define STD_SUP_7                     "\xa1\x67"
#define STD_SUP_8                     "\xa1\x68"
#define STD_SUP_9                     "\xa1\x69"
#define STD_SUP_PLUS                  "\xa1\x6a"
#define STD_SUP_MINUS                 "\xa1\x6b"
#define STD_LEFT_ARROW                "\xa1\x90"
#define STD_UP_ARROW                  "\xa1\x91"
#define STD_RIGHT_ARROW               "\xa1\x92"
#define STD_DOWN_ARROW                "\xa1\x93"
#define STD_SERIAL_IO                 "\xa1\x95"
#define STD_RIGHT_SHORT_ARROW         "\xa1\xc0"
#define STD_LEFT_RIGHT_ARROWS         "\xa1\xc4"
#define STD_BST                       "\xa1\xc9"
#define STD_SST                       "\xa1\xcb"
#define STD_HAMBURGER                 "\xa1\xcc"
#define STD_UNDO                      "\xa1\xcd"
#define STD_HOLLOW_UP_ARROW           "\xa1\xe7"
#define STD_HOLLOW_DOWN_ARROW         "\xa1\xe9"
#define STD_FOR_ALL                   "\xa2\x00"
#define STD_COMPLEMENT                "\xa2\x01"
#define STD_PARTIAL_DIFF              "\xa2\x02"
#define STD_THERE_EXISTS              "\xa2\x03"
#define STD_THERE_DOES_NOT_EXIST      "\xa2\x04"
#define STD_EMPTY_SET                 "\xa2\x05"
#define STD_INCREMENT                 "\xa2\x06"
#define STD_NABLA                     "\xa2\x07"
#define STD_ELEMENT_OF                "\xa2\x08"
#define STD_NOT_ELEMENT_OF            "\xa2\x09"
#define STD_CONTAINS                  "\xa2\x0b"
#define STD_DOES_NOT_CONTAIN          "\xa2\x0c"
#define STD_BINARY_ZERO               "\xa2\x0e"
#define STD_PRODUCT                   "\xa2\x0f"
#define STD_MINUS_PLUS                "\xa2\x13"
#define STD_RING                      "\xa2\x18"
#define STD_BULLET                    "\xa2\x19"
#define STD_SQUARE_ROOT               "\xa2\x1a"
#define STD_CUBE_ROOT                 "\xa2\x1b"
#define STD_xTH_ROOT                  "\xa2\x1c"
#define STD_PROPORTIONAL              "\xa2\x1d"
#define STD_INFINITY                  "\xa2\x1e"
#define STD_RIGHT_ANGLE               "\xa2\x1f"
#define STD_ANGLE                     "\xa2\x20"
#define STD_MEASURED_ANGLE            "\xa2\x21"
#define STD_SPHERICAL_ANGLE           "\xa2\x22"
#define STD_DIVIDES                   "\xa2\x23"
#define STD_DOES_NOT_DIVIDE           "\xa2\x24"
#define STD_PARALLEL                  "\xa2\x25"
#define STD_NOT_PARALLEL              "\xa2\x26"
#define STD_AND                       "\xa2\x27"
#define STD_OR                        "\xa2\x28"
#define STD_INTERSECTION              "\xa2\x29"
#define STD_UNION                     "\xa2\x2a"
#define STD_INTEGRAL                  "\xa2\x2b"
#define STD_DOUBLE_INTEGRAL           "\xa2\x2c"
#define STD_TRIPLE_INTEGRAL           "\xa2\x2d"
#define STD_CONTOUR_INTEGRAL          "\xa2\x2e"
#define STD_SURFACE_INTEGRAL          "\xa2\x2f"
#define STD_VOLUME_INTEGRAL           "\xa2\x30"
#define STD_RATIO                     "\xa2\x36"
#define STD_ASYMPOTICALLY_EQUAL       "\xa2\x43"
#define STD_ALMOST_EQUAL              "\xa2\x48"
#define STD_COLON_EQUALS              "\xa2\x54"
#define STD_CORRESPONDS_TO            "\xa2\x58"
#define STD_ESTIMATES                 "\xa2\x59"
#define STD_NOT_EQUAL                 "\xa2\x60"
#define STD_IDENTICAL_TO              "\xa2\x61"
#define STD_LESS_EQUAL                "\xa2\x64"
#define STD_GREATER_EQUAL             "\xa2\x65"
#define STD_MUCH_LESS                 "\xa2\x6a"
#define STD_MUCH_GREATER              "\xa2\x6b"
#define STD_SUB_EARTH                 "\xa2\x95"
#define STD_SUB_alpha                 "\xa2\x96"
#define STD_SUB_delta                 "\xa2\x97"
#define STD_SUB_mu                    "\xa2\x98"
#define STD_SUN                       "\xa2\x99"
#define STD_SUB_SUN                   "\xa2\x9a"
#define STD_DOWN_TACK                 "\xa2\xa4"
#define STD_PERPENDICULAR             "\xa2\xa5"
#define STD_XOR                       "\xa2\xbb"
#define STD_NAND                      "\xa2\xbc"
#define STD_NOR                       "\xa2\xbd"
#define STD_LEFT_CEILING              "\xa3\x08"
#define STD_RIGHT_CEILING             "\xa3\x09"
#define STD_LEFT_FLOOR                "\xa3\x0a"
#define STD_RIGHT_FLOOR               "\xa3\x0b"
#define STD_WATCH                     "\xa3\x1a"
#define STD_HOURGLASS                 "\xa3\x1b"
#define STD_PRINTER                   "\xa3\x99"
#define STD_MAT_TL                    "\xa3\xa1"
#define STD_MAT_ML                    "\xa3\xa2"
#define STD_MAT_BL                    "\xa3\xa3"
#define STD_MAT_TR                    "\xa3\xa4"
#define STD_MAT_MR                    "\xa3\xa5"
#define STD_MAT_BR                    "\xa3\xa6"
#define STD_TIMER                     "\xa3\xf1"
#define STD_OBLIQUE1                  "\xa4\x21"
#define STD_OBLIQUE2                  "\xa4\x22"
#define STD_OPEN_BOX                  "\xa4\x23"
#define STD_OBLIQUE3                  "\xa4\x25"
#define STD_OBLIQUE4                  "\xa4\x26"
#define STD_CURSOR                    "\xa4\x27"
#define STD_PERIOD34                  "\xa4\x28"
#define STD_COMMA34                   "\xa4\x29"
#define STD_BATTERY                   "\xa4\x2a"
#define STD_PGM_BEGIN                 "\xa4\x2b"
#define STD_USER_MODE                 "\xa4\x2c"
#define STD_UK                        "\xa4\x2d"
#define STD_US                        "\xa4\x2e"
#define STD_NEG_EXCLAMATION_MARK      "\xa4\x2f"
#define STD_GAUSS_BLACK_L             "\xa4\x30"
#define STD_GAUSS_WHITE_R             "\xa4\x31"
#define STD_GAUSS_WHITE_L             "\xa4\x32"
#define STD_GAUSS_BLACK_R             "\xa4\x33"
#define STD_USB                       "\xa4\x34"
#define STD_BASE_1                    "\xa4\x60"
#define STD_BASE_2                    "\xa4\x61"
#define STD_BASE_3                    "\xa4\x62"
#define STD_BASE_4                    "\xa4\x63"
#define STD_BASE_5                    "\xa4\x64"
#define STD_BASE_6                    "\xa4\x65"
#define STD_BASE_7                    "\xa4\x66"
#define STD_BASE_8                    "\xa4\x67"
#define STD_BASE_9                    "\xa4\x68"
#define STD_BASE_10                   "\xa4\x69"
#define STD_BASE_11                   "\xa4\x6a"
#define STD_BASE_12                   "\xa4\x6b"
#define STD_BASE_13                   "\xa4\x6c"
#define STD_BASE_14                   "\xa4\x6d"
#define STD_BASE_15                   "\xa4\x6e"
#define STD_BASE_16                   "\xa4\x6f"
#define STD_SUB_10                    "\xa4\x7d"
#define STD_SUP_a                     "\xa4\x82"
#define STD_SUP_b                     "\xa4\x83"
#define STD_SUP_c                     "\xa4\x84"
#define STD_SUP_d                     "\xa4\x85"
#define STD_SUP_e                     "\xa4\x86"
#define STD_SUP_f                     "\xa4\x87"
#define STD_SUP_g                     "\xa4\x88"
#define STD_SUP_h                     "\xa4\x89"
#define STD_SUP_i                     "\xa4\x8a"
#define STD_SUP_j                     "\xa4\x8b"
#define STD_SUP_k                     "\xa4\x8c"
#define STD_SUP_l                     "\xa4\x8d"
#define STD_SUP_m                     "\xa4\x8e"
#define STD_SUP_n                     "\xa4\x8f"
#define STD_SUP_o                     "\xa4\x90"
#define STD_SUP_p                     "\xa4\x91"
#define STD_SUP_q                     "\xa4\x92"
#define STD_SUP_r                     "\xa4\x93"
#define STD_SUP_s                     "\xa4\x94"
#define STD_SUP_t                     "\xa4\x95"
#define STD_SUP_u                     "\xa4\x96"
#define STD_SUP_v                     "\xa4\x97"
#define STD_SUP_w                     "\xa4\x98"
#define STD_SUP_x                     "\xa4\x99"
#define STD_SUP_y                     "\xa4\x9a"
#define STD_SUP_z                     "\xa4\x9b"
#define STD_SUB_a                     "\xa4\x9c"
#define STD_SUB_b                     "\xa4\x9d"
#define STD_SUB_c                     "\xa4\x9e"
#define STD_SUB_d                     "\xa4\x9f"
#define STD_SUB_e                     "\xa4\xa0"
#define STD_SUB_f                     "\xa4\xa1"
#define STD_SUB_g                     "\xa4\xa2"
#define STD_SUB_h                     "\xa4\xa3"
#define STD_SUB_i                     "\xa4\xa4"
#define STD_SUB_j                     "\xa4\xa5"
#define STD_SUB_k                     "\xa4\xa6"
#define STD_SUB_l                     "\xa4\xa7"
#define STD_SUB_m                     "\xa4\xa8"
#define STD_SUB_n                     "\xa4\xa9"
#define STD_SUB_o                     "\xa4\xaa"
#define STD_SUB_p                     "\xa4\xab"
#define STD_SUB_q                     "\xa4\xac"
#define STD_SUB_r                     "\xa4\xad"
#define STD_SUB_s                     "\xa4\xae"
#define STD_SUB_t                     "\xa4\xaf"
#define STD_SUB_u                     "\xa4\xb0"
#define STD_SUB_v                     "\xa4\xb1"
#define STD_SUB_w                     "\xa4\xb2"
#define STD_SUB_x                     "\xa4\xb3"
#define STD_SUB_y                     "\xa4\xb4"
#define STD_SUB_z                     "\xa4\xb5"
#define STD_SUP_A                     "\xa4\xb6"
#define STD_SUP_B                     "\xa4\xb7"
#define STD_SUP_C                     "\xa4\xb8"
#define STD_SUP_D                     "\xa4\xb9"
#define STD_SUP_E                     "\xa4\xba"
#define STD_SUP_F                     "\xa4\xbb"
#define STD_SUP_G                     "\xa4\xbc"
#define STD_SUP_H                     "\xa4\xbd"
#define STD_SUP_I                     "\xa4\xbe"
#define STD_SUP_J                     "\xa4\xbf"
#define STD_SUP_K                     "\xa4\xc0"
#define STD_SUP_L                     "\xa4\xc1"
#define STD_SUP_M                     "\xa4\xc2"
#define STD_SUP_N                     "\xa4\xc3"
#define STD_SUP_O                     "\xa4\xc4"
#define STD_SUP_P                     "\xa4\xc5"
#define STD_SUP_Q                     "\xa4\xc6"
#define STD_SUP_R                     "\xa4\xc7"
#define STD_SUP_S                     "\xa4\xc8"
#define STD_SUP_T                     "\xa4\xc9"
#define STD_SUP_U                     "\xa4\xca"
#define STD_SUP_V                     "\xa4\xcb"
#define STD_SUP_W                     "\xa4\xcc"
#define STD_SUP_X                     "\xa4\xcd"
#define STD_SUP_Y                     "\xa4\xce"
#define STD_SUP_Z                     "\xa4\xcf"
#define STD_SUB_A                     "\xa4\xd0"
#define STD_SUB_B                     "\xa4\xd1"
#define STD_SUB_C                     "\xa4\xd2"
#define STD_SUB_D                     "\xa4\xd3"
#define STD_SUB_E                     "\xa4\xd4"
#define STD_SUB_F                     "\xa4\xd5"
#define STD_SUB_G                     "\xa4\xd6"
#define STD_SUB_H                     "\xa4\xd7"
#define STD_SUB_I                     "\xa4\xd8"
#define STD_SUB_J                     "\xa4\xd9"
#define STD_SUB_K                     "\xa4\xda"
#define STD_SUB_L                     "\xa4\xdb"
#define STD_SUB_M                     "\xa4\xdc"
#define STD_SUB_N                     "\xa4\xdd"
#define STD_SUB_O                     "\xa4\xde"
#define STD_SUB_P                     "\xa4\xdf"
#define STD_SUB_Q                     "\xa4\xe0"
#define STD_SUB_R                     "\xa4\xe1"
#define STD_SUB_S                     "\xa4\xe2"
#define STD_SUB_T                     "\xa4\xe3"
#define STD_SUB_U                     "\xa4\xe4"
#define STD_SUB_V                     "\xa4\xe4"
#define STD_SUB_W                     "\xa4\xe6"
#define STD_SUB_X                     "\xa4\xe7"
#define STD_SUB_Y                     "\xa4\xe8"
#define STD_SUB_Z                     "\xa4\xe9"
#define STD_CHECK_MARK                "\xa7\x13"
#define STD_RIGHT_DOUBLE_ARROW        "\xa1\xD2"    //JM
#define STD_RECYCLE                   "\xa6\x7A"    //JM
#endif // !FONTS_H
