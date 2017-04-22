#include "5x7_font.h"


const u8 font_data[] = {0x00,                                   //first unusable char
                        0x00,                                   //space char    1
                        0x1D,                                   //!             2           
                        0x10,0x10,                              //"             3
                        0x00,                                   //#             5
                        0x00,                                   //$             6
                        0x18,0x19,0x02,0x04,0x08,0x13,03,       //%             7
                        0x00,                                   //&             14
                        0x18,                                   //'             15
                        0x0E,0x11,                              //(             16
                        0x11,0x0E,                              //)             18
                        0x14,0x08,0x14,                         //*             20
                        0x04,0x0E,0x04,                         //+             23
                        0x01,0x02,                              //,             26
                        0x04,0x04,0x04,                         //-             28
                        0x01,                                   //.             31
                        0x01,0x02,0x04,0x08,0x10,               // /            32
                        0x1F,0x11,0x1F,                         //0             37
                        0x11,0x1F,0x01,                         //1             40
                        0x17,0x15,0x1D,                         //2             43
                        0x15,0x15,0x1F,                         //3             46
                        0x1C,0x04,0x1F,                         //4             49
                        0x1D,0x15,0x17,                         //5             52
                        0x1F,0x15,0x17,                         //6             55
                        0x10,0x10,0x1F,                         //7             58
                        0x1F,0x15,0x1F,                         //8             61
                        0x1D,0x15,0x1F,                         //9             64
                        0x0A,                                   //:             67
                        0x01,0x0A,                              //;             68
                        0x04,0x0A,0x11,                         //<             70
                        0x0A,0x0A,0x0A,                         //=             73
                        0x11,0x0A,0x04,                         //>             76
                        0x10,0x15,0x18,                         //?             79
                        0x12,0x15,0x0E,                         //@             82
                        0x0F,0x14,0x0F,                         //A             85
                        0x1F,0x15,0x0A,                         //B             88
                        0x0E,0x11,0x11,                         //C             91
                        0x1F,0x11,0x0E,                         //D             94
                        0x1F,0x15,0x11,                         //E             97
                        0x1F,0x14,0x10,                         //F             100
                        0x0E,0x11,0x15,0x16,                    //G             103
                        0x1F,0x04,0x1F,                         //H             107
                        0x11,0x1F,0x11,                         //I             110
                        0x12,0x11,0x1E,                         //J             113
                        0x1F,0x04,0x0A,0x11,                    //K             116
                        0x1F,0x01,0x01,                         //L             120
                        0x1F,0x08,0x04,0x08,0x1F,               //M             123
                        0x1F,0x08,0x04,0x02,0x1F,               //N             128
                        0x0E,0x11,0x0E,                         //O             133
                        0x1F,0x14,0x08,                         //P             136
                        0x0C,0x12,0x0D,                         //Q             139
                        0x1F,0x14,0x0B,                         //R             142
                        0x09,0x15,0x12,                         //S             145
                        0x10,0x1F,0x10,                         //T             148
                        0x1E,0x01,0x01,0x1E,                    //U             151
                        0x1E,0x01,0x1E,                         //V             155
                        0x1E,0x01,0x0E,0x01,0x1E,               //W             158
                        0x1B,0x04,0x1B,                         //X             163
                        0x18,0x07,0x18,                         //Y             166
                        0x13,0x15,0x19,                         //Z             169
                        0x1F,0x11,                              //[             172
                        0x10,0x08,0x04,0x02,0x01,               //\             174
                        0x11,0x1F,                              //]             179
                        0x08,0x10,0x08,                         //^             181
                        0x01,0x01,0x01,                         //_             184
                        0x18,                                   //'             187
                        0x11,0x0E,0x04,                         //}             188
                        0x1F,                                   //|             191
                        0x04,0x0E,0x11,                         //{             192
                        0x02,0x04,0x02,0x04                     //~             195
                                                                //199
};

const u8 font_pos[] = { 1,      
                        2,      
                        3,      
                        5,      
                        6,      
                        7,      
                        14,     
                        15,     
                        16,
                        18,
                        20,
                        23,
                        26,
                        28,
                        31,
                        32,
                        37,     // 0
                        40,     // 1
                        43,     // 2
                        46,     // 3
                        49,     // 4
                        52,     // 5
                        55,     // 6
                        58,     // 7
                        61,     // 8
                        64,     // 9     
                        67,     // :
                        68,     // ;
                        70,     //<
                        73,     //=
                        76,     //>
                        79,     //?
                        82,     //@
                        85,     //A
                        88,     //B
                        91,     //C
                        94,     //D
                        97,     //E
                        100,    //F
                        103,    //G
                        107,    //H
                        110,    //I
                        113,    //J
                        116,    //K
                        120,    //L
                        123,    //M
                        128,    //N
                        133,    //O
                        136,    //P
                        139,    //Q
                        142,    //R
                        145,    //S
                        148,    //T
                        151,    //U
                        155,    //V
                        158,    //W
                        163,    //X
                        166,    //Y
                        169,    //Z
                        172,    //[
                        174,    // back-slash
                        179,    //]
                        181,    //^
                        184,    //_
                        187,    //'
                        188,    //}
                        191,    //|
                        192,    //{
                        195,    //~
                        199
};      

const u8 font_len = ARRAY_SIZE(font_pos);