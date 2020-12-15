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


#define ITM_NULL                         0
#define ITM_LBL                          1
#define ITM_GTO                          2
#define ITM_XEQ                          3
#define ITM_RTN                          4
#define ITM_ISE                          5
#define ITM_ISG                          6
#define ITM_ISZ                          7
#define ITM_DSE                          8
#define ITM_DSL                          9
#define ITM_DSZ                         10
#define ITM_XEQU                        11
#define ITM_XNE                         12
#define ITM_XEQUP0                      13
#define ITM_XEQUM0                      14
#define ITM_XAEQU                       15
#define ITM_XLT                         16
#define ITM_XLE                         17
#define ITM_XGE                         18
#define ITM_XGT                         19
#define ITM_FC                          20
#define ITM_FS                          21
#define ITM_EVEN                        22
#define ITM_ODD                         23
#define ITM_FPQ                         24
#define ITM_INT                         25
#define ITM_CPX                         26
#define ITM_MATR                        27
#define ITM_NAN                         28
#define ITM_REAL                        29
#define ITM_SPEC                        30
#define ITM_STRI                        31
#define ITM_PMINFINITY                  32
#define ITM_PRIME                       33
#define ITM_TOP                         34
#define ITM_ENTER                       35
#define ITM_XexY                        36
#define ITM_DROP                        37
#define ITM_DROPY                       38
#define ITM_Rup                         39
#define ITM_Rdown                       40
#define ITM_CLX                         41
#define ITM_FILL                        42
#define ITM_INPUT                       43
#define ITM_STO                         44
#define ITM_STOPLUS                     45
#define ITM_STOMINUS                    46
#define ITM_STOMULT                     47
#define ITM_STODIV                      48
#define ITM_STOMAX                      49
#define ITM_STOMIN                      50
#define ITM_RCL                         51
#define ITM_RCLPLUS                     52
#define ITM_RCLMINUS                    53
#define ITM_RCLMULT                     54
#define ITM_RCLDIV                      55
#define ITM_RCLMAX                      56
#define ITM_RCLMIN                      57
#define ITM_SQUARE                      58
#define ITM_CUBE                        59
#define ITM_YX                          60
#define ITM_SQUAREROOTX                 61
#define ITM_CUBEROOT                    62
#define ITM_XTHROOT                     63
#define ITM_2X                          64
#define ITM_EXP                         65
#define ITM_EX1                         66
#define ITM_10x                         67
#define ITM_LOG2                        68
#define ITM_LN                          69
#define ITM_LN1X                        70
#define ITM_LOG10                       71
#define ITM_LOGXY                       72
#define ITM_1ONX                        73
#define ITM_cos                         74
#define ITM_cosh                        75
#define ITM_sin                         76
#define ITM_sinc                        77
#define ITM_sinh                        78
#define ITM_tan                         79
#define ITM_tanh                        80
#define ITM_arccos                      81
#define ITM_arcosh                      82
#define ITM_arcsin                      83
#define ITM_arsinh                      84
#define ITM_arctan                      85
#define ITM_artanh                      86
#define ITM_CEIL                        87
#define ITM_FLOOR                       88
#define ITM_GCD                         89
#define ITM_LCM                         90
#define ITM_DEC                         91
#define ITM_INC                         92
#define ITM_IP                          93
#define ITM_FP                          94
#define ITM_ADD                         95
#define ITM_SUB                         96
#define ITM_CHS                         97
#define ITM_MULT                        98
#define ITM_DIV                         99
#define ITM_IDIV                       100
#define ITM_IDIVR                      101
#define ITM_MOD                        102
#define ITM_MAX                        103
#define ITM_MIN                        104
#define ITM_MAGNITUDE                  105
#define ITM_NEIGHB                     106
#define ITM_NEXTP                      107
#define ITM_XFACT                      108
#define ITM_CONSTpi                    109
#define ITM_CF                         110
#define ITM_SF                         111
#define ITM_FF                         112
#define ITM_sincpi                     113
#define ITM_0114                       114
#define ITM_0115                       115
#define ITM_0116                       116
#define ITM_0117                       117
#define ITM_0118                       118
#define ITM_0119                       119
#define ITM_0120                       120
#define ITM_0121                       121
#define ITM_0122                       122
#define ITM_0123                       123
#define ITM_0124                       124
#define ITM_0125                       125
#define ITM_0126                       126
#define ITM_0127                       127
#define CST_01                         128
#define CST_02                         129
#define CST_03                         130
#define CST_04                         131
#define CST_05                         132
#define CST_06                         133
#define CST_07                         134
#define CST_08                         135
#define CST_09                         136
#define CST_10                         137
#define CST_11                         138
#define CST_12                         139
#define CST_13                         140
#define CST_14                         141
#define CST_15                         142
#define CST_16                         143
#define CST_17                         144
#define CST_18                         145
#define CST_19                         146
#define CST_20                         147
#define CST_21                         148
#define CST_22                         149
#define CST_23                         150
#define CST_24                         151
#define CST_25                         152
#define CST_26                         153
#define CST_27                         154
#define CST_28                         155
#define CST_29                         156
#define CST_30                         157
#define CST_31                         158
#define CST_32                         159
#define CST_33                         160
#define CST_34                         161
#define CST_35                         162
#define CST_36                         163
#define CST_37                         164
#define CST_38                         165
#define CST_39                         166
#define CST_40                         167
#define CST_41                         168
#define CST_42                         169
#define CST_43                         170
#define CST_44                         171
#define CST_45                         172
#define CST_46                         173
#define CST_47                         174
#define CST_48                         175
#define CST_49                         176
#define CST_50                         177
#define CST_51                         178
#define CST_52                         179
#define CST_53                         180
#define CST_54                         181
#define CST_55                         182
#define CST_56                         183
#define CST_57                         184
#define CST_58                         185
#define CST_59                         186
#define CST_60                         187
#define CST_61                         188
#define CST_62                         189
#define CST_63                         190
#define CST_64                         191
#define CST_65                         192
#define CST_66                         193
#define CST_67                         194
#define CST_68                         195
#define CST_69                         196
#define CST_70                         197
#define CST_71                         198
#define CST_72                         199
#define CST_73                         200
#define CST_74                         201
#define CST_75                         202
#define CST_76                         203
#define CST_77                         204
#define CST_78                         205
#define CST_79                         206
#define ITM_CNST                       207
#define ITM_0208                       208
#define ITM_0209                       209
#define ITM_0210                       210
#define ITM_0211                       211
#define ITM_0212                       212
#define ITM_0213                       213
#define ITM_0214                       214
#define ITM_0215                       215
#define ITM_0216                       216
#define ITM_0217                       217
#define ITM_0218                       218
#define ITM_0219                       219
#define ITM_CtoF                       220
#define ITM_FtoC                       221
#define ITM_DBtoPR                     222
#define ITM_DBtoPRb                    223
#define ITM_DBtoPRc                    224
#define ITM_DBtoFR                     225
#define ITM_DBtoFRb                    226
#define ITM_DBtoFRc                    227
#define ITM_PRtoDB                     228
#define ITM_PRtoDBb                    229
#define ITM_PRtoDBc                    230
#define ITM_FRtoDB                     231
#define ITM_FRtoDBb                    232
#define ITM_FRtoDBc                    233
#define ITM_ACtoM2                     234
#define ITM_ACtoM2b                    235
#define ITM_M2toAC                     236
#define ITM_M2toACb                    237
#define ITM_ACUStoM2                   238
#define ITM_ACUStoM2b                  239
#define ITM_M2toACUS                   240
#define ITM_M2toACUSb                  241
#define ITM_ATMtoPA                    242
#define ITM_PAtoATM                    243
#define ITM_AUtoM                      244
#define ITM_MtoAU                      245
#define ITM_BARtoPA                    246
#define ITM_PAtoBAR                    247
#define ITM_BTUtoJ                     248
#define ITM_JtoBTU                     249
#define ITM_CALtoJ                     250
#define ITM_JtoCAL                     251
#define ITM_LBFFTtoNM                  252
#define ITM_LBFFTtoNMb                 253
#define ITM_NMtoLBFFT                  254
#define ITM_NMtoLBFFTb                 255
#define ITM_CWTtoKG                    256
#define ITM_KGtoCWT                    257
#define ITM_FTtoM                      258
#define ITM_MtoFT                      259
#define ITM_FTUStoM                    260
#define ITM_FTUStoMb                   261
#define ITM_FTUStoMc                   262
#define ITM_MtoFTUS                    263
#define ITM_MtoFTUSb                   264
#define ITM_MtoFTUSc                   265
#define ITM_FZUKtoM3                   266
#define ITM_FZUKtoM3b                  267
#define ITM_M3toFZUK                   268
#define ITM_M3toFZUKb                  269
#define ITM_FZUStoM3                   270
#define ITM_FZUStoM3b                  271
#define ITM_M3toFZUS                   272
#define ITM_M3toFZUSb                  273
#define ITM_GLUKtoM3                   274
#define ITM_M3toGLUK                   275
#define ITM_GLUStoM3                   276
#define ITM_M3toGLUS                   277
#define ITM_HPEtoW                     278
#define ITM_WtoHPE                     279
#define ITM_HPMtoW                     280
#define ITM_WtoHPM                     281
#define ITM_HPUKtoW                    282
#define ITM_WtoHPUK                    283
#define ITM_IHGtoPA                    284
#define ITM_IHGtoPAb                   285
#define ITM_PAtoIHG                    286
#define ITM_PAtoIHGb                   287
#define ITM_INtoM                      288
#define ITM_MtoINCH                    289
#define ITM_WHtoJ                      290
#define ITM_JtoWH                      291
#define ITM_KGtoLBS                    292
#define ITM_LBStoKG                    293
#define ITM_KGtoOZ                     294
#define ITM_OZtoKG                     295
#define ITM_KGtoSCW                    296
#define ITM_KGtoSCWb                   297
#define ITM_SCWtoKG                    298
#define ITM_SCWtoKGb                   299
#define ITM_KGtoSTO                    300
#define ITM_KGtoSTOb                   301
#define ITM_STOtoKG                    302
#define ITM_STOtoKGb                   303
#define ITM_KGtoST                     304
#define ITM_KGtoSTb                    305
#define ITM_KGtoSTc                    306
#define ITM_STtoKG                     307
#define ITM_STtoKGb                    308
#define ITM_STtoKGc                    309
#define ITM_KGtoTON                    310
#define ITM_KGtoTONb                   311
#define ITM_KGtoTONc                   312
#define ITM_TONtoKG                    313
#define ITM_TONtoKGb                   314
#define ITM_TONtoKGc                   315
#define ITM_KGtoTRZ                    316
#define ITM_KGtoTRZb                   317
#define ITM_TRZtoKG                    318
#define ITM_TRZtoKGb                   319
#define ITM_LBFtoN                     320
#define ITM_NtoLBF                     321
#define ITM_LYtoM                      322
#define ITM_MtoLY                      323
#define ITM_MMHGtoPA                   324
#define ITM_MMHGtoPAb                  325
#define ITM_PAtoMMHG                   326
#define ITM_PAtoMMHGb                  327
#define ITM_MItoM                      328
#define ITM_MtoMI                      329
#define ITM_MtoNMI                     330
#define ITM_NMItoM                     331
#define ITM_MtoPC                      332
#define ITM_PCtoM                      333
#define ITM_MtoPOINTb                  334
#define ITM_MtoPOINT                   335
#define ITM_MtoPOINTc                  336
#define ITM_POINTtoM                   337
#define ITM_POINTtoMb                  338
#define ITM_POINTtoMc                  339
#define ITM_MtoYD                      340
#define ITM_YDtoM                      341
#define ITM_PSItoPA                    342
#define ITM_PAtoPSI                    343
#define ITM_PAtoTOR                    344
#define ITM_PAtoTORb                   345
#define ITM_TORtoPA                    346
#define ITM_TORtoPAb                   347
#define ITM_StoYEAR                    348
#define ITM_YEARtoS                    349
#define ITM_CARATtoKG                  350
#define ITM_CARATtoKGb                 351
#define ITM_CARATtoKGc                 352
#define ITM_KGtoCARAT                  353
#define ITM_KGtoCARATb                 354
#define ITM_KGtoCARATc                 355
#define ITM_QTtoM3                     356
#define ITM_M3toQT                     357
#define ITM_FATHOMtoM                  358
#define ITM_FATHOMtoMb                 359
#define ITM_FATHOMtoMc                 360
#define ITM_MtoFATHOM                  361
#define ITM_MtoFATHOMb                 362
#define ITM_MtoFATHOMc                 363
#define ITM_BARRELtoM3                 364
#define ITM_BARRELtoM3b                365
#define ITM_M3toBARREL                 366
#define ITM_M3toBARRELb                367
#define ITM_ATMtoPAb                   368
#define ITM_PAtoATMb                   369
#define ITM_HECTAREtoM2                370
#define ITM_M2toHECTARE                371
#define ITM_0372                       372
#define ITM_0373                       373
#define ITM_0374                       374
#define ITM_0375                       375
#define ITM_0376                       376
#define ITM_0377                       377
#define ITM_0378                       378
#define ITM_0379                       379
#define ITM_0380                       380
#define ITM_0381                       381
#define ITM_0382                       382
#define ITM_0383                       383
#define ITM_0384                       384
#define ITM_0385                       385
#define ITM_FCC                        386
#define ITM_FCS                        387
#define ITM_FCF                        388
#define ITM_FSC                        389
#define ITM_FSS                        390
#define ITM_FSF                        391
#define ITM_LOGICALNOT                 392
#define ITM_LOGICALAND                 393
#define ITM_LOGICALOR                  394
#define ITM_LOGICALXOR                 395
#define ITM_LOGICALNAND                396
#define ITM_LOGICALNOR                 397
#define ITM_LOGICALXNOR                398
#define ITM_BS                         399
#define ITM_BC                         400
#define ITM_CB                         401
#define ITM_SB                         402
#define ITM_FB                         403
#define ITM_RL                         404
#define ITM_RLC                        405
#define ITM_RR                         406
#define ITM_RRC                        407
#define ITM_SL                         408
#define ITM_SR                         409
#define ITM_ASR                        410
#define ITM_LJ                         411
#define ITM_RJ                         412
#define ITM_MASKL                      413
#define ITM_MASKR                      414
#define ITM_MIRROR                     415
#define ITM_NUMB                       416
#define ITM_SDL                        417
#define ITM_SDR                        418
#define ITM_0419                       419
#define ITM_0420                       420
#define ITM_0421                       421
#define ITM_0422                       422
#define ITM_SIGMAPLUS                  423
#define ITM_SIGMAMINUS                 424
#define ITM_NSIGMA                     425
#define ITM_SIGMAx                     426
#define ITM_SIGMAy                     427
#define ITM_SIGMAx2                    428
#define ITM_SIGMAx2y                   429
#define ITM_SIGMAy2                    430
#define ITM_SIGMAxy                    431
#define ITM_SIGMAlnxy                  432
#define ITM_SIGMAlnx                   433
#define ITM_SIGMAln2x                  434
#define ITM_SIGMAylnx                  435
#define ITM_SIGMAlny                   436
#define ITM_SIGMAln2y                  437
#define ITM_SIGMAxlny                  438
#define ITM_SIGMAlnyonx                439
#define ITM_SIGMAx2ony                 440
#define ITM_SIGMA1onx                  441
#define ITM_SIGMA1onx2                 442
#define ITM_SIGMAxony                  443
#define ITM_SIGMA1ony                  444
#define ITM_SIGMA1ony2                 445
#define ITM_SIGMAx3                    446
#define ITM_SIGMAx4                    447
#define ITM_0448                       448
#define ITM_0449                       449
#define ITM_0450                       450
#define ITM_0451                       451
#define ITM_0452                       452
#define SFL_USB                        453
#define SFL_TDM24                      454
#define SFL_YMD                        455
#define SFL_DMY                        456
#define SFL_MDY                        457
#define SFL_CPXRES                     458
#define SFL_CPXj                       459
#define SFL_POLAR                      460
#define SFL_FRACT                      461
#define SFL_PROPFR                     462
#define SFL_DENANY                     463
#define SFL_DENFIX                     464
#define SFL_CARRY                      465
#define SFL_OVERFL                     466
#define SFL_LEAD0                      467
#define SFL_ALPHA                      468
#define SFL_alphaCAP                   469
#define SFL_RUNTIM                     470
#define SFL_RUNIO                      471
#define SFL_PRINT                      472
#define SFL_TRACE                      473
#define SFL_USER                       474
#define SFL_LOWBAT                     475
#define SFL_SLOW                       476
#define SFL_SPCRES                     477
#define SFL_SSIZE8                     478
#define SFL_QUIET                      479
#define SFL_DECIMP                     480
#define SFL_MULTx                      481
#define SFL_ALLENG                     482
#define SFL_GROW                       483
#define SFL_AUTOFF                     484
#define SFL_AUTXEQ                     485
#define SFL_PRTACT                     486
#define SFL_NUMIN                      487
#define SFL_ALPIN                      488
#define SFL_ASLIFT                     489
#define SFL_IGN1ER                     490
#define SFL_INTING                     491
#define SFL_SOLVING                    492
#define SFL_VMDISP                     493
#define ITM_0494                       494
#define ITM_0495                       495
#define ITM_0496                       496
#define ITM_0497                       497
#define ITM_0498                       498
#define ITM_0499                       499
#define ITM_0500                       500
#define ITM_0501                       501
#define ITM_0502                       502
#define ITM_0503                       503
#define ITM_0504                       504
#define ITM_0505                       505
#define ITM_0506                       506
#define ITM_0507                       507
#define ITM_0508                       508
#define ITM_0509                       509
#define ITM_0510                       510
#define ITM_0511                       511
#define ITM_0512                       512
#define ITM_0513                       513
#define ITM_0514                       514
#define ITM_0515                       515
#define ITM_0516                       516
#define ITM_ST_A                       517
#define ITM_ST_B                       518
#define ITM_ST_C                       519
#define ITM_ST_D                       520
#define ITM_ST_T                       521
#define ITM_ST_X                       522
#define ITM_ST_Y                       523
#define ITM_ST_Z                       524
#define ITM_INDIRECTION                525
#define ITM_REG_X                      526
#define ITM_REG_Y                      527
#define ITM_REG_Z                      528
#define ITM_REG_T                      529
#define ITM_REG_A                      530
#define ITM_REG_B                      531
#define ITM_REG_C                      532
#define ITM_REG_D                      533
#define ITM_REG_L                      534
#define ITM_REG_I                      535
#define ITM_REG_J                      536
#define ITM_REG_K                      537
#define ITM_0                          538
#define ITM_1                          539
#define ITM_2                          540
#define ITM_3                          541
#define ITM_4                          542
#define ITM_5                          543
#define ITM_6                          544
#define ITM_7                          545
#define ITM_8                          546
#define ITM_9                          547
#define ITM_A                          548
#define ITM_B                          549
#define ITM_C                          550
#define ITM_D                          551
#define ITM_E                          552
#define ITM_F                          553
#define ITM_G                          554
#define ITM_H                          555
#define ITM_I                          556
#define ITM_J                          557
#define ITM_K                          558
#define ITM_L                          559
#define ITM_M                          560
#define ITM_N                          561
#define ITM_O                          562
#define ITM_P                          563
#define ITM_Q                          564
#define ITM_R                          565
#define ITM_S                          566
#define ITM_T                          567
#define ITM_U                          568
#define ITM_V                          569
#define ITM_W                          570
#define ITM_X                          571
#define ITM_Y                          572
#define ITM_Z                          573
#define ITM_a                          574
#define ITM_b                          575
#define ITM_c                          576
#define ITM_d                          577
#define ITM_e                          578
#define ITM_f                          579
#define ITM_g                          580
#define ITM_h                          581
#define ITM_i                          582
#define ITM_j                          583
#define ITM_k                          584
#define ITM_l                          585
#define ITM_m                          586
#define ITM_n                          587
#define ITM_o                          588
#define ITM_p                          589
#define ITM_q                          590
#define ITM_r                          591
#define ITM_s                          592
#define ITM_t                          593
#define ITM_u                          594
#define ITM_v                          595
#define ITM_w                          596
#define ITM_x                          597
#define ITM_y                          598
#define ITM_z                          599
#define ITM_ALPHA                      600   //NOTE the RANGE STARTS HERE, with +36 for lower case
#define ITM_0601                       601
#define ITM_BETA                       602
#define ITM_GAMMA                      603
#define ITM_DELTA                      604
#define ITM_EPSILON                    605
#define ITM_0606                       606
#define ITM_ZETA                       607
#define ITM_ETA                        608
#define ITM_0609                       609
#define ITM_THETA                      610
#define ITM_IOTA                       611
#define ITM_0612                       612
#define ITM_0613                       613
#define ITM_IOTA_DIALYTIKA             614
#define ITM_KAPPA                      615
#define ITM_LAMBDA                     616
#define ITM_MU                         617
#define ITM_NU                         618
#define ITM_XI                         619
#define ITM_OMICRON                    620
#define ITM_0621                       621
#define ITM_PI                         622
#define ITM_RHO                        623
#define ITM_SIGMA                      624
#define ITM_0625                       625
#define ITM_TAU                        626
#define ITM_UPSILON                    627
#define ITM_0628                       628
#define ITM_UPSILON_DIALYTIKA          629
#define ITM_0630                       630
#define ITM_PHI                        631
#define ITM_CHI                        632
#define ITM_PSI                        633
#define ITM_OMEGA                      634   //NOTE the RANGE STOPS HERE, with +36 for lower case
#define ITM_0635                       635
#define ITM_alpha                      636
#define ITM_alpha_TONOS                637
#define ITM_beta                       638
#define ITM_gamma                      639
#define ITM_delta                      640
#define ITM_epsilon                    641
#define ITM_epsilon_TONOS              642
#define ITM_zeta                       643
#define ITM_eta                        644
#define ITM_eta_TONOS                  645
#define ITM_theta                      646
#define ITM_iota                       647
#define ITM_iotaTON                    648
#define ITM_iota_DIALYTIKA_TONOS       649
#define ITM_iota_DIALYTIKA             650
#define ITM_kappa                      651
#define ITM_lambda                     652
#define ITM_mu                         653
#define ITM_nu                         654
#define ITM_xi                         655
#define ITM_omicron                    656
#define ITM_omicron_TONOS              657
#define ITM_pi                         658
#define ITM_rho                        659
#define ITM_sigma                      660
#define ITM_sigma_end                  661
#define ITM_tau                        662
#define ITM_upsilon                    663
#define ITM_upsilon_TONOS              664
#define ITM_upsilon_DIALYTIKA          665
#define ITM_upsilon_DIALYTIKA_TONOS    666
#define ITM_phi                        667
#define ITM_chi                        668
#define ITM_psi                        669
#define ITM_omega                      670
#define ITM_omega_TONOS                671
#define ITM_0672                       672
#define ITM_0673                       673
#define ITM_0674                       674
#define ITM_0675                       675
#define ITM_0676                       676
#define ITM_0677                       677
#define ITM_A_MACRON                   678
#define ITM_A_ACUTE                    679
#define ITM_A_BREVE                    680
#define ITM_A_GRAVE                    681
#define ITM_A_DIARESIS                 682
#define ITM_A_TILDE                    683
#define ITM_A_CIRC                     684
#define ITM_A_RING                     685
#define ITM_AE                         686
#define ITM_A_OGONEK                   687
#define ITM_C_ACUTE                    688
#define ITM_C_CARON                    689
#define ITM_C_CEDILLA                  690
#define ITM_D_STROKE                   691
#define ITM_D_CARON                    692
#define ITM_E_MACRON                   693
#define ITM_E_ACUTE                    694
#define ITM_E_BREVE                    695
#define ITM_E_GRAVE                    696
#define ITM_E_DIARESIS                 697
#define ITM_E_CIRC                     698
#define ITM_E_OGONEK                   699
#define ITM_G_BREVE                    700
#define ITM_0701                       701
#define ITM_I_MACRON                   702
#define ITM_I_ACUTE                    703
#define ITM_I_BREVE                    704
#define ITM_I_GRAVE                    705
#define ITM_I_DIARESIS                 706
#define ITM_I_CIRC                     707
#define ITM_I_OGONEK                   708
#define ITM_I_DOT                      709
#define ITM_I_DOTLESS                  710
#define ITM_L_STROKE                   711
#define ITM_L_ACUTE                    712
#define ITM_L_APOSTROPHE               713
#define ITM_N_ACUTE                    714
#define ITM_N_CARON                    715
#define ITM_N_TILDE                    716
#define ITM_O_MACRON                   717
#define ITM_O_ACUTE                    718
#define ITM_O_BREVE                    719
#define ITM_O_GRAVE                    720
#define ITM_O_DIARESIS                 721
#define ITM_O_TILDE                    722
#define ITM_O_CIRC                     723
#define ITM_O_STROKE                   724
#define ITM_OE                         725
#define ITM_0726                       726
#define ITM_0727                       727
#define ITM_S_SHARP                    728
#define ITM_S_ACUTE                    729
#define ITM_S_CARON                    730
#define ITM_S_CEDILLA                  731
#define ITM_T_CARON                    732
#define ITM_T_CEDILLA                  733
#define ITM_U_MACRON                   734
#define ITM_U_ACUTE                    735
#define ITM_U_BREVE                    736
#define ITM_U_GRAVE                    737
#define ITM_U_DIARESIS                 738
#define ITM_U_TILDE                    739
#define ITM_U_CIRC                     740
#define ITM_U_RING                     741
#define ITM_W_CIRC                     742
#define ITM_0743                       743
#define ITM_0744                       744
#define ITM_0745                       745
#define ITM_Y_CIRC                     746
#define ITM_Y_ACUTE                    747
#define ITM_Y_DIARESIS                 748
#define ITM_Z_ACUTE                    749
#define ITM_Z_CARON                    750
#define ITM_Z_DOT                      751
#define ITM_0752                       752
#define ITM_0753                       753
#define ITM_0754                       754
#define ITM_0755                       755
#define ITM_0756                       756
#define ITM_0757                       757
#define ITM_a_MACRON                   758
#define ITM_a_ACUTE                    759
#define ITM_a_BREVE                    760
#define ITM_a_GRAVE                    761
#define ITM_a_DIARESIS                 762
#define ITM_a_TILDE                    763
#define ITM_a_CIRC                     764
#define ITM_a_RING                     765
#define ITM_ae                         766
#define ITM_a_OGONEK                   767
#define ITM_c_ACUTE                    768
#define ITM_c_CARON                    769
#define ITM_c_CEDILLA                  770
#define ITM_d_STROKE                   771
#define ITM_d_APOSTROPHE               772
#define ITM_e_MACRON                   773
#define ITM_e_ACUTE                    774
#define ITM_e_BREVE                    775
#define ITM_e_GRAVE                    776
#define ITM_e_DIARESIS                 777
#define ITM_e_CIRC                     778
#define ITM_e_OGONEK                   779
#define ITM_g_BREVE                    780
#define ITM_h_STROKE                   781
#define ITM_i_MACRON                   782
#define ITM_i_ACUTE                    783
#define ITM_i_BREVE                    784
#define ITM_i_GRAVE                    785
#define ITM_i_DIARESIS                 786
#define ITM_i_CIRC                     787
#define ITM_i_OGONEK                   788
#define ITM_i_DOT                      789
#define ITM_i_DOTLESS                  790
#define ITM_l_STROKE                   791
#define ITM_l_ACUTE                    792
#define ITM_l_APOSTROPHE               793
#define ITM_n_ACUTE                    794
#define ITM_n_CARON                    795
#define ITM_n_TILDE                    796
#define ITM_o_MACRON                   797
#define ITM_o_ACUTE                    798
#define ITM_o_BREVE                    799
#define ITM_o_GRAVE                    800
#define ITM_o_DIARESIS                 801
#define ITM_o_TILDE                    802
#define ITM_o_CIRC                     803
#define ITM_o_STROKE                   804
#define ITM_oe                         805
#define ITM_r_CARON                    806
#define ITM_r_ACUTE                    807
#define ITM_s_SHARP                    808
#define ITM_s_ACUTE                    809
#define ITM_s_CARON                    810
#define ITM_s_CEDILLA                  811
#define ITM_t_APOSTROPHE               812
#define ITM_t_CEDILLA                  813
#define ITM_u_MACRON                   814
#define ITM_u_ACUTE                    815
#define ITM_u_BREVE                    816
#define ITM_u_GRAVE                    817
#define ITM_u_DIARESIS                 818
#define ITM_u_TILDE                    819
#define ITM_u_CIRC                     820
#define ITM_u_RING                     821
#define ITM_w_CIRC                     822
#define ITM_x_BAR                      823
#define ITM_x_CIRC                     824
#define ITM_y_BAR                      825
#define ITM_y_CIRC                     826
#define ITM_y_ACUTE                    827
#define ITM_y_DIARESIS                 828
#define ITM_z_ACUTE                    829
#define ITM_z_CARON                    830
#define ITM_z_DOT                      831
#define ITM_0832                       832
#define ITM_0833                       833
#define ITM_0834                       834
#define ITM_0835                       835
#define ITM_0836                       836
#define ITM_0837                       837
#define ITM_SUB_alpha                  838
#define ITM_SUB_delta                  839
#define ITM_SUB_mu                     840
#define ITM_SUB_SUN                    841
#define ITM_SUB_SUN_b                  842
#define ITM_SUB_EARTH                  843
#define ITM_SUB_EARTH_b                844
#define ITM_SUB_PLUS                   845
#define ITM_SUB_MINUS                  846
#define ITM_SUB_INFINITY               847
#define ITM_SUB_0                      848
#define ITM_SUB_1                      849
#define ITM_SUB_2                      850
#define ITM_SUB_3                      851
#define ITM_SUB_4                      852
#define ITM_SUB_5                      853
#define ITM_SUB_6                      854
#define ITM_SUB_7                      855
#define ITM_SUB_8                      856
#define ITM_SUB_9                      857
#define ITM_SUB_10                     858
#define ITM_SUB_A                      859
#define ITM_SUB_B                      860
#define ITM_SUB_C                      861
#define ITM_SUB_D                      862
#define ITM_SUB_E                      863
#define ITM_SUB_F                      864
#define ITM_SUB_G                      865
#define ITM_SUB_H                      866
#define ITM_SUB_I                      867
#define ITM_SUB_J                      868
#define ITM_SUB_K                      869
#define ITM_SUB_L                      870
#define ITM_SUB_M                      871
#define ITM_SUB_N                      872
#define ITM_SUB_O                      873
#define ITM_SUB_P                      874
#define ITM_SUB_Q                      875
#define ITM_SUB_R                      876
#define ITM_SUB_S                      877
#define ITM_SUB_T                      878
#define ITM_SUB_U                      879
#define ITM_SUB_V                      880
#define ITM_SUB_W                      881
#define ITM_SUB_X                      882
#define ITM_SUB_Y                      883
#define ITM_SUB_Z                      884
#define ITM_SUB_E_OUTLINE              885
#define ITM_SUB_a                      886
#define ITM_SUB_b                      887
#define ITM_SUB_c                      888
#define ITM_SUB_d                      889
#define ITM_SUB_e                      890
#define ITM_SUB_h                      891
#define ITM_SUB_i                      892
#define ITM_SUB_j                      893
#define ITM_SUB_k                      894
#define ITM_SUB_l                      895
#define ITM_SUB_m                      896
#define ITM_SUB_n                      897
#define ITM_SUB_o                      898
#define ITM_SUB_p                      899
#define ITM_SUB_q                      900
#define ITM_SUB_s                      901
#define ITM_SUB_t                      902
#define ITM_SUB_u                      903
#define ITM_SUB_v                      904
#define ITM_SUB_w                      905
#define ITM_SUB_x                      906
#define ITM_SUB_y                      907
#define ITM_SUB_z                      908
#define ITM_SUB_a_b                    909
#define ITM_SUB_e_b                    910
#define ITM_SUB_k_b                    911
#define ITM_SUB_l_b                    912
#define ITM_SUB_m_b                    913
#define ITM_SUB_n_b                    914
#define ITM_SUB_o_b                    915
#define ITM_SUB_p_b                    916
#define ITM_SUB_s_b                    917
#define ITM_SUB_u_b                    918
#define ITM_SUB_x_b                    919
#define ITM_SUP_PLUS                   920
#define ITM_SUP_MINUS                  921
#define ITM_SUP_MINUS_1                922
#define ITM_SUP_INFINITY               923
#define ITM_SUP_ASTERISK               924
#define ITM_SUP_0                      925
#define ITM_SUP_1                      926
#define ITM_SUP_2                      927
#define ITM_SUP_3                      928
#define ITM_SUP_4                      929
#define ITM_SUP_5                      930
#define ITM_SUP_6                      931
#define ITM_SUP_7                      932
#define ITM_SUP_8                      933
#define ITM_SUP_9                      934
#define ITM_0935                       935
#define ITM_SUP_T                      936
#define ITM_SUP_a                      937
#define ITM_SUP_f                      938
#define ITM_SUP_g                      939
#define ITM_SUP_h                      940
#define ITM_SUP_r                      941
#define ITM_SUP_x                      942
#define ITM_SPACE                      943
#define ITM_EXCLAMATION_MARK           944
#define ITM_DOUBLE_QUOTE               945
#define ITM_NUMBER_SIGN                946
#define ITM_DOLLAR                     947
#define ITM_PERCENT                    948
#define ITM_AMPERSAND                  949
#define ITM_QUOTE                      950
#define ITM_LEFT_PARENTHESIS           951
#define ITM_RIGHT_PARENTHESIS          952
#define ITM_ASTERISK                   953
#define ITM_0954                       954
#define ITM_PLUS                       955
#define ITM_COMMA                      956
#define ITM_MINUS                      957
#define ITM_PERIOD                     958
#define ITM_SLASH                      959
#define ITM_COLON                      960
#define ITM_SEMICOLON                  961
#define ITM_LESS_THAN                  962
#define ITM_EQUAL                      963
#define ITM_GREATER_THAN               964
#define ITM_QUESTION_MARK              965
#define ITM_AT                         966
#define ITM_LEFT_SQUARE_BRACKET        967
#define ITM_BACK_SLASH                 968
#define ITM_RIGHT_SQUARE_BRACKET       969
#define ITM_CIRCUMFLEX                 970
#define ITM_UNDERSCORE                 971
#define ITM_LEFT_CURLY_BRACKET         972
#define ITM_PIPE                       973
#define ITM_RIGHT_CURLY_BRACKET        974
#define ITM_TILDE                      975
#define ITM_INVERTED_EXCLAMATION_MARK  976
#define ITM_CENT                       977
#define ITM_POUND                      978
#define ITM_YEN                        979
#define ITM_SECTION                    980
#define ITM_OVERFLOW_CARRY             981
#define ITM_LEFT_DOUBLE_ANGLE          982
#define ITM_NOT                        983
#define ITM_DEGREE                     984
#define ITM_PLUS_MINUS                 985
#define ITM_mu_b                       986
#define ITM_DOT                        987
#define ITM_ORDINAL                    988
#define ITM_RIGHT_DOUBLE_ANGLE         989
#define ITM_ONE_HALF                   990
#define ITM_ONE_QUARTER                991
#define ITM_INVERTED_QUESTION_MARK     992
#define ITM_ETH                        993
#define ITM_CROSS                      994
#define ITM_eth                        995
#define ITM_OBELUS                     996
#define ITM_E_DOT                      997
#define ITM_e_DOT                      998
#define ITM_E_CARON                    999
#define ITM_e_CARON                   1000
#define ITM_R_ACUTE                   1001
#define ITM_R_CARON                   1002
#define ITM_U_OGONEK                  1003
#define ITM_u_OGONEK                  1004
#define ITM_y_UNDER_ROOT              1005
#define ITM_x_UNDER_ROOT              1006
#define ITM_SPACE_EM                  1007
#define ITM_SPACE_3_PER_EM            1008
#define ITM_SPACE_4_PER_EM            1009
#define ITM_SPACE_6_PER_EM            1010
#define ITM_SPACE_FIGURE              1011
#define ITM_SPACE_PUNCTUATION         1012
#define ITM_SPACE_HAIR                1013
#define ITM_LEFT_SINGLE_QUOTE         1014
#define ITM_RIGHT_SINGLE_QUOTE        1015
#define ITM_SINGLE_LOW_QUOTE          1016
#define ITM_SINGLE_HIGH_QUOTE         1017
#define ITM_LEFT_DOUBLE_QUOTE         1018
#define ITM_RIGHT_DOUBLE_QUOTE        1019
#define ITM_DOUBLE_LOW_QUOTE          1020
#define ITM_DOUBLE_HIGH_QUOTE         1021
#define ITM_ELLIPSIS                  1022
#define ITM_ONE                       1023
#define ITM_EURO                      1024
#define ITM_COMPLEX_C                 1025
#define ITM_PLANCK                    1026
#define ITM_PLANCK_2PI                1027
#define ITM_NATURAL_N                 1028
#define ITM_RATIONAL_Q                1029
#define ITM_REAL_R                    1030
#define ITM_LEFT_ARROW                1031
#define ITM_UP_ARROW                  1032
#define ITM_1033                      1033
#define ITM_RIGHT_ARROW               1034
#define ITM_DOWN_ARROW                1035
#define ITM_1036                      1036
#define ITM_SERIAL_IO                 1037
#define ITM_RIGHT_SHORT_ARROW         1038
#define ITM_LEFT_RIGHT_ARROWS         1039
#define ITM_BST_SIGN                  1040
#define ITM_SST_SIGN                  1041
#define ITM_HAMBURGER                 1042
#define ITM_UNDO_SIGN                 1043
#define ITM_FOR_ALL                   1044
#define ITM_COMPLEMENT                1045
#define ITM_PARTIAL_DIFF              1046
#define ITM_THERE_EXISTS              1047
#define ITM_THERE_DOES_NOT_EXIST      1048
#define ITM_EMPTY_SET                 1049
#define ITM_INCREMENT                 1050
#define ITM_NABLA                     1051
#define ITM_ELEMENT_OF                1052
#define ITM_NOT_ELEMENT_OF            1053
#define ITM_CONTAINS                  1054
#define ITM_DOES_NOT_CONTAIN          1055
#define ITM_ZERO                      1056
#define ITM_PRODUCT                   1057
#define ITM_1058                      1058
#define ITM_1059                      1059
#define ITM_MINUS_PLUS                1060
#define ITM_1061                      1061
#define ITM_1062                      1062
#define ITM_RING                      1063
#define ITM_BULLET                    1064
#define ITM_SQUARE_ROOT               1065
#define ITM_CUBE_ROOT                 1066
#define ITM_xTH_ROOT                  1067
#define ITM_PROPORTIONAL              1068
#define ITM_INFINITY                  1069
#define ITM_RIGHT_ANGLE               1070
#define ITM_ANGLE_SIGN                1071
#define ITM_MEASURED_ANGLE            1072
#define ITM_DIVIDES                   1073
#define ITM_DOES_NOT_DIVIDE           1074
#define ITM_PARALLEL_SIGN             1075
#define ITM_NOT_PARALLEL              1076
#define ITM_AND                       1077
#define ITM_OR                        1078
#define ITM_INTERSECTION              1079
#define ITM_UNION                     1080
#define ITM_INTEGRAL_SIGN             1081
#define ITM_DOUBLE_INTEGRAL           1082
#define ITM_CONTOUR_INTEGRAL          1083
#define ITM_SURFACE_INTEGRAL          1084
#define ITM_RATIO                     1085
#define ITM_CHECK_MARK                1086
#define ITM_ASYMPOTICALLY_EQUAL       1087
#define ITM_ALMOST_EQUAL              1088
#define ITM_COLON_EQUALS              1089
#define ITM_CORRESPONDS_TO            1090
#define ITM_ESTIMATES                 1091
#define ITM_NOT_EQUAL                 1092
#define ITM_IDENTICAL_TO              1093
#define ITM_LESS_EQUAL                1094
#define ITM_GREATER_EQUAL             1095
#define ITM_MUCH_LESS                 1096
#define ITM_MUCH_GREATER              1097
#define ITM_SUN                       1098
#define ITM_DOWN_TACK                 1099
#define ITM_PERPENDICULAR             1100
#define ITM_XOR                       1101
#define ITM_NAND                      1102
#define ITM_NOR                       1103
#define ITM_WATCH                     1104
#define ITM_HOURGLASS                 1105
#define ITM_PRINTER                   1106
#define ITM_MAT_TL                    1107
#define ITM_MAT_ML                    1108
#define ITM_MAT_BL                    1109
#define ITM_MAT_TR                    1110
#define ITM_MAT_MR                    1111
#define ITM_MAT_BR                    1112
#define ITM_OBLIQUE1                  1113
#define ITM_OBLIQUE2                  1114
#define ITM_OBLIQUE3                  1115
#define ITM_OBLIQUE4                  1116
#define ITM_CURSOR                    1117
#define ITM_PERIOD34                  1118
#define ITM_COMMA34                   1119
#define ITM_BATTERY                   1120
#define ITM_PGM_BEGIN                 1121
#define ITM_USER_MODE                 1122
#define ITM_UK                        1123
#define ITM_US                        1124
#define ITM_NEG_EXCLAMATION_MARK      1125
#define ITM_ex                        1126
#define ITM_Max                       1127
#define ITM_Min                       1128
#define ITM_Config                    1129
#define ITM_Stack                     1130
#define ITM_dddEL                     1131
#define ITM_dddIJ                     1132
#define ITM_0P                        1133
#define ITM_1P                        1134
#define ITM_EXPONENT                  1135
#define ITM_MA11                      1136
#define ITM_MA12                      1137
#define ITM_MA13                      1138
#define ITM_MA14                      1139
#define ITM_MA15                      1140
#define ITM_MA16                      1141
#define ITM_MA21                      1142
#define ITM_MA22                      1143
#define ITM_MA23                      1144
#define ITM_MA24                      1145
#define ITM_MA25                      1146
#define ITM_MA26                      1147
#define ITM_MA31                      1148
#define ITM_MA32                      1149
#define ITM_MA33                      1150
#define ITM_MA34                      1151
#define ITM_MA35                      1152
#define ITM_MA36                      1153
#define ITM_HEX                       1154
#define ITM_1155                      1155
#define ITM_1156                      1156
#define ITM_1157                      1157
#define ITM_1158                      1158
#define ITM_1159                      1159
#define ITM_1160                      1160
#define ITM_1161                      1161
#define ITM_1162                      1162
#define ITM_1163                      1163
#define ITM_1164                      1164
#define ITM_REGI                      1165
#define ITM_REGJ                      1166
#define ITM_REGK                      1167
#define ITM_REGL                      1168
#define ITM_REGA                      1169
#define ITM_ACC                       1170
#define ITM_REGB                      1171
#define ITM_REGC                      1172
#define ITM_REGD                      1173
#define ITM_FV                        1174
#define ITM_IPCA                      1175
#define ITM_MATA                      1176
#define ITM_MATB                      1177
#define ITM_NPER                      1178
#define ITM_PERonA                    1179
#define ITM_PMT                       1180
#define ITM_PV                        1181
#define ITM_REGS                      1182
#define ITM_ULIM                      1183
#define ITM_DLIM                      1184
#define ITM_1185                      1185
#define ITM_1186                      1186
#define ITM_1187                      1187
#define ITM_1188                      1188
#define ITM_1189                      1189
#define ITM_1190                      1190
#define ITM_1191                      1191
#define ITM_1192                      1192
#define ITM_1193                      1193
#define ITM_1194                      1194
#define ITM_1195                      1195
#define ITM_1196                      1196
#define MNU_BINOM                     1197
#define ITM_BINOMP                    1198
#define ITM_BINOM                     1199
#define ITM_BINOMU                    1200
#define ITM_BINOMM1                   1201
#define MNU_CAUCH                     1202
#define ITM_CAUCHP                    1203
#define ITM_CAUCH                     1204
#define ITM_CAUCHU                    1205
#define ITM_CAUCHM1                   1206
#define MNU_EXPON                     1207
#define ITM_EXPONP                    1208
#define ITM_EXPON                     1209
#define ITM_EXPONU                    1210
#define ITM_EXPONM1                   1211
#define MNU_F                         1212
#define ITM_FPX                       1213
#define ITM_FX                        1214
#define ITM_FUX                       1215
#define ITM_FM1P                      1216
#define MNU_GEOM                      1217
#define ITM_GEOMP                     1218
#define ITM_GEOM                      1219
#define ITM_GEOMU                     1220
#define ITM_GEOMM1                    1221
#define MNU_HYPER                     1222
#define ITM_HYPERP                    1223
#define ITM_HYPER                     1224
#define ITM_HYPERU                    1225
#define ITM_HYPERM1                   1226
#define MNU_LGNRM                     1227
#define ITM_LGNRMP                    1228
#define ITM_LGNRM                     1229
#define ITM_LGNRMU                    1230
#define ITM_LGNRMM1                   1231
#define MNU_LOGIS                     1232
#define ITM_LOGISP                    1233
#define ITM_LOGIS                     1234
#define ITM_LOGISU                    1235
#define ITM_LOGISM1                   1236
#define MNU_NBIN                      1237
#define ITM_NBINP                     1238
#define ITM_NBIN                      1239
#define ITM_NBINU                     1240
#define ITM_NBINM1                    1241
#define MNU_NORML                     1242
#define ITM_NORMLP                    1243
#define ITM_NORML                     1244
#define ITM_NORMLU                    1245
#define ITM_NORMLM1                   1246
#define MNU_POISS                     1247
#define ITM_POISSP                    1248
#define ITM_POISS                     1249
#define ITM_POISSU                    1250
#define ITM_POISSM1                   1251
#define MNU_T                         1252
#define ITM_TPX                       1253
#define ITM_TX                        1254
#define ITM_TUX                       1255
#define ITM_TM1P                      1256
#define MNU_WEIBL                     1257
#define ITM_WEIBLP                    1258
#define ITM_WEIBL                     1259
#define ITM_WEIBLU                    1260
#define ITM_WEIBLM1                   1261
#define MNU_CHI2                      1262
#define ITM_chi2Px                    1263
#define ITM_chi2x                     1264
#define ITM_chi2ux                    1265
#define ITM_chi2M1                    1266
#define ITM_1267                      1267
#define ITM_1268                      1268
#define ITM_1269                      1269
#define ITM_1270                      1270
#define ITM_1271                      1271
#define ITM_1272                      1272
#define ITM_1273                      1273
#define ITM_1274                      1274
#define ITM_1275                      1275
#define ITM_1276                      1276
#define ITM_1277                      1277
#define ITM_1278                      1278
#define ITM_1279                      1279
#define ITM_1280                      1280
#define ITM_1281                      1281
#define ITM_1282                      1282
#define ITM_1283                      1283
#define ITM_1284                      1284
#define ITM_1285                      1285
#define ITM_1286                      1286
#define ITM_BESTF                     1287
#define ITM_EXPF                      1288
#define ITM_LINF                      1289
#define ITM_LOGF                      1290
#define ITM_ORTHOF                    1291
#define ITM_POWERF                    1292
#define ITM_GAUSSF                    1293
#define ITM_CAUCHF                    1294
#define ITM_PARABF                    1295
#define ITM_HYPF                      1296
#define ITM_ROOTF                     1297
#define ITM_1298                      1298
#define ITM_1299                      1299
#define ITM_1300                      1300
#define ITM_1301                      1301
#define ITM_1302                      1302
#define MNU_ADV                       1303
#define MNU_ANGLES                    1304
#define MNU_PRINT                     1305
#define MNU_CONVA                     1306
#define MNU_BITS                      1307
#define MNU_CATALOG                   1308
#define MNU_CHARS                     1309
#define MNU_CLK                       1310
#define MNU_REGIST                    1311
#define MNU_CLR                       1312
#define MNU_CONST                     1313
#define MNU_CPX                       1314
#define MNU_CPXS                      1315
#define MNU_DATES                     1316
#define MNU_DIGITS                    1317
#define MNU_DSP                       1318
#define MNU_EQN                       1319
#define MNU_EXP                       1320
#define MNU_CONVE                     1321
#define MNU_FCNS                      1322
#define MNU_FIN                       1323
#define MNU_SINTS                     1324
#define MNU_FLAGS                     1325
#define MNU_FLASH                     1326
#define MNU_1STDERIV                  1327
#define MNU_2NDDERIV                  1328
#define MNU_CONVFP                    1329
#define MNU_LINTS                     1330
#define MNU_INFO                      1331
#define MNU_INTS                      1332
#define MNU_IO                        1333
#define MNU_LOOP                      1334
#define MNU_MATRS                     1335
#define MNU_MATX                      1336
#define MNU_MENUS                     1337
#define MNU_MODE                      1338
#define MNU_SIMQ                      1339
#define MNU_M_EDIT                    1340
#define MNU_MyMenu                    1341
#define MNU_MyAlpha                   1342
#define MNU_CONVM                     1343
#define MNU_ORTHOG                    1344
#define MNU_PARTS                     1345
#define MNU_PROB                      1346
#define MNU_PROGS                     1347
#define MNU_PFN                       1348
#define MNU_PFN2                      1349
#define MNU_CONVP                     1350
#define MNU_RAM                       1351
#define MNU_REALS                     1352
#define MNU_Solver                    1353
#define MNU_STAT                      1354
#define MNU_STK                       1355
#define MNU_STRING                    1356
#define MNU_TEST                      1357
#define MNU_TIMES                     1358
#define MNU_TRI                       1359
#define MNU_TVM                       1360
#define MNU_UNITCONV                  1361
#define MNU_VARS                      1362
#define MNU_CONVV                     1363
#define MNU_XFN                       1364
#define MNU_CONVX                     1365
#define MNU_ALPHAINTL                 1366
#define MNU_ALPHAMATH                 1367
#define MNU_ALPHAFN                   1368
#define MNU_ALPHA_OMEGA               1369
#define MNU_ALPHADOT                  1370
#define MNU_SYSFL                     1371
#define MNU_Sf                        1372
#define MNU_Sfdx                      1373
#define MNU_ANGLECONV                 1374
#define MNU_alpha_omega               1375
#define MNU_ALPHAintl                 1376
#define MNU_TAM                       1377
#define MNU_TAMCMP                    1378
#define MNU_TAMSTORCL                 1379
#define MNU_SUMS                      1380
#define MNU_VAR                       1381
#define MNU_TAMFLAG                   1382
#define MNU_TAMSHUFFLE                1383
#define CHR_1384                      1384
#define CHR_1385                      1385
#define ITM_1386                      1386
#define ITM_1387                      1387
#define ITM_1388                      1388
#define ITM_1389                      1389
#define ITM_1390                      1390
#define ITM_1391                      1391
#define ITM_1392                      1392
#define ITM_1393                      1393
#define ITM_1COMPL                    1394
#define ITM_SCRDMP                    1395
#define ITM_2COMPL                    1396
#define ITM_ABS                       1397
#define ITM_AGM                       1398
#define ITM_AGRAPH                    1399
#define ITM_ALL                       1400
#define ITM_ASSIGN                    1401
#define ITM_BACK                      1402
#define ITM_BATT                      1403
#define ITM_BEEP                      1404
#define ITM_BEGINP                    1405
#define ITM_BN                        1406
#define ITM_BNS                       1407
#define ITM_CASE                      1408
#define ITM_CLALL                     1409
#define ITM_CLCVAR                    1410
#define ITM_CLFALL                    1411
#define ITM_TGLFRT                    1412
#define ITM_CLLCD                     1413
#define ITM_CLMENU                    1414
#define ITM_CLP                       1415
#define ITM_CLPALL                    1416
#define ITM_CLREGS                    1417
#define ITM_CLSTK                     1418
#define ITM_CLSIGMA                   1419
#define ITM_COMB                      1420
#define ITM_CONJ                      1421
#define ITM_CONVG                     1422
#define ITM_CORR                      1423
#define ITM_COV                       1424
#define ITM_BESTFQ                    1425
#define ITM_CROSS_PROD                1426
#define ITM_CXtoRE                    1427
#define ITM_DATE                      1428
#define ITM_DATEto                    1429
#define ITM_DAY                       1430
#define ITM_DBLR                      1431
#define ITM_DBLCROSS                  1432
#define ITM_DBLSLASH                  1433
#define ITM_DECOMP                    1434
#define ITM_DEG                       1435
#define ITM_DEGto                     1436
#define ITM_SA                        1437
#define ITM_DENMAX                    1438
#define ITM_DOT_PROD                  1439
#define ITM_DSTACK                    1440
#define ITM_DMS                       1441
#define ITM_DMSto                     1442
#define ITM_DMY                       1443
#define ITM_DtoJ                      1444
#define ITM_DtoR                      1445
#define ITM_EIGVAL                    1446
#define ITM_EIGVEC                    1447
#define ITM_END                       1448
#define ITM_ENDP                      1449
#define ITM_ENG                       1450
#define ITM_ENORM                     1451
#define ITM_ENTRY                     1452
#define ITM_EQ_DEL                    1453
#define ITM_EQ_EDI                    1454
#define ITM_EQ_NEW                    1455
#define ITM_ERF                       1456
#define ITM_ERFC                      1457
#define ITM_ERR                       1458
#define ITM_EXITALL                   1459
#define ITM_EXPT                      1460
#define MNU_PLOT                      1461
#define ITM_FIB                       1462
#define ITM_FIX                       1463
#define ITM_FLASH                     1464
#define ITM_FQX                       1465
#define ITM_FDQX                      1466
#define ITM_GAP                       1467
#define ITM_GD                        1468
#define ITM_GDM1                      1469
#define ITM_GRAD                      1470
#define ITM_GRADto                    1471
#define ITM_GTOP                      1472
#define ITM_HN                        1473
#define ITM_HNP                       1474
#define ITM_IM                        1475
#define ITM_INDEX                     1476
#define ITM_IXYZ                      1477
#define ITM_IGAMMAP                   1478
#define ITM_IGAMMAQ                   1479
#define ITM_IPLUS                     1480
#define ITM_IMINUS                    1481
#define ITM_JYX                       1482
#define ITM_JPLUS                     1483
#define ITM_JMINUS                    1484
#define ITM_JonG                      1485
#define ITM_JtoD                      1486
#define ITM_KEY                       1487
#define ITM_KEYG                      1488
#define ITM_KEYX                      1489
#define ITM_KEYQ                      1490
#define ITM_KTYP                      1491
#define ITM_LASTX                     1492
#define ITM_LBLQ                      1493
#define ITM_LEAP                      1494
#define ITM_Ln                        1495
#define ITM_LNALPHA                   1496
#define ITM_LNBETA                    1497
#define ITM_LNGAMMA                   1498
#define ITM_LOAD                      1499
#define ITM_LOADP                     1500
#define ITM_LOADR                     1501
#define ITM_LOADSS                    1502
#define ITM_LOADSIGMA                 1503
#define ITM_LocR                      1504
#define ITM_LocRQ                     1505
#define ITM_LR                        1506
#define ITM_MANT                      1507
#define ITM_MATX                      1508
#define ITM_MEM                       1509
#define ITM_MENU                      1510
#define ITM_MONTH                     1511
#define ITM_MSG                       1512
#define ITM_MULPI                     1513
#define ITM_MVAR                      1514
#define ITM_M_DELR                    1515
#define ITM_M_DIM                     1516
#define ITM_M_DIMQ                    1517
#define ITM_MDY                       1518
#define ITM_M_EDI                     1519
#define ITM_M_EDIN                    1520
#define ITM_M_GET                     1521
#define ITM_M_GOTO                    1522
#define ITM_M_GROW                    1523
#define ITM_M_INSR                    1524
#define ITM_M_LU                      1525
#define ITM_M_NEW                     1526
#define ITM_M_OLD                     1527
#define ITM_M_PUT                     1528
#define ITM_M_RR                      1529
#define ITM_M_SQR                     1530
#define ITM_M_WRAP                    1531
#define ITM_NOP                       1532
#define ITM_OFF                       1533
#define ITM_PAUSE                     1534
#define ITM_PERM                      1535
#define ITM_PGMINT                    1536
#define ITM_PGMSLV                    1537
#define ITM_PIXEL                     1538
#define ITM_PLOT                      1539
#define ITM_PN                        1540
#define ITM_POINT                     1541
#define ITM_LOADV                     1542
#define ITM_POPLR                     1543
#define ITM_PRCL                      1544
#define ITM_PSTO                      1545
#define ITM_PUTK                      1546
#define ITM_RAD                       1547
#define ITM_RADto                     1548
#define ITM_RAN                       1549
#define ITM_RBR                       1550
#define ITM_RCLCFG                    1551
#define ITM_RCLEL                     1552
#define ITM_RCLIJ                     1553
#define ITM_RCLS                      1554
#define ITM_RDP                       1555
#define ITM_RE                        1556
#define ITM_RECV                      1557
#define ITM_RESET                     1558
#define ITM_REtoCX                    1559
#define ITM_REexIM                    1560
#define ITM_RM                        1561
#define ITM_RMQ                       1562
#define ITM_RMD                       1563
#define ITM_RNORM                     1564
#define ITM_ROUND                     1565
#define ITM_ROUNDI                    1566
#define ITM_RSD                       1567
#define ITM_RSUM                      1568
#define ITM_RTNP1                     1569
#define ITM_R_CLR                     1570
#define ITM_R_COPY                    1571
#define ITM_R_SORT                    1572
#define ITM_R_SWAP                    1573
#define ITM_RtoD                      1574
#define ITM_STDDEVWEIGHTED            1575
#define ITM_SAVE                      1576
#define ITM_SCI                       1577
#define ITM_SDIGS                     1578
#define ITM_SEED                      1579
#define ITM_SEND                      1580
#define ITM_SETCHN                    1581
#define ITM_SETDAT                    1582
#define ITM_SETEUR                    1583
#define ITM_SETIND                    1584
#define ITM_SETJPN                    1585
#define ITM_SETSIG                    1586
#define ITM_SETTIM                    1587
#define ITM_SETUK                     1588
#define ITM_SETUSA                    1589
#define ITM_SIGN                      1590
#define ITM_SIGNMT                    1591
#define ITM_SIM_EQ                    1592
#define ITM_SKIP                      1593
#define ITM_SLVQ                      1594
#define ITM_SM                        1595
#define ITM_ISM                       1596
#define ITM_SMW                       1597
#define ITM_SOLVE                     1598
#define ITM_SSIZE                     1599
#define ITM_STATUS                    1600
#define ITM_STOCFG                    1601
#define ITM_STOEL                     1602
#define ITM_STOIJ                     1603
#define ITM_STOP                      1604
#define ITM_STOS                      1605
#define ITM_SUM                       1606
#define ITM_SW                        1607
#define ITM_SXY                       1608
#define ITM_TDISP                     1609
#define ITM_TICKS                     1610
#define ITM_TIME                      1611
#define ITM_TIMER                     1612
#define ITM_TN                        1613
#define ITM_TONE                      1614
#define ITM_Tex                       1615
#define ITM_ULP                       1616
#define ITM_UN                        1617
#define ITM_UNITV                     1618
#define ITM_UNSIGN                    1619
#define ITM_VARMNU                    1620
#define ITM_VERS                      1621
#define ITM_VIEW                      1622
#define ITM_WDAY                      1623
#define ITM_WHO                       1624
#define ITM_WM                        1625
#define ITM_WP                        1626
#define ITM_WM1                       1627
#define ITM_WSIZE                     1628
#define ITM_WSIZEQ                    1629
#define ITM_XBAR                      1630
#define ITM_XG                        1631
#define ITM_XW                        1632
#define ITM_XCIRC                     1633
#define ITM_XtoDATE                   1634
#define ITM_XtoALPHA                  1635
#define ITM_Xex                       1636
#define ITM_YEAR                      1637
#define ITM_YCIRC                     1638
#define ITM_YMD                       1639
#define ITM_Yex                       1640
#define ITM_Zex                       1641
#define ITM_ALPHALENG                 1642
#define ITM_XMAX                      1643
#define ITM_XMIN                      1644
#define ITM_ALPHAPOS                  1645
#define ITM_ALPHARL                   1646
#define ITM_ALPHARR                   1647
#define ITM_ALPHASL                   1648
#define ITM_ALPHASR                   1649
#define ITM_ALPHAtoX                  1650
#define ITM_BETAXY                    1651
#define ITM_gammaXY                   1652
#define ITM_GAMMAXY                   1653
#define ITM_GAMMAX                    1654
#define ITM_deltaX                    1655
#define ITM_DELTAPC                   1656
#define ITM_SCATTFACT                 1657
#define ITM_SCATTFACTm                1658
#define ITM_SCATTFACTp                1659
#define ITM_zetaX                     1660
#define ITM_PIn                       1661
#define ITM_SIGMAn                    1662
#define ITM_STDDEV                    1663
#define ITM_STDDEVPOP                 1664
#define ITM_RANI                      1665
#define ITM_PRINTERX                  1666
#define ITM_RANGE                     1667
#define ITM_GETRANGE                  1668
#define ITM_M1X                       1669
#define ITM_XMOD                      1670
#define ITM_toDATE                    1671
#define ITM_toDEG                     1672
#define ITM_toDMS                     1673
#define ITM_toGRAD                    1674
#define ITM_toHR                      1675
#define ITM_toHMS                     1676
#define ITM_toINT                     1677   // JM #
#define ITM_toMULpi                   1678
#define ITM_toPOL                     1679
#define ITM_toRAD                     1680
#define ITM_toREAL                    1681
#define ITM_toREC                     1682
#define ITM_DtoDMS                    1683
#define ITM_SHUFFLE                   1684
#define ITM_PC                        1685
#define ITM_PCMRR                     1686
#define ITM_PCT                       1687
#define ITM_PCSIGMA                   1688
#define ITM_PCPMG                     1689
#define ITM_INTEGRAL                  1690
#define ITM_PMOD                      1691
#define ITM_M_DET                     1692
#define ITM_PARALLEL                  1693
#define ITM_M_TRANSP                  1694
#define ITM_M_INV                     1695
#define ITM_ANGLE                     1696
#define ITM_MULPIto                   1697
#define ITM_PRINTERADV                1698
#define ITM_PRINTERCHAR               1699
#define ITM_PRINTERDLAY               1700
#define ITM_PRINTERLCD                1701
#define ITM_PRINTERMODE               1702
#define ITM_PRINTERPROG               1703
#define ITM_PRINTERR                  1704
#define ITM_PRINTERREGS               1705
#define ITM_PRINTERSTK                1706
#define ITM_PRINTERTAB                1707
#define ITM_PRINTERUSER               1708
#define ITM_PRINTERWIDTH              1709
#define ITM_PRINTERSIGMA              1710
#define ITM_PRINTERHASH               1711
#define ITM_FBR                       1712
#define ITM_UNDO                      1713
#define ITM_PR                        1714
#define ITM_RS                        1715
#define ITM_Not                       1716
#define ITM_yet                       1717
#define ITM_defined                   1718
#define ITM_USERMODE                  1719
#define ITM_CC                        1720
#define ITM_SHIFTf                    1721
#define ITM_SHIFTg                    1722
#define ITM_UP1                       1723
#define ITM_BST                       1724
#define ITM_DOWN1                     1725
#define ITM_SST                       1726
#define ITM_EXIT1                     1727
#define ITM_BACKSPACE                 1728
#define ITM_PRTX                      1729
#define ITM_AIM                       1730
#define ITM_dotD                      1731
#define ITM_SHOW                      1732
#define ITM_SYSTEM                    1733
#define ITM_DMStoD                    1734
#define ITM_VANGLE                    1735
#define ITM_XH                        1736
#define ITM_XRMS                      1737
#define ITM_ACOS                      1738
#define ITM_ASIN                      1739
#define ITM_ATAN                      1740
#define ITM_DET                       1741
#define ITM_INVRT                     1742
#define ITM_TRANS                     1743
#define ITM_XIN                       1744
#define ITM_XOUT                      1745
#define ITM_FG_LINE                   1746   //JM FG              **
#define ITM_FG_DOTS                   1747   //JM FG              **
#define ITM_G_DOUBLETAP               1748   //JM FG              **
#define ITM_1749                      1749
#define ITM_P_ALLREGS                 1750   //JM Print all regs
#define ITM_SI_f                      1751   //JM PRE UNIT
#define ITM_SI_p                      1752   //JM PRE UNIT
#define ITM_SI_n                      1753   //JM PRE UNIT
#define ITM_SI_u                      1754   //JM PRE UNIT
#define ITM_SI_m                      1755   //JM PRE UNIT
#define ITM_SI_k                      1756   //JM PRE UNIT
#define ITM_SI_M                      1757   //JM PRE UNIT
#define ITM_SI_G                      1758   //JM PRE UNIT
#define ITM_SI_T                      1759   //JM PRE UNIT
#define ITM_QOPPA                     1760   //JM GREEK   //NOTE the RANGE STARTS HERE, with +36 for lower case
#define ITM_DIGAMMA                   1761   //JM GREEK
#define ITM_SAMPI                     1762   //JM GREEK   //NOTE the RANGE STOPS HERE, with +36 for lower case
#define ITM_1599                      1763   //JM SPARE
#define ITM_1600                      1764   //JM SPARE
#define ITM_1601                      1765   //JM SPARE
#define ITM_1602                      1766   //JM SPARE
#define ITM_1603                      1767   //JM SPARE
#define ITM_1604                      1768   //JM SPARE
#define ITM_1605                      1769   //JM SPARE
#define ITM_1606                      1770   //JM SPARE
#define ITM_1607                      1771   //JM SPARE
#define ITM_1608                      1772   //JM SPARE
#define ITM_1609                      1773   //JM SPARE
#define ITM_1610                      1774   //JM SPARE
#define ITM_1611                      1775   //JM SPARE
#define ITM_1612                      1776   //JM SPARE
#define ITM_1613                      1777   //JM SPARE
#define ITM_1614                      1778   //JM SPARE
#define ITM_1615                      1779   //JM SPARE
#define ITM_1616                      1780   //JM SPARE
#define ITM_1617                      1781   //JM SPARE
#define ITM_1618                      1782   //JM SPARE
#define ITM_1619                      1783   //JM SPARE
#define ITM_1620                      1784   //JM SPARE
#define ITM_1621                      1785   //JM SPARE
#define ITM_1622                      1786   //JM SPARE
#define ITM_1623                      1787   //JM SPARE
#define ITM_1624                      1788   //JM SPARE
#define ITM_1625                      1789   //JM SPARE
#define ITM_1626                      1790   //JM SPARE
#define ITM_1627                      1791   //JM SPARE
#define ITM_1628                      1792   //JM SPARE
#define ITM_1629                      1793   //JM SPARE
#define ITM_1630                      1794   //JM SPARE
#define ITM_1631                      1795   //JM SPARE
#define ITM_qoppa                     1796   //JM GREEK
#define ITM_digamma                   1797   //JM GREEK
#define ITM_sampi                     1798   //JM GREEK
#define KEY_COMPLEX                   1799   //JM CPX
#define ITM_toPOL2                    1800   //JM
#define ITM_toREC2                    1801   //JM
#define ITM_eRPN_ON                   1802
#define ITM_eRPN_OFF                  1803
#define ITM_ERPN                      1804   //JM eRPN
#define ITM_HOMEx3                    1805   //JM HOME.3
#define ITM_SHTIM                     1806   //JM SHIFT CANCEL
#define ITM_CB_CPXRES                 1807   //JM cb ComplexResult
#define ITM_CB_LEADING_ZERO           1808   //JM cb LeadingZeros
#define CHR_case                      1809   //JM CAPS
#define ITM_BASE_HOME                 1810   //JM BASEMENU
#define ITM_BASE_AHOME                1811   //JM BASEMENU
#define ITM_H_SUMRY                   1812   //JMHOME
#define ITM_H_REPLCA                  1813   //JMHOME
#define ITM_H_FIXED                   1814   //JMHOME
#define ITM_HOMEx3T                   1815   //JM HOME.3T
#define ITM_LARGELI                   1816
#define ITM_PGMTST                    1817   //JM Generic program test
#define ITM_SIGFIG                    1818   //JM SIGFIG
#define ITM_UNIT                      1819   //JM UNIT
#define ITM_ROUND2                    1820
#define ITM_ROUNDI2                   1821
#define ITM_op_a                      1822   //JM OPERATORS
#define ITM_op_a2                     1823   //JM OPERATORS
#define ITM_op_j                      1824   //JM OPERATORS
#define ITM_EE_D2Y                    1825   //JM EE
#define ITM_EE_Y2D                    1826   //JM EE
#define ITM_EE_A2S                    1827   //JM EE
#define ITM_EE_S2A                    1828   //JM EE
#define ITM_EE_EXP_TH                 1829   //JM EE
#define ITM_EE_STO_Z                  1830   //JM EE
#define ITM_EE_RCL_Z                  1831   //JM EE
#define ITM_EE_STO_V                  1832   //JM EE
#define ITM_EE_RCL_V                  1833   //JM EE
#define ITM_EE_STO_I                  1834   //JM EE
#define ITM_EE_RCL_I                  1835   //JM EE
#define ITM_EE_STO_V_I                1836   //JM EE
#define ITM_EE_STO_IR                 1837   //JM EE
#define ITM_EE_STO_V_Z                1838   //JM EE
#define ITM_EE_X2BAL                  1839   //JM EE
#define ITM_DMPMNU                    1840
#define ITM_LI                        1841
#define ITM_2BIN                      1842   //JM BASE
#define ITM_2OCT                      1843   //JM BASE
#define ITM_2DEC                      1844   //JM BASE
#define ITM_2HEX                      1845   //JM BASE
#define ITM_WS8                       1846   //JM BASE
#define ITM_WS16                      1847   //JM BASE
#define ITM_WS32                      1848   //JM BASE
#define ITM_WS64                      1849   //JM BASE
#define ITM_RI                        1850   //JM ROUND
#define ITM_HASH_JM                   1851
#define ITM_GRF_X0                    1852   //JM GRAPH
#define ITM_GRF_X1                    1853   //JM GRAPH
#define ITM_GRF_Y0                    1854   //JM GRAPH
#define ITM_GRF_Y1                    1855   //JM GRAPH
#define ITM_GRF_DX                    1856   //JM GRAPH
#define ITM_GRF_DY                    1857   //JM GRAPH
#define ITM_GRF_HLP                   1858   //JM GRAPH
#define ITM_CLA                       1859   //GRAPH
#define ITM_CLN                       1860   //GRAPH
#define ITM_1861                      1861
#define ITM_1862                      1862
#define CHR_caseUP                    1863   //JM CAPS
#define CHR_caseDN                    1864   //JM CAPS
#define ITM_LISTXY                    1865   //JMSTAT
#define ITM_SH_ERPN                   1866   //JM SHOW
#define ITM_SYS_FREE_RAM              1867   //dr
#define MNU_INL_TST                   1868   //INLINE_TEST
#define ITM_TEST                      1869   //INLINE_TEST
#define ITM_GET_TEST_BS               1870   //INLINE_TEST
#define ITM_SET_TEST_BS               1871   //INLINE_TEST
#define ITM_INP_DEF_DP                1872   //JM INPUT DEFAULT
#define ITM_SH_INP_DEF                1873   //JM INPUT DEFAULT
#define ITM_INP_DEF_CPXDP             1874   //JM INPUT DEFAULT
#define ITM_INP_DEF_SI                1875   //JM INPUT DEFAULT
#define ITM_INP_DEF_LI                1876   //JM INPUT DEFAULT
#define ITM_USER_V43                  1877   //V43
#define KEY_fg                        1878
#define ITM_USER_DEFAULTS             1879   //JM USER
#define ITM_USER_COMPLEX              1880   //JM USER
#define ITM_USER_SHIFTS               1881   //JM USER
#define ITM_USER_RESET                1882   //JM USER
#define ITM_U_KEY_USER                1883   //JM USER
#define ITM_U_KEY_CC                  1884   //JM USER
#define ITM_U_KEY_MM                  1885   //JM USER
#define ITM_U_KEY_SIGMA               1886   //JM USER
#define ITM_U_KEY_PRGM                1887   //JM USER
#define ITM_U_KEY_ALPHA               1888   //JM USER
#define ITM_SH_NORM_E                 1889   //JM USER NORMAL
#define ITM_JM_ASN                    1890   //JM ASSIGN
#define ITM_JM_SEEK                   1891   //JM ASSIGN
#define ITM_INP_DEF_43S               1892   //JM INPUT DEFAULT
#define ITM_XXEQ                      1893
#define ITM_USER_ALPHA                1894   //JM USER
#define ITM_USER_GSHFT                1895   //JM USER
#define ITM_USER_CC                   1896   //JM USER
#define ITM_USER_MYM                  1897   //JM USER
#define ITM_USER_PRGM                 1898   //JM USER
#define ITM_USER_USER                 1899   //JM USER
#define ITM_USER_SIGMAPLUS            1900   //JM USER
#define ITM_USER_V43MIN               1901   //V43
#define ITM_USER_SHIFTS2              1902   //V43
#define ITM_USER_HOME                 1903   //JM USER
#define ITM_USER_WP43S                1904   //V43
#define ITM_USER_DM42                 1905   //V43
#define ITM_USER_C43                  1906
#define ITM_GET_NORM_E                1907   //JM USER NORMAL
#define MNU_GRAPH                     1908   //JM GRAPH
#define MNU_ASN_N                     1909   //JM USER NORMAL
#define MNU_HOME                      1910   //JM HOME
#define MNU_ALPHA                     1911   //JM ALPHA
#define MNU_BASE                      1912   //JM BASE
#define MNU_XEQ                       1913   //JM EXEC
#define MNU_EE                        1914   //JM EE
#define ITM_T_UP_ARROW                1915
#define MNU_ASN                       1916   //JM USER
#define ITM_T_DOWN_ARROW              1917
#define ITM_T_HOME                    1918
#define ITM_T_END                     1919
#define MNU_ASN_U                     1920   //V43
#define MNU_T_EDIT                    1921
#define MNU_XXEQ                      1922
#define ITM_RNG                       1923   //JM RANGE
#define ITM_FLGSV                     1924
#define ITM_CPXI                      1925
#define ITM_CPXJ                      1926
#define ITM_SSIZE4                    1927
#define ITM_SSIZE8                    1928
#define ITM_CB_SPCRES                 1929
#define ITM_CFG                       1930   //JM Replacements
#define ITM_CLK12                     1931   //JM Replacements
#define ITM_CLK24                     1932   //JM Replacements
#define ITM_MULTCR                    1933
#define ITM_MULTDOT                   1934
#define ITM_POLAR                     1935   //JM Replacements
#define ITM_RDXCOM                    1936   //JM Replacements
#define ITM_RDXPER                    1937   //JM Replacements
#define ITM_RECT                      1938   //JM Replacements
#define ITM_SCIOVR                    1939   //JM Replacements
#define ITM_ENGOVR                    1940   //JM Replacements
#define ITM_T_LEFT_ARROW              1941
#define ITM_T_RIGHT_ARROW             1942
#define ITM_T_LLEFT_ARROW             1943
#define ITM_T_RRIGHT_ARROW            1944
#define ITM_XNEW                      1945
#define ITM_XEDIT                     1946
#define ITM_ms                        1947   //JM DMS HMS
#define ITM_DEG2                      1948
#define ITM_DMS2                      1949
#define ITM_GRAD2                     1950
#define ITM_MULPI2                    1951
#define ITM_RAD2                      1952
#define ITM_HMS2                      1953
#define USER_PRIM00U                  1954   //JM USER MODE TEST
#define USER_SFTf00U                  1955   //JM USER MODE TEST
#define USER_SFTg00U                  1956   //JM USER MODE TEST
#define USER_PRIM01U                  1957   //JM USER MODE TEST
#define USER_SFTf01U                  1958   //JM USER MODE TEST
#define USER_SFTg01U                  1959   //JM USER MODE TEST
#define USER_PRIM02U                  1960   //JM USER MODE TEST
#define USER_SFTf02U                  1961   //JM USER MODE TEST
#define USER_SFTg02U                  1962   //JM USER MODE TEST
#define USER_PRIM03U                  1963   //JM USER MODE TEST
#define USER_SFTf03U                  1964   //JM USER MODE TEST
#define USER_SFTg03U                  1965   //JM USER MODE TEST
#define USER_PRIM04U                  1966   //JM USER MODE TEST
#define USER_SFTf04U                  1967   //JM USER MODE TEST
#define USER_SFTg04U                  1968   //JM USER MODE TEST
#define USER_PRIM05U                  1969   //JM USER MODE TEST
#define USER_SFTf05U                  1970   //JM USER MODE TEST
#define USER_SFTg05U                  1971   //JM USER MODE TEST
#define USER_PRIM06U                  1972   //JM USER MODE TEST
#define USER_SFTf06U                  1973   //JM USER MODE TEST
#define USER_SFTg06U                  1974   //JM USER MODE TEST
#define USER_PRIM07U                  1975   //JM USER MODE TEST
#define USER_SFTf07U                  1976   //JM USER MODE TEST
#define USER_SFTg07U                  1977   //JM USER MODE TEST
#define USER_PRIM08U                  1978   //JM USER MODE TEST
#define USER_SFTf08U                  1979   //JM USER MODE TEST
#define USER_SFTg08U                  1980   //JM USER MODE TEST
#define USER_PRIM09U                  1981   //JM USER MODE TEST
#define USER_SFTf09U                  1982   //JM USER MODE TEST
#define USER_SFTg09U                  1983   //JM USER MODE TEST
#define USER_PRIM10U                  1984   //JM USER MODE TEST
#define USER_SFTf10U                  1985   //JM USER MODE TEST
#define USER_SFTg10U                  1986   //JM USER MODE TEST
#define USER_PRIM11U                  1987   //JM USER MODE TEST
#define USER_SFTf11U                  1988   //JM USER MODE TEST
#define USER_SFTg11U                  1989   //JM USER MODE TEST
#define USER_PRIM12U                  1990   //JM USER MODE TEST
#define USER_SFTf12U                  1991   //JM USER MODE TEST
#define USER_SFTg12U                  1992   //JM USER MODE TEST
#define USER_PRIM13U                  1993   //JM USER MODE TEST
#define USER_SFTf13U                  1994   //JM USER MODE TEST
#define USER_SFTg13U                  1995   //JM USER MODE TEST
#define USER_PRIM14U                  1996   //JM USER MODE TEST
#define USER_SFTf14U                  1997   //JM USER MODE TEST
#define USER_SFTg14U                  1998   //JM USER MODE TEST
#define USER_PRIM15U                  1999   //JM USER MODE TEST
#define USER_SFTf15U                  2000   //JM USER MODE TEST
#define USER_SFTg15U                  2001   //JM USER MODE TEST
#define USER_PRIM16U                  2002   //JM USER MODE TEST
#define USER_SFTf16U                  2003   //JM USER MODE TEST
#define USER_SFTg16U                  2004   //JM USER MODE TEST
#define USER_PRIM17U                  2005   //JM USER MODE TEST
#define USER_SFTf17U                  2006   //JM USER MODE TEST
#define USER_SFTg17U                  2007   //JM USER MODE TEST
#define USER_PRIM18U                  2008   //JM USER MODE TEST
#define USER_SFTf18U                  2009   //JM USER MODE TEST
#define USER_SFTg18U                  2010   //JM USER MODE TEST
#define USER_PRIM19U                  2011   //JM USER MODE TEST
#define USER_SFTf19U                  2012   //JM USER MODE TEST
#define USER_SFTg19U                  2013   //JM USER MODE TEST
#define USER_PRIM20U                  2014   //JM USER MODE TEST
#define USER_SFTf20U                  2015   //JM USER MODE TEST
#define USER_SFTg20U                  2016   //JM USER MODE TEST
#define USER_PRIM21U                  2017   //JM USER MODE TEST
#define USER_SFTf21U                  2018   //JM USER MODE TEST
#define USER_SFTg21U                  2019   //JM USER MODE TEST
#define USER_PRIM22U                  2020   //JM USER MODE TEST
#define USER_SFTf22U                  2021   //JM USER MODE TEST
#define USER_SFTg22U                  2022   //JM USER MODE TEST
#define USER_PRIM23U                  2023   //JM USER MODE TEST
#define USER_SFTf23U                  2024   //JM USER MODE TEST
#define USER_SFTg23U                  2025   //JM USER MODE TEST
#define USER_PRIM24U                  2026   //JM USER MODE TEST
#define USER_SFTf24U                  2027   //JM USER MODE TEST
#define USER_SFTg24U                  2028   //JM USER MODE TEST
#define USER_PRIM25U                  2029   //JM USER MODE TEST
#define USER_SFTf25U                  2030   //JM USER MODE TEST
#define USER_SFTg25U                  2031   //JM USER MODE TEST
#define USER_PRIM26U                  2032   //JM USER MODE TEST
#define USER_SFTf26U                  2033   //JM USER MODE TEST
#define USER_SFTg26U                  2034   //JM USER MODE TEST
#define USER_PRIM27U                  2035   //JM USER MODE TEST
#define USER_SFTf27U                  2036   //JM USER MODE TEST
#define USER_SFTg27U                  2037   //JM USER MODE TEST
#define USER_PRIM28U                  2038   //JM USER MODE TEST
#define USER_SFTf28U                  2039   //JM USER MODE TEST
#define USER_SFTg28U                  2040   //JM USER MODE TEST
#define USER_PRIM29U                  2041   //JM USER MODE TEST
#define USER_SFTf29U                  2042   //JM USER MODE TEST
#define USER_SFTg29U                  2043   //JM USER MODE TEST
#define USER_PRIM30U                  2044   //JM USER MODE TEST
#define USER_SFTf30U                  2045   //JM USER MODE TEST
#define USER_SFTg30U                  2046   //JM USER MODE TEST
#define USER_PRIM31U                  2047   //JM USER MODE TEST
#define USER_SFTf31U                  2048   //JM USER MODE TEST
#define USER_SFTg31U                  2049   //JM USER MODE TEST
#define USER_PRIM32U                  2050   //JM USER MODE TEST
#define USER_SFTf32U                  2051   //JM USER MODE TEST
#define USER_SFTg32U                  2052   //JM USER MODE TEST
#define USER_PRIM33U                  2053   //JM USER MODE TEST
#define USER_SFTf33U                  2054   //JM USER MODE TEST
#define USER_SFTg33U                  2055   //JM USER MODE TEST
#define USER_PRIM34U                  2056   //JM USER MODE TEST
#define USER_SFTf34U                  2057   //JM USER MODE TEST
#define USER_SFTg34U                  2058   //JM USER MODE TEST
#define USER_PRIM35U                  2059   //JM USER MODE TEST
#define USER_SFTf35U                  2060   //JM USER MODE TEST
#define USER_SFTg35U                  2061   //JM USER MODE TEST
#define USER_PRIM36U                  2062   //JM USER MODE TEST
#define USER_SFTf36U                  2063   //JM USER MODE TEST
#define USER_SFTg36U                  2064   //JM USER MODE TEST
#define ITM_X_P1                      2065   //JM EXEC
#define ITM_X_P2                      2066   //JM EXEC
#define ITM_X_P3                      2067   //JM EXEC
#define ITM_X_P4                      2068   //JM EXEC
#define ITM_X_P5                      2069   //JM EXEC
#define ITM_X_P6                      2070   //JM EXEC
#define ITM_X_f1                      2071   //JM EXEC
#define ITM_X_f2                      2072   //JM EXEC
#define ITM_X_f3                      2073   //JM EXEC
#define ITM_X_f4                      2074   //JM EXEC
#define ITM_X_f5                      2075   //JM EXEC
#define ITM_X_f6                      2076   //JM EXEC
#define ITM_X_g1                      2077   //JM EXEC
#define ITM_X_g2                      2078   //JM EXEC
#define ITM_X_g3                      2079   //JM EXEC
#define ITM_X_g4                      2080   //JM EXEC
#define ITM_X_g5                      2081   //JM EXEC
#define ITM_X_g6                      2082   //JM EXEC
#define ITM_XSAVE                     2083
#define ITM_XLOAD                     2084
#define ITM_FB00                      2085   //JM SHOI
#define ITM_FB01                      2086   //JM SHOI
#define ITM_FB02                      2087   //JM SHOI
#define ITM_FB03                      2088   //JM SHOI
#define ITM_FB04                      2089   //JM SHOI
#define ITM_FB05                      2090   //JM SHOI
#define ITM_FB06                      2091   //JM SHOI
#define ITM_FB07                      2092   //JM SHOI
#define ITM_FB08                      2093   //JM SHOI
#define ITM_FB09                      2094   //JM SHOI
#define ITM_FB10                      2095   //JM SHOI
#define ITM_FB11                      2096   //JM SHOI
#define ITM_FB12                      2097   //JM SHOI
#define ITM_FB13                      2098   //JM SHOI
#define ITM_FB14                      2099   //JM SHOI
#define ITM_FB15                      2100   //JM SHOI
#define ITM_FB16                      2101   //JM SHOI
#define ITM_FB17                      2102   //JM SHOI
#define ITM_FB18                      2103   //JM SHOI
#define ITM_FB19                      2104   //JM SHOI
#define ITM_FB20                      2105   //JM SHOI
#define ITM_FB21                      2106   //JM SHOI
#define ITM_FB22                      2107   //JM SHOI
#define ITM_FB23                      2108   //JM SHOI
#define ITM_FB24                      2109   //JM SHOI
#define ITM_FB25                      2110   //JM SHOI
#define ITM_FB26                      2111   //JM SHOI
#define ITM_FB27                      2112   //JM SHOI
#define ITM_FB28                      2113   //JM SHOI
#define ITM_FB29                      2114   //JM SHOI
#define ITM_FB30                      2115   //JM SHOI
#define ITM_FB31                      2116   //JM SHOI
#define ITM_FB32                      2117   //JM SHOI
#define ITM_FB33                      2118   //JM SHOI
#define ITM_FB34                      2119   //JM SHOI
#define ITM_FB35                      2120   //JM SHOI
#define ITM_FB36                      2121   //JM SHOI
#define ITM_FB37                      2122   //JM SHOI
#define ITM_FB38                      2123   //JM SHOI
#define ITM_FB39                      2124   //JM SHOI
#define ITM_FB40                      2125   //JM SHOI
#define ITM_FB41                      2126   //JM SHOI
#define ITM_FB42                      2127   //JM SHOI
#define ITM_FB43                      2128   //JM SHOI
#define ITM_FB44                      2129   //JM SHOI
#define ITM_FB45                      2130   //JM SHOI
#define ITM_FB46                      2131   //JM SHOI
#define ITM_FB47                      2132   //JM SHOI
#define ITM_FB48                      2133   //JM SHOI
#define ITM_FB49                      2134   //JM SHOI
#define ITM_FB50                      2135   //JM SHOI
#define ITM_FB51                      2136   //JM SHOI
#define ITM_FB52                      2137   //JM SHOI
#define ITM_FB53                      2138   //JM SHOI
#define ITM_FB54                      2139   //JM SHOI
#define ITM_FB55                      2140   //JM SHOI
#define ITM_FB56                      2141   //JM SHOI
#define ITM_FB57                      2142   //JM SHOI
#define ITM_FB58                      2143   //JM SHOI
#define ITM_FB59                      2144   //JM SHOI
#define ITM_FB60                      2145   //JM SHOI
#define ITM_FB61                      2146   //JM SHOI
#define ITM_FB62                      2147   //JM SHOI
#define ITM_FB63                      2148   //JM SHOI
#define ITM_S06                       2149   //JM SHOI
#define ITM_S08                       2150   //JM SHOI
#define ITM_S16                       2151   //JM SHOI
#define ITM_S32                       2152   //JM SHOI
#define ITM_S64                       2153   //JM SHOI
#define ITM_U06                       2154   //JM SHOI
#define ITM_U08                       2155   //JM SHOI
#define ITM_U16                       2156   //JM SHOI
#define ITM_U32                       2157   //JM SHOI
#define ITM_U64                       2158   //JM SHOI
#define ITM_SL1                       2159   //JM SHOI
#define ITM_SR1                       2160   //JM SHOI
#define ITM_RL1                       2161   //JM SHOI
#define ITM_RR1                       2162   //JM SHOI
#define ITM_FWORD                     2163   //JM SHOI
#define ITM_FBYTE                     2164   //JM SHOI
#define ITM_CLAIM                     2165
#define ITM_SHOIREP                   2166   //JM SHOI
#define ITM_SCALE                     2167   //JM GRAPHING
#define ITM_PLOTLS                    2168
#define ITM_PLINE                     2169   //GRAPH
#define ITM_PCROS                     2170   //GRAPH
#define ITM_PBOX                      2171   //GRAPH
#define ITM_VECT                      2172   //JMVECT
#define ITM_NVECT                     2173   //JMVECT
#define ITM_EXTX                      2174
#define ITM_EXTY                      2175
#define ITM_PLOTJM                    2176
#define ITM_GRAPH                     2177   //JM GRAPHING
#define ITM_DEMO1                     2178   //JM USER
#define ITM_DEMO2                     2179   //JM USER
#define ITM_DEMO3                     2180   //JM USER
#define ITM_DEMO4                     2181   //JM USER
#define ITM_DEMO5                     2182   //JM USER
#define ITM_DEMO6                     2183   //JM USER
#define ITM_INTG                      2184   //GRAPH
#define ITM_DIFF                      2185   //GRAPH
#define ITM_RMS                       2186   //GRAPH
#define ITM_SHADE                     2187   //GRAPH
#define CHR_num                       2188   //JM CAPS
#define CHR_numL                      2189   //JM CAPS
#define CHR_numU                      2190   //JM CAPS
#define ITM_EEXCHR                    2191

#define LAST_ITEM                     2192   //JM

#define NOPARAM                       9876 // Item for function who don't need an item
#define CONFIRMED                     9877 // Confirmation for RESET, CLPALL, CLALL
#define NOT_CONFIRMED                 9878 // Confirmation for RESET, CLPALL, CLALL
#define ITM_PROD_SIGN                 9999 // Multiplication sign × or ·

void reallyRunFunction(int16_t func, uint16_t param);
void runFunction      (int16_t func);
void fnToBeCoded      (void);
void itemToBeCoded    (uint16_t unusedParamButMandatory);
void fnNop            (uint16_t unusedParamButMandatory);
