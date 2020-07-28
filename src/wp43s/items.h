/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file items.h
 ***********************************************/

#define NOPARAM       9876
#define CONFIRMED     9877
#define NOT_CONFIRMED 9878

#define SLS_ENABLED   0
#define SLS_DISABLED  1
#define SLS_UNCHANGED 2


#define ITM_NULL                         0
#define ITM_CtoF                         1
#define ITM_FtoC                         2
#define ITM_10x                          3
#define ITM_1COMPL                       4
#define ITM_SCRDMP                       5
#define ITM_1ONX                         6
#define ITM_2COMPL                       7
#define ITM_2X                           8
#define ITM_CUBEROOT                     9
#define ITM_REGA                        10
#define CST_01                          11
#define CST_02                          12
#define ITM_ABS                         13
#define ITM_ACC                         14
#define ITM_ACtoM2                      15
#define ITM_ACUStoM2                    16
#define MNU_ADV                         17
#define ITM_AGM                         18
#define ITM_AGRAPH                      19
#define ITM_ALL                         20
#define CST_03                          21
#define ITM_AND                         22
#define MNU_ANGLES                      23
#define ITM_arccos                      24
#define ITM_arcosh                      25
#define ITM_arcsin                      26
#define ITM_arctan                      27
#define ITM_arsinh                      28
#define ITM_artanh                      29
#define ITM_ASR                         30
#define ITM_ASSIGN                      31
#define ITM_ATMtoPA                     32
#define ITM_AUtoM                       33
#define MNU_PRINT                       34
#define MNU_CONVA                       35
#define CST_04                          36
#define ITM_REGB                        37
#define ITM_BACK                        38
#define ITM_BARtoPA                     39
#define ITM_BATT                        40
#define ITM_BC                          41
#define ITM_BEEP                        42
#define ITM_BEGINP                      43
#define ITM_BESTF                       44
#define ITM_BINOMP                      45
#define ITM_BINOM                       46
#define ITM_BINOMU                      47
#define ITM_BINOMM1                     48
#define MNU_BINOM                       49
#define MNU_BITS                        50
#define ITM_BN                          51
#define ITM_BNS                         52
#define ITM_BS                          53
#define ITM_BTUtoJ                      54
#define ITM_REGC                        55
#define CST_05                          56
#define CST_06                          57
#define CST_07                          58
#define ITM_CALtoJ                      59
#define ITM_CASE                        60
#define MNU_CATALOG                     61
#define ITM_CAUCHP                      62
#define ITM_CAUCH                       63
#define ITM_CAUCHU                      64
#define ITM_CAUCHM1                     65
#define MNU_CAUCH                       66
#define ITM_CB                          67
#define ITM_CEIL                        68
#define ITM_CF                          69
#define MNU_CHARS                       70
#define ITM_CLALL                       71
#define ITM_CLCVAR                      72
#define ITM_CLFALL                      73
#define MNU_CLK                         74
#define ITM_TGLFRT                      75
#define MNU_REGIST                      76
#define ITM_CLLCD                       77
#define ITM_CLMENU                      78
#define ITM_CLP                         79
#define ITM_CLPALL                      80
#define MNU_CLR                         81
#define ITM_CLREGS                      82
#define ITM_CLSTK                       83
#define ITM_CLX                         84
#define ITM_CLSIGMA                     85
#define MNU_CONST                       86
#define ITM_COMB                        87
#define ITM_CONJ                        88
#define ITM_CNST                        89
#define ITM_CONVG                       90
#define ITM_CORR                        91
#define ITM_cos                         92
#define ITM_cosh                        93
#define ITM_COV                         94
#define MNU_CPX                         95
#define ITM_LBFFTtoNM                   96
#define ITM_LBFFTtoNMb                  97
#define ITM_BESTFQ                      98
#define MNU_CPXS                        99
#define ITM_CPX                        100
#define ITM_CROSS                      101
#define ITM_CWTtoKG                    102
#define ITM_CXtoRE                     103
#define ITM_REGD                       104
#define ITM_DATE                       105
#define MNU_DATES                      106
#define ITM_DATEto                     107
#define ITM_DAY                        108
#define ITM_0109                       109
#define ITM_DBLR                       110
#define ITM_DBLCROSS                   111
#define ITM_DBLSLASH                   112
#define ITM_DBtoFR                     113
#define ITM_DBtoPR                     114
#define ITM_DEC                        115
#define ITM_DECOMP                     116
#define ITM_DEG                        117
#define ITM_DEGto                      118
#define ITM_NMtoLBFFT                  119
#define ITM_NMtoLBFFTb                 120
#define ITM_SA                         121
#define ITM_DENMAX                     122
#define MNU_DIGITS                     123
#define MNU_DSP                        124
#define ITM_DOT                        125
#define ITM_toDP                       126
#define ITM_DROP                       127
#define ITM_DROPY                      128
#define ITM_DSE                        129
#define ITM_DSL                        130
#define ITM_0131                       131
#define ITM_DSTACK                     132
#define ITM_DSZ                        133
#define ITM_DMS                        134
#define ITM_DMSto                      135
#define ITM_DMY                        136
#define ITM_DtoJ                       137
#define ITM_DtoR                       138
#define CST_08                         139
#define CST_09                         140
#define ITM_EIGVAL                     141
#define ITM_EIGVEC                     142
#define ITM_END                        143
#define ITM_ENDP                       144
#define ITM_ENG                        145
#define ITM_0146                       146
#define ITM_ENORM                      147
#define ITM_ENTER                      148
#define ITM_ENTRY                      149
#define MNU_EQN                        150
#define ITM_EQ_DEL                     151
#define ITM_EQ_EDI                     152
#define ITM_EQ_NEW                     153
#define ITM_ERF                        154
#define ITM_ERFC                       155
#define ITM_ERR                        156
#define ITM_EVEN                       157
#define ITM_EX                         158
#define ITM_EXITALL                    159
#define MNU_EXP                        160
#define ITM_EXPF                       161
#define ITM_EXPONP                     162
#define ITM_EXPON                      163
#define ITM_EXPONU                     164
#define ITM_EXPONM1                    165
#define MNU_EXPON                      166
#define ITM_EXPT                       167
#define ITM_EX1                        168
#define CST_10                         169
#define MNU_CONVE                      170
#define CST_11                         171
#define ITM_0172                       172
#define ITM_FB                         173
#define MNU_FCNS                       174
#define ITM_FC                         175
#define ITM_FCC                        176
#define ITM_FCF                        177
#define ITM_FCS                        178
#define ITM_FTtoM                      179
#define ITM_FF                         180
#define ITM_FIB                        181
#define ITM_FILL                       182
#define MNU_FIN                        183
#define MNU_SINTS                      184
#define ITM_FIX                        185
#define MNU_FLAGS                      186
#define MNU_FLASH                      187
#define ITM_FLASH                      188
#define ITM_FLOOR                      189
#define ITM_FP                         190
#define ITM_FPQ                        191
#define ITM_FPX                        192
#define ITM_FX                         193
#define ITM_FUX                        194
#define ITM_FM1P                       195
#define ITM_FRtoDB                     196
#define ITM_FS                         197
#define ITM_FSC                        198
#define ITM_FSF                        199
#define ITM_FSS                        200
#define ITM_FTUStoM                    201
#define ITM_FV                         202
#define ITM_FZUKtoM3                   203
#define ITM_FZUStoM3                   204
#define CST_12                         205
#define CST_13                         206
#define MNU_F                          207
#define MNU_1STDERIV                   208
#define MNU_2NDDERIV                   209
#define ITM_FQX                        210
#define ITM_FDQX                       211
#define MNU_CONVFP                     212
#define CST_14                         213
#define CST_15                         214
#define ITM_GAP                        215
#define CST_16                         216
#define ITM_GCD                        217
#define ITM_GD                         218
#define ITM_GDM1                       219
#define CST_17                         220
#define ITM_GEOMP                      221
#define ITM_GEOM                       222
#define ITM_GEOMU                      223
#define ITM_GEOMM1                     224
#define MNU_GEOM                       225
#define ITM_GLUKtoM3                   226
#define ITM_GLUStoM3                   227
#define CST_18                         228
#define ITM_GRAD                       229
#define ITM_GRADto                     230
#define ITM_GTO                        231
#define ITM_GTOP                       232
#define CST_19                         233
#define CST_20                         234
#define ITM_HN                         235
#define ITM_HNP                        236
#define ITM_HPEtoW                     237
#define ITM_HPMtoW                     238
#define ITM_HPUKtoW                    239
#define ITM_HYPERP                     240
#define ITM_HYPER                      241
#define ITM_HYPERU                     242
#define ITM_HYPERM1                    243
#define MNU_HYPER                      244
#define CST_21                         245
#define ITM_REGI                       246
#define ITM_IDIV                       247
#define ITM_IHGtoPA                    248
#define MNU_LINTS                      249
#define ITM_IM                         250
#define ITM_0251                       251
#define ITM_INC                        252
#define ITM_INDEX                      253
#define MNU_INFO                       254
#define ITM_INPUT                      255
#define MNU_INTS                       256
#define ITM_INT                        257
#define ITM_INtoM                      258
#define ITM_IP                         259
#define ITM_ISE                        260
#define ITM_ISG                        261
#define ITM_ISZ                        262
#define ITM_IXYZ                       263
#define ITM_IGAMMAP                    264
#define ITM_IGAMMAQ                    265
#define ITM_IPLUS                      266
#define ITM_IMINUS                     267
#define MNU_IO                         268
#define ITM_IPCA                       269
#define ITM_REGJ                       270
#define ITM_JYX                        271
#define ITM_JPLUS                      272
#define ITM_JMINUS                     273
#define ITM_JonG                       274
#define ITM_JtoBTU                     275
#define ITM_JtoCAL                     276
#define ITM_JtoD                       277
#define ITM_JtoWH                      278
#define ITM_REGK                       279
#define CST_22                         280
#define ITM_KEY                        281
#define ITM_KEYG                       282
#define ITM_KEYX                       283
#define ITM_KEYQ                       284
#define ITM_KGtoCWT                    285
#define ITM_KGtoLBS                    286
#define ITM_KGtoOZ                     287
#define ITM_KGtoSCW                    288
#define ITM_KGtoSTO                    289
#define ITM_KGtoST                     290
#define ITM_KGtoTON                    291
#define ITM_KGtoTRZ                    292
#define CST_23                         293
#define ITM_KTYP                       294
#define ITM_REGL                       295
#define ITM_LASTX                      296
#define ITM_LBFtoN                     297
#define ITM_LBL                        298
#define ITM_LBLQ                       299
#define ITM_LBStoKG                    300
#define ITM_LCM                        301
#define ITM_LEAP                       302
#define ITM_LGNRMP                     303
#define ITM_LGNRM                      304
#define ITM_LGNRMU                     305
#define ITM_LGNRMM1                    306
#define MNU_LGNRM                      307
#define ITM_LINF                       308
#define ITM_LJ                         309
#define ITM_LN                         310
#define ITM_Ln                         311
#define ITM_LN1X                       312
#define ITM_LNALPHA                    313
#define ITM_LNBETA                     314
#define ITM_LNGAMMA                    315
#define ITM_LOAD                       316
#define ITM_LOADP                      317
#define ITM_LOADR                      318
#define ITM_LOADSS                     319
#define ITM_LOADSIGMA                  320
#define ITM_LocR                       321
#define ITM_LocRQ                      322
#define ITM_LOG10                      323
#define ITM_LOG2                       324
#define ITM_LOGF                       325
#define ITM_LOGISP                     326
#define ITM_LOGIS                      327
#define ITM_LOGISU                     328
#define ITM_LOGISM1                    329
#define MNU_LOGIS                      330
#define ITM_LOGXY                      331
#define MNU_LOOP                       332
#define CST_24                         333
#define ITM_LYtoM                      334
#define ITM_MMHGtoPA                   335
#define ITM_MMHGtoPAb                  336
#define ITM_LR                         337
#define ITM_M2toAC                     338
#define ITM_M2toACUS                   339
#define ITM_M3toFZUK                   340
#define ITM_M3toFZUS                   341
#define ITM_M3toGLUK                   342
#define ITM_M3toGLUS                   343
#define ITM_MANT                       344
#define ITM_MASKL                      345
#define ITM_MASKR                      346
#define MNU_MATRS                      347
#define ITM_MATR                       348
#define MNU_MATX                       349
#define ITM_MATA                       350
#define ITM_MATB                       351
#define ITM_MATX                       352
#define ITM_MAX                        353
#define CST_25                         354
#define ITM_MEM                        355
#define ITM_MENU                       356
#define MNU_MENUS                      357
#define ITM_MIN                        358
#define ITM_MIRROR                     359
#define ITM_MItoM                      360
#define CST_26                         361
#define CST_27                         362
#define CST_28                         363
#define ITM_MOD                        364
#define MNU_MODE                       365
#define ITM_MONTH                      366
#define CST_29                         367
#define CST_30                         368
#define CST_31                         369
#define ITM_MSG                        370
#define CST_32                         371
#define CST_33                         372
#define MNU_SIMQ                       373
#define MNU_M_EDIT                     374
#define ITM_MULPI                      375
#define ITM_MVAR                       376
#define MNU_MyMenu                     377
#define MNU_MyAlpha                    378
#define CST_34                         379
#define ITM_M_DELR                     380
#define ITM_M_DIM                      381
#define ITM_M_DIMQ                     382
#define ITM_MDY                        383
#define ITM_M_EDI                      384
#define ITM_M_EDIN                     385
#define ITM_PAtoMMHG                   386
#define ITM_M_GET                      387
#define ITM_M_GOTO                     388
#define ITM_M_GROW                     389
#define ITM_M_INSR                     390
#define ITM_M_LU                       391
#define ITM_M_NEW                      392
#define ITM_M_OLD                      393
#define ITM_M_PUT                      394
#define ITM_M_RR                       395
#define ITM_PAtoMMHGb                  396
#define ITM_M_SQR                      397
#define ITM_M_WRAP                     398
#define MNU_CONVM                      399
#define ITM_MtoAU                      400
#define ITM_MtoFT                      401
#define ITM_MtoFTUS                    402
#define ITM_MtoINCH                    403
#define ITM_MtoLY                      404
#define ITM_MtoMI                      405
#define ITM_MtoNMI                     406
#define ITM_MtoPC                      407
#define ITM_MtoPOINT                   408
#define ITM_MtoYD                      409
#define CST_35                         410
#define CST_36                         411
#define CST_37                         412
#define CST_38                         413
#define ITM_NAND                       414
#define ITM_NAN                        415
#define ITM_NBINP                      416
#define ITM_NBIN                       417
#define ITM_NBINU                      418
#define ITM_NBINM1                     419
#define MNU_NBIN                       420
#define ITM_NEIGHB                     421
#define ITM_NEXTP                      422
#define ITM_NMItoM                     423
#define ITM_NOP                        424
#define ITM_NOR                        425
#define ITM_NORMLP                     426
#define ITM_NORML                      427
#define ITM_NORMLU                     428
#define ITM_NORMLM1                    429
#define MNU_NORML                      430
#define ITM_NOT                        431
#define ITM_NPER                       432
#define ITM_NSIGMA                     433
#define ITM_NtoLBF                     434
#define ITM_ODD                        435
#define ITM_OFF                        436
#define ITM_OR                         437
#define ITM_ORTHOF                     438
#define MNU_ORTHOG                     439
#define ITM_OZtoKG                     440
#define CST_39                         441
#define ITM_PAUSE                      442
#define ITM_PAtoATM                    443
#define ITM_PAtoBAR                    444
#define ITM_PAtoIHG                    445
#define ITM_PAtoPSI                    446
#define ITM_PAtoTOR                    447
#define MNU_PARTS                      448
#define ITM_PCtoM                      449
#define ITM_PERM                       450
#define ITM_PERonA                     451
#define ITM_PGMINT                     452
#define ITM_PGMSLV                     453
#define ITM_PIXEL                      454
#define ITM_PLOT                       455
#define ITM_PMT                        456
#define ITM_PN                         457
#define ITM_POINT                      458
#define ITM_POISSP                     459
#define ITM_POISS                      460
#define ITM_POISSU                     461
#define ITM_POISSM1                    462
#define MNU_POISS                      463
#define ITM_LOADV                      464
#define ITM_POPLR                      465
#define ITM_POWERF                     466
#define ITM_PRtoDB                     467
#define ITM_PRCL                       468
#define ITM_PRIME                      469
#define MNU_PROB                       470
#define ITM_0471                       471
#define MNU_PROGS                      472
#define ITM_PSItoPA                    473
#define ITM_PSTO                       474
#define ITM_POINTtoM                   475
#define ITM_PUTK                       476
#define ITM_PV                         477
#define MNU_PFN                        478
#define MNU_PFN2                       479
#define MNU_CONVP                      480
#define ITM_0481                       481
#define CST_40                         482
#define ITM_RAD                        483
#define ITM_RADto                      484
#define MNU_RAM                        485
#define ITM_RAN                        486
#define ITM_RBR                        487
#define ITM_RCL                        488
#define ITM_RCLCFG                     489
#define ITM_RCLEL                      490
#define ITM_RCLIJ                      491
#define ITM_RCLS                       492
#define ITM_RCLPLUS                    493
#define ITM_RCLMINUS                   494
#define ITM_RCLMULT                    495
#define ITM_RCLDIV                     496
#define ITM_RCLMAX                     497
#define ITM_RCLMIN                     498
#define ITM_RDP                        499
#define ITM_0500                       500
#define ITM_0501                       501
#define CST_41                         502
#define ITM_RE                         503
#define ITM_0504                       504
#define MNU_REALS                      505
#define ITM_REAL                       506
#define ITM_0507                       507
#define ITM_REGS                       508
#define ITM_RECV                       509
#define ITM_RESET                      510
#define ITM_REtoCX                     511
#define ITM_REexIM                     512
#define ITM_RJ                         513
#define CST_42                         514
#define ITM_RL                         515
#define ITM_RLC                        516
#define CST_43                         517
#define ITM_RM                         518
#define ITM_RMQ                        519
#define ITM_RMD                        520
#define ITM_RNORM                      521
#define ITM_ROUND                      522
#define ITM_ROUNDI                     523
#define ITM_RR                         524
#define ITM_RRC                        525
#define ITM_RSD                        526
#define ITM_RSUM                       527
#define ITM_RTN                        528
#define ITM_RTNP1                      529
#define ITM_R_CLR                      530
#define ITM_R_COPY                     531
#define ITM_R_SORT                     532
#define ITM_R_SWAP                     533
#define ITM_RtoD                       534
#define ITM_Rup                        535
#define ITM_Rdown                      536
#define CST_44                         537
#define CST_45                         538
#define CST_46                         539
#define ITM_S                          540
#define CST_47                         541
#define ITM_SAVE                       542
#define ITM_SB                         543
#define CST_48                         544
#define ITM_SCI                        545
#define ITM_SCWtoKG                    546
#define ITM_0547                       547
#define ITM_SDIGS                      548
#define ITM_SDL                        549
#define ITM_SDR                        550
#define CST_49                         551
#define ITM_SEED                       552
#define ITM_SEND                       553
#define ITM_SETCHN                     554
#define ITM_SETDAT                     555
#define ITM_SETEUR                     556
#define ITM_SETIND                     557
#define ITM_SETJPN                     558
#define ITM_SETSIG                     559
#define ITM_SETTIM                     560
#define ITM_SETUK                      561
#define ITM_SETUSA                     562
#define CST_50                         563
#define ITM_SF                         564
#define CST_51                         565
#define ITM_SIGN                       566
#define ITM_SIGNMT                     567
#define ITM_SIM_EQ                     568
#define ITM_sin                        569
#define ITM_sinc                       570
#define ITM_sinh                       571
#define ITM_SKIP                       572
#define ITM_SL                         573
#define ITM_0574                       574
#define ITM_SLVQ                       575
#define ITM_SM                         576
#define ITM_SMODE                      577
#define ITM_SMW                        578
#define ITM_SOLVE                      579
#define MNU_Solver                     580
#define ITM_SPEC                       581
#define ITM_SR                         582
#define ITM_0583                       583
#define ITM_0584                       584
#define ITM_SSIZE                      585
#define MNU_STAT                       586
#define ITM_STATUS                     587
#define MNU_STK                        588
#define ITM_STO                        589
#define ITM_STOCFG                     590
#define ITM_STOEL                      591
#define ITM_STOIJ                      592
#define ITM_STOP                       593
#define ITM_STOS                       594
#define ITM_STOPLUS                    595
#define ITM_STOMINUS                   596
#define ITM_STOMULT                    597
#define ITM_STODIV                     598
#define ITM_STOMAX                     599
#define ITM_STOMIN                     600
#define ITM_STOtoKG                    601
#define ITM_STRI                       602
#define MNU_STRING                     603
#define ITM_ST_A                       604
#define ITM_ST_B                       605
#define ITM_ST_C                       606
#define ITM_ST_D                       607
#define ITM_ST_T                       608
#define ITM_ST_X                       609
#define ITM_ST_Y                       610
#define ITM_ST_Z                       611
#define ITM_SUM                        612
#define ITM_SW                         613
#define ITM_SXY                        614
#define ITM_STtoKG                     615
#define ITM_StoYEAR                    616
#define CST_52                         617
#define ITM_tan                        618
#define ITM_tanh                       619
#define ITM_TDISP                      620
#define MNU_TEST                       621
#define ITM_TICKS                      622
#define ITM_TIME                       623
#define ITM_TIMER                      624
#define MNU_TIMES                      625
#define ITM_TN                         626
#define ITM_TONE                       627
#define ITM_TONtoKG                    628
#define ITM_TOP                        629
#define ITM_TORtoPA                    630
#define CST_53                         631
#define CST_54                         632
#define ITM_TPX                        633
#define ITM_TX                         634
#define ITM_TUX                        635
#define ITM_TM1P                       636
#define MNU_TRI                        637
#define ITM_TRZtoKG                    638
#define MNU_TVM                        639
#define MNU_T                          640
#define ITM_Tex                        641
#define ITM_ULP                        642
#define ITM_UN                         643
#define ITM_UNITV                      644
#define ITM_UNSIGN                     645
#define MNU_UNITCONV                   646
#define ITM_VARMNU                     647
#define MNU_VARS                       648
#define ITM_VERS                       649
#define ITM_VIEW                       650
#define CST_55                         651
#define MNU_CONVV                      652
#define ITM_WDAY                       653
#define ITM_WEIBLP                     654
#define ITM_WEIBL                      655
#define ITM_WEIBLU                     656
#define ITM_WEIBLM1                    657
#define MNU_WEIBL                      658
#define ITM_WHO                        659
#define ITM_WHtoJ                      660
#define ITM_WM                         661
#define ITM_WP                         662
#define ITM_WM1                        663
#define ITM_WSIZE                      664
#define ITM_WSIZEQ                     665
#define ITM_WtoHPE                     666
#define ITM_WtoHPM                     667
#define ITM_WtoHPUK                    668
#define ITM_SQUARE                     669
#define ITM_CUBE                       670
#define ITM_XEQ                        671
#define ITM_XNOR                       672
#define ITM_XOR                        673
#define ITM_XBAR                       674
#define ITM_XG                         675
#define ITM_XW                         676
#define ITM_XCIRC                      677
#define MNU_XFN                        678
#define ITM_XFACT                      679
#define MNU_CONVX                      680
#define ITM_XtoDATE                    681
#define ITM_XtoALPHA                   682
#define ITM_Xex                        683
#define ITM_XexY                       684
#define ITM_XEQU                       685
#define ITM_XNE                        686
#define ITM_XEQUP0                     687
#define ITM_XEQUM0                     688
#define ITM_XAEQU                      689
#define ITM_XLT                        690
#define ITM_XLE                        691
#define ITM_XGE                        692
#define ITM_XGT                        693
#define ITM_XTHROOT                    694
#define ITM_YDtoM                      695
#define ITM_YEAR                       696
#define ITM_YEARtoS                    697
#define ITM_YX                         698
#define ITM_YCIRC                      699
#define ITM_YMD                        700
#define ITM_Yex                        701
#define CST_56                         702
#define ITM_Zex                        703
#define CST_57                         704
#define MNU_ALPHAINTL                  705
#define ITM_ALPHALENG                  706
#define MNU_ALPHAMATH                  707
#define ITM_XMAX                       708
#define ITM_XMIN                       709
#define ITM_ALPHAPOS                   710
#define ITM_ALPHARL                    711
#define ITM_ALPHARR                    712
#define ITM_ALPHASL                    713
#define MNU_ALPHAFN                    714
#define MNU_ALPHA_OMEGA                715
#define MNU_ALPHADOT                   716
#define ITM_ALPHAtoX                   717
#define ITM_BETAXY                     718
#define CST_58                         719
#define CST_59                         720
#define CST_60                         721
#define ITM_gammaXY                    722
#define ITM_GAMMAXY                    723
#define ITM_GAMMAX                     724
#define ITM_deltaX                     725
#define ITM_DELTAPC                    726
#define ITM_epsilon                    727
#define CST_61                         728
#define ITM_epsilonM                   729
#define ITM_epsilonP                   730
#define ITM_zetaX                      731
#define CST_62                         732
#define CST_63                         733
#define CST_64                         734
#define CST_65                         735
#define CST_66                         736
#define CST_67                         737
#define CST_68                         738
#define CST_69                         739
#define CST_70                         740
#define CST_71                         741
#define CST_72                         742
#define ITM_PI                         743
#define ITM_pi                         744
#define ITM_SIGMA                      745
#define ITM_sigma                      746
#define CST_73                         747
#define ITM_SIGMAln2x                  748
#define ITM_SIGMAln2y                  749
#define ITM_SIGMAlnx                   750
#define ITM_SIGMAlnxy                  751
#define ITM_SIGMAlny                   752
#define ITM_sigmaw                     753
#define ITM_SIGMAx                     754
#define ITM_SIGMAx2                    755
#define ITM_SIGMAx2y                   756
#define ITM_SIGMAxlny                  757
#define ITM_SIGMAxy                    758
#define ITM_SIGMAy                     759
#define ITM_SIGMAy2                    760
#define ITM_SIGMAylnx                  761
#define ITM_SIGMAPLUS                  762
#define ITM_SIGMAMINUS                 763
#define CST_74                         764
#define CST_75                         765
#define ITM_RANI                       766
#define ITM_PRINTERX                   767
#define MNU_SYSFL                      768
#define ITM_RANGE                      769
#define ITM_GETRANGE                   770
#define ITM_chi2Px                     771
#define ITM_chi2x                      772
#define ITM_chi2ux                     773
#define ITM_chi2M1                     774
#define MNU_CHI2                       775
#define CST_76                         776
#define ITM_M1X                        777
#define ITM_ADD                        778
#define ITM_CHS                        779
#define ITM_SUB                        780
#define CST_77                         781
#define ITM_MULT                       782
#define ITM_XMOD                       783
#define ITM_DIV                        784
#define ITM_PMINFINITY                 785
#define ITM_INDIRECTION                786
#define ITM_toDATE                     787
#define ITM_toDEG                      788
#define ITM_toDMS                      789
#define ITM_toGRAD                     790
#define ITM_toHR                       791
#define ITM_toHMS                      792
#define ITM_toINT                      793   // JM #
#define ITM_toMULpi                    794
#define ITM_toPOL                      795
#define ITM_toRAD                      796
#define ITM_toREAL                     797
#define ITM_toREC                      798
#define ITM_DtoDMS                     799
#define ITM_ULIM                       800
#define ITM_DLIM                       801
#define ITM_ex                         802
#define ITM_PC                         803
#define ITM_PCMRR                      804
#define ITM_PCT                        805
#define ITM_PCSIGMA                    806
#define ITM_PCPMG                      807
#define ITM_SQUAREROOTX                808
#define ITM_INTEGRAL                   809
#define MNU_Sf                         810
#define MNU_Sfdx                       811
#define CST_78                         812
#define ITM_PMOD                       813
#define ITM_M_DET                      814
#define ITM_MAGNITUDE                  815
#define ITM_PARALLEL                   816
#define ITM_M_TRANSP                   817
#define ITM_M_INV                      818
#define ITM_ANGLE                      819
#define ITM_MULPIto                    820
#define MNU_ANGLECONV                  821
#define ITM_PRINTERADV                 822
#define ITM_PRINTERCHAR                823
#define ITM_PRINTERDLAY                824
#define ITM_PRINTERLCD                 825
#define ITM_PRINTERMODE                826
#define ITM_PRINTERPROG                827
#define ITM_PRINTERR                   828
#define ITM_PRINTERREGS                829
#define ITM_PRINTERSTK                 830
#define ITM_PRINTERTAB                 831
#define ITM_PRINTERUSER                832
#define ITM_PRINTERWIDTH               833
#define ITM_PRINTERSIGMA               834
#define ITM_PRINTERHASH                835
#define CST_79                         836
#define ITM_NUMB                       837

