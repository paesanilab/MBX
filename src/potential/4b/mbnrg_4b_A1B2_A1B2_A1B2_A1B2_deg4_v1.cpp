#include "mbnrg_4b_A1B2_A1B2_A1B2_A1B2_deg4_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B2_A1B2_A1B2_A1B2_deg4 {

mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1::mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1(const std::string mon1, const std::string mon2, const std::string mon3, const std::string mon4) {

    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o" and mon4 == "h2o") {
        coefficients = std::vector<double> {
             7.940769271483272e-09, // 0
            -1.308543373753519e-06, // 1
            -2.164263341372207e-08, // 2
            -1.810063884967643e-09, // 3
             6.519393065732154e-06, // 4
             8.960406397632067e-07, // 5
            -4.952465560338112e-06, // 6
             5.647247647156101e-10, // 7
             2.387623936564725e-04, // 8
             3.361028773031729e+00, // 9
            -7.053161675283932e-09, // 10
             4.111819271479543e-02, // 11
            -3.916350798674231e+01, // 12
            -6.344403192759432e-07, // 13
            -2.003599054195958e-06, // 14
             7.780916244914565e-02, // 15
            -4.502309503364172e-09, // 16
             1.643825813826297e-05, // 17
            -6.003761960131506e-08, // 18
            -5.260448484722342e-08, // 19
            -3.932685889700047e-09, // 20
            -3.234186100105049e+01, // 21
             1.011551429422586e-08, // 22
            -5.972999965160629e-01, // 23
             2.174417184194354e-09, // 24
             8.731478607507632e-10, // 25
             7.484474403682481e-10, // 26
            -5.631860274045211e-09, // 27
             1.167222581055614e-08, // 28
            -4.568185096917268e-05, // 29
            -2.454170165724778e-10, // 30
            -6.228214880627530e-09, // 31
            -2.133443800964564e-04, // 32
            -1.205716769597618e-06, // 33
            -1.078413380297127e-05, // 34
             5.595549213363116e-05, // 35
             1.288907707738474e-04, // 36
             3.512184309675468e-08, // 37
            -3.088275467295983e-02, // 38
             5.079103567476220e-06, // 39
            -3.115081778930052e-10, // 40
            -8.333810315869579e+00, // 41
             4.173295428079409e+00, // 42
            -1.107108531668251e-01, // 43
             2.376297296665159e-04, // 44
            -9.738722475807437e-09, // 45
            -3.074609236599239e-06, // 46
             1.531321153879269e-06, // 47
            -5.750384460761033e-01, // 48
             1.700146672730521e-09, // 49
            -1.477476518626911e-04, // 50
            -1.178264508017876e-09, // 51
             5.177564114016027e+00, // 52
             1.591944162008603e-09, // 53
             2.484974174526742e-04, // 54
            -2.894425472123881e-09, // 55
            -4.973984240911673e-02, // 56
             5.146693705902546e-07, // 57
             4.638800968886990e-02, // 58
            -1.061681366820359e-04, // 59
             1.234112101769795e+00, // 60
             1.773820401774182e-08, // 61
             1.933646538776827e-06, // 62
             1.842653736000552e-08, // 63
            -6.456288261577332e-09, // 64
            -1.142173799695980e-06, // 65
            -4.967206590509520e+00, // 66
             2.265051803978840e-09, // 67
            -8.565217212784522e-09, // 68
            -8.986547311994125e-09, // 69
             1.330094792693770e-08, // 70
             1.661077945032634e-08, // 71
             1.293646711400440e-08, // 72
             2.100062963148938e-09, // 73
             3.378059276539613e-02, // 74
            -2.197353598014913e-08, // 75
            -2.416522236038730e-09, // 76
            -2.852360623601877e-04, // 77
            -1.721786248849359e-02, // 78
             2.508890835390631e-09, // 79
             1.375391714251065e-08, // 80
            -2.190456603558948e-05, // 81
             2.932277793508922e-04, // 82
            -8.941715353824882e-05, // 83
             2.083411526217979e+01, // 84
             2.626880281517735e-07, // 85
            -1.883815390759838e-05, // 86
            -3.749235497136943e-09, // 87
            -8.780570892492081e-07, // 88
            -1.999684714880867e-07, // 89
             1.799819150441666e-08, // 90
             5.904959661317801e-08, // 91
             4.722636460804645e-06, // 92
            -2.437550877544955e-04, // 93
            -6.966381562754453e-09, // 94
            -1.310960468973423e-04, // 95
            -2.010675986742500e-08, // 96
             1.594458245572528e-09, // 97
             2.392790597066724e-08, // 98
             1.091852823815822e-04, // 99
             5.732367158210146e-09, // 100
             2.221387612591498e+01, // 101
            -2.305604899373797e-06, // 102
            -5.325347157271481e-04, // 103
             1.148653600004769e-09, // 104
            -8.976177451474094e-05, // 105
             1.785197041568990e-08, // 106
             4.837263776002262e-09, // 107
             1.325762515671255e-01, // 108
             1.479123233403864e-06, // 109
             6.646477860299230e-09, // 110
             9.687762354441326e-03, // 111
             1.111693432496619e-09, // 112
             7.968658491230348e-07, // 113
            -1.823411286644838e-08, // 114
             1.611349340805835e-09, // 115
            -1.771633075218786e-06, // 116
             1.452402403746529e-04, // 117
            -2.908069595281775e-08, // 118
             3.188632457616307e-05, // 119
            -9.811213354766743e-08, // 120
            -9.699092750755522e-09, // 121
             3.265890075048095e-08, // 122
             2.000020823230470e+01, // 123
            -5.235916075955398e-09, // 124
            -2.945440130878038e-10, // 125
             9.882696256041188e-05, // 126
            -1.545319804282028e-06, // 127
            -1.310671587894364e-04, // 128
            -6.711078518848293e-03, // 129
            -3.602671095584134e-04, // 130
             1.329167524848583e-08, // 131
             8.773795762509750e-05, // 132
            -3.879601205436235e-09, // 133
             3.515477020006526e-07, // 134
             5.203961504238704e-06, // 135
            -4.612966760221520e-05, // 136
             5.509325691530785e-10, // 137
            -5.940812665316705e-09, // 138
             1.180952819154054e-08, // 139
            -4.333123350035793e-09, // 140
             1.663879377301124e-06, // 141
             1.834691496436666e-08, // 142
            -1.933002991301806e-08, // 143
             8.471339765555301e-06, // 144
             1.296643168734032e-04, // 145
             1.263705217790441e-05, // 146
             2.621941279938935e-06, // 147
            -4.257967965162770e-07, // 148
            -8.545200466130382e+00, // 149
            -6.233649036605585e-05, // 150
            -4.475237124715867e-05, // 151
            -2.661727038767150e-05, // 152
             3.807153599971428e-05, // 153
             7.065789856245368e-05, // 154
             1.609245900962595e-02, // 155
            -3.507792230360766e-05, // 156
            -3.167031145626700e-02, // 157
            -8.142852026597308e-05, // 158
            -1.395000905574919e+00, // 159
            -1.856749607245996e-05, // 160
            -6.721576988372336e-06, // 161
            -1.261577294911556e-03, // 162
            -4.634276411769761e-02, // 163
             2.036174881975887e-05, // 164
             2.601604033209130e-05, // 165
            -1.845692611425422e+00, // 166
            -3.656195008310825e-05, // 167
            -2.403039938783209e-05, // 168
             8.608670545095427e-05, // 169
            -9.704774725307526e+00, // 170
            -6.222037248967863e-05, // 171
             4.007770731865966e-05, // 172
             4.895007017723799e-05, // 173
             3.365489460129886e-05, // 174
            -6.346011191606378e-02, // 175
            -1.348514161840990e-05, // 176
            -6.505272718722040e-03, // 177
            -2.293811939201373e-05, // 178
            -1.479986643835075e-05, // 179
             6.006383087687887e-06, // 180
             1.129757963005443e+01, // 181
             2.439647200433787e-05, // 182
             5.426237022917788e-05, // 183
             6.324176024350421e-02, // 184
             2.216337241440102e+01, // 185
             5.640801481726029e-05, // 186
            -1.020458649855737e-04, // 187
            -3.273728178889124e-02, // 188
            -3.704026627522380e-03, // 189
            -8.020236699388984e-05, // 190
            -3.740460633518908e-05, // 191
             3.947639583235708e-02, // 192
            -4.535221959095647e+00, // 193
            -9.783125012273459e-04, // 194
            -6.832763718174018e+00, // 195
             5.732798681152184e-05, // 196
            -5.273626004662320e+00, // 197
             2.599396677307623e-05, // 198
            -2.856453824681273e-06, // 199
             1.337582852328657e+00, // 200
            -7.515505466261136e-06, // 201
            -3.754265803244224e-02, // 202
            -3.112994879670650e-05, // 203
            -1.263672484019480e-04, // 204
            -7.024469922483293e-03, // 205
             5.005945407685675e-05, // 206
             1.306550722665870e+01, // 207
             4.030884503023307e+00, // 208
             1.752950158393794e-05, // 209
            -3.627501965195351e-05, // 210
            -3.098969173112382e-05, // 211
            -2.750225926084171e-05, // 212
            -7.928596955079192e-05, // 213
             4.054425462940335e-05, // 214
             8.276235120755229e-05, // 215
             5.153910147185758e-04, // 216
             4.875469064269978e-05, // 217
             4.904023564430088e-05, // 218
             1.035246028435747e-05, // 219
            -1.179752574074740e+01, // 220
            -5.537320277985059e-01, // 221
            -1.351235776780836e-05, // 222
            -2.176209723013441e-05, // 223
             2.317949983773617e-05, // 224
             2.221500318967394e-05, // 225
            -2.492486748214802e-05, // 226
             9.329221687334943e-05, // 227
            -3.102043485034507e-05, // 228
             8.149375308040772e+00, // 229
            -2.887069523009860e-05, // 230
             1.153994828059509e+01, // 231
             3.711224095317921e-02, // 232
             4.251894222065951e-06, // 233
            -3.222163597126400e-05, // 234
             9.364035036936852e-06, // 235
            -5.969916557301250e-05, // 236
             8.873468012103876e-05, // 237
             1.076220391901718e-02, // 238
            -1.622399243438784e-06, // 239
            -6.752064695405298e-01, // 240
             4.109820382191887e+00, // 241
            -4.590766794543281e-05, // 242
             5.679328857266396e+00, // 243
             7.156544771220760e-06, // 244
             2.572037802759554e-05, // 245
            -1.142424644480897e-06, // 246
            -1.336824141328905e+01, // 247
             1.037487681998144e-05, // 248
             7.510643274383859e-05, // 249
             1.193949031784405e-05, // 250
            -5.804148165534833e-06, // 251
             2.549833708281636e+00, // 252
             3.632286049655579e+00, // 253
            -2.398959162982429e+00, // 254
            -4.237313705442400e+00, // 255
             6.468578636173637e-02, // 256
            -1.241227226117188e+01, // 257
             4.548406759601207e-05, // 258
             7.821930694599419e+00, // 259
             2.641396687845862e-04, // 260
            -2.907894420066538e-05, // 261
            -4.438352722551246e+00, // 262
            -2.371616923486868e-03, // 263
            -1.882579323981556e-05, // 264
             2.561514697711602e-05, // 265
             7.877261010006594e-05, // 266
            -6.121358500559075e-02, // 267
            -1.926098809718961e-05, // 268
             3.802150403423436e-05, // 269
             3.114176166208156e-06, // 270
            -1.058826029207436e-04, // 271
            -3.039952984666560e-05, // 272
            -6.489634247977940e-05, // 273
             1.918233736634486e-05, // 274
            -3.668189805867121e-02, // 275
            -7.530154128324692e-05, // 276
             2.034351691379850e-05, // 277
            -9.954668828905056e-05, // 278
             6.958840782660675e-05, // 279
             2.219188043435459e-05, // 280
            -6.166212226718580e-05, // 281
             2.737623429923241e+00, // 282
            -4.453143339610110e-05, // 283
            -5.041971881768440e-05, // 284
             9.665913739506538e-03, // 285
             3.813532025260093e-05, // 286
            -3.138210762282829e-05, // 287
            -1.313407140123328e-01, // 288
             6.933225496973336e-02, // 289
             4.310767545629371e-05, // 290
            -1.853177934979523e+00, // 291
             2.593132771731976e-02, // 292
             6.025700260365594e-06, // 293
            -7.187093917015840e-06, // 294
            -2.327875336328726e-05, // 295
             9.880053743678392e-06, // 296
             7.081997371248198e-05, // 297
            -2.585634423777648e+00, // 298
             7.528580622272975e-05, // 299
            -5.936800210755447e-05, // 300
             6.166624482042652e+00, // 301
             5.824137044592811e+00, // 302
            -1.772771595602180e-02, // 303
             3.793821502422344e-05, // 304
             6.482338882798286e-05, // 305
             6.959347917458476e+00, // 306
             7.711354494783998e-05, // 307
            -5.458505548670440e-01, // 308
             2.299401657803538e-05, // 309
            -3.100713170164799e+00, // 310
             6.788324717830830e-03, // 311
            -1.177360403206733e+01, // 312
             1.296020638621023e-05, // 313
            -9.511427455833620e-05, // 314
             1.037499809726796e+01, // 315
             2.005318216707462e-05, // 316
             4.119925714136440e-02, // 317
            -1.197883505017106e-04, // 318
             2.523274230822487e-02, // 319
             2.081168229603635e+00, // 320
            -4.845042211878241e-05, // 321
             1.049853924581884e+01, // 322
            -1.524344016015383e+01, // 323
            -4.439875189581099e-02, // 324
             1.223681623043137e-02, // 325
             4.437321462112881e-05, // 326
             2.291019202119967e-05, // 327
            -3.400515605596607e-05, // 328
            -2.095317184551895e-02, // 329
            -1.889994761328485e-02, // 330
            -5.298450144260863e-05, // 331
             1.119220403905397e-04, // 332
            -1.031927171081692e-02, // 333
             1.555055893559293e-05, // 334
            -2.450615816846416e-05, // 335
            -4.837543450460238e+00, // 336
            -7.169207944124395e-03, // 337
             3.173652128236224e+00, // 338
            -1.064439398766668e-02, // 339
             5.705786217339821e+00, // 340
             2.177853811609141e+00, // 341
            -7.393021341046688e-05, // 342
            -1.746583239730287e-05, // 343
             2.789303048428319e+00, // 344
             4.610815934787474e-05, // 345
            -5.312016831839073e+00, // 346
             6.419799172087585e-05, // 347
             3.234895848496792e-05, // 348
            -9.788768635204793e-03, // 349
            -1.892170019384881e-02, // 350
            -1.511058434372976e+00, // 351
             3.816520386730886e-05, // 352
             5.069947869339917e-05, // 353
             1.122765757783485e-03, // 354
             1.894356736050253e-05, // 355
             1.283723190014772e-05, // 356
             1.848411713029026e-01, // 357
            -5.064266471027513e+00, // 358
             3.797978970156823e-05, // 359
            -3.210844571222906e-02, // 360
             3.056841037569512e-05, // 361
            -2.526737027067314e-06, // 362
             1.086048885339735e-05, // 363
            -8.848436605221174e+00, // 364
            -1.779412859801395e-05, // 365
             1.735507914994976e-05, // 366
             6.482810968192799e-05, // 367
             9.139281855939414e-02, // 368
            -3.447056568130623e-05, // 369
            -3.049358353709739e+00, // 370
            -2.992806006744686e+00, // 371
            -1.030559643265373e-04, // 372
            -6.789766922253065e-05, // 373
             6.655452187935468e+00, // 374
             5.407520265858617e-02, // 375
            -2.161673182397257e-05, // 376
            -1.051003137850627e-04, // 377
             5.425117397785169e+00, // 378
             1.941252648837079e+00, // 379
             5.673694598743683e+00, // 380
             2.990598296454687e-02, // 381
             2.814519297701557e-01, // 382
             2.136599329594506e-05, // 383
             4.653831670699373e-07, // 384
            -5.401862109349354e-02, // 385
             3.482740952443937e-05, // 386
            -1.499090967223726e-05, // 387
             2.169662405529363e-05, // 388
             7.722523041576796e-05, // 389
             6.720733047356461e-05, // 390
             3.638166474185834e-05, // 391
            -3.106135079094711e-05, // 392
            -5.921885259384431e-05, // 393
             5.064041350100954e-05, // 394
            -7.305201958148150e+00, // 395
            -5.170947450556033e-05, // 396
             9.899420718424392e-03}; // 397
    m_k_x_inter_A_A_0 =  9.036577471971099e-01; // A^(-1))
    m_d_x_inter_A_A_0 =  3.185476180052597e+00; // A^(-1))
    m_k_x_intra_A_B_1 =  3.892654993081070e-02; // A^(-1))
    m_d_x_intra_A_B_1 =  2.395318740980312e+00; // A^(-1))
    m_k_x_inter_A_B_0 =  2.009195264499977e+00; // A^(-1))
    m_d_x_inter_A_B_0 =  4.480728401443660e+00; // A^(-1))
    m_k_x_intra_B_B_1 =  1.516518448888630e-02; // A^(-1))
    m_d_x_intra_B_B_1 =  2.461029162923705e+00; // A^(-1))
    m_k_x_inter_B_B_0 =  1.885889492099722e+00; // A^(-1))
    m_d_x_inter_B_B_0 =  4.882104279689145e+00; // A^(-1))
    m_ri =  5.000000000000000e+00; // A
    m_ro =  6.000000000000000e+00; // A

    } // end if mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o" and mon4 == "h2o"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1::f_switch(const double r, double& g)
{
    if (r > m_ro) {
        g = 0.0;
        return 0.0;
    } else if (r > m_ri) {
        const double t1 = M_PI/(m_ro - m_ri);
        const double x = (r - m_ri)*t1;
        g = - std::sin(x)*t1/2.0;
        return (1.0 + std::cos(x))/2.0;
    } else {
        g = 0.0;
        return 1.0;
    }
}

