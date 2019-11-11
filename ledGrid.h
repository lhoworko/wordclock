#ifndef WORD_H
#define WORD_H

// Each WORD is defined as an array of {num, x, y, len, ...} where top left is (0, 0).
// num is the number of distinct words in the WORD. See TWENTYFIVE.

int IT[] {1, 0, 0, 2};
int IS[] {1, 3, 0, 2};
int MINUTES[] {1, 5, 2, 7};
int PAST[] {1, 0, 3, 4};
int TO[] {1, 4, 3, 2};
int OCLOCK[] {1, 7, 7, 6};

// Minutes
int FIVE[] {1, 0, 2, 4};
int TEN[] {1, 6, 0, 3};
int QUARTER[] {1, 0, 1, 6};
int TWENTY[] {1, 7, 1, 6};
int TWENTYFIVE[] {2, 7, 1, 6, 0, 2, 4};
int HALF[] {1, 9, 0, 4};

// HOURS
int ONE[] {1, 7, 3, 3};
int TWO[] {1, 10, 3, 3};
int THREE[] {1, 0, 4, 5};
int FOUR[] {1, 5, 4, 4};
int FIVE[] {1, 9, 4, 4};
int SIX[] {1, 0, 5, 3};
int SEVEN[] {1, 3, 5, 5};
int EIGHT[] {1, 8, 5, 5};
int NINE[] {1, 0, 6, 4};
int TEN[] {1, 4, 6, 3};
int ELEVEN[] {1, 7, 6, 6};
int TWELVE[] {1, 0, 7, 6};
#endif