#define ITM_ACtoM2b                    838
#define ITM_ACUStoM2b                  839
#define ITM_CARATtoKG                  840
#define ITM_DBtoFRb                    841
#define ITM_DBtoFRc                    842
#define ITM_DBtoPRb                    843
#define ITM_DBtoPRc                    844
#define ITM_FRtoDBb                    845
#define ITM_FRtoDBc                    846
#define ITM_FTUStoMb                   847
#define ITM_FZUKtoM3b                  848
#define ITM_FZUStoM3b                  849
#define ITM_IHGtoPAb                   850
#define ITM_KGtoSCWb                   851
#define ITM_KGtoSTOb                   852
#define ITM_KGtoSTb                    853
#define ITM_KGtoSTc                    854
#define ITM_KGtoCARAT                  855
#define ITM_KGtoTRZb                   856
#define ITM_M2toACb                    857
#define ITM_M2toACUSb                  858
#define ITM_M3toFZUKb                  859
#define ITM_M3toFZUSb                  860
#define ITM_MtoFTUSb                   861
#define ITM_CARATtoKGb                 862
#define ITM_PAtoIHGb                   863
#define ITM_PAtoTORb                   864
#define ITM_PRtoDBb                    865
#define ITM_PRtoDBc                    866
#define ITM_SCWtoKGb                   867
#define ITM_STOtoKGb                   868
#define ITM_STtoKGb                    869
#define ITM_STtoKGc                    870
#define ITM_KGtoCARATb                 871
#define ITM_TORtoPAb                   872
#define ITM_TRZtoKGb                   873

