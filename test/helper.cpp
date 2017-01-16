/**
 * Copyright (c) 2017 Zefiros Software.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "helper.h"

uint32_t g_seed = 0;

template<>
float GetRandom()
{
    // return with max an arbitrary number
    return ( float )GetFastRand() / ( float )( std::numeric_limits< uint32_t >::max() / ( 1e-8 / 3.0f ) );
}

template<>
double GetRandom()
{
    // return with max an arbitrary number
    return ( double )GetFastRand() / ( float )( std::numeric_limits< uint32_t >::max() / ( 1e-16 / 3.0f ) );
}

std::string GenerateRandomString()
{
    // Imagine the boredom it took to write this :)
    // Sometimes I wonder whether this is the real life...
    static const char *lyrics[58] =
    {
        "Is this the real life?",
        "Is this just fantasy?",
        "Caught in a landslide,",
        "No escape from reality.",

        "Open your eyes,",
        "Look up to the skies and see,",
        "I'm just a poor boy, I need no sympathy,",
        "Because I'm easy come, easy go,",
        "Little high, little low,",
        "Anyway the wind blows doesn't really matter to me, to me.",

        "Mama, just killed a man,",
        "Put a gun against his head,",
        "Pulled my trigger, now he's dead.",
        "Mama, life had just begun,",
        "But now I've gone and thrown it all away.",

        "Mama, ooh,",
        "Didn't mean to make you cry,",
        "If I'm not back again this time tomorrow,",
        "Carry on, carry on as if nothing really matters.",

        "Too late, my time has come,",
        "Sent shivers down my spine,",
        "Body's aching all the time.",
        "Goodbye, everybody, I've got to go,",
        "Gotta leave you all behind and face the truth.",

        "Mama, ooh (anyway the wind blows),",
        "I don't wanna die,",
        "I sometimes wish I'd never been born at all.",

        "I see a little silhouetto of a man,",
        "Scaramouche, Scaramouche, will you do the Fandango?",
        "Thunderbolt and lightning,",
        "Very, very frightening me.",
        "(Galileo) Galileo.",
        "(Galileo) Galileo,",
        "Galileo Figaro",
        "Magnifico.",

        "I'm just a poor boy, nobody loves me.",
        "He's just a poor boy from a poor family,",
        "Spare him his life from this monstrosity.",

        "Easy come, easy go, will you let me go?",
        "Bismillah! No, we will not let you go. (Let him go!)",
        "Bismillah! We will not let you go. (Let him go!)",
        "Bismillah! We will not let you go. (Let me go!)",
        "Will not let you go. (Let me go!)",
        "Never, never let you go",
        "Never let me go, oh.",
        "No, no, no, no, no, no, no.",
        "Oh, mama mia, mama mia (Mama mia, let me go.)",
        "Beelzebub has a devil put aside for me, for me, for me.",

        "So you think you can stone me and spit in my eye?",
        "So you think you can love me and leave me to die?",
        "Oh, baby, can't do this to me, baby,",
        "Just gotta get out, just gotta get right outta here.",

        "(Oh, yeah, oh yeah)",

        "Nothing really matters,",
        "Anyone can see,",
        "Nothing really matters,",
        "Nothing really matters to me.",

        "Anyway the wind blows."
    };

    return lyrics[GetRandom< uint16_t >() % 57];
}
