/*
 * setup.h
 *
 *  Created on: 28 févr. 2020
 *      Author: WilliamR
 */

#ifndef SETUP_H_
#define SETUP_H_


// Desired MCLK frequency

#define MCLK_FREQ_KHZ 20000

// On board crystals frequencies (in Hz)

#define XT1_FREQ 32768
#define XT2_FREQ 4000000

#define XT1_KHZ XT1_FREQ/1000
#define XT2_KHZ XT2_FREQ/1000

#define MCLK_FLLREF_RATIO MCLK_FREQ_KHZ/(XT2_KHZ/4)


void initGlobal();
void launchInitSequence();
void SetVCoreUp (unsigned int level);
void setupDCO(void);



#endif /* SETUP_H_ */