#define ITM_FBR                        874
#define ITM_0875                       875
#define MNU_alpha_omega                876
#define MNU_ALPHAintl                  877
#define ITM_REG_X                      878
#define ITM_REG_Y                      879
#define ITM_REG_Z                      880
#define ITM_REG_T                      881
#define ITM_REG_A                      882
#define ITM_REG_B                      883
#define ITM_REG_C                      884
#define ITM_REG_D                      885
#define ITM_REG_L                      886
#define ITM_REG_I                      887
#define ITM_REG_J                      888
#define ITM_REG_K                      889

#define CHR_0                          890
#define CHR_1                          891
#define CHR_2                          892
#define CHR_3                          893
#define CHR_4                          894
#define CHR_5                          895
#define CHR_6                          896
#define CHR_7                          897
#define CHR_8                          898
#define CHR_9                          899
#define CHR_A                          900
#define CHR_B                          901
#define CHR_C                          902
#define CHR_D                          903
#define CHR_E                          904
#define CHR_F                          905
#define CHR_G                          906
#define CHR_H                          907
#define CHR_I                          908
#define CHR_J                          909
#define CHR_K                          910
#define CHR_L                          911
#define CHR_M                          912
#define CHR_N                          913
#define CHR_O                          914
#define CHR_P                          915
#define CHR_Q                          916
#define CHR_R                          917
#define CHR_S                          918
#define CHR_T                          919
#define CHR_U                          920
#define CHR_V                          921
#define CHR_W                          922
#define CHR_X                          923
#define CHR_Y                          924
#define CHR_Z                          925
#define CHR_a                          926
#define CHR_b                          927
#define CHR_c                          928
#define CHR_d                          929
#define CHR_e                          930
#define CHR_f                          931
#define CHR_g                          932
#define CHR_h                          933
#define CHR_i                          934
#define CHR_j                          935
#define CHR_k                          936
#define CHR_l                          937
#define CHR_m                          938
#define CHR_n                          939
#define CHR_o                          940
#define CHR_p                          941
#define CHR_q                          942
#define CHR_r                          943
#define CHR_s                          944
#define CHR_t                          945
#define CHR_u                          946
#define CHR_v                          947
#define CHR_w                          948
#define CHR_x                          949
#define CHR_y                          950
#define CHR_z                          951
#define CHR_ALPHA                      952   //NOTE the RANGE STARTS HERE, with +36 for lower case
#define ITM_0953                       953
#define CHR_BETA                       954
#define CHR_GAMMA                      955
#define CHR_DELTA                      956
#define CHR_EPSILON                    957
#define ITM_0958                       958
#define CHR_ZETA                       959
#define CHR_ETA                        960
#define CHR_0961                       961
#define CHR_THETA                      962
#define CHR_IOTA                       963
#define CHR_0964                       964
#define CHR_0965                       965
#define CHR_IOTA_DIALYTIKA             966
#define CHR_KAPPA                      967
#define CHR_LAMBDA                     968
#define CHR_MU                         969
#define CHR_NU                         970
#define CHR_XI                         971
#define CHR_OMICRON                    972
#define CHR_0973                       973
#define CHR_PI                         974
#define CHR_RHO                        975
#define CHR_SIGMA                      976
#define CHR_0977                       977
#define CHR_TAU                        978
#define CHR_UPSILON                    979
#define CHR_0980                       980
#define CHR_UPSILON_DIALYTIKA          981
#define CHR_0982                       982
#define CHR_PHI                        983
#define CHR_CHI                        984
#define CHR_PSI                        985
#define CHR_OMEGA                      986   //NOTE the RANGE STOPS HERE, with +36 for lower case
#define CHR_0987                       987
#define CHR_alpha                      988
#define CHR_alpha_TONOS                989
#define CHR_beta                       990
#define CHR_gamma                      991
#define CHR_delta                      992
#define CHR_epsilon                    993
#define CHR_epsilon_TONOS              994
#define CHR_zeta                       995
#define CHR_eta                        996
#define CHR_eta_TONOS                  997
#define CHR_theta                      998
#define CHR_iota                       999
#define CHR_iotaTON                   1000
#define CHR_iota_DIALYTIKA_TONOS      1001
#define CHR_iota_DIALYTIKA            1002
#define CHR_kappa                     1003
#define CHR_lambda                    1004
#define CHR_mu                        1005
#define CHR_nu                        1006
#define CHR_xi                        1007
#define CHR_omicron                   1008
#define CHR_omicron_TONOS             1009
#define CHR_pi                        1010
#define CHR_rho                       1011
#define CHR_sigma                     1012
#define CHR_sigma_end                 1013
#define CHR_tau                       1014
#define CHR_upsilon                   1015
#define CHR_upsilon_TONOS             1016
#define CHR_upsilon_DIALYTIKA         1017
#define CHR_upsilon_DIALYTIKA_TONOS   1018
#define CHR_phi                       1019
#define CHR_chi                       1020
#define CHR_psi                       1021
#define CHR_omega                     1022
#define CHR_omega_TONOS               1023
#define CHR_1024                      1024
#define CHR_1025                      1025
#define CHR_1026                      1026
#define CHR_1027                      1027
#define CHR_1028                      1028
#define CHR_1029                      1029
#define CHR_A_MACRON                  1030
#define CHR_A_ACUTE                   1031
#define CHR_A_BREVE                   1032
#define CHR_A_GRAVE                   1033
#define CHR_A_DIARESIS                1034
#define CHR_A_TILDE                   1035
#define CHR_A_CIRC                    1036
#define CHR_A_RING                    1037
#define CHR_AE                        1038
#define CHR_A_OGONEK                  1039
#define CHR_C_ACUTE                   1040
#define CHR_C_CARON                   1041
#define CHR_C_CEDILLA                 1042
#define CHR_D_STROKE                  1043
#define CHR_D_CARON                   1044
#define CHR_E_MACRON                  1045
#define CHR_E_ACUTE                   1046
#define CHR_E_BREVE                   1047
#define CHR_E_GRAVE                   1048
#define CHR_E_DIARESIS                1049
#define CHR_E_CIRC                    1050
#define CHR_E_OGONEK                  1051
#define CHR_G_BREVE                   1052
#define CHR_1053                      1053
#define CHR_I_MACRON                  1054
#define CHR_I_ACUTE                   1055
#define CHR_I_BREVE                   1056
#define CHR_I_GRAVE                   1057
#define CHR_I_DIARESIS                1058
#define CHR_I_CIRC                    1059
#define CHR_I_OGONEK                  1060
#define CHR_I_DOT                     1061
#define CHR_I_DOTLESS                 1062
#define CHR_L_STROKE                  1063
#define CHR_L_ACUTE                   1064
#define CHR_L_APOSTROPHE              1065
#define CHR_N_ACUTE                   1066
#define CHR_N_CARON                   1067
#define CHR_N_TILDE                   1068
#define CHR_O_MACRON                  1069
#define CHR_O_ACUTE                   1070
#define CHR_O_BREVE                   1071
#define CHR_O_GRAVE                   1072
#define CHR_O_DIARESIS                1073
#define CHR_O_TILDE                   1074
#define CHR_O_CIRC                    1075
#define CHR_O_STROKE                  1076
#define CHR_OE                        1077
#define CHR_1078                      1078
#define CHR_1079                      1079
#define CHR_S_SHARP                   1080
#define CHR_S_ACUTE                   1081
#define CHR_S_CARON                   1082
#define CHR_S_CEDILLA                 1083
#define CHR_T_CARON                   1084
#define CHR_T_CEDILLA                 1085
#define CHR_U_MACRON                  1086
#define CHR_U_ACUTE                   1087
#define CHR_U_BREVE                   1088
#define CHR_U_GRAVE                   1089
#define CHR_U_DIARESIS                1090
#define CHR_U_TILDE                   1091
#define CHR_U_CIRC                    1092
#define CHR_U_RING                    1093
#define CHR_W_CIRC                    1094
#define CHR_1095                      1095
#define CHR_1096                      1096
#define CHR_1097                      1097
#define CHR_Y_CIRC                    1098
#define CHR_Y_ACUTE                   1099
#define CHR_Y_DIARESIS                1100
#define CHR_Z_ACUTE                   1101
#define CHR_Z_CARON                   1102
#define CHR_Z_DOT                     1103
#define CHR_1104                      1104
#define CHR_1105                      1105
#define CHR_1106                      1106
#define CHR_1107                      1107
#define CHR_1108                      1108
#define CHR_1109                      1109
#define CHR_a_MACRON                  1110
#define CHR_a_ACUTE                   1111
#define CHR_a_BREVE                   1112
#define CHR_a_GRAVE                   1113
#define CHR_a_DIARESIS                1114
#define CHR_a_TILDE                   1115
#define CHR_a_CIRC                    1116
#define CHR_a_RING                    1117
#define CHR_ae                        1118
#define CHR_a_OGONEK                  1119
#define CHR_c_ACUTE                   1120
#define CHR_c_CARON                   1121
#define CHR_c_CEDILLA                 1122
#define CHR_d_STROKE                  1123
#define CHR_d_APOSTROPHE              1124
#define CHR_e_MACRON                  1125
#define CHR_e_ACUTE                   1126
#define CHR_e_BREVE                   1127
#define CHR_e_GRAVE                   1128
#define CHR_e_DIARESIS                1129
#define CHR_e_CIRC                    1130
#define CHR_e_OGONEK                  1131
#define CHR_g_BREVE                   1132
#define CHR_h_STROKE                  1133
#define CHR_i_MACRON                  1134
#define CHR_i_ACUTE                   1135
#define CHR_i_BREVE                   1136
#define CHR_i_GRAVE                   1137
#define CHR_i_DIARESIS                1138
#define CHR_i_CIRC                    1139
#define CHR_i_OGONEK                  1140
#define CHR_i_DOT                     1141
#define CHR_i_DOTLESS                 1142
#define CHR_l_STROKE                  1143
#define CHR_l_ACUTE                   1144
#define CHR_l_APOSTROPHE              1145
#define CHR_n_ACUTE                   1146
#define CHR_n_CARON                   1147
#define CHR_n_TILDE                   1148
#define CHR_o_MACRON                  1149
#define CHR_o_ACUTE                   1150
#define CHR_o_BREVE                   1151
#define CHR_o_GRAVE                   1152
#define CHR_o_DIARESIS                1153
#define CHR_o_TILDE                   1154
#define CHR_o_CIRC                    1155
#define CHR_o_STROKE                  1156
#define CHR_oe                        1157
#define CHR_r_CARON                   1158
#define CHR_r_ACUTE                   1159
#define CHR_s_SHARP                   1160
#define CHR_s_ACUTE                   1161
#define CHR_s_CARON                   1162
#define CHR_s_CEDILLA                 1163
#define CHR_t_APOSTROPHE              1164
#define CHR_t_CEDILLA                 1165
#define CHR_u_MACRON                  1166
#define CHR_u_ACUTE                   1167
#define CHR_u_BREVE                   1168
#define CHR_u_GRAVE                   1169
#define CHR_u_DIARESIS                1170
#define CHR_u_TILDE                   1171
#define CHR_u_CIRC                    1172
#define CHR_u_RING                    1173
#define CHR_w_CIRC                    1174
#define CHR_x_BAR                     1175
#define CHR_x_CIRC                    1176
#define CHR_y_BAR                     1177
#define CHR_y_CIRC                    1178
#define CHR_y_ACUTE                   1179
#define CHR_y_DIARESIS                1180
#define CHR_z_ACUTE                   1181
#define CHR_z_CARON                   1182
#define CHR_z_DOT                     1183
#define CHR_1184                      1184
#define CHR_1185                      1185
#define CHR_1186                      1186
#define CHR_1187                      1187
#define CHR_1188                      1188
#define CHR_1189                      1189