//----------------------------------------------------------------------------//

 double mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1::eval(const double *xyz1, const double *xyz2, const double *xyz3, const double *xyz4, const size_t n) {
    std::vector<double> energies(n,0.0);
    std::vector<double> energies_sw(n,0.0);

    std::vector<double> xyz(36);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 9*j;
        const double *mon2 = xyz2 + 9*j;
        const double *mon3 = xyz3 + 9*j;
        const double *mon4 = xyz4 + 9*j;


        const double d12[3] =
                         {mon1[0] - mon2[0],
                          mon1[1] - mon2[1],
                          mon1[2] - mon2[2]};

        const double d12rsq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double d12r = std::sqrt(d12rsq);

        const double d13[3] =
                         {mon1[0] - mon3[0],
                          mon1[1] - mon3[1],
                          mon1[2] - mon3[2]};

        const double d13rsq = d13[0]*d13[0] + d13[1]*d13[1] + d13[2]*d13[2];
        const double d13r = std::sqrt(d13rsq);

        const double d14[3] =
                         {mon1[0] - mon4[0],
                          mon1[1] - mon4[1],
                          mon1[2] - mon4[2]};

        const double d14rsq = d14[0]*d14[0] + d14[1]*d14[1] + d14[2]*d14[2];
        const double d14r = std::sqrt(d14rsq);

        const double d23[3] =
                         {mon2[0] - mon3[0],
                          mon2[1] - mon3[1],
                          mon2[2] - mon3[2]};

        const double d23rsq = d23[0]*d23[0] + d23[1]*d23[1] + d23[2]*d23[2];
        const double d23r = std::sqrt(d23rsq);

        const double d24[3] =
                         {mon2[0] - mon4[0],
                          mon2[1] - mon4[1],
                          mon2[2] - mon4[2]};

        const double d24rsq = d24[0]*d24[0] + d24[1]*d24[1] + d24[2]*d24[2];
        const double d24r = std::sqrt(d24rsq);

        const double d34[3] =
                         {mon3[0] - mon4[0],
                          mon3[1] - mon4[1],
                          mon3[2] - mon4[2]};

        const double d34rsq = d34[0]*d34[0] + d34[1]*d34[1] + d34[2]*d34[2];
        const double d34r = std::sqrt(d34rsq);

        if (true  && d12r > m_ro  && d13r > m_ro  && d14r > m_ro  && d23r > m_ro  && d24r > m_ro  && d34r > m_ro ) {
             continue;
        }

        std::copy(mon1, mon1 + 9, xyz.begin() + 0);

        std::copy(mon2, mon2 + 9, xyz.begin() + 9);

        std::copy(mon3, mon3 + 9, xyz.begin() + 18);

        std::copy(mon4, mon4 + 9, xyz.begin() + 27);


        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        const double* coords_A_2_b = xyz.data() + 9;

        const double* coords_B_3_b = xyz.data() + 12;

        const double* coords_B_4_b = xyz.data() + 15;

        const double* coords_A_3_c = xyz.data() + 18;

        const double* coords_B_5_c = xyz.data() + 21;

        const double* coords_B_6_c = xyz.data() + 24;

        const double* coords_A_4_d = xyz.data() + 27;

        const double* coords_B_7_d = xyz.data() + 30;

        const double* coords_B_8_d = xyz.data() + 33;


        double w12 =     -9.721486914088159e-02;  //from MBpol
        double w13 =     -9.721486914088159e-02;
        double wcross =   9.859272078406150e-02;

    
        variable vs[66];

        double xs[66];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_2_b);
        xs[1] = vs[1].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_3_c);
        xs[2] = vs[2].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_4_d);
        xs[3] = vs[3].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[4] = vs[4].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[5] = vs[5].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_3_b);
        xs[6] = vs[6].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_4_b);
        xs[7] = vs[7].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_5_c);
        xs[8] = vs[8].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_6_c);
        xs[9] = vs[9].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_7_d);
        xs[10] = vs[10].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_8_d);
        xs[11] = vs[11].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_2_b, coords_A_3_c);
        xs[12] = vs[12].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_2_b, coords_A_4_d);
        xs[13] = vs[13].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_1_a);
        xs[14] = vs[14].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_2_a);
        xs[15] = vs[15].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_2_b, coords_B_3_b);
        xs[16] = vs[16].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_2_b, coords_B_4_b);
        xs[17] = vs[17].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_5_c);
        xs[18] = vs[18].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_6_c);
        xs[19] = vs[19].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_7_d);
        xs[20] = vs[20].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_8_d);
        xs[21] = vs[21].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_3_c, coords_A_4_d);
        xs[22] = vs[22].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_1_a);
        xs[23] = vs[23].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_2_a);
        xs[24] = vs[24].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_3_b);
        xs[25] = vs[25].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_4_b);
        xs[26] = vs[26].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_3_c, coords_B_5_c);
        xs[27] = vs[27].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_3_c, coords_B_6_c);
        xs[28] = vs[28].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_7_d);
        xs[29] = vs[29].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_8_d);
        xs[30] = vs[30].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_1_a);
        xs[31] = vs[31].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_2_a);
        xs[32] = vs[32].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_3_b);
        xs[33] = vs[33].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_4_b);
        xs[34] = vs[34].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_5_c);
        xs[35] = vs[35].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_6_c);
        xs[36] = vs[36].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_4_d, coords_B_7_d);
        xs[37] = vs[37].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_4_d, coords_B_8_d);
        xs[38] = vs[38].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);
        xs[39] = vs[39].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_3_b);
        xs[40] = vs[40].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_4_b);
        xs[41] = vs[41].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_5_c);
        xs[42] = vs[42].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_6_c);
        xs[43] = vs[43].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_7_d);
        xs[44] = vs[44].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_8_d);
        xs[45] = vs[45].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_3_b);
        xs[46] = vs[46].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_4_b);
        xs[47] = vs[47].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_5_c);
        xs[48] = vs[48].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_6_c);
        xs[49] = vs[49].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_7_d);
        xs[50] = vs[50].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_8_d);
        xs[51] = vs[51].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_3_b, coords_B_4_b);
        xs[52] = vs[52].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_5_c);
        xs[53] = vs[53].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_6_c);
        xs[54] = vs[54].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_7_d);
        xs[55] = vs[55].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_8_d);
        xs[56] = vs[56].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_5_c);
        xs[57] = vs[57].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_6_c);
        xs[58] = vs[58].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_7_d);
        xs[59] = vs[59].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_8_d);
        xs[60] = vs[60].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_5_c, coords_B_6_c);
        xs[61] = vs[61].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_5_c, coords_B_7_d);
        xs[62] = vs[62].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_5_c, coords_B_8_d);
        xs[63] = vs[63].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_6_c, coords_B_7_d);
        xs[64] = vs[64].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_6_c, coords_B_8_d);
        xs[65] = vs[65].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_7_d, coords_B_8_d);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);
        double gsw13 = 0.0;
        double sw13 = f_switch(d13r, gsw13);
        double gsw14 = 0.0;
        double sw14 = f_switch(d14r, gsw14);
        double gsw23 = 0.0;
        double sw23 = f_switch(d23r, gsw23);
        double gsw24 = 0.0;
        double sw24 = f_switch(d24r, gsw24);
        double gsw34 = 0.0;
        double sw34 = f_switch(d34r, gsw34);

        // sw = sw12*sw13*sw14*sw23*sw24*sw34; // cutoff based on largest d-OO
        sw = 3*sw12*sw13*sw14*sw23*sw24*sw34 - sw13*sw14*sw23*sw24*sw34 - sw12*sw14*sw23*sw24*sw34 - sw12*sw13*sw23*sw24*sw34 - sw12*sw13*sw14*sw24*sw34 - sw12*sw13*sw14*sw23*sw34 - sw12*sw13*sw14*sw23*sw24 + sw12*sw13*sw14 + sw12*sw23*sw24 + sw13*sw23*sw34 + sw14*sw24*sw34; // cutoff based on center-3-neighbor criteria

        energies[j] = my_poly.eval(xs,coefficients.data());
        energies_sw[j] = energies[j]*sw;

    }

    double energy = 0.0;
    for (size_t i = 0; i < n; i++)
        energy += energies_sw[i];

    return energy;

}