#define CHR_SUB_alpha                 1190
#define CHR_SUB_delta                 1191
#define CHR_SUB_mu                    1192
#define CHR_SUB_SUN                   1193
#define CHR_SUB_SUN_b                 1194
#define CHR_SUB_EARTH                 1195
#define CHR_SUB_EARTH_b               1196
#define CHR_SUB_PLUS                  1197
#define CHR_SUB_MINUS                 1198
#define CHR_SUB_INFINITY              1199
#define CHR_SUB_0                     1200
#define CHR_SUB_1                     1201
#define CHR_SUB_2                     1202
#define CHR_SUB_3                     1203
#define CHR_SUB_4                     1204
#define CHR_SUB_5                     1205
#define CHR_SUB_6                     1206
#define CHR_SUB_7                     1207
#define CHR_SUB_8                     1208
#define CHR_SUB_9                     1209
#define CHR_SUB_10                    1210
#define CHR_SUB_A                     1211
#define CHR_SUB_B                     1212
#define CHR_SUB_C                     1213
#define CHR_SUB_D                     1214
#define CHR_SUB_E                     1215
#define CHR_SUB_F                     1216
#define CHR_SUB_G                     1217
#define CHR_SUB_H                     1218
#define CHR_SUB_I                     1219
#define CHR_SUB_J                     1220
#define CHR_SUB_K                     1221
#define CHR_SUB_L                     1222
#define CHR_SUB_M                     1223
#define CHR_SUB_N                     1224
#define CHR_SUB_O                     1225
#define CHR_SUB_P                     1226
#define CHR_SUB_Q                     1227
#define CHR_SUB_R                     1228
#define CHR_SUB_S                     1229
#define CHR_SUB_T                     1230
#define CHR_SUB_U                     1231
#define CHR_SUB_V                     1232
#define CHR_SUB_W                     1233
#define CHR_SUB_X                     1234
#define CHR_SUB_Y                     1235
#define CHR_SUB_Z                     1236
#define CHR_SUB_E_OUTLINE             1237
#define CHR_SUB_a                     1238
#define CHR_SUB_b                     1239
#define CHR_SUB_c                     1240
#define CHR_SUB_d                     1241
#define CHR_SUB_e                     1242
#define CHR_SUB_h                     1243
#define CHR_SUB_i                     1244
#define CHR_SUB_j                     1245
#define CHR_SUB_k                     1246
#define CHR_SUB_l                     1247
#define CHR_SUB_m                     1248
#define CHR_SUB_n                     1249
#define CHR_SUB_o                     1250
#define CHR_SUB_p                     1251
#define CHR_SUB_q                     1252
#define CHR_SUB_s                     1253
#define CHR_SUB_t                     1254
#define CHR_SUB_u                     1255
#define CHR_SUB_v                     1256
#define CHR_SUB_w                     1257
#define CHR_SUB_x                     1258
#define CHR_SUB_y                     1259
#define CHR_SUB_z                     1260
#define CHR_SUB_a_b                   1261
#define CHR_SUB_e_b                   1262
#define CHR_SUB_k_b                   1263
#define CHR_SUB_l_b                   1264
#define CHR_SUB_m_b                   1265
#define CHR_SUB_n_b                   1266
#define CHR_SUB_o_b                   1267
#define CHR_SUB_p_b                   1268
#define CHR_SUB_s_b                   1269
#define CHR_SUB_u_b                   1270
#define CHR_SUB_x_b                   1271

#define CHR_SUP_PLUS                  1272
#define CHR_SUP_MINUS                 1273
#define CHR_SUP_MINUS_1               1274
#define CHR_SUP_INFINITY              1275
#define CHR_SUP_ASTERISK              1276
#define CHR_SUP_0                     1277
#define CHR_SUP_1                     1278
#define CHR_SUP_2                     1279
#define CHR_SUP_3                     1280
#define CHR_SUP_4                     1281
#define CHR_SUP_5                     1282
#define CHR_SUP_6                     1283
#define CHR_SUP_7                     1284
#define CHR_SUP_8                     1285
#define CHR_SUP_9                     1286
#define CHR_1287                      1287
#define CHR_SUP_T                     1288
#define CHR_SUP_a                     1289
#define CHR_SUP_f                     1290
#define CHR_SUP_g                     1291
#define CHR_SUP_h                     1292
#define CHR_SUP_r                     1293
#define CHR_SUP_x                     1294

#define CHR_SPACE                     1295
#define CHR_EXCLAMATION_MARK          1296
#define CHR_DOUBLE_QUOTE              1297
#define CHR_NUMBER_SIGN               1298
#define CHR_DOLLAR                    1299
#define CHR_PERCENT                   1300
#define CHR_AMPERSAND                 1301
#define CHR_QUOTE                     1302
#define CHR_LEFT_PARENTHESIS          1303
#define CHR_RIGHT_PARENTHESIS         1304
#define CHR_ASTERISK                  1305
#define CHR_ASTERISK_b                1306
#define CHR_PLUS                      1307
#define CHR_COMMA                     1308
#define CHR_MINUS                     1309
#define CHR_PERIOD                    1310
#define CHR_SLASH                     1311
#define CHR_COLON                     1312
#define CHR_SEMICOLON                 1313
#define CHR_LESS_THAN                 1314
#define CHR_EQUAL                     1315
#define CHR_GREATER_THAN              1316
#define CHR_QUESTION_MARK             1317
#define CHR_AT                        1318
#define CHR_LEFT_SQUARE_BRACKET       1319
#define CHR_BACK_SLASH                1320
#define CHR_RIGHT_SQUARE_BRACKET      1321
#define CHR_CIRCUMFLEX                1322
#define CHR_UNDERSCORE                1323
#define CHR_LEFT_CURLY_BRACKET        1324
#define CHR_PIPE                      1325
#define CHR_RIGHT_CURLY_BRACKET       1326
#define CHR_TILDE                     1327
#define CHR_INVERTED_EXCLAMATION_MARK 1328
#define CHR_CENT                      1329
#define CHR_POUND                     1330
#define CHR_YEN                       1331
#define CHR_SECTION                   1332
#define CHR_OVERFLOW_CARRY            1333
#define CHR_LEFT_DOUBLE_ANGLE         1334
#define CHR_NOT                       1335
#define CHR_DEGREE                    1336
#define CHR_PLUS_MINUS                1337
#define CHR_mu_b                      1338
#define CHR_DOT                       1339
#define CHR_ORDINAL                   1340
#define CHR_RIGHT_DOUBLE_ANGLE        1341
#define CHR_ONE_HALF                  1342
#define CHR_ONE_QUARTER               1343
#define CHR_INVERTED_QUESTION_MARK    1344
#define CHR_ETH                       1345
#define CHR_CROSS                     1346
#define CHR_eth                       1347
#define CHR_DIVIDE                    1348
#define CHR_E_DOT                     1349
#define CHR_e_DOT                     1350
#define CHR_E_CARON                   1351
#define CHR_e_CARON                   1352
#define CHR_R_ACUTE                   1353
#define CHR_R_CARON                   1354
#define CHR_U_OGONEK                  1355
#define CHR_u_OGONEK                  1356
#define CHR_y_UNDER_ROOT              1357
#define CHR_x_UNDER_ROOT              1358
#define CHR_SPACE_EM                  1359
#define CHR_SPACE_3_PER_EM            1360
#define CHR_SPACE_4_PER_EM            1361
#define CHR_SPACE_6_PER_EM            1362
#define CHR_SPACE_FIGURE              1363
#define CHR_SPACE_PUNCTUATION         1364
#define CHR_SPACE_HAIR                1365
#define CHR_LEFT_SINGLE_QUOTE         1366
#define CHR_RIGHT_SINGLE_QUOTE        1367
#define CHR_SINGLE_LOW_QUOTE          1368
#define CHR_SINGLE_HIGH_QUOTE         1369
#define CHR_LEFT_DOUBLE_QUOTE         1370
#define CHR_RIGHT_DOUBLE_QUOTE        1371
#define CHR_DOUBLE_LOW_QUOTE          1372
#define CHR_DOUBLE_HIGH_QUOTE         1373
#define CHR_ELLIPSIS                  1374
#define CHR_ONE                       1375
#define CHR_EURO                      1376
#define CHR_COMPLEX_C                 1377
#define CHR_PLANCK                    1378
#define CHR_PLANCK_2PI                1379
#define CHR_NATURAL_N                 1380
#define CHR_RATIONAL_Q                1381
#define CHR_REAL_R                    1382
#define CHR_LEFT_ARROW                1383
#define CHR_UP_ARROW                  1384
#define CHR_1385                      1385
#define CHR_RIGHT_ARROW               1386
#define CHR_DOWN_ARROW                1387
#define CHR_1388                      1388
#define CHR_SERIAL_IO                 1389
#define CHR_RIGHT_SHORT_ARROW         1390
#define CHR_LEFT_RIGHT_ARROWS         1391
#define CHR_BST                       1392
#define CHR_SST                       1393
#define CHR_HAMBURGER                 1394
#define CHR_UNDO                      1395
#define CHR_FOR_ALL                   1396
#define CHR_COMPLEMENT                1397
#define CHR_PARTIAL_DIFF              1398
#define CHR_THERE_EXISTS              1399
#define CHR_THERE_DOES_NOT_EXIST      1400
#define CHR_EMPTY_SET                 1401
#define CHR_INCREMENT                 1402
#define CHR_NABLA                     1403
#define CHR_ELEMENT_OF                1404
#define CHR_NOT_ELEMENT_OF            1405
#define CHR_CONTAINS                  1406
#define CHR_DOES_NOT_CONTAIN          1407
#define CHR_ZERO                      1408
#define CHR_PRODUCT                   1409
#define CHR_SUM                       1410
#define CHR_MINUS_SIGN                1411
#define CHR_MINUS_PLUS                1412
#define CHR_DIVISION                  1413
#define CHR_SET_MINUS                 1414
#define CHR_RING                      1415
#define CHR_BULLET                    1416
#define CHR_SQUARE_ROOT               1417
#define CHR_CUBE_ROOT                 1418
#define CHR_xTH_ROOT                  1419
#define CHR_PROPORTIONAL              1420
#define CHR_INFINITY                  1421
#define CHR_RIGHT_ANGLE               1422
#define CHR_ANGLE                     1423
#define CHR_MEASURED_ANGLE            1424
#define CHR_DIVIDES                   1425
#define CHR_DOES_NOT_DIVIDE           1426
#define CHR_PARALLEL                  1427
#define CHR_NOT_PARALLEL              1428
#define CHR_AND                       1429
#define CHR_OR                        1430
#define CHR_INTERSECTION              1431
#define CHR_UNION                     1432
#define CHR_INTEGRAL                  1433
#define CHR_DOUBLE_INTEGRAL           1434
#define CHR_CONTOUR_INTEGRAL          1435
#define CHR_SURFACE_INTEGRAL          1436
#define CHR_RATIO                     1437
#define CHR_CHECK_MARK                1438
#define CHR_ASYMPOTICALLY_EQUAL       1439
#define CHR_ALMOST_EQUAL              1440
#define CHR_COLON_EQUALS              1441
#define CHR_CORRESPONDS_TO            1442
#define CHR_ESTIMATES                 1443
#define CHR_NOT_EQUAL                 1444
#define CHR_IDENTICAL_TO              1445
#define CHR_LESS_EQUAL                1446
#define CHR_GREATER_EQUAL             1447
#define CHR_MUCH_LESS                 1448
#define CHR_MUCH_GREATER              1449
#define CHR_SUN                       1450
#define CHR_DOWN_TACK                 1451
#define CHR_PERPENDICULAR             1452
#define CHR_XOR                       1453
#define CHR_NAND                      1454
#define CHR_NOR                       1455
#define CHR_WATCH                     1456
#define CHR_HOURGLASS                 1457
#define CHR_PRINTER                   1458
#define CHR_MAT_TL                    1459
#define CHR_MAT_ML                    1460
#define CHR_MAT_BL                    1461
#define CHR_MAT_TR                    1462
#define CHR_MAT_MR                    1463
#define CHR_MAT_BR                    1464
#define CHR_OBLIQUE1                  1465
#define CHR_OBLIQUE2                  1466
#define CHR_OBLIQUE3                  1467
#define CHR_OBLIQUE4                  1468
#define CHR_CURSOR                    1469
#define CHR_PERIOD34                  1470
#define CHR_COMMA34                   1471
#define CHR_BATTERY                   1472
#define CHR_PGM_BEGIN                 1473
#define CHR_USER_MODE                 1474
#define CHR_UK                        1475
#define CHR_US                        1476
#define CHR_NEG_EXCLAMATION_MARK      1477
#define CHR_ex                        1478