//----------------------------------------------------------------------------//

double mbnrg_A1B2_A1B2_A1B2_A1B2_deg4_v1::eval(const double *xyz1, const double *xyz2, const double *xyz3, const double *xyz4, double *grad1, double *grad2, double *grad3, double *grad4 , const size_t n, std::vector<double> *virial) {
    std::vector<double> energies(n,0.0);
    std::vector<double> energies_sw(n,0.0);

    std::vector<double> xyz(36);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 9*j;
        const double *mon2 = xyz2 + 9*j;
        const double *mon3 = xyz3 + 9*j;
        const double *mon4 = xyz4 + 9*j;


        const double d12[3] =
                         {mon1[0] - mon2[0],
                          mon1[1] - mon2[1],
                          mon1[2] - mon2[2]};

        const double d12rsq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double d12r = std::sqrt(d12rsq);

        const double d13[3] =
                         {mon1[0] - mon3[0],
                          mon1[1] - mon3[1],
                          mon1[2] - mon3[2]};

        const double d13rsq = d13[0]*d13[0] + d13[1]*d13[1] + d13[2]*d13[2];
        const double d13r = std::sqrt(d13rsq);

        const double d14[3] =
                         {mon1[0] - mon4[0],
                          mon1[1] - mon4[1],
                          mon1[2] - mon4[2]};

        const double d14rsq = d14[0]*d14[0] + d14[1]*d14[1] + d14[2]*d14[2];
        const double d14r = std::sqrt(d14rsq);

        const double d23[3] =
                         {mon2[0] - mon3[0],
                          mon2[1] - mon3[1],
                          mon2[2] - mon3[2]};

        const double d23rsq = d23[0]*d23[0] + d23[1]*d23[1] + d23[2]*d23[2];
        const double d23r = std::sqrt(d23rsq);

        const double d24[3] =
                         {mon2[0] - mon4[0],
                          mon2[1] - mon4[1],
                          mon2[2] - mon4[2]};

        const double d24rsq = d24[0]*d24[0] + d24[1]*d24[1] + d24[2]*d24[2];
        const double d24r = std::sqrt(d24rsq);

        const double d34[3] =
                         {mon3[0] - mon4[0],
                          mon3[1] - mon4[1],
                          mon3[2] - mon4[2]};

        const double d34rsq = d34[0]*d34[0] + d34[1]*d34[1] + d34[2]*d34[2];
        const double d34r = std::sqrt(d34rsq);

        if (true  && d12r > m_ro  && d13r > m_ro  && d14r > m_ro  && d23r > m_ro  && d24r > m_ro  && d34r > m_ro ) {
             continue;
        }

        std::vector<double> gradients(36,0.0);

        std::copy(mon1, mon1 + 9, xyz.begin() + 0);

        std::copy(mon2, mon2 + 9, xyz.begin() + 9);

        std::copy(mon3, mon3 + 9, xyz.begin() + 18);

        std::copy(mon4, mon4 + 9, xyz.begin() + 27);
        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        const double* coords_A_2_b = xyz.data() + 9;

        const double* coords_B_3_b = xyz.data() + 12;

        const double* coords_B_4_b = xyz.data() + 15;

        const double* coords_A_3_c = xyz.data() + 18;

        const double* coords_B_5_c = xyz.data() + 21;

        const double* coords_B_6_c = xyz.data() + 24;

        const double* coords_A_4_d = xyz.data() + 27;

        const double* coords_B_7_d = xyz.data() + 30;

        const double* coords_B_8_d = xyz.data() + 33;


        double* coords_A_1_a_g = gradients.data() + 0;

        double* coords_B_1_a_g = gradients.data() + 3;

        double* coords_B_2_a_g = gradients.data() + 6;

        double* coords_A_2_b_g = gradients.data() + 9;

        double* coords_B_3_b_g = gradients.data() + 12;

        double* coords_B_4_b_g = gradients.data() + 15;

        double* coords_A_3_c_g = gradients.data() + 18;

        double* coords_B_5_c_g = gradients.data() + 21;

        double* coords_B_6_c_g = gradients.data() + 24;

        double* coords_A_4_d_g = gradients.data() + 27;

        double* coords_B_7_d_g = gradients.data() + 30;

        double* coords_B_8_d_g = gradients.data() + 33;



        double w12 =     -9.721486914088159e-02;  //from MBpol
        double w13 =     -9.721486914088159e-02;
        double wcross =   9.859272078406150e-02;

    
        variable vs[66];

        double xs[66];


        double gxs[66];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_2_b);
        xs[1] = vs[1].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_3_c);
        xs[2] = vs[2].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_4_d);
        xs[3] = vs[3].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[4] = vs[4].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[5] = vs[5].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_3_b);
        xs[6] = vs[6].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_4_b);
        xs[7] = vs[7].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_5_c);
        xs[8] = vs[8].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_6_c);
        xs[9] = vs[9].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_7_d);
        xs[10] = vs[10].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_8_d);
        xs[11] = vs[11].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_2_b, coords_A_3_c);
        xs[12] = vs[12].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_2_b, coords_A_4_d);
        xs[13] = vs[13].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_1_a);
        xs[14] = vs[14].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_2_a);
        xs[15] = vs[15].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_2_b, coords_B_3_b);
        xs[16] = vs[16].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_2_b, coords_B_4_b);
        xs[17] = vs[17].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_5_c);
        xs[18] = vs[18].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_6_c);
        xs[19] = vs[19].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_7_d);
        xs[20] = vs[20].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_2_b, coords_B_8_d);
        xs[21] = vs[21].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_3_c, coords_A_4_d);
        xs[22] = vs[22].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_1_a);
        xs[23] = vs[23].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_2_a);
        xs[24] = vs[24].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_3_b);
        xs[25] = vs[25].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_4_b);
        xs[26] = vs[26].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_3_c, coords_B_5_c);
        xs[27] = vs[27].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_3_c, coords_B_6_c);
        xs[28] = vs[28].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_7_d);
        xs[29] = vs[29].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_3_c, coords_B_8_d);
        xs[30] = vs[30].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_1_a);
        xs[31] = vs[31].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_2_a);
        xs[32] = vs[32].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_3_b);
        xs[33] = vs[33].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_4_b);
        xs[34] = vs[34].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_5_c);
        xs[35] = vs[35].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_4_d, coords_B_6_c);
        xs[36] = vs[36].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_4_d, coords_B_7_d);
        xs[37] = vs[37].v_exp0(m_d_x_intra_A_B_1, m_k_x_intra_A_B_1, coords_A_4_d, coords_B_8_d);
        xs[38] = vs[38].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);
        xs[39] = vs[39].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_3_b);
        xs[40] = vs[40].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_4_b);
        xs[41] = vs[41].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_5_c);
        xs[42] = vs[42].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_6_c);
        xs[43] = vs[43].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_7_d);
        xs[44] = vs[44].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_1_a, coords_B_8_d);
        xs[45] = vs[45].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_3_b);
        xs[46] = vs[46].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_4_b);
        xs[47] = vs[47].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_5_c);
        xs[48] = vs[48].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_6_c);
        xs[49] = vs[49].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_7_d);
        xs[50] = vs[50].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_2_a, coords_B_8_d);
        xs[51] = vs[51].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_3_b, coords_B_4_b);
        xs[52] = vs[52].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_5_c);
        xs[53] = vs[53].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_6_c);
        xs[54] = vs[54].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_7_d);
        xs[55] = vs[55].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_3_b, coords_B_8_d);
        xs[56] = vs[56].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_5_c);
        xs[57] = vs[57].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_6_c);
        xs[58] = vs[58].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_7_d);
        xs[59] = vs[59].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_4_b, coords_B_8_d);
        xs[60] = vs[60].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_5_c, coords_B_6_c);
        xs[61] = vs[61].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_5_c, coords_B_7_d);
        xs[62] = vs[62].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_5_c, coords_B_8_d);
        xs[63] = vs[63].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_6_c, coords_B_7_d);
        xs[64] = vs[64].v_exp0(m_d_x_inter_B_B_0, m_k_x_inter_B_B_0, coords_B_6_c, coords_B_8_d);
        xs[65] = vs[65].v_exp0(m_d_x_intra_B_B_1, m_k_x_intra_B_B_1, coords_B_7_d, coords_B_8_d);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);
        double gsw13 = 0.0;
        double sw13 = f_switch(d13r, gsw13);
        double gsw14 = 0.0;
        double sw14 = f_switch(d14r, gsw14);
        double gsw23 = 0.0;
        double sw23 = f_switch(d23r, gsw23);
        double gsw24 = 0.0;
        double sw24 = f_switch(d24r, gsw24);
        double gsw34 = 0.0;
        double sw34 = f_switch(d34r, gsw34);

        // sw = sw12*sw13*sw14*sw23*sw24*sw34; // cutoff based on largest d-OO
        sw = 3*sw12*sw13*sw14*sw23*sw24*sw34 - sw13*sw14*sw23*sw24*sw34 - sw12*sw14*sw23*sw24*sw34 - sw12*sw13*sw23*sw24*sw34 - sw12*sw13*sw14*sw24*sw34 - sw12*sw13*sw14*sw23*sw34 - sw12*sw13*sw14*sw23*sw24 + sw12*sw13*sw14 + sw12*sw23*sw24 + sw13*sw23*sw34 + sw14*sw24*sw34; // cutoff based on center-3-neighbor criteria

        energies[j] = my_poly.eval(xs,coefficients.data(),gxs);
        energies_sw[j] = energies[j]*sw;

        for (size_t i = 0; i < 66; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_A_2_b_g, coords_A_1_a, coords_A_2_b);
        vs[1].grads(gxs[1], coords_A_1_a_g, coords_A_3_c_g, coords_A_1_a, coords_A_3_c);
        vs[2].grads(gxs[2], coords_A_1_a_g, coords_A_4_d_g, coords_A_1_a, coords_A_4_d);
        vs[3].grads(gxs[3], coords_A_1_a_g, coords_B_1_a_g, coords_A_1_a, coords_B_1_a);
        vs[4].grads(gxs[4], coords_A_1_a_g, coords_B_2_a_g, coords_A_1_a, coords_B_2_a);
        vs[5].grads(gxs[5], coords_A_1_a_g, coords_B_3_b_g, coords_A_1_a, coords_B_3_b);
        vs[6].grads(gxs[6], coords_A_1_a_g, coords_B_4_b_g, coords_A_1_a, coords_B_4_b);
        vs[7].grads(gxs[7], coords_A_1_a_g, coords_B_5_c_g, coords_A_1_a, coords_B_5_c);
        vs[8].grads(gxs[8], coords_A_1_a_g, coords_B_6_c_g, coords_A_1_a, coords_B_6_c);
        vs[9].grads(gxs[9], coords_A_1_a_g, coords_B_7_d_g, coords_A_1_a, coords_B_7_d);
        vs[10].grads(gxs[10], coords_A_1_a_g, coords_B_8_d_g, coords_A_1_a, coords_B_8_d);
        vs[11].grads(gxs[11], coords_A_2_b_g, coords_A_3_c_g, coords_A_2_b, coords_A_3_c);
        vs[12].grads(gxs[12], coords_A_2_b_g, coords_A_4_d_g, coords_A_2_b, coords_A_4_d);
        vs[13].grads(gxs[13], coords_A_2_b_g, coords_B_1_a_g, coords_A_2_b, coords_B_1_a);
        vs[14].grads(gxs[14], coords_A_2_b_g, coords_B_2_a_g, coords_A_2_b, coords_B_2_a);
        vs[15].grads(gxs[15], coords_A_2_b_g, coords_B_3_b_g, coords_A_2_b, coords_B_3_b);
        vs[16].grads(gxs[16], coords_A_2_b_g, coords_B_4_b_g, coords_A_2_b, coords_B_4_b);
        vs[17].grads(gxs[17], coords_A_2_b_g, coords_B_5_c_g, coords_A_2_b, coords_B_5_c);
        vs[18].grads(gxs[18], coords_A_2_b_g, coords_B_6_c_g, coords_A_2_b, coords_B_6_c);
        vs[19].grads(gxs[19], coords_A_2_b_g, coords_B_7_d_g, coords_A_2_b, coords_B_7_d);
        vs[20].grads(gxs[20], coords_A_2_b_g, coords_B_8_d_g, coords_A_2_b, coords_B_8_d);
        vs[21].grads(gxs[21], coords_A_3_c_g, coords_A_4_d_g, coords_A_3_c, coords_A_4_d);
        vs[22].grads(gxs[22], coords_A_3_c_g, coords_B_1_a_g, coords_A_3_c, coords_B_1_a);
        vs[23].grads(gxs[23], coords_A_3_c_g, coords_B_2_a_g, coords_A_3_c, coords_B_2_a);
        vs[24].grads(gxs[24], coords_A_3_c_g, coords_B_3_b_g, coords_A_3_c, coords_B_3_b);
        vs[25].grads(gxs[25], coords_A_3_c_g, coords_B_4_b_g, coords_A_3_c, coords_B_4_b);
        vs[26].grads(gxs[26], coords_A_3_c_g, coords_B_5_c_g, coords_A_3_c, coords_B_5_c);
        vs[27].grads(gxs[27], coords_A_3_c_g, coords_B_6_c_g, coords_A_3_c, coords_B_6_c);
        vs[28].grads(gxs[28], coords_A_3_c_g, coords_B_7_d_g, coords_A_3_c, coords_B_7_d);
        vs[29].grads(gxs[29], coords_A_3_c_g, coords_B_8_d_g, coords_A_3_c, coords_B_8_d);
        vs[30].grads(gxs[30], coords_A_4_d_g, coords_B_1_a_g, coords_A_4_d, coords_B_1_a);
        vs[31].grads(gxs[31], coords_A_4_d_g, coords_B_2_a_g, coords_A_4_d, coords_B_2_a);
        vs[32].grads(gxs[32], coords_A_4_d_g, coords_B_3_b_g, coords_A_4_d, coords_B_3_b);
        vs[33].grads(gxs[33], coords_A_4_d_g, coords_B_4_b_g, coords_A_4_d, coords_B_4_b);
        vs[34].grads(gxs[34], coords_A_4_d_g, coords_B_5_c_g, coords_A_4_d, coords_B_5_c);
        vs[35].grads(gxs[35], coords_A_4_d_g, coords_B_6_c_g, coords_A_4_d, coords_B_6_c);
        vs[36].grads(gxs[36], coords_A_4_d_g, coords_B_7_d_g, coords_A_4_d, coords_B_7_d);
        vs[37].grads(gxs[37], coords_A_4_d_g, coords_B_8_d_g, coords_A_4_d, coords_B_8_d);
        vs[38].grads(gxs[38], coords_B_1_a_g, coords_B_2_a_g, coords_B_1_a, coords_B_2_a);
        vs[39].grads(gxs[39], coords_B_1_a_g, coords_B_3_b_g, coords_B_1_a, coords_B_3_b);
        vs[40].grads(gxs[40], coords_B_1_a_g, coords_B_4_b_g, coords_B_1_a, coords_B_4_b);
        vs[41].grads(gxs[41], coords_B_1_a_g, coords_B_5_c_g, coords_B_1_a, coords_B_5_c);
        vs[42].grads(gxs[42], coords_B_1_a_g, coords_B_6_c_g, coords_B_1_a, coords_B_6_c);
        vs[43].grads(gxs[43], coords_B_1_a_g, coords_B_7_d_g, coords_B_1_a, coords_B_7_d);
        vs[44].grads(gxs[44], coords_B_1_a_g, coords_B_8_d_g, coords_B_1_a, coords_B_8_d);
        vs[45].grads(gxs[45], coords_B_2_a_g, coords_B_3_b_g, coords_B_2_a, coords_B_3_b);
        vs[46].grads(gxs[46], coords_B_2_a_g, coords_B_4_b_g, coords_B_2_a, coords_B_4_b);
        vs[47].grads(gxs[47], coords_B_2_a_g, coords_B_5_c_g, coords_B_2_a, coords_B_5_c);
        vs[48].grads(gxs[48], coords_B_2_a_g, coords_B_6_c_g, coords_B_2_a, coords_B_6_c);
        vs[49].grads(gxs[49], coords_B_2_a_g, coords_B_7_d_g, coords_B_2_a, coords_B_7_d);
        vs[50].grads(gxs[50], coords_B_2_a_g, coords_B_8_d_g, coords_B_2_a, coords_B_8_d);
        vs[51].grads(gxs[51], coords_B_3_b_g, coords_B_4_b_g, coords_B_3_b, coords_B_4_b);
        vs[52].grads(gxs[52], coords_B_3_b_g, coords_B_5_c_g, coords_B_3_b, coords_B_5_c);
        vs[53].grads(gxs[53], coords_B_3_b_g, coords_B_6_c_g, coords_B_3_b, coords_B_6_c);
        vs[54].grads(gxs[54], coords_B_3_b_g, coords_B_7_d_g, coords_B_3_b, coords_B_7_d);
        vs[55].grads(gxs[55], coords_B_3_b_g, coords_B_8_d_g, coords_B_3_b, coords_B_8_d);
        vs[56].grads(gxs[56], coords_B_4_b_g, coords_B_5_c_g, coords_B_4_b, coords_B_5_c);
        vs[57].grads(gxs[57], coords_B_4_b_g, coords_B_6_c_g, coords_B_4_b, coords_B_6_c);
        vs[58].grads(gxs[58], coords_B_4_b_g, coords_B_7_d_g, coords_B_4_b, coords_B_7_d);
        vs[59].grads(gxs[59], coords_B_4_b_g, coords_B_8_d_g, coords_B_4_b, coords_B_8_d);
        vs[60].grads(gxs[60], coords_B_5_c_g, coords_B_6_c_g, coords_B_5_c, coords_B_6_c);
        vs[61].grads(gxs[61], coords_B_5_c_g, coords_B_7_d_g, coords_B_5_c, coords_B_7_d);
        vs[62].grads(gxs[62], coords_B_5_c_g, coords_B_8_d_g, coords_B_5_c, coords_B_8_d);
        vs[63].grads(gxs[63], coords_B_6_c_g, coords_B_7_d_g, coords_B_6_c, coords_B_7_d);
        vs[64].grads(gxs[64], coords_B_6_c_g, coords_B_8_d_g, coords_B_6_c, coords_B_8_d);
        vs[65].grads(gxs[65], coords_B_7_d_g, coords_B_8_d_g, coords_B_7_d, coords_B_8_d);

        // cutoff based on largest d-OO
        // gsw12 *= (sw13*sw14*sw23*sw24*sw34)*energies[j]/d12r;
        // gsw13 *= (sw12*sw14*sw23*sw24*sw34)*energies[j]/d13r;
        // gsw14 *= (sw12*sw13*sw23*sw24*sw34)*energies[j]/d14r;
        // gsw23 *= (sw12*sw13*sw14*sw24*sw34)*energies[j]/d23r;
        // gsw24 *= (sw12*sw13*sw14*sw23*sw34)*energies[j]/d24r;
        // gsw34 *= (sw12*sw13*sw14*sw23*sw24)*energies[j]/d34r;

        // cutoff based on center-3-neighbor criteria
        gsw12 *= (3*sw13*sw14*sw23*sw24*sw34 - sw14*sw23*sw24*sw34 - sw13*sw23*sw24*sw34 - sw13*sw14*sw24*sw34 - sw13*sw14*sw23*sw34 - sw13*sw14*sw23*sw24 + sw13*sw14 + sw23*sw24)*energies[j]/d12r;
        gsw13 *= (3*sw12*sw14*sw23*sw24*sw34 - sw14*sw23*sw24*sw34 - sw12*sw23*sw24*sw34 - sw12*sw14*sw24*sw34 - sw12*sw14*sw23*sw34 - sw12*sw14*sw23*sw24 + sw12*sw14 + sw23*sw34)*energies[j]/d13r;
        gsw14 *= (3*sw12*sw13*sw23*sw24*sw34 - sw13*sw23*sw24*sw34 - sw12*sw23*sw24*sw34 - sw12*sw13*sw24*sw34 - sw12*sw13*sw23*sw34 - sw12*sw13*sw23*sw24 + sw12*sw13 + sw24*sw34)*energies[j]/d14r;
        gsw23 *= (3*sw12*sw13*sw14*sw24*sw34 - sw13*sw14*sw24*sw34 - sw12*sw14*sw24*sw34 - sw12*sw13*sw24*sw34 - sw12*sw13*sw14*sw34 - sw12*sw13*sw14*sw24 + sw12*sw24 + sw13*sw34)*energies[j]/d23r;
        gsw24 *= (3*sw12*sw13*sw14*sw23*sw34 - sw13*sw14*sw23*sw34 - sw12*sw14*sw23*sw34 - sw12*sw13*sw23*sw34 - sw12*sw13*sw14*sw34 - sw12*sw13*sw14*sw23 + sw12*sw23 + sw14*sw34)*energies[j]/d24r;
        gsw34 *= (3*sw12*sw13*sw14*sw23*sw24 - sw13*sw14*sw23*sw24 - sw12*sw14*sw23*sw24 - sw12*sw13*sw23*sw24 - sw12*sw13*sw14*sw24 - sw12*sw13*sw14*sw23 + sw13*sw23 + sw14*sw24)*energies[j]/d34r;


        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0 + (gsw12*d12[i])+ (gsw13*d13[i])+ (gsw14*d14[i]);
            gradients[9 + i] += 0.0 - (gsw12*d12[i])+ (gsw23*d23[i])+ (gsw24*d24[i]);
            gradients[18 + i] += 0.0 - (gsw13*d13[i])- (gsw23*d23[i])+ (gsw34*d34[i]);
            gradients[27 + i] += 0.0 - (gsw14*d14[i])- (gsw24*d24[i])- (gsw34*d34[i]);
        }


        for (size_t i = 0; i < 9; i++) {
            grad1[i + j*9] += gradients[0 + i];
        }


        for (size_t i = 0; i < 9; i++) {
            grad2[i + j*9] += gradients[9 + i];
        }


        for (size_t i = 0; i < 9; i++) {
            grad3[i + j*9] += gradients[18 + i];
        }


        for (size_t i = 0; i < 9; i++) {
            grad4[i + j*9] += gradients[27 + i];
        }

        
        if (virial != 0) {
        
            (*virial)[0] += -coords_A_1_a[0]*coords_A_1_a_g[0]
                        -coords_B_1_a[0]*coords_B_1_a_g[0]
                        -coords_B_2_a[0]*coords_B_2_a_g[0]
                        -coords_A_2_b[0]*coords_A_2_b_g[0]
                        -coords_B_3_b[0]*coords_B_3_b_g[0]
                        -coords_B_4_b[0]*coords_B_4_b_g[0]
                        -coords_A_3_c[0]*coords_A_3_c_g[0]
                        -coords_B_5_c[0]*coords_B_5_c_g[0]
                        -coords_B_6_c[0]*coords_B_6_c_g[0]
                        -coords_A_4_d[0]*coords_A_4_d_g[0]
                        -coords_B_7_d[0]*coords_B_7_d_g[0]
                        -coords_B_8_d[0]*coords_B_8_d_g[0];

        
            (*virial)[1] += -coords_A_1_a[0]*coords_A_1_a_g[1]
                        -coords_B_1_a[0]*coords_B_1_a_g[1]
                        -coords_B_2_a[0]*coords_B_2_a_g[1]
                        -coords_A_2_b[0]*coords_A_2_b_g[1]
                        -coords_B_3_b[0]*coords_B_3_b_g[1]
                        -coords_B_4_b[0]*coords_B_4_b_g[1]
                        -coords_A_3_c[0]*coords_A_3_c_g[1]
                        -coords_B_5_c[0]*coords_B_5_c_g[1]
                        -coords_B_6_c[0]*coords_B_6_c_g[1]
                        -coords_A_4_d[0]*coords_A_4_d_g[1]
                        -coords_B_7_d[0]*coords_B_7_d_g[1]
                        -coords_B_8_d[0]*coords_B_8_d_g[1];

        
            (*virial)[2] += -coords_A_1_a[0]*coords_A_1_a_g[2]
                        -coords_B_1_a[0]*coords_B_1_a_g[2]
                        -coords_B_2_a[0]*coords_B_2_a_g[2]
                        -coords_A_2_b[0]*coords_A_2_b_g[2]
                        -coords_B_3_b[0]*coords_B_3_b_g[2]
                        -coords_B_4_b[0]*coords_B_4_b_g[2]
                        -coords_A_3_c[0]*coords_A_3_c_g[2]
                        -coords_B_5_c[0]*coords_B_5_c_g[2]
                        -coords_B_6_c[0]*coords_B_6_c_g[2]
                        -coords_A_4_d[0]*coords_A_4_d_g[2]
                        -coords_B_7_d[0]*coords_B_7_d_g[2]
                        -coords_B_8_d[0]*coords_B_8_d_g[2];

        
            (*virial)[4] += -coords_A_1_a[1]*coords_A_1_a_g[1]
                        -coords_B_1_a[1]*coords_B_1_a_g[1]
                        -coords_B_2_a[1]*coords_B_2_a_g[1]
                        -coords_A_2_b[1]*coords_A_2_b_g[1]
                        -coords_B_3_b[1]*coords_B_3_b_g[1]
                        -coords_B_4_b[1]*coords_B_4_b_g[1]
                        -coords_A_3_c[1]*coords_A_3_c_g[1]
                        -coords_B_5_c[1]*coords_B_5_c_g[1]
                        -coords_B_6_c[1]*coords_B_6_c_g[1]
                        -coords_A_4_d[1]*coords_A_4_d_g[1]
                        -coords_B_7_d[1]*coords_B_7_d_g[1]
                        -coords_B_8_d[1]*coords_B_8_d_g[1];

        
            (*virial)[5] += -coords_A_1_a[1]*coords_A_1_a_g[2]
                        -coords_B_1_a[1]*coords_B_1_a_g[2]
                        -coords_B_2_a[1]*coords_B_2_a_g[2]
                        -coords_A_2_b[1]*coords_A_2_b_g[2]
                        -coords_B_3_b[1]*coords_B_3_b_g[2]
                        -coords_B_4_b[1]*coords_B_4_b_g[2]
                        -coords_A_3_c[1]*coords_A_3_c_g[2]
                        -coords_B_5_c[1]*coords_B_5_c_g[2]
                        -coords_B_6_c[1]*coords_B_6_c_g[2]
                        -coords_A_4_d[1]*coords_A_4_d_g[2]
                        -coords_B_7_d[1]*coords_B_7_d_g[2]
                        -coords_B_8_d[1]*coords_B_8_d_g[2];

        
            (*virial)[8] += -coords_A_1_a[2]*coords_A_1_a_g[2]
                        -coords_B_1_a[2]*coords_B_1_a_g[2]
                        -coords_B_2_a[2]*coords_B_2_a_g[2]
                        -coords_A_2_b[2]*coords_A_2_b_g[2]
                        -coords_B_3_b[2]*coords_B_3_b_g[2]
                        -coords_B_4_b[2]*coords_B_4_b_g[2]
                        -coords_A_3_c[2]*coords_A_3_c_g[2]
                        -coords_B_5_c[2]*coords_B_5_c_g[2]
                        -coords_B_6_c[2]*coords_B_6_c_g[2]
                        -coords_A_4_d[2]*coords_A_4_d_g[2]
                        -coords_B_7_d[2]*coords_B_7_d_g[2]
                        -coords_B_8_d[2]*coords_B_8_d_g[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];


        }


    }

    double energy = 0.0;
    for (size_t i = 0; i < n; i++)
        energy += energies_sw[i];

    return energy;

}

//----------------------------------------------------------------------------//
} // namespace mbnrg_A1B2_A1B2_A1B2_A1B2_deg4