#define ITM_Max                       1479
#define ITM_Min                       1480
#define ITM_Config                    1481
#define ITM_Stack                     1482
#define ITM_dddEL                     1483
#define ITM_dddIJ                     1484
#define ITM_0P                        1485
#define ITM_1P                        1486
#define ITM_EXPONENT                  1487
#define ITM_MA11                      1488
#define ITM_MA12                      1489
#define ITM_MA13                      1490
#define ITM_MA14                      1491
#define ITM_MA15                      1492
#define ITM_MA16                      1493
#define ITM_MA21                      1494
#define ITM_MA22                      1495
#define ITM_MA23                      1496
#define ITM_MA24                      1497
#define ITM_MA25                      1498
#define ITM_MA26                      1499
#define ITM_MA31                      1500
#define ITM_MA32                      1501
#define ITM_MA33                      1502
#define ITM_MA34                      1503
#define ITM_MA35                      1504
#define ITM_MA36                      1505
#define KEY_UNDO                      1506
#define ITM_PR                        1507
#define ITM_RS                        1508
#define ITM_Not                       1509
#define ITM_yet                       1510
#define ITM_defined                   1511
#define MNU_TAM                       1512
#define MNU_TAMCMP                    1513
#define MNU_TAMSTORCL                 1514
#define KEY_USERMODE                  1515
#define KEY_CC                        1516
#define KEY_f                         1517
#define KEY_g                         1518
#define KEY_UP1                       1519
#define KEY_BST                       1520
#define KEY_DOWN1                     1521
#define KEY_SST                       1522
#define KEY_EXIT1                     1523
#define KEY_BACKSPACE                 1524
#define KEY_PRTX                      1525
#define ITM_AIM                       1526
#define KEY_dotD                      1527
#define ITM_QTtoM3                    1528
#define ITM_M3toQT                    1529
#define ITM_toSP                      1530
#define ITM_SHOW                      1531
#define ITM_SYSTEM                    1532
#define ITM_DMStoD                    1533
#define ITM_VANGLE                    1534
#define ITM_FATHOMtoM                 1535
#define ITM_FATHOMtoMb                1536
#define ITM_MtoFATHOM                 1537
#define ITM_MtoFATHOMb                1538
#define ITM_FTUStoMc                  1539
#define ITM_MtoFTUSc                  1540
#define ITM_MtoPOINTb                 1541
#define ITM_POINTtoMb                 1542
#define ITM_BARRELtoM3                1543
#define ITM_BARRELtoM3b               1544
#define ITM_M3toBARREL                1545
#define ITM_M3toBARRELb               1546
#define ITM_TONtoKGb                  1547
#define ITM_TONtoKGc                  1548
#define ITM_KGtoTONb                  1549
#define ITM_KGtoTONc                  1550
#define ITM_CARATtoKGc                1551
#define ITM_KGtoCARATc                1552
#define ITM_ATMtoPAb                  1553
#define ITM_PAtoATMb                  1554
#define ITM_FATHOMtoMc                1555
#define ITM_MtoFATHOMc                1556
#define ITM_POINTtoMc                 1557
#define ITM_MtoPOINTc                 1558
#define ITM_XH                        1559
#define ITM_XRMS                      1560
#define MNU_SUMS                      1561
#define ITM_GAUSSF                    1562
#define ITM_CAUCHF                    1563
#define ITM_PARABF                    1564
#define ITM_HYPF                      1565
#define ITM_ROOTF                     1566
#define ITM_SIGMAlnyonx               1567
#define ITM_SIGMAx2ony                1568
#define ITM_SIGMA1onx                 1569
#define ITM_SIGMA1onx2                1570
#define ITM_SIGMAxony                 1571
#define ITM_SIGMA1ony                 1572
#define ITM_SIGMA1ony2                1573
#define ITM_SIGMAx3                   1574
#define ITM_SIGMAx4                   1575
#define ITM_HEX                       1576
#define ITM_IDIVR                     1577
#define ITM_ACOS                      1578
#define ITM_ASIN                      1579
#define ITM_ATAN                      1580
#define ITM_DET                       1581
#define ITM_INVRT                     1582
#define ITM_TRANS                     1583
#define ITM_XIN                       1584
#define ITM_XOUT                      1585
#define ITM_ALPHASR                   1586
#define ITM_HECTAREtoM2               1587
#define ITM_M2toHECTARE               1588
#define MNU_VAR                       1589
#define MNU_TAMFLAG                   1590
#define ITM_1591                      1591
#define SFL_TDM24                     1592
#define SFL_YMD                       1593
#define SFL_DMY                       1594
#define SFL_MDY                       1595
#define SFL_CPXRES                    1596
#define SFL_CPXj                      1597
#define SFL_POLAR                     1598
#define SFL_FRACT                     1599
#define SFL_PROPFR                    1600
#define SFL_DENANY                    1601
#define SFL_DENFIX                    1602
#define SFL_CARRY                     1603
#define SFL_OVERFL                    1604
#define SFL_LEAD0                     1605
#define SFL_ALPHA                     1606
#define SFL_alphaCAP                  1607
#define SFL_RUNTIM                    1608
#define SFL_RUNIO                     1609
#define SFL_PRINT                     1610
#define SFL_TRACE                     1611
#define SFL_USER                      1612
#define SFL_LOWBAT                    1613
#define SFL_SLOW                      1614
#define SFL_SPCRES                    1615
#define SFL_SSIZE8                    1616
#define SFL_QUIET                     1617
#define SFL_DECIMP                    1618
#define SFL_MULTx                     1619
#define SFL_ALLENG                    1620
#define SFL_GROW                      1621
#define SFL_AUTOFF                    1622
#define SFL_AUTXEQ                    1623
#define SFL_PRTACT                    1624
#define SFL_NUMIN                     1625
#define SFL_ALPIN                     1626
#define SFL_ASLIFT                    1627
#define SFL_IGN1ER                    1628
#define SFL_INTING                    1629
#define SFL_SOLVING                   1630
#define SFL_VMDISP                    1631
#define MNU_TAMSHUFFLE                1632
#define Not_used1633                  1633
#define Not_used1634                  1634
#define Not_used1635                  1635
#define Not_used1636                  1636
#define Not_used1637                  1637
#define Not_used1638                  1638
#define Not_used1639                  1639
#define Not_used1640                  1640
#define Not_used1641                  1641
#define Not_used1642                  1642
#define Not_used1643                  1643
#define Not_used1644                  1644
#define Not_used1645                  1645
#define Not_used1646                  1646
#define Not_used1647                  1647
#define Not_used1648                  1648
#define Not_used1649                  1649
#define Not_used1650                  1650
#define Not_used1651                  1651
#define Not_used1652                  1652
#define Not_used1653                  1653
#define Not_used1654                  1654
#define Not_used1655                  1655
#define Not_used1656                  1656
#define Not_used1657                  1657
#define Not_used1658                  1658
#define Not_used1659                  1659
#define Not_used1660                  1660
#define Not_used1661                  1661
#define Not_used1662                  1662
#define Not_used1663                  1663
#define Not_used1664                  1664
#define Not_used1665                  1665
#define Not_used1666                  1666
#define Not_used1667                  1667
#define Not_used1668                  1668
#define Not_used1669                  1669
#define Not_used1670                  1670
#define Not_used1671                  1671
#define Not_used1672                  1672
#define Not_used1673                  1673


#define Not_used1674                  1674
#define Not_used1675                  1675
#define Not_used1676                  1676
#define ITM_SYS_FREE_RAM              1677   //dr

#define ITM_ERPN                      1678   //JM eRPN
#define ITM_HOMEx3                    1679   //JM HOME.3
#define ITM_SHTIM                     1680   //JM SHIFT CANCEL
#define MNU_HOME                      1681   //JM HOME
#define ITM_SIGFIG                    1682   //JM SIGFIG
#define MNU_ALPHA                     1683   //JM ALPHA
#define MNU_BASE                      1684   //JM BASE
#define ITM_2BIN                      1685   //JM BASE
#define ITM_2OCT                      1686   //JM BASE
#define ITM_2DEC                      1687   //JM BASE
#define ITM_2HEX                      1688   //JM BASE
#define ITM_WS8                       1689   //JM BASE
#define ITM_WS16                      1690   //JM BASE
#define ITM_WS32                      1691   //JM BASE
#define ITM_WS64                      1692   //JM BASE
#define ITM_UNIT                      1693   //JM UNIT
#define ITM_SH_ERPN                   1694   //JM SHOW
#define ITM_CB_CPXRES                 1695   //JM cb ComplexResult
#define ITM_CB_LEADING_ZERO           1696   //JM cb LeadingZeros
#define CHR_QOPPA                     1697   //JM GREEK   //NOTE the RANGE STARTS HERE, with +36 for lower case
#define CHR_DIGAMMA                   1698   //JM GREEK
#define CHR_SAMPI                     1699   //JM GREEK   //NOTE the RANGE STOPS HERE, with +36 for lower case
#define CHR_1599                      1700   //JM SPARE
#define CHR_1600                      1701   //JM SPARE
#define CHR_1601                      1702   //JM SPARE
#define CHR_1602                      1703   //JM SPARE
#define CHR_1603                      1704   //JM SPARE
#define CHR_1604                      1705   //JM SPARE
#define CHR_1605                      1706   //JM SPARE
#define CHR_1606                      1707   //JM SPARE
#define CHR_1607                      1708   //JM SPARE
#define CHR_1608                      1709   //JM SPARE
#define CHR_1609                      1710   //JM SPARE
#define CHR_1610                      1711   //JM SPARE
#define CHR_1611                      1712   //JM SPARE
#define CHR_1612                      1713   //JM SPARE
#define CHR_1613                      1714   //JM SPARE
#define CHR_1614                      1715   //JM SPARE
#define CHR_1615                      1716   //JM SPARE
#define CHR_1616                      1717   //JM SPARE
#define CHR_1617                      1718   //JM SPARE
#define CHR_1618                      1719   //JM SPARE
#define CHR_1619                      1720   //JM SPARE
#define CHR_1620                      1721   //JM SPARE
#define CHR_1621                      1722   //JM SPARE
#define CHR_1622                      1723   //JM SPARE
#define CHR_1623                      1724   //JM SPARE
#define CHR_1624                      1725   //JM SPARE
#define CHR_1625                      1726   //JM SPARE
#define CHR_1626                      1727   //JM SPARE
#define CHR_1627                      1728   //JM SPARE
#define CHR_1628                      1729   //JM SPARE
#define CHR_1629                      1730   //JM SPARE
#define CHR_1630                      1731   //JM SPARE
#define CHR_1631                      1732   //JM SPARE
#define CHR_qoppa                     1733   //JM GREEK
#define CHR_digamma                   1734   //JM GREEK
#define CHR_sampi                     1735   //JM GREEK
#define CHR_case                      1736   //JM CAPS
#define KEY_HASH                      1737   //JM BASE ## test
#define MNU_XEQ                       1738   //JM EXEC
#define ITM_op_a                      1739   //JM OPERATORS
#define ITM_op_a2                     1740   //JM OPERATORS
#define ITM_op_j                      1741   //JM OPERATORS
#define ITM_BASE_HOME                 1742   //JM BASEMENU
#define ITM_PGMTST                    1743   //JM Generic program test
#define ITM_BASE_AHOME                1744   //JM BASEMENU
#define ITM_H_SUMRY                   1745   //JMHOME
#define ITM_H_REPLCA                  1746   //JMHOME
#define ITM_H_FIXED                   1747   //JMHOME
#define ITM_CB_SPCRES                 1748
#define ITM_USER_V43                  1749   //V43
#define ITM_EE_D2Y                    1750   //JM EE
#define ITM_EE_Y2D                    1751   //JM EE
#define ITM_EE_A2S                    1752   //JM EE
#define ITM_EE_S2A                    1753   //JM EE
#define MNU_EE                        1754   //JM EE
#define ITM_EE_EXP_TH                 1755   //JM EE
#define ITM_EE_STO_Z                  1756   //JM EE
#define ITM_EE_RCL_Z                  1757   //JM EE
#define ITM_EE_STO_V                  1758   //JM EE
#define ITM_EE_RCL_V                  1759   //JM EE
#define ITM_EE_STO_I                  1760   //JM EE
#define ITM_EE_RCL_I                  1761   //JM EE
#define ITM_EE_STO_V_I                1762   //JM EE
#define ITM_EE_STO_IR                 1763   //JM EE
#define ITM_EE_STO_V_Z                1764   //JM EE
#define ITM_EE_X2BAL                  1765   //JM EE
#define KEY_COMPLEX                   1766   //JM CPX
#define ITM_RNG                       1767   //JM RANGE
#define KEY_TYPCON_UP                 1768   //JM CPX
#define KEY_TYPCON_DN                 1769   //JM CPX
#define ITM_HOMEx3T                   1770   //JM HOME.3T
#define STAT_DEMO                     1771   //LOAD STATS GRAPH

#define MNU_ASN                       1772   //JM USER
#define K_00U                         1773   //JM USER MODE TEST
#define Kf00U                         1774   //JM USER MODE TEST
#define Kg00U                         1775   //JM USER MODE TEST
#define K_01U                         1776   //JM USER MODE TEST
#define Kf01U                         1777   //JM USER MODE TEST
#define Kg01U                         1778   //JM USER MODE TEST
#define K_02U                         1779   //JM USER MODE TEST
#define Kf02U                         1780   //JM USER MODE TEST
#define Kg02U                         1781   //JM USER MODE TEST
#define K_03U                         1782   //JM USER MODE TEST
#define Kf03U                         1783   //JM USER MODE TEST
#define Kg03U                         1784   //JM USER MODE TEST
#define K_04U                         1785   //JM USER MODE TEST
#define Kf04U                         1786   //JM USER MODE TEST
#define Kg04U                         1787   //JM USER MODE TEST
#define K_05U                         1788   //JM USER MODE TEST
#define Kf05U                         1789   //JM USER MODE TEST
#define Kg05U                         1790   //JM USER MODE TEST
#define K_06U                         1791   //JM USER MODE TEST
#define Kf06U                         1792   //JM USER MODE TEST
#define Kg06U                         1793   //JM USER MODE TEST
#define K_07U                         1794   //JM USER MODE TEST
#define Kf07U                         1795   //JM USER MODE TEST
#define Kg07U                         1796   //JM USER MODE TEST
#define K_08U                         1797   //JM USER MODE TEST
#define Kf08U                         1798   //JM USER MODE TEST
#define Kg08U                         1799   //JM USER MODE TEST
#define K_09U                         1800   //JM USER MODE TEST
#define Kf09U                         1801   //JM USER MODE TEST
#define Kg09U                         1802   //JM USER MODE TEST
#define K_10U                         1803   //JM USER MODE TEST
#define Kf10U                         1804   //JM USER MODE TEST
#define Kg10U                         1805   //JM USER MODE TEST
#define K_11U                         1806   //JM USER MODE TEST
#define Kf11U                         1807   //JM USER MODE TEST
#define Kg11U                         1808   //JM USER MODE TEST
#define K_12U                         1809   //JM USER MODE TEST
#define Kf12U                         1810   //JM USER MODE TEST
#define Kg12U                         1811   //JM USER MODE TEST
#define K_13U                         1812   //JM USER MODE TEST
#define Kf13U                         1813   //JM USER MODE TEST
#define Kg13U                         1814   //JM USER MODE TEST
#define K_14U                         1815   //JM USER MODE TEST
#define Kf14U                         1816   //JM USER MODE TEST
#define Kg14U                         1817   //JM USER MODE TEST
#define K_15U                         1818   //JM USER MODE TEST
#define Kf15U                         1819   //JM USER MODE TEST
#define Kg15U                         1820   //JM USER MODE TEST
#define K_16U                         1821   //JM USER MODE TEST
#define Kf16U                         1822   //JM USER MODE TEST
#define Kg16U                         1823   //JM USER MODE TEST
#define K_17U                         1824   //JM USER MODE TEST
#define Kf17U                         1825   //JM USER MODE TEST
#define Kg17U                         1826   //JM USER MODE TEST
#define K_18U                         1827   //JM USER MODE TEST
#define Kf18U                         1828   //JM USER MODE TEST
#define Kg18U                         1829   //JM USER MODE TEST
#define K_19U                         1830   //JM USER MODE TEST
#define Kf19U                         1831   //JM USER MODE TEST
#define Kg19U                         1832   //JM USER MODE TEST
#define K_20U                         1833   //JM USER MODE TEST
#define Kf20U                         1834   //JM USER MODE TEST
#define Kg20U                         1835   //JM USER MODE TEST
#define K_21U                         1836   //JM USER MODE TEST
#define Kf21U                         1837   //JM USER MODE TEST
#define Kg21U                         1838   //JM USER MODE TEST
#define K_22U                         1839   //JM USER MODE TEST
#define Kf22U                         1840   //JM USER MODE TEST
#define Kg22U                         1841   //JM USER MODE TEST
#define K_23U                         1842   //JM USER MODE TEST
#define Kf23U                         1843   //JM USER MODE TEST
#define Kg23U                         1844   //JM USER MODE TEST
#define K_24U                         1845   //JM USER MODE TEST
#define Kf24U                         1846   //JM USER MODE TEST
#define Kg24U                         1847   //JM USER MODE TEST
#define K_25U                         1848   //JM USER MODE TEST
#define Kf25U                         1849   //JM USER MODE TEST
#define Kg25U                         1850   //JM USER MODE TEST
#define K_26U                         1851   //JM USER MODE TEST
#define Kf26U                         1852   //JM USER MODE TEST
#define Kg26U                         1853   //JM USER MODE TEST
#define K_27U                         1854   //JM USER MODE TEST
#define Kf27U                         1855   //JM USER MODE TEST
#define Kg27U                         1856   //JM USER MODE TEST
#define K_28U                         1857   //JM USER MODE TEST
#define Kf28U                         1858   //JM USER MODE TEST
#define Kg28U                         1859   //JM USER MODE TEST
#define K_29U                         1860   //JM USER MODE TEST
#define Kf29U                         1861   //JM USER MODE TEST
#define Kg29U                         1862   //JM USER MODE TEST
#define K_30U                         1863   //JM USER MODE TEST
#define Kf30U                         1864   //JM USER MODE TEST
#define Kg30U                         1865   //JM USER MODE TEST
#define K_31U                         1866   //JM USER MODE TEST
#define Kf31U                         1867   //JM USER MODE TEST
#define Kg31U                         1868   //JM USER MODE TEST
#define K_32U                         1869   //JM USER MODE TEST
#define Kf32U                         1870   //JM USER MODE TEST
#define Kg32U                         1871   //JM USER MODE TEST
#define K_33U                         1872   //JM USER MODE TEST
#define Kf33U                         1873   //JM USER MODE TEST
#define Kg33U                         1874   //JM USER MODE TEST
#define K_34U                         1875   //JM USER MODE TEST
#define Kf34U                         1876   //JM USER MODE TEST
#define Kg34U                         1877   //JM USER MODE TEST
#define K_35U                         1878   //JM USER MODE TEST
#define Kf35U                         1879   //JM USER MODE TEST
#define Kg35U                         1880   //JM USER MODE TEST
#define K_36U                         1881   //JM USER MODE TEST
#define Kf36U                         1882   //JM USER MODE TEST
#define Kg36U                         1883   //JM USER MODE TEST


#define KEY_fg                        1884
#define ITM_USER_DEFAULTS             1885   //JM USER
#define ITM_USER_COMPLEX              1886   //JM USER
#define ITM_USER_SHIFTS               1887   //JM USER
#define ITM_USER_RESET                1888   //JM USER
#define ITM_U_KEY_USER                1889   //JM USER
#define MNU_INL_TST                   1890   //INLINE_TEST
#define ITM_U_KEY_CC                  1891   //JM USER
#define ITM_TEST                      1892   //INLINE_TEST
#define ITM_U_KEY_MM                  1893   //JM USER
#define ITM_GET_TEST_BS               1894   //INLINE_TEST
#define ITM_U_KEY_SIGMA               1895   //JM USER
#define ITM_SET_TEST_BS               1896   //INLINE_TEST
#define ITM_U_KEY_PRGM                1897   //JM USER
#define ITM_RI                        1898   //JM ROUND
#define ITM_U_KEY_ALPHA               1899   //JM USER
#define ITM_SH_NORM_E                 1900   //JM USER NORMAL
#define ITM_JM_ASN                    1901   //JM ASSIGN
#define ITM_JM_SEEK                   1902   //JM ASSIGN
#define MNU_ASN_N                     1903   //JM USER NORMAL
#define ITM_GET_NORM_E                1904   //JM USER NORMAL
#define ITM_INP_DEF_43S               1905   //JM INPUT DEFAULT
#define ITM_FG_LINE                   1906   //JM FG              **
#define ITM_INP_DEF_DP                1907   //JM INPUT DEFAULT
#define ITM_SH_INP_DEF                1908   //JM INPUT DEFAULT
#define ITM_FG_DOTS                   1909   //JM FG              **
#define ITM_INP_DEF_CPXDP             1910   //JM INPUT DEFAULT
#define ITM_G_DOUBLETAP               1911   //JM FG              **
#define ITM_INP_DEF_SI                1912   //JM INPUT DEFAULT
#define ITM_INP_DEF_LI                1913   //JM INPUT DEFAULT
#define ITM_GRAPH                     1914   //JM GRAPHING
#define MNU_A_Z                       1915   //  34
#define MNU_a_z                       1916   //  875
#define MNU_GRAPH                     1917   //JM GRAPH
#define ITM_GRF_X0                    1918   //JM GRAPH
#define ITM_GRF_X1                    1919   //JM GRAPH
#define ITM_GRF_Y0                    1920   //JM GRAPH
#define ITM_GRF_Y1                    1921   //JM GRAPH
#define ITM_GRF_DX                    1922   //JM GRAPH
#define ITM_GRF_DY                    1923   //JM GRAPH
#define ITM_GRF_HLP                   1924   //JM GRAPH
#define ITM_toSI                      1925   //JM SHORTINT
#define ITM_ms                        1926   //JM DMS HMS
#define ITM_SI_p                      1927   //JM PRE UNIT
#define ITM_SI_n                      1928   //JM PRE UNIT
#define ITM_SI_u                      1929   //JM PRE UNIT
#define ITM_SI_m                      1930   //JM PRE UNIT
#define ITM_SI_k                      1931   //JM PRE UNIT
#define ITM_SI_M                      1932   //JM PRE UNIT
#define ITM_USER_ALPHA                1933   //JM USER
#define ITM_USER_GSHFT                1934   //JM USER
#define ITM_DOTDEMU                   1935   //JM USER
#define ITM_DEMO1                     1936   //JM USER
#define ITM_DEMO2                     1937   //JM USER
#define ITM_DEMO3                     1938   //JM USER
#define ITM_DEMO4                     1939   //JM USER
#define ITM_DEMO5                     1940   //JM USER
#define ITM_DEMO6                     1941   //JM USER
#define MNU_ST_GRAPH                  1942   //JM USER
#define ITM_VECT                      1943   //JMVECT
#define ITM_LARGELI                   1944
#define ITM_P_ALLREGS                 1945   //JM Print all regs
#define ITM_SI_f                      1946   //JM PRE UNIT
#define ITM_SI_G                      1947   //JM PRE UNIT
#define ITM_SI_T                      1948   //JM PRE UNIT
#define ITM_USER_CC                   1949   //JM USER
#define ITM_USER_MYM                  1950   //JM USER
#define ITM_USER_PRGM                 1951   //JM USER
#define ITM_USER_USER                 1952   //JM USER
#define ITM_USER_SIGMAPLUS            1953   //JM USER
#define ITM_LISTXY                    1954   //JMSTAT
#define ITM_toPOL2                    1955   //JM
#define ITM_toREC2                    1956   //JM
#define ITM_CFG                       1957   //JM Replacements
#define ITM_CLK12                     1958   //JM Replacements
#define ITM_CLK24                     1959   //JM Replacements
#define ITM_CPXI                      1960
#define ITM_CPXJ                      1961
#define ITM_MULTCR                    1962
#define ITM_MULTDOT                   1963
#define ITM_SSIZE4                    1964
#define ITM_POLAR                     1965   //JM Replacements
#define ITM_RDXCOM                    1966   //JM Replacements
#define ITM_RDXPER                    1967   //JM Replacements
#define ITM_SSIZE8                    1968
#define ITM_RECT                      1969   //JM Replacements
#define ITM_FLGSV                     1970
#define ITM_SCIOVR                    1971   //JM Replacements
#define ITM_ENGOVR                    1972   //JM Replacements
#define ITM_USER_V43MIN               1973   //V43
#define ITM_USER_SHIFTS2              1974   //V43
#define ITM_USER_HOME                 1975   //JM USER
#define ITM_USER_WP43S                1976   //V43
#define ITM_USER_DM42                 1977   //V43
#define MNU_ASN_U                     1978   //V43
#define ITM_X_P1                      1979   //JM EXEC
#define ITM_X_P2                      1980   //JM EXEC
#define ITM_X_P3                      1981   //JM EXEC
#define ITM_X_P4                      1982   //JM EXEC
#define ITM_X_P5                      1983   //JM EXEC
#define ITM_X_P6                      1984   //JM EXEC
#define ITM_X_f1                      1985   //JM EXEC
#define ITM_X_f2                      1986   //JM EXEC
#define ITM_X_f3                      1987   //JM EXEC
#define ITM_X_f4                      1988   //JM EXEC
#define ITM_X_f5                      1989   //JM EXEC
#define ITM_X_f6                      1990   //JM EXEC
#define ITM_X_g1                      1991   //JM EXEC
#define ITM_X_g2                      1992   //JM EXEC
#define ITM_X_g3                      1993   //JM EXEC
#define ITM_X_g4                      1994   //JM EXEC
#define ITM_X_g5                      1995   //JM EXEC
#define ITM_X_g6                      1996   //JM EXEC
#define ITM_ROUND2                    1997
#define ITM_ROUNDI2                   1998
#define ITM_USER_C43                  1999
#define ITM_eRPN_ON                   2000
#define ITM_eRPN_OFF                  2001
#define ITM_T_LEFT_ARROW              2002
#define ITM_T_RIGHT_ARROW             2003
#define ITM_T_LLEFT_ARROW             2004
#define ITM_T_RRIGHT_ARROW            2005
#define ITM_T_INSERT                  2006
#define ITM_XNEW                      2007
#define MNU_T_EDIT                    2008
#define ITM_XSAVE                     2009
#define ITM_XLOAD                     2010
#define ITM_XEDIT                     2011
#define ITM_XXEQ                      2012
#define MNU_XXEQ                      2013

#define LAST_ITEM                     2014   //JM

#define CHR_PROD_SIGN                 9999

/********************************************//**
 * \typedef item_t
 * \brief Structure keeping the information for one item
 ***********************************************/
#define CAT_NONE  0
#define CAT_FNCT  1 // Function
#define CAT_MENU  2 // Menu
#define CAT_CNST  3 // Constant
#define CAT_FREE  4 // To identify and find the free items
#define CAT_REGS  5 // Registers
#define CAT_RVAR  6 // Reserved variable
#define CAT_DUPL  7 // Duplicate of another item e.g. acus->m^2
#define CAT_SYFL  8 // System flags
#define CAT_AINT  9 // Upper case alpha_INTL
#define CAT_aint 10 // Lower case alpha_intl
typedef struct {
  void     (*func)(uint16_t); ///< Function called to execute the item
  uint16_t param;             ///< 1st parameter to the above function
  char     *itemCatalogName;  ///< Name of the item in the catalogs
  char     *itemSoftmenuName; ///< Representation of the item in the menus or on the keyboard
  uint16_t tamMin;            ///< Minimul value for TAM argument
  uint16_t tamMax;            ///< Maximal value for TAM argument
  char     catalog;           ///< Menu of CATALOG in which the item is located: see #define CAT_*
  uint8_t  stackLiftStatus;   ///< Stack lift status after item execution.
} item_t;

void runFunction  (int16_t func);
void fnToBeCoded  (void);
void itemToBeCoded(uint16_t unusedParamButMandatory);
void fnNop        (uint16_t unusedParamButMandatory);
